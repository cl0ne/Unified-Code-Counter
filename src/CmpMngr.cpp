//! Compare manager class methods.
/*!
 * \file CmpMngr.h
 *
 * This file contains the compare manager class methods.
 * This class implements the GDiff07 algorithm, developed by Harley Green for
 * The University of Southern California Center for Systems and Software Engineering (USC-CSSE),
 * in conjunction with The Aerospace Corporation.
 * The GDiff07 algorithm breaks the counting process for finding differences between two revisions of a file
 * into several steps. First it counts and removes all unmodified lines during this process lines that are
 * not matched are put into a separate list for each of the two files.
 * These lists are then searched for swapped lines (DISABLED)
 * and have those removed. Then the remaining lines are searched again for modified matches. Once the unmodified, swapped
 * and modified lines are removed the only remaining lines will be either added or deleted so no further action is necessary
 * and the algorithm is finished.
 * Uses function for finding a similar line provided by J. Kim 10/09/2006 as part of the USC-CSSE Development process.
 */

#include "CmpMngr.h"
#include "CUtil.h"

#include <iostream>
#include <cstring>

using namespace std;

const string CmpMngr::MARKER_TABLE[] = {"MODIFIED", "UNMODIFIED", "DELETED", "ADDED"};

CmpMngr::CmpMngr()
{
	file_dump_stream = NULL;	// Modification: 2015.12
}

CmpMngr::CmpMngr(ofstream *file_stream) // Modification: 2014.08
{
    file_dump_stream = file_stream;
}


 /*!
 * 1. Function Description:
 *    Calls routines to get the final count of the differences between two files.
 *
 * 2. Parameters:
 *    baseFileMap: list of base files
 *    compFileMap: list of comparison files
 *    match_threshold: % threshold for matching, if greater then added/deleted instead of modified
 *
 * 3. Creatation Time: 
 *    Version 2007.07
 *    Revised 2015.12  Changed to support single object instance (per thread) with long duration life
 *    Revised 2016.12  Changed to return the diff details (now this function doesn't write files directly)
 */
string CmpMngr::Compare(srcLineVector* baseFileMap, srcLineVector* compFileMap, const double match_threshold)
{
	// Set default values
    MATCH_THRESHOLD = match_threshold;
    nNochangedLines = nChangedLines = nAddedLines = nDeletedLines = 0;
	base_file.resize( 0 );
	comp_file.resize( 0 );
	mapping_list.resize( 0 );
    
    // First, fill out the base file vector and comp file vector.
    fill_source_code_vec(baseFileMap, base_file);
    fill_source_code_vec(compFileMap, comp_file);
    
    mapping_list.clear(); // Modification: 2014.08
    
    FindUnmodifiedLines(baseFileMap, compFileMap);
    FindModifiedLines(baseFileMap, compFileMap);
    
    // remaining lines are added or deleted, no further details needed to be searched.
    for (srcLineVector::iterator iter = baseFileMap->begin(); iter != baseFileMap->end(); ++iter)
    {
        for (srcLineVector::mapped_type::iterator it = iter->second.begin();
             it != iter->second.end();
             ++ it) {
            mapping_list.push_back(mapping_code(*it, 0, DELETED));
        }
        
        nDeletedLines += iter->second.size();
    }
    
    for (srcLineVector::iterator iter = compFileMap->begin(); iter != compFileMap->end(); ++iter)
    {
        for (srcLineVector::mapped_type::iterator it = iter->second.begin();
             it != iter->second.end();
             ++ it) {
            mapping_list.push_back(mapping_code(0, *it, ADDED));
        }
        
        nAddedLines += iter->second.size();
    }
	/*  Bug Fix: Also fix the warning
        Description about the bug:
        UCC crashes when running Diff on two directories - used to work in 2014.08 version
        
        The cause of the bug:
        redundant declaration of ofstream *file_dump_stream in line 76 of CmpMngr.cpp.
		2015.12  Or cause may be not initializing file_dump_stream to NULL in CmpMngr constructor.

        The approach to fix the bug:
        comment or erase the line of code "ofstream *file_dump_stream;", in line 76 of CmpMngr.cpp.
    */
     //ofstream *file_dump_stream; 
	return print_info();
/*  
	// Check print_info Precondition
	if ( NULL != file_dump_stream )		// Avoid call if output not possible.  Modification: 2015.12
		print_info(file_dump_stream); // Modification 2014.08
		*/
}


 /*!
 * 1. Function Description:
 *    Finds the number of modified lines, anything remaining in lists is either deleted or added.
 *
 * 2. Parameters:
 *    aHm: list of base files
 *    bHm: list of comparison files
 *
 * 3. Creation Time:
 *    Version 2007.07
 *    Revised 2015.12  Allocates/manages 2 arrays of int types for faster SimilarLine Performance
 */
void CmpMngr::FindModifiedLines(srcLineVector* aHm, srcLineVector* bHm)
{
    srcLineVector* listA(NULL);
    srcLineVector* listB(NULL);
    stringSizeMap mySSList;
    
    size_t minSize(0);
    size_t maxSize(0);
    size_t maxSizeListA(0);
    
    bool deletedB, deletedA;
    
    if (aHm->size() == 0 || bHm->size() == 0)
        return;
    
    bool base_larger_than_comp(true);
    
    // pick the shortest list to do the sorting on
    if (aHm->size() > bHm->size()) // Modification: 2014.08s
    {
        listA = bHm;
        listB = aHm;
        base_larger_than_comp = true;
    }
    else
    {
        listA = aHm;
        listB = bHm;
        base_larger_than_comp = false;
    }
    
	// Allocate 2 int arrays for SimilarLine to use.  Much faster than std vector.    Modification: 2015.12
	unsigned int	my_line_size = 0;
	unsigned int	x_array_count = 256;
	int * x1 = (int *)calloc( x_array_count, sizeof( int ) );
	int * x2 = (int *)calloc( x_array_count, sizeof( int ) );

	if ( ( NULL == x1 ) || ( NULL == x2 ) )
	{
		// Unable to allocate memory to use
		cout << endl << "ERROR: FindModifiedLines unable to Allocate memory.  Returning early...\n" << endl;
        if ( NULL != x1 ) {
            free( x1 );
            x1 = NULL;
        }
        if ( NULL != x2 ) {
            free( x2 );
            x2 = NULL;
        }

		return;
	}    // Modification: 2015.12

    // create a map of string size -> [iterators]
    // quickly access the locations of lines that are in valid size range of modified line for the given search line
    for (srcLineVector::iterator myI = listA->begin(); myI != listA->end(); ++myI) // Modification 2014.08
    {
        
        // populate with the number of copies of this string
        stringList myList(myI->second.size(), myI->first);
        
        stringSizeMap::iterator result_iter = mySSList.find(myI->first.size());
        
        if (result_iter != mySSList.end())
        {
            // already had a copy of something this size
            result_iter->second.insert(result_iter->second.begin(), myList.begin(), myList.end());	//append the list
        }
        else
        {
            mySSList[myI->first.size()] = myList;
        }
        
        if (maxSizeListA < (*myI).first.size())
        {
            maxSizeListA = (*myI).first.size();
        }
    }
    
    // iterate through the longer list and searches for modified lines in the shorter list
    // find the min and max size range for modified lines and only looks through those specific locations
    // since the map is sorted, lookup using find is log(n) to get the iterator
    
    // once an entry is found of a list of lines that have the same size in valid modified range, checks each to see if it is modified
    for (srcLineVector::iterator myI = listB->begin(); myI != listB->end();)
    {
        // find the valid size range (dictated by the MATCH_TRESHOLD)
        minSize = static_cast<size_t>((*myI).first.length() * (MATCH_THRESHOLD / 100));
        maxSize = MATCH_THRESHOLD != 0 ? static_cast<size_t>((*myI).first.length() / (MATCH_THRESHOLD / 100)) : maxSizeListA;
        if (maxSize > maxSizeListA)
        {
            maxSize = maxSizeListA;
        }
        
        deletedB = false;
        deletedA = false;
        
        for (size_t i = minSize; i <= maxSize; ++i)
        {
            // find the list of strings of this length
            stringSizeMap::iterator mySSI = mySSList.find(i);
            if (mySSI != mySSList.end())
            {
                // found some
                for (stringList::iterator mySLI = mySSI->second.begin(); mySLI != mySSI->second.end();)
                {
					// Make sure int arrays have enough elements to match the first arg    Modified: 2015.12
					my_line_size = (*mySLI).size();
					if ( my_line_size >= x_array_count -1 )
					{
						x_array_count = ( ( my_line_size / 16 ) * 16 ) + 32;
						x1 = (int *)realloc( x1, x_array_count * sizeof( int ) );
						x2 = (int *)realloc( x2, x_array_count * sizeof( int ) );
						if ( ( NULL == x1 ) || ( NULL == x2 ) )
						{
							// Unable to increase memory to use
							cout << endl << "ERROR: FindModifiedLines unable to Reallocate memory.  Returning early...\n" << endl;
							// Better to release in reverse order of allocation
                            if ( NULL != x2 ) {
                                free( x2 );
                                x2 = NULL;
                            }
                            if ( NULL != x1 ) {
                                free( x1 );
                                x1 = NULL;
                            }
							return;
						}
						// memset or anything else will be done within SimilarLine
					}   // Modified: 2015.12

                    if ( SimilarLine( (*mySLI), x1, (*myI).first, x2 ) )    // Modified: 2015.12
                    {
                        string prev(*mySLI);
                        srcLineVector::iterator listAi = listA->find(prev);
                        
                        vector<size_t> *base_ptr(NULL);
                        vector<size_t> *comp_ptr(NULL);
                        
                        if (base_larger_than_comp)
                        {
                            comp_ptr = &listAi->second;
                            base_ptr = &myI->second;
                        }
                        else
                        {
                            base_ptr = &listAi->second;
                            comp_ptr = &myI->second;
                        }
                        
                        // eliminates the need to call SimilarLine function on every entry if they are the same
                        while (myI->second.size() > 0 && (mySLI != mySSI->second.end() && (*mySLI) == prev))
                        {
                            vector<size_t>::const_iterator base_iter = base_ptr->end() - 1;
                            vector<size_t>::const_iterator comp_iter = comp_ptr->end() - 1;
                            mapping_list.push_back(mapping_code(*base_iter, *comp_iter, MODIFIED));
                            base_ptr->pop_back();
                            comp_ptr->pop_back();
                            
                            stringList::iterator tmpSLI = mySLI++;
                            mySSI->second.erase(tmpSLI);
                            deletedA = true;
                            nChangedLines+=1;

                        }
                        
                        if (myI->second.size() == 0)
                        {
                            srcLineVector::iterator tmpI = myI++;
                            listB->erase(tmpI);
                            deletedB = true;
                        }
                        
                        if (listAi->second.size() == 0)
                        {
                            listA->erase(listAi);
                        }
                        
                        if (deletedB || deletedA)
                        {
                            break;
                        }
                    }
                    else
                    {
                        ++mySLI;
                    }
                }
                
                if (deletedB || deletedA)
                {
                    break;
                }
            }
        }
        
        if (!deletedB)
        {
            ++myI;
        }
    }

	// Better to release in reverse order of allocation
    if(x2) {
       free( x2 );
    }
    if (x1) {
       free( x1 );
    }

}


 /*!
 * 1. Function Description:
 *    Performs a binary search to find unmodified lines, relies on fact that they are in
 *    STL hash (srcLineVector) so they are already sorted and the find function is log(n)
 *
 * 2. Parameters:
 *    aHm: list of base files
 *    bHm: list of comparison files
 *
 * 3. Creation Time:
 *    Version 2007.07
 */
void CmpMngr::FindUnmodifiedLines(srcLineVector* aHm, srcLineVector* bHm)
{
    srcLineVector* listA;
    srcLineVector* listB;
    bool base_larger_than_comp(true);
    
    if (aHm->size() > bHm->size())
    {
        listA = bHm;
        listB = aHm;
        base_larger_than_comp = true; // Modification: 2014.08
    }
    else
    {
        listA = aHm;
        listB = bHm;
        base_larger_than_comp = false; // Modification: 2014.08
    }
    
    for (srcLineVector::iterator myI = listA->begin(); myI != listA->end();)
    {
        bool deleted = false;
        srcLineVector::iterator result = listB->find((*myI).first);
        if (result != listB->end())
        {
            vector<size_t> *base_ptr(NULL);
            vector<size_t> *comp_ptr(NULL);
            
            if (base_larger_than_comp)
            {
                comp_ptr = &myI->second;
                base_ptr = &result->second;
            }
            else
            {
                base_ptr = &myI->second;
                comp_ptr = &result->second;
            }
            
            while (base_ptr->size() > 0 && comp_ptr->size() > 0) // Modification: 2014.08
            {

                vector<size_t>::const_iterator base_iter = base_ptr->end() - 1;
                vector<size_t>::const_iterator comp_iter = comp_ptr->end() - 1;
                mapping_list.push_back(mapping_code(*base_iter, *comp_iter, UNMODIFIED));
                base_ptr->pop_back();
                comp_ptr->pop_back();
                nNochangedLines+=1;
            }
            
            if ((*myI).second.size() == 0)
            {
                srcLineVector::iterator tmpI = myI++;
                listA->erase(tmpI);
                deleted = true;
            }
            
            if ((*result).second.size() == 0)
            {
                listB->erase(result);
            }
        }
        
        if (!deleted)
        {
            ++myI;
        }
    }
}

 /*!
 * 1. Function Description:
 *    Determines whether two lines are similar.
 *    Originally written by J. Kim 10/09/2006
 *    Revised version of the original method
 *    - order of characters taken into account
 *    - criteria for determining 'similar lines' :
 *      length(LCS) / length(baseLine) * 100 >= MATCH_THRESHOLD &&
 *      length(LCS) / length(compLine) * 100 >= MATCH_THRESHOLD
 *    - above criteria solves two problems encountered in the original function
 *      <1>
 *      similar_lines("ABC", "ABCDEF") = 1
 *      similar_lines("ABCDEF", "ABC") = 0
 *      => because of this, if we modify a line from the original file, sometimes
 *      we get 1 MODIFIED and sometimes get 1 ADDED + 1 DELETED.
 *      <2>
 *      "ABCDEF" and "FDBACE" classified as similar lines (order not taken into account)
 *      => because of this, when we delete a certain line from the original
 *      file and insert a new one, instead of getting 1 ADDED and 1 DELETED, we might get 1 MODIFIED.
 *      (ex)
 *      DELETE: out = "test sentence";
 *      INSERT: cout << "censstetetne";
 *      and get 1 MODIFIED
 *      instead of getting 1 ADDED + 1 DELETED
 *      Return whether two lines are similar
 *
 *  Precondition:		x1 and x2 are large enough: >= (baseLine size + 1) * sizeof(int)
 *
 * 2. Parameters:
 *    baseLine:     base line
 *    x1:           int array whose size is known to be big enough for comparisons
 *    compareLine:  comparison line
 *    x2:           int array whose size is known to be big enough for comparisons
 *
 * 3. Creatation Time: 
 *	  Version 2007.07
 *    Revised 2015.12  Refactored for faster Performance.  Now uses 2 int arrays for intermediate compare values.
 */
bool CmpMngr::SimilarLine( const string &baseLine, int *x1, const string &compareLine, int *x2 )    // Modified: 2015.12
{
    int m, n, i, j;       // Modified: 2015.12
    double LCSlen;
	char	cmp_j = 0;    // Modified: 2015.12

    m = (int)baseLine.size();
    n = (int)compareLine.size();

	memset( x1, 0, (m + 1) * sizeof( int ) );    // Modified: 2015.12
	// No need to Zero set x2 as it gets copied over from x1 below
    
    // compute length of LCS
    // - no need to use CBitMatrix
    for (j = n - 1; j >= 0; j--)
    {
		memcpy( x2, x1, ( m + 1 ) * sizeof( int ) );    // Modified: 2015.12
		memset( x1,  0, ( m + 1 ) * sizeof( int ) );    // Modified: 2015.12

		cmp_j = compareLine[ (unsigned int)j ];		// Simplify invariant expressions in loops  Modified: 2015.12

        for (i = m - 1; i >= 0; i--)
        {
            if ( baseLine[ (unsigned int)i ] == cmp_j )    // Modified: 2015.12
            {
                x1[i] = 1 + x2[i+1];
            }
            else
            {
                if (x1[i+1] > x2[i])
                {
                    x1[i] = x1[i+1];
                }
                else
                {
                    x1[i] = x2[i];
                }
            }
        }
    }
    LCSlen = x1[0];
    if ((LCSlen / (double)m * 100 >= MATCH_THRESHOLD) &&
        (LCSlen / (double)n * 100 >= MATCH_THRESHOLD))
        return true;
    else 
        return false;
}

/*!
 * 1. Function Description:
 *    Store the logical lines of a file from file_map into source_code_vec
 *
 * 2. Parameters:
 *    file_map: the map containing all logical lines in a file
 *    source_code_vec: the vector to store logical lines of a file
 *
 * 3. Creation Time:
 *    Version 2014.08
 */
void CmpMngr::fill_source_code_vec(const srcLineVector *file_map, vector<string> &source_code_vec)
{
    size_t file_size(0); // This variable records the size of the file. It should equal to the maximum line number in file map.
    vector<code_line_pair> temp;
    for (srcLineVector::const_iterator iter = file_map->begin(); iter != file_map->end(); ++iter)
    {
        const vector<size_t> &line_num_list(iter->second);
        for (vector<size_t>::const_iterator it = line_num_list.begin(); it != line_num_list.end(); ++it)
        {
            temp.push_back(make_pair(&iter->first, *it));
        }
    }
    
    sort(temp.begin(), temp.end(), compare_code_line_pair);
    
    source_code_vec.reserve(file_size);
    
    for (vector<code_line_pair>::const_iterator iter = temp.begin(); iter != temp.end(); ++iter)
    {
        source_code_vec.push_back(*(iter->first));
    }
}

/*!
 * 1. Function Description:
 *    If the parameter file_dump_stream is not null, prepare print info about line numbers of base files and compare files into file_dump_stream
 *    Revised 2016.12 Now this function doesn't write files direclty, it returns the diff details instead.
 * Precondition: file_dump_stream is NOT NULL
 *
 * 2. Parameters:
 *    file_dump_stream: pointer to the stream to store print info. This parameter is removed.  2016.10
 *
 * 3. Creation Time: 
 *    Version 2014.08
 *    Revised 2015.12  Better interface comments
 *    Revised 2016.12  Remove file_dump_stream from function parameter list and now function returns diff details
 */
string CmpMngr::print_info(string * pDiff_details_str ) const
{
	// Still check for Precondition because: It is easy and very lightweight, this is good defensive programming.
	string		diffDetails;
	string		numBuf;
	for ( vector<mapping_code>::const_iterator iter = mapping_list.begin(); iter != mapping_list.end(); ++iter )
	{
		//Remove unmodified lines from visual diff log -- 2016.10
		if (MARKER_TABLE[iter->marker] == "UNMODIFIED") continue;

		LongToStr( (long)(iter->base_file_line_num), numBuf );
		diffDetails += numBuf + "\t";

		LongToStr( (long)(iter->comp_file_line_num), numBuf );
		diffDetails += numBuf + "\t" + MARKER_TABLE[iter->marker] + "\n";
	}

	if ( pDiff_details_str )
		(*pDiff_details_str) += diffDetails;

	return diffDetails;
}

/*!
 * 1. Function Description:
 *    Compare two lines' size
 *    Return whether these two lines have the same size
 *
 * 2. Parameters:
 *    lhs: one line of base files
 *    rhs: one line of comparison files
 *
 * 3. Creation Time: 2014.08
 */
bool compare_code_line_pair(const code_line_pair &lhs, const code_line_pair &rhs)
{
    return lhs.second < rhs.second;
}

// Default constructor
mapping_code::mapping_code()
{
	base_file_line_num = 0;
	comp_file_line_num = 0;
	marker = UNMODIFIED;
}

// Constructor
mapping_code::mapping_code(const size_t base_file, const size_t comp_file, const MARKER mk) :
base_file_line_num(base_file), comp_file_line_num(comp_file), marker(mk)
{
}

// Constructor
mapping_code::mapping_code(const mapping_code &rhs) : base_file_line_num(rhs.base_file_line_num),
comp_file_line_num(rhs.comp_file_line_num), marker(rhs.marker)
{
}

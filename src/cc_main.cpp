//! Main class methods containing results.
/*!
* \file cc_main.cpp
*
* This file contains the main class methods containing results.
*/

#include "cc_main.h"
#include "CUtil.h"


// Statistics about Baseline A and B
// Here so any module can access
string				commonPathPrefixA = "";
unsigned long		numFilesInA       = 0L;
unsigned long long	totalFileSizesA   = 0L;
unsigned long long	largestFileSizeA  = 0L;

string				commonPathPrefixB = "";
unsigned long		numFilesInB       = 0L;
unsigned long long	totalFileSizesB   = 0L;
unsigned long long	largestFileSizeB  = 0L;

// Used to speed up Diff directory Path comparisons
string				commonPathPrefixBoth = "";


// Clean up Baseline stats to allow multiple uses of UCC (Qt style)
void ClearBaselineStats()
{
	commonPathPrefixA = "";
	numFilesInA       = 0L;
	totalFileSizesA   = 0L;
	largestFileSizeA  = 0L;

	commonPathPrefixB = "";
	numFilesInB       = 0L;
	totalFileSizesB   = 0L;
	largestFileSizeB  = 0L;

	commonPathPrefixBoth = "";
}

/*!
* Copies a results object.
*
* \param obj results to copy
*
* \return new results object
*/
results& results::operator= (const results& obj)
{
	error_code = obj.error_code;
	blank_lines = obj.blank_lines;
	comment_lines = obj.comment_lines;
	e_comm_lines = obj.e_comm_lines;
	directive_lines[0] = obj.directive_lines[0];
	directive_lines[1] = obj.directive_lines[1];
	data_lines[0] = obj.data_lines[0];
	data_lines[1] = obj.data_lines[1];
	exec_lines[0] = obj.exec_lines[0];
	exec_lines[1] = obj.exec_lines[1];
	SLOC_lines[0] = obj.SLOC_lines[0];
	SLOC_lines[1] = obj.SLOC_lines[1];
	cmplx_math_lines = obj.cmplx_math_lines;
	cmplx_trig_lines = obj.cmplx_trig_lines;
	cmplx_logarithm_lines = obj.cmplx_logarithm_lines;
	cmplx_calc_lines = obj.cmplx_calc_lines;
	cmplx_cond_lines = obj.cmplx_cond_lines;
	cmplx_logic_lines = obj.cmplx_logic_lines;
	cmplx_preproc_lines = obj.cmplx_preproc_lines;
	cmplx_assign_lines = obj.cmplx_assign_lines;
	cmplx_pointer_lines = obj.cmplx_pointer_lines;
	directive_count.assign(obj.directive_count.begin(), obj.directive_count.end());
	data_name_count.assign(obj.data_name_count.begin(), obj.data_name_count.end());
	exec_name_count.assign(obj.exec_name_count.begin(), obj.exec_name_count.end());
	math_func_count.assign(obj.math_func_count.begin(), obj.math_func_count.end());
	trig_func_count.assign(obj.trig_func_count.begin(), obj.trig_func_count.end());
	log_func_count.assign(obj.log_func_count.begin(), obj.log_func_count.end());
	cmplx_calc_count.assign(obj.cmplx_calc_count.begin(), obj.cmplx_calc_count.end());
	cmplx_cond_count.assign(obj.cmplx_cond_count.begin(), obj.cmplx_cond_count.end());
	cmplx_logic_count.assign(obj.cmplx_logic_count.begin(), obj.cmplx_logic_count.end());
	cmplx_preproc_count.assign(obj.cmplx_preproc_count.begin(), obj.cmplx_preproc_count.end());
	cmplx_assign_count.assign(obj.cmplx_assign_count.begin(), obj.cmplx_assign_count.end());
	cmplx_pointer_count.assign(obj.cmplx_pointer_count.begin(), obj.cmplx_pointer_count.end());
	cmplx_nestloop_count.assign(obj.cmplx_nestloop_count.begin(), obj.cmplx_nestloop_count.end());

	cmplx_cycfunct_count.assign(obj.cmplx_cycfunct_count.begin(), obj.cmplx_cycfunct_count.end());
	cmplx_cycfunct_CC2_count.assign(obj.cmplx_cycfunct_CC2_count.begin(), obj.cmplx_cycfunct_CC2_count.end());
	cmplx_cycfunct_CC3_count.assign(obj.cmplx_cycfunct_CC3_count.begin(), obj.cmplx_cycfunct_CC3_count.end());
	cmplx_cycfunct_CC4_count.assign(obj.cmplx_cycfunct_CC4_count.begin(), obj.cmplx_cycfunct_CC4_count.end());

	trunc_lines = obj.trunc_lines;
	total_lines = obj.total_lines;
	e_flag = obj.e_flag;
	file_name_isEmbedded = obj.file_name_isEmbedded;    // Modification: 2015.12
	file_name = obj.file_name;
	file_name_only = obj.file_name_only;                // Modification: 2015.12
	clearCaseTrailer = obj.clearCaseTrailer;            // Modification: 2015.12
	file_type = obj.file_type;
	class_type = obj.class_type;
	firstDuplicate = obj.firstDuplicate;
	duplicate = obj.duplicate;
	matched = obj.matched;
	mySLOCLines = obj.mySLOCLines;
	mySLOC_size = obj.mySLOC_size;

	return *this;
}


/*!
*    Resets Metrics data members to initial values.
*    This supports just in time File Physical line Read/Analyze/Count
*    File information was acquired earlier without Reading Physical lines.
*
*    Put Metrics value here !
*/
void results::resetMetrics()
{
// These metric values are filled in during Language parsing/analysis/counting
//
	// File related metrics
	trunc_lines = 0;
	total_lines = 0;
	
	// Physical and extracted Logical Source Lines Of Code
	// Original Physical lines are in related filemap struct
	SLOC_lines[0] = 0;
	SLOC_lines[1] = 0;

	// Line / comment type metrics
	blank_lines = 0;
	comment_lines = 0;
	e_comm_lines = 0;

	// Keyword related metrics
	directive_lines[0] = 0;
	directive_lines[1] = 0;
	data_lines[0] = 0;
	data_lines[1] = 0;
	exec_lines[0] = 0;
	exec_lines[1] = 0;

	// Math Keywords and Operands (and a few others)
	cmplx_math_lines = 0;
	cmplx_trig_lines = 0;
	cmplx_logarithm_lines = 0;
	cmplx_calc_lines = 0;
	cmplx_cond_lines = 0;
	cmplx_logic_lines = 0;
	cmplx_preproc_lines = 0;
	cmplx_assign_lines = 0;
	cmplx_pointer_lines = 0;
	directive_count.clear();
	data_name_count.clear();
	exec_name_count.clear();
	math_func_count.clear();
	trig_func_count.clear();
	log_func_count.clear();

	// Complexity Keywords / Operands
	cmplx_calc_count.clear();
	cmplx_cond_count.clear();
	cmplx_logic_count.clear();
	cmplx_preproc_count.clear();
	cmplx_assign_count.clear();
	cmplx_pointer_count.clear();

	// Cyclomatic Complexity metrics
	cmplx_nestloop_count.clear();
	cmplx_cycfunct_count.clear();
	cmplx_cycfunct_CC2_count.clear();
	cmplx_cycfunct_CC3_count.clear();
}


/*!
* Resets results to initial values.
* Do NOT put Metrics value here !
*/
void results::reset()
{
	// Clear all the Metrics values.
	resetMetrics();

	// Processing success results
	error_code = "";				// message
	e_flag = false;

	// File related values
	file_name_isEmbedded = false;	// Modification: 2015.12
	file_name = "";
	file_name_only = "";			// Modification: 2015.12
	clearCaseTrailer = "";			// Modification: 2015.12
	file_type = DATA;
	file_size = 0;					// Number of Physical lines, lines are in related fmap struct
	file_size_bytes = 0L;			// size in Bytes on OS file system
	class_type = UNKNOWN;			// Parser language for the file

	// Duplicate checking / Differencing
	duplicate = false;
	firstDuplicate = false;
	matched = false;

	// Logical source lines
	mySLOCLines.clear();
	mySLOC_size = 0;
}

/*!
* Clears the logical SLOC lines.
*/
void results::clearSLOC()
{
	mySLOCLines.clear();
}

/*!
* Adds a logical SLOC to the vector for differencing.
*
* \param line line to add
* \param trunc_flag indicates whether the SLOC has been truncated
*/

bool results::addSLOC(const string &line, const size_t line_num, bool &trunc_flag)
{
	string new_line(CUtil::ClearRedundantSpaces(line));
    if (new_line.length() == 0)
        return false;

    if (mySLOCLines.find(line) == mySLOCLines.end())
    {
        // mySLOCLines does not contain the line at this moment.
        // Create a new entry for thie line of source code amd add its line number to the vector.
        mySLOCLines[line] = vector<size_t>(1, line_num);
    }
    else
    {
        // mySLOCLines already contains this line at this moment.
        // Append the new line number to the end of the vector.
        mySLOCLines[line].push_back(line_num);
    }

    if (trunc_flag)
    {
        ++trunc_lines;
        trunc_flag = false;
    }

    return true;
}

/*!
* (overloaded function. This is taken from the public contribution folder "ucc_2014.11B_TAC")
* Adds a logical SLOC to the vector for differencing.
*
* \param line line to add
* \param trunc_flag indicates whether the SLOC has been truncated
*/
bool results::addSLOC(const string &line, bool &trunc_flag)
{
    string new_line = CUtil::ClearRedundantSpaces(line);
    if (new_line.length() == 0)
        return false;
    mySLOC_size++;
    pair<srcLineVector::iterator, bool> insertResult = mySLOCLines.insert(make_pair(new_line, vector<size_t>(1,1U)));
    if (!insertResult.second)
    {
        // element already exists
        //(*insertResult.first).second++;	// update number of times this SLOC exists

        // my code
        vector<size_t>::iterator it = (*insertResult.first).second.begin();
 
		while( it != (*insertResult.first).second.end() ){
		 (*it)=(*it)+1;
		 ++it;
		}

    }
    if (trunc_flag)
    {
        trunc_lines++;
        trunc_flag = false;
    }
    return true;
}


#ifdef	_DEBUG

// Call (for example) to make sure a long list of files is OK before Duplicate checking or Differencing
//
// We go to the trouble of having this bool flag as it GREATLY speeds up processing.
bool results::isEmbeddedConsistent()
{
	bool retVal = true;

	size_t	embFileIdx = file_name.find( EMBEDDED_FILE_PREFIX );
	if ( file_name_isEmbedded )
	{
		if ( string::npos == embFileIdx )
			retVal = false;		// actual file name has NO *.* as part of name.
	}
	else
	{
		if ( string::npos != embFileIdx )
			retVal = false;		// actual file name HAS *.* as part of name.
	}

	return retVal;
}

#endif	// #ifdef	_DEBUG

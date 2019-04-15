//! Common code counter methods for sub-classing individual languages.
/*!
* \file CCodeCounter.h
*
* This file contains the common code counter methods for sub-classing individual languages.
*
* Modified: 2015.12 by Randy Maxwell 
*    Support for 1 pass language parsers (Scala)
*/

#include "UCCBeforeLibraryIncludes.h"    // Modification: 2015.12
//#include <time.h>
#include	<ctime>
//#include "stdio.h"
#include	<cstdio>
//#include "sstream"
#include	<sstream>
#include <errno.h>
#include <cstring>
#include "UCCAfterLibraryIncludes.h"     // Modification: 2015.12

#include "UCCGlobals.h"                  // Modification: 2015.12
#include "UCCFilesOut.h"                 // Modification: 2015.12
#include "CCodeCounter.h"


#ifdef	_DEBUG
// Enable this to get more detailed information (likely using Debugger to look at structures)
// MAY improve details provided in Stack Dump
// This will slow processing speed somewhat
//
//	#define		_DEBUG_1_PASS_PARSE
#endif
/*!
* 1.File Description: 
*   Constructs a CCodeCounter object.
*
* 2.Parameter:
*   None
* 
* 3.Creation Time and Owner: 
*   version 2009.01
*/
CCodeCounter::CCodeCounter()
{
#ifdef	ENABLE_1_PASS_PARSER
	// 1 pass through the source file support
	use_1pass_parser = false;			// Modification: 2015.12
	parse_alloc_count = 0;				// Modification: 2015.12
	pe = (parseElement *) NULL;			// Modification: 2015.12
	pe_dup_keywords = false;			// Modification: 2015.12
	pe_max_idx = 0;						// Modification: 2015.12
	pe_mid_idx = 0;						// Modification: 2015.12
	pe_mid_keyword = "";				// Modification: 2015.12
#endif

	print_cmplx = false; //modified in version 2014.08 
	cc4enable   = false;				// Modification: 2015.12
	lsloc_truncate = DEFAULT_TRUNCATE;
	QuoteStart = "";      //modified in version 2009.01
	QuoteEnd = "";
	quote_start_is_end = true;			// Modification: 2015.12
	QuoteMultiStart = "";				// Modification: 2015.12
	QuoteMultiEnd   = "";				// Modification: 2015.12
	quote_multi_start_is_end = true;	// Modification: 2015.12
	QuoteEscapeFront = 0;
	QuoteEscapeRear = 0;
	ContinueLine = "";
	classtype = UNKNOWN; //modified in version 2011.10B
	language_name = DEF_LANG_NAME;
	language_version = "";				// Modification: 2015.12
	language_version_authority = "";	// Modification: 2015.12
	casesensitive = true; //modified in version 2009.01
	total_filesA = 0;      //modified in version 2011.05
	total_filesB = 0;
	total_dupFilesA = 0;
	total_dupFilesB = 0;

	// Support for simple Stack Dump				Modified: 2015.12
	// Keep track of the current line of the source file being parsed.
	currentPhyLine = (unsigned int)(-1);	// Set to invalid value to start
	currentLSrcLine = 0;
	ENTER_1( "" );
	ENTER_2( "" );
	ENTER_3( "" );
	ENTER_4( "" );
    //Modification: 11.2016 Ext-4
    CUtil::allocate();
}

/*!
* 1.File Description: 
*   Destroys a CCodeCounter object.
*
* 2.Parameters:
*   None 
*
* 3.Creation Time and Owner: 
*   version 2009.01
*   Revised 2015.12  Free allocated resources
*/
CCodeCounter::~CCodeCounter()
{
#ifdef	ENABLE_1_PASS_PARSER
	if ( 0 < parse_alloc_count )
	{
		// Free memory used by parse array
		if ( NULL != pe )
		{
			for ( unsigned int i = 0; i < parse_alloc_count; i++ )
				pe[i].keyword.resize( 0 );

			delete [] pe;
			pe = (parseElement *) NULL;
		}
		parse_alloc_count = 0;
	}
#endif
}

/*!
* 1.File Description: 
*   This method removes the existing content of the vectors and assigns them all zeros and initializes the count vectors.
*
* 2.Parameters:
*   None 
*
* 3.Creation Time and Owner: 
*   version 2011.05
*/
void CCodeCounter::InitializeCounts()
{
	ENTER_1( "Start" );
	unsigned int i = 0;	//modified in version 2013.04
	counted_files = 0;
	counted_dupFiles = 0;

	directive_count.assign(directive.size(), make_pair(i, i));
	data_name_count.assign(data_name_list.size(), make_pair(i, i));
	exec_name_count.assign(exec_name_list.size(), make_pair(i, i));
	math_func_count.assign(math_func_list.size(), make_pair(i, i));
	trig_func_count.assign(trig_func_list.size(), make_pair(i, i));
	log_func_count.assign(log_func_list.size(), make_pair(i, i));

	cmplx_calc_count.assign(cmplx_calc_list.size(), make_pair(i, i));
	cmplx_cond_count.assign(cmplx_cond_list.size(), make_pair(i, i));
	cmplx_logic_count.assign(cmplx_logic_list.size(), make_pair(i, i));
	cmplx_preproc_count.assign(cmplx_preproc_list.size(), make_pair(i, i));
	cmplx_assign_count.assign(cmplx_assign_list.size(), make_pair(i, i));
	cmplx_pointer_count.assign(cmplx_pointer_list.size(), make_pair(i, i));
}

/*!
* 1.File Description: 
*   This method removes the existing content of the vectors and assigns them all zeros and initializes the count vectors for a result
*
* 2.Parameters: 
*   result: counter results
*
* 3.Creation Time and Owner: 
*   version 2011.05
*/
void CCodeCounter::InitializeResultsCounts(results* result)
{
	ENTER_1( "Start" );
	result->directive_count.assign(directive.size(), 0);
	result->data_name_count.assign(data_name_list.size(), 0);
	result->exec_name_count.assign(exec_name_list.size(), 0);
	result->math_func_count.assign(math_func_list.size(), 0);
	result->trig_func_count.assign(trig_func_list.size(), 0);
	result->log_func_count.assign(log_func_list.size(), 0);

	result->cmplx_calc_count.assign(cmplx_calc_list.size(), 0);
	result->cmplx_cond_count.assign(cmplx_cond_list.size(), 0);
	result->cmplx_logic_count.assign(cmplx_logic_list.size(), 0);
	result->cmplx_preproc_count.assign(cmplx_preproc_list.size(), 0);
	result->cmplx_assign_count.assign(cmplx_assign_list.size(), 0);
	result->cmplx_pointer_count.assign(cmplx_pointer_list.size(), 0);
}

/*!
* 1.File Description: 
*   Processes and counts the source file
*   2015.12  Added details if Exception happens, currentPhyLine
*   Always returns 0
*
* 2.Parameter: 
*   fmap: list of file lines and result counter results
*
* 3.Creation Time and Owner:
*   version  2009.01
*   Revised  2015.12  Added support for Stack Dump if Exception happens: SAVE_TO_n
*/
int CCodeCounter::CountSLOC(filemap* fmap, results* result)
{
	// backup file content before modifying it (comments and directive lines are cleared)
	// fmapBak is same as fmap except that it stores unmodified quoted strings
	// fmap has quoted strings replaced with '$'
	filemap fmapMod = *fmap;     //added in version 2011.05
	filemap fmapModBak = *fmap;
	
	AddTraceLine(__LINE__, UCC_FUNC_NAME, "<no info>");

	// Set up lightweight support for Stack Dump
	SAVE_TO_2( "" );
	SAVE_TO_3( "" );
	SAVE_TO_4( "" );
	currentPhyLine  = 1;
	currentLSrcLine = 0;

	SAVE_TO_1( "InitializeResultsCounts" );
	InitializeResultsCounts(result);

	SAVE_TO_1( "PreCountProcess" );
	PreCountProcess(&fmapMod);

	int parse_result = 1;

#ifdef	ENABLE_1_PASS_PARSER
	if ( true == use_1pass_parser )
	{
		SAVE_TO_1( "ParseFile" );
		parse_result = ParseFile( &fmapMod, result, &fmapModBak );
	}
	
	if ( ( 0 != parse_result )
	  || ( false == use_1pass_parser ) )
#else
	if ( 0 != parse_result )
#endif
	{
		SAVE_TO_1( "CountBlankSLOC" );
		CountBlankSLOC(&fmapMod, result);

		SAVE_TO_1( "CountCommentsSLOC" );
		CountCommentsSLOC(&fmapMod, result, &fmapModBak);

		if (print_cmplx)
		{
			SAVE_TO_1( "CountComplexity" );
			CountComplexity(&fmapMod, result);
		}

		SAVE_TO_1( "CountDirectiveSLOC" );
		CountDirectiveSLOC(&fmapMod, result, &fmapModBak);

                SAVE_TO_1( "FindHalsteadsVolume" );
                FindHalsteadsVolume(fmapModBak, result);
               
		SAVE_TO_1( "LanguageSpecificProcess" );
		LanguageSpecificProcess(&fmapMod, result, &fmapModBak);
	}

	SAVE_TO_1( "Exit CountSLOC" );
	return 0;
}

/*!
* 1.File Description: 
*   Checks whether the file extension is supported by the language counter.Return whether file extension is supported
*
* 2.Parameters: 
*   file_name: file name
*
* 3.Creation Time and Owner: 
*   version 2009.01
*/
bool CCodeCounter::IsSupportedFileExtension(const string &file_name)
{
    // if Makefile, check whether name equals MAKEFILE since no extension exists
    if (classtype == MAKEFILE && file_name.size() >= 8)
    {
        if (CUtil::ToLower(file_name.substr(file_name.size() - 8)) == "makefile")
            return true;
    }
    for (StringVector::iterator vit = file_extension.begin(); vit != file_extension.end(); vit++)
    {
        // to check if one of the extensions does not contain a .
        if((*vit).find_last_of(".") == string::npos) {
            // to check for a no extension
            if((*vit).empty()) {
                size_t idx = file_name.find_last_of(".");
                if (idx == string::npos)
                    return true;
            }
        } else {
            if (CUtil::EndsWith(file_name, (*vit), false))
            {
                // if X-Midas/NeXtMidas, parse file to check for startmacro or endmacro (needed since Midas can use .txt or .mm)
                if (classtype == XMIDAS || classtype == NEXTMIDAS)
                {
                    string oneline;
                    ifstream fr(file_name.c_str(), ios::in);
                    if (!fr.is_open())
                        return false;

                    // search for "startmacro" (optional) or "endmacro" (required)
                    while (fr.good() || fr.eof())
                    {
                        getline(fr, oneline);
                        if ((!fr.good() && !fr.eof()) || (fr.eof() && oneline.empty()))
                            break;
                        oneline = CUtil::ToLower(CUtil::TrimString(oneline));
                        if (oneline.compare(0, 10, "startmacro") == 0 || oneline.compare(0, 8, "endmacro") == 0)
                        {
                            fr.clear();
                            fr.close();
                            return true;
                        }
                        if (!fr.good())
                            break;
                    }
                    fr.clear();
                    fr.close();
                }
                else
                    return true;
                break;
            }
        }
    }
    return false;
}

/*!
* 1.Function Description: 
    Retrieves the language output file stream and Opens a new stream if it has not been opened already.And return output file stream
*
* 2.Parameters: 
*   outputFileNamePrePend: name to prepend to the output file; cmd current command line string;
*   csvOutput: CSV file stream? (otherwise ASCII text file); legacyOutput legacy format file stream? (otherwise standard text file)
*
* 3.Creation Time and Owner:
*   version  2009.01
*/
ofstream* CCodeCounter::GetOutputStream(const string &outputFileNamePrePend, const string &cmd, bool csvOutput, bool legacyOutput)
{
	if (csvOutput)   //modified in version 2011.05
	{
		if (!output_file_csv.is_open())
		{
			string fname = outputFileNamePrePend + language_name + OUTPUT_FILE_NAME_CSV;
			output_file_csv.open(fname.c_str(), ofstream::out);

			if (!output_file_csv.is_open()) return NULL;

			PrintFileHeader(output_file_csv, "SLOC COUNT RESULTS", cmd);

			PrintFileHeaderLine(output_file_csv, "RESULTS FOR " + language_name + " FILES");
			output_file_csv << endl;
			output_file_csv << "Total,Blank,Comments,,Compiler,Data,Exec.,Logical,Physical,File,Module" << endl;
			output_file_csv << "Lines,Lines,Whole,Embedded,Direct.,Decl.,Instr.,SLOC,SLOC,Type,Name" << endl;
		}
		return &output_file_csv;
	}
	else
	{
		if (!output_file.is_open())
		{
			string fname = outputFileNamePrePend + language_name + OUTPUT_FILE_NAME;
			output_file.open(fname.c_str(), ofstream::out);

			if (!output_file.is_open()) return NULL;

			PrintFileHeader(output_file, "SLOC COUNT RESULTS", cmd);

			PrintFileHeaderLine(output_file, "RESULTS FOR " + language_name + " FILES");
			output_file << endl;
			if (legacyOutput)
			{
				output_file << "   Total   Blank |      Comments    | Compiler  Data   Exec.  | Logical | File  Module" << endl;
				output_file << "   Lines   Lines |   Whole Embedded | Direct.   Decl.  Instr. |   SLOC  | Type  Name" << endl;
				output_file << "-----------------+------------------+-------------------------+---------+---------------------------" << endl;
			}
			else
			{
				output_file << "   Total   Blank |      Comments    | Compiler  Data   Exec.  | Logical Physical | File  Module" << endl;
				output_file << "   Lines   Lines |   Whole Embedded | Direct.   Decl.  Instr. |   SLOC    SLOC   | Type  Name" << endl;
				output_file << "-----------------+------------------+-------------------------+------------------+---------------------------" << endl;
			}
		}
		return &output_file;
	}
}

/*!
* 1.Function Description:
*   Closes the language output file stream.
*
* 2.Parameters:
*   None
*
* 3.Creation Time and Owner:
*   version  2009.01
* 4. Modification : 2016.12
*    Before closing the language output files, check if copyright footer needs to be printed
*/
void CCodeCounter::CloseOutputStream()
{
	if (output_file.is_open()) {
		PrintFileFooter(output_file);    //Modification 2016.12
		output_file.close();
	}
	if (output_file_csv.is_open()) {
		PrintFileFooter(output_file_csv);  //Modification 2016.12
		output_file_csv.close();
	}
		
}

/*!
* 1.Function Description:
*   Finds the first index of one of the characters of strQuote in strline.Return value is index of strQuote character in strline
*
* 2.Parameters:
*   strline: string line
*   strQuote: string of character(s) to find in strline
*   idx_start: index of line character to start search
*   QuoteEscapeFront: quote escape character
*
* 3.Creation Time and Owner:
*   version 2009.01
*/
size_t CCodeCounter::FindQuote(string const &strline, string const &strQuote, size_t idx_start, char QuoteEscapeFront)
{
	size_t min_idx, idx;    //modified in version 2009.01
	min_idx = strline.length();

	for (size_t i = 0; i < strQuote.length(); i++)
	{
		idx = CUtil::FindCharAvoidEscape(strline, strQuote[i], idx_start, QuoteEscapeFront);
		if (idx != string::npos && idx < min_idx)
			min_idx = idx;
	}
	if (min_idx < strline.length())
		return min_idx;
	return string::npos;
}

/*!
* 1.Function Description:
*   Replaces up to ONE quoted string inside a string starting at idx_start.Return value is the method status
*
* 2.Parameters:
*   strline: string to be processed
*   idx_start: index of line character to start search
*   contd: specifies the quote string is continued from the previous line
*   CurrentQuoteEnd: end quote character of the current status
*
* 3.Creation Time and Owner:
*   version 2009.01
* 
*/
int CCodeCounter::ReplaceQuote(string &strline, size_t &idx_start, bool &contd, char &CurrentQuoteEnd)
{
	size_t idx_end, idx_quote;
	AddTraceLine( __LINE__, UCC_FUNC_NAME, strline );    // Modified: 2015.12
	if (contd)
	{
		idx_start = 0;    //modified in version 2009.01
		if (strline[0] == CurrentQuoteEnd)   //modified in version 2011.05
		{
			idx_start = 1;
			contd = false;
			return 1;
		}
		strline[0] = '$';  //modified in version 2009.01
	}
	else
	{
		// handle two quote chars in some languages, both " and ' may be accepted
		idx_start = FindQuote(strline, QuoteStart, idx_start, QuoteEscapeFront);
		if (idx_start != string::npos)
		{
			idx_quote = QuoteStart.find_first_of(strline[idx_start]);
			CurrentQuoteEnd = QuoteEnd[idx_quote];
		}
		else
		{
			idx_start = strline.length();
			return 0;
		}
	}

	idx_end = CUtil::FindCharAvoidEscape(strline, CurrentQuoteEnd, idx_start + 1, QuoteEscapeFront);
	if (idx_end == string::npos)
	{
		idx_end = strline.length() - 1;
		strline.replace(idx_start + 1, idx_end - idx_start, idx_end - idx_start, '$');
		contd = true;
		idx_start = idx_end + 1; //modified in version 2011.05
	}
	else
	{
		if ((QuoteEscapeRear) && (strline.length() > idx_end + 1) && (strline[idx_end+1] == QuoteEscapeRear))  //modified in version 2009.01
		{
			strline[idx_end] = '$';
			strline[idx_end+1] = '$';
		}
		else
		{
			contd = false;
			strline.replace(idx_start + 1, idx_end - idx_start - 1, idx_end - idx_start - 1, '$');
			idx_start = idx_end + 1;  //modified in version 2011.10
		}
	}

	return 1;
}

/*!
* 1.Function Description:
*   Counts blank lines in a file.Return value is method status
*
* 2.Parameters:
*   fmap: list of file lines
*   result: counter results
*
* 3.Creation Time and Owner:
*   version 2009.01
*   Revised 2015.12  Some support for Stack Dump if exception
*/
int CCodeCounter::CountBlankSLOC(filemap* fmap, results* result)
{
	ENTER_1( "Start" );	// Due to Polymorphism change level above
	SAVE_TO_2( "CheckBlank" );
	currentPhyLine = 0;
	for (filemap::iterator i = fmap->begin(); i != fmap->end(); i++) //modified in version 2009.01
	{
		currentPhyLine++;
		if (CUtil::CheckBlank(i->line)) // from version 2009.01
			result->blank_lines++;
	}

	return 1;
}

/*!
* 1.Function Description:
*   Counts the number of comment lines, removes comments, and
*   replaces quoted strings by special chars, e.g., $.All arguments are modified by the method.Return value is the method status
*                        
* 2.Parameters:
*   fmap: list of processed file lines
*   result: counter results
*   fmapBak: list of original file lines (same as fmap except it contains unmodified quoted strings)
*
* 3.Creation Time and Owner:
*   fversion 2009.01
*   Revised  2015.12  Added lightweight support for Stack Dump
*/
int CCodeCounter::CountCommentsSLOC(filemap* fmap, results* result, filemap *fmapBak)
{
	// Check Preconditions
	if (BlockCommentStart.empty() && LineCommentStart.empty())
		return 0;
	if (classtype == UNKNOWN || classtype == DATAFILE)
		return 0;

	bool contd = false;
	bool contd_nextline;
	int comment_type = 0;
	/*
	comment_type:
	0 : not a comment
	1 : line comment, whole line
	2 : line comment, embedded
	3 : block comment, undecided
	4 : block comment, embedded
	*/

	size_t idx_start, idx_end, comment_start;
	size_t quote_idx_start;
	string curBlckCmtStart, curBlckCmtEnd;
	char CurrentQuoteEnd = 0;
	bool quote_contd = false;
	filemap::iterator itfmBak = fmapBak->begin();

	quote_idx_start = 0;
	
	ENTER_1( "Start" );	// Due to Polymorphism change level above
	SAVE_TO_2( "Loop start" );
	currentPhyLine = 0;
	for (filemap::iterator iter = fmap->begin(); iter != fmap->end(); iter++, itfmBak++)
	{
		currentPhyLine++;
		contd_nextline = false;

		quote_idx_start = 0;
		idx_start = 0;

		SAVE_TO_2( "CUtil::CheckBlank" );
		if (CUtil::CheckBlank(iter->line))
			continue;

		if (quote_contd)
		{
			AddTraceLine( __LINE__, UCC_FUNC_NAME, iter->line );    // Modified: 2015.12
			
			// Replace quote until next character
			SAVE_TO_2( "ReplaceQuote" );
			ReplaceQuote(iter->line, quote_idx_start, quote_contd, CurrentQuoteEnd);
			if (quote_contd)
				continue;
		}

		if (contd)
			comment_type = 3;

		while (!contd_nextline && idx_start < iter->line.length())
		{
			AddTraceLine( __LINE__, UCC_FUNC_NAME, iter->line );    // Modified: 2015.12
			
			// need to handle multiple quote chars in some languages, both " and ' may be accepted
			SAVE_TO_2( "FindQuote" );
			quote_idx_start = FindQuote(iter->line, QuoteStart, quote_idx_start, QuoteEscapeFront);
			comment_start = idx_start;
			if (!contd)
			{
				SAVE_TO_2( "FindCommentStart" );
				FindCommentStart(iter->line, comment_start, comment_type, curBlckCmtStart, curBlckCmtEnd);
			}

			if (comment_start == string::npos && quote_idx_start == string::npos)
				break;

			if (comment_start != string::npos)
				idx_start = comment_start;

			// if found quote before comment, e.g., "this is quote");//comment
			if (quote_idx_start != string::npos && (comment_start == string::npos || quote_idx_start < comment_start))
			{
				AddTraceLine( __LINE__, UCC_FUNC_NAME, iter->line );    // Modified: 2015.12
				
				SAVE_TO_2( "ReplaceQuote_2" );
				ReplaceQuote(iter->line, quote_idx_start, quote_contd, CurrentQuoteEnd);
				if (quote_idx_start > idx_start && quote_idx_start != iter->line.length())
				{
					// comment delimiter inside quote
					idx_start = quote_idx_start;
					continue;
				}
			}
			else if (comment_start != string::npos)   
			{
				// comment delimiter starts first
				switch (comment_type)
				{
				case 1:	// line comment, definitely whole line
					AddTraceLine( __LINE__, UCC_FUNC_NAME, iter->line );    // Modified: 2015.12
					
					iter->line = "";
					itfmBak->line = "";
					result->comment_lines++;
					contd_nextline = true;
					break;
				case 2:	// line comment, possibly embedded
					AddTraceLine( __LINE__, UCC_FUNC_NAME, iter->line );    // Modified: 2015.12
					
					iter->line = iter->line.substr(0, idx_start);
					itfmBak->line = itfmBak->line.substr(0, idx_start);
					
					// trim trailing space
					SAVE_TO_2( "CUtil::TrimString" );
					iter->line = CUtil::TrimString(iter->line, 1);
					itfmBak->line = CUtil::TrimString(itfmBak->line, 1);
					if (iter->line.empty())
						result->comment_lines++;	// whole line
					else
						result->e_comm_lines++;		// embedded
					contd_nextline = true;
					break;
				case 3:	// block comment
				case 4:
					AddTraceLine( __LINE__, UCC_FUNC_NAME, iter->line );    // Modified: 2015.12
					
					if (contd)
						idx_end = iter->line.find(curBlckCmtEnd);
					else
						idx_end = iter->line.find(curBlckCmtEnd, idx_start + curBlckCmtStart.length());

					if (idx_end == string::npos)
					{
						if (comment_type == 3)
						{
							iter->line = "";
							itfmBak->line = "";
							result->comment_lines++;
						}
						else if (comment_type == 4)
						{
							iter->line = iter->line.substr(0, idx_start);
							itfmBak->line = itfmBak->line.substr(0, idx_start);

							// trim trailing space
							SAVE_TO_2( "CUtil::TrimString_2" );
							iter->line = CUtil::TrimString(iter->line, 1);
							itfmBak->line = CUtil::TrimString(itfmBak->line, 1);
							if (iter->line.empty())          //block added in 2011.10
								result->comment_lines++;	// whole line
							else
								result->e_comm_lines++;		// embedded
						}
						contd = true;
						contd_nextline = true;
						break;
					}
					else
					{
						contd = false;
						iter->line.erase(idx_start, idx_end - idx_start + curBlckCmtEnd.length());
						itfmBak->line.erase(idx_start, idx_end - idx_start + curBlckCmtEnd.length());
						if (iter->line.empty())
							result->comment_lines++;
						else
						{
							// trim trailing space
							SAVE_TO_2( "CUtil::TrimString_3" );
							iter->line = CUtil::TrimString(iter->line, 1);
							itfmBak->line = CUtil::TrimString(itfmBak->line, 1);
							if (iter->line.empty())		//block added in 2011.10
								result->comment_lines++;	// whole line
							else
								result->e_comm_lines++;		// embedded
						}

						// quote chars found may be erased as it is inside comment
						quote_idx_start = idx_start;
					}
					break;
				default:
					cout << "Error in CountCommentsSLOC()" << endl;
					break;
				}
			}
		}
	}
	SAVE_TO_2( "Exit" );
	return 1;
}


/*!
* Per Line code that Counts the number of comment lines, removes comments, and
* replaces quoted strings by special chars, e.g., $
* All arguments are modified by the method.
*
* \param fmap list of processed file lines
* \param result counter results
* \param fmapBak list of original file lines (same as fmap except it contains unmodified quoted strings)
* \param lots of other args
* \return 0 if no errors
*/
int CCodeCounter::CountCommentsSLOCperLine( filemap::iterator iter,
										results* result,
										filemap::iterator itfmBak,
										bool & contd, 
										int & comment_type,
										bool & contd_nextline,
										size_t & idx_start,
										size_t & idx_end,
										size_t & quote_idx_start,
										size_t & comment_start,
										string & curBlckCmtStart,
										string & curBlckCmtEnd,
										bool & quote_contd,
										char & CurrentQuoteEnd )
{
	// NOTE:
	// Kept the args the same as if was NOT a function call 
	// so below code is unchanged (except for less indentation)
	//
	// After use and peer review, 
	// strongly suggest using a struct to hold the args (maybe except for first 3)

	int retVal = 0;

	// This block of code was originally in a loop in CountCommentsSLOC
	// Extracted to here so can also be called when 1 pass parser needs
	//
	if (contd)
		comment_type = 3;

	while (!contd_nextline && idx_start < iter->line.length())
	{
		// need to handle multiple quote chars in some languages, both " and ' may be accepted
		quote_idx_start = FindQuote(iter->line, QuoteStart, quote_idx_start, QuoteEscapeFront);
		comment_start = idx_start;
		if (!contd)
			FindCommentStart(iter->line, comment_start, comment_type, curBlckCmtStart, curBlckCmtEnd);

		if (comment_start == string::npos && quote_idx_start == string::npos)
			break;

		if (comment_start != string::npos)
			idx_start = comment_start;

		// if found quote before comment, e.g., "this is quote");//comment
		if (quote_idx_start != string::npos && (comment_start == string::npos || quote_idx_start < comment_start))
		{
			ReplaceQuote(iter->line, quote_idx_start, quote_contd, CurrentQuoteEnd);
			if (quote_idx_start > idx_start && quote_idx_start != iter->line.length())
			{
				// comment delimiter inside quote
				idx_start = quote_idx_start;
				continue;
			}
		}
		else if (comment_start != string::npos)
		{
			// comment delimiter starts first
			switch (comment_type)
			{
			case 1:	// line comment, definitely whole line
				iter->line = "";
				itfmBak->line = "";
				result->comment_lines++;
				contd_nextline = true;
				break;
			case 2:	// line comment, possibly embedded
				iter->line = iter->line.substr(0, idx_start);
				itfmBak->line = itfmBak->line.substr(0, idx_start);
				// trim trailing space
				iter->line = CUtil::TrimString(iter->line, 1);
				itfmBak->line = CUtil::TrimString(itfmBak->line, 1);
				if (iter->line.empty())
					result->comment_lines++;	// whole line
				else
					result->e_comm_lines++;		// embedded
				contd_nextline = true;
				break;
			case 3:	// block comment
			case 4:
				if (contd)
					idx_end = iter->line.find(curBlckCmtEnd);
				else
					idx_end = iter->line.find(curBlckCmtEnd, idx_start + curBlckCmtStart.length());

				if (idx_end == string::npos)
				{
					if (comment_type == 3)
					{
						iter->line = "";
						itfmBak->line = "";
						result->comment_lines++;
					}
					else if (comment_type == 4)
					{
						iter->line = iter->line.substr(0, idx_start);
						itfmBak->line = itfmBak->line.substr(0, idx_start);
						// trim trailing space
						iter->line = CUtil::TrimString(iter->line, 1);
						itfmBak->line = CUtil::TrimString(itfmBak->line, 1);
						if (iter->line.empty())
							result->comment_lines++;	// whole line
						else
							result->e_comm_lines++;		// embedded
					}
					contd = true;
					contd_nextline = true;
					break;
				}
				else
				{
					contd = false;
					iter->line.erase(idx_start, idx_end - idx_start + curBlckCmtEnd.length());
					itfmBak->line.erase(idx_start, idx_end - idx_start + curBlckCmtEnd.length());
					if (iter->line.empty())
						result->comment_lines++;
					else
					{
						// trim trailing space
						iter->line = CUtil::TrimString(iter->line, 1);
						itfmBak->line = CUtil::TrimString(itfmBak->line, 1);
						if (iter->line.empty())
							result->comment_lines++;	// whole line
						else
							result->e_comm_lines++;		// embedded
					}

					// quote chars found may be erased as it is inside comment
					quote_idx_start = idx_start;
				}
				break;
			default:
				cout << "Error in CountCommentsSLOCPerLine()" << endl;
				retVal = -7;
				break;
			}
		}
	}

	return retVal;
}


/*!
* 1.Function Description:
    Finds a starting position of a comment in a string starting at idx_start.Return value is the method status
*
* 2.Parameters:
*   strline: string to be processed
*   idx_start: index of line character to start search
*   comment_type: comment type (0=not a comment, 1=whole line, 2=embedded line, 3=whole line block, 4=embedded block)
*   curBlckCmtStart: current block comment start string
*   curBlckCmtEnd: current block comment end string
*
* 3.Creation Time and Owner:
*   version 2009.01
*/
int CCodeCounter::FindCommentStart(string strline, size_t &idx_start, int &comment_type,
								   string &curBlckCmtStart, string &curBlckCmtEnd) 
{
	size_t idx_line, idx_tmp, idx_block;
	string line = strline;
	comment_type = 0;

	ENTER_2( "Start" );	// Due to Polymorphism change level above

	if (!casesensitive)
	{
		SAVE_TO_3( "CUtil::ToLower" );
		line = CUtil::ToLower(line);
	}

	// searching for starting of line comment
	idx_line = string::npos;
	for (StringVector::iterator i = LineCommentStart.begin(); i != LineCommentStart.end(); i++)
	{
		idx_tmp = line.find((casesensitive ? (*i) : CUtil::ToLower(*i)), idx_start);
		if (idx_tmp < idx_line) 
			idx_line = idx_tmp;
	}

	// searching for starting of block comment
	idx_block = string::npos;
	for (StringVector::iterator i = BlockCommentStart.begin(); i != BlockCommentStart.end(); i++)
	{
		idx_tmp = strline.find(*i, idx_start);
		if (idx_tmp < idx_block)
		{
			idx_block = idx_tmp;
			curBlckCmtStart = *i;
			curBlckCmtEnd = *(BlockCommentEnd.begin() + (i - BlockCommentStart.begin()));
		}
	}

	// see what kind of comment appears first
	if (idx_line == string::npos && idx_block == string::npos)
	{
		comment_type = 0;
		idx_start = idx_line;
	}
	else if (idx_block > idx_line)
	{
		idx_start = idx_line;
		comment_type = idx_start == 0 ? 1 : 2;
	}
	else
	{
		idx_start = idx_block;
		comment_type = idx_start == 0 ? 3 : 4;
	}
	return 1;
}

/*!
* 1.Function Description:
*   Counts file language complexity based on specified language keywords/characters.Return value is the method status
*
* 2.Parameters:
*   fmap: list of processed file lines
*   result: counter results
*
* 3.Creation Time and Owner:
*   version 2009.01
*   Revised 2015.12  Added lightweight support for Stack Dump
*/
int CCodeCounter::CountComplexity(filemap* fmap, results* result)
{
	//AddTraceLine( __LINE__, UCC_FUNC_NAME, "<no info>" );    // Modified: 2015.12
	
	if (classtype == UNKNOWN || classtype == DATAFILE) // modified in version 2011.10
		return 0;

	ENTER_1( "Start" );		// Due to Polymorphism change level above
	SAVE_TO_2( "Start CCodeCounter::CountComplexity" );

	filemap::iterator fit; //modified in version 2009.01
	size_t idx;            //modified in version 2013.04
	unsigned int cnt, ret, cyclomatic_cnt = 0, ignore_cyclomatic_cnt = 0, main_cyclomatic_cnt = 0, function_count = 0, cyclomatic_logic_cnt = 0, main_cyclomatic_logic_cnt = 1, cyclomatic_case_cnt = 0, main_cyclomatic_case_cnt = 1, cyclomatic_default_cnt = 0, cyclomatic_switch_cnt = 0;//modified in version 2014.08

	unsigned int main_cyclomatic_distinct_cond_cnt = 1; // integrate from CC4 2015:06:26
	unsigned int cyclomatic_repeated_cond_cnt = 0; //integrate from CC4 2015:06:26

	string line, lastline, file_ext, function_name = "";
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$"; //modified in version 2009.01
	filemap function_stack; //modified in version 2013.04

	stack<unsigned int> cyclomatic_repeated_cond_stack;// integrate from CC4 2015:06:26

	stack<unsigned int> cyclomatic_stack;
	stack<unsigned int> cyclomatic_logic_stack; //modified in version 2014.08
	stack<unsigned int> cyclomatic_case_stack;

	//////Integrate from CC4 2015:06:26
	stack<set<string> > cyclomatic_distinct_cond_stack; // at the end of a function, the size of this stack is CC4 value of this function
	set<string> cyclomatic_distinct_cond_set;
	string cc4_valid_if;  // the vaild if get from current line
	stack<string> cc4_parent_stack;
	int cc4_nested_dup=0; // in a function
	stack<int> cc4_nested_dup_stack;
	set<string> nested_set;
	map<unsigned int, lineElement> cond_CC4_map;
	/////////

	map<unsigned int, lineElement> function_map; //modified in version 2013.04
	map<unsigned int, lineElement> logical_map; //modified in version 2014.08
	map<unsigned int, lineElement> case_map;
	bool process_cyclomatic_complexity = false; // modified in version 2013.04

	// check whether to process cyclomatic complexity
	if (cmplx_cyclomatic_list.size() > 0)
	{
		//AddTraceLine( __LINE__, UCC_FUNC_NAME, "Process Cyclomatic Complexity" );    // Modified: 2015.12
		
		process_cyclomatic_complexity = true;
		if (skip_cmplx_cyclomatic_file_extension_list.size() > 0)
		{
			idx = result->file_name.find_last_of(".");
			if (idx != string::npos)
			{
				file_ext = result->file_name.substr(idx);
				file_ext = CUtil::ToLower(file_ext);
				if (find(skip_cmplx_cyclomatic_file_extension_list.begin(), skip_cmplx_cyclomatic_file_extension_list.end(), file_ext) != skip_cmplx_cyclomatic_file_extension_list.end())
					process_cyclomatic_complexity = false;
			}
		}
	}

	// process each line
	SAVE_TO_2( "CCodeCounter::CountComplexity loop start" );
	currentPhyLine = 0;
	for (fit = fmap->begin(); fit != fmap->end(); fit++)  // added in version 2009.01
	{
		currentPhyLine++;

		//AddTraceLine( __LINE__, UCC_FUNC_NAME, fit->line );    // Modified: 2015.12

		line = fit->line;

		SAVE_TO_2( "CUtil::CheckBlank" );
		if (CUtil::CheckBlank(line))
			continue;

		line = " " + line;
		// mathematical functions
		cnt = 0;
		SAVE_TO_2( "CUtil::CountTally Math" );
		CUtil::CountTally(line, math_func_list, cnt, 1, exclude, "", "", &result->math_func_count, casesensitive);
		result->cmplx_math_lines += cnt;

		// trigonometric functions
		cnt = 0;
		SAVE_TO_2( "CUtil::CountTally Trigonometric" );
		CUtil::CountTally(line, trig_func_list, cnt, 1, exclude, "", "", &result->trig_func_count, casesensitive);
		result->cmplx_trig_lines += cnt;

		// logarithmic functions
		cnt = 0;
		SAVE_TO_2( "CUtil::CountTally Logrithmic" );
		CUtil::CountTally(line, log_func_list, cnt, 1, exclude, "", "", &result->log_func_count, casesensitive);

		// calculations
		cnt = 0;
		SAVE_TO_2( "CUtil::CountTally Calculations" );
		CUtil::CountTally(line, cmplx_calc_list, cnt, 1, exclude, "", "", &result->cmplx_calc_count, casesensitive);
		result->cmplx_calc_lines += cnt;

		// conditionals
		cnt = 0;
		SAVE_TO_2( "CUtil::CountTally Conditionals" );
		CUtil::CountTally(line, cmplx_cond_list, cnt, 1, exclude, "", "", &result->cmplx_cond_count, casesensitive);
		result->cmplx_cond_lines += cnt;

		// logical operators
		cnt = 0;
		SAVE_TO_2( "CUtil::CountTally Logical" );
		CUtil::CountTally(line, cmplx_logic_list, cnt, 1, exclude, "", "", &result->cmplx_logic_count, casesensitive);
		result->cmplx_logic_lines += cnt;

		// preprocessor directives
		cnt = 0;
		SAVE_TO_2( "CUtil::CountTally Preprocessor" );
		CUtil::CountTally(line, cmplx_preproc_list, cnt, 1, exclude, "", "", &result->cmplx_preproc_count, casesensitive);
		result->cmplx_preproc_lines += cnt;

		// assignments
		cnt = 0;
		SAVE_TO_2( "CUtil::CountTally Assignments" );
		CUtil::CountTally(line, cmplx_assign_list, cnt, 1, exclude, "", "", &result->cmplx_assign_count, casesensitive);
		result->cmplx_assign_lines += cnt;

		// pointers
		cnt = 0;
		SAVE_TO_2( "CUtil::CountTally Pointers" );
		// Pointers are embedded syntax so there is NO exclude string or include strings
		CUtil::CountTally(line, cmplx_pointer_list, cnt, 1, "", "", "", &result->cmplx_pointer_count, casesensitive);
		result->cmplx_pointer_lines += cnt;
		//AddTraceLine( __LINE__, UCC_FUNC_NAME, fit->line );    // Modified: 2015.12
		
		// cyclomatic complexity
		if (process_cyclomatic_complexity)  //added in version 2013.04
		{
			// search for cyclomatic complexity keywords
			SAVE_TO_2( "CUtil::CountTally Cyclomatic" );
			CUtil::CountTally(line, cmplx_cyclomatic_list, cyclomatic_cnt, 1, exclude, "", "", 0, casesensitive);

			SAVE_TO_2( "CUtil::CountTally Cyclomatic switch" );
			CUtil::CountTally(line, cmplx_cyclomatic_switch_list, cyclomatic_switch_cnt, 1, exclude, "", "", 0, casesensitive); //added in version 2014.08

			// Allow User to control this as the RECURSIVE implementation uses extra RAM and runs SLOWER.  About 17% slower than 2014 !
			if ( cc4enable )
			{
				//////Integrate from CC4 2015:06:26
				//AddTraceLine( __LINE__, UCC_FUNC_NAME, line );    // Modified: 2015.12
				SAVE_TO_2( "CUtil::CountDistinctCond" );
				CUtil::CountDistinctCond(cc4_valid_if, line, cmplx_cyclomatic_list,cyclomatic_repeated_cond_cnt , 1, exclude, "", "", cyclomatic_distinct_cond_set, 0, casesensitive);

				SAVE_TO_2( "CUtil::SemanticDeduplicate" );
				cc4_nested_dup += CUtil::SemanticDeduplicate(cc4_valid_if, cc4_parent_stack, cyclomatic_distinct_cond_stack, nested_set);

				//std::ostringstream ss;
				//ss << "cc4_nested_dup " << cc4_nested_dup;
				//AddTraceLine( __LINE__, UCC_FUNC_NAME, ss.str() );    // Modified: 2015.12
				///////////////
			}  //  END if ( cc4enable )

			// search for keywords to exclude
			if (ignore_cmplx_cyclomatic_list.size() > 0)
			{
				SAVE_TO_2( "CUtil::CountTally Exclude Keywords" );
				CUtil::CountTally(line, ignore_cmplx_cyclomatic_list, ignore_cyclomatic_cnt, 1, exclude, "", "", 0, casesensitive);
			}

			// search for cyclomatic complexity logical keywords
			if (cmplx_cyclomatic_logic_list.size() > 0)
			{
				SAVE_TO_2( "CUtil::CountTally Cyclomatic Complexity Logical Keywords" );
				CUtil::CountTally(line, cmplx_cyclomatic_logic_list, cyclomatic_logic_cnt, 1, exclude, "", "", 0, casesensitive);
			}

			// search for cyclomatic complexity case keywords
			if (cmplx_cyclomatic_case_list.size() > 0)
			{
				SAVE_TO_2( "CUtil::CountTally Cyclomatic Complexity Case Keywords" );
				CUtil::CountTally(line, cmplx_cyclomatic_case_list, cyclomatic_case_cnt, 1, exclude, "", "", 0, casesensitive);
			}

			// search for cyclomatic complexity case default keywords
			if (cmplx_cyclomatic_default_list.size() > 0)
			{
				SAVE_TO_2( "CUtil::CountTally Cyclomatic Complexity Case Default Keywords" );
				CUtil::CountTally(line, cmplx_cyclomatic_default_list, cyclomatic_default_cnt, 1, exclude, "", "", 0, casesensitive);
			}

			if(cyclomatic_default_cnt > 0)
			{
				cyclomatic_cnt -= cyclomatic_default_cnt;
				cyclomatic_case_cnt -= cyclomatic_default_cnt;
				cyclomatic_default_cnt = 0;
			}

			// parse function name if found
			SAVE_TO_2( "ParseFunctionName" );
			ret = (unsigned int)ParseFunctionName(line, lastline, function_stack, function_name, function_count);  //added in version 2013.04
			if (ret != 1 && !cyclomatic_stack.empty() && cyclomatic_stack.size() == function_stack.size())
			{
				// remove count stack entry for non-function names
				cyclomatic_cnt += cyclomatic_stack.top();
				ignore_cyclomatic_cnt = 0;
				cyclomatic_stack.pop();
			}
			if (ret != 1 && !cyclomatic_logic_stack.empty() && cyclomatic_logic_stack.size() == function_stack.size())  //added in version 2014.08
			{
				// remove count stack entry for non-function names
				cyclomatic_logic_cnt += cyclomatic_logic_stack.top();
				cyclomatic_logic_stack.pop();
			}
			if (ret != 1 && !cyclomatic_case_stack.empty() && cyclomatic_case_stack.size() == function_stack.size()) //added in version 2014.08
			{
				// remove count stack entry for non-function names
				cyclomatic_case_cnt += cyclomatic_case_stack.top();
				cyclomatic_case_stack.pop();
			}

			if ( cc4enable )
			{
				///////////Integrate from CC4 2015:06:26
				if (ret != 1 && !cyclomatic_distinct_cond_stack.empty() && cyclomatic_distinct_cond_stack.size() == function_stack.size())
				{
					// remove count stack entry for non-function names

					set<string> temp_set = cyclomatic_distinct_cond_stack.top();
					if (temp_set.size() > 0){
						//cout << "-----------line796"<<endl;
						cyclomatic_distinct_cond_set.insert(temp_set.begin(), temp_set.end());
					}

					cyclomatic_distinct_cond_stack.pop();

					cc4_parent_stack.pop();

					/***   COMMENTED OUT as AddTraceLine is not to be used
					std::ostringstream ss;
					ss << "cc4_parent_stack pop" << cc4_valid_if;
					AddTraceLine( __LINE__, UCC_FUNC_NAME, ss.str() );    // Modified: 2015.12
					************  END of COMMENTED OUT  */

					// this is the end of a if(){}, nice place to perform NestIfDup

					//cout << "------safe to combine if in same level. ready to accept a new same level"<<endl;
					//cout << "------stack size="<<cyclomatic_distinct_cond_stack.size()<<endl;
					//cout << "------stack top set size="<< cyclomatic_distinct_cond_set.size()<<endl;
				}
				/////////////////////
			}

			if (ret == 1)  //added in version 2013.04
			{
				// capture count at end of function
				SAVE_TO_2( "capture count at end of function" );
				lineElement element(cyclomatic_cnt - ignore_cyclomatic_cnt + 1, function_name);
				function_map[function_count] = element;
				
				lineElement n_element(cyclomatic_cnt - ignore_cyclomatic_cnt + cyclomatic_logic_cnt + 1, function_name); //added in version 2014.08
				logical_map[function_count] = n_element;

				lineElement c_element(cyclomatic_cnt - ignore_cyclomatic_cnt - cyclomatic_case_cnt + cyclomatic_switch_cnt + 1, function_name);
				case_map[function_count] = c_element;

				if ( cc4enable )
				{
					//////////Integrate from CC4 2015:06:26
					int cc4_counter=0;
					set<string>::iterator it;
					string temp;
					for(it=cyclomatic_distinct_cond_set.begin(); it!=cyclomatic_distinct_cond_set.end(); it++){
						temp = *it;
						if(temp.find("&&")==string::npos){
							cc4_counter++;
						}else{
							cc4_counter+=CUtil::CountNestedNum(temp);

						}
					}

					/***   COMMENTED OUT as AddTraceLine is not to be used
					std::ostringstream ss;
					ss << "cc4_counter " << cc4_counter;
					AddTraceLine( __LINE__, UCC_FUNC_NAME, ss.str() );    // Modified: 2015.12
					ss.str("");
					ss.clear();

					ss << "cc4_nested_dup " << cc4_nested_dup;
					AddTraceLine( __LINE__, UCC_FUNC_NAME, ss.str() );    // Modified: 2015.12
					ss.str("");
					ss.clear();

					ss << "ignore_cyclomatic_cnt " << ignore_cyclomatic_cnt;
					AddTraceLine( __LINE__, UCC_FUNC_NAME, ss.str() );    // Modified: 2015.12
					ss.str("");
					ss.clear();
					************  END of COMMENTED OUT  */

					lineElement cc4_element(cc4_counter -cc4_nested_dup -ignore_cyclomatic_cnt + 1, function_name);
					
					/***   COMMENTED OUT as AddTraceLine is not to be used
					ss << "function name: " << function_name << " CC4: " << cc4_element.lineNumber;
					AddTraceLine( __LINE__, UCC_FUNC_NAME, ss.str() );    // Modified: 2015.12
					ss.str("");
					ss.clear();
					************  END of COMMENTED OUT  */
					

					nested_set.clear();
					cc4_nested_dup=0;

					cond_CC4_map[function_count] = cc4_element;
					////////////////////
				}

				if (!function_stack.empty())  //added in version 2013.04
				{
					// grab previous function from stack to continue
					if (!cyclomatic_stack.empty())
					{
						cyclomatic_cnt = cyclomatic_stack.top();
						cyclomatic_stack.pop();
					}
					if (!cyclomatic_logic_stack.empty())  //added in version 2014.08
					{
						cyclomatic_logic_cnt = cyclomatic_logic_stack.top();
						cyclomatic_logic_stack.pop();
					}
					if (!cyclomatic_case_stack.empty())  //added in version 2014.08
					{
						cyclomatic_case_cnt = cyclomatic_case_stack.top();
						cyclomatic_case_stack.pop();
					}

					if ( cc4enable )
					{
						///////Integrate from CC4 2015:06:26
						if (!cyclomatic_distinct_cond_stack.empty())
						{
							AddTraceLine( __LINE__, UCC_FUNC_NAME, "CC4: go back to leavl" );    // Modified: 2015.12
							cyclomatic_distinct_cond_set = cyclomatic_distinct_cond_stack.top();
							cyclomatic_distinct_cond_stack.pop();
						}
						////////
					}
				}
				else {
					cyclomatic_cnt = 0;
					cyclomatic_logic_cnt = 0;
					cyclomatic_case_cnt = 0;
					cyclomatic_distinct_cond_set.clear();///Integrate from CC4 2015:06:26
				}
				function_name = "";
				ignore_cyclomatic_cnt = 0;
				cyclomatic_switch_cnt = 0;
			}
			else if (ret == 2)  //added in version 2013.04
			{
				if (main_cyclomatic_cnt < 1)
					main_cyclomatic_cnt = 1;	// add 1 for main function here in case no other decision points are found in main
				// some code doesn't belong to any function
				main_cyclomatic_cnt += cyclomatic_cnt - ignore_cyclomatic_cnt;  //added in version 2014.08
				main_cyclomatic_logic_cnt += cyclomatic_cnt - ignore_cyclomatic_cnt + cyclomatic_logic_cnt;
				main_cyclomatic_case_cnt += cyclomatic_cnt - ignore_cyclomatic_cnt - cyclomatic_case_cnt + cyclomatic_switch_cnt;
				if ( cc4enable )
				{
					main_cyclomatic_distinct_cond_cnt += cyclomatic_distinct_cond_set.size() - ignore_cyclomatic_cnt;/////Integrate from CC4 2015:06:26
					cyclomatic_distinct_cond_set.clear();/////Integrate from CC4 2015:06:26
				}
				cyclomatic_cnt = ignore_cyclomatic_cnt = cyclomatic_logic_cnt = cyclomatic_case_cnt = cyclomatic_switch_cnt = 0;
			}
			else { 
				if (!function_stack.empty() && (function_stack.size() > cyclomatic_stack.size() + 1 || (cyclomatic_stack.empty() && function_stack.size() > 1)))  //added in version 2013.04
				{
					// capture previous complexity count from open function
					cyclomatic_stack.push(cyclomatic_cnt - ignore_cyclomatic_cnt);
					cyclomatic_cnt = ignore_cyclomatic_cnt = 0;
				}
				if (!function_stack.empty() && (function_stack.size() > cyclomatic_logic_stack.size() + 1 || (cyclomatic_logic_stack.empty() && function_stack.size() > 1)))  //added in version 2014.08
				{
					// capture previous complexity count from open function
					cyclomatic_logic_stack.push(cyclomatic_logic_cnt);
					cyclomatic_logic_cnt = 0;
				}
				if (!function_stack.empty() && (function_stack.size() > cyclomatic_case_stack.size() + 1 || (cyclomatic_case_stack.empty() && function_stack.size() > 1)))  //added in version 2014.08
				{
					// capture previous complexity count from open function
					cyclomatic_case_stack.push(cyclomatic_case_cnt - cyclomatic_switch_cnt);
					cyclomatic_case_cnt = 0;
					cyclomatic_switch_cnt = 0;
				}

				if ( cc4enable )
				{
					////Integrate from CC4 2015:06:26
					if (!function_stack.empty() && (function_stack.size() > cyclomatic_distinct_cond_stack.size() + 1 || (cyclomatic_distinct_cond_stack.empty() && function_stack.size() > 1)))
					{
						// capture previous complexity count from open function
						cyclomatic_distinct_cond_stack.push(cyclomatic_distinct_cond_set);
						cyclomatic_distinct_cond_set.clear();
						std::ostringstream ss;
						cc4_parent_stack.push(cc4_valid_if);
						ss << "cc4_parent_stack push " << cc4_valid_if;
						AddTraceLine( __LINE__, UCC_FUNC_NAME, ss.str() );    // Modified: 2015.12
						ss.str("");
						ss.clear();
					}
					/////////////////
				}
			}
		}
	}

	// done with a file
	SAVE_TO_2( "CCodeCounter::CountComplexity done with a file" );
	if (main_cyclomatic_cnt > 0)  //added in version 2013.04
	{
		// add "main" code
		lineElement element(main_cyclomatic_cnt, "main");
		lineElement n_element(main_cyclomatic_logic_cnt, "main"); //added in version 2014.08
		lineElement c_element(main_cyclomatic_case_cnt, "main");
		
		function_map[0] = element;  //added in version 2013.04
		logical_map[0] = n_element; //added in version 2014.08
		case_map[0] = c_element;
		if ( cc4enable )
		{
			lineElement cc4_element(main_cyclomatic_distinct_cond_cnt, "main"); //Integrate from CC4 2015:06:26
			cond_CC4_map[0] = cc4_element;  //Integrate from CC4 2015:06:26
		}
	}
	else  //added in version 2013.04
	{
		// finish the first function if not closed
		SAVE_TO_2( "CCodeCounter::CountComplexity finish the first function if not closed" );
		while (!function_stack.empty())  //added in version 2013.04
		{
			function_name = function_stack.back().line;
			function_count = function_stack.back().lineNumber;
			function_stack.pop_back();

			if (!function_stack.empty())  //added in version 2013.04
			{
				// grab previous function from stack to continue
				if (!cyclomatic_stack.empty())
				{
					cyclomatic_cnt = cyclomatic_stack.top();
					cyclomatic_stack.pop();
				}

				if ( cc4enable )
				{
					/////Integrate from CC4 2015:06:26
					if (!cyclomatic_distinct_cond_stack.empty())
					{
						set<string> temp_set = cyclomatic_distinct_cond_stack.top();
						cyclomatic_distinct_cond_set.insert(temp_set.begin(), temp_set.end());
						cyclomatic_distinct_cond_stack.pop();
					}
					////////
				}
			}
			else  //added in version 2013.04
			{
				// capture count at end of function
				lineElement element(cyclomatic_cnt - ignore_cyclomatic_cnt + 1, function_name); //added in version 2014.08
				lineElement n_element(cyclomatic_cnt - ignore_cyclomatic_cnt + cyclomatic_logic_cnt + 1, function_name);
				lineElement c_element(cyclomatic_cnt - ignore_cyclomatic_cnt - cyclomatic_case_cnt + cyclomatic_switch_cnt + 1, function_name);
				
				function_map[function_count] = element;
				logical_map[function_count] = n_element;
				case_map[function_count] = c_element;

				if ( cc4enable )
				{
					lineElement cc4_element(cyclomatic_distinct_cond_set.size() -cc4_nested_dup- ignore_cyclomatic_cnt + 1, function_name);  //Integrate from CC4 2015:06:26
					cond_CC4_map[function_count] = cc4_element; //Integrate from CC4 2015:06:26
				}
			}
		}
	}

	// process ordered functions
	SAVE_TO_2( "CCodeCounter::CountComplexity process ordered functions" );
	for (map<unsigned int, lineElement>::iterator it = function_map.begin(); it != function_map.end(); ++it) //added in version 2013.04
		result->cmplx_cycfunct_count.push_back(it->second);
	if(cmplx_cyclomatic_logic_list.size() > 0)  //added in version 2014.08
	{
		for (map<unsigned int, lineElement>::iterator it = logical_map.begin(); it != logical_map.end(); ++it)
			result->cmplx_cycfunct_CC2_count.push_back(it->second);
	}
	if(cmplx_cyclomatic_case_list.size() > 0)
	{
		for (map<unsigned int, lineElement>::iterator it = case_map.begin(); it != case_map.end(); ++it)
			result->cmplx_cycfunct_CC3_count.push_back(it->second);
	}

	if ( cc4enable )
	{
		/////Integrate from CC4 2015:06:26
		if (cmplx_cyclomatic_list.size() > 0)
		{
			//cout << "CC4 output" << endl;
			for (map<unsigned int, lineElement>::iterator it = cond_CC4_map.begin(); it != cond_CC4_map.end(); ++it)
			{
				//cout << it->second.line << endl << it->second.lineNumber<<endl;
				result->cmplx_cycfunct_CC4_count.push_back(it->second);
				//AddTraceLine( __LINE__, UCC_FUNC_NAME, "CC4: push into result " );    // Modified: 2015.12
			}
		}
		//////////////////
	}

	SAVE_TO_2( "Exit CCodeCounter::CountComplexity" );
	return 1;
}


/*!
* See if information is available to do Cyclomatic Complexity parsing
*
* \param result counter results
*
* \return true if OK to do Cyclomatic checks (call depths)
*/
bool CCodeCounter::CanProcessCyclomaticComplexity( const results* result )
{
	bool	process_cyclomatic_complexity = false;

	// check whether to process cyclomatic complexity
	if ( cmplx_cyclomatic_list.size() > 0 )
	{
		process_cyclomatic_complexity = true;
		if (skip_cmplx_cyclomatic_file_extension_list.size() > 0)
		{
			size_t idx = result->file_name.find_last_of(".");
			if (idx != string::npos)
			{
				string file_ext = result->file_name.substr( idx );
				file_ext = CUtil::ToLower( file_ext );
				if ( find( skip_cmplx_cyclomatic_file_extension_list.begin(), skip_cmplx_cyclomatic_file_extension_list.end(), file_ext ) != skip_cmplx_cyclomatic_file_extension_list.end() )
				{
					// Skip the extension of this file
					process_cyclomatic_complexity = false;
				}
			}
		}
	}

	return process_cyclomatic_complexity;
}


/*!
* 1.Function Description:
*   Processes physical and logical lines.
*   This method is typically implemented in the specific language sub-class.Return value is the method status
*
* 2.Parameters:
*   fmap: list of processed file lines
*   result: counter results
*   fmapBak: list of original file lines (same as fmap except it contains unmodified quoted strings)
*
* 3.Creation Time and Owner: 
*   version 2011.05
*/
int CCodeCounter::LanguageSpecificProcess(filemap* fmap, results* result, filemap* /*fmapBak*/)
{
	SAVE_TO_2( "Start CCodeCounter::LanguageSpecificProcess" );

	// Set to check blank here as that is only call in loop
	SAVE_TO_2( "CUtil::CheckBlank" );
	currentPhyLine = 0;
	for (filemap::iterator iter = fmap->begin(); iter != fmap->end(); iter++)
	{
		currentPhyLine++;
		if (!CUtil::CheckBlank(iter->line))
			result->exec_lines[PHY]++;

		AddTraceLine( __LINE__, UCC_FUNC_NAME, iter->line );    // Modified: 2015.12
	}

	SAVE_TO_2( "Exit CCodeCounter::LanguageSpecificProcess" );
	return 1;
}


//////////////////////////////////////////////////////////////////////////////
//
//			ALL below is support for 1 Pass parsing
//
//////////////////////////////////////////////////////////////////////////////
#ifdef	ENABLE_1_PASS_PARSER
/*!
* Parse values of interest and find counts in the source file.
*
* \param fmap list of file lines
* \param result counter results
* \param fmapBak list of original file lines (same as fmap except it contains unmodified quoted strings)
*
* \return method status (whatever that is)
*/
int CCodeCounter::ParseFile( filemap* fmap, results* result, filemap* fmapBak )
{
/*		Alternate Parsing Approach: 
*		--------------------------
* Ideally each Physical line in a file should be parsed just once.
* Instead of multiple passes to maybe find special keywords/symbols
* the code should just work through the Physical line once.
*
*		How to implement this new single pass parse approach?
* For each Physical line in a file:
*	Code is needed to get parts of a given Physical line so that:
*	An API like strtok would be needed (using whitespace as delimiter?)
*	Then for each piece of text found:
*	Try to pick it apart based on various special characters.
*	When no more smaller pieces can be found,
*	Search a data structure for matches of a small piece.
*	Based on the match found, set various flags, counters, etc.
*	Keep going until no more pieces of the Physical line remain.
* Loop back to parse another Physical line.
*
* What Data structure(s) are useful to support the single pass?
* 
* How about an Array of all the keywords and all the special symbols?
*	The keyword/symbol Array would be sorted so a Binary search may be used.
* 
* If the keyword/symbol Array has a match, then the Type of the match is needed.
* Consider a Type array with a 1 to 1 correspondence to the keyword array.
* The Type array would give guidance on how to proceed with parsing.
* Types may be: Math function(s), Data declarations, Directives (preprocessor), etc.
*/
	int retVal = 0;

	// check for file types (languages) that don't get parsed here
	if (classtype == UNKNOWN || classtype == DATAFILE)
		return NO_PARSE_DONE;

// DEBUG
	//print_cmplx = false; // true adds duplicate keywords such as: while

	// Set up some parse keyword search support for single pass parsing
	ENTER_1( "Start" );
	retVal = PreParseFile( result );

// Set up initial values needed across multiple lines
	unsigned int	lines_with_errors = 0;
	currentPhyLine = 0;

// COMMENTS code support
	bool check_comments = true;
	if (BlockCommentStart.empty() && LineCommentStart.empty())
		check_comments = false;

	bool contd = false;
	bool contd_nextline;
	/*
	comment_type:
	0 : not a comment
	1 : line comment, whole line
	2 : line comment, embedded
	3 : block comment, undecided
	4 : block comment, embedded
	*/
	int comment_type = 0;

	size_t idx_start, idx_end, comment_start;
	size_t quote_idx_start;
	string curBlckCmtStart, curBlckCmtEnd;
	char CurrentQuoteEnd = 0;
	bool quote_contd = false;
	
	quote_idx_start = 0;


// PARSE the file one line at a time
	filemap::iterator itfmBak = fmapBak->begin();

	for (filemap::iterator iter = fmap->begin(); iter != fmap->end(); iter++, itfmBak++)
	{
		// Keep track of current line for use in Error or other messages
		currentPhyLine++;

		// Always check for a blank line first; compare with code in CountBlankSLOC
		if ( CUtil::CheckBlank(iter->line) )
		{
			result->blank_lines++;
			continue;
		}

		result->exec_lines[PHY]++;	// Not blank, so counts as Physical line

#ifdef	_DEBUG_1_PASS_PARSE
		parse_physical_line          = iter->line;
		parse_physical_line_comments = itfmBak->line;
#endif

		if ( true == check_comments )
		{
			// Compare this with code in CountCommentsSLOC
			contd_nextline = false;

			quote_idx_start = 0;
			idx_start = 0;

			if (quote_contd)
			{
				// Replace quote until next character
				ReplaceQuote(iter->line, quote_idx_start, quote_contd, CurrentQuoteEnd);
				if (quote_contd)
				{
					parse_physical_line          = iter->line;
					parse_physical_line_comments = itfmBak->line;
					continue;		// OK to continue here as rest is in a Comment
				}
			}

			int comment_error = CountCommentsSLOCperLine( iter,
											result,
											itfmBak,
											contd, 
											comment_type,
											contd_nextline,
											idx_start,
											idx_end,
											quote_idx_start,
											comment_start,
											curBlckCmtStart,
											curBlckCmtEnd,
											quote_contd,
											CurrentQuoteEnd );
			if ( comment_error )
				lines_with_errors++;

		}	// END of processing Comments

		// Here we know this Line is not Blank and any Comments were handled
		// Line could now be an Empty string from Comments handling
		if ( 0 < iter->line.size() )
		{
			parse_physical_line          = iter->line;
			parse_physical_line_comments = itfmBak->line;
			int parse_error = ParseLine( currentPhyLine, iter, result, itfmBak );

			if ( parse_error )
				lines_with_errors++;
		}
	}

	//if ( lines_with_errors )

	return retVal;
}


/*!
* Populate data structures needed by ParseFile 
*
* \param		result counter results
* \class global	parse_alloc_count
* \class global pe_keyword_max_size
* \class global	pe_op_max_size
* \class global pe
* \class global	pe_max_idx
* \return		0 = no errors, negative = severe errors, positive = recoverable errors
*/
int CCodeCounter::PreParseFile( const results* result )
{
	int retVal = 0;

	if ( 0 < parse_alloc_count )
		return retVal;			// already done

	ENTER_2( "Start" );
	bool process_cyclomatic_complexity = false;

	// Find total number of parse helper structs to allocate
	parse_alloc_count = 0;
	pe_keyword_max_size = 0;	// This will also cover op max size
	pe_op_max_size = 0;			// for Calc, Logic, Assign, Pointer

	parse_alloc_count += BlockCommentStart.size();
	parse_alloc_count += BlockCommentEnd.size();
	parse_alloc_count += LineCommentStart.size();
	parse_alloc_count += directive.size();
	parse_alloc_count += data_name_list.size();
	parse_alloc_count += exec_name_list.size();
	parse_alloc_count += math_func_list.size();
	parse_alloc_count += trig_func_list.size();
	parse_alloc_count += log_func_list.size();

	if ( true == print_cmplx )
	{
		parse_alloc_count += cmplx_calc_list.size();
		// parse_alloc_count += cmplx_cond_list.size();			done after sort
		parse_alloc_count += cmplx_logic_list.size();
		// parse_alloc_count += cmplx_preproc_list.size();		done after sort
		parse_alloc_count += cmplx_assign_list.size();
		parse_alloc_count += cmplx_pointer_list.size();

		// check whether to process cyclomatic complexity
		if ( 0 < aux_meaning_keys.size() )
		{
			process_cyclomatic_complexity = CanProcessCyclomaticComplexity( result );
			if ( true == process_cyclomatic_complexity )
			{
				// DO NOT add to parse list!
				// The auxillary meanings will be done after parse list sort
				// parse_alloc_count += cmplx_cyclomatic_list.size();
			}
		}
	}

	// Allocate the array of parse helper structures
	pe = (parseElement *) new parseElement[ parse_alloc_count ];

	if ( (parseElement *) NULL == pe )
		return -1;		// Memory allocation error

	// Populate the parse element array
	pe_max_idx = 0;
	unsigned int empty_string_count = 0;

	AddToParse( pe_max_idx, pe_keyword_max_size, empty_string_count, BLK_COMMENT_START, BlockCommentStart );
	AddToParse( pe_max_idx, pe_keyword_max_size, empty_string_count, BLK_COMMENT_END, BlockCommentEnd );
	AddToParse( pe_max_idx, pe_keyword_max_size, empty_string_count, LINE_COMMENT_START, LineCommentStart );
	AddToParse( pe_max_idx, pe_keyword_max_size, empty_string_count, DIRECTIVE, directive );
	AddToParse( pe_max_idx, pe_keyword_max_size, empty_string_count, DATA_NAME, data_name_list );
	AddToParse( pe_max_idx, pe_keyword_max_size, empty_string_count, EXEC_NAME, exec_name_list );
	AddToParse( pe_max_idx, pe_keyword_max_size, empty_string_count, MATH_FUNC, math_func_list );
	AddToParse( pe_max_idx, pe_keyword_max_size, empty_string_count, TRIG_FUNC, trig_func_list );
	AddToParse( pe_max_idx, pe_keyword_max_size, empty_string_count, LOG_FUNC, log_func_list );

	if ( true == print_cmplx )
	{
		AddToParse( pe_max_idx, pe_op_max_size, empty_string_count, CMPLX_CALC, cmplx_calc_list );
		//AddToParse( pe_max_idx, pe_keyword_max_size, empty_string_count, CMPLX_COND, cmplx_cond_list );			done after sort
		AddToParse( pe_max_idx, pe_op_max_size, empty_string_count, CMPLX_LOGIC, cmplx_logic_list );
		// AddToParse( pe_max_idx, pe_keyword_max_size, empty_string_count, CMPLX_PREPROC, cmplx_preproc_list );	done after sort
		AddToParse( pe_max_idx, pe_op_max_size, empty_string_count, CMPLX_ASSIGN, cmplx_assign_list );
		AddToParse( pe_max_idx, pe_op_max_size, empty_string_count, CMPLX_POINTER, cmplx_pointer_list );
		
		if ( true == process_cyclomatic_complexity )
		{
			// DO NOT add these to the parse list!  Auxillary meanings is now used.
			// AddToParse( pe_max_idx, pe_keyword_max_size, empty_string_count, CMPLX_CYCLOMATIC, cmplx_cyclomatic_list );
		}
	}

	if ( pe_keyword_max_size < pe_op_max_size )
		pe_keyword_max_size = pe_op_max_size;

	// NOTE: If there were any empty strings the pe_max_idx value is less than the parse_alloc_count.

// SORT
	// the array by keyword name or symbol and then by parse type if same name
	bool			swap_needed = false;
	unsigned int	dup_keywords = 0;
	unsigned int	total_swaps = 0;

	// Temporary variables for swap kept outside loops
	ParseType   type = PARSE_UNKNOWN;
	string		keyword = "";
	unsigned int idx = 0;

	for ( unsigned int i = 0; i < pe_max_idx; i++ )
	{
		for ( unsigned int j = 0; j < pe_max_idx; j++ )
		{
			if ( i == j )
				continue;

			if ( pe[i].keyword <= pe[j].keyword )
			{
				swap_needed = true;
				if ( pe[i].keyword == pe[j].keyword )
				{
					// In the 1 pass approach, this is a coding table definition error
					pe_dup_keywords = true;
					cout << endl << "Error: PreParseFile given duplicate Keyword " << pe[i].keyword << endl;

					// NOT according to design but allow continuing to maybe find other coding errors.
					// Keywords same, sort by type
					dup_keywords++;
					if ( pe[i].type > pe[j].type )
					{
						swap_needed = false;
					}
				}

				if ( true == swap_needed )
				{
					type    = pe[i].type;
					keyword = pe[i].keyword;
					idx     = pe[i].idx;

					pe[i].type    = pe[j].type;
					pe[i].keyword = pe[j].keyword;
					pe[i].idx     = pe[j].idx;

					pe[j].type    = type;
					pe[j].keyword = keyword;
					pe[j].idx     = idx;
					total_swaps++;
				}
			}
		}
	}

	// Set up for successful searching of parse elements
	pe_mid_idx = pe_max_idx / 2;
	pe_mid_keyword = pe[ pe_mid_idx ].keyword;

	if ( 0 != empty_string_count )
		retVal = (int) empty_string_count;

	if ( true == process_cyclomatic_complexity )
	{
		// Keep keywords (or operators) unique in the parse element array by
		// setting extra meaning bit flags (and sometimes indexes) as needed.
		if ( aux_meaning_keys.size() )
		{
			// Put in values to support Cyclomatic Complexity rings 1 to 4
			SetInPe( aux_meaning_keys, aux_meaning_vals, false );
		}

		if ( cmplx_cond_list.size() )
		{
			UIntVector cmplx_cond_vars;
			for ( unsigned int k = 0; k < cmplx_cond_list.size(); k++ )
				cmplx_cond_vars.push_back( COMPLEX_CALC );

			SetInPe( cmplx_cond_list, cmplx_cond_vars, true );
		}

		if ( cmplx_preproc_list.size() )
		{
			UIntVector cmplx_preproc_vars;
			for ( unsigned int k = 0; k < cmplx_preproc_list.size(); k++ )
				cmplx_preproc_vars.push_back( COMPLEX_PREPROC );

			SetInPe( cmplx_preproc_list, cmplx_preproc_vars, true );
		}
	}

	return retVal;
}


/*!
* Set up extra meanings in data structures needed by ParseFile 
*
*/
void CCodeCounter::SetInPe( StringVector & keys, UIntVector & vals, const bool useExtraIdx )
{
	unsigned int	match_count  = 0;
	ParseType		keyword_type = PARSE_UNKNOWN;
	unsigned int	idx          = 0;
	unsigned int	keys_idx     = 0;

	UIntVector::iterator itVal = vals.begin();
	for ( StringVector::iterator it = keys.begin(); it != keys.end(); it++, itVal++ )
	{
		match_count  = 0;
		keyword_type = PARSE_UNKNOWN;
		idx          = 0;
		int idx_found = FindInPe( *it, match_count, keyword_type, idx );
		if ( 0 > idx_found )
		{
			// This is a UCC source file editing error.  Every entry in keys should be found!
			cout << endl << "Error: keyword or operator not found " << *it << endl;
			keys_idx++;
			continue;
		}

		if ( 1 != match_count )
		{
			// This is a UCC source file editing error.  Match count should only be 1.
			// Set breakpoint at start of this and see the 2 or more places where the keyword is declared.
			// Resolve by using aux_meanings  or extra_idx or similar.
			cout << endl << "Error: Parse element; " << *it << " has more than 1 match.  Wrong set up." << endl;
		}

		// Assign special meanings as needed
		pe[ idx_found ].aux |= *itVal;

		if ( useExtraIdx )
		{
			// Set extra index to be index of the provided keys
			// The bitmask ORed in above will indicate to use this extra index
			pe[ idx_found ].extra_idx = keys_idx;
		}

		keys_idx++;
	}

	return;
}


/*!
* Set up data structures (already in this class) needed by ParseFile 
*
* \param index,					IN/OUT beginning position in the array to change
* \param max_str_size,			IN/OUT size of longest string in pe array
* \param empty_string_count,	IN/OUT number of empty strings found (should be Zero)
* \param type,					IN     enum of what is the type of these parse keywords
* \param sv,					IN     a string vector with 1 or more keywords that will be added
*/
void CCodeCounter::AddToParse( unsigned int	& index, 
								unsigned int &	max_str_size,
								unsigned int &	empty_string_count, 
								const ParseType		type,
								StringVector	sv )
{
	unsigned int originating_index = 0;		// index of the sv array

	for ( StringVector::iterator it = sv.begin(); it != sv.end(); it++, originating_index++ )
	{
		if ( "" != *it )
		{
			pe[ index ].keyword   = *it;
			pe[ index ].type      = type;
			pe[ index ].idx       = originating_index;
			pe[ index ].aux       = 0;
			pe[ index ].extra_idx = 0;

			if ( max_str_size < pe[ index ].keyword.size() )
				max_str_size = pe[ index ].keyword.size();

			index++;
		}
		else
		{
			// Found an empty string where it should NOT be
			// This is a coding table error.
			cout << endl << "Error: Empty string given to AddToParse()" << endl;
			empty_string_count++;
		}
	}
}

/*!
* See if a string value is in an array of well known keywords
*
* Precondition: parse element array is already in sorted order
*
* \param value,			IN	is this value a well known keyword ?
* \param match_count,	OUT number of same keywords found
* \param type_found,	OUT type of parse keyword
* \param original_idx,	OUT index of the name array holding the keyword
* \returns index value of first match or NO_MATCH (-1)
*/
int CCodeCounter::FindInPe( const string value, unsigned int & match_count, 
						ParseType & type_found, unsigned int & original_idx )
{
	int retVal = NO_MATCH;
	match_count = 0;

	if ( pe_keyword_max_size < value.size() )
		return NO_MATCH;

	// This uses a simple Linear Search as the number of array elements is not large (< 150?)
	//
	// We can plug in a Binary Search later if we can see a difference in time
	//

	// Limit search to half of the array 
	int cmp_result = pe[ pe_mid_idx ].keyword.compare( value );
	if ( 0 == cmp_result )
		retVal = (int) pe_mid_idx;		// found
	else
	{
		unsigned int start = 0;
		unsigned int end = pe_mid_idx;
		if (  -1 == cmp_result )
		{
			start = pe_mid_idx;
			end = pe_max_idx - 1;
		}

		// Linear Search 
		// with check for finding a array keyword above value
		for ( unsigned int j = start; j <= end; j++ )
		{
			cmp_result = pe[ j ].keyword.compare( value );
			if ( cmp_result >= 0 )
			{
				// Either went too far or found it
				if ( 0 == cmp_result )
					retVal = (int)j;
				break;
			}
		}
	}

	if ( NO_MATCH != retVal ) 
	{
		match_count = 1;
		if ( true == pe_dup_keywords )
		{
		// This is considered an ERROR as duplicate entries happened 
		// rather than using extra decision/attribute/meaning flags and offsets
			match_count++;
		}
		type_found   = pe[ retVal ].type;
		original_idx = pe[ retVal ].idx;
	}

	return retVal;
}

/*!
* See if first part of a string value is in an array of well known keywords
*
* \param value,			IN	is the first part of this value a well known keyword ?
* \param match_count,	OUT number of same keywords found
* \param type_found,	OUT type of parse keyword
* \param original_idx,	OUT index of the name array holding the keyword
* \returns index value of first match or NO_MATCH (-1)
*/
int CCodeCounter::FindSpecialInPe( const string value, unsigned int & match_count, 
								ParseType & type_found, unsigned int & original_idx, unsigned int & size_found )
{
	int retVal = NO_MATCH;

	// This is a helper to see if the first few characters of a given string
	// match a well known keyword.

	// Start with longest value and decrease to 0 size
	size_t size = value.size();
	if ( size > pe_op_max_size )
		size = pe_op_max_size;
	do
	{
		string to_find = value.substr( 0, size );

		int pe_idx = FindInPe( to_find, match_count, type_found, original_idx );

		if ( match_count )
		{
			size_found = size;
			retVal = pe_idx;
			break;
		}

		size--;
	} while ( size > 0 );

	return retVal;
}

/*!
* Find possible smaller strings in a string based on given separators.
* Similar to clib strtok.
*
* \param input,			IN		string to consider
* \param separators,	IN		array of separator characters between tokens
* \param tokens,		IN/OUT	list of token strings found
*
* \returns				OUT		number of tokens found, there are no error values
*/
unsigned int CCodeCounter::StrTok( const string input, const char * separators, StringVector & tokens )
{
	unsigned int num_found = 0;

	tokens.clear();
	if ( 0 == input.size() )
	{
		// May be an error in the caller.
		cout << endl << "StrTok called to process Empty string" << endl;
		return 0;
	}

	size_t idx_start, idx_end;
	string frag;
	string working = input;
	
	for ( ; ; )
	{
		idx_start = working.find_first_not_of( separators );
		if ( idx_start == string::npos )
			break;
		
		if ( 0 < idx_start )
		{
			working = working.substr( idx_start );
			idx_start = 0;
		}
		idx_end = working.find_first_of( separators );

		frag = working.substr( idx_start, idx_end );
		tokens.push_back( frag );
		num_found++;
		
		if ( idx_end == string::npos )
			break;

		idx_start = idx_end;
		working = working.substr( idx_start );
	}
	return num_found;
}

/*!
* Get separate tokens (some have syntax meanings) from a given source line.
* The syntax meanings are like the results of a first pass Lexical scan.
* 
* \param input,				IN		string to consider
* \param unchanged,			IN		original string with Quoted string literals intact
* \param parse_elements,	IN/OUT	list of partially parsed token strings found
*
* \returns					OUT		number of partially parsed token found, there are no error values
*/
unsigned int CCodeCounter::GetTokens( const string input, const string unchanged, ParseElementVector & parse_elements )
{
	unsigned int	token_count_total = 0;
	unsigned int	idx_end;
	StringVector	my_tokens;
	string			frag;
	string			quote_start;
	string			quote_end;
	string			temp;
	parseElement	temp_parse;
	size_t			idx_start;
	char			buf[128];

	parse_elements.clear();

// Define fixed arrays of chars that have specific meanings	
#define	WHITESPACE	" \t\n\r\f"

// Calculation operations characters
//#define CALC_OPS	"%^*+-/<>"

// Logical operations characters
//#define LOGIC_OPS	"&|=!<>"

// These are in the Calculations list and the Logic list
//#define AMBIG_OPS	"<>"

// Any character NOT in the below means the token is NOT a single ID but may have other parts
#define ID_CHARS	"ABCDEFGHIJKLMNOPQRSTUVWXYZ_$abcdefghijklmnopqrstuvwxyz0123456789"

// Characters that could represent numbers (Scala)
#define NUMERIC_CHARS	"+-ABCDEFabcdefLlXx0123456789."

	// First get (possibly compound) tokens using whitespace as the separators.
	// When vector string is returned there will be no embedded white space in a token.
	unsigned int token_count = StrTok( input, WHITESPACE, my_tokens );
	if ( token_count != my_tokens.size() )
	{
	#ifdef	_DEBUG_1_PASS_PARSE
		cout << endl << "ERROR: StrTok count mismatch. Using actual size." << endl;
	#endif
		token_count = my_tokens.size();
	}

	unsigned int parse_pos = 0;		// Keep track of parse position to recover Literal strings

	for ( unsigned int j = 0; j < token_count; j++ )
	{
		// Observation: there are no number digit values as tokens in the pe array.
		// So if there are any embedded numeric digits then 
		// it may be an identifier like a variable, procedure, or class name, etc.
		frag = my_tokens[ j ];
		do
		{	// Keep going until all of this fragment has been parsed
			temp_parse.keyword = frag;
			temp_parse.idx     = 0;
			temp_parse.type    = PARSE_UNKNOWN;

		// See if this is a composite fragment with more than 1 kind of parse type

			// Look for characters not found in a Number
			idx_start = frag.find_first_not_of( NUMERIC_CHARS );
			if ( 0 < idx_start )
			{
				bool	is_long = false;
				size_t	last_pos = idx_start - 1;
				if ( idx_start == string::npos )
					last_pos = (size_t)(frag.size() - 1);

				// fragment may be a number
				// Check if last character is L or l
				if ( ( 'L' == frag[ last_pos ] )
				  || ( 'l' == frag[ last_pos ] ) )
					is_long = true;

				// Check to see if this is a number
				if ( last_pos + 1 < ( sizeof( buf )/sizeof( buf[0] ) ) )
				{
					memset( buf, 0, sizeof( buf ) );
					frag.copy( buf, last_pos + 1 );
					char * p_afterwards;
					errno = 0;
					// Try converting string to double
					strtod( buf, &p_afterwards );
					if ( 0 == errno )
					{
						if ( p_afterwards != &( buf[0] ) )
						{
							if ( true == is_long )
								temp_parse.type = NUMBER_LONG;
							else
								temp_parse.type = NUMBER_PART;

							temp_parse.keyword = frag.substr( 0, last_pos + 1 );
							parse_elements.push_back( temp_parse );
							parse_pos += temp_parse.keyword.size();
							frag = frag.substr( last_pos + 1 );
							token_count_total++;
							continue;
						}
					}
				}
			}

			// Look for characters not found in an Identifier
			idx_start = frag.find_first_not_of( ID_CHARS );
			if ( idx_start == string::npos )
			{
				// Check for Scala Wildcard (match any found) _ Underline
				if ( ( 1 == frag.size() )
				  && ( "_" == frag ) )
				{
					temp_parse.type = WILDCARD;
					idx_start = 0;
				}
				else
				{
				// NO characters that did not belong in an ID (variable, procedure, class name)
				// OR this may be a well known token.
				// In either case, this token string is ready for higher level processing
					if ( PARSE_UNKNOWN == temp_parse.type )
						temp_parse.type = IDENTIFIER; // May be found to be well know token later.

					parse_elements.push_back( temp_parse );
					parse_pos += temp_parse.keyword.size();
					frag.erase();
					token_count_total++;
					continue;
				}
			}

			// Not a simple identifier so separate into parts.  
			// idx_start gives the position of the first non ID character
			if ( 0 < idx_start )
			{
				// The string up to idx_start may be a useable ID token.  See comments above
				// Put the leading part in the output
				temp_parse.keyword = frag.substr( 0, idx_start );
				temp_parse.type = IDENTIFIER; // May be found to be well know token later.
				parse_elements.push_back( temp_parse );
				parse_pos += temp_parse.keyword.size();
				token_count_total++;

				// Trim the saved leading part and fall through
				frag = frag.substr( idx_start );
				idx_start = 0;
				temp_parse.type = PARSE_UNKNOWN;
			}

			// Check what the first character is.
			// These are hard coded now but could be provided by language specific strings
			temp = frag.substr( 0, 1 );
			bool keyword_set = false;
			switch ( frag[0] )
			{
				// check for Grouping symbols
				case '{':
					temp_parse.type = OPEN_CURLY_BRACE;
					break;
				case '}':
					temp_parse.type = CLOSE_CURLY_BRACE;
					break;
				case '(':
					temp_parse.type = OPEN_PAREN;
					break;
				case ')':
					temp_parse.type = CLOSE_PAREN;
					break;
				case '[':
					temp_parse.type = OPEN_SQUARE_BRACKET;
					break;
				case ']':
					temp_parse.type = CLOSE_SQUARE_BRACKET;
					break;

				// check for other symbols of interest
				case '_':		// Underline  Wildcard (match any found) in Scala
					// Do nothing here as would be handled above if applicable
					break;
				case ';':		// Semicolon
					temp_parse.type = LOGICAL_END;
					break;

				case '.':		// Period or dot
					temp_parse.type = SCOPE_RESOLUTION;
					break;
				case ',':		// Comma
					temp_parse.type = DELIM_COMMA;
					break;
				case ':':		// Colon
					if ( 1 < frag.size() )
					{
						if ( ':' == frag[1] )
						{
							temp_parse.type = DOUBLE_COLON;
							temp_parse.keyword = "::";
							keyword_set = true;
						}
					}
					
					if ( PARSE_UNKNOWN == temp_parse.type )
						temp_parse.type = TYPE_PREFIX_COLON;
					break;
				
				// Double or Single Quote mark characters
				case '"':
					// TODO:  Fix this to use flag set from previous line parsing
					// about multiline string literals
					if ( ( 0 < QuoteMultiStart.size() )
					  && ( QuoteMultiStart.size() <= frag.size() ) )
					{
						// See if multiline Start or End
						quote_start = QuoteMultiStart;
						quote_end   = QuoteMultiEnd;
						int cmp_result = quote_start.compare( frag.substr( 0, quote_start.size() ) );
						if ( 0 == cmp_result )
						{
							temp_parse.type = STRING_MULTILINE_START;
							if ( false == quote_multi_start_is_end )
							{
								// Look for the end on this line

							}
						}
						else
						{
							cmp_result = quote_end.compare( frag.substr( 0, quote_end.size() ) );
							if ( 0 == cmp_result )
							{
								temp_parse.type = STRING_MULTILINE_END;
								temp = QuoteMultiEnd;
							}
						}
					}
					// fall through
				case '\'':		// Single Quote mark symbol
					
					// TODO:  Improve this to also handle Python style of ''' for start/end
					if ( PARSE_UNKNOWN == temp_parse.type )
					{
						temp_parse.type = STRING_LITERAL;
						quote_start = frag.substr( 0, 1 );
						quote_end   = quote_start;
						// Get the rest of the string literal
						temp = frag.substr( 1 );
					}

					
					idx_end = temp.find_first_of( quote_end );
					if ( string::npos == (size_t)idx_end ) 
					{
						// Expected to find the following Quote mark
						// So just output an Information message and use the rest of the string
						cout << endl << "Information: Unable to find ending Quote: " + quote_end << endl;
						temp_parse.keyword = frag;
						frag.clear();
					}
					else
					{
						// Get Literal string from given unchanged string
						temp = unchanged.substr( parse_pos, parse_pos + idx_end + quote_end.size() );
						temp.resize( quote_start.size() + idx_end + quote_end.size() );
						temp_parse.keyword = temp;
						idx_end += quote_end.size() + 1;
						if ( idx_end >= frag.size() )
							frag.clear();
						else
							frag = frag.substr( idx_end );
					}

					keyword_set = true;
					break;

				default:
					// See if the first few characters are well known
					unsigned int match_count;
					ParseType	type_found;
					unsigned int original_idx;
					unsigned int size_found;

					// int pe_idx = FindSpecialInPe( frag, match_count, type_found, original_idx, size_found );
					FindSpecialInPe( frag, match_count, type_found, original_idx, size_found );
					
					if ( 0 < match_count )
					{
						temp_parse.type    = type_found;
						temp_parse.keyword = frag.substr( 0, size_found );
						temp_parse.idx     = original_idx;
						// Remove it from the start of the string
						frag = frag.substr( size_found );
						idx_start = 0;
						keyword_set = true;
					}
					else
					{
						// Show the first character of what was not done
						temp = frag.substr( 0, 1 );
						cout << endl << "GetTokens did NOT do " + temp << endl;
						
						// Remove it from the start of the string
						frag = frag.substr( 1 );
						idx_start = 0;
					}
					break;
			}

			if ( PARSE_UNKNOWN != temp_parse.type )
			{
				// Save it as a parse element
				if ( false == keyword_set )
				{
					temp_parse.keyword = frag.substr( 0, 1 );
					// Remove it from the start of the string
					frag = frag.substr( 1 );
					idx_start = 0;
				}
				
				parse_elements.push_back( temp_parse );
				parse_pos += temp_parse.keyword.size();
				token_count_total++;
			}

			if ( 0 == frag.size() )
				continue;

			// Insert any other special token parsing logic here

		} while ( 0 < frag.size() );
		
		if ( j < token_count - 1 )
		{
			// Put in a blank space between non blank tokens as some
			// languages (like Scala) use whitespace as delimiters.
			// MAY interfere when parser gets more complete or for more languages
			temp_parse.keyword = " ";
			temp_parse.idx     = 0;
			temp_parse.type    = DLIM_BLANK;
			parse_elements.push_back( temp_parse );
			parse_pos += temp_parse.keyword.size();
			token_count_total++;
		}

	}	// END loop processing tokens extracted from whitespace

	return token_count_total;
}


// Increment related count for the keyword
// Returns 0 if count updated
int CCodeCounter::UpdateKeywordCount( const ParseType type_found, 
									const unsigned int pe_idx, 
									results		*	result, 
									unsigned int &	direct_items,
									unsigned int &	data_items, 
									unsigned int &	exec_items,
									unsigned int &	math_items,
									unsigned int &	trig_items,
									unsigned int &	logarithm_items,
									unsigned int &	cmplx_calc_items,
									unsigned int &	cmplx_cond_items,
									unsigned int &	cmplx_logic_items,
									unsigned int &	cmplx_preproc_items,
									unsigned int &	cmplx_assign_items,
									unsigned int &	cmplx_pointer_items )
{
	int retVal = 0;

	unsigned int	org_idx = pe[ pe_idx ].idx;

#ifdef _MSC_VER
	// VC++ using -Wall still get the below so explicitly set to show 1 time ONLY HERE and NOT for all UCC sources
	// The switch statement has a "good enough" default handler.
	// There are many other enums in place of 'ADDRESS_OF' for the warning messages.
	#ifdef	_DEBUG
	//#pragma warning( once: 4061 ) // show 1 time only: enumerator 'ADDRESS_OF' in switch of enum 'CCodeCounter::ParseType' is not explicitly handled by a case label
		#pragma warning( disable: 4061 )
	#else
		// For Release build just totally suppress this Warning
		#pragma warning( disable: 4061 )
	#endif
#endif

	switch ( type_found )
	{
		// Comments were already handled ?
		case BLK_COMMENT_START:
		case BLK_COMMENT_END:
		case LINE_COMMENT_START:
			break;
		case DIRECTIVE:
			direct_items++;
			result->directive_count[org_idx]++;
			if ( 1 == direct_items )
				result->directive_lines[PHY]++;
			result->directive_lines[LOG]++;
			result->SLOC_lines[LOG]++;
			if ( print_cmplx )
			{
				unsigned int idx_type =  UseExtraIdx( pe[ pe_idx ].aux);
				unsigned int extra_idx = pe[ pe_idx ].extra_idx;
				if ( COMPLEX_CALC == idx_type )
				{
					cmplx_calc_items++;
					result->cmplx_calc_count[ extra_idx ]++;
				}
				else if ( COMPLEX_PREPROC == idx_type )
				{
					cmplx_preproc_items++;
					result->cmplx_preproc_count[ extra_idx ]++;
				}
				else
				{
					if ( 0 != idx_type )
						printf( "\nError: UpdateTokenCount  Unknown index type found %d\n", idx_type );
					break;
				}
			}
			break;
		case DATA_NAME:
			data_items++;
			result->data_name_count[org_idx]++;
			if ( 1 == data_items )
				result->data_lines[PHY]++;
			result->data_lines[LOG]++;
			result->SLOC_lines[LOG]++;
			if ( print_cmplx )
			{
				unsigned int idx_type =  UseExtraIdx( pe[ pe_idx ].aux);
				unsigned int extra_idx = pe[ pe_idx ].extra_idx;
				if ( COMPLEX_CALC == idx_type )
				{
					cmplx_calc_items++;
					result->cmplx_calc_count[ extra_idx ]++;
				}
				else if ( COMPLEX_PREPROC == idx_type )
				{
					cmplx_preproc_items++;
					result->cmplx_preproc_count[ extra_idx ]++;
				}
				else
				{
					if ( 0 != idx_type )
						printf( "\nError: UpdateTokenCount  Unknown index type found %d\n", idx_type );
					break;
				}
			}
			break;
		case EXEC_NAME:
			exec_items++;
			result->exec_name_count[org_idx]++;
			if ( 1 == exec_items )
				result->exec_lines[PHY]++;
			result->exec_lines[LOG]++;
			result->SLOC_lines[LOG]++;
			if ( print_cmplx )
			{
				unsigned int idx_type =  UseExtraIdx( pe[ pe_idx ].aux );
				unsigned int extra_idx = pe[ pe_idx ].extra_idx;
				if ( COMPLEX_CALC == idx_type )
				{
					cmplx_calc_items++;
					result->cmplx_calc_count[ extra_idx ]++;
				}
				else if ( COMPLEX_PREPROC == idx_type )
				{
					cmplx_preproc_items++;
					result->cmplx_preproc_count[ extra_idx ]++;
				}
				else
				{
					if ( 0 != idx_type )
						printf( "\nError: UpdateTokenCount  Unknown index type found %d\n", idx_type );
					break;
				}
			}
			break;
		case MATH_FUNC:
			math_items++;
			result->math_func_count[org_idx]++;
			break;
		case TRIG_FUNC:
			trig_items++;
			result->trig_func_count[org_idx]++;
			break;
		case LOG_FUNC:
			logarithm_items++;
			result->log_func_count[org_idx]++;
			break;
		case CMPLX_CALC:
			cmplx_calc_items++;
			result->cmplx_calc_count[org_idx]++;
			break;
		case CMPLX_COND:
			cmplx_cond_items++;
			result->cmplx_cond_count[org_idx]++;
			break;
		case CMPLX_LOGIC:
			cmplx_logic_items++;
			result->cmplx_logic_count[org_idx]++;
			break;
		case CMPLX_PREPROC:
			// from cmplx_preproc_list,		Preprocessor directives (complexity)
			cmplx_preproc_items++;
			result->cmplx_preproc_count[org_idx]++;
			break;
		case CMPLX_ASSIGN:
			// from cmplx_assign_list,		Assignments (complexity)
			cmplx_assign_items++;
			result->cmplx_assign_count[org_idx]++;
			break;
		case CMPLX_POINTER:
			// from cmplx_pointer_list,		Pointers (complexity)
			cmplx_pointer_items++;
			result->cmplx_pointer_count[org_idx]++;
			break;
		case CMPLX_CYCLOMATIC:
			// from cmplx_cyclomatic_list
			// This is processed elsewhere
			// result->cmplx_cycfunct_count[org_idx]++;
			break;
		case PARSE_UNKNOWN:
		default:
			retVal = -1;

		#ifdef	_DEBUG
			int		unk_type = type_found;
			printf( "\nERROR: UpdateTokenCount() Found unexpected type: %d\n", unk_type );
		#endif
			break;
	}

	return retVal;
}


// Here we know that this Line is not Blank and any Comments were handled
int CCodeCounter::ParseLine( const unsigned long	cur_line, 
							filemap::iterator		iter, 
							results *				result, 
							filemap::iterator		itfmBak )
{
	int		parse_error_count = 0;

	string	to_parse  = iter->line;
	string	unchanged = itfmBak->line;
	string	keyword;
	string	type_str;
	
	ParseElementVector parse_found;

	// First get (possibly compound) tokens using whitespace
	unsigned int num_tokens = GetTokens( to_parse, unchanged, parse_found );
	unsigned int token_count = parse_found.size();
	if ( num_tokens != token_count )		// DEBUG helper
	{
		printf( "\nERROR: GetTokens count mismatch\n" );
	}
		
	unsigned int direct_items = 0;
	unsigned int data_items = 0;
	unsigned int exec_items = 0;
	unsigned int math_items = 0;
	unsigned int trig_items = 0;
	unsigned int logarithm_items = 0;
	unsigned int cmplx_calc_items = 0;
	unsigned int cmplx_cond_items = 0;
	unsigned int cmplx_logic_items = 0;
	unsigned int cmplx_preproc_items = 0;
	unsigned int cmplx_assign_items = 0;
	unsigned int cmplx_pointer_items = 0;

	for ( unsigned int j = 0; j < token_count; j++ )
	{
	// Observation: there are no number digit values as tokens in the pe array.
	// So if there are any embedded numeric digits then 
	// it may be an identifier like a variable, procedure, or class name, etc.
	
		unsigned int	match_count;
		unsigned int	org_idx;
		ParseType		type_found = parse_found[ j ].type;

		if ( ( PARSE_UNKNOWN == type_found )
		  || ( IDENTIFIER == type_found ) )
		{
			keyword = parse_found[ j ].keyword;
			int pe_idx = FindInPe( keyword, match_count, type_found, org_idx );

			if ( match_count )
			{
				// Increment related count for the keyword
				int update_OK = UpdateKeywordCount( type_found, (unsigned int)pe_idx, result,
													direct_items,
													data_items, 
													exec_items,
													math_items,
													trig_items,
													logarithm_items,
													cmplx_calc_items,
													cmplx_cond_items,
													cmplx_logic_items,
													cmplx_preproc_items,
													cmplx_assign_items,
													cmplx_pointer_items );

				if ( 0 != update_OK )
				{
					// This is a failure in the parse code or the parse data structs
					// TODO: show update_OK error value and pe_idx value
					printf( "ERROR: ParseLine on line: %ld  UpdateTokenCount() Finding: %s had an ERROR.\n", cur_line, keyword.c_str() );
				}
				parse_found[ j ].idx  = org_idx;
				parse_found[ j ].type = type_found;
			}
			else
			{
				// cout << "Information: Not found: " + keyword << endl;

				//

			}
		}

	}  // END of getting token types resolved

#ifdef	_DEBUG_1_PASS_PARSE
	// DEBUG  output the tokens and types from this line
	char buf[16];
	for ( int j = 0; j < token_count; j++ )
	{
		memset( buf, 0, sizeof( buf ) );
		itoa( parse_found[ j ].type, buf, 10 );
		type_str = buf;
		if ( PARSE_UNKNOWN == parse_found[ j ].type )
			cout << endl << "ERROR: " + parse_found[ j ].keyword + " is UNKNOWN type " << endl;
		else
			cout << parse_found[ j ].keyword + " " + type_str + "  " ;
	}
	cout << endl;
#endif

	// Do other parse stuff here



	return parse_error_count;
}

#endif	//  #ifdef	ENABLE_1_PASS_PARSER

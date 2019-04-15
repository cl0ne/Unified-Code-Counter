//! Code counter class methods for the C/C++, Java, C# and Scala languages.
/*!
* \file CCJavaCsScalaCounter.cpp
*
* This file contains the code counter class methods for the C/C++, Java, C# and Scala languages.
*/

#include "UCCBeforeLibraryIncludes.h"
#include <cmath> //Modification: 2018.01
#include <iostream>
#include <sstream> //Modification: 2018.01
#include "UCCAfterLibraryIncludes.h"

#include "CCJavaCsScalaCounter.h"

/*!
* Constructs a CCJavaCsScalaCounter object.
*/
CCJavaCsScalaCounter::CCJavaCsScalaCounter( string lang )
{
	QuoteStart = "\"'";
	QuoteEnd   = QuoteStart;
	quote_start_is_end = true;

	if ( "SCALA" == lang )
	{
		// Either one double Quote character to start and end a string literal
		// as above
		// Or 3 double Quote characters to start and end a multiline string literal
		QuoteMultiStart = "\"\"\"";
		QuoteMultiEnd   = QuoteMultiStart;
	}

	quote_multi_start_is_end = true;

	QuoteEscapeFront = '\\';

	if ( "SCALA" == lang )			// Scala does not have a line continuation symbol
		ContinueLine = "";
	else
		ContinueLine = "\\";

	BlockCommentStart.push_back("/*");
	BlockCommentEnd.push_back("*/");

	LineCommentStart.push_back("//");

	// these properties are common for C/C++, Java, C# and Scala (specific where noted)
	cmplx_calc_list.push_back("%");
	cmplx_calc_list.push_back("^");
	cmplx_calc_list.push_back("++");
	cmplx_calc_list.push_back("+");
	cmplx_calc_list.push_back("--");
	cmplx_calc_list.push_back("-");
	cmplx_calc_list.push_back("*");
	cmplx_calc_list.push_back("/");
	cmplx_calc_list.push_back(">>");
	cmplx_calc_list.push_back("<<");

	cmplx_cond_list.push_back("case");
	cmplx_cond_list.push_back("else");
	cmplx_cond_list.push_back("else if");
	cmplx_cond_list.push_back("for");
	cmplx_cond_list.push_back("if");

	if ( "SCALA" == lang )
		cmplx_cond_list.push_back("match");		// switch not in Scala
	else
		cmplx_cond_list.push_back("switch");

	cmplx_cond_list.push_back("while");

	if ( "SCALA" != lang )			// Scala does not have trinary compare operator
		cmplx_cond_list.push_back("?");

	cmplx_logic_list.push_back("&&");	// 1 pass parser uses this for Cyclomatic complexity ring 2 CC2
	cmplx_logic_list.push_back("||");	// 1 pass parser uses this for Cyclomatic complexity ring 2 CC2
	cmplx_logic_list.push_back("==");
	cmplx_logic_list.push_back("!"); 
	cmplx_logic_list.push_back(">");
	cmplx_logic_list.push_back("<");
	cmplx_logic_list.push_back(">=");
	cmplx_logic_list.push_back("=<");

	cmplx_assign_list.push_back("=");

	if ( "SCALA" == lang )
		cmplx_assign_list.push_back("<-");	// Scala also uses left pointing arrow for assignment

// Set up Auxillary meanings list of Keywords or Operations and related list of values
//
// This is different from using cmplx_cyclomatic_list as the multiple pass parser does.
// Instead an extra field aux is set from the below that gives Cyclomatic, OO, FP info.
//
	cmplx_cyclomatic_list.push_back("if");
	cmplx_cyclomatic_listVals.push_back( CYCLOMATIC_CC_ALL );

	cmplx_cyclomatic_list.push_back("case");
	cmplx_cyclomatic_listVals.push_back( CYCLOMATIC_EXCEPT_CC3 );

	cmplx_cyclomatic_list.push_back("while");
	cmplx_cyclomatic_listVals.push_back( CYCLOMATIC_CC_ALL );

	cmplx_cyclomatic_list.push_back("for");
	cmplx_cyclomatic_listVals.push_back( CYCLOMATIC_CC_ALL );

	if ( "C_SHARP" == lang )
	{
		cmplx_cyclomatic_list.push_back("foreach");
		cmplx_cyclomatic_listVals.push_back( CYCLOMATIC_CC_ALL );
	}

	// Exception handler clause
	cmplx_cyclomatic_list.push_back("catch");
	cmplx_cyclomatic_listVals.push_back( CYCLOMATIC_CC_ALL );

	// ? trinary compare is not used for CC1 to CC4 metrics
	if ( "SCALA" != lang )			// Scala does not have trinary compare operator
		cmplx_cyclomatic_list.push_back("?");


	cmplx_cyclomatic_logic_list.push_back("||");
	cmplx_cyclomatic_logic_list.push_back("&&");


	cmplx_cyclomatic_case_list.push_back("case");
	cmplx_cyclomatic_switch_list.push_back("switch");

        //Modification: 2018.01 Integration Starts
        three_char_operator_list.push_back("<<=");
        three_char_operator_list.push_back(">>=");

        two_char_operator_list.push_back("||");
        two_char_operator_list.push_back("&&");
        two_char_operator_list.push_back("==");
        two_char_operator_list.push_back("!=");
        two_char_operator_list.push_back("<=");
        two_char_operator_list.push_back(">=");
        two_char_operator_list.push_back("<<");
        two_char_operator_list.push_back(">>");
        two_char_operator_list.push_back("++");
        two_char_operator_list.push_back("--");
        two_char_operator_list.push_back("+=");
        two_char_operator_list.push_back("-=");
        two_char_operator_list.push_back("&=");
        two_char_operator_list.push_back("|=");
        two_char_operator_list.push_back("+=");
        two_char_operator_list.push_back("-=");
        two_char_operator_list.push_back("*=");
        two_char_operator_list.push_back("/=");
        two_char_operator_list.push_back("^=");
        two_char_operator_list.push_back("%=");

        one_char_operator_list.push_back("=");
        one_char_operator_list.push_back("+");
        one_char_operator_list.push_back("-");
        one_char_operator_list.push_back("*");
        one_char_operator_list.push_back("/");
        one_char_operator_list.push_back("~");
        one_char_operator_list.push_back("^");
        one_char_operator_list.push_back("&");
        one_char_operator_list.push_back("|");
        one_char_operator_list.push_back(">");
        one_char_operator_list.push_back("<");

        one_char_operator_list.push_back("(");
        one_char_operator_list.push_back(")");
        one_char_operator_list.push_back("[");
        one_char_operator_list.push_back("]");
        one_char_operator_list.push_back("{");
        one_char_operator_list.push_back("}");
        one_char_operator_list.push_back("!");
        one_char_operator_list.push_back("%");
        one_char_operator_list.push_back(":");
        one_char_operator_list.push_back("?");
        one_char_operator_list.push_back(";");
        one_char_operator_list.push_back(",");
        one_char_operator_list.push_back(".");
        one_char_operator_list.push_back("!");
        one_char_operator_list.push_back("%");
        one_char_operator_list.push_back("\\");

        //Modification: 2018.01 Integration ends

// TODO: Add more keywords/values for Object Orientation and Functional Programming metrics collection

}

/*!
* Counts directive lines of code.
*
* \param fmap list of processed file lines
* \param result counter results
* \param fmapBak list of original file lines (same as fmap except it contains unmodified quoted strings)
*
* \return method status
*/
int CCJavaCsScalaCounter::CountDirectiveSLOC(filemap* fmap, results* result, filemap* fmapBak)
{
	bool contd = false, trunc_flag = false;
	size_t idx, strSize;
	unsigned int cnt = 0;
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$";
	string strDirLine = "";

	// Because of Polymorphism change step above to show running here.
	ENTER_1( "CCJavaCsScalaCounter::CountDirectiveSLOC" );
	SAVE_TO_2( "CCJavaCsScalaCounter::CountDirectiveSLOC loop start" );
	currentPhyLine = 0;
	filemap::iterator itfmBak = fmapBak->begin();
	for (filemap::iterator iter = fmap->begin(); iter != fmap->end(); iter++, itfmBak++)
	{
		currentPhyLine++;
		SAVE_TO_2( "CUtil::CheckBlank" );
		if (CUtil::CheckBlank(iter->line))
			continue;
		size_t lineNumber = iter->lineNumber;

		if (print_cmplx)
		{
			cnt = 0;
			SAVE_TO_2( "CUtil::CountTally directive" );
			CUtil::CountTally(" " + iter->line, directive, cnt, 1, exclude, "", "", &result->directive_count);
		}

		if (!contd)
		{
			// if not a continuation of a previous directive
			for (vector<string>::iterator viter = directive.begin(); viter != directive.end(); viter++)
			{
				// ensures the keyword stands alone, avoid, e.g., #ifabc
				SAVE_TO_2( "CUtil::FindKeyword directive" );
				if (((idx = CUtil::FindKeyword(iter->line, *viter)) != string::npos) && idx == 0)
				{
					contd = true;
					break;
				}
			}
			if (contd)
			{
				SAVE_TO_2( "CUtil::TruncateLine Not continuation" );
				strSize = CUtil::TruncateLine(itfmBak->line.length(), 0, this->lsloc_truncate, trunc_flag);
				if (strSize > 0)
					strDirLine = itfmBak->line.substr(0, strSize);
				result->directive_lines[PHY]++;
			}
 		}
		else
		{
			// continuation of a previous directive
			SAVE_TO_2( "CUtil::TruncateLine Continuation" );
			strSize = CUtil::TruncateLine(itfmBak->line.length(), strDirLine.length(), this->lsloc_truncate, trunc_flag);
			if (strSize > 0)
				strDirLine += "\n" + itfmBak->line.substr(0, strSize);
			result->directive_lines[PHY]++;
		}
	
		if (contd)
		{
			// drop continuation symbol
			if (strDirLine[strDirLine.length()-1] == '\\')
				strDirLine = strDirLine.substr(0, strDirLine.length()-1);

			// if a directive or continuation of a directive (no continuation symbol found)
			if (iter->line[iter->line.length()-1] != ',' && iter->line[iter->line.length()-1] != '\\')
			{
				contd = false;
				SAVE_TO_2( "result->addSLOC" );
				if (result->addSLOC(strDirLine, lineNumber, trunc_flag))
					result->directive_lines[LOG]++;
			}
			iter->line = "";
		}
	}

	SAVE_TO_2( "Exit CCJavaCsScalaCounter::CountDirectiveSLOC" );
	return 1;
}

/*!
* Processes physical and logical lines according to language specific rules.
* NOTE: all the blank lines +
*               whole line comments +
*               lines with compiler directives
*       should have been blanked from filemap by previous processing
*       before reaching this function
*
* \param fmap list of processed file lines
* \param result counter results
* \param fmapBak list of original file lines (same as fmap except it contains unmodified quoted strings)
*
* \return method status
*/
int CCJavaCsScalaCounter::LanguageSpecificProcess(filemap* fmap, results* result, filemap* fmapBak)
{
	unsigned int	paren_count			= 0;
	bool			for_flag			= false;
	bool			found_for			= false;
	bool			found_forifwhile	= false;
	bool			found_while			= false;
	char			prev_char			= 0;
	bool			data_continue		= false;
	bool			inArrayDec			= false;
	string			strLSLOC			= "";
	string			strLSLOCBak			= "";
	unsigned int	openBrackets		= 0;

	filemap::iterator fit, fitbak;
	string line, lineBak;
	StringVector loopLevel;

	unsigned int phys_exec_lines = 0;
	unsigned int phys_data_lines = 0;
	unsigned int temp_lines = 0;
	unsigned int cnt = 0;
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$";

	// Because of Polymorphism change step above to show running here.
	ENTER_1( "Start" );
	SAVE_TO_2( "Loop start" );
	currentPhyLine = 0;
	for (fit = fmap->begin(), fitbak = fmapBak->begin(); fit != fmap->end(); fit++, fitbak++)
	{
		currentPhyLine++;
		line = fit->line;
		size_t lineNumber = fit->lineNumber;

		// insert blank at the beginning (for searching keywords)
		line = ' ' + line;
		lineBak = ' ' + fitbak->line;
	
		// do not process blank lines
		// blank line means blank_line/comment_line/directive
		SAVE_TO_2( "CUtil::CheckBlank" );
		if (!CUtil::CheckBlank(line))
		{
			SAVE_TO_2( "CCJavaCsCounter::LSLOC" );
			LSLOC(result, line, lineNumber, lineBak, strLSLOC, strLSLOCBak, paren_count, for_flag, found_forifwhile, found_while,
				prev_char, data_continue, temp_lines, phys_exec_lines, phys_data_lines, inArrayDec, found_for,
				openBrackets, loopLevel);

			if (print_cmplx)
			{
				cnt = 0;
				SAVE_TO_2( "CUtil::CountTally Exec name list" );
				CUtil::CountTally(line, exec_name_list, cnt, 1, exclude, "", "", &result->exec_name_count);
			}

			result->exec_lines[PHY] += phys_exec_lines;
			phys_exec_lines = 0;

			result->data_lines[PHY] += phys_data_lines;
			phys_data_lines = 0;
		}
	}

	SAVE_TO_2( "Exit CCJavaCsScalaCounter::LanguageSpecificProcess" );
	return 1;
}

/*!
* Extracts and stores logical lines of code.
* Determines and extract logical SLOC to place in the result variable
* using addSLOC function. Each time the addSLOC function is called,
* a new logical SLOC is added. This function assumes that the directive
* is handled before it is called.
*
* \param result counter results
* \param line processed physical line of code
* \param lineBak original physical line of code
* \param strLSLOC processed logical string
* \param strLSLOCBak original logical string
* \param paren_cnt count of parenthesis
* \param forflag found for flag
* \param found_forifwhile found for, if, or while flag
* \param found_while found while flag
* \param prev_char previous character
* \param data_continue continuation of a data declaration line
* \param temp_lines tracks physical line count
* \param phys_exec_lines number of physical executable lines
* \param phys_data_lines number of physical data lines
* \param inArrayDec marks an array declaration
* \param found_for found for loop
* \param openBrackets number of open brackets (no matching close bracket)
* \param loopLevel nested loop level
*/
void CCJavaCsScalaCounter::LSLOC(results* result, string line, size_t lineNumber, string lineBak, string &strLSLOC, 
							string &strLSLOCBak, unsigned int &paren_cnt,
							bool &forflag, bool &found_forifwhile, bool &found_while, char &prev_char, bool &data_continue,
							unsigned int &temp_lines, unsigned int &phys_exec_lines, unsigned int &phys_data_lines,
							bool &inArrayDec, bool &found_for, unsigned int &openBrackets, StringVector &loopLevel)
{
	// paren_cnt is used with 'for' statement only
	size_t start = 0; //starting index of the working string
	size_t i = 0, strSize;
	bool found_do, found_try, found_else, trunc_flag = false;
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$:";
	string dataExclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$:().";	// avoid double count of casts as data and executable lines (e.g. set { m_uiValue = (uint)value; }

	unsigned int cnt = 0;

	SAVE_TO_3( "CUtil::TrimString" );
	string tmp = CUtil::TrimString(strLSLOC);

	// do, try
	SAVE_TO_3( "CUtil::FindKeyword" );
	found_do = (CUtil::FindKeyword(tmp, "do") != string::npos);
	found_try = (CUtil::FindKeyword(tmp, "try") != string::npos);
	// else is treated differently, else is included in SLOC, do and try are not
	found_else = (CUtil::FindKeyword(tmp, "else") != string::npos);

	// there may be more than 1 logical SLOC in this line
	while (i < line.length())
	{
		switch (line[i])
		{
		case ';': case '{': // LSLOC terminators
			                // ';' for normal executable or declaration statement
			                // '{' for starting a function or 'do' stmt or a block (which is counted)
			// get the previous logical mark until i-1 index is the new LSLOC
			// except 'do' precedes '{'
			// except '}' precedes ';' ??
			// do nothing inside 'for' statement
			if (found_for == true && paren_cnt > 0 && line[i] == ';')
				break;

			// record open bracket for nested loop processing
			if (print_cmplx)
			{
				if (line[i] == '{')
				{
					openBrackets++;
					if ((unsigned int)loopLevel.size() < openBrackets)
						loopLevel.push_back("");
				}
				else
				{
					if ((unsigned int)loopLevel.size() > openBrackets && openBrackets > 0)
						loopLevel.pop_back();
				}
			}

			// case 'while(...);', 'while(...) {', and '} while(...);'
			// this case is handled in case ')'
			if (found_while && found_forifwhile)
			{
				found_while = false;
				found_forifwhile = false;
				start = i + 1;
				break;
			}

			if (line[i] == '{')
			{
				if (prev_char == '=')
					inArrayDec = true;

				// continue until seeing ';'
				if (inArrayDec)
					break;

				// case for(...); and if (...) {
				// these specials are handled
				if (found_forifwhile)
				{
					found_forifwhile = false;
					start = i + 1;
					break;
				}

				// check if 'do' precedes '{'
				if (!found_do && !found_try && !found_else)
				{
					// find for 'do' in string before tmp string
					SAVE_TO_3( "CUtil::TrimString_2" );
					tmp = CUtil::TrimString(line.substr(start, i - start));
					found_do = (tmp == "do");		// found 'do' statement
					found_try = (tmp == "try");		// found 'try' statement
					// same as else
					found_else = (tmp == "else");	// found 'else' statement
				}
				if (found_do || found_try || found_else)
				{
					if (found_do && print_cmplx)
					{
						if (loopLevel.size() > 0) 
							loopLevel.pop_back();
						loopLevel.push_back("do");
					}
					found_do = false;
					found_try = false;
					if (!found_else)
					{
						// everything before 'do', 'try' are cleared
						strLSLOC = "";
						strLSLOCBak = "";
						start = i + 1;
					}
					break; // do not store '{' following 'do'
				}
			}

			// wrong, e.g., a[]={1,2,3};
			if (line[i] == ';' && prev_char == '}')
			{
				// check if in array declaration or not
				// if no, skip, otherwise, complete the SLOC containing array declaration
				if (!inArrayDec)
				{
					start = i + 1;
					break;
				}
			}

			inArrayDec = false;

			// check for empty statement (=1 LSLOC)
			SAVE_TO_3( "CUtil::TrimString_3" );
			if (CUtil::TrimString(line.substr(start, i + 1 - start)) == ";" && strLSLOC.length() < 1)
			{
				strLSLOC = ";";
				strLSLOCBak = ";";
			}
			else
			{
				SAVE_TO_3( "CUtil::TruncateLine" );
				strSize = CUtil::TruncateLine(i + 1 - start, strLSLOC.length(), this->lsloc_truncate, trunc_flag);
				if (strSize > 0)
				{
					strLSLOC += line.substr(start, strSize);
					strLSLOCBak += lineBak.substr(start, strSize);
				}
			}
			SAVE_TO_3( "result->addSLOC" );
			if (result->addSLOC(strLSLOCBak, lineNumber, trunc_flag))
			{
				cnt = 0;
				SAVE_TO_3( "CUtil::CountTally" );
				CUtil::CountTally(strLSLOC, data_name_list, cnt, 1, dataExclude, "", "", &result->data_name_count);

				temp_lines++;
				if (data_continue == true && line[i] == ';')
				{
					result->data_lines[LOG]++;
					phys_data_lines = temp_lines;
				}
				else
				{
					if (cnt > 0 && line[i] == ';')
					{
						result->data_lines[LOG]++;
						phys_data_lines = temp_lines;
					}
					else
					{
						result->exec_lines[LOG]++;
						phys_exec_lines = temp_lines;
					}
				}
			}
			else if (data_continue == true && line[i] == ';')
				phys_data_lines = temp_lines;
			else
				phys_exec_lines = temp_lines;
			data_continue = false;
			temp_lines = 0;
			strLSLOC = strLSLOCBak = "";
			start = i + 1;

			// reset some flagging parameters
			forflag = false;
			paren_cnt = 0;
			found_while = false;
			found_forifwhile = false;
			found_for = false;

			break;
		case '(':
			if (forflag)
				paren_cnt++;
			else
			{
				// handle 'for', 'foreach', 'while', 'if' the same way
				SAVE_TO_3( "CUtil::TrimString_4" );
				tmp = CUtil::TrimString(line.substr(start, i));
				SAVE_TO_3( "CUtil::FindKeyword_2" );
				if (CUtil::FindKeyword(tmp, "for") != string::npos
					|| CUtil::FindKeyword(tmp, "foreach") != string::npos
					|| CUtil::FindKeyword(tmp, "while") != string::npos
					|| CUtil::FindKeyword(tmp, "if") != string::npos)
				{
					forflag = true;
					paren_cnt++;

					if (print_cmplx && (unsigned int)loopLevel.size() > openBrackets && openBrackets > 0)
						loopLevel.pop_back();

					SAVE_TO_3( "CUtil::FindKeyword_3" );
					if (CUtil::FindKeyword(tmp, "for") != string::npos)
					{
						if (print_cmplx)
							loopLevel.push_back("for");
						found_for = true;
					}
					else if (CUtil::FindKeyword(tmp, "while") != string::npos)
					{
						if (print_cmplx)
							loopLevel.push_back("while");
						found_while = true;
					}
					else if (print_cmplx && CUtil::FindKeyword(tmp, "foreach") != string::npos)
						loopLevel.push_back("foreach");

					// record nested loop level
					if (print_cmplx)
					{
						SAVE_TO_3( "CUtil::FindKeyword_4" );
						if (CUtil::FindKeyword(tmp, "if") == string::npos)
						{
							unsigned int loopCnt = 0;
							for (StringVector::iterator lit = loopLevel.begin(); lit < loopLevel.end(); lit++)
							{
								if ((*lit) != "")
									loopCnt++;
							}
							if ((unsigned int)result->cmplx_nestloop_count.size() < loopCnt)
								result->cmplx_nestloop_count.push_back(1);
							else
								result->cmplx_nestloop_count[loopCnt-1]++;
						}
					}
				}
			}
			break;
		case ')':
			if (forflag)
			{
				if (paren_cnt > 0)
					paren_cnt--;
				if (paren_cnt == 0)
				{
					// handle 'for', 'foreach', 'while', 'if'
					SAVE_TO_3( "CUtil::TruncateLine" );
					strSize = CUtil::TruncateLine(i + 1 - start, strLSLOC.length(), this->lsloc_truncate, trunc_flag);
					if (strSize > 0)
					{
						strLSLOC += line.substr(start, strSize);
						strLSLOCBak += lineBak.substr(start, strSize);
					}
					SAVE_TO_3( "result->addSLOC_2" );
					if (result->addSLOC(strLSLOCBak, lineNumber, trunc_flag))
						result->exec_lines[LOG]++;
					strLSLOCBak = strLSLOC = "";
					phys_exec_lines = temp_lines;
					temp_lines = 0;
					start = i + 1;
					found_forifwhile = true;
					forflag = false;
					found_for = false;
				}
			}
			break;
		case '}':
			// skip '}' when found ';' and then '}' because '{' is counted already
			// also, {} is also skipped, counted
			if (prev_char == ';' || prev_char == '{' || prev_char == '}')
				if (!inArrayDec) start = i + 1;

			// record close bracket for nested loop processing
			if (print_cmplx)
			{
				if (openBrackets > 0)
					openBrackets--;
				if (loopLevel.size() > 0)
					loopLevel.pop_back();
			}
			break;
		}

		if (line[i] != ' ' && line[i] != '\t') 
		{
			// if ;}}} --> don't count }}} at all
			// also, if {}}} --> don't count }}} at all
			// if ( !(line[i] == '}' && (prev_char == ';' || prev_char == '{'))) // see case '}' above
			prev_char = line[i];

			// change to not found if a char appears before
			if (line[i] != ')' && found_forifwhile)
				found_forifwhile = false;
		}
		i++;
	}

	SAVE_TO_3( "CUtil::TrimString_5" );
	tmp = CUtil::TrimString(line.substr(start, i - start));
	SAVE_TO_3( "CUtil::TruncateLine_2" );
	strSize = CUtil::TruncateLine(tmp.length(), strLSLOC.length(), this->lsloc_truncate, trunc_flag);
	if (strSize > 0)
	{
		strLSLOC += tmp.substr(0, strSize);
		SAVE_TO_3( "CUtil::TrimString_6" );
		tmp = CUtil::TrimString(lineBak.substr(start, i - start));
		strLSLOCBak += tmp.substr(0, strSize);

		// drop continuation symbol
		if (strLSLOC[strLSLOC.length()-1] == '\\')
		{
			strLSLOC = strLSLOC.substr(0, strLSLOC.length()-1);
			strLSLOCBak = strLSLOCBak.substr(0, strLSLOCBak.length()-1);
		}
	}
	
	// make sure that we are not beginning to process a new data line
	cnt = 0;
	SAVE_TO_3( "CUtil::CountTally_2" );
	CUtil::CountTally(strLSLOC, data_name_list, cnt, 1, exclude, "", "", NULL);

	if (cnt > 0)
		data_continue = true;
	if (data_continue)
		temp_lines++;
	if (temp_lines == 0 && phys_data_lines == 0 && phys_exec_lines == 0)
		phys_exec_lines = 1;

	SAVE_TO_3( "Exit CCJavaCsScalaCounter::LSLOC" );
}

/*!
* Parses lines for function/method names.
*
* \param line line to be processed
* \param lastline last line processed
* \param functionStack stack of functions
* \param functionName function name found
* \param functionCount function count found
*
* \return 1 if function name is found
*/
int CCJavaCsScalaCounter::ParseFunctionName(const string &line, string &lastline,
	filemap &functionStack, string &functionName, unsigned int &functionCount)
{
	string tline, str;
	size_t idx, tidx, cnt, cnt2;
	unsigned int fcnt, cyclomatic_cnt = 0;
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$";

	// Because of Polymorphism change step above to show running here.
	ENTER_2( "CCJavaCsScalaCounter::ParseFunctionName" );
	SAVE_TO_3( "CUtil::TrimString" );
	tline = CUtil::TrimString(line);
	idx = tline.find('{');
	if (idx != string::npos)
	{
		// check whether it is at first index, if yes then function name is at above line
		if (idx == 0)
		{
			lineElement element(++functionCount, lastline);
			functionStack.push_back(element);
			lastline.erase();
		}
		else
		{
			str = tline.substr(0, idx);
			tidx = cnt = cnt2 = 0;
			if (str[0] != '(' && str[0] != ':' && (lastline.length() < 1 || lastline[lastline.length() - 1] != ':'))
			{
				while (tidx != string::npos)
				{
					tidx = str.find('(', tidx);
					if (tidx != string::npos)
					{
						cnt++;
						tidx++;
					}
				}
				if (cnt > 0)
				{
					tidx = 0;
					while (tidx != string::npos)
					{
						tidx = str.find(')', tidx);
						if (tidx != string::npos)
						{
							cnt2++;
							tidx++;
						}
					}
				}
			}
			// make sure parentheses are closed and no parent class listed
			if ((cnt > 0 && cnt == cnt2) || (lastline.length() > 0 && lastline[lastline.length() - 1] == ';'))
				lastline = str;
			else
				lastline += " " + str;
			lineElement element(++functionCount, CUtil::TrimString(lastline));
			functionStack.push_back(element);
			lastline.erase();
		}
	}
	else if (tline.length() > 0 && tline[tline.length() - 1] != ';' &&
		lastline.length() > 0 && lastline[lastline.length() - 1] != ';')
	{
		// append until all parentheses are closed
		tidx = lastline.find('(');
		if (tidx != string::npos)
		{
			cnt = 1;
			while (tidx != string::npos)
			{
				tidx = lastline.find('(', tidx + 1);
				if (tidx != string::npos)
					cnt++;
			}
			tidx = lastline.find(')');
			while (tidx != string::npos)
			{
				cnt++;
				tidx = lastline.find(')', tidx + 1);
			}
			if (cnt % 2 != 0)
				lastline += " " + tline;
			else
				lastline = tline;
		}
		else
			lastline = tline;
	}
	else
		lastline = tline;

	idx = line.find('}');
	if (idx != string::npos && !functionStack.empty())
	{
		str = functionStack.back().line;
		fcnt = functionStack.back().lineNumber;
		functionStack.pop_back();
		idx = str.find('(');

		if (idx != string::npos)
		{
			// search for cyclomatic complexity keywords and other possible keywords
			SAVE_TO_3( "CUtil::CountTally cyclomatic complexity keywords and other" );
			CUtil::CountTally(str, cmplx_cyclomatic_list, cyclomatic_cnt, 1, exclude, "", "", 0, casesensitive);

			SAVE_TO_3( "CUtil::FindKeyword" );
			if (cyclomatic_cnt <= 0 && CUtil::FindKeyword(str, "switch") == string::npos &&
				CUtil::FindKeyword(str, "try") == string::npos && CUtil::FindKeyword(str, "finally") == string::npos &&
				CUtil::FindKeyword(str, "return") == string::npos && str.find('=') == string::npos)
			{
				SAVE_TO_3( "CUtil::ClearRedundantSpaces" );
				string myFunctionName = CUtil::ClearRedundantSpaces(str.substr(0, idx));

			#ifdef	_DEBUG
				if ( myFunctionName.size() == 0 )
				{
					// PrintCyclomaticComplexity will now skip any empty Function Names.
					// Done this way as is easier and less Risk of adding Defects from complex Cyclomatic Complexity code
					// TODO: Go through the CC code and the ParseFunctionName code and try to SAFELY improve.
					// cout << endl << "CCJavaCsScalaCounter::ParseFunctionName() Empty function name allowed" << endl;
				}
			#endif
				// May have the actual arg list so look for first ( and do not copy
				size_t myIdx = myFunctionName.find( '(' );
				if ( myIdx != string::npos )
					myFunctionName = myFunctionName.substr( 0, myIdx );
				functionName = myFunctionName;
				functionCount = fcnt;
				lastline.erase();
				SAVE_TO_3( "Exit CCJavaCsScalaCounter::ParseFunctionName Found" );
				return 1;
			}
		}
		lastline.erase();
	}

	SAVE_TO_3( "Exit CCJavaCsScalaCounter::ParseFunctionName Not found" );
	return 0;
}

//Modification: 2018.01 Integration starts

/*!
* Finds the first unescaped quote occurring in a line, after position start_pos.
* Used to determine the index of the ending quote of a string.
*
* \param line a source line
* \param start_pos the index into 'line' at which a quote character has been found, we start searching from start_pos + 1
* \param quote the quote character to look for (for our use, it will be a single quote ' or a double quote ")
* \param quote_escape the string that is used to escape the passed in quote (e.g. the string \" or \')
*
* \return the index into line at which the next (after start_pos) unescaped quote is found
*/
int CCJavaCsScalaCounter::FindIdxOfNextQuote(const string &line, int start_pos, const char &quote) {
    size_t idx = line.find(quote, start_pos + 1); //Modification: 2018.01, changed data type
    int num_backslash;
    int curr_idx;

    while (idx != string::npos) {
        num_backslash = 0;

        // Count consecutive backslashes before idx
        curr_idx = idx - 1;
        while (curr_idx >= start_pos + 1 && line[curr_idx] == '\\') {
                curr_idx--;
                num_backslash++;
        }

        // If even number of backslashes then the quote at idx is a quote for ending the string/char
        // otherwise it's not and we have to continue looking
        if (num_backslash % 2 == 0) {
                break;
        } else {
                idx = line.find(quote, idx + 1);
        }
    }

    return idx;
}

/*!
* Removes all strings and chars in 'line' and stores the count of them if add_to_count is true
*
* \param line a source line. line will be modified
* \param string_char_counts the map for storing the counts of strings and chars
* \param add_to_count true to indicate that we should store the detected string and char counts into string_char_counts
* \return 0 if no error and return 1 if 'line' begins a string a string or char but does not end it
*/
int CCJavaCsScalaCounter::RemoveStringsAndChars(string &line, map<string, unsigned int> &string_char_counts, const bool &add_to_count) {
    // escape quotes
    char double_quote = '"';
    char single_quote = '\'';

    //Modification: 2018.01, changed data types to size_t
    size_t next_double_quote_idx = -1;
    size_t next_single_quote_idx = -1;

    size_t idx;

    idx = line.find("'\"'");
    // Count and remove double quotes within single quotes (i.e. '"')
    while (idx != string::npos) {
        line.replace(idx, 3, " ");
        if (add_to_count)
            string_char_counts["\""]++;
        idx = line.find("'\"'", idx + 1);
    }

    idx = line.find("'\\\"'");
    // Count and remove double quotes within single quotes (i.e. '\"')
    while (idx != string::npos) {
        line.replace(idx, 4, " ");
        if (add_to_count)
            string_char_counts["\\\""]++;
        idx = line.find("'\\\"'", idx + 1);
    }

    idx = line.find(double_quote);

    // Count and remove strings
    while (idx != string::npos) {
        // if next_double_quote is string::npos, error out
        // assume fine for now
        next_double_quote_idx = FindIdxOfNextQuote(line, idx, double_quote);
        if (next_double_quote_idx == string::npos) {
                return 1;
        }

        if (add_to_count)
            string_char_counts[line.substr(idx, next_double_quote_idx - idx + 1)]++;
        line.replace(idx, next_double_quote_idx - idx + 1, " ");
        idx = line.find(double_quote, idx + 1);
    }

    idx = line.find(single_quote);

    // Count and remove chars
    while (idx != string::npos) {
        next_single_quote_idx = FindIdxOfNextQuote(line, idx, single_quote);
        if (next_single_quote_idx == string::npos) {
                return 1;
        }


        if (add_to_count)
            string_char_counts[line.substr(idx, next_single_quote_idx - idx + 1)]++;
        line.replace(idx, next_single_quote_idx - idx + 1, " ");
        idx = line.find(single_quote, idx + 1);
    }

    return 0;
}

/*!
* Removes 'symbols' from 'line' except where '(' belongs
* to a function call (e.g. the line "somecall()" would become "somecall( ").
* For this function, a 'symbol' is considered to be those
* in two_char_operator_list or one_char_operator_list.
*
* \param line a source line. line will be modified
* \param string_char_counts the map for storing the counts of nonfunction operators
* \param should_count true to indicate that we should store the detected string and char counts into nonfunction_operator_counts
*/
void CCJavaCsScalaCounter::RemoveSymbolsExceptOpenRoundBracketOfFunction(string &line, map<string, unsigned int> &nonfunction_operator_counts, const bool &should_count) {
        size_t idx; //Modification: 2018.01 changed data type

        for (size_t i = 0; i < three_char_operator_list.size(); i++) {
                idx = line.find(three_char_operator_list[i]);

                while (idx != string::npos) {
                        if (should_count)
                                nonfunction_operator_counts[three_char_operator_list[i]]++;

                        line.replace(idx, 3, " ");
                        idx = line.find(three_char_operator_list[i], idx + 1);
                }
        }

    for (size_t i = 0; i < two_char_operator_list.size(); i++) {
        idx = line.find(two_char_operator_list[i]);

        // Keep finding the symbol two_char_operator_list[i]
        // and replacing it with a space " " until no more are found
        while (idx != string::npos) {
            if (should_count)
                nonfunction_operator_counts[two_char_operator_list[i]]++;

            line.replace(idx, 2, " ");
            idx = line.find(two_char_operator_list[i], idx + 1);
        }
    }

    // Count and replace one character operators
    for (size_t i = 0; i < one_char_operator_list.size(); i++) {
        idx = line.find(one_char_operator_list[i]);

        // Keep finding the symbol one_char_operator_list[i]
        // and replacing it with a space " " until no more are found
        while (idx != string::npos) {
            if (one_char_operator_list[i] != "(" ||
                (one_char_operator_list[i] == "(" && (idx == 0 || !IsAlphanumericOrUnderscore(line[idx - 1])))) {
                                line.replace(idx, 1, " ");

                            if (should_count)
                        nonfunction_operator_counts[one_char_operator_list[i]]++;
                        }

            idx = line.find(one_char_operator_list[i], idx + 1);
        }
    }
}

/*!
* Checks if a source line contains a function call or a function definition
* and stores the name of this function, if any, in function_name
*
* \param line a source line
* \param function_name stores the name of the first function called or defined in 'line', if any is found
* \return true if 'line' has a function call or a function definition
*/
bool CCJavaCsScalaCounter::IsEitherFunctionDefinitionOrInvocation(string line, string &function_name) {
    int idx;
    line = CUtil::TrimString(line, 0);
    map<string, unsigned int> unused_map;
    RemoveStringsAndChars(line, unused_map, false);

    // RemoveSymbolsExceptOpenRoundBracketOfFunction(modified_line, unused_map, false);
    idx = line.find_first_of("(");
    if (idx > 0 && IsAlphanumericOrUnderscore(line[idx - 1])) {
        function_name = line.substr(0, idx);
        return true;
    }
    return false;
}

/*!
* Checks if a source line contains a function call function definition.
* If it does contain a function definition, then the name of the function
* is stored in function_name.
* The implementation idea is to look for either a semicolon or an opening curly brace.
*
* \param line_idx the index into fmap from which we can extract the source line
* \param fmap the filemap of the file
* \param function_name stores the name of the first function called or defined in 'line', if any is found
* \return true if 'line' has a function call or a function definition
*/
bool CCJavaCsScalaCounter::IsFunctionDefinition(int line_idx, filemap &fmap, string &function_name) {
    unsigned int curr_line_idx = line_idx;
    bool result = false;
    string tline = CUtil::TrimString(fmap[line_idx].line, 1);
    if (IsEitherFunctionDefinitionOrInvocation(tline, function_name)) {
        // find first of ';' and '{'
        size_t idx_of_semicolon = tline.find(";");
        size_t idx_of_open_curly = tline.find("{");
        while (curr_line_idx < fmap.size() - 1 &&
                idx_of_semicolon == string::npos &&
                idx_of_open_curly == string::npos) {
            curr_line_idx++;
            tline = CUtil::TrimString(fmap[curr_line_idx].line, 1);
            idx_of_semicolon = tline.find(";");
            idx_of_open_curly = tline.find("{");
        }

        // the line is a function definition if we found a '{'
        // and either 1) we didn't find a semicolon, or
        //            2) we found a semicolon but the first one we found was after the first '{' we found
        if (idx_of_open_curly != string::npos)
            if (idx_of_semicolon == string::npos || idx_of_open_curly < idx_of_semicolon)
                result = true;
    }

    return result;
}

/*!
* Adds the counts from add_from_counts to the counts in into_counts
*
* \param add_from_counts the map containing the source data of the counts
* \param into_counts the map that is added to
*/
void CCJavaCsScalaCounter::SumUp(map<string, map<string, unsigned int> > &add_from_counts, map<string, unsigned int> &into_counts) {
    for (map<string, map<string, unsigned int> >::iterator func_iter = add_from_counts.begin(); func_iter != add_from_counts.end(); func_iter++) {
        map<string, unsigned int> op_counts = func_iter->second;
        for (map<string, unsigned int>::iterator op_to_count = op_counts.begin(); op_to_count != op_counts.end(); op_to_count++)
                into_counts[op_to_count->first] += op_to_count->second;
    }
}

/*!
* Determines if we have arrived at the end of a function (by using the count of open and ending curly braces)
*
* \param nonfunction_operator_counts the map containing the counts of the nonfunction operators seen
* \return true if there is more than one '{' in nonfunction_operator_counts and the number of '{' equals the number of '}'
*/
bool CCJavaCsScalaCounter::IsLastLineOfFunction(map<string, unsigned int> &nonfunction_operator_counts) {
    return nonfunction_operator_counts["{"] > 0 &&
        nonfunction_operator_counts["{"] == nonfunction_operator_counts["}"];
}

/*!
* Calculates Halstead's volume using the counts passed in
*
* \param string_char_counts the map containing the counts of strings and chars seen
* \param word_counts the map containing the counts of items such as variable names (these are considered operators)
* \param bool_counts the map containing the counts of bools (true and false)
* \param number_counts the map containing the counts of numbers seen (e.g. integers, floats, etc)
* \param function_counts the map containing the counts of functions (e.g. printf, user-defined functions invoked, etc)
* \param nonfunction_operator_counts the map containing the counts of the nonfunction operators (e.g. counts of '+', '-', etc)
* \return Halstead's volume or -1.0 if the total number of unique operators plus the total number of unique operands is zero
*/
double CalculateHalsteadsVolumeFromCounts(map<string, unsigned int> &string_char_counts,
                                              map<string, unsigned int> &word_counts,
                                              map<string, unsigned int> &bool_counts,
                                              map<string, unsigned int> &number_counts,
                                              map<string, unsigned int> &function_counts,
                                              map<string, unsigned int> &nonfunction_operator_counts) {
    unsigned int total_operators = 0;
    unsigned int total_operands = 0;

    unsigned int unique_operators = 0;
    unsigned int unique_operands = 0;

    vector<map<string, unsigned int> > counts_list;
    counts_list.push_back(string_char_counts);
    counts_list.push_back(word_counts);
    counts_list.push_back(bool_counts);
    counts_list.push_back(number_counts);
    counts_list.push_back(function_counts);
    counts_list.push_back(nonfunction_operator_counts);

    for (vector<map<string, unsigned int> >::iterator ot = counts_list.begin(); ot != counts_list.end(); ot++) {
            for (map<string, unsigned int>::iterator it = ot->begin(); it != ot->end(); it++) {
                total_operands += it->second;
                unique_operands++;
            }
        }

        if (unique_operators + unique_operands == 0)
                return -1.0;

    return (double)(total_operators + total_operands) * log2((double)(unique_operators + unique_operands));
}

/*!
* True if a char is a letter, number, or underscore
* This is used because variable names are a combination of letters, numbers, and underscores
*
* \param c the character in consideration
* \return true if c is a letter, a number, or an underscore, otherwise returns false
*/
bool CCJavaCsScalaCounter::IsAlphanumericOrUnderscore(const char &c) {
        return isalnum(c) || c == '_';
}

/*!
* Counts the operators and operands in a source line and stores the counts in the maps
*
* \param string_char_counts the map for storing the counts of strings and chars seen
* \param word_counts the map for storing the counts of items such as variable names (these are considered operators)
* \param bool_counts the map for storing the counts of bools (true and false)
* \param number_counts the map for storing the counts of numbers seen (e.g. integers, floats, etc)
* \param function_counts the map for storing the counts of functions (e.g. printf, user-defined functions invoked, etc)
* \param nonfunction_operator_counts the map for storing the counts of the nonfunction operators (e.g. counts of '+', '-', etc)
* \return 0 if no error and return 1 if 'line' begins a string a string or char but does not end it
*/
int CCJavaCsScalaCounter::CountOperatorsAndOperands(string &line,
        map<string, unsigned int> &string_char_counts,
        map<string, unsigned int> &word_counts,
        map<string, unsigned int> &bool_counts,
        map<string, unsigned int> &number_counts,
        map<string, unsigned int> &function_counts,
        map<string, unsigned int> &nonfunction_operator_counts) {

    int count = 0;
    int status = 0;

    string curr_token;


    // Count the strings in 'tline' and remove them from 'tline'
    // so that we can work on the line without these strings
    status = RemoveStringsAndChars(line, string_char_counts, true);
    if (status != 0)
        return 1;

    RemoveSymbolsExceptOpenRoundBracketOfFunction(line, nonfunction_operator_counts, true);
 
        // Insert a space after every (
    size_t idx = line.find("("); //Modification: 2018.01, changed data type to size_t
    while (idx != string::npos) {
        line.replace(idx, 1, "( ");
        idx = line.find("(", idx + 2);
    }

    // Get tokens using the blank space as the delimiter
    // Tokenize line
    vector<string> tokens;
    istringstream iss(line);
    string tmp_token;
    while (iss >> tmp_token) {
        tokens.push_back(tmp_token);
    }

    string token;
    // Parse tokens for variables, numbers, and bools
    for (vector<string>::iterator it = tokens.begin(); it != tokens.end(); it++) {
        token = *it;
        if (token.size() == 0)
            continue;

        if (keyword_operators.find(token) != keyword_operators.end()) {
            nonfunction_operator_counts[token]++;
        } else if (token[token.size() - 1] == '(') {
            function_counts[(token.substr(0, token.size() - 1))]++;
            nonfunction_operator_counts["("]++;
        } else if (token[0] >= '0' && token[0] <= '9') {
            // number
            number_counts[token]++;
            count++;
        } else if (token == "true" || token == "false") {
            // boolean
            bool_counts[token]++;
        } else if (IsAlphanumericOrUnderscore(token[0])) {
            word_counts[token]++;
        }
    }
    return 0;
}

/*!
* Calculates Halstead's volume for the file and for each function within the file,
* storing the computed values in result.
*
* \param fmapModBak the filemap containing the lines of the file without any comments
* \param result the results structure that contains variables for storing Halstead's volume
* \return 0 if the function did not encounter any errors or return 1 if there was an error
*/
int CCJavaCsScalaCounter::FindHalsteadsVolume(filemap fmapModBak, results* result) {
        size_t curr_line_idx = 0;
        bool has_function_name = false;
    string line;
    string function_name = "";
    map<string, unsigned int> curr_string_and_char_operand_counts;
    map<string, unsigned int> curr_word_operand_counts;
    map<string, unsigned int> curr_bool_operand_counts;
    map<string, unsigned int> curr_number_operand_counts;
    map<string, unsigned int> curr_function_operator_counts;
    map<string, unsigned int> curr_nonfunction_operator_counts;
    bool encountered_error = false;
    int status = 0;

    while (curr_line_idx < fmapModBak.size()) {
        line = CUtil::TrimString(fmapModBak[curr_line_idx].line, 0);
        has_function_name = IsFunctionDefinition(curr_line_idx, fmapModBak, function_name);

        if (has_function_name) {
            // process until end of function and calculate halsteads volume and put into class variable
            // reset counts for functions
            curr_string_and_char_operand_counts.clear();
            curr_word_operand_counts.clear();
            curr_bool_operand_counts.clear();
            curr_number_operand_counts.clear();

            curr_function_operator_counts.clear();
            curr_nonfunction_operator_counts.clear();

            while (true) {
                //Modification : 2018.05 Function not being used
				/*curr_line_idx = GetLineUntilEndOfMultistringIfAny(curr_line_idx, line, fmapModBak, curr_nonfunction_operator_counts);*/

                status = CountOperatorsAndOperands(line,
                                                   curr_string_and_char_operand_counts,
                                                   curr_word_operand_counts,
                                                   curr_bool_operand_counts,
                                                   curr_number_operand_counts,
                                                   curr_function_operator_counts,
                                                   curr_nonfunction_operator_counts);
                if (status != 0)
                        return 1;

                if (IsLastLineOfFunction(curr_nonfunction_operator_counts) ||
                        curr_line_idx >= fmapModBak.size() - 1) {
                    // Store per function counts
                    result->func_string_and_char_operand_counts[function_name] = curr_string_and_char_operand_counts;
                    result->func_word_operand_counts[function_name] = curr_word_operand_counts;
                    result->func_bool_operand_counts[function_name] = curr_bool_operand_counts;
                    result->func_number_operand_counts[function_name] = curr_number_operand_counts;

                    result->func_function_operator_counts[function_name] = curr_function_operator_counts;
                    result->func_nonfunction_operator_counts[function_name] = curr_nonfunction_operator_counts;

                    result->func_halsteads_volume[function_name] = CalculateHalsteadsVolumeFromCounts(
                                                                          curr_string_and_char_operand_counts,
                                                                          curr_word_operand_counts,
                                                                          curr_bool_operand_counts,
                                                                          curr_number_operand_counts,
                                                                          curr_function_operator_counts,
                                                                          curr_nonfunction_operator_counts);
                    if (result->func_halsteads_volume[function_name] < 0)
                        encountered_error = true;

                    break;
                }

                curr_line_idx++;
                line = fmapModBak[curr_line_idx].line;
            }
        } else {
			//Modification : 2018.05 Function not being used 
            /*curr_line_idx = GetLineUntilEndOfMultistringIfAny(curr_line_idx, line, fmapModBak, result->file_nonfunction_operator_counts);*/

            // add to file count only
            status = CountOperatorsAndOperands(line,
                                               result->file_string_and_char_operand_counts,
                                               result->file_word_operand_counts,
                                               result->file_bool_operand_counts,
                                               result->file_number_operand_counts,
                                               result->file_function_operator_counts,
                                               result->file_nonfunction_operator_counts);
            if (status != 0)
                return 1;
        }

        curr_line_idx++;
    }

    // Calculator halsteads volume for file
    // but first sum up all the per-function operators and operands to get the counts for the whole file
    SumUp(result->func_string_and_char_operand_counts, result->file_string_and_char_operand_counts);
    SumUp(result->func_word_operand_counts, result->file_word_operand_counts);
    SumUp(result->func_bool_operand_counts, result->file_bool_operand_counts);
    SumUp(result->func_number_operand_counts, result->file_number_operand_counts);

    // eg + - == and keywords
    SumUp(result->func_nonfunction_operator_counts, result->file_nonfunction_operator_counts);

    // eg printf
    SumUp(result->func_function_operator_counts, result->file_function_operator_counts);

    // // Calculate Halsteads volume of the file
    result->halsteads_volume = CalculateHalsteadsVolumeFromCounts(result->file_string_and_char_operand_counts,
                                                                  result->file_word_operand_counts,
                                                                  result->file_bool_operand_counts,
                                                                  result->file_number_operand_counts,
                                                                  result->file_function_operator_counts,
                                                                  result->file_nonfunction_operator_counts);

    if (result->halsteads_volume < 0)
        encountered_error = true;

    if (encountered_error)
        return 1;

    return 0;
}

//Modification: 2018.01 Integration ends

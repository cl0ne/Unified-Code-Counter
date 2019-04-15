//! Code counter class methods for the Python language.
/*!
* \file CPythonCounter.cpp
*
* This file contains the code counter class methods for the Python language.
*/

#include "CPythonCounter.h"
#include "UCCExceptDump.h"		// Modification: 2015.12

/*!
* Constructs a CPythonCounter object.
*/
CPythonCounter::CPythonCounter()
{
	classtype = PYTHON;
	language_name = "Python";
    
    //Modification: 11.2016 Ext-4
    file_extension = CUtil::getExtensionsToLanguage("Python", file_extension);
	//file_extension.push_back(".py");

	BlockCommentStart.push_back("\"\"\"");
	BlockCommentEnd.push_back("\"\"\"");
	BlockCommentStart.push_back("'''");
	BlockCommentEnd.push_back("'''");
	LineCommentStart.push_back("#");
	QuoteStart = "\"\'";
	QuoteEnd = "\"\'";
	QuoteEscapeRear = '\\';

	loop_keywords.push_back("for");
	loop_keywords.push_back("while");

	directive.push_back("do");
	directive.push_back("from");
	directive.push_back("import");
	directive.push_back("no");
	directive.push_back("package");
	directive.push_back("use");
	directive.push_back("require");

	exec_name_list.push_back("and");
	exec_name_list.push_back("as");
	exec_name_list.push_back("assert");
	exec_name_list.push_back("break");
	exec_name_list.push_back("continue");
	exec_name_list.push_back("def");
	exec_name_list.push_back("del");
	exec_name_list.push_back("elif");
	exec_name_list.push_back("else");
	exec_name_list.push_back("except");
	exec_name_list.push_back("exec");
	exec_name_list.push_back("exit");
	exec_name_list.push_back("finally");
	exec_name_list.push_back("for");
	exec_name_list.push_back("global");
	exec_name_list.push_back("if");
	exec_name_list.push_back("in");
	exec_name_list.push_back("is");
	exec_name_list.push_back("lambda");
	exec_name_list.push_back("not");
	exec_name_list.push_back("or");
	exec_name_list.push_back("pass");
	exec_name_list.push_back("print");
	exec_name_list.push_back("raise");
	exec_name_list.push_back("return");
	exec_name_list.push_back("try");
	exec_name_list.push_back("while");
	exec_name_list.push_back("with");
	exec_name_list.push_back("yield");

	math_func_list.push_back("math.ceil");
	math_func_list.push_back("math.copysign");
	math_func_list.push_back("math.degrees");
	math_func_list.push_back("math.e");
	math_func_list.push_back("math.exp");
	math_func_list.push_back("math.fabs");
	math_func_list.push_back("math.factorial");
	math_func_list.push_back("math.floor");
	math_func_list.push_back("math.fmod");
	math_func_list.push_back("math.frexp");
	math_func_list.push_back("math.fsum");
	math_func_list.push_back("math.hypot");
	math_func_list.push_back("math.ldexp");
	math_func_list.push_back("math.modf");
	math_func_list.push_back("math.pi");
	math_func_list.push_back("math.pow");
	math_func_list.push_back("math.radians");
	math_func_list.push_back("math.sqrt");
	math_func_list.push_back("math.trunc");
	math_func_list.push_back("cmath.phase");
	math_func_list.push_back("cmath.polar");
	math_func_list.push_back("cmath.rect");
	
	trig_func_list.push_back("math.acos");
	trig_func_list.push_back("math.acosh");
	trig_func_list.push_back("math.asinh");
	trig_func_list.push_back("math.atanh");
	trig_func_list.push_back("math.asin");
	trig_func_list.push_back("math.atan");
	trig_func_list.push_back("math.atan2");
	trig_func_list.push_back("math.cos");
	trig_func_list.push_back("math.cosh");
	trig_func_list.push_back("math.sin");
	trig_func_list.push_back("math.sinh");
	trig_func_list.push_back("math.tan");
	trig_func_list.push_back("math.tanh");

	log_func_list.push_back("math.log");
	log_func_list.push_back("math.log10");
	log_func_list.push_back("math.log1p");

	cmplx_calc_list.push_back("+");
	cmplx_calc_list.push_back("-");
	cmplx_calc_list.push_back("*");
	cmplx_calc_list.push_back("/");
	cmplx_calc_list.push_back("%");
	cmplx_calc_list.push_back("**");

	cmplx_cond_list.push_back("elif");
	cmplx_cond_list.push_back("else");
	cmplx_cond_list.push_back("except");
	cmplx_cond_list.push_back("for");
	cmplx_cond_list.push_back("if");
	cmplx_cond_list.push_back("try");
	cmplx_cond_list.push_back("while");

	cmplx_logic_list.push_back("==");
	cmplx_logic_list.push_back("!=");
	cmplx_logic_list.push_back(">");
	cmplx_logic_list.push_back("<");
	cmplx_logic_list.push_back(">=");
	cmplx_logic_list.push_back("=<");

	cmplx_assign_list.push_back("=");

   	cmplx_cyclomatic_list.push_back("if");
	cmplx_cyclomatic_list.push_back("elif");
	cmplx_cyclomatic_list.push_back("for");
	cmplx_cyclomatic_list.push_back("while");
	cmplx_cyclomatic_list.push_back("except");
	//cmplx_cyclomatic_list.push_back("case");

	cmplx_cyclomatic_logic_list.push_back("and");
	cmplx_cyclomatic_logic_list.push_back("or");
	
	//cmplx_cyclomatic_case_list.push_back("case");
}

/*!
* Replaces up to ONE quoted string inside a string starting at idx_start.
* Uses a string instead of a character to allow processing multi-line
*  literals """ and '''.
*
* \param strline string to be processed
* \param idx_start index of line character to start search
* \param contd specifies the quote string is continued from the previous line
* \param CurrentQuoteEnd end quote string of the current status
*
* \return method status
*/
int CPythonCounter::ReplaceQuote(string &strline, size_t &idx_start, bool &contd, string &CurrentQuoteEnd)
{
	size_t idx_end, idx_quote;

	if (contd)
	{
		// python: use string instead of character to check for """ and '''
		idx_start = 0;
		if (strline.length() >= CurrentQuoteEnd.length() &&
			strline.substr(0, CurrentQuoteEnd.length()) == CurrentQuoteEnd)
		{
			idx_start = CurrentQuoteEnd.length();
			contd = false;
			return 1;
		}
		strline[0] = '$';
	}
	else
	{
		// handle two quote chars in some languages, both " and ' may be accepted
		idx_start = FindQuote(strline, QuoteStart, idx_start, QuoteEscapeFront);
		if (idx_start != string::npos)
		{
			idx_quote = QuoteStart.find_first_of(strline[idx_start]);
			CurrentQuoteEnd = QuoteEnd[idx_quote];
			// python: check for """ or '''
			if (strline.length() >= idx_start + 3)
			{
				if (CurrentQuoteEnd == "\"")
				{
					if (strline.substr(idx_start, 3) == "\"\"\"")
						CurrentQuoteEnd = "\"\"\"";
				}
				else if (CurrentQuoteEnd == "'")
				{
					if (strline.substr(idx_start, 3) == "'''")
						CurrentQuoteEnd = "'''";
				}
			}
		}
		else
		{
			idx_start = strline.length();
			return 0;
		}
	}

	// python: handle """ and '''
	if (CurrentQuoteEnd.length() == 3)
	{
		if (idx_start + 3 >= strline.length())
			idx_end = string::npos;
		else
		{
			idx_end = strline.find(CurrentQuoteEnd, idx_start + 3);
			if (idx_end != string::npos)
				idx_end += 2;	// shift to last quote character
		}
	}
	else
		idx_end = CUtil::FindCharAvoidEscape(strline, CurrentQuoteEnd[0], idx_start + 1, QuoteEscapeFront);
	if (idx_end == string::npos)
	{
		idx_end = strline.length() - 1;
		strline.replace(idx_start + 1, idx_end - idx_start, idx_end - idx_start, '$');
		contd = true;
		idx_start = idx_end + 1;
	}
	else
	{
		if (CurrentQuoteEnd.length() != 3 && (QuoteEscapeRear) && (strline.length() > idx_end + 1) && (strline[idx_end+1] == QuoteEscapeRear))
		{
			strline[idx_end] = '$';
			strline[idx_end+1] = '$';
		}
		else
		{
			contd = false;
			strline.replace(idx_start + 1, idx_end - idx_start - 1, idx_end - idx_start - 1, '$');
			idx_start = idx_end + 1;
		}
	}
	return 1;
}

/*!
* Counts the number of comment lines, removes comments, and
* replaces quoted strings by special chars, e.g., $
* All arguments are modified by the method.
* Special processing for """ and ''' which can be multi-line literal
*  or a multi-line comment if it stands alone.
*
* \param fmap list of processed file lines
* \param result counter results
* \param fmapBak list of original file lines (same as fmap except it contains unmodified quoted strings)
*
* \return method status
*/
int CPythonCounter::CountCommentsSLOC(filemap* fmap, results* result, filemap *fmapBak)
{
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
	string curBlckCmtStart, curBlckCmtEnd, tmp;
	string CurrentQuoteEnd = "";
	bool quote_contd = false;
	filemap::iterator itfmBak = fmapBak->begin();

	quote_idx_start = 0;

	for (filemap::iterator iter = fmap->begin(); iter != fmap->end(); iter++, itfmBak++)
	{
		contd_nextline = false;

		quote_idx_start = 0;
		idx_start = 0;

		if (CUtil::CheckBlank(iter->line))
			continue;
		if (quote_contd)
		{
			// Replace quote until next character
			ReplaceQuote(iter->line, quote_idx_start, quote_contd, CurrentQuoteEnd);
			if (quote_contd)
				continue;
		}

		if (contd)
			comment_type = 3;

		while (!contd_nextline && idx_start < iter->line.length())
		{
			// need to handle multiple quote chars in some languages, both " and ' may be accepted
			quote_idx_start = FindQuote(iter->line, QuoteStart, quote_idx_start, QuoteEscapeFront);
			comment_start = idx_start;
			if (!contd)
			{
				FindCommentStart(iter->line, comment_start, comment_type, curBlckCmtStart, curBlckCmtEnd);
				if (comment_start != string::npos && comment_type > 2)
				{
					// python: check whether this is a multi-line literal or a block comment
					tmp = CUtil::TrimString(iter->line, -1);
					if (iter->line.length() - tmp.length() != comment_start)
					{
						quote_idx_start = comment_start;
						comment_start = string::npos;
					}
				}
			}

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
					cout << "Error in CountCommentsSLOC()" << endl;
					break;
				}
			}
		}
	}
	return 1;
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
int CPythonCounter::CountDirectiveSLOC(filemap* fmap, results* result, filemap* fmapBak)
{
	bool contd = false, trunc_flag = false;
	size_t idx, strSize;
	unsigned int cnt = 0;
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$";
	string strDirLine = "";

	filemap::iterator itfmBak = fmapBak->begin();
	for (filemap::iterator iter = fmap->begin(); iter != fmap->end(); iter++, itfmBak++)
	{
		if (CUtil::CheckBlank(iter->line))
			continue;
		size_t lineNumber = iter->lineNumber;

		if (print_cmplx)
		{
			cnt = 0;
			CUtil::CountTally(" " + iter->line, directive, cnt, 1, exclude, "", "", &result->directive_count);
		}

		if (!contd)
		{
			// if not a continuation of a previous directive
			for (vector<string>::iterator viter = directive.begin(); viter != directive.end(); viter++)
			{
				if ((idx = iter->line.find((*viter), 0)) != string::npos && idx == 0)
				{
					contd = true;
					break;
   		        }
        	}
			if (contd)
			{
				strSize = CUtil::TruncateLine(itfmBak->line.length(), 0, this->lsloc_truncate, trunc_flag);
				if (strSize > 0)
					strDirLine = itfmBak->line.substr(0, strSize);
				result->directive_lines[PHY]++;
			}
 		}
		else
		{
			// continuation of a previous directive
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
			if (iter->line[iter->line.length()-1] != '_')
			{
				contd = false;
				if (result->addSLOC(strDirLine, lineNumber, trunc_flag))
					result->directive_lines[LOG]++;
			}
			iter->line = "";
		}
	}
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
int CPythonCounter::LanguageSpecificProcess(filemap* fmap, results* result, filemap* fmapBak)
{
	unsigned int	paren_count			= 0;
	string			strLSLOC			= "";
	string			strLSLOCBak			= "";

	filemap::iterator fit, fitbak;
	string line, lineBak;
	UIntVector loopWhiteSpace;

	unsigned int cnt = 0;
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$";

	for (fit = fmap->begin(), fitbak = fmapBak->begin(); fit != fmap->end(); fit++, fitbak++)
	{
		line = fit->line;
		lineBak = fitbak->line;

		// do not process blank lines
		// blank line means blank_line/comment_line/directive
		if (!CUtil::CheckBlank(line))
		{
			// does the ReplaceQuote get the continuation character \ replaced?
			LSLOC(result, line, fit->lineNumber, lineBak, strLSLOC, strLSLOCBak, paren_count, loopWhiteSpace);

			if (print_cmplx)
			{
				cnt = 0;
				CUtil::CountTally(line, exec_name_list, cnt, 1, exclude, "", "", &result->exec_name_count);
			}
		}
	}
	return 1;
}

// Logical Counting Consideration
/*
Not counted line:
else:
\ (continuation char)
in ()
in []
in {}
end of line preceding by operator characters + - * / = < > | & is in % ^ \ ~ not , : 
compound statement, with : and ;
esp. compound statement with : in middle line, not in () [] {} or in else:
*/

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
* \param loopWhiteSpace count of white space to determine loop ends
*/
void CPythonCounter::LSLOC(results* result, string line, size_t lineNumber, string lineBak, string &strLSLOC, string &strLSLOCBak,
						   unsigned int &paren_cnt, UIntVector &loopWhiteSpace)
{
#define CONT_STR_LENGTH 18
	string continuation_str[] = {"is", "in", "not", "+", "-", "*", "/", "=", "<", ">", "|", "&", "%", "^", "\\", "~", ",", "$"};
	
	size_t start = 0;	// starting index of the working string
	size_t i = 0, idx, strSize;
	int n;
	bool trunc_flag = false;
	unsigned int cnt = 0, numWS;
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$";

	string tmp;

	// process:
	// paren_cnt is used with {} [] ()
	// 1. check if the  current char is in one of the parentheses
	// 2.   if no, check if the line has : or ; (statement separators), except else:
	// 3.      if yes, count and put the statement in the result
	// 4. if the line does not ends with a continuation string or a statement separator (handled)
	//      and the line is not in one of the parentheses
	//      then count and put the statement in the result
	// 5. physical count considers all lines executables (or directives, no declarations)

	// check for loop ends, new loops, and record white space in order to determine ends
	if (print_cmplx)
	{
		// check white space for loop ends
		if (loopWhiteSpace.size() > 0)
		{
			// get white space
			tmp = line;
			tmp = CUtil::TrimString(tmp, -1);
			numWS = (unsigned)(line.length() - tmp.length());

			// check for loop ends
			for (n = (int)loopWhiteSpace.size() - 1; n >= 0; n--)
			{
				if (loopWhiteSpace.at( (unsigned)n ) != numWS)
					break;
				else
					loopWhiteSpace.pop_back();
			}
		}

		// check for loop keywords (for, while)
		cnt = 0;
		CUtil::CountTally(line, loop_keywords, cnt, 1, exclude, "", "", NULL);
		if (cnt > 0)
		{
			if (loopWhiteSpace.size() < 1)
			{
				// get white space
				tmp = line;
				tmp = CUtil::TrimString(tmp, -1);
				numWS = (unsigned)(line.length() - tmp.length());
			}

			// add nested loop white space and record nested loop level
			for (i = 0; i < cnt; i++)
			{
				loopWhiteSpace.push_back(numWS);

				if ((unsigned int)result->cmplx_nestloop_count.size() < loopWhiteSpace.size())
					result->cmplx_nestloop_count.push_back(1);
				else
					result->cmplx_nestloop_count[loopWhiteSpace.size()-1]++;
			}
		}
	}

	line = CUtil::TrimString(line);
	lineBak = CUtil::TrimString(lineBak);
	size_t line_length = line.length();
	bool lineContinued = false;

	while (i < line_length)
	{
		switch (line[i])
		{
		case '{': case '[': case '(': // parentheses opener
			paren_cnt++;
			break;
		case '}': case ']': case ')': // parentheses closer
			if (paren_cnt > 0)
				paren_cnt--;
			break;
		}

		// 2. if no parentheses enclosing, and if the char is a statement separator
		if (paren_cnt == 0 && (line[i] == ';' || line[i] == ':'))
		{
			tmp = CUtil::ClearRedundantSpaces(line);
			// if line[..i] is else: then exit the outer if
			if (tmp.rfind("else:") != tmp.length() - 5)
			{
				// 3.
				strSize = CUtil::TruncateLine(i + 1 - start, strLSLOC.length(), this->lsloc_truncate, trunc_flag);
				if (strSize > 0)
				{
					strLSLOC += line.substr(start, i);
					strLSLOCBak += lineBak.substr(start, i);
				}
				if (result->addSLOC(strLSLOCBak, lineNumber, trunc_flag))
				{
					// increase logical SLOC here
					result->exec_lines[LOG]++;
				}
				strLSLOC = strLSLOCBak = "";
				start = i + 1;
			}
			else
				lineContinued = true;
		}
		i++;
	}

	if (paren_cnt == 0)
	{
		// add logical SLOC if the line does not end with a continuation string/char
		if (!lineContinued)
		{
			for (i = 0; i < CONT_STR_LENGTH; i++)
			{
				if (continuation_str[i].length() == 1)
				{
					if (line[line_length - 1] == continuation_str[i][0])
					{
						lineContinued = true;
						break;
					}
				}
				else
				{
					idx = CUtil::FindKeyword(line, continuation_str[i]);
					if (idx != string::npos && idx == line_length - continuation_str[i].length() - 1)
					{
						lineContinued = true;
						break;
					}
				}
			}
		}

		if (!lineContinued)
		{
			strSize = CUtil::TruncateLine(line_length - start, strLSLOC.length(), this->lsloc_truncate, trunc_flag);
			if (strSize > 0)
			{
				strLSLOC += line.substr(start, line_length);
				strLSLOCBak += lineBak.substr(start, line_length);
			}
			if (result->addSLOC(strLSLOCBak, lineNumber, trunc_flag))
			{
				// increase logical SLOC here
				result->exec_lines[LOG]++;
			}
			strLSLOC = strLSLOCBak = "";
		}
		else
		{
			tmp = CUtil::TrimString(line.substr(start, line_length - start));
			strSize = CUtil::TruncateLine(tmp.length(), strLSLOC.length(), this->lsloc_truncate, trunc_flag);
			if (strSize > 0)
			{
				strLSLOC += tmp.substr(0, strSize);
				tmp = CUtil::TrimString(lineBak.substr(start, line_length - start));
				strLSLOCBak += tmp.substr(0, strSize);
			}
		}
	}
	result->exec_lines[PHY]++;
}

/*!
 * Counts file language complexity based on specified language keywords/characters.
 *
 * \param fmap list of processed file lines
 * \param result counter results
 *
 * \return method status
 */
int CPythonCounter::CountComplexity(filemap* fmap, results* result)
{
	if (classtype == UNKNOWN || classtype == DATAFILE)
		return 0;
	
	ENTER_1( "Start" );		// Due to Polymorphism change level above
//	try
//	{

	filemap::iterator fit;
	unsigned int cnt, ret, cyclomatic_cnt = 0, ignore_cyclomatic_cnt = 0, main_cyclomatic_cnt = 1, /*function_count = 0, */cyclomatic_logic_cnt = 0, main_cyclomatic_logic_cnt = 1; // warning fix
    bool isMainExisted = false;
	string line, needIndentation = "", file_ext, function_name = "";
    	vector<int> indenStack;
    	indenStack.push_back(0);
    	int numWS;
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$";
	StringVector function_stack;
	stack<unsigned int> cyclomatic_stack;
	stack<unsigned int> cyclomatic_logic_stack;

// from above we know Python has Cyclomatic Complexity keywords with no skip

	SAVE_TO_2( "Start CPythonCounter::CountComplexity" );

	// process each line
	bool notEndFile = true;
	filemap::iterator fitEnd = fmap->end();
	currentPhyLine = 0;
	for (fit = fmap->begin(); notEndFile; fit++)
	{
		currentPhyLine++;
        	if ( fitEnd == fit ) {
            		notEndFile = false;
        	}
        	if (!notEndFile) {
           		 line = " endofmain";
            		//to handle last line of a file belongs to one function:
            		//                                  return(1) --> end of function.
        	}else{
            		line = fit->line;
				SAVE_TO_2( "CUtil::CheckBlank" );
           	 	if (CUtil::CheckBlank(line))
                		continue;
            
            		line = " " + line;
        	}
        
        	//cout << line << "meiyige";
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
		SAVE_TO_2( "CUtil::CountTally Logarithmic" );
		CUtil::CountTally(line, log_func_list, cnt, 1, exclude, "", "", &result->log_func_count, casesensitive);
		result->cmplx_logarithm_lines += cnt;
        
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
        
	// Pointers not in Python

// Python has Cyclomatic Complexity keywords with no skip
// cyclomatic complexity

		// search for cyclomatic complexity keywords
		SAVE_TO_2( "CUtil::CountTally Cyclomatic Complexity" );
		CUtil::CountTally(line, cmplx_cyclomatic_list, cyclomatic_cnt, 1, exclude, "", "", 0, casesensitive);
            
		// Python does not have Cyclomatic Complexity keywords to exclude
	/*
		if (ignore_cmplx_cyclomatic_list.size() > 0)
		{
			SAVE_TO_2( "CUtil::CountTally exclude keywords" );
			CUtil::CountTally(line, ignore_cmplx_cyclomatic_list, ignore_cyclomatic_cnt, 1, exclude, "", "", 0, casesensitive);
		}
	*/

		// Python has cyclomatic complexity logical keywords
		SAVE_TO_2( "CUtil::CountTally complexity Logical" );
		CUtil::CountTally(line, cmplx_cyclomatic_logic_list, cyclomatic_logic_cnt, 1, exclude, "", "", 0, casesensitive);
           
		// parse function name if found
		SAVE_TO_2( "ParseFunctionName" );
		ret = (unsigned)ParseFunctionName(line, needIndentation, function_stack, function_name, indenStack, numWS);
            
		if (ret != 1 && !cyclomatic_stack.empty() && cyclomatic_stack.size() == function_stack.size()-1)
		{
			// remove count stack entry for non-function names
			cyclomatic_cnt += cyclomatic_stack.top();
			ignore_cyclomatic_cnt = 0;
			cyclomatic_stack.pop();
		}
		if (ret != 1 && !cyclomatic_logic_stack.empty() && cyclomatic_logic_stack.size() == function_stack.size()-1)
		{
			// remove count stack entry for non-function names
			cyclomatic_logic_cnt += cyclomatic_logic_stack.top();
			cyclomatic_logic_stack.pop();
		}

		// modify the condition when comes to function_stack.size() [add 1]
		if (ret == 1) // and modify this part
		{
                //this line is not in the current function, handle it later
                int remember = (int)( cyclomatic_cnt - ignore_cyclomatic_cnt + 1 );
                int remember2 = remember + (int)cyclomatic_logic_cnt;

      
                cyclomatic_cnt = 0;
                ignore_cyclomatic_cnt = 0;
		cyclomatic_logic_cnt = 0;
		SAVE_TO_2( "CUtil::CountTally Cyclomatic" );
                CUtil::CountTally(line, cmplx_cyclomatic_list, cyclomatic_cnt, 1, exclude, "", "", 0, casesensitive);
                
			// Python does not have Cyclomatic Complexity keywords to exclude
			/*
                // get this line's complexity
                if (ignore_cmplx_cyclomatic_list.size() > 0)
				{
					SAVE_TO_2( "CUtil::CountTally Ignore Cyclomatic" );
                    	CUtil::CountTally(line, ignore_cmplx_cyclomatic_list, ignore_cyclomatic_cnt, 1, exclude, "", "", 0, casesensitive);
				}
			*/

		// Python has Complex Cyclomatic logic keywords
		SAVE_TO_2( "CUtil::CountTally Cyclomatic Logic" );
		CUtil::CountTally(line, cmplx_cyclomatic_logic_list, cyclomatic_logic_cnt, 1, exclude, "", "", 0, casesensitive);

		//substract the current line's complexity
		remember  -= cyclomatic_cnt - ignore_cyclomatic_cnt;
		remember2 -= cyclomatic_cnt - ignore_cyclomatic_cnt + cyclomatic_logic_cnt;

		// capture count at end of function
                lineElement element((unsigned)remember, function_name);
                result->cmplx_cycfunct_count.push_back(element);
		lineElement n_element((unsigned)remember2, function_name);
		result->cmplx_cycfunct_CC2_count.push_back(n_element);

		function_name = "";
		if (function_stack.size() > 1)
		{
			// grab previous function from stack to continue
			if (!cyclomatic_stack.empty())
			{
                        int flag = 0;
                        //both the stacks' first element is redundant so " ** > 1 "
                        while (indenStack.size() > 1 && function_stack.size() > 1) {
                            	if (flag == 1) {
                                	indenStack.pop_back();
                                	function_stack.pop_back();
                            	}
                            	if (numWS >= indenStack.back()) {
                                	//the current line belongs to one of the functions in the function_stack
                                	break;
                            	}
                            				
                            	int temp = (int)cyclomatic_stack.top() + 1;
                            	cyclomatic_stack.pop();
				int temp2 = (int)cyclomatic_logic_stack.top();
				cyclomatic_logic_stack.pop();
                            	string str = function_stack.back();
                            
                            	size_t idx = str.find("(");
                            	if (idx != string::npos)
                            	{
									SAVE_TO_2( "CUtil::ClearRedundantSpaces" );
                                	function_name = CUtil::ClearRedundantSpaces(str.substr(0, idx));
                            	}
                            	lineElement element( (unsigned)temp, function_name);
                            	result->cmplx_cycfunct_count.push_back( element );
				lineElement n_element( (unsigned)( temp + temp2 ), function_name);
				result->cmplx_cycfunct_CC2_count.push_back(n_element);

                            	function_name = "";

                            	flag = 1;
                        	}
                        	if (indenStack.size() > 1) 
							{
                            		//the current line belongs to one of the functions in the function_stack
                            		cyclomatic_cnt += cyclomatic_stack.top() - ignore_cyclomatic_cnt;
                            		cyclomatic_stack.pop();
					cyclomatic_logic_cnt += cyclomatic_logic_stack.top();
					cyclomatic_logic_stack.pop();
                        	}else{
                            		//the current line not belongs to any of the functions in the function_stack, so it should belong to main
                            		if (line != " endofmain") 
									{
                                		main_cyclomatic_cnt += cyclomatic_cnt - ignore_cyclomatic_cnt;
						main_cyclomatic_logic_cnt += cyclomatic_cnt - ignore_cyclomatic_cnt + cyclomatic_logic_cnt;
                                		isMainExisted = true;
                            		}
                            		cyclomatic_cnt = cyclomatic_logic_cnt = 0;
                        	}
			}
		}
			else{
                    		if (line != " endofmain") {
                                	main_cyclomatic_cnt += cyclomatic_cnt - ignore_cyclomatic_cnt;
					main_cyclomatic_logic_cnt += cyclomatic_cnt - ignore_cyclomatic_cnt + cyclomatic_logic_cnt;
                        		isMainExisted = true;
                    		}
                            	cyclomatic_cnt = cyclomatic_logic_cnt = 0;
                	}
			ignore_cyclomatic_cnt = 0;
		}
		else if (ret == 2)
		{
			// some code doesn't belong to any function
			if (line != " endofmain") {
                                main_cyclomatic_cnt += cyclomatic_cnt - ignore_cyclomatic_cnt;
				main_cyclomatic_logic_cnt += cyclomatic_cnt - ignore_cyclomatic_cnt + cyclomatic_logic_cnt;
                    		isMainExisted = true;
                    
                	}
			cyclomatic_cnt = ignore_cyclomatic_cnt = cyclomatic_logic_cnt = 0;
		}
		else { 
			if (function_stack.size() > 1 && (function_stack.size() - 1 > cyclomatic_stack.size() + 1 || (cyclomatic_stack.empty() && function_stack.size() - 1 > 1)))
			{
				// capture previous complexity count from open function
				cyclomatic_stack.push(cyclomatic_cnt - ignore_cyclomatic_cnt);
				cyclomatic_cnt = ignore_cyclomatic_cnt = 0;
			}
			if (function_stack.size() > 1 && (function_stack.size() - 1 > cyclomatic_logic_stack.size() + 1 || (cyclomatic_logic_stack.empty() && function_stack.size() - 1 > 1)))
			{
				// capture previous complexity count from open function
				cyclomatic_logic_stack.push(cyclomatic_logic_cnt);
				cyclomatic_logic_cnt = 0;
			}
			}

		// Avoid going past end.
		if ( fitEnd == fit )
			break;
	}
    
	// done with a file, if has "main" code add it
	if (isMainExisted)
	{
		lineElement element(main_cyclomatic_cnt, "main");
		lineElement n_element(main_cyclomatic_logic_cnt, "main");
		result->cmplx_cycfunct_count.push_back(element);
		result->cmplx_cycfunct_CC2_count.push_back(n_element);
	}

/*
	} 
	catch(const std::exception& e)
	{
		string	dumpInfo;
		string	what = e.what();
		StackDump( dumpInfo, EXCEPTION_HANDLER_PARSER, this->parse_threadIdx, EXCEPTION_STD_EXCEPTION, what, this );
		throw( e );
	}
*/
	return 1;
}

/*!
 * Parses lines for function/method names.
 *
 * \param line line to be processed
 * \param needIndentation last line processed
 * \param functionStack stack of functions
 * \param functionName function name found
 *
 * \return 1 if function name is found
 */
int CPythonCounter::ParseFunctionName(const string &line, string & needIndentation, StringVector &functionStack, string &functionName, vector<int> &indenStack, int & numWS)
{
    	string str;
	size_t idx;
    	int functionWS = 0;
    	if(indenStack.empty()){
        	indenStack.push_back(0);
    	}
    	functionWS = indenStack.back(); // current function's indentation
    	if (functionStack.empty()) {        
        	functionStack.push_back("");
        	needIndentation = "";
    	}
    
    	string tmp;
    	//get white spaces
    	tmp = line;
    	tmp = CUtil::TrimString(tmp, -1);
    
    	numWS = (int)( line.length() - tmp.length() );

    	//last line is with "def", if needIndentation=="YES"
    	//not consider the body of function is in the same line of "def"
    	if(needIndentation.length() >= 3 && needIndentation == "YES"){
        	indenStack.push_back(numWS);
        	needIndentation = "";
    	}
    	string mark = needIndentation; 
    
	idx = CUtil::FindKeyword(line, "def");
	if (idx != string::npos)
	{
		if (idx + 4 < line.length())
		{
			str = line.substr(idx + 4);
			functionStack.push_back(str);
            		needIndentation = "YES"; //indicate next line need to get the indentation
		}
	}
    if (functionStack.size() == 1 || functionStack.size() == 0)
	{
		// dealing with some code out of any subroutines, it is a "main" code
		return 2;
	}
    
    	if(mark != "YES"){
        	if (functionWS > numWS && functionStack.size() > 1 ) {
		    	//the first element in functionStack is ""
		    
		    	//we need to get the function and pop the function from the functionStack
			if(needIndentation=="YES"){
				//we have already pushed the new function to the functionStack
				string tempp = functionStack.back(); //pop new function
				functionStack.pop_back();
				
				str = functionStack.back();
				functionStack.pop_back();
				
				functionStack.push_back(tempp); // push new function
				indenStack.pop_back(); //pop the function's indentation in the stack
			}
			else{
		        	str = functionStack.back();
		        	functionStack.pop_back();
		        	indenStack.pop_back(); //pop the function's indentation in the stack

			}
		   
			//get function's name
			idx = str.find("(");
			if (idx != string::npos)
			{
				functionName = CUtil::ClearRedundantSpaces(str.substr(0, idx));
				return 1;
			}
        	}
	}
	return 0;    
}

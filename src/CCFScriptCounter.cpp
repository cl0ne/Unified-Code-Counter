//! Code counter class methods for the CFScript language.
/*!
* \file CCFScriptCounter.cpp
*
* This file contains the code counter class methods for the CFScript language.
*/

#include "CCFScriptCounter.h"

/*!
* Constructs a CCFScriptCounter object.
*/
CCFScriptCounter::CCFScriptCounter()
{
	classtype = CFSCRIPT;
	language_name = "CFScript";
	casesensitive = false;
    
    //Modification: 11.2016 Ext-4
    file_extension = CUtil::getExtensionsToLanguage("CFScript", file_extension);

	//file_extension.push_back(".cfs");

	QuoteStart = "\"'";
	QuoteEnd = "\"'";
	LineCommentStart.push_back("//");
	BlockCommentStart.push_back("/*");
	BlockCommentEnd.push_back("*/");

	data_name_list.push_back("function");
	data_name_list.push_back("import");
	data_name_list.push_back("include");
	data_name_list.push_back("interface");
	data_name_list.push_back("property");
	data_name_list.push_back("var");

	exec_name_list.push_back("abort");
	exec_name_list.push_back("break");
	exec_name_list.push_back("case");
	exec_name_list.push_back("catch");
	exec_name_list.push_back("component");
	exec_name_list.push_back("continue");
	exec_name_list.push_back("createobject");
	exec_name_list.push_back("default");
	exec_name_list.push_back("else");
	exec_name_list.push_back("exit");
	exec_name_list.push_back("finally");
	exec_name_list.push_back("for");
	exec_name_list.push_back("if");
	exec_name_list.push_back("location");
	exec_name_list.push_back("lock");
	exec_name_list.push_back("new");
	exec_name_list.push_back("param");
	exec_name_list.push_back("pageecoding");
	exec_name_list.push_back("rethrow");
	exec_name_list.push_back("return");
	exec_name_list.push_back("savecontent");
	exec_name_list.push_back("switch");
	exec_name_list.push_back("thread");
	exec_name_list.push_back("throw");
	exec_name_list.push_back("trace");
	exec_name_list.push_back("transaction");
	exec_name_list.push_back("try");
	exec_name_list.push_back("while");
	exec_name_list.push_back("writedump");
	exec_name_list.push_back("writelog");
	exec_name_list.push_back("writeoutput");

	math_func_list.push_back("abs");
	math_func_list.push_back("arrayavg");
	math_func_list.push_back("arraysum");
	math_func_list.push_back("ceiling");
	math_func_list.push_back("decrementvalue");
	math_func_list.push_back("exp");
	math_func_list.push_back("fix");
	math_func_list.push_back("incrementvalue");
	math_func_list.push_back("int");
	math_func_list.push_back("max");
	math_func_list.push_back("min");
	math_func_list.push_back("mod");
	math_func_list.push_back("pi");
	math_func_list.push_back("precisionevaluate");
	math_func_list.push_back("rand");
	math_func_list.push_back("randomize");
	math_func_list.push_back("randrange");
	math_func_list.push_back("round");
	math_func_list.push_back("sgn");
	math_func_list.push_back("sqr");

	trig_func_list.push_back("acos");
	trig_func_list.push_back("asin");
	trig_func_list.push_back("atn");
	trig_func_list.push_back("cos");
	trig_func_list.push_back("sin");
	trig_func_list.push_back("tan");

	log_func_list.push_back("log");
	log_func_list.push_back("log10");

	cmplx_calc_list.push_back("+");
	cmplx_calc_list.push_back("-");
	cmplx_calc_list.push_back("*");
	cmplx_calc_list.push_back("/");
	cmplx_calc_list.push_back("**");

	cmplx_cond_list.push_back("case");
	cmplx_cond_list.push_back("else");
	cmplx_cond_list.push_back("for");
	cmplx_cond_list.push_back("if");
	cmplx_cond_list.push_back("switch");
	cmplx_cond_list.push_back("while");
	cmplx_cond_list.push_back("?");

	cmplx_logic_list.push_back("==");
	cmplx_logic_list.push_back("!=");
	cmplx_logic_list.push_back(">");
	cmplx_logic_list.push_back("<");
	cmplx_logic_list.push_back(">=");
	cmplx_logic_list.push_back("=<");
	cmplx_logic_list.push_back("eq");
	cmplx_logic_list.push_back("neq");
	cmplx_logic_list.push_back("gt");
	cmplx_logic_list.push_back("lt");
	cmplx_logic_list.push_back("gte");
	cmplx_logic_list.push_back("lte");

	cmplx_assign_list.push_back("=");

	cmplx_cyclomatic_list.push_back("if");
	cmplx_cyclomatic_list.push_back("for");
	cmplx_cyclomatic_list.push_back("case");
	cmplx_cyclomatic_list.push_back("elif");
	cmplx_cyclomatic_list.push_back("while");
	//cmplx_cyclomatic_list.push_back("switch");
	cmplx_cyclomatic_list.push_back("?");

	//there is no bollean operators in CFScript, so no CC2 results.

	cmplx_cyclomatic_case_list.push_back("case");
	cmplx_cyclomatic_switch_list.push_back("switch");
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
int CCFScriptCounter::LanguageSpecificProcess(filemap* fmap, results* result, filemap* fmapBak)
{
	unsigned int	paren_count			= 0;
	bool			for_flag			= false;
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

	// two strings used for string match
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$";

	for (fit = fmap->begin(), fitbak = fmapBak->begin(); fit != fmap->end(); fit++, fitbak++)
	{
		line = fit->line;

		// insert blank at the beginning(for searching keywords
		line = ' ' + line;
		lineBak = ' ' + fitbak->line;

		// do not process blank lines
		// blank line means blank_line/comment_line/directive
		if (!CUtil::CheckBlank(line))
		{
			LSLOC(result, line, fit->lineNumber, lineBak, strLSLOC, strLSLOCBak, paren_count, for_flag, found_forifwhile,
				found_while, prev_char, data_continue, temp_lines, phys_exec_lines, phys_data_lines, inArrayDec,
				openBrackets, loopLevel);

			if (print_cmplx)
			{
				cnt = 0;
				CUtil::CountTally(line, exec_name_list, cnt, 1, exclude, "", "", &result->exec_name_count, false);
			}

			result->exec_lines[PHY] += phys_exec_lines;
			phys_exec_lines = 0;

			result->data_lines[PHY] += phys_data_lines;
			phys_data_lines = 0;

		}
	}
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
* \param openBrackets number of open brackets (no matching close bracket)
* \param loopLevel nested loop level
*/
void CCFScriptCounter::LSLOC(results* result, string line, size_t lineNumber, string lineBak, string &strLSLOC, string &strLSLOCBak, 
							unsigned int &paren_cnt, bool &forflag, bool &found_forifwhile, bool &found_while, 
							char &prev_char, bool &data_continue, unsigned int &temp_lines, 
							unsigned int &phys_exec_lines, unsigned int &phys_data_lines, bool &inArrayDec,
							unsigned int &openBrackets, StringVector &loopLevel)
{
	// paren_cnt is used with 'for' statement only
	size_t start = 0;	//starting index of the working string
	size_t i = 0, strSize;
	bool found_do, found_try, found_else, trunc_flag = false;
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$:";
	unsigned int cnt = 0;

	string tmp = CUtil::TrimString(strLSLOC);

	// do, try
	found_do = (CUtil::FindKeyword(tmp, "do", 0, TO_END_OF_STRING, false) != string::npos);
	found_try = (CUtil::FindKeyword(tmp, "try", 0, TO_END_OF_STRING, false) != string::npos);
	// else is treated differently, else is included in SLOC, do and try are not
	found_else = (CUtil::FindKeyword(tmp, "else", 0, TO_END_OF_STRING, false) != string::npos);

	while (i < line.length()) // there may be more than 1 logical SLOC in this line
	{
		switch (line[i])
		{
		case ';': case '{': // LSLOC terminators
			                // ';' for normal executable or declaration statement
			                // '{' for starting a function or 'do' statement or a block (which is counted)
			
			// get the previous logical mark until i-1 index is the new LSLOC
			// except 'do' precedes '{'
			// except '}' precedes ';' ??
			if (paren_cnt > 0 && line[i] == ';') // do nothing inside 'for' statement
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
				if (prev_char == '=') inArrayDec = true;
				if (inArrayDec) break; // continue until seeing ';'

				// case for(...); and if (...) {
				if (found_forifwhile) // these specials are handled
				{
					found_forifwhile = false;
					start = i + 1;
					break;
				}

				// check if 'do' precedes '{'
				if (!found_do && !found_try && !found_else)
				{
					// find for 'do' in string before tmp string
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
						if (loopLevel.size() > 0) loopLevel.pop_back();
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

			if (line[i] == ';' && prev_char == '}')  // wrong, e.g., a[]={1,2,3};
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
			if (CUtil::TrimString(line.substr(start, i + 1 - start)) == ";" && strLSLOC.length() < 1)
			{
				strLSLOC = ";";
				strLSLOCBak = ";";
			}
			else
			{
				strSize = CUtil::TruncateLine(i + 1 - start, strLSLOC.length(), this->lsloc_truncate, trunc_flag);
				if (strSize > 0)
				{
					strLSLOC += line.substr(start, strSize);
					strLSLOCBak += lineBak.substr(start, strSize);
				}
			}
			if (result->addSLOC(strLSLOCBak, lineNumber, trunc_flag))
			{
				cnt = 0;
				CUtil::CountTally(strLSLOC, data_name_list, cnt, 1, exclude, "", "", &result->data_name_count, false);

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

			break;
		case '(':
			if (forflag)
				paren_cnt++;
			else
			{
				// handle 'for', 'while', 'if' the same way
				tmp = CUtil::TrimString(line.substr(start,i));
				if (CUtil::FindKeyword(tmp, "for", 0, TO_END_OF_STRING, false) != string::npos
					|| CUtil::FindKeyword(tmp, "while", 0, TO_END_OF_STRING, false)!= string::npos
					|| CUtil::FindKeyword(tmp, "if", 0, TO_END_OF_STRING, false) != string::npos)
				{
					forflag = true;
					paren_cnt++;

					if (print_cmplx && (unsigned int)loopLevel.size() > openBrackets && openBrackets > 0)
						loopLevel.pop_back();

					if (CUtil::FindKeyword(tmp, "while", 0, TO_END_OF_STRING, false)!= string::npos)
					{
						if (print_cmplx)
							loopLevel.push_back("while");
						found_while = true;
					}
					else if (print_cmplx && CUtil::FindKeyword(tmp, "for", 0, TO_END_OF_STRING, false) != string::npos)
					{
						loopLevel.push_back("for");
					}

					// record nested loop level
					if (print_cmplx)
					{
						if (CUtil::FindKeyword(tmp, "if", 0, TO_END_OF_STRING, false) == string::npos)
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
					strSize = CUtil::TruncateLine(i + 1 - start, strLSLOC.length(), this->lsloc_truncate, trunc_flag);
					if (strSize > 0)
					{
						strLSLOC += line.substr(start, strSize);
						strLSLOCBak += lineBak.substr(start, strSize);
					}
					if (result->addSLOC(strLSLOCBak, lineNumber, trunc_flag))
						result->exec_lines[LOG]++;
					strLSLOCBak = strLSLOC = "";
					phys_exec_lines = temp_lines;
					temp_lines = 0;
					start = i + 1;
					found_forifwhile = true;	
					forflag = false;
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
				// record close bracket for nested loop processing
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
			//if ( !(line[i] == '}' && (prev_char == ';' || prev_char == '{'))) // see case '}' above
			prev_char = line[i];

			// change to not found if a char appears before
			if (line[i] != ')' && found_forifwhile)
				found_forifwhile = false;			
		}
		i++;
	
	}

	tmp = CUtil::TrimString(line.substr(start, i - start));
	strSize = CUtil::TruncateLine(tmp.length(), strLSLOC.length(), this->lsloc_truncate, trunc_flag);
	if (strSize > 0)
	{
		strLSLOC += tmp.substr(0, strSize);
		tmp = CUtil::TrimString(lineBak.substr(start, i - start));
		strLSLOCBak += tmp.substr(0, strSize);
	}
	
	// make sure that we are not beginning to process a new data line
	cnt = 0;
	CUtil::CountTally(strLSLOC, data_name_list, cnt, 1, exclude, "", "", NULL, false);

	if (cnt > 0)
		data_continue = true;
	if (data_continue)
		temp_lines++;
	if (temp_lines == 0 && phys_data_lines == 0 && phys_exec_lines == 0)
		phys_exec_lines = 1;
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
int CCFScriptCounter::ParseFunctionName(const string &line, string &lastline,
		filemap &functionStack, string &functionName, unsigned int &functionCount)
{
	string tline, str;
	size_t idx, tidx, cnt, cnt2;
	unsigned int fcnt;
	unsigned int cyclomatic_cnt = 0;
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$";

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
			CUtil::CountTally(str, cmplx_cyclomatic_list, cyclomatic_cnt, 1, exclude, "", "", 0, casesensitive);
			if (cyclomatic_cnt <= 0 && CUtil::FindKeyword(str, "switch") == string::npos &&
				CUtil::FindKeyword(str, "try") == string::npos && CUtil::FindKeyword(str, "finally") == string::npos &&
				CUtil::FindKeyword(str, "return") == string::npos && str.find('=') == string::npos)
			{
				functionName = CUtil::ClearRedundantSpaces(str.substr(0, idx));
				functionCount = fcnt;
				lastline.erase();
				return 1;
			}
		}
		lastline.erase();
	}
	return 0;
}

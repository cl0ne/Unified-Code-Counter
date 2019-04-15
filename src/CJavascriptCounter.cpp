//! Code counter class methods for the JavaScript language.
/*!
* \file CJavascriptCounter.cpp
*
* This file contains the code counter class methods for the JavaScript language.
*/

#include "CJavascriptCounter.h"

/*!
* Constructs a CJavascriptCounter object.
*/
CJavascriptCounter::CJavascriptCounter()
{
	classtype = JAVASCRIPT;
	language_name = "JavaScript";

    //Modification: 11.2016 Ext-4
    file_extension = CUtil::getExtensionsToLanguage("JavaScript", file_extension);
    
	//file_extension.push_back(".js");

	QuoteStart = "\"'/";
	QuoteEnd = "\"'/";
	QuoteEscapeFront = '\\';
	LineCommentStart.push_back("//");
	BlockCommentStart.push_back("/*");
	BlockCommentEnd.push_back("*/");

	data_name_list.push_back("abstract");
	data_name_list.push_back("boolean");
	data_name_list.push_back("byte");
	data_name_list.push_back("char");
	data_name_list.push_back("class");
	data_name_list.push_back("double");
	data_name_list.push_back("enum");
	data_name_list.push_back("float");
	data_name_list.push_back("implements");
	data_name_list.push_back("instanceOf");
	data_name_list.push_back("int");
	data_name_list.push_back("interface");
	data_name_list.push_back("long");
	data_name_list.push_back("private");
	data_name_list.push_back("protected");
	data_name_list.push_back("public");
	data_name_list.push_back("short");
	data_name_list.push_back("static");
	data_name_list.push_back("void");

	exec_name_list.push_back("alert");
	exec_name_list.push_back("arguments");
	exec_name_list.push_back("assign");
	exec_name_list.push_back("break");
	exec_name_list.push_back("case");
	exec_name_list.push_back("catch");
	exec_name_list.push_back("close");
	exec_name_list.push_back("comment");
	exec_name_list.push_back("constructor");
	exec_name_list.push_back("continue");
	exec_name_list.push_back("default");
	exec_name_list.push_back("debugger");
	exec_name_list.push_back("delete");
	exec_name_list.push_back("do");
	exec_name_list.push_back("else");
	exec_name_list.push_back("escape");
	exec_name_list.push_back("eval");
	exec_name_list.push_back("export");
	exec_name_list.push_back("extends");
	exec_name_list.push_back("false");
	exec_name_list.push_back("find");
	exec_name_list.push_back("final");
	exec_name_list.push_back("finally");
	exec_name_list.push_back("focus");
	exec_name_list.push_back("for");
	exec_name_list.push_back("function");
	exec_name_list.push_back("if");
	exec_name_list.push_back("import");
	exec_name_list.push_back("label");
	exec_name_list.push_back("length");
	exec_name_list.push_back("location");
	exec_name_list.push_back("native");
	exec_name_list.push_back("new");
	exec_name_list.push_back("null");
	exec_name_list.push_back("open");
	exec_name_list.push_back("package");
	exec_name_list.push_back("print");
	exec_name_list.push_back("prompt");
	exec_name_list.push_back("prototype");
	exec_name_list.push_back("ref");
	exec_name_list.push_back("return");
	exec_name_list.push_back("self");
	exec_name_list.push_back("status");
	exec_name_list.push_back("stop");
	exec_name_list.push_back("super");
	exec_name_list.push_back("switch");
	exec_name_list.push_back("synchronized");
	exec_name_list.push_back("taint");
	exec_name_list.push_back("this");
	exec_name_list.push_back("throw");
	exec_name_list.push_back("throws");
	exec_name_list.push_back("transient");
	exec_name_list.push_back("true");
	exec_name_list.push_back("try");
	exec_name_list.push_back("typeof");
	exec_name_list.push_back("untaint");
	exec_name_list.push_back("var");
	exec_name_list.push_back("watch");
	exec_name_list.push_back("while");
	exec_name_list.push_back("with");

	math_func_list.push_back("abs");
	math_func_list.push_back("ceil");
	math_func_list.push_back("exp");
	math_func_list.push_back("floor");
	math_func_list.push_back("max");
	math_func_list.push_back("min");
	math_func_list.push_back("pow");
	math_func_list.push_back("random");
	math_func_list.push_back("round");
	math_func_list.push_back("sqrt");

	trig_func_list.push_back("acos");
	trig_func_list.push_back("asin");
	trig_func_list.push_back("atan");
	trig_func_list.push_back("atan2");
	trig_func_list.push_back("cos");
	trig_func_list.push_back("sin");
	trig_func_list.push_back("tan");

	log_func_list.push_back("log");

	cmplx_calc_list.push_back("+");
	cmplx_calc_list.push_back("-");
	cmplx_calc_list.push_back("*");
	cmplx_calc_list.push_back("/");
	cmplx_calc_list.push_back("%");
	cmplx_calc_list.push_back("++");
	cmplx_calc_list.push_back("--");

	cmplx_cond_list.push_back("case");
	cmplx_cond_list.push_back("else");
	cmplx_cond_list.push_back("else if");
	cmplx_cond_list.push_back("for");
	cmplx_cond_list.push_back("if");
	cmplx_cond_list.push_back("switch");
	cmplx_cond_list.push_back("while");
	cmplx_cond_list.push_back("?");

	cmplx_logic_list.push_back("==");
	cmplx_logic_list.push_back("===");
	cmplx_logic_list.push_back("!=");
	cmplx_logic_list.push_back("!==");
	cmplx_logic_list.push_back(">");
	cmplx_logic_list.push_back("<");
	cmplx_logic_list.push_back(">=");
	cmplx_logic_list.push_back("=<");

	cmplx_assign_list.push_back("=");
	
	cmplx_cyclomatic_list.push_back("if");
	cmplx_cyclomatic_list.push_back("case");
	cmplx_cyclomatic_list.push_back("while");
	cmplx_cyclomatic_list.push_back("for");
	cmplx_cyclomatic_list.push_back("catch");
	cmplx_cyclomatic_list.push_back("?");

	cmplx_cyclomatic_logic_list.push_back("&&");
	cmplx_cyclomatic_logic_list.push_back("||");

	cmplx_cyclomatic_case_list.push_back("case");
	cmplx_cyclomatic_switch_list.push_back("switch");
	
}

/*!
* Handles special case for quote characters within regexp operators.
*
* \param strline string to be processed
* \param idx_start index of line character to start search
* \param contd specifies the quote string is continued from the previous line
* \param CurrentQuoteEnd end quote character of the current status
*
* \return method status
*/
int CJavascriptCounter::ReplaceQuote(string &strline, size_t &idx_start, bool &contd, char &CurrentQuoteEnd) 
{
	static bool inRegexp = false;
	size_t idx = idx_start;
	size_t i = idx;

	if (inRegexp || strline[idx] == '/')
	{
		if (!inRegexp)
		{
			// check for open parenthesis to indicate a regexp (match(//), replace(//, ""), split(//))
			while (i > 0)
			{
				if (strline[i-1] == '(')
					break;
				else if (strline[i-1] != ' ' && strline[i-1] != '\t')
				{
					idx_start++;
					return 1;
				}
				i--;
			}
			if (i <= 0)
			{
				idx_start++;
				return 1;
			}
			i = idx;
		}

		// replace all "\\" by "$$"
		size_t start = idx_start;
		while ((start = strline.find("\\\\", start)) != string::npos)
		{
			strline.replace(start, 2, "$$");
			start += 2;
		}

		while (i < strline.length())
		{
			if (inRegexp)
			{
				if ((strline[i] == '/' && (i == 0 || (i > 0 && strline[i - 1] != '\\')))
					|| (contd && strline[i] == ';'))
				{
					// replace everything in the regexp
					strline.replace(idx, i - idx + 1, i - idx + 1, '$');
					inRegexp = false;
					contd = false;
					idx = i + 1;
					idx_start = idx;
					return 1;
				}
			}
			else if (strline[i] == '/')
			{
				idx = i;
				inRegexp = true;
			}
			i++;
		}

		if (inRegexp)
		{
			strline.replace(idx, i - idx, i - idx, '$');
			contd = true;
		}
	}
	idx_start = idx;

	if (!inRegexp)
		return CCodeCounter::ReplaceQuote(strline, idx_start, contd, CurrentQuoteEnd);

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
int CJavascriptCounter::CountDirectiveSLOC(filemap* fmap, results* result, filemap* fmapBak)
{
	bool contd = false, trunc_flag = false, foundSeperatingTag = false;
	size_t idx, strSize;
	size_t lineNumber = 0;
	unsigned int cnt = 0;
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$";
	string strDirLine = "";
	filemap::iterator itfmBak = fmapBak->begin();

	for (filemap::iterator iter = fmap->begin(); iter!=fmap->end(); iter++, itfmBak++)
	{
		if (CUtil::CheckBlank(iter->line))
			continue;

		if(iter->lineNumber == 0)
		{
			if(!foundSeperatingTag)
			{
				lineNumber++;
				foundSeperatingTag = true;
			}
			else
			{
				if((iter+1)!=fmap->end())
				{
					lineNumber = (iter+1)->lineNumber - 1;
				}
				foundSeperatingTag = false;
			}
		}
		else
		{
			lineNumber = iter->lineNumber;
		}

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
				// ensures the keyword stands alone, avoid, e.g., #ifabc
				if (((idx = CUtil::FindKeyword(iter->line, *viter)) != string::npos) && idx == 0)
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
			if (iter->line[iter->line.length()-1] != ',' && iter->line[iter->line.length()-1] != '\\')
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
int CJavascriptCounter::LanguageSpecificProcess(filemap* fmap, results* result, filemap* fmapBak)
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
	bool foundSeperatingTag = false;

	unsigned int phys_exec_lines = 0;
	unsigned int phys_data_lines = 0;
	unsigned int temp_lines = 0;
	unsigned int cnt = 0;
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$";
	size_t lineNumber = 0;

	for (fit = fmap->begin(), fitbak = fmapBak->begin(); fit != fmap->end(); fit++, fitbak++)
	{
		line = fit->line;

		// insert blank at the beginning (for searching keywords
		line = ' ' + line;
		lineBak = ' ' + fitbak->line;

		if(fit->lineNumber == 0)
		{
			if(!foundSeperatingTag)
			{
				lineNumber++;
				foundSeperatingTag = true;
			}
			else
			{
				if((fit+1)!=fmap->end())
				{
					lineNumber = (fit+1)->lineNumber - 1;
				}
				foundSeperatingTag = false;
			}
		}
		else
		{
			lineNumber = fit->lineNumber;
		}

		// do not process blank lines
		// blank line means blank_line/comment_line/directive
		if (!CUtil::CheckBlank(line))
		{
			LSLOC(result, line, lineNumber, lineBak, strLSLOC, strLSLOCBak, paren_count, for_flag, found_forifwhile, found_while,
				prev_char, data_continue, temp_lines, phys_exec_lines, phys_data_lines, inArrayDec,
				openBrackets, loopLevel);

			if (print_cmplx)
			{
				cnt = 0;
				CUtil::CountTally(line, exec_name_list, cnt, 1, exclude, "", "", &result->exec_name_count);
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
void CJavascriptCounter::LSLOC(results* result, string line, size_t lineNumber, string lineBak, string &strLSLOC, string &strLSLOCBak, unsigned int &paren_cnt,
							   bool &forflag, bool &found_forifwhile, bool &found_while, char &prev_char, bool &data_continue,
							   unsigned  int &temp_lines, unsigned int &phys_exec_lines, unsigned int &phys_data_lines, bool &inArrayDec,
							   unsigned int &openBrackets, StringVector &loopLevel)
{
	// paren_cnt is used with 'for' statement only
	size_t start = 0; //starting index of the working string
	size_t i = 0, strSize;
	bool found_do, found_try, found_else, trunc_flag = false;
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$:";
	unsigned int cnt = 0;
	unsigned int loopCnt = 0;
	StringVector::iterator lit;

	string tmp = CUtil::TrimString(strLSLOC);

	// do, try
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
			if (paren_cnt > 0 && line[i] == ';')
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
				CUtil::CountTally(strLSLOC, data_name_list, cnt, 1, exclude, "", "", &result->data_name_count);

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
				// handle 'for', 'foreach', 'while', 'if' the same way
				tmp = CUtil::TrimString(line.substr(start,i));
				if (CUtil::FindKeyword(tmp, "for") != string::npos
					|| CUtil::FindKeyword(tmp, "foreach") != string::npos
					|| CUtil::FindKeyword(tmp, "while")!= string::npos
					|| CUtil::FindKeyword(tmp, "if") != string::npos)
				{
					forflag = true;
					paren_cnt++;

					if (print_cmplx && (unsigned int)loopLevel.size() > openBrackets && openBrackets > 0)
						loopLevel.pop_back();

					if (CUtil::FindKeyword(tmp, "while")!= string::npos)
					{
						if (print_cmplx)
							loopLevel.push_back("while");
						found_while = true;
					}
					else if (print_cmplx)
					{
						if (CUtil::FindKeyword(tmp, "for") != string::npos)
							loopLevel.push_back("for");
						else if (CUtil::FindKeyword(tmp, "foreach") != string::npos)
							loopLevel.push_back("foreach");

						// record nested loop level
						if (CUtil::FindKeyword(tmp, "if") == string::npos)
						{
							loopCnt = 0;
							for (lit = loopLevel.begin(); lit < loopLevel.end(); lit++)
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

	tmp = CUtil::TrimString(line.substr(start, i - start));
	strSize = CUtil::TruncateLine(tmp.length(), strLSLOC.length(), this->lsloc_truncate, trunc_flag);
	if (strSize > 0)
	{
		strLSLOC += tmp.substr(0, strSize);
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
	CUtil::CountTally(strLSLOC, data_name_list, cnt, 1, exclude, "", "", NULL);

	if (cnt > 0)
		data_continue = true;
	if (data_continue)
		temp_lines++;
	if (temp_lines == 0 && phys_data_lines == 0 && phys_exec_lines == 0)
		phys_exec_lines = 1;
}

/*!
* Constructs a CJavascriptPhpCounter object.
*/
CJavascriptPhpCounter::CJavascriptPhpCounter()
{
	classtype = JAVASCRIPT_PHP;
	language_name = "JavaScript/PHP";

	file_extension.clear();
	file_extension.push_back(".*jsphp");
}

/*!
* Constructs a CJavascriptHtmlCounter object.
*/
CJavascriptHtmlCounter::CJavascriptHtmlCounter()
{
	classtype = JAVASCRIPT_HTML;
	language_name = "JavaScript/HTML";

	file_extension.clear();
	file_extension.push_back(".*jshtm");
}

/*!
* Constructs a CJavascriptXmlCounter object.
*/
CJavascriptXmlCounter::CJavascriptXmlCounter()
{
	classtype = JAVASCRIPT_XML;
	language_name = "JavaScript/XML";

	file_extension.clear();
	file_extension.push_back(".*jsxml");
}

/*!
* Constructs a CJavascriptJspCounter object.
*/
CJavascriptJspCounter::CJavascriptJspCounter()
{
	classtype = JAVASCRIPT_JSP;
	language_name = "JavaScript/JSP";

	file_extension.clear();
	file_extension.push_back(".*jsjsp");
}

/*!
* Constructs a CJavascriptAspServerCounter object.
*/
CJavascriptAspServerCounter::CJavascriptAspServerCounter()
{
	classtype = JAVASCRIPT_ASP_S;
	language_name = "JavaScript/ASP Server";

	file_extension.clear();
	file_extension.push_back(".*jsasps");
}

/*!
* Constructs a CJavascriptAspClientCounter object.
*/
CJavascriptAspClientCounter::CJavascriptAspClientCounter()
{
	classtype = JAVASCRIPT_ASP_C;
	language_name = "JavaScript/ASP Client";

	file_extension.clear();
	file_extension.push_back(".*jsaspc");
}

/*!
* Constructs a CJavascriptColdFusionCounter object.
*/
CJavascriptColdFusionCounter::CJavascriptColdFusionCounter()
{
	classtype = JAVASCRIPT_CFM;
	language_name = "JavaScript/ColdFusion";

	file_extension.clear();
	file_extension.push_back(".*jscfm");
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
int CJavascriptCounter::ParseFunctionName(const string &line, string &lastline,
		filemap &functionStack, string &functionName, unsigned int &functionCount)
{
        string tline, str, front, back;
	size_t idx, tidx, cnt, cnt2;
	unsigned int fcnt;

	tline = CUtil::TrimString(line);
	idx = tline.find('{');
	if (idx != string::npos)
	{
		// check whether it is at first index, if yes then function name is at above line
		if (idx == 0)
		{
			idx = lastline.find("function");
			if (idx != string::npos)
			{
				front = lastline.substr(0, idx);
				back = lastline.substr(idx + 8);

				idx = front.find('=');
				if (idx != string::npos)
				{
					front = front.substr(0, idx);
					lineElement element(++functionCount, CUtil::TrimString(front));
					functionStack.push_back(element);
					lastline.erase();
				}
				else {
					idx = back.find('(');
					if (idx != string::npos)
					{
						back = back.substr(0, idx);
						lineElement element(++functionCount, CUtil::TrimString(back));
						functionStack.push_back(element);
						lastline.erase();
					}
				}
			}
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
			idx = lastline.find("function");
			if (idx != string::npos)
			{
				front = lastline.substr(0, idx);
				back = lastline.substr(idx + 8);
				idx = front.find('=');
				if (idx != string::npos)
				{
					front = front.substr(0, idx);
					lineElement element(++functionCount, CUtil::TrimString(front));
					functionStack.push_back(element);
					lastline.erase();
				}
				else {
					idx = back.find('(');
					if (idx != string::npos)
					{
						back = back.substr(0, idx);
						lineElement element(++functionCount, CUtil::TrimString(back));
						functionStack.push_back(element);
						lastline.erase();
					}
				}
			}
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

	if (functionStack.empty())
	{
		// dealing with some code out of any subroutines, it a "main" code
		return 2;
	}

	idx = CUtil::FindKeyword(line, "{");
	if (idx != string::npos)
	{
		lineElement element(functionCount, "{");
		functionStack.push_back(element);	
	}
	idx = CUtil::FindKeyword(line, "}");
	if (idx != string::npos)
        {
		functionStack.pop_back();
		if (functionStack.size() == 0)
			return 0;
		str = functionStack.back().line;
		fcnt = functionStack.back().lineNumber;
		if (str != "{")
		{
			functionStack.pop_back();
			functionName = CUtil::ClearRedundantSpaces(str);
			functionCount = fcnt;
			return 1;
		}
	}	
	return 0;


}

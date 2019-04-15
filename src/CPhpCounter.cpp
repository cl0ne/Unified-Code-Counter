//! Code counter class methods for the PHP language.
/*!
* \file CPhpCounter.cpp
*
* This file contains the code counter class methods for the PHP language.
*/

#include "CPhpCounter.h"

/*!
* Constructs a CPhpCounter object.
*/
CPhpCounter::CPhpCounter()
{
	classtype = PHP;
	language_name = "PHP";
    
    //Modification: 11.2016 Ext-4
    file_extension = CUtil::getExtensionsToLanguage("PHP", file_extension);
	//file_extension.push_back(".*php");

	QuoteStart = "\"'";
	QuoteEnd = "\"'";
	QuoteEscapeFront = '\\';
	ContinueLine = "\\";
	BlockCommentStart.push_back("/*");
	BlockCommentEnd.push_back("*/");

	LineCommentStart.push_back("//");
	LineCommentStart.push_back("#");

	exclude_keywords.push_back("endif");
	exclude_keywords.push_back("endfor");
	exclude_keywords.push_back("endforeach");
	exclude_keywords.push_back("endswitch");
	exclude_keywords.push_back("endwhile");

	exclude_loop.push_back("endfor");
	exclude_loop.push_back("endforeach");
	exclude_loop.push_back("endwhile");

	directive.push_back("define");
	directive.push_back("include");
	directive.push_back("include_once");
	directive.push_back("require");
	directive.push_back("require_once");

	data_name_list.push_back("array");
	data_name_list.push_back("bool");
	data_name_list.push_back("class");
	data_name_list.push_back("const");
	data_name_list.push_back("declare");
	data_name_list.push_back("extends");
	data_name_list.push_back("float");
	data_name_list.push_back("function");
	data_name_list.push_back("global");
	data_name_list.push_back("int");
	data_name_list.push_back("interface");
	data_name_list.push_back("NULL");
	data_name_list.push_back("object");
	data_name_list.push_back("private");
	data_name_list.push_back("protected");
	data_name_list.push_back("public");
	data_name_list.push_back("string");
	data_name_list.push_back("var");

	exec_name_list.push_back("break");
	exec_name_list.push_back("case");
	exec_name_list.push_back("catch");
	exec_name_list.push_back("continue");
	exec_name_list.push_back("default");
	exec_name_list.push_back("die");
	exec_name_list.push_back("do");
	exec_name_list.push_back("echo");
	exec_name_list.push_back("else");
	exec_name_list.push_back("exception");
	exec_name_list.push_back("exit");
	exec_name_list.push_back("for");
	exec_name_list.push_back("foreach");
	exec_name_list.push_back("if");
	exec_name_list.push_back("isset"); 
	exec_name_list.push_back("new");
	exec_name_list.push_back("print");
	exec_name_list.push_back("return");
	exec_name_list.push_back("switch");
	exec_name_list.push_back("this");
	exec_name_list.push_back("throw");
	exec_name_list.push_back("try");
	exec_name_list.push_back("while");

	math_func_list.push_back("abs");
	math_func_list.push_back("base_convert");
	math_func_list.push_back("bindec");
	math_func_list.push_back("ceil");
	math_func_list.push_back("decbin");
	math_func_list.push_back("dechex");
	math_func_list.push_back("decoct");
	math_func_list.push_back("deg2rad");
	math_func_list.push_back("exp");
	math_func_list.push_back("expm1");
	math_func_list.push_back("floor");
	math_func_list.push_back("fmod");
	math_func_list.push_back("getrandmax");
	math_func_list.push_back("hexdec");
	math_func_list.push_back("hypot");
	math_func_list.push_back("is_finite");
	math_func_list.push_back("is_infinite");
	math_func_list.push_back("is_nan");
	math_func_list.push_back("lcg_value");
	math_func_list.push_back("max");
	math_func_list.push_back("min");
	math_func_list.push_back("mt_getrandmax");
	math_func_list.push_back("mt_rand");
	math_func_list.push_back("mt_srand");
	math_func_list.push_back("octdec");
	math_func_list.push_back("pi");
	math_func_list.push_back("pow");
	math_func_list.push_back("rad2deg");
	math_func_list.push_back("rand");
	math_func_list.push_back("round");
	math_func_list.push_back("sqrt");
	math_func_list.push_back("srand");

	trig_func_list.push_back("acos");
	trig_func_list.push_back("acosh");
	trig_func_list.push_back("asin");
	trig_func_list.push_back("asinh");
	trig_func_list.push_back("atan");
	trig_func_list.push_back("atan2");
	trig_func_list.push_back("atanh");
	trig_func_list.push_back("cos");
	trig_func_list.push_back("cosh");
	trig_func_list.push_back("sin");
	trig_func_list.push_back("sinh");
	trig_func_list.push_back("tan");
	trig_func_list.push_back("tanh");

	log_func_list.push_back("log");
	log_func_list.push_back("log10");
	log_func_list.push_back("log1p");

	cmplx_calc_list.push_back("%");
	cmplx_calc_list.push_back("++");
	cmplx_calc_list.push_back("+");
	cmplx_calc_list.push_back("--");
	cmplx_calc_list.push_back("-");
	cmplx_calc_list.push_back("*");
	cmplx_calc_list.push_back("/");
	cmplx_calc_list.push_back(">>");
	cmplx_calc_list.push_back("<<");

	cmplx_cond_list.push_back("else");
	cmplx_cond_list.push_back("else if");
	cmplx_cond_list.push_back("elseif");
	cmplx_cond_list.push_back("for");
	cmplx_cond_list.push_back("if");
	cmplx_cond_list.push_back("case");
	cmplx_cond_list.push_back("switch");
	cmplx_cond_list.push_back("while");

	cmplx_logic_list.push_back("&");
	cmplx_logic_list.push_back("|");
	cmplx_logic_list.push_back("^");
	cmplx_logic_list.push_back("~");
	cmplx_logic_list.push_back("==");
	cmplx_logic_list.push_back("===");
	cmplx_logic_list.push_back("!=");
	cmplx_logic_list.push_back("!==");
	cmplx_logic_list.push_back("<>");
	cmplx_logic_list.push_back("&&");
	cmplx_logic_list.push_back("||");
	cmplx_logic_list.push_back("!");
	cmplx_logic_list.push_back("and"); 
	cmplx_logic_list.push_back("not"); 
	cmplx_logic_list.push_back("or"); 
	cmplx_logic_list.push_back("xor"); 
	cmplx_logic_list.push_back(">");
	cmplx_logic_list.push_back("<");
	cmplx_logic_list.push_back(">=");
	cmplx_logic_list.push_back("=<");

	cmplx_assign_list.push_back("=");
	cmplx_assign_list.push_back("=>");

	cmplx_cyclomatic_list.push_back("if");
	cmplx_cyclomatic_list.push_back("elsif");
	cmplx_cyclomatic_list.push_back("while");
	cmplx_cyclomatic_list.push_back("for");
	cmplx_cyclomatic_list.push_back("foreach");
	cmplx_cyclomatic_list.push_back("catch");
	cmplx_cyclomatic_list.push_back("case");

	cmplx_cyclomatic_logic_list.push_back("&&");
	cmplx_cyclomatic_logic_list.push_back("||");
	//cmplx_cyclomatic_logic_list.push_back("^");
	cmplx_cyclomatic_logic_list.push_back("and");
	cmplx_cyclomatic_logic_list.push_back("or");
	//cmplx_cyclomatic_logic_list.push_back("xor");

	cmplx_cyclomatic_case_list.push_back("case");
	cmplx_cyclomatic_switch_list.push_back("switch");
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
int CPhpCounter::CountDirectiveSLOC(filemap* fmap, results* result, filemap* fmapBak)
{
	bool contd = false, trunc_flag = false;
	size_t idx, strSize;
	unsigned int cnt = 0;
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$";
	string strDirLine = "";
	filemap::iterator itfmBak = fmapBak->begin();

	for (filemap::iterator iter = fmap->begin(); iter!=fmap->end(); iter++, itfmBak++)
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
int CPhpCounter::LanguageSpecificProcess(filemap* fmap, results* result, filemap* fmapBak)
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

	filemap::iterator fit, fitbak;
	string line, lineBak;
	StringVector loopLevel;

	unsigned int phys_exec_lines = 0;
	unsigned int phys_data_lines = 0;
	unsigned int temp_lines = 0;
	unsigned int cnt = 0;
	string  exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$";

	for (fit = fmap->begin(), fitbak = fmapBak->begin(); fit != fmap->end(); fit++, fitbak++)
	{
		// insert blank at the beginning (for searching keywords)
		line = ' ' + fit->line;
		lineBak = ' ' + fitbak->line;

		// do not process blank lines
		// blank line means blank_line/comment_line/directive
		if (!CUtil::CheckBlank(line))
		{
			LSLOC(result, line, fit->lineNumber, lineBak, strLSLOC, strLSLOCBak, paren_count, for_flag, found_forifwhile, found_while,
				prev_char, data_continue, temp_lines, phys_exec_lines, phys_data_lines, inArrayDec, found_for, loopLevel);

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
* \param found_for found for loop
* \param loopLevel nested loop level
*/
void CPhpCounter::LSLOC(results* result, string line, size_t lineNumber, string lineBak, string &strLSLOC, string &strLSLOCBak, unsigned int &paren_cnt,
						bool &forflag, bool &found_forifwhile, bool &found_while, char &prev_char, bool &data_continue,
						unsigned int &temp_lines, unsigned int &phys_exec_lines, unsigned int &phys_data_lines,
						bool &inArrayDec, bool &found_for, StringVector &loopLevel)
{
	size_t start = 0;
	size_t i = 0, strSize;
	bool found_do, found_try, found_else, found_declare, trunc_flag = false;
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$:";
	unsigned int cnt = 0;

	string tmp = CUtil::TrimString(strLSLOC);

	// do, try
	found_do = (CUtil::FindKeyword(tmp, "do") != string::npos);
	found_try = (CUtil::FindKeyword(tmp, "try") != string::npos);
	// else, declare are treated differently, else and declare are included in SLOC, do and try are not
	found_else = (CUtil::FindKeyword(tmp, "else") != string::npos);
	found_declare = (CUtil::FindKeyword(tmp, "declare") != string::npos);

	while (i < line.length()) // there may be more than 1 logical SLOC in this line
	{
		switch (line[i])
		{
		case ';': case '{': case ':':	// LSLOC terminators
										// ';' for normal executable or declaration statement
										// '{' or ':' for starting a function or 'do' stmt or a block (which is counted)
			// get the previous logical mark until i-1 index is the new LSLOC
			// except 'do' precedes '{'
			// except '}' precedes ';' ??
			// do nothing inside 'for' statement
			if (found_for == true && paren_cnt > 0 && line[i] == ';')
				break;

			// record open bracket for nested loop processing
			// check for excluded loop keywords for alternate control syntax
			if (print_cmplx)
			{
				cnt = 0;
				CUtil::CountTally(line, exclude_loop, cnt, 1, exclude, "", "");
				if (cnt > 0)
				{
					if ((unsigned int)loopLevel.size() > 0)
						loopLevel.pop_back();
				}
				else if (line[i] == '{')
					loopLevel.push_back("");
				else if (line[i] == ';')
				{
					if ((unsigned int)loopLevel.size() > 0 && loopLevel.back() != "")
					{
						tmp = loopLevel.back();
						if (tmp[tmp.length()-1] != ':')
							loopLevel.pop_back();
					}
				}
				else if (line[i] == ':')
				{
					if ((unsigned int)loopLevel.size() > 0 && loopLevel.back() != "")
					{
						tmp = CUtil::TrimString(line.substr(0, i));
						if (CUtil::FindKeyword(tmp, loopLevel.back()) != string::npos)
						{
							tmp = loopLevel.back() + ":";
							loopLevel.pop_back();
							loopLevel.push_back(tmp);
						}
					}
				}
			}

			// check for excluded keywords for alternate control syntax (don't count as LSLOC)
			cnt = 0;
			CUtil::CountTally(line, exclude_keywords, cnt, 1, exclude, "", "");
			if (cnt > 0)
			{
				start = i + 1;
				break;
			}

			// case 'if(...):', 'while(...):, for(...):, foreach(...):, switch(...):'
			// this case is handled in case ')'
			// skip other ':'
			if (line[i] == ':')
			{
				if (found_forifwhile)
				{
					found_forifwhile = false;
					start = i + 1;
				}
				break;
			}

			// case 'while(...);', 'while(...) {', '} while(...);'
			// this case is handled in case ')'
			if (found_while && found_forifwhile)
			{
				found_while = false;
				found_forifwhile = false;
				start = i + 1;
				break;
			}

			if (line[i] == '{' || line[i] == ':')
			{
				if (prev_char == '=')
					inArrayDec = true;

				// continue until seeing ';'
				if (inArrayDec)
					break;

				// case for(...);, if (...) {, and if (...):, elseif (...) {, and elseif (...):
				// these specials are handled
				if (found_forifwhile)
				{
					found_forifwhile = false;
					start = i + 1;
					break;
				}

				// check if 'do' precedes '{' or ':'
				if (!found_do && !found_try && !found_else && !found_declare)
				{
					// find 'do' in string before tmp string
					tmp = CUtil::TrimString(line.substr(start, i - start));
					found_do = (tmp == "do");			// found 'do' statement
					found_try = (tmp == "try");			// found 'try' statement
					found_else = (tmp == "else");		// found 'else' statement
					found_declare = (CUtil::FindKeyword(tmp, "declare") != string::npos);	// found 'declare' statement
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
					break; // do not store '{' or ':' following 'do'
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
						if (found_declare)
						{
							result->directive_lines[PHY] += temp_lines;
							result->directive_lines[LOG]++;
						}
						else
						{
							result->exec_lines[LOG]++;
							phys_exec_lines = temp_lines;
						}
					}
				}
			}
			else if (data_continue == true && line[i] == ';')
				phys_data_lines = temp_lines;
			else if (found_declare)
				result->directive_lines[PHY] += temp_lines;
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
			found_declare = false;

			break;
		case '(':
			if (forflag)
				paren_cnt++;
			else
			{
				// handle 'for', 'foreach', 'while', 'if' the same way
				tmp = CUtil::TrimString(line.substr(start, i));
				if (CUtil::FindKeyword(tmp, "for") != string::npos
					|| CUtil::FindKeyword(tmp, "foreach") != string::npos
					|| CUtil::FindKeyword(tmp, "while")!= string::npos
					|| CUtil::FindKeyword(tmp, "if") != string::npos
					|| CUtil::FindKeyword(tmp, "elseif") != string::npos
					|| CUtil::FindKeyword(tmp, "switch") != string::npos)
				{
					forflag = true;
					paren_cnt++;

					if (CUtil::FindKeyword(tmp, "for") != string::npos)
					{
						if (print_cmplx)
							loopLevel.push_back("for");
						found_for = true;
					}
					else if (CUtil::FindKeyword(tmp, "while")!= string::npos)
					{
						if (print_cmplx)
							loopLevel.push_back("while");
						found_while = true;
					}
					else if (CUtil::FindKeyword(tmp, "foreach") != string::npos)
						loopLevel.push_back("foreach");

					else if (print_cmplx && CUtil::FindKeyword(tmp, "foreach") != string::npos)
						loopLevel.push_back("foreach");
					
					// record nested loop level
					if (print_cmplx)
					{	
						if (CUtil::FindKeyword(tmp, "if") == string::npos &&
							CUtil::FindKeyword(tmp, "elseif") == string::npos &&
							CUtil::FindKeyword(tmp, "switch") == string::npos)
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
					// handle 'for', 'foreach', 'while', 'if', 'elseif', 'switch'
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
					found_for = false;
				}
			}
			break;
		case '}':
			// skip '}' when found ';' and then '}' because '{' is counted already
			// also, {} is also skipped, counted
			if (prev_char == ';' || prev_char == '{' || prev_char == '}')
			{
				if (!inArrayDec)
					start = i + 1;
			}

			// record close bracket for nested loop processing
			if (print_cmplx)
			{
				if ((unsigned int)loopLevel.size() > 0)
					loopLevel.pop_back();
				if ((unsigned int)loopLevel.size() > 0 && loopLevel.back() != "")
				{
					tmp = loopLevel.back();
					if (tmp[tmp.length()-1] != ':')
						loopLevel.pop_back();
				}
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
 * Parses lines for function/method names.
 *
 * \param line line to be processed
 * \param lastline last line processed
 * \param functionStack stack of functions
 * \param functionName function name found
 *
 * \return 1 if function name is found
 */
int CPhpCounter::ParseFunctionName(const string &line, string &lastline,
		filemap &functionStack, string &functionName, unsigned int &functionCount)
{
	
	string tline, str;
	size_t i, idx, tidx, cnt;
	unsigned int fcnt;

	tline = CUtil::TrimString(line);

	// look for start of a block
	// -- a block could be a function, or an "if" "for" "while" statement
	idx = tline.find('{');
	if (idx != string::npos)
	{
		// check whether it is at first index, if yes then block name is at above line
		if (idx == 0)
		{
			// record block name
			lineElement element(++functionCount, lastline);
			functionStack.push_back(element);
			lastline.erase();
		}
		else
		{
			// concatenate with last line if necessary and record block name
			str = tline.substr(0, idx);
			if (str.find('(') != string::npos && str[0] != '(')
				lastline = str;
			else
				lastline += " " + str;
			lineElement element(++functionCount, CUtil::TrimString(lastline));
			functionStack.push_back(element);
			lastline.erase();
		}
	}

	// didn't find start of a block
	// Regular line -- it's not terminated and its last line is also not terminated
	else if (tline.length() > 0 && tline[tline.length() - 1] != ';' &&
		lastline.length() > 0 && lastline[lastline.length() - 1] != ';')
	{
		// append until all parenthesis are closed
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

	// Regular line
	else
		lastline = tline;

	// Find end of a block
	idx = line.find('}');
	if (idx != string::npos && !functionStack.empty())
	{
		// pop start of the block from functionStack
		str = functionStack.back().line;
		fcnt = functionStack.back().lineNumber;
		functionStack.pop_back();
		idx = CUtil::FindKeyword(str, "function");

		// The block is a function
		if (idx != string::npos && idx + 9 < str.length())
		{
			functionName = CUtil::ClearRedundantSpaces(str.substr(idx + 9));
			functionCount = fcnt;
			lastline.erase();
			return 1;
		}
		lastline.erase();
	}

	// check stack for any "function" return 2 if not found
	idx = string::npos;
	if (!functionStack.empty())
	{
		for (i = 0; i < functionStack.size(); i++)
		{
			idx = CUtil::FindKeyword(functionStack[i].line, "function");
			if (idx != string::npos)
				break;
		}
	}
	if (idx == string::npos)
	{
		// dealing with some code out of any subroutines, it a "main" code
		return 2;
	}
	return 0;
}

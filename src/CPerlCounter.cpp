//! Code counter class methods for the Perl language.
/*!
* \file CPerlCounter.cpp
*
* This file contains the code counter class methods for the Perl language.
*/

#include "CPerlCounter.h"

/*!
* Constructs a CPerlCounter object.
*/
CPerlCounter::CPerlCounter()
{
	classtype = PERL;
	language_name = "Perl";
    
    //Modification: 11.2016 Ext-4
    file_extension = CUtil::getExtensionsToLanguage("Perl", file_extension);
	//file_extension.push_back(".pl");
	//file_extension.push_back(".pm");

	LineCommentStart.push_back("#");

	QuoteStart = "\"'/";
	QuoteEnd = "\"'/";
	QuoteEscapeRear = '\"';

	directive.push_back("import");
	directive.push_back("no");
	directive.push_back("package");
	directive.push_back("require");
	directive.push_back("use");

	data_name_list.push_back("AUTOLOAD");
	data_name_list.push_back("BEGIN");
	data_name_list.push_back("CHECK");
	data_name_list.push_back("CORE");
	data_name_list.push_back("DESTROY");
	data_name_list.push_back("END");
	data_name_list.push_back("INIT");
	data_name_list.push_back("NULL");

	exec_name_list.push_back("catch");
	exec_name_list.push_back("elsif");
	exec_name_list.push_back("eval");
	exec_name_list.push_back("for");
	exec_name_list.push_back("foreach");
	exec_name_list.push_back("if");
	exec_name_list.push_back("sub");
	exec_name_list.push_back("switch");
	exec_name_list.push_back("try");
	exec_name_list.push_back("unless");
	exec_name_list.push_back("until");
	exec_name_list.push_back("while");

	math_func_list.push_back("abs");
	math_func_list.push_back("exp");
	math_func_list.push_back("int");
	math_func_list.push_back("rand");
	math_func_list.push_back("sqrt");
	math_func_list.push_back("srand");
	math_func_list.push_back("time");

	trig_func_list.push_back("acos");
	trig_func_list.push_back("acosh");
	trig_func_list.push_back("asinh");
	trig_func_list.push_back("atanh");
	trig_func_list.push_back("asin");
	trig_func_list.push_back("atan");
	trig_func_list.push_back("atan2");
	trig_func_list.push_back("cos");
	trig_func_list.push_back("cosh");
	trig_func_list.push_back("sin");
	trig_func_list.push_back("sinh");
	trig_func_list.push_back("tan");
	trig_func_list.push_back("tanh");

	log_func_list.push_back("log");

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

	cmplx_cond_list.push_back("elsif");
	cmplx_cond_list.push_back("for");
	cmplx_cond_list.push_back("foreach");
	cmplx_cond_list.push_back("if");
	cmplx_cond_list.push_back("unless");
	cmplx_cond_list.push_back("until");
	cmplx_cond_list.push_back("while");
	
	cmplx_logic_list.push_back("&&");
	cmplx_logic_list.push_back("||");
	cmplx_logic_list.push_back("==");
	cmplx_logic_list.push_back("<=>");
	cmplx_logic_list.push_back("!"); 
	cmplx_logic_list.push_back("and"); 
	cmplx_logic_list.push_back("not"); 
	cmplx_logic_list.push_back("or"); 
	cmplx_logic_list.push_back("xor"); 
	cmplx_logic_list.push_back("~"); 
	cmplx_logic_list.push_back(">");
	cmplx_logic_list.push_back("<");
	cmplx_logic_list.push_back(">=");
	cmplx_logic_list.push_back("=<");
	cmplx_logic_list.push_back("lt");
	cmplx_logic_list.push_back("gt");
	cmplx_logic_list.push_back("ge");
	cmplx_logic_list.push_back("le");
	cmplx_logic_list.push_back("eq");
	cmplx_logic_list.push_back("ne");
	cmplx_logic_list.push_back("cmp");

	cmplx_preproc_list.push_back("import");
	cmplx_preproc_list.push_back("no");
	cmplx_preproc_list.push_back("package");
	cmplx_preproc_list.push_back("require");
	cmplx_preproc_list.push_back("use");

	cmplx_assign_list.push_back("=");

	cmplx_cyclomatic_list.push_back("if");
	cmplx_cyclomatic_list.push_back("elsif");
	cmplx_cyclomatic_list.push_back("case");
	cmplx_cyclomatic_list.push_back("while");
	cmplx_cyclomatic_list.push_back("until");
	cmplx_cyclomatic_list.push_back("for");
	cmplx_cyclomatic_list.push_back("foreach");
	cmplx_cyclomatic_list.push_back("catch");
	cmplx_cyclomatic_list.push_back("unless");
	cmplx_cyclomatic_list.push_back("?");

	cmplx_cyclomatic_logic_list.push_back("||");
	cmplx_cyclomatic_logic_list.push_back("&&");
	//cmplx_cyclomatic_logic_list.push_back("^");
	cmplx_cyclomatic_logic_list.push_back("and");
	cmplx_cyclomatic_logic_list.push_back("or"); 
	//cmplx_cyclomatic_logic_list.push_back("xor"); 

	cmplx_cyclomatic_case_list.push_back("case");
	cmplx_cyclomatic_switch_list.push_back("switch");
}

/*!
* Perform preprocessing of file lines before counting.
*
* \param fmap list of file lines
*
* \return method status
*/
int CPerlCounter::PreCountProcess(filemap* fmap)
{
	size_t i, j;
	filemap::iterator fit;
	for (fit = fmap->begin(); fit != fmap->end(); fit++) 
	{
		if (fit->line.empty())
			continue;
		// check for $#, $', $", $`, {$...}
		for (i = fit->line.length() - 1; i > 0; i--)
		{
			if (fit->line[i-1] == '{' && fit->line[i] == '$')
			{
				fit->line[i-1] = '$';
				for (j = i+1; j < fit->line.length(); j++)
				{
					if (fit->line[j] == '}')
					{
						fit->line[j] = '$';
						break;
					}
				}
			}
			if (fit->line[i-1] == '$' &&
				(fit->line[i] == '#' || fit->line[i] == '\'' || fit->line[i] == '"' || fit->line[i] == '`'))
				fit->line[i] = '$';
		}
	}
	return 0;
}

/*!
* Handles special case for Perl regexp operators m// s/// tr///.
*
* \param strline string to be processed
* \param idx_start index of line character to start search
* \param contd specifies the quote string is continued from the previous line
* \param CurrentQuoteEnd end quote character of the current status
*
* \return method status
*/
int CPerlCounter::ReplaceQuote(string &strline, size_t &idx_start, bool &contd, char &CurrentQuoteEnd) 
{
	static bool inRegexp = false;
	static int slashCount = 1; // =1 for match m//, =1 for s/// and tr///
	size_t idx = idx_start;
	size_t i = idx;

	if (inRegexp || strline[idx] == '/')
	{ 
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
					slashCount--;
					if (slashCount == 0)
					{
						strline.replace(idx, i - idx + 1, i - idx + 1, '$');
						inRegexp = false;
						contd = false;
						idx = i + 1;
						idx_start = idx;
						return 1;
					}
				}
			}
			else if (strline[i] == '/')
			{
				if ((i > 0 && strline[i-1] == 's') || (i > 1 && strline[i-1] == 'r' && strline[i-2] == 't'))
					slashCount = 2;
				else
					slashCount = 1;
				idx = i;
				inRegexp = true;
			}

			// quick fix to replace '#'
			if (i > 0 && strline[i] == '#' && strline[i-1] == '$')
				strline[i] = '$';

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
int CPerlCounter::CountDirectiveSLOC(filemap* fmap, results* result, filemap* fmapBak)
{
	bool contd = false, trunc_flag = false;
	size_t idx, strSize;
	unsigned int cnt = 0;
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$/\\<>.+?!@#$%^&*()-+*";
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
				// ensures the keyword stands alone, avoid, e.g., pack instead of package
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
			// if a directive or continuation of a directive (no continuation symbol found)
			if (iter->line[iter->line.length()-1] == ';')
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
int CPerlCounter::LanguageSpecificProcess(filemap* fmap, results* result, filemap* fmapBak)
{
	string strLSLOC;
	string strLSLOCBak;
	unsigned int l_paren_cnt = 0;
	bool l_forflag, found_forifwhile, found_while;
	char prev_char = 0;
	l_forflag = found_forifwhile = found_while = false;

	bool comment = false;
	unsigned int cnt = 0;
	size_t comPos = string::npos;
	size_t p;
	filemap::iterator fit, fitbak;
	string line, lineBak;
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_";
	bool data_continue = false;
	unsigned int phys_exec_lines = 0;
	unsigned int phys_data_lines = 0;
	unsigned int temp_lines = 0;
	unsigned int openBrackets = 0;
	StringVector loopLevel;

	for (fit = fmap->begin(), fitbak = fmapBak->begin(); fit != fmap->end(); fit++, fitbak++)
	{
		// insert blank at the beginning(for searching keywords)
		line = ' ' + fit->line;
		lineBak = ' ' + fitbak->line;
		size_t lineNumber = fit->lineNumber;

		if (!CUtil::CheckBlank(line))
		{
			if (comment == true)
			{
				p = line.find("\n");
				if (p != string::npos)
				{
					line.replace(0, p + 2, " ");
					lineBak.replace(0, p + 2, " ");
				}
				else
					continue;
			}

			comPos = line.find("#");
			if (comPos == string::npos)
				comment = false;
			else
			{
				comment = true;
				p = line.substr(comPos + 3).find("");
				if (p != string::npos)
				{
					line.replace(comPos, p - comPos + 1, " ");
					lineBak.replace(comPos, p - comPos + 1, " ");
					comment = false;
				}
				else
				{
					line.replace(comPos, line.size() - comPos, " ");
					lineBak.replace(comPos, lineBak.size() - comPos, " ");
				}
			}

			LSLOC(result, line, lineNumber, lineBak, strLSLOC, strLSLOCBak, l_paren_cnt, l_forflag, found_forifwhile,
				found_while, prev_char, data_continue, temp_lines, phys_exec_lines, phys_data_lines, openBrackets, loopLevel);

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
* \param openBrackets number of open brackets (no matching close bracket)
* \param loopLevel nested loop level
*/
void CPerlCounter::LSLOC(results* result, string line, size_t lineNumber, string lineBak, string &strLSLOC, string &strLSLOCBak, unsigned int &paren_cnt,
	bool &forflag, bool &found_forifwhile, bool &found_while, char &prev_char, bool &data_continue,
	unsigned int &temp_lines, unsigned int &phys_exec_lines, unsigned int &phys_data_lines,
	unsigned int &openBrackets, StringVector &loopLevel)
{
	size_t start = 0; // starting index of the working string
	size_t i = 0, strSize, pos;
	bool do_boolean, trunc_flag = false;
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_";
	unsigned int cnt = 0;
	unsigned int loopCnt = 0;
	StringVector::iterator lit;
	string tmp = CUtil::TrimString(strLSLOC);
	string tmp2;

	// check for the keyword do
	do_boolean = (CUtil::FindKeyword(tmp, "do") == tmp.length() - 2);

	// check the entire line for SLOC present in it
	while (i < line.length())
	{
		if (line[i] == ';' || line[i] == '{')
		{
			// LSLOC terminators
			// ';' any valid perl command ends with a ';' terminator
			// do statements start with a '{' and ends with '}'

			if (line[i] == ';' && paren_cnt > 0)
			{
				// for a 'for' statement counter is incremented.
				i++;
				continue;
			}

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

			if (found_while && found_forifwhile)
			{
				found_while = false;
				found_forifwhile = false;
				start = i + 1;
				i++;
				continue;
			}

			if (line[i] == '{')
			{
				// case for(...); and if (...) {
				// these specials are handled
				if (found_forifwhile)
				{
					found_forifwhile = false;
					start = i + 1;
					i++;
					continue;
				}

				// check if 'do' precedes '{'
				if (!do_boolean)
				{
					// find for 'do' in string before tmp string
					tmp = CUtil::TrimString(line.substr(start, i - start));

					// check for 'do' statement
					do_boolean = (tmp == "do");
				}
				if (do_boolean)
				{
					if (print_cmplx)
					{
						if (loopLevel.size() > 0) loopLevel.pop_back();
						loopLevel.push_back("do");
					}

					do_boolean = false;
					start = i + 1;
					i++;
					continue; // do not store '{' following 'do'
				}
			}

			if (line[i] == ';' && prev_char == '}')
			{
				i++;
				continue;
			}

			// the 'for(...)' or 'while(..)' or anything with the '{' on the next line gets counted as an extra SLOC
			// so to avoid that increment the counter and continue
			if (line[i] == '{' && prev_char == ')')
			{
				i++;
				continue;
			}

			// check for expression modifiers using 'foreach', 'while', 'if', 'unless', 'until' (for example, statement unless condition;)
			pos = string::npos;
			if (line[i] == ';')
			{
				// check for empty statement (=1 LSLOC)
				if (CUtil::TrimString(line.substr(start, i + 1 - start)) == ";" && strLSLOC.length() < 1)
				{
					strLSLOC = ";";
					strLSLOCBak = ";";
				}
				else
				{
					tmp = CUtil::TrimString(strLSLOC + line.substr(start, i + 1 - start));
					pos = CUtil::FindKeyword(tmp, "foreach");
					if (pos == string::npos)
					{
						pos = CUtil::FindKeyword(tmp, "while");
						if (pos == string::npos)

							pos = CUtil::FindKeyword(tmp, "if");
						if (pos == string::npos)
						{
							pos = CUtil::FindKeyword(tmp, "unless");
							if (pos == string::npos)
								pos = CUtil::FindKeyword(tmp, "until");
						}
					}
				}
			}
			if (pos != string::npos)
			{
				// capture statement before modifier
				tmp2 = CUtil::TrimString(strLSLOCBak + lineBak.substr(start, i - start));
				strSize = CUtil::TruncateLine(pos, 0, this->lsloc_truncate, trunc_flag);
				if (strSize > 0)
				{
					strLSLOC = tmp.substr(0, strSize);
					strLSLOCBak = tmp2.substr(0, strSize);
				}
				if (result->addSLOC(strLSLOCBak, lineNumber, trunc_flag))
					result->exec_lines[LOG]++;
				strLSLOC = "";
				strLSLOCBak = "";

				strSize = CUtil::TruncateLine(tmp.length() - pos, 0, this->lsloc_truncate, trunc_flag);
				if (strSize > 0)
				{
					strLSLOC = tmp.substr(pos, strSize);
					strLSLOCBak = tmp2.substr(pos, strSize);
				}
				if (result->addSLOC(strLSLOCBak, lineNumber, trunc_flag))
					result->exec_lines[LOG]++;
				found_forifwhile = false;
			}
			else
			{
				strSize = CUtil::TruncateLine(i - start, strLSLOC.length(), this->lsloc_truncate, trunc_flag);
				if (strSize > 0)
				{
					strLSLOC += line.substr(start, strSize);
					strLSLOCBak += lineBak.substr(start, strSize);
				}
				tmp = strLSLOC;
				if (result->addSLOC(strLSLOCBak, lineNumber, trunc_flag))
				{
					cnt = 0;
					CUtil::CountTally(strLSLOC, data_name_list, cnt, 1, exclude, "", "", &result->data_name_count);

					temp_lines++;
					if (data_continue == true)
					{
						result->data_lines[LOG]++;
						phys_data_lines = temp_lines;
					}
					else
					{
						if (cnt > 0)
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
				else if (data_continue == true)
					phys_data_lines = temp_lines;
				else
					phys_exec_lines = temp_lines;
			}
			data_continue = false;
			temp_lines = 0;
			strLSLOC = "";
			strLSLOCBak = "";
			start = i + 1;
		}
		else if (line[i] == '}')
		{
			// also, {} is also skipped, empty block is not counted
			if (prev_char == ';' || prev_char == '{')
				start = i + 1; 

			// record close bracket for nested loop processing
			if (print_cmplx)
			{
				if (openBrackets > 0)
					openBrackets--;
				if (loopLevel.size() > 0)
					loopLevel.pop_back();
			}
		}
		else if (line[i] == '(')
		{ 
			if (!forflag)
			{
				// handle 'for', 'foreach', 'while', 'if', 'elsif,  and 'unless'
				tmp = "xxx " + CUtil::TrimString(line.substr(start, i));
				if ((CUtil::FindKeyword(tmp, "for") != string::npos) || (CUtil::FindKeyword(tmp, "foreach") != string::npos) ||
					(CUtil::FindKeyword(tmp, "while")!= string::npos) || (CUtil::FindKeyword(tmp, "if") != string::npos) ||
					(CUtil::FindKeyword(tmp, "elsif") != string::npos) || (CUtil::FindKeyword(tmp, "unless") != string::npos) ||
					(CUtil::FindKeyword(tmp, "until") != string::npos))
				{
					forflag = true;
					paren_cnt++;

					if (print_cmplx && (loopLevel.size() > openBrackets) && (openBrackets > 0))
						loopLevel.pop_back();

					if (CUtil::FindKeyword(tmp, "while")!= string::npos)
					{
						if (print_cmplx)
							loopLevel.push_back("while");
						found_while = true;
					}
					else if (CUtil::FindKeyword(tmp, "until")!= string::npos)
					{
						if (print_cmplx)
							loopLevel.push_back("until");
						found_while = true;
					}
					else if (print_cmplx)
					{
						if (CUtil::FindKeyword(tmp, "for") != string::npos)
							loopLevel.push_back("for");
						else if (CUtil::FindKeyword(tmp, "foreach") != string::npos)
							loopLevel.push_back("foreach");

						// record nested loop level
						if (CUtil::FindKeyword(tmp, "if") == string::npos && CUtil::FindKeyword(tmp, "elsif") == string::npos &&
							CUtil::FindKeyword(tmp, "unless") == string::npos)
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
			else
				paren_cnt++;
		}
		else if (line[i] == ')')
		{
			/* 
			cases 
			'while(...);', 
			'while(...) {' and 
			'} while(...);' 
			is handled in this case
			*/
			if (forflag)
			{
				if (paren_cnt > 0)
					paren_cnt--;
				if (paren_cnt == 0)
				{
					// handling 'for', 'foreach', 'while', 'if', 'elsif', 'unless', 'until'
					// check for expression modifiers using 'foreach', 'while', 'if', 'unless', 'until' (for example, statement unless (condition);)
					tmp = CUtil::TrimString(strLSLOC + line.substr(start, i + 1 - start));
					pos = CUtil::FindKeyword(tmp, "foreach");
					if (pos == string::npos)
					{
						pos = CUtil::FindKeyword(tmp, "while");
						if (pos == string::npos)
						{
							pos = CUtil::FindKeyword(tmp, "if");
							if (pos == string::npos)
							{
								pos = CUtil::FindKeyword(tmp, "unless");
								if (pos == string::npos)
									pos = CUtil::FindKeyword(tmp, "until");
							}
						}
					}
					if (pos != string::npos)
					{
						// capture statement before modifier
						tmp2 = CUtil::TrimString(strLSLOCBak + lineBak.substr(start, i + 1 - start));
						strSize = CUtil::TruncateLine(pos, 0, this->lsloc_truncate, trunc_flag);
						if (strSize > 0)
						{
							strLSLOC = tmp.substr(0, strSize);
							strLSLOCBak = tmp2.substr(0, strSize);
						}
						if (result->addSLOC(strLSLOCBak, lineNumber, trunc_flag))
							result->exec_lines[LOG]++;
						strLSLOC = "";
						strLSLOCBak = "";

						strSize = CUtil::TruncateLine(tmp.length() - pos, 0, this->lsloc_truncate, trunc_flag);
						if (strSize > 0)
						{
							strLSLOC = tmp.substr(pos, strSize);
							strLSLOCBak = tmp2.substr(pos, strSize);
						}
						if (result->addSLOC(strLSLOCBak, lineNumber, trunc_flag))
							result->exec_lines[LOG]++;
						found_forifwhile = false;

						// skip trailing ';'
						tmp = CUtil::TrimString(line.substr(i + 1));
						if (tmp.length() > 0 && tmp[0] == ';')
							i++;
					}
					else
					{
						strSize = CUtil::TruncateLine(i + 1 - start, strLSLOC.length(), this->lsloc_truncate, trunc_flag);
						if (strSize > 0)
						{
							strLSLOC += line.substr(start, strSize);
							strLSLOCBak += lineBak.substr(start, strSize);
						}
						tmp = strLSLOC;
						if (result->addSLOC(strLSLOCBak, lineNumber, trunc_flag))
							result->exec_lines[LOG]++;
						found_forifwhile = true;
					}
					strLSLOC = "";
					strLSLOCBak = "";
					phys_exec_lines = temp_lines;
					temp_lines = 0;
					start = i + 1; 
					forflag = false;
				}
			}
		}

		if (line[i] != ' ' && line[i] != '\t')
		{
			// if ;}}} --> don't count }}} at all
			// also, if {}}} --> don't count }}} at all
			if (!(line[i] == '}' && (prev_char == ';' || prev_char == '{'))) // see case '}' above
				prev_char = line[i];

			// change to not found if a char appears before
			if (line[i] != ')' && found_forifwhile)
				found_forifwhile = false;

			if (CUtil::FindKeyword(line, "or", i, i + 2, true) == i)
			{
				strSize = CUtil::TruncateLine(i + 1 - start, strLSLOC.length(), this->lsloc_truncate, trunc_flag);
				if (strSize > 0)
				{
					strLSLOC += line.substr(start, strSize);
					strLSLOCBak += lineBak.substr(start, strSize);
				}
				tmp = strLSLOC;
				if (result->addSLOC(strLSLOCBak, lineNumber, trunc_flag))
					result->exec_lines[LOG]++;
				phys_exec_lines = temp_lines;
				temp_lines = 0;
				strLSLOC = "";
				strLSLOCBak = "";
				start = i;
			}
		}
		i++;
	}

	tmp2 = CUtil::TrimString(line.substr(start, i - start));
	strSize = CUtil::TruncateLine(tmp2.length(), strLSLOC.length(), this->lsloc_truncate, trunc_flag);
	if (strSize > 0)
	{
		strLSLOC += tmp2.substr(0, strSize);
		tmp2 = CUtil::TrimString(lineBak.substr(start, i - start));
		strLSLOCBak += tmp2.substr(0, strSize);
	}
	if (tmp == "")
		found_forifwhile = found_while = false;

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
* \param functionCount function count found
*
* \return 1 if function name is found
*/
int CPerlCounter::ParseFunctionName(const string &line, string &lastline,
	filemap &functionStack, string &functionName, unsigned int &functionCount)
{
	string tline, str;
	size_t i, idx, tidx, cnt;
	unsigned int fcnt;

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
			if (str.find('(') != string::npos && str[0] != '(')
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
	else
		lastline = tline;

	idx = line.find('}');
	if (idx != string::npos && !functionStack.empty())
	{
		str = functionStack.back().line;
		fcnt = functionStack.back().lineNumber;
		functionStack.pop_back();
		idx = CUtil::FindKeyword(str, "sub");
		if (idx != string::npos && idx + 4 < str.length())
		{
			functionName = CUtil::ClearRedundantSpaces(str.substr(idx + 4));
			functionCount = fcnt;
			lastline.erase();
			return 1;
		}
		lastline.erase();
	}

	// check stack for any "sub"
	idx = string::npos;
	if (!functionStack.empty())
	{
		for (i = 0; i < functionStack.size(); i++)
		{
			idx = CUtil::FindKeyword(functionStack[i].line, "sub");
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

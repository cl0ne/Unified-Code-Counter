//! Code counter class methods for the Ada language.
/*!
* \file CAdaCounter.cpp
*
* This file contains the code counter class methods for the Ada language.
*/

#include "CAdaCounter.h"

/*!
* Constructs a CAdaCounter object.
*/
CAdaCounter::CAdaCounter()
{
	classtype = ADA;
	language_name = "Ada";
	casesensitive = false;
    
    //Modification: 11.2016 Ext-4 starts
    file_extension = CUtil::getExtensionsToLanguage("Ada", file_extension);
    /*
	file_extension.push_back(".ada");
	file_extension.push_back(".a");
	file_extension.push_back(".adb");
	file_extension.push_back(".ads");*/
    //Modification: 11.2016 Ext-4

	LineCommentStart.push_back("--");

	QuoteStart = "\"";
	QuoteEnd = "\"";
	QuoteEscapeRear = '\"';

	directive.push_back("controlled");
	directive.push_back("elaborate");
	directive.push_back("inline");
	directive.push_back("interface");
	directive.push_back("list");
	directive.push_back("memory_size");
	directive.push_back("optimize");
	directive.push_back("pack");
	directive.push_back("page");
	directive.push_back("pragma");
	directive.push_back("priority");
	directive.push_back("shared");
	directive.push_back("storage_unit");
	directive.push_back("suppress");
	directive.push_back("system_name");

	data_name_list.push_back("access");
	data_name_list.push_back("array");
	data_name_list.push_back("body");
	data_name_list.push_back("constant");
	data_name_list.push_back("declare");
	data_name_list.push_back("function");
	data_name_list.push_back("generic");
	data_name_list.push_back("limited");
	data_name_list.push_back("new");
	data_name_list.push_back("package");
	data_name_list.push_back("private");
	data_name_list.push_back("procedure");
	data_name_list.push_back("record");
	data_name_list.push_back("renames");
	data_name_list.push_back("separate");
	data_name_list.push_back("subtype");
	data_name_list.push_back("task");
	data_name_list.push_back("type");
	data_name_list.push_back("use");
	data_name_list.push_back("with");
	
	exec_name_list.push_back("abort");
	exec_name_list.push_back("accept");
	exec_name_list.push_back("begin");
	exec_name_list.push_back("case");
	exec_name_list.push_back("delay");
	exec_name_list.push_back("else");
	exec_name_list.push_back("elsif");
	exec_name_list.push_back("end");
	exec_name_list.push_back("entry");
	exec_name_list.push_back("exception");
	exec_name_list.push_back("exit");	
	exec_name_list.push_back("goto");
	exec_name_list.push_back("if");
	exec_name_list.push_back("loop");
	exec_name_list.push_back("others");
	exec_name_list.push_back("raise");
	exec_name_list.push_back("return");
	exec_name_list.push_back("select");
	exec_name_list.push_back("terminate");
	exec_name_list.push_back("when");

	math_func_list.push_back("exp");
	math_func_list.push_back("random");
	math_func_list.push_back("sqrt");
	
	trig_func_list.push_back("arccos");
	trig_func_list.push_back("arccosh");
	trig_func_list.push_back("arccot");
	trig_func_list.push_back("arccoth");
	trig_func_list.push_back("arcsin");
	trig_func_list.push_back("arcsinh");
	trig_func_list.push_back("arctan");
	trig_func_list.push_back("arctanh");
	trig_func_list.push_back("cos");
	trig_func_list.push_back("cosh");
	trig_func_list.push_back("cot");
	trig_func_list.push_back("coth");
	trig_func_list.push_back("sin");
	trig_func_list.push_back("sinh");
	trig_func_list.push_back("tan");
	trig_func_list.push_back("tanh");

	log_func_list.push_back("log");

	cmplx_calc_list.push_back("+");
	cmplx_calc_list.push_back("-");
	cmplx_calc_list.push_back("*");
	cmplx_calc_list.push_back("/");
	cmplx_calc_list.push_back("mod");
	cmplx_calc_list.push_back("rem");

	cmplx_cond_list.push_back("case");
	cmplx_cond_list.push_back("else");
	cmplx_cond_list.push_back("elsif");
	cmplx_cond_list.push_back("if");
	cmplx_cond_list.push_back("loop");
	cmplx_cond_list.push_back("when");

	cmplx_logic_list.push_back("=");
	cmplx_logic_list.push_back("<");
	cmplx_logic_list.push_back(">");
	cmplx_logic_list.push_back("/=");
	cmplx_logic_list.push_back(">=");
	cmplx_logic_list.push_back("<=");
	cmplx_logic_list.push_back("&");
	cmplx_logic_list.push_back("and");
	cmplx_logic_list.push_back("or");
	cmplx_logic_list.push_back("xor");
	cmplx_logic_list.push_back("in");
	cmplx_logic_list.push_back("not in");

	cmplx_preproc_list.push_back("controlled");
	cmplx_preproc_list.push_back("elaborate");
	cmplx_preproc_list.push_back("inline");
	cmplx_preproc_list.push_back("interface");
	cmplx_preproc_list.push_back("list");
	cmplx_preproc_list.push_back("memory_size");
	cmplx_preproc_list.push_back("optimize");
	cmplx_preproc_list.push_back("pack");
	cmplx_preproc_list.push_back("page");
	cmplx_preproc_list.push_back("pragma");
	cmplx_preproc_list.push_back("priority");
	cmplx_preproc_list.push_back("shared");
	cmplx_preproc_list.push_back("storage_unit");
	cmplx_preproc_list.push_back("suppress");
	cmplx_preproc_list.push_back("system_name");

	cmplx_assign_list.push_back(":=");

	//cmplx_cyclomatic_list.push_back("if");
        cmplx_cyclomatic_list.push_back("if");
	cmplx_cyclomatic_list.push_back("elsif");
	//cmplx_cyclomatic_list.push_back("IIf");
	cmplx_cyclomatic_list.push_back("for");
	cmplx_cyclomatic_list.push_back("while");
	//cmplx_cyclomatic_list.push_back("Until");
	//cmplx_cyclomatic_list.push_back("Catch");
	cmplx_cyclomatic_list.push_back("when");
	//cmplx_cyclomatic_list.push_back("case");

	ignore_cmplx_cyclomatic_list.push_back("end if");

	//cmplx_cyclomatic_logic_list.push_back("&");
	cmplx_cyclomatic_logic_list.push_back("and");
	cmplx_cyclomatic_logic_list.push_back("or");
	//cmplx_cyclomatic_logic_list.push_back("xor");

	cmplx_cyclomatic_case_list.push_back("when");
	cmplx_cyclomatic_switch_list.push_back("end case");
	cmplx_cyclomatic_default_list.push_back("others");
}

/*!
* Replaces quoted strings inside a string starting at idx_start with '$'.
* Handles special cases for Ada literal strings.
*
* \param strline string to be processed
* \param idx_start index of line character to start search
* \param contd specifies the quote string is continued from the previous line
* \param CurrentQuoteEnd end quote character of the current status
*
* \return method status
*/
int CAdaCounter::ReplaceQuote(string &strline, size_t &idx_start, bool &contd, char &CurrentQuoteEnd)
{
	size_t idx = 0;
	bool	done = false;
	while( ! done )
	{
		idx = strline.find("'\"'", idx);	// replace all '"' by '$'
		if (idx != string::npos)
			strline.replace(idx, 3, 3, '$');
		else
			break;
	}
	return CCodeCounter::ReplaceQuote(strline, idx_start, contd, CurrentQuoteEnd);
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
int CAdaCounter::CountDirectiveSLOC(filemap* fmap, results* result, filemap* fmapBak)
{
	bool contd = false, trunc_flag = false;
	size_t idx, strSize;
	unsigned int cnt = 0;
	string strDirLine = "";
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$:";

	filemap::iterator itfmBak = fmapBak->begin();
	for (filemap::iterator iter = fmap->begin(); iter != fmap->end(); iter++, itfmBak++)
	{
		if (CUtil::CheckBlank(iter->line))
			continue;
		size_t lineNumber = iter->lineNumber;

		if (print_cmplx)
		{
			cnt = 0;
			CUtil::CountTally(" " + iter->line, directive, cnt, 1, exclude, "", "", &result->directive_count, false);
		}

		if (!contd)
		{
			// if not a continuation of a previous directive
			for (vector<string>::iterator viter = directive.begin(); viter != directive.end(); viter++)
			{
				// merged bug fix for considering only stand-alone keywords
				// e.g. package should not be considered a directive (only 'pack' is)
				if (((idx = CUtil::FindKeyword(iter->line, *viter, 0, TO_END_OF_STRING, false)) != string::npos) && idx == 0)
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
int CAdaCounter::LanguageSpecificProcess(filemap* fmap, results* result, filemap* fmapBak)
{
	bool			found_accept	= false;
	string			strLSLOC		= "";
	string			strLSLOCBak		= "";
	unsigned int	cnt				= 0;
	unsigned int	loopLevel		= 0;

	filemap::iterator fit, fitbak;
	string line, lineBak, tmp;
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$:";

	unsigned int l_paren_cnt = 0;
	bool l_foundblock, found_forifwhile, found_end, found_type, found_is;
	l_foundblock = found_forifwhile = found_end = found_is = false;

	for (fit = fmap->begin(), fitbak = fmapBak->begin(); fit != fmap->end(); fit++, fitbak++)
	{
		// insert blank at the beginning (for searching keywords)
		line = ' ' + fit->line;
		lineBak = ' ' + fitbak->line;

		if (!CUtil::CheckBlank(line))
		{
			// blank line means blank_line/comment_line/directive
			// call SLOC function to detect logical SLOC and add to result
			LSLOC(result, line, fit->lineNumber, lineBak, strLSLOC, strLSLOCBak, l_paren_cnt, l_foundblock,
				found_forifwhile, found_end, found_type, found_is, found_accept, loopLevel);

			cnt = 0;
			CUtil::CountTally(line, data_name_list, cnt, 1, exclude, "", "", NULL, false);

			// need to check also if the data line continues
			if (cnt > 0)
				result->data_lines[PHY]++;
			else
				result->exec_lines[PHY]++;

			if (print_cmplx)
			{
				cnt = 0;
				CUtil::CountTally(line, exec_name_list, cnt, 1, exclude, "", "", &result->exec_name_count, false);
			}
		}
	}
	return 1;
}

/*!
* Processes a logical line of code.
* This method is called after a logical SLOC is determined.
* The method adds LSLOC to the result, increases counts, and resets variables.
*
* \param result counter results
* \param strLSLOC processed logical string
* \param strLSLOCBak original logical string
* \param found_block found block flag
* \param found_forifwhile found for, if, or while flag
* \param found_end found end flag
* \param found_type found type flag
* \param found_is found is flag
* \param found_accept found accept flag
* \param trunc_flag truncate lines?
*/
void CAdaCounter::FoundSLOC(results* result, size_t lineNumber, string &strLSLOC, string &strLSLOCBak, bool &found_block, bool &found_forifwhile,
							bool &found_end, bool &found_type, bool &found_is, bool &found_accept, bool &trunc_flag)
{
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$:";

	// add to the list for comparison purpose
	if (result->addSLOC(CUtil::TrimString(strLSLOCBak), lineNumber, trunc_flag))
	{
		// determine logical type, data declaration or executable
		unsigned int cnt = 0;
		CUtil::CountTally(strLSLOC, data_name_list, cnt, 1, exclude, "", "", &result->data_name_count, false);
		if (cnt > 0)
			result->data_lines[LOG]++;
		else
			result->exec_lines[LOG]++;
	}

	// reset all variables whenever a new statement/logical SLOC is found
	strLSLOC = "";
	strLSLOCBak = "";
	found_block = false;
	found_forifwhile = false;
	found_end = false;
	found_type = false;
	found_is = false;
	found_accept = false;
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
* \param found_block found block flag
* \param found_forifwhile found for, if, or while flag
* \param found_end found end flag
* \param found_type found type flag
* \param found_is found is flag
* \param found_accept found accept flag
* \param loopLevel nested loop level
*/
void CAdaCounter::LSLOC(results* result, string line, size_t lineNumber, string lineBak, string &strLSLOC, string &strLSLOCBak, 
						unsigned int &paren_cnt, bool &found_block, bool &found_forifwhile, bool &found_end, 
						bool &found_type, bool &found_is, bool &found_accept, unsigned int &loopLevel)
{
	size_t start = 0; //starting index of the working string
	size_t i = 0, tempi, strSize;
	string templine = CUtil::TrimString(line);
	string tmp;
	bool trunc_flag = false;
	string keywordchars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$";

	// there may be more than 1 logical SLOC in a line
	for (i = 0; i < line.length(); i++)
	{
		switch (line[i])
		{
		case ';':
			/*if (paren_cnt > 0)
				break;*/ //this is not necessary
			if (!found_end)
			{
				// check for empty statement (=1 LSLOC)
				if (CUtil::TrimString(line.substr(start, i + 1 - start)) == ";" && strLSLOC.length() < 1)
				{
					strLSLOC = ";";
					strLSLOCBak = ";";
				}
				else
				{
					strSize = CUtil::TruncateLine(i - start, strLSLOC.length(), this->lsloc_truncate, trunc_flag);
					if (strSize > 0)
					{
						strLSLOC += line.substr(start, strSize);
						strLSLOCBak += lineBak.substr(start, strSize);
					}
				}
				FoundSLOC(result, lineNumber, strLSLOC, strLSLOCBak, found_block, found_forifwhile, 
					found_end, found_type, found_is, found_accept, trunc_flag);
			}
			else
			{
				found_end = false;	// end xxx
				found_block = false;
				found_is = false;
				found_forifwhile = false;
				found_type = false;
				found_accept = false;
				strLSLOC = "";
				strLSLOCBak = "";
			}
			start = i + 1;
			break;
		case '(':
			if (found_type)
				found_type = false;
			paren_cnt++;
			break;
		case ')':
			if (paren_cnt > 0)
				paren_cnt--;
			break;
		}

		// continue the following processing only if line[i] is not in a middle of a word
		if (keywordchars.find(line[i]) != string::npos && i < line.length() - 1)
			continue;

		// if it ends in xxx, then it has already been counted, so ignore it
		tmp = "xxx " + CUtil::TrimString(line.substr(start, i + 1 - start));
		tempi = CUtil::FindKeyword(tmp, "end", 0, TO_END_OF_STRING, false);
		if (tempi != string::npos)
		{
			found_end = true;

			// record end loop for nested loop processing
			if (print_cmplx)
			{
				tmp = CUtil::TrimString(line.substr(start, i + 5 - start));
				if (CUtil::FindKeyword(tmp, "end loop", 0, TO_END_OF_STRING, false) != string::npos)
					if (loopLevel > 0)
						loopLevel--;
			}
			start = i + 1;
		}

		if (!found_end)
		{
			if (!found_forifwhile)
			{
				if (CUtil::FindKeyword(tmp, "for", 0 , TO_END_OF_STRING, false) != string::npos ||
					CUtil::FindKeyword(tmp, "while", 0, TO_END_OF_STRING, false) != string::npos ||
					CUtil::FindKeyword(tmp, "if", 0, TO_END_OF_STRING, false) != string::npos ||
					CUtil::FindKeyword(tmp, "elsif", 0, TO_END_OF_STRING, false) != string::npos)
				{
					found_forifwhile = true;
				}

				// 'exception' is removed because it is not counted
				if (CUtil::FindKeyword(tmp, "loop", 0, TO_END_OF_STRING, false) != string::npos)
				{
					// found a SLOC
					strSize = CUtil::TruncateLine(i + 1 - start, strLSLOC.length(), this->lsloc_truncate, trunc_flag);
					if (strSize > 0)
					{
						strLSLOC += line.substr(start, strSize);
						strLSLOCBak += lineBak.substr(start, strSize);
					}
					FoundSLOC(result, lineNumber, strLSLOC, strLSLOCBak, found_block, found_forifwhile,
						found_end, found_type, found_is, found_accept, trunc_flag);

					start = i + 1;

					// record nested loop level
					if (print_cmplx)
					{
						loopLevel++;
						if ((unsigned int)result->cmplx_nestloop_count.size() < loopLevel)
							result->cmplx_nestloop_count.push_back(1);
						else
							result->cmplx_nestloop_count[loopLevel-1]++;
					}
					continue;
				}
			}
			else if (CUtil::FindKeyword(tmp, "loop", 0, TO_END_OF_STRING, false) != string::npos ||
				CUtil::FindKeyword(tmp, "then", 0, TO_END_OF_STRING, false) != string::npos ||
				CUtil::FindKeyword(tmp, "record", 0, TO_END_OF_STRING, false) != string::npos) // for..use..record
			{
				// found a SLOC
				strSize = CUtil::TruncateLine(i + 1 - start, strLSLOC.length(), this->lsloc_truncate, trunc_flag);
				if (strSize > 0)
				{
					strLSLOC += line.substr(start, strSize);
					strLSLOCBak += lineBak.substr(start, strSize);
				}
				FoundSLOC(result, lineNumber, strLSLOC, strLSLOCBak, found_block, found_forifwhile,
					found_end, found_type, found_is, found_accept, trunc_flag);
				start = i + 1;

				// record nested loop level
				if (print_cmplx)
				{
					if (CUtil::FindKeyword(tmp, "loop", 0, TO_END_OF_STRING, false) != string::npos)
					{
						loopLevel++;
						if ((unsigned int)result->cmplx_nestloop_count.size() < loopLevel)
							result->cmplx_nestloop_count.push_back(1);
						else
							result->cmplx_nestloop_count[loopLevel-1]++;
					}
				}
				continue;
			}
			
			// similarly, check for procedure, task, function - it ends with 'is' keyword
			// procedure ... is...
			// package ... is ...
			if (!found_block)
			{
				if (CUtil::FindKeyword(tmp, "procedure", 0, TO_END_OF_STRING, false) != string::npos ||
					CUtil::FindKeyword(tmp, "function", 0, TO_END_OF_STRING, false) != string::npos ||
					CUtil::FindKeyword(tmp, "package", 0, TO_END_OF_STRING, false) !=string::npos ||
					CUtil::FindKeyword(tmp, "task", 0, TO_END_OF_STRING, false) != string::npos ||
					CUtil::FindKeyword(tmp, "case",0, TO_END_OF_STRING, false) != string::npos ||
					CUtil::FindKeyword(tmp, "protected", 0, TO_END_OF_STRING, false) != string::npos)
				{
					found_block = true;
				}
			}
			else // procedure...is...
			{
				// the 'if' statement below attempts to 'alleviate' the issue with
				// procedure Swap is new Exchange(Elem => Integer);
				// procedure Test1 is begin end Test1;
				// only add new SLOC if 'is' is at the end of line and follows 'procedure', etc.
				// NOTE: this implementation may not be complete
				tempi = CUtil::FindKeyword(templine, "is", 0, TO_END_OF_STRING, false);
				if (tempi == templine.length() - 2)
				{
					strSize = CUtil::TruncateLine(i + 1 - start, strLSLOC.length(), this->lsloc_truncate, trunc_flag);
					if (strSize > 0)
					{
						strLSLOC += line.substr(start, strSize);
						strLSLOCBak += lineBak.substr(start, strSize);
					}
					FoundSLOC(result, lineNumber, strLSLOC, strLSLOCBak, found_block, found_forifwhile,
						found_end, found_type, found_is, found_accept, trunc_flag);
					start = i + 1;
					found_is = true;
					continue;
				}
			}
			if (!found_type)
			{
				if (CUtil::FindKeyword(tmp, "type", 0, TO_END_OF_STRING, false) != string::npos)
					found_type = true;
			}
			else
			{
				if (CUtil::FindKeyword(tmp, "record", 0, TO_END_OF_STRING, false) != string::npos)
				{
					// the 'if' statement below attempts to resolves the issue with 
					// type Expression is tagged null record;
					// so, ignore this case.
					// NOTE: this implementation may not be complete
					if (templine.at(templine.length() - 1) != ';')
					{
						strSize = CUtil::TruncateLine(i + 1 - start, strLSLOC.length(), this->lsloc_truncate, trunc_flag);
						if (strSize > 0)
						{
							strLSLOC += line.substr(start, strSize);
							strLSLOCBak += lineBak.substr(start, strSize);
						}
						FoundSLOC(result, lineNumber, strLSLOC, strLSLOCBak, found_block, found_forifwhile,
							found_end, found_type, found_is, found_accept, trunc_flag);
						start = i + 1;
						continue;				
					}
				}
			}

			// process 'select...end select;', 'accept ... end accept;'
			// 'record ... end record;' is handled via 'type'
			// select ... end select;  --> only one word statement 'select'
			// accept id... do ... end [id]; --> SLOC starting from 'accept' to 'do'
			// find 'do' only already found 'accept'
			if (CUtil::FindKeyword(tmp, "select", 0, TO_END_OF_STRING, false) != string::npos)
			{
				// found 'select' statement, one SLOC
				strSize = CUtil::TruncateLine(i + 1 - start, strLSLOC.length(), this->lsloc_truncate, trunc_flag);
				if (strSize > 0)
				{
					strLSLOC += line.substr(start, strSize);
					strLSLOCBak += lineBak.substr(start, strSize);
				}
				FoundSLOC(result, lineNumber, strLSLOC, strLSLOCBak, found_block, found_forifwhile,
					found_end, found_type, found_is, found_accept, trunc_flag);
				start = i + 1;
				continue;
			}

			if (!found_accept)
			{
				if (CUtil::FindKeyword(tmp, "accept", 0, TO_END_OF_STRING, false) != string::npos)
					found_accept = true;
			}
			else
			{
				if (CUtil::FindKeyword(tmp, "do", 0, TO_END_OF_STRING, false) != string::npos)
				{
					// found a SLOC
					strSize = CUtil::TruncateLine(i + 1 - start, strLSLOC.length(), this->lsloc_truncate, trunc_flag);
					if (strSize > 0)
					{
						strLSLOC += line.substr(start, strSize);
						strLSLOCBak += lineBak.substr(start, strSize);
					}
					FoundSLOC(result, lineNumber, strLSLOC, strLSLOCBak, found_block, found_forifwhile,
						found_end, found_type, found_is, found_accept, trunc_flag);
					start = i + 1;
				}
			}
		}
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
	if (tmp == "") 
		found_forifwhile = false;
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
int CAdaCounter::ParseFunctionName(const string &line, string &/*lastline*/,
	filemap &functionStack, string &functionName, unsigned int &functionCount)
{
	string str;
	size_t idx;
	//unsigned int fcnt; waring fix

	idx = CUtil::FindKeyword(line, "procedure");
	if (idx != string::npos)
	{
		if (idx + 10 < line.length())
		{
			str = line.substr(idx + 10);
			idx = str.find("is");
			if (idx != string::npos)
			{
				str = CUtil::ClearRedundantSpaces(str.substr(0, idx));
			}
			lineElement element(++functionCount, str);
			functionStack.push_back(element);
		}
	}
	else
	{
		idx = CUtil::FindKeyword(line, "function");
		if (idx != string::npos)
		{
			if (idx + 9 < line.length())
			{
				str = line.substr(idx + 9);
				idx = str.find("(");
				if (idx != string::npos)
				{
					str = CUtil::ClearRedundantSpaces(str.substr(0, idx));
				}
				lineElement element(++functionCount, str);
				functionStack.push_back(element);
			}
		}
	}

	idx = CUtil::FindKeyword(line, "with");
	if (idx == string::npos)
	{
		idx = CUtil::FindKeyword(line, "use");
		if (idx == string::npos)
		{
			if (functionStack.empty())
			{
				// dealing with some code out of any subroutines, it a "main" code
				return 2;
			}
		}
	}

	idx = CUtil::FindKeyword(line, "end");
	if (idx != string::npos)
	{
                // Modification: 2018.01
                if (!functionStack.empty())
                {
            	        idx = line.find(functionStack.back().line);
            	        if (idx != string::npos)
		        {
			        functionName = functionStack.back().line;
			        functionCount = functionStack.back().lineNumber;
			        functionStack.pop_back();
			        return 1;
		        }
                }
	}
	
	return 0;
}

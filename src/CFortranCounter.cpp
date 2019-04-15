//! Code counter class methods for the Fortran language.
/*!
* \file CFortranCounter.cpp
*
* This file contains the code counter class methods for the Fortran language.
* This includes F77, F90, F95, and F03 including fixed and free formats.
*/

#include "CFortranCounter.h"

/*!
* Constructs a CFortranCounter object.
*/
CFortranCounter::CFortranCounter()
{
	classtype = FORTRAN;
	language_name = "Fortran";
	casesensitive = false;
    
    //Modification: 11.2016 Ext-4
    file_extension = CUtil::getExtensionsToLanguage("Fortran", file_extension);
    
    /*
	file_extension.push_back(".f");
	file_extension.push_back(".for");
	file_extension.push_back(".f77");
	file_extension.push_back(".f90");
	file_extension.push_back(".f95");
	file_extension.push_back(".f03");
	file_extension.push_back(".hpf");*/

	QuoteStart = "\"'";
	QuoteEnd = "\"'";
	ContinueLine = "&";

	LineCommentStart.push_back("!");

	exclude_keywords.push_back("case default");
	exclude_keywords.push_back("else");
	exclude_keywords.push_back("end");
	exclude_keywords.push_back("endblockdata");
	exclude_keywords.push_back("enddo");
	exclude_keywords.push_back("endfile");
	exclude_keywords.push_back("endfunction");
	exclude_keywords.push_back("endif");
	exclude_keywords.push_back("endinterface");
	exclude_keywords.push_back("endmodule");
	exclude_keywords.push_back("endprogram");
	exclude_keywords.push_back("endselect");
	exclude_keywords.push_back("endsubroutine");
	exclude_keywords.push_back("endtype");
	exclude_keywords.push_back("endwhere");
	exclude_keywords.push_back("endforall");	// f95+ only
	exclude_keywords.push_back("endassociate");	// f03+ only
	exclude_keywords.push_back("endenum");		// f03+ only

	c_keywords.push_back("call");
	c_keywords.push_back("case default");
	c_keywords.push_back("class default");		// f03+ only
	c_keywords.push_back("character");
	c_keywords.push_back("common");
	c_keywords.push_back("complex");
	c_keywords.push_back("contains");
	c_keywords.push_back("continue");
	c_keywords.push_back("cycle");

	directive.push_back("dictionary");
	directive.push_back("include");
	directive.push_back("options");

	data_name_list.push_back("allocate");
	data_name_list.push_back("assign");
	data_name_list.push_back("associate");		// f03+ only
	data_name_list.push_back("common");
	data_name_list.push_back("complex");
	data_name_list.push_back("character");
	data_name_list.push_back("contains");
	data_name_list.push_back("data");
	data_name_list.push_back("deallocate");
	data_name_list.push_back("dimension");
	data_name_list.push_back("double precision");
	data_name_list.push_back("enum");			// f03+ only
	data_name_list.push_back("equivalence");
	data_name_list.push_back("external");
	data_name_list.push_back("final");			// f03+ only
	data_name_list.push_back("function");
	data_name_list.push_back("generic");		// f03+ only
	data_name_list.push_back("implicit");
	data_name_list.push_back("import");			// f03+ only
	data_name_list.push_back("integer");
	data_name_list.push_back("interface");
	data_name_list.push_back("intrinsic");
	data_name_list.push_back("logical");
	data_name_list.push_back("module");
	data_name_list.push_back("namelist");
	data_name_list.push_back("nullify");
	data_name_list.push_back("optional");
	data_name_list.push_back("parameter");
	data_name_list.push_back("program");
	data_name_list.push_back("real");
	data_name_list.push_back("reallocate");
	data_name_list.push_back("recursive");
	data_name_list.push_back("save");
	data_name_list.push_back("select type");	// f03+ only
	data_name_list.push_back("subroutine");
	data_name_list.push_back("type");
	data_name_list.push_back("use");

	exec_name_list.push_back("backspace");
	exec_name_list.push_back("call");
	exec_name_list.push_back("close");
	exec_name_list.push_back("cycle");
	exec_name_list.push_back("do");
	exec_name_list.push_back("elseif");
	exec_name_list.push_back("entry");
	exec_name_list.push_back("exit");
	exec_name_list.push_back("forall");			// f95+ only
	exec_name_list.push_back("format");
	exec_name_list.push_back("goto");
	exec_name_list.push_back("if");
	exec_name_list.push_back("inquire");
	exec_name_list.push_back("open");
	exec_name_list.push_back("pause");
	exec_name_list.push_back("print");
	exec_name_list.push_back("read");
	exec_name_list.push_back("return");
	exec_name_list.push_back("rewind");
	exec_name_list.push_back("select case");
	exec_name_list.push_back("stop");
	exec_name_list.push_back("where");
	exec_name_list.push_back("write");

	math_func_list.push_back("abs");
	math_func_list.push_back("ceiling");
	math_func_list.push_back("dim");
	math_func_list.push_back("dot_product");
	math_func_list.push_back("dprod");
	math_func_list.push_back("exp");
	math_func_list.push_back("floor");
	math_func_list.push_back("matmul");
	math_func_list.push_back("max");
	math_func_list.push_back("min");
	math_func_list.push_back("mod");
	math_func_list.push_back("modulo");
	math_func_list.push_back("sign");
	math_func_list.push_back("sqrt");
	
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

	cmplx_calc_list.push_back("+");
	cmplx_calc_list.push_back("-");
	cmplx_calc_list.push_back("*");
	cmplx_calc_list.push_back("/");
	cmplx_calc_list.push_back("**");

	cmplx_cond_list.push_back("do");
	cmplx_cond_list.push_back("else");
	cmplx_cond_list.push_back("else if");
	cmplx_cond_list.push_back("elseif");
	cmplx_cond_list.push_back("forall");		// f95+ only
	cmplx_cond_list.push_back("if");
	cmplx_cond_list.push_back("select case");
	cmplx_cond_list.push_back("select type");	// f03+ only
	
	cmplx_logic_list.push_back(".and.");
	cmplx_logic_list.push_back(".or.");
	cmplx_logic_list.push_back(".not."); 
	cmplx_logic_list.push_back(".eqv.");
	cmplx_logic_list.push_back(".neqv.");
	cmplx_logic_list.push_back("==");
	cmplx_logic_list.push_back("/=");
	cmplx_logic_list.push_back(".eq.");
	cmplx_logic_list.push_back(".ne.");
	cmplx_logic_list.push_back(">");
	cmplx_logic_list.push_back("<");
	cmplx_logic_list.push_back(">=");
	cmplx_logic_list.push_back("=<");
	cmplx_logic_list.push_back(".gt.");
	cmplx_logic_list.push_back(".lt.");
	cmplx_logic_list.push_back(".ge.");
	cmplx_logic_list.push_back(".le.");
	cmplx_logic_list.push_back(".true.");
	cmplx_logic_list.push_back(".false.");

	cmplx_preproc_list.push_back("dictionary");
	cmplx_preproc_list.push_back("include");
	cmplx_preproc_list.push_back("options");
	
	cmplx_assign_list.push_back("=");

	cmplx_pointer_list.push_back("=>");

	cmplx_cyclomatic_list.push_back("if");
	cmplx_cyclomatic_list.push_back("do");
	//cmplx_cyclomatic_list.push_back("IIf");
	//cmplx_cyclomatic_list.push_back("for");
	//cmplx_cyclomatic_list.push_back("while");
	//cmplx_cyclomatic_list.push_back("Until");
	//cmplx_cyclomatic_list.push_back("Catch");
	//cmplx_cyclomatic_list.push_back("WHEN");
	//cmplx_cyclomatic_list.push_back("select");
	cmplx_cyclomatic_list.push_back("case");

	ignore_cmplx_cyclomatic_list.push_back("end do");
	ignore_cmplx_cyclomatic_list.push_back("end if");

	cmplx_cyclomatic_logic_list.push_back(".and.");
	cmplx_cyclomatic_logic_list.push_back(".or.");
	//cmplx_cyclomatic_logic_list.push_back(".eqv.");
	//cmplx_cyclomatic_logic_list.push_back(".neqv.");

	cmplx_cyclomatic_case_list.push_back("case");
	cmplx_cyclomatic_switch_list.push_back("end select");
	cmplx_cyclomatic_default_list.push_back("default");
	cmplx_cyclomatic_default_list.push_back("select case");
}

/*!
* Perform preprocessing of file lines before counting.
* Replace quote stuffing in literals '' or "" to avoid quote matching problems.
*
* \param fmap list of file lines
*
* \return method status
*/
int CFortranCounter::PreCountProcess(filemap* fmap)
{
	size_t i;
	filemap::iterator fit;
	for (fit = fmap->begin(); fit != fmap->end(); fit++) 
	{
		if (fit->line.empty())
			continue;
		for (i = fit->line.length() - 1; i > 0; i--)
		{
			if ((fit->line[i] == '\'' && fit->line[i-1] == '\'') || (fit->line[i] == '"' && fit->line[i-1] == '"'))
			{
				fit->line[i] = '$';
				fit->line[i-1] = '$';
			}
		}
	}
	return 0;
}

/*!
* Counts the number of comment lines, removes comments, and
* replaces quoted strings by special chars, e.g., $
* All arguments are modified by the method.
*
* \param fmap list of processed file lines
* \param result counter results
* \param fmapBak list of original file lines (same as fmap except it contains unmodified quoted strings)
*
* \return method status
*/
int CFortranCounter::CountCommentsSLOC(filemap* fmap, results* result, filemap *fmapBak)
{
	if (LineCommentStart.empty())
		return 0;
	if (classtype == UNKNOWN || classtype == DATAFILE)
		return 0;

	bool contd_nextline;
	int comment_type = 0;
	/*
	comment_type:
	0 : not comment
	1 : line comment, whole line
	2 : line comment, embedded
	*/

	size_t i, idx_start, comment_start;
	size_t quote_idx_start;
	string curBlckCmtStart, curBlckCmtEnd, prevLine;
	char CurrentQuoteEnd = 0;
	bool quote_contd = false, found, foundSpc;
	filemap::iterator itfmBak = fmapBak->begin();

	quote_idx_start = 0;
	prevLine = "";

	for (filemap::iterator iter = fmap->begin(); iter != fmap->end(); iter++, itfmBak++)
	{
		contd_nextline = false;

		quote_idx_start = 0;
		idx_start = 0;

		if (CUtil::CheckBlank(iter->line))
			continue;
		if (quote_contd)
		{
			// replace quote until next character
			ReplaceQuote(iter->line, quote_idx_start, quote_contd, CurrentQuoteEnd);
			prevLine = itfmBak->line;
			if (quote_contd)
				continue;
		}

		while (!contd_nextline && idx_start < iter->line.length())
		{
			quote_idx_start = FindQuote(iter->line, QuoteStart, quote_idx_start, QuoteEscapeFront);
			comment_start = idx_start;

			// check for comment delimiters 'C', 'c' in col 1 (works for most cases)
			found = false;
			if ((iter->line[0] == 'C' || iter->line[0] == 'c') &&
				(prevLine.length() < 1 || prevLine[prevLine.length() - 1] != '&'))
			{
				// check for reserved 'c' words
				for (vector<string>::iterator viter = c_keywords.begin(); viter != c_keywords.end(); viter++)
				{
					if (CUtil::FindKeyword(iter->line, *viter, 0, TO_END_OF_STRING, false) == 0)
					{
						found = true;
						break;
					}
				}
				if (!found)
				{
					// check for function or assignment (check for 'c__()' or 'c__ =')
					foundSpc = false;
					for (i = 1; i < iter->line.length(); i++)
					{
						if (iter->line[i] == '(')
						{
							found = true;
							break;
						}
						else if (iter->line[i] == '=')
						{
							if (i >= iter->line.length() - 1 || iter->line[i + 1] != '=')
								found = true;
							break;
						}
						else if (iter->line[i] == ' ')
							foundSpc = true;
						else if (foundSpc)
							break;
					}
				}
				found = !found;
			}

			// check for comment delimiters '*', '!' in col 1
			if (found || ((iter->line[0] == '*' || iter->line[0] == '!') &&
				(prevLine.length() < 1 || prevLine[prevLine.length() - 1] != '&')))
			{
				comment_start = 0;
				comment_type = 1;
			}
			// commented out to favor Fortran 90+ (in Fortran 77 any character in column 6 indicates continuation, not comment)
			// else if (iter->line.length() > 6 && iter->line[5] == '!' && CUtil::CheckBlank(iter->line.substr(0, 5)))
			//	comment_start = string::npos;
			else
			{
				FindCommentStart(iter->line, comment_start, comment_type, curBlckCmtStart, curBlckCmtEnd);
				if (comment_start != string::npos)
				{
					// check for characters before comment
					for (i = 0; i < comment_start; i++)
					{
						if (iter->line[i] != ' ')
						{
							comment_type = 2;
							break;
						}
					}
				}
			}

			if (comment_start == string::npos && quote_idx_start == string::npos)
			{
				prevLine = itfmBak->line;
				break;
			}

			if (comment_start != string::npos)
				idx_start = comment_start;

			// if found quote before comment
			if (quote_idx_start != string::npos && (comment_start == string::npos || quote_idx_start < comment_start))
			{
				ReplaceQuote(iter->line, quote_idx_start, quote_contd, CurrentQuoteEnd);
				if (quote_idx_start > idx_start)
				{
					if (quote_contd)
					{
						if (itfmBak->line[itfmBak->line.length() - 1] == '&')
						{
							iter->line[iter->line.length() - 1] = '&';
							if (itfmBak->line.length() > 2 && itfmBak->line[itfmBak->line.length() - 2] == ' ')
								iter->line[iter->line.length() - 2] = ' ';
						}
					}
					idx_start = quote_idx_start;
					prevLine = itfmBak->line;
					continue; // comment delimiter inside quote
				}
			}
			else if (idx_start != string::npos)
			{
				 // comment delimiter starts first
				switch(comment_type)
				{
				case 1:	// line comment, definitely whole line
				case 3:
					prevLine = "";
					iter->line = "";
					itfmBak->line = "";
					result->comment_lines++;
					contd_nextline = true;
					break;
				case 2:	// line comment, possibly embedded
				case 4:
					result->e_comm_lines++;
					prevLine = "";
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
				default:
					cout << "Error in CountCommentsSLOC()";
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
int CFortranCounter::CountDirectiveSLOC(filemap* fmap, results* result, filemap* fmapBak)
{
	bool contd = false, trunc_flag = false;
	size_t idx, strSize;
	unsigned int cnt = 0;
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$";
	string strDirLine = "", tmp, str;
	filemap::iterator itfmBak = fmapBak->begin();

	for (filemap::iterator iter = fmap->begin(); iter!=fmap->end(); iter++, itfmBak++)
	{
		tmp = CUtil::TrimString(iter->line);
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
					strDirLine = CUtil::TrimString(itfmBak->line.substr(0, strSize));
				if (strDirLine[strDirLine.length() - 1] == '&')
					strDirLine = CUtil::TrimString(strDirLine.substr(0, strDirLine.length() - 1));
				result->directive_lines[PHY]++;
			}
 		}
		else
		{
			// continuation of a previous directive
			strSize = CUtil::TruncateLine(itfmBak->line.length(), strDirLine.length(), this->lsloc_truncate, trunc_flag);
			if (strSize > 0)
			{
				str = CUtil::TrimString(itfmBak->line.substr(0, strSize));
				if (str[0] == '&')
					strDirLine += CUtil::TrimString(str.substr(1, str.length() - 1));
				else
					strDirLine += str;
			}
			result->directive_lines[PHY]++;
		}
	
		if (contd)
		{
			// if a directive or continuation of a directive (no continuation symbol found)
			if (iter->line[iter->line.length() - 1] != '&')
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
int CFortranCounter::LanguageSpecificProcess(filemap* fmap, results* result, filemap* fmapBak)
{
	filemap::iterator fit, fitbak, fitNext = fmap->begin();
	string line, lineBak, lineNext;

	bool data_continue = false, fixed_continue = false;
	string strLSLOC = "";
	string strLSLOCBak = "";
	string str;
	unsigned int phys_exec_lines = 0;
	unsigned int phys_data_lines = 0;
	unsigned int temp_lines = 0;
	unsigned int cnt = 0;
	StringVector loopEnd;
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$";

	for (fit = fmap->begin(), fitbak = fmapBak->begin(); fit != fmap->end(); fit++, fitbak++)
	{
		if (fitNext != fmap->end())
			fitNext++;
		line = fit->line;
		lineBak = fitbak->line;

		// do not process blank lines (blank_line/comment_line/directive)
		if (!CUtil::CheckBlank(line))
		{
			// get next line to check for fixed format continuation (non-blank and non-0 in col 6)
			lineNext = "";
			fixed_continue = false;
			if (fitNext != fmap->end())
			{
				lineNext = fitNext->line;
				if (!CUtil::CheckBlank(lineNext))
				{
					if (lineNext.length() > 6)
					{
						str = lineNext.substr(0, 5);
						if ((CUtil::CheckBlank(str) || CUtil::IsInteger(str)) &&
							lineNext[5] != ' ' && lineNext[5] != '0')
						{
							// fixed format continuation
							fixed_continue = true;
						}
					}
				}
			}

			// process logical SLOC
			LSLOC(result, line, fit->lineNumber, lineBak, strLSLOC, strLSLOCBak, fixed_continue,
				data_continue, temp_lines, phys_exec_lines, phys_data_lines, loopEnd);

			if (print_cmplx)
			{
				cnt = 0;
				CUtil::CountTally(line, exec_name_list, cnt, 1, exclude, "", "", &result->exec_name_count, false);
			}

			// update physical SLOC lines
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
* \param fixed_continue fixed format line continues on next line
* \param data_continue continuation of a data declaration line
* \param temp_lines tracks physical line count
* \param phys_exec_lines number of physical executable lines
* \param phys_data_lines number of physical data lines
* \param loopEnd nested loop end string(s)
*/
void CFortranCounter::LSLOC(results* result, string line, size_t lineNumber, string lineBak, string &strLSLOC, string &strLSLOCBak,
					bool &fixed_continue, bool &data_continue, unsigned int &temp_lines, unsigned int &phys_exec_lines,
					unsigned int &phys_data_lines, StringVector &loopEnd)
{
	size_t start, end;
	size_t i, j, k, m, strSize;
	bool found_exclusion = false, trunc_flag = false, found;
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$";
	string str, spc;
	unsigned int cnt = 0;
	unsigned int paren_cnt;

	string tmp    = CUtil::TrimString(line);
	string tmpBak = CUtil::TrimString(lineBak);
	start = 0;

	// if continuation, prepend previous line for correct processing
	if (strLSLOC.length() > 0)
	{
		found = false;
		if (tmpBak[0] == '&' && tmpBak.length() > 1)
			found = true;
		else
		{
			if (line.length() > 6)
			{
				str = line.substr(0, 5);
				if ((CUtil::CheckBlank(str) || CUtil::IsInteger(str)) &&
					line[5] != ' ' && line[5] != '0')
					found = true;
			}
		}
		if (found)
		{
			tmp = tmp.substr(1, tmp.length() - 1);
			tmpBak = tmpBak.substr(1, tmpBak.length() - 1);
		}
		tmp = strLSLOC + tmp;
		tmpBak = strLSLOCBak + tmpBak;
		strLSLOC = strLSLOCBak = "";
	}

	// there may be more than 1 logical SLOC in this line
	while (start < tmp.length())
	{
		// check for semicolon to denote end of SLOC
		end = tmp.find(";", start);
		if (end == string::npos)
			end = tmp.length() - 1;
		else
		{
			// skip empty ";"
			str = CUtil::TrimString(tmp.substr(start, end - start + 1));
			if (str == ";")
			{
				start = end + 1;
				continue;
			}
		}

		// record nested loops
		if (print_cmplx)
		{
			bool new_loop = false;
			i = CUtil::FindKeyword(tmp, "end do", start, end, false);
			if (i != string::npos)
				i = string::npos;
			else
				i = CUtil::FindKeyword(tmp, "do", start, end, false);
			if (i != string::npos)
			{
				// check for label after do
				found = false;
				if (i + 2 < end && tmp[i+2] == ' ')
				{
					for (j = i + 3; j <= end; j++)
					{
						if (tmp[j] != ' ')
						{
							for (k = j; k <= end; k++)
							{
								if (tmp[k] == ' ' || tmp[k] == ',')
									break;
							}
							k--;
							str = CUtil::TrimString(tmp.substr(j, k - j + 1));
							if (CUtil::IsInteger(str))
							{
								loopEnd.push_back(str);
								found = true;
							}
							break;
						}
					}
				}
				if (!found)
					loopEnd.push_back("end do");
				new_loop = true;
			}
			else
			{
				i = CUtil::FindKeyword(tmp, "end forall", start, end, false);
				if (i != string::npos)
					i = string::npos;
				else
					i = CUtil::FindKeyword(tmp, "forall", start, end, false);
				if (i != string::npos)
				{
					loopEnd.push_back("end forall");
					new_loop = true;
				}
				else if (loopEnd.size() > 0)
				{
					str = loopEnd.back();
					if (CUtil::FindKeyword(tmp, str, start, end, false) != string::npos)
					{
						loopEnd.pop_back();
						if (CUtil::IsInteger(str))
						{
							// remove additional label terminators
							if (loopEnd.size() > 0)
							{
								for (m = loopEnd.size() - 1; m > 0; m--)
								{
									if (loopEnd[m] == str)
										loopEnd.pop_back();
									else
										break;
								}
								if (m == 0)
								{
									if (loopEnd[0] == str)
										loopEnd.pop_back();
								}
							}
						}
					}
					else if (loopEnd.back() == "end do")
					{
						if (CUtil::FindKeyword(tmp, "enddo", start, end, false) != string::npos)
							loopEnd.pop_back();
					}
					else if (loopEnd.back() == "end forall")
					{
						if (CUtil::FindKeyword(tmp, "endforall", start, end, false) != string::npos)
							loopEnd.pop_back();
					}
				}
			}
			if (new_loop)
			{
				if (result->cmplx_nestloop_count.size() < loopEnd.size())
					result->cmplx_nestloop_count.push_back(1);
				else
					result->cmplx_nestloop_count[loopEnd.size()-1]++;
			}
		}

		// check for line containing excluded keywords (don't count as LSLOC)
		found_exclusion = false;
		for (StringVector::iterator it = exclude_keywords.begin(); it != exclude_keywords.end(); it++)
		{
			i = CUtil::FindKeyword(tmp, (*it), start, end, false);
			if (i != string::npos)
			{
				found_exclusion = true;

				// exceptions
				if ((*it) == "else")
				{
					// make sure not else if
					if (CUtil::FindKeyword(tmp, "else if", i, i + 6, false) == string::npos)
						break;
				}
				if ((*it) == "elsewhere")
				{
					// make sure elsewhere does not have condition
					str = CUtil::TrimString(tmp.substr(i + 9, end - (i + 9) + 1));
					if (str[0] != '(')
						break;
				}
				else
					break;
			}
		}
		if (found_exclusion)
		{
			strLSLOC = strLSLOCBak = "";
			phys_exec_lines++;
			temp_lines = 0;
			return;
		}

		// check for inline if, where, forall
		found = false;
		i = CUtil::FindKeyword(tmp, "if", start, end, false);
		if (i != string::npos)
		{
			if (CUtil::FindKeyword(tmp, "then", start, end, false) == string::npos)
				found = true;
		}
		if (!found)
		{
			i = CUtil::FindKeyword(tmp, "where", start, end, false);
			if (i != string::npos)
				found = true;
		}
		if (!found)
		{
			i = CUtil::FindKeyword(tmp, "forall", start, end, false);
			if (i != string::npos)
				found = true;
		}
		if (found)
		{
			// check if in-line action exists after if statement (past ())
			found = false;
			paren_cnt = 0;
			for (j = i + 2; j <= end; j++)
			{
				if (tmp[j] == '(')
				{
					found = true;
					paren_cnt++;
				}
				else if (tmp[j] == ')' && paren_cnt > 0)
					paren_cnt--;
				if (found && paren_cnt == 0)
				{
					if (j < end)
					{
						str = CUtil::TrimString(tmp.substr(j + 1, end - j + 1));
						if (!CUtil::CheckBlank(str) && str != ";" && str != "&" && !fixed_continue)
						{
							// save LSLOC for if statement, then process in-line action
							strSize = CUtil::TruncateLine(j - start + 1, strLSLOC.length(), this->lsloc_truncate, trunc_flag);
							if (strSize > 0)
							{
								strLSLOC += CUtil::TrimString(tmp.substr(start, strSize));
								strLSLOCBak += CUtil::TrimString(tmpBak.substr(start, strSize));
							}
							if (result->addSLOC(strLSLOCBak, lineNumber, trunc_flag))
								result->exec_lines[LOG]++;
							strLSLOC = strLSLOCBak = "";
							start = j + 1;
						}
					}
					break;
				}
			}
		}

		// check for fixed continuation or free continuation (&)
		if (tmp[end] == '&' || (fixed_continue && end >= tmp.length() - 1))
		{
			// strip off trailing (&)
			if (tmp[end] == '&')
				strSize = CUtil::TruncateLine(end - start, strLSLOC.length(), this->lsloc_truncate, trunc_flag);
			else
				strSize = CUtil::TruncateLine(end - start + 1, strLSLOC.length(), this->lsloc_truncate, trunc_flag);
			if (strSize > 0)
			{
				spc = "";
				str = tmp.substr(start, strSize);
				for (m = str.length() - 1; m > 0; m--)
				{
					if (str[m] == ' ')
						spc += " ";
					else
						break;
				}
				if (m == 0)
				{
					if (str[0] == ' ')
						spc += " ";
				}
				strLSLOC += CUtil::TrimString(tmp.substr(start, strSize)) + spc;
				strLSLOCBak += CUtil::TrimString(tmpBak.substr(start, strSize)) + spc;
			}
			start = end + 1;

			// make sure that we are not beginning to process a new data line
			cnt = 0;
			CUtil::CountTally(strLSLOC, data_name_list, cnt, 1, exclude, "", "", NULL, false);

			if (cnt > 0)
				data_continue = true;
			if (data_continue == true)
				temp_lines++;
			if (temp_lines == 0 && phys_data_lines == 0 && phys_exec_lines == 0)
				phys_exec_lines = 1;
		}
		else
		{
			// save LSLOC
			if (tmp[end] == ';')
				strSize = CUtil::TruncateLine(end - start, strLSLOC.length(), this->lsloc_truncate, trunc_flag);
			else
				strSize = CUtil::TruncateLine(end - start + 1, strLSLOC.length(), this->lsloc_truncate, trunc_flag);
			if (strSize > 0)
			{
				strLSLOC += CUtil::TrimString(tmp.substr(start, strSize));
				strLSLOCBak += CUtil::TrimString(tmpBak.substr(start, strSize));
			}
			start = end + 1;
			if (strLSLOCBak.length() > 0)
			{
				if (result->addSLOC(strLSLOCBak, lineNumber, trunc_flag))
				{
					// add a logical SLOC
					cnt = 0;
					CUtil::CountTally(strLSLOC, data_name_list, cnt, 1, exclude, "", "", &result->data_name_count, false);

					temp_lines++;
					if (data_continue == true || cnt > 0)
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
				else if (data_continue == true)
					phys_data_lines = temp_lines;
				else
					phys_exec_lines = temp_lines;
			}
			data_continue = false;
			temp_lines = 0;
			strLSLOC = strLSLOCBak = "";
		}
	}
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
int CFortranCounter::ParseFunctionName(const string &line, string &/*lastline*/,
		filemap &functionStack, string &functionName, unsigned int &functionCount)
{
        string str;
	size_t idx;
	unsigned int fcnt;

	idx = CUtil::FindKeyword(line, "PROGRAM");
	if (idx != string::npos)
	{
		if (idx + 8 < line.length())
		{
			str = line.substr(idx + 8);
			lineElement element(++functionCount, str);
			functionStack.push_back(element);
		}
	}
	else
	{
		idx = CUtil::FindKeyword(line, "SUBROUTINE");
		if (idx != string::npos)
		{
			if (idx + 11 < line.length())
			{
				str = line.substr(idx + 11);
				lineElement element(++functionCount, str);
				functionStack.push_back(element);
			}
		}
	}

	if (functionStack.empty())
	{
		// dealing with some code out of any subroutines, it a "main" code
		return 2;
	}

	string temp = CUtil::ClearRedundantSpaces(line);
	idx = CUtil::FindKeyword(temp, "END");   
	if (idx != string::npos && temp == "END")
	{
		str = functionStack.back().line;
		fcnt = functionStack.back().lineNumber;
		functionStack.pop_back();
		idx = str.find("(");
		if (idx != string::npos)
		{
			functionName = CUtil::ClearRedundantSpaces(str.substr(0, idx));
			functionCount = fcnt;
			return 1;
		}
		else {
			functionName = CUtil::ClearRedundantSpaces(str);
			functionCount = fcnt;
			return 1;
		}
	}	
	return 0;


}

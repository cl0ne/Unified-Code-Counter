//! Code counter class methods for the Midas macro languages.
/*!
* \file CMidasCounter.cpp
*
* This file contains the code counter class methods for the Midas macro languages.
*/

#include "CMidasCounter.h"

/*!
* Constructs a CMidasCounter object.
*/
CMidasCounter::CMidasCounter()
{
	casesensitive = false;

	QuoteStart = "\"";
	QuoteEnd = "\"";
	QuoteEscapeFront = '\"';
	ContinueLine = "&";

	LineCommentStart.push_back("!");

	exclude_keywords.push_back("endcontrols");
	exclude_keywords.push_back("endif");
	exclude_keywords.push_back("endloop");
	exclude_keywords.push_back("endl");		// abbreviation for endloop
	exclude_keywords.push_back("endmacro");
	exclude_keywords.push_back("endsubroutine");
	exclude_keywords.push_back("endwhile");
	exclude_keywords.push_back("endw");		// abbreviation for endwhile
	exclude_keywords.push_back("else");
	exclude_keywords.push_back("label");

	directive.push_back("include");

	exec_name_list.push_back("break");
	exec_name_list.push_back("call");
	exec_name_list.push_back("continue");
	exec_name_list.push_back("else");
	exec_name_list.push_back("elseif");
	exec_name_list.push_back("forall");
	exec_name_list.push_back("goto");
	exec_name_list.push_back("if");
	exec_name_list.push_back("loop");
	exec_name_list.push_back("pipe");
	exec_name_list.push_back("procedure");
	exec_name_list.push_back("return");
	exec_name_list.push_back("subroutine");
	exec_name_list.push_back("trap");
	exec_name_list.push_back("while");

	math_func_list.push_back("calc");
	math_func_list.push_back("fcalc");
	math_func_list.push_back("fft");
	math_func_list.push_back("firwind");
	math_func_list.push_back("histogram");
	math_func_list.push_back("maxmin");
	math_func_list.push_back("peakpick");
	math_func_list.push_back("ramp");

	trig_func_list.push_back("waveform");

	cmplx_calc_list.push_back("**");
	cmplx_calc_list.push_back("+");
	cmplx_calc_list.push_back("-");
	cmplx_calc_list.push_back("*");
	cmplx_calc_list.push_back("/");

	cmplx_cond_list.push_back("else");
	cmplx_cond_list.push_back("elseif");
	cmplx_cond_list.push_back("forall");
	cmplx_cond_list.push_back("if");
	cmplx_cond_list.push_back("loop");
	cmplx_cond_list.push_back("trap");
	cmplx_cond_list.push_back("while");
	
	cmplx_logic_list.push_back("and");
	cmplx_logic_list.push_back("or");
	cmplx_logic_list.push_back("gt");
	cmplx_logic_list.push_back("lt"); 
	cmplx_logic_list.push_back("ge");
	cmplx_logic_list.push_back("le");
	cmplx_logic_list.push_back("eq");
	cmplx_logic_list.push_back("eqs");
	cmplx_logic_list.push_back("eqss");
	cmplx_logic_list.push_back("ngt");
	cmplx_logic_list.push_back("nlt"); 
	cmplx_logic_list.push_back("nge");
	cmplx_logic_list.push_back("nle");
	cmplx_logic_list.push_back("neq");
	cmplx_logic_list.push_back("neqs");
	cmplx_logic_list.push_back("neqss");

	cmplx_preproc_list.push_back("include");

	cmplx_assign_list.push_back("results");

    //Modification 2016.10; USC
    cmplx_cyclomatic_list.push_back("if");
    cmplx_cyclomatic_list.push_back("elseif");
    cmplx_cyclomatic_list.push_back("forall");
    cmplx_cyclomatic_list.push_back("loop");
    cmplx_cyclomatic_list.push_back("trap");
    cmplx_cyclomatic_list.push_back("while");

    cmplx_cyclomatic_logic_list.push_back("and");
    cmplx_cyclomatic_logic_list.push_back("or");

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
int CMidasCounter::CountDirectiveSLOC(filemap* fmap, results* result, filemap* fmapBak)
{
	bool contd = false, trunc_flag = false;
	size_t idx, strSize;
	unsigned int cnt = 0;
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$";
	string strDirLine = "", tmp, tmpBak;
	filemap::iterator itfmBak = fmapBak->begin();

	for (filemap::iterator iter = fmap->begin(); iter != fmap->end(); iter++, itfmBak++)
	{
		if (CUtil::CheckBlank(iter->line))
			continue;
		size_t lineNumber = iter->lineNumber;

		tmp    = CUtil::TrimString(iter->line);
		tmpBak = CUtil::TrimString(itfmBak->line);

		if (print_cmplx)
		{
			cnt = 0;
			CUtil::CountTally(" " + tmp, directive, cnt, 1, exclude, "", "", &result->directive_count, false);
		}

		if (!contd)
		{
			// if not a continuation of a previous directive
			for (vector<string>::iterator viter = directive.begin(); viter != directive.end(); viter++)
			{
				if (((idx = CUtil::FindKeyword(tmp, *viter, 0, TO_END_OF_STRING, false)) != string::npos) && idx == 0)
				{
					contd = true;
					break;
   		        }
        	}
			if (contd)
			{
				// strip off trailing (&)
				if (tmpBak[tmpBak.length()-1] == '&')
					tmpBak = tmpBak.substr(0, tmpBak.length() - 1);
				strSize = CUtil::TruncateLine(tmpBak.length(), 0, this->lsloc_truncate, trunc_flag);
				if (strSize > 0)
					strDirLine = tmpBak.substr(0, strSize);
				result->directive_lines[PHY]++;
			}
 		}
		else
		{
			// continuation of a previous directive
			// strip off trailing (&)
			if (tmpBak[tmpBak.length()-1] == '&')
				tmpBak = tmpBak.substr(0, tmpBak.length() - 1);
			strSize = CUtil::TruncateLine(tmpBak.length(), strDirLine.length(), this->lsloc_truncate, trunc_flag);
			if (strSize > 0)
				strDirLine += tmpBak.substr(0, strSize);
			result->directive_lines[PHY]++;
		}
	
		if (contd)
		{
			// if a directive or continuation of a directive
			if (tmp[tmp.length()-1] != '&')
			{
				// add another logical directive line, should also have type
				// if no continuation symbol found
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
int CMidasCounter::LanguageSpecificProcess(filemap* fmap, results* result, filemap* fmapBak)
{
	filemap::iterator fit, fitbak;
	string line, lineBak;

	bool data_continue = false;
	string strLSLOC = "";
	string strLSLOCBak = "";
	unsigned int phys_exec_lines = 0;
	unsigned int phys_data_lines = 0;
	unsigned int temp_lines = 0;
	unsigned int cnt = 0;
	StringVector loopEnd;
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$";

	for (fit = fmap->begin(), fitbak = fmapBak->begin(); fit != fmap->end(); fit++, fitbak++)
	{
		line = fit->line;

		// insert blank at the beginning (for searching keywords)
		line = ' ' + line;
		lineBak = ' ' + fitbak->line;
	
		// do not process blank lines (blank_line/comment_line/directive)
		if (!CUtil::CheckBlank(line))
		{
			// process logical SLOC
			LSLOC(result, line, fit->lineNumber, lineBak, strLSLOC, strLSLOCBak,
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
* \param data_continue continuation of a data declaration line
* \param temp_lines tracks physical line count
* \param phys_exec_lines number of physical executable lines
* \param phys_data_lines number of physical data lines
* \param loopEnd nested loop end string(s)
*/
void CMidasCounter::LSLOC(results* result, string line, size_t lineNumber, string lineBak, string &strLSLOC, string &strLSLOCBak,
						  bool &data_continue, unsigned int &temp_lines, unsigned int &phys_exec_lines,
						  unsigned int &phys_data_lines, StringVector &loopEnd)
{
	size_t start = 0; //starting index of the working string
	size_t i = 0, strSize;
	bool found_exclusion = false, trunc_flag = false;
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$:";
	unsigned int cnt = 0;

	string tmp    = CUtil::TrimString(line);
	string tmpBak = CUtil::TrimString(lineBak);

	// record nested loops
	if (print_cmplx)
	{
		bool new_loop = false;
		if (CUtil::FindKeyword(tmp, "do", 0, TO_END_OF_STRING, false) == 0)
		{
			loopEnd.push_back("enddo");
			new_loop = true;
		}
		else if (CUtil::FindKeyword(tmp, "forall", 0, TO_END_OF_STRING, false) == 0 ||
			CUtil::FindKeyword(tmp, "foreach", 0, TO_END_OF_STRING, false) == 0 ||
			CUtil::FindKeyword(tmp, "for", 0, TO_END_OF_STRING, false) == 0)
		{
			loopEnd.push_back("endfor");
			new_loop = true;
		}
		else if (CUtil::FindKeyword(tmp, "loop", 0, TO_END_OF_STRING, false) == 0)
		{
			loopEnd.push_back("endloop");
			new_loop = true;
		}
		else if (CUtil::FindKeyword(tmp, "while", 0, TO_END_OF_STRING, false) == 0)
		{
			loopEnd.push_back("endwhile");
			new_loop = true;
		}
		else if (loopEnd.size() > 0)
		{
			if (CUtil::FindKeyword(tmp, loopEnd.back(), 0, TO_END_OF_STRING, false) == 0)
				loopEnd.pop_back();
		}
		if (new_loop)
		{
			if (result->cmplx_nestloop_count.size() < loopEnd.size())
				result->cmplx_nestloop_count.push_back(1);
			else
				result->cmplx_nestloop_count[loopEnd.size()-1]++;

			// drop end keyword on forall since it is a single line loop
			if (CUtil::FindKeyword(tmp, "forall", 0, TO_END_OF_STRING, false) == 0)
				loopEnd.pop_back();
		}
	}

	// check for line starting with excluded keywords (don't count as LSLOC)
	for (StringVector::iterator it = exclude_keywords.begin(); it != exclude_keywords.end(); it++)
	{
		found_exclusion = (CUtil::FindKeyword(tmp, (*it), 0, TO_END_OF_STRING, false) == 0);
		if (found_exclusion)
		{
			// process else
			if ((*it) == "else")
			{
				if (CUtil::FindKeyword(tmp, "elseif", 0, TO_END_OF_STRING, false) != 0)
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

	// check for inline if
	if (CUtil::FindKeyword(tmp, "if", 0, TO_END_OF_STRING, false) == 0)
	{
		i = CUtil::FindKeyword(tmp, "then", 0, TO_END_OF_STRING, false);
		if (i != string::npos)
		{
			// check if anything exists past the "then"
			start = i + 4;
			if (start < tmp.length())
			{
				// save LSLOC for if statement, then process in-line action
				strSize = CUtil::TruncateLine(start, strLSLOC.length(), this->lsloc_truncate, trunc_flag);
				if (strSize > 0)
				{
					strLSLOC += tmp.substr(0, strSize);
					strLSLOCBak += tmpBak.substr(0, strSize);
					tmp = CUtil::TrimString(tmp.substr(start, tmp.length() - start));
					tmpBak = CUtil::TrimString(tmpBak.substr(start, tmpBak.length() - start));
				}
				if (result->addSLOC(strLSLOCBak, lineNumber, trunc_flag))
					result->exec_lines[LOG]++;
				strLSLOC = strLSLOCBak = "";
				phys_exec_lines++;
				temp_lines = 0;
			}
		}
	}
	// check for "forall" or "for"
	else if (CUtil::FindKeyword(tmp, "forall", 0, TO_END_OF_STRING, false) == 0 ||
		CUtil::FindKeyword(tmp, "for", 0, TO_END_OF_STRING, false) == 0)
	{
		// split after forall/for
		if (CUtil::FindKeyword(tmp, "forall", 0, TO_END_OF_STRING, false) == 0)
			start = 6;
		else
			start = 3;
		if (tmp.length() > start)
		{
			// save LSLOC for if statement, then process loop action
			strSize = CUtil::TruncateLine(start, strLSLOC.length(), this->lsloc_truncate, trunc_flag);
			if (strSize > 0)
			{
				strLSLOC += tmp.substr(0, strSize);
				strLSLOCBak += tmpBak.substr(0, strSize);
				tmp = CUtil::TrimString(tmp.substr(start, tmp.length() - start));
				tmpBak = CUtil::TrimString(tmpBak.substr(start, tmpBak.length() - start));
			}
			if (result->addSLOC(strLSLOCBak, lineNumber, trunc_flag))
				result->exec_lines[LOG]++;
			strLSLOC = strLSLOCBak = "";
			phys_exec_lines++;
			temp_lines = 0;
		}
	}

	// check for continuation (&); exception is (&&) to use literal (&)
	if (tmpBak.substr(tmpBak.length() - 1, 1) == "&" &&
		(tmpBak.length() < 2 || tmpBak.substr(tmpBak.length() - 2, 1) != "&"))
	{
		// strip off trailing (&)
		if (tmp.length() > 1)
		{
			tmp = tmp.substr(start, tmp.length() - 1);
			tmpBak = tmpBak.substr(start, tmpBak.length() - 1);
			strSize = CUtil::TruncateLine(tmp.length(), strLSLOC.length(), this->lsloc_truncate, trunc_flag);
			if (strSize > 0)
			{
				strLSLOC += tmp.substr(0, strSize);
				strLSLOCBak += tmpBak.substr(0, strSize);
			}
		}
		else
			tmp = tmpBak = "";

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
	else
	{
		// save LSLOC
		strSize = CUtil::TruncateLine(tmp.length(), strLSLOC.length(), this->lsloc_truncate, trunc_flag);
		if (strSize > 0)
		{
			strLSLOC += tmp.substr(0, strSize);
			strLSLOCBak += tmpBak.substr(0, strSize);
		}
		if (result->addSLOC(strLSLOCBak, lineNumber, trunc_flag))
		{
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
		data_continue = false;
		temp_lines = 0;
		strLSLOC = strLSLOCBak = "";
	}
}

/**
 * Counts file language complexity based on specified language keywords/characters.
 *
 * @param fmap list of processed file lines
 * @param result counter results
 *
 * @return method status
 *
 * Creation Time and Owner : 2016.10; USC
 */
int CMidasCounter::CountComplexity(filemap* fmap, results* result)
{
    if (classtype == UNKNOWN || classtype == DATAFILE)
        return 0;
    filemap::iterator fit;
    size_t idx;
    unsigned int cnt, proc_cnt=0, cyclomatic_cnt = 0, ignore_cyclomatic_cnt = 0, cyclomatic_logic_cnt = 0, procedure_count = 0;
    string line, file_ext, procedure_name ="";
    string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$";

    map<unsigned int, lineElement> procedure_map;
    map<unsigned int, lineElement> logical_map;

    filemap procedureStack;

    bool process_cyclomatic_complexity = false;

    // check whether to process cyclomatic complexity
    if (cmplx_cyclomatic_list.size() > 0)
    {
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
    for (fit = fmap->begin(); fit != fmap->end(); fit++)
    {
        line = fit->line;

        if (CUtil::CheckBlank(line))
            continue;

        line = " " + line;

        // mathematical functions
        cnt = 0;
        CUtil::CountTally(line, math_func_list, cnt, 1, exclude, "", "", &result->math_func_count, casesensitive);
        result->cmplx_math_lines += cnt;

        // trigonometric functions
        cnt = 0;
        CUtil::CountTally(line, trig_func_list, cnt, 1, exclude, "", "", &result->trig_func_count, casesensitive);
        result->cmplx_trig_lines += cnt;

        // logarithmic functions
        cnt = 0;
        CUtil::CountTally(line, log_func_list, cnt, 1, exclude, "", "", &result->log_func_count, casesensitive);
        result->cmplx_logarithm_lines += cnt;

        // calculations
        cnt = 0;
        CUtil::CountTally(line, cmplx_calc_list, cnt, 1, exclude, "", "", &result->cmplx_calc_count, casesensitive);
        result->cmplx_calc_lines += cnt;

        // conditionals
        cnt = 0;
        CUtil::CountTally(line, cmplx_cond_list, cnt, 1, exclude, "", "", &result->cmplx_cond_count, casesensitive);
        result->cmplx_cond_lines += cnt;

        // logical operators
        cnt = 0;
        CUtil::CountTally(line, cmplx_logic_list, cnt, 1, exclude, "", "", &result->cmplx_logic_count, casesensitive);
        result->cmplx_logic_lines += cnt;

        // preprocessor directives
        cnt = 0;
        CUtil::CountTally(line, cmplx_preproc_list, cnt, 1, exclude, "", "", &result->cmplx_preproc_count,
                          casesensitive);
        result->cmplx_preproc_lines += cnt;

        // assignments
        cnt = 0;
        CUtil::CountTally(line, cmplx_assign_list, cnt, 1, exclude, "", "", &result->cmplx_assign_count, casesensitive);
        result->cmplx_assign_lines += cnt;

        /*
        // pointers
        cnt = 0;
        // Pointers are embedded syntax so there is NO exclude string or include strings
        CUtil::CountTally(line, cmplx_pointer_list, cnt, 1, "", "", "", &result->cmplx_pointer_count, casesensitive);
        result->cmplx_pointer_lines += cnt;
         */

        if(process_cyclomatic_complexity) {
            unsigned int temp = 0;
            CUtil::CountTally(line, cmplx_cyclomatic_list, temp, 1, exclude, "", "", 0, casesensitive);
            cyclomatic_cnt += temp;

            if (ignore_cmplx_cyclomatic_list.size() > 0)
                CUtil::CountTally(line, ignore_cmplx_cyclomatic_list, ignore_cyclomatic_cnt, 1, exclude, "", "", 0,
                                  casesensitive);

            // search for cyclomatic complexity logical keywords
            if (cmplx_cyclomatic_logic_list.size() > 0)
                CUtil::CountTally(line, cmplx_cyclomatic_logic_list, cyclomatic_logic_cnt, 1, exclude, "", "", 0,
                                  casesensitive);

            //search for startmacro
            size_t idx_macro = CUtil::FindKeyword(line, "startmacro");
            if (idx_macro != string::npos) {
                lineElement element(++proc_cnt, "startmacro");
                procedureStack.push_back(element);
            }
        }
    }

    while(!procedureStack.empty())
    {
        procedure_name = procedureStack.back().line;
        procedure_count = procedureStack.back().lineNumber;
        procedureStack.pop_back();

        if(procedureStack.empty())
        {
            lineElement element(cyclomatic_cnt - ignore_cyclomatic_cnt + 1, procedure_name);
            lineElement n_element(cyclomatic_cnt + cyclomatic_logic_cnt + 1, procedure_name);
            procedure_map[procedure_count] = element;
            logical_map[procedure_count] = n_element;
        }

    }

    for (map<unsigned int, lineElement>::iterator it = procedure_map.begin(); it != procedure_map.end(); ++it)
        result->cmplx_cycfunct_count.push_back(it->second);
    if(cmplx_cyclomatic_logic_list.size() > 0)
    {
        for (map<unsigned int, lineElement>::iterator it = logical_map.begin(); it != logical_map.end(); ++it)
            result->cmplx_cycfunct_CC2_count.push_back(it->second);
    }
    return 1;

}

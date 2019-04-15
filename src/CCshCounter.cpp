//! Code counter class methods for the C shell script language.
/*!
* \file CCshCounter.cpp
*
* This file contains the code counter class methods for the C shell script language.
* This also includes the Tcsh language.*/

#include "CCshCounter.h"

/*!
* Constructs a CCshCounter object.
*/
CCshCounter::CCshCounter()
{
	classtype = CSH;
	language_name = "C-Shell";
    
    //Modification: 11.2016 Ext-4
    file_extension = CUtil::getExtensionsToLanguage("C-Shell", file_extension);
    /*
	file_extension.push_back(".csh");
	file_extension.push_back(".tcsh")*/

	QuoteStart = "\"'";
	QuoteEnd = "\"'";
	QuoteEscapeFront = '\\';
	ContinueLine = "\\";
	LineCommentStart.push_back("#");

	exclude_keywords.push_back("end");
	exclude_keywords.push_back("endif");
	exclude_keywords.push_back("endsw");

	continue_keywords.push_back("case");
	continue_keywords.push_back("default");
	continue_keywords.push_back("else");

	exec_name_list.push_back("alias");
	exec_name_list.push_back("break");
	exec_name_list.push_back("breaksw");
	exec_name_list.push_back("builtins");
	exec_name_list.push_back("case");
	exec_name_list.push_back("cd");
	exec_name_list.push_back("chdir");
	exec_name_list.push_back("continue");
	exec_name_list.push_back("dirs");
	exec_name_list.push_back("echo");
	exec_name_list.push_back("eval");
	exec_name_list.push_back("exec");
	exec_name_list.push_back("exit");
	exec_name_list.push_back("foreach");
	exec_name_list.push_back("glob");
	exec_name_list.push_back("goto");
	exec_name_list.push_back("if");
	exec_name_list.push_back("onintr");
	exec_name_list.push_back("popd");
	exec_name_list.push_back("pushd");
	exec_name_list.push_back("rehash");
	exec_name_list.push_back("repeat");
	exec_name_list.push_back("set");
	exec_name_list.push_back("setenv");
	exec_name_list.push_back("shift");
	exec_name_list.push_back("source");
	exec_name_list.push_back("switch");
	exec_name_list.push_back("time");
	exec_name_list.push_back("umask");
	exec_name_list.push_back("unalias");
	exec_name_list.push_back("unhash");
	exec_name_list.push_back("unset");
	exec_name_list.push_back("unsetenv");
	exec_name_list.push_back("while");

	cmplx_calc_list.push_back("+");	
	cmplx_calc_list.push_back("-");
	cmplx_calc_list.push_back("*");
	cmplx_calc_list.push_back("/");
	cmplx_calc_list.push_back("%");
	cmplx_calc_list.push_back("++");
	cmplx_calc_list.push_back("--");

	cmplx_cond_list.push_back("case");
	cmplx_cond_list.push_back("else");
	cmplx_cond_list.push_back("foreach");
	cmplx_cond_list.push_back("if");
	cmplx_cond_list.push_back("switch");
	cmplx_cond_list.push_back("while");

	cmplx_logic_list.push_back("&&");
	cmplx_logic_list.push_back("||");
	cmplx_logic_list.push_back("==");
	cmplx_logic_list.push_back("!=");
	cmplx_logic_list.push_back("=~");
	cmplx_logic_list.push_back("!~");
	cmplx_logic_list.push_back(">");
	cmplx_logic_list.push_back("<");
	cmplx_logic_list.push_back(">=");
	cmplx_logic_list.push_back("=<");

	cmplx_assign_list.push_back("=");

	cmplx_cyclomatic_list.push_back("if");
       	cmplx_cyclomatic_list.push_back("for");
       	cmplx_cyclomatic_list.push_back("case");
       	cmplx_cyclomatic_list.push_back("elif");
       	cmplx_cyclomatic_list.push_back("while");
       	//cmplx_cyclomatic_list.push_back("switch");
       	cmplx_cyclomatic_list.push_back("foreach");
       	cmplx_cyclomatic_list.push_back("?");

	cmplx_cyclomatic_logic_list.push_back("||");
	cmplx_cyclomatic_logic_list.push_back("&&");

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
int CCshCounter::PreCountProcess(filemap* fmap)
{
	filemap::iterator fit;
	for (fit = fmap->begin(); fit != fmap->end(); fit++) 
	{
		if (fit->line.empty())
			continue;
		for (size_t i = fit->line.length() - 1; i > 0; i--)
		{
			// replace $# and ${# with $ to avoid determination of a comment
			if (fit->line[i] == '#' && (fit->line[i-1] == '$' || fit->line[i-1] == '{'))
				fit->line[i] = '$';
		}
	}
	return 0;
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
int CCshCounter::LanguageSpecificProcess(filemap* fmap, results* result, filemap* fmapBak)
{
	filemap::iterator fit, fitbak;
	string line, lineBak;

	bool data_continue = false;
	string strLSLOC = "";
	string strLSLOCBak = "";
	string str;
	unsigned int phys_exec_lines = 0;
	unsigned int phys_data_lines = 0;
	unsigned int temp_lines = 0;
	unsigned int cnt = 0;
	unsigned int loopLevel = 0;
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$";

	for (fit = fmap->begin(), fitbak = fmapBak->begin(); fit != fmap->end(); fit++, fitbak++)
	{
		line = fit->line;
		lineBak = fitbak->line;

		// do not process blank lines (blank_line/comment_line/directive)
		if (!CUtil::CheckBlank(line))
		{
			// process logical SLOC
			LSLOC(result, line, fit->lineNumber, lineBak, strLSLOC, strLSLOCBak, data_continue,
				temp_lines, phys_exec_lines, phys_data_lines, loopLevel);

			if (print_cmplx)
			{
				cnt = 0;
				CUtil::CountTally(line, exec_name_list, cnt, 1, exclude, "", "", &result->exec_name_count);
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
* \param loopLevel nested loop level
*/
void CCshCounter::LSLOC(results* result, string line, size_t lineNumber, string lineBak, string &strLSLOC, string &strLSLOCBak,
						bool &data_continue, unsigned int &temp_lines, unsigned int &phys_exec_lines,
						unsigned int &phys_data_lines, unsigned int &loopLevel)
{
	size_t start, end;
	size_t i, j, m, strSize;
	bool trunc_flag = false, found;
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$";
	string str, spc;
	unsigned int cnt = 0;

	string tmp    = CUtil::TrimString(line);
	string tmpBak = CUtil::TrimString(lineBak);
	start = 0;

	// there may be more than 1 logical SLOC in this line
	while (start < tmp.length())
	{
		// check for semicolon to denote end of SLOC
		end = tmp.find(";", start);
		if (end != string::npos)
		{
			// handle empty statement
			if (CUtil::TrimString(tmp.substr(start, end - start + 1)) == ";")
			{
				start = end + 1;
				strLSLOC = strLSLOCBak = "";
				temp_lines = 0;
				if (tmp == ";")
					phys_exec_lines++;
				continue;
			}
		}
		else
			end = tmp.length() - 1;

		// check for label
		if (tmp[end] == ':' && tmp.substr(start, end - start + 1) != "default:")
		{
			i = tmp.find_first_of("\t ", start);
			if (i == string::npos || i > end)
			{
				// skip label
				start = end + 1;
				if (start >= tmp.length())
				{
					if (temp_lines == 0 && phys_data_lines == 0 && phys_exec_lines == 0)
						phys_exec_lines = 1;
					continue;
				}
			}
		}

		// process nested loops
		if (print_cmplx)
		{
			str = CUtil::TrimString(tmp.substr(start, end - start + 1));
			if (CUtil::FindKeyword(str, "foreach") != string::npos
				|| CUtil::FindKeyword(str, "while") != string::npos)
			{
				loopLevel++;

				// record nested loop level
				if ((unsigned int)result->cmplx_nestloop_count.size() < loopLevel)
					result->cmplx_nestloop_count.push_back(1);
				else
					result->cmplx_nestloop_count[loopLevel-1]++;
			}
			else if (CUtil::FindKeyword(str, "end") != string::npos && loopLevel > 0)
				loopLevel--;
		}

		// check for line containing excluded keywords
		for (StringVector::iterator it = exclude_keywords.begin(); it != exclude_keywords.end(); it++)
		{
			i = CUtil::FindKeyword(tmp, (*it), start, end);
			if (i != string::npos)
			{
				// strip specified keyword and skip if empty
				start = i + (*it).length();
				if (CUtil::CheckBlank(CUtil::TrimString(tmp.substr(start, end - start))))
					start = end + 1;
				break;
			}
		}
		if (start > end)
		{
			if (temp_lines == 0 && phys_data_lines == 0 && phys_exec_lines == 0)
				phys_exec_lines = 1;
			continue;
		}

		// check for continuation words
		found = false;
		if (tmp[end] == ';')
			str = CUtil::TrimString(tmp.substr(start, end - start));
		else
			str = CUtil::TrimString(tmp.substr(start, end - start + 1));
		for (StringVector::iterator it = continue_keywords.begin(); it != continue_keywords.end(); it++)
		{
			i = str.find((*it));
			if (i == 0)
			{
				// process else if
				if ((*it) != "else" || str.length() < 7 || str.substr(0, 7) != "else if")
				{
					found = true;
					strLSLOC += str + " ";
					if (tmp[end] == ';')
						str = CUtil::TrimString(tmpBak.substr(start, end - start));
					else
						str = CUtil::TrimString(tmpBak.substr(start, end - start + 1));
					strLSLOCBak += str + " ";
					start = end + 1;
					if (temp_lines == 0 && phys_data_lines == 0 && phys_exec_lines == 0)
						phys_exec_lines = 1;
					temp_lines = 0;
				}
			}
		}
		if (found)
			continue;

		// check for inline if
		if (CUtil::FindKeyword(tmp, "if", start, end) == start)
		{
			i = CUtil::FindKeyword(tmp, "then", start, end);
			if (i == string::npos)
			{
				// get end of if SLOC
				found = false;
				cnt = 0;
				for (j = start + 2; j <= end; j++)
				{
					if (tmp[j] == '(')
					{
						found = true;
						cnt++;
					}
					else if (tmp[j] == ')')
						cnt--;
					if (found && cnt < 1)
						break;
				}

				// save LSLOC for if statement, then process in-line action
				strSize = CUtil::TruncateLine(j - start + 1, strLSLOC.length(), this->lsloc_truncate, trunc_flag);
				if (strSize > 0)
				{
					strLSLOC += tmp.substr(start, strSize);
					strLSLOCBak += tmpBak.substr(start, strSize);
				}
				start = j + 1;
				if (result->addSLOC(strLSLOCBak, lineNumber, trunc_flag))
					result->exec_lines[LOG]++;
				strLSLOC = strLSLOCBak = "";
				phys_exec_lines++;
				temp_lines = 0;
				continue;
			}
		}

		// check for line continuation
		if (tmp[end] == '\\')
		{
			// strip off trailing (\)
			strSize = CUtil::TruncateLine(end - start, strLSLOC.length(), this->lsloc_truncate, trunc_flag);
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
			CUtil::CountTally(strLSLOC, data_name_list, cnt, 1, exclude, "", "", NULL);

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
					CUtil::CountTally(strLSLOC, data_name_list, cnt, 1, exclude, "", "", &result->data_name_count);

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
 * Counts file language complexity based on specified language keywords/characters.
 *
 * \param fmap list of processed file lines
 * \param result counter results
 *
 * \return method status
 */
int CCshCounter::CountComplexity(filemap* fmap, results* result)
{
	if (classtype == UNKNOWN || classtype == DATAFILE)
		return 0;
	filemap::iterator fit;
	size_t idx;
	unsigned int cnt, ret, cyclomatic_cnt = 0, ignore_cyclomatic_cnt = 0, main_cyclomatic_cnt = 0, function_count = 0, cyclomatic_logic_cnt = 0, main_cyclomatic_logic_cnt = 1, cyclomatic_case_cnt = 0, main_cyclomatic_case_cnt = 1, cyclomatic_default_cnt = 0, cyclomatic_switch_cnt = 0;

	string line, lastline, file_ext, function_name = "";
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$";
	filemap function_stack;
	stack<unsigned int> cyclomatic_stack;
	stack<unsigned int> cyclomatic_logic_stack;
	stack<unsigned int> cyclomatic_case_stack;
	map<unsigned int, lineElement> function_map;
	map<unsigned int, lineElement> logical_map;
	map<unsigned int, lineElement> case_map;
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
        	line = echoHelper(line);
        
        
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
		CUtil::CountTally(line, cmplx_preproc_list, cnt, 1, exclude, "", "", &result->cmplx_preproc_count, casesensitive);
		result->cmplx_preproc_lines += cnt;
        
		// assignments
		cnt = 0;
		CUtil::CountTally(line, cmplx_assign_list, cnt, 1, exclude, "", "", &result->cmplx_assign_count, casesensitive);
		result->cmplx_assign_lines += cnt;
     
	/* No pointers for shell scripts
		// pointers
		cnt = 0;
		// Pointers are embedded syntax so there is NO exclude string or include strings
		CUtil::CountTally(line, cmplx_pointer_list, cnt, 1, "", "", "", &result->cmplx_pointer_count, casesensitive);
		result->cmplx_pointer_lines += cnt;
	*/
        
		// cyclomatic complexity
		if (process_cyclomatic_complexity)
		{
			// search for cyclomatic complexity keywords
			CUtil::CountTally(line, cmplx_cyclomatic_list, cyclomatic_cnt, 1, exclude, "", "", 0, casesensitive);

			CUtil::CountTally(line, cmplx_cyclomatic_switch_list, cyclomatic_switch_cnt, 1, exclude, "", "", 0, casesensitive);
          
			// search for keywords to exclude
			if (ignore_cmplx_cyclomatic_list.size() > 0)
				CUtil::CountTally(line, ignore_cmplx_cyclomatic_list, ignore_cyclomatic_cnt, 1, exclude, "", "", 0, casesensitive);

			// search for cyclomatic complexity logical keywords
			if (cmplx_cyclomatic_logic_list.size() > 0)
				CUtil::CountTally(line, cmplx_cyclomatic_logic_list, cyclomatic_logic_cnt, 1, exclude, "", "", 0, casesensitive);

			// search for cyclomatic complexity logical keywords
			if (cmplx_cyclomatic_case_list.size() > 0)
				CUtil::CountTally(line, cmplx_cyclomatic_case_list, cyclomatic_case_cnt, 1, exclude, "", "", 0, casesensitive);

			// search for cyclomatic complexity case default keywords
			if (cmplx_cyclomatic_default_list.size() > 0)
				CUtil::CountTally(line, cmplx_cyclomatic_default_list, cyclomatic_default_cnt, 1, exclude, "", "", 0, casesensitive);

			if(cyclomatic_default_cnt > 0)
			{
				cyclomatic_cnt -= cyclomatic_default_cnt;
				cyclomatic_case_cnt -= cyclomatic_default_cnt;
				cyclomatic_default_cnt = 0;
			}

			// parse function name if found
			ret = (unsigned)ParseFunctionName(line, lastline, function_stack, function_name);
			if (ret != 1 && !cyclomatic_stack.empty() && cyclomatic_stack.size() == function_stack.size())
			{
				// remove count stack entry for non-function names
				cyclomatic_cnt += cyclomatic_stack.top();
				ignore_cyclomatic_cnt = 0;
				cyclomatic_stack.pop();
			}
			if (ret != 1 && !cyclomatic_logic_stack.empty() && cyclomatic_logic_stack.size() == function_stack.size())
			{
				// remove count stack entry for non-function names
				cyclomatic_logic_cnt += cyclomatic_logic_stack.top();
				cyclomatic_logic_stack.pop();
			}
			if (ret != 1 && !cyclomatic_case_stack.empty() && cyclomatic_case_stack.size() == function_stack.size())
			{
				// remove count stack entry for non-function names
				cyclomatic_case_cnt += cyclomatic_case_stack.top();
				cyclomatic_case_stack.pop();
			}
			if (ret == 1)
			{
				// capture count at end of function
				lineElement element(cyclomatic_cnt - ignore_cyclomatic_cnt + 1, function_name);
				function_map[function_count] = element;
				
				lineElement n_element(cyclomatic_cnt - ignore_cyclomatic_cnt + cyclomatic_logic_cnt + 1, function_name);
				logical_map[function_count] = n_element;

				if (cyclomatic_case_cnt > 0)
				{
					lineElement c_element(cyclomatic_cnt - ignore_cyclomatic_cnt - cyclomatic_case_cnt + cyclomatic_switch_cnt + 1, function_name);
					case_map[function_count] = c_element;
				}
				else
					case_map[function_count] = element;
                
				if (!function_stack.empty())
				{
					// grab previous function from stack to continue
					if (!cyclomatic_stack.empty())
					{
						cyclomatic_cnt = cyclomatic_stack.top();
						cyclomatic_stack.pop();
					}
					if (!cyclomatic_logic_stack.empty())
					{
						cyclomatic_logic_cnt = cyclomatic_logic_stack.top();
						cyclomatic_logic_stack.pop();
					}
					if (!cyclomatic_case_stack.empty())
					{
						cyclomatic_case_cnt = cyclomatic_case_stack.top();
						cyclomatic_case_stack.pop();
					}
				}
				else {
					cyclomatic_cnt = 0;
					cyclomatic_logic_cnt = 0;
					cyclomatic_case_cnt = 0;
				}
				function_name = "";
				ignore_cyclomatic_cnt = 0;
				cyclomatic_switch_cnt = 0;
			}
			else if (ret == 2)
			{
				if (main_cyclomatic_cnt < 1)
					main_cyclomatic_cnt = 1;	// add 1 for main function here in case no other decision points are found in main
				// some code doesn't belong to any function
				main_cyclomatic_cnt += cyclomatic_cnt - ignore_cyclomatic_cnt;
				main_cyclomatic_logic_cnt += cyclomatic_cnt - ignore_cyclomatic_cnt + cyclomatic_logic_cnt;
				main_cyclomatic_case_cnt += cyclomatic_cnt - ignore_cyclomatic_cnt - cyclomatic_case_cnt + cyclomatic_switch_cnt;
				cyclomatic_cnt = ignore_cyclomatic_cnt = cyclomatic_logic_cnt = cyclomatic_case_cnt = cyclomatic_switch_cnt = 0;
			}
			else { 
				if (!function_stack.empty() && (function_stack.size() > cyclomatic_stack.size() + 1 || (cyclomatic_stack.empty() && function_stack.size() > 1)))
				{
					// capture previous complexity count from open function
					cyclomatic_stack.push(cyclomatic_cnt - ignore_cyclomatic_cnt);
					cyclomatic_cnt = ignore_cyclomatic_cnt = 0;
				}
				if (!function_stack.empty() && (function_stack.size() > cyclomatic_logic_stack.size() + 1 || (cyclomatic_logic_stack.empty() && function_stack.size() > 1)))
				{
					// capture previous complexity count from open function
					cyclomatic_logic_stack.push(cyclomatic_logic_cnt);
					cyclomatic_logic_cnt = 0;
				}
				if (!function_stack.empty() && (function_stack.size() > cyclomatic_case_stack.size() + 1 || (cyclomatic_case_stack.empty() && function_stack.size() > 1)))
				{
					// capture previous complexity count from open function
					cyclomatic_case_stack.push(cyclomatic_case_cnt - cyclomatic_switch_cnt);
					cyclomatic_case_cnt = 0;
					cyclomatic_switch_cnt = 0;
				}
			}
		}
	}

	// done with a file
	if (main_cyclomatic_cnt > 0)
	{
		// add "main" code
		lineElement element(main_cyclomatic_cnt, "main");
		lineElement n_element(main_cyclomatic_logic_cnt, "main");
		lineElement c_element(main_cyclomatic_case_cnt, "main");
		function_map[0] = element;
		logical_map[0] = n_element;
		case_map[0] = c_element;
	}
	else
	{
		// finish the first function if not closed
		while (!function_stack.empty())
		{
			function_name = function_stack.back().line;
			function_count = function_stack.back().lineNumber;
			function_stack.pop_back();

			if (!function_stack.empty())
			{
				// grab previous function from stack to continue
				if (!cyclomatic_stack.empty())
				{
					cyclomatic_cnt = cyclomatic_stack.top();
					cyclomatic_stack.pop();
				}
			}
			else
			{
				// capture count at end of function
				lineElement element(cyclomatic_cnt - ignore_cyclomatic_cnt + 1, function_name);
				lineElement n_element(cyclomatic_cnt - ignore_cyclomatic_cnt + cyclomatic_logic_cnt + 1, function_name);
				lineElement c_element(cyclomatic_cnt - ignore_cyclomatic_cnt - cyclomatic_case_cnt + cyclomatic_switch_cnt + 1, function_name);
				function_map[function_count] = element;
				logical_map[function_count] = n_element;
				case_map[function_count] = c_element;
			}
		}
	}

	// process ordered functions
	for (map<unsigned int, lineElement>::iterator it = function_map.begin(); it != function_map.end(); ++it)
		result->cmplx_cycfunct_count.push_back(it->second);
	if(cmplx_cyclomatic_logic_list.size() > 0)
	{
		for (map<unsigned int, lineElement>::iterator it = logical_map.begin(); it != logical_map.end(); ++it)
			result->cmplx_cycfunct_CC2_count.push_back(it->second);
	}
	if(cmplx_cyclomatic_case_list.size() > 0)
	{
		for (map<unsigned int, lineElement>::iterator it = case_map.begin(); it != case_map.end(); ++it)
			result->cmplx_cycfunct_CC3_count.push_back(it->second);
	}
	return 1;
}

string CCshCounter::echoHelper(string line){
    string linetmp = "";
    int echoIdx;
    bool isEnd = false;
    int end = 0;
    while (line.length() > 0 && !isEnd) {
        end = (int)line.find(";", 0);
        
        echoIdx = (int)CUtil::FindKeyword(line, "echo");
        
        if (end > 0 && (end < echoIdx || echoIdx < 0)) {
            linetmp += line.substr( 0, (unsigned)end + 1 );
            int len = (int)line.length();
            line = line.substr( (unsigned)end + 1, (unsigned)len + 1 );
            continue;
        }
        else if(end < 0){
            isEnd = true;
        }
        if (echoIdx >= 0) {
            linetmp += " " + line.substr( 0, (unsigned)echoIdx + 5 );
            unsigned int len = line.length();
            if (!isEnd) {
                line = line.substr( (unsigned)end + 1, len + 1 );
            }
            
        }
    }
    if (linetmp != "") {
        line = linetmp;
    }
    return line;

}

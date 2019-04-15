//! Code counter class methods for the Visual Basic language.
/*!
* \file CVbCounter.cpp
*
* This file contains the code counter class methods for the Visual Basic language.
*/

#include "CVbCounter.h"

#define CONTINUATION_CHAR '_'

/*!
* Constructs a CVbCounter object.
*/
CVbCounter::CVbCounter()
{
	classtype = VB;
	language_name = "Visual_Basic";
    
    //Modification: 11.2016 Ext-4
    file_extension = CUtil::getExtensionsToLanguage("Visual_Basic", file_extension);
    
	/*file_extension.push_back(".vb");
	file_extension.push_back(".frm");
	file_extension.push_back(".mod");
	file_extension.push_back(".cls");
	file_extension.push_back(".bas");*/

	QuoteStart = "\x93";
	
	QuoteEnd = "\x94";
	QuoteEscapeRear = '\"';
	casesensitive = false;

	exclude_keywords.push_back("Do");	// must be alone, e.g., not Do i = 5
	exclude_keywords.push_back("Else");
	exclude_keywords.push_back("Loop");
	exclude_keywords.push_back("Wend");

	exclude_start_keywords.push_back("End");
	exclude_start_keywords.push_back("Next");

	LineCommentStart.push_back("'");
	LineCommentStart.push_back("REM ");

	directive.push_back("#Const");
	directive.push_back("#Else");
	directive.push_back("#ElseIf");
	directive.push_back("#End");
	directive.push_back("#ExternalSource");
	directive.push_back("#If");
	directive.push_back("#Region");

	data_name_list.push_back("Boolean");
	data_name_list.push_back("Byte");
	data_name_list.push_back("Collection");
	data_name_list.push_back("Const");
	data_name_list.push_back("Currency");
	data_name_list.push_back("Date");
	data_name_list.push_back("Dim");
	data_name_list.push_back("Double");
	data_name_list.push_back("Integer");
	data_name_list.push_back("Item");
	data_name_list.push_back("Long");
	data_name_list.push_back("New");
	data_name_list.push_back("Object");
	data_name_list.push_back("Option");
	data_name_list.push_back("Private");
	data_name_list.push_back("Public");
	data_name_list.push_back("ReDim");
	data_name_list.push_back("Single");
	data_name_list.push_back("Static");
	data_name_list.push_back("String");
	data_name_list.push_back("Time");
	data_name_list.push_back("Variant");

	exec_name_list.push_back("Add");
	exec_name_list.push_back("AppActivate");
	exec_name_list.push_back("Asc");
	exec_name_list.push_back("Beep");
	exec_name_list.push_back("Call");
	exec_name_list.push_back("CBool");
	exec_name_list.push_back("CByte");
	exec_name_list.push_back("CCur");
	exec_name_list.push_back("CDate");
	exec_name_list.push_back("CDbl");
	exec_name_list.push_back("CDec");
	exec_name_list.push_back("CInt");
	exec_name_list.push_back("CStr");
	exec_name_list.push_back("CVar");
	exec_name_list.push_back("ChDir");
	exec_name_list.push_back("Clear");
	exec_name_list.push_back("Close");
	exec_name_list.push_back("Command");
	exec_name_list.push_back("CreateObject");
	exec_name_list.push_back("CurDir");
	exec_name_list.push_back("Dir");
	exec_name_list.push_back("Do");
	exec_name_list.push_back("DoEvents");
	exec_name_list.push_back("Else");
	exec_name_list.push_back("End");
	exec_name_list.push_back("Environ");
	exec_name_list.push_back("Erase");
	exec_name_list.push_back("Error");
	exec_name_list.push_back("Exit");
	exec_name_list.push_back("FileAttr");
	exec_name_list.push_back("FileCopy");
	exec_name_list.push_back("FileDateTime");
	exec_name_list.push_back("FileLen");
	exec_name_list.push_back("Fix");
	exec_name_list.push_back("For");
	exec_name_list.push_back("Format");
	exec_name_list.push_back("FreeFile");
	exec_name_list.push_back("Function");
	exec_name_list.push_back("Get");
	exec_name_list.push_back("GetAttr");
	exec_name_list.push_back("GetObject");
	exec_name_list.push_back("GoSub");
	exec_name_list.push_back("GoTo");
	exec_name_list.push_back("If");
	exec_name_list.push_back("Input");
	exec_name_list.push_back("InStr");
	exec_name_list.push_back("IsError");
	exec_name_list.push_back("Kill");
	exec_name_list.push_back("Left");
	exec_name_list.push_back("Len");
	exec_name_list.push_back("Line");
	exec_name_list.push_back("Loc");
	exec_name_list.push_back("Lock");
	exec_name_list.push_back("LOF");
	exec_name_list.push_back("Loop");
	exec_name_list.push_back("Mid");
	exec_name_list.push_back("MkDir");
	exec_name_list.push_back("Name");
	exec_name_list.push_back("Next");
	exec_name_list.push_back("Now");
	exec_name_list.push_back("On");
	exec_name_list.push_back("Open");
	exec_name_list.push_back("Print");
	exec_name_list.push_back("Put");
	exec_name_list.push_back("Raise");
	exec_name_list.push_back("Randomize");
	exec_name_list.push_back("Remove");
	exec_name_list.push_back("Reset");
	exec_name_list.push_back("Resume");
	exec_name_list.push_back("Return");
	exec_name_list.push_back("Right");
	exec_name_list.push_back("RmDir");
	exec_name_list.push_back("Rnd");
	exec_name_list.push_back("Seek");
	exec_name_list.push_back("Select");
	exec_name_list.push_back("SendKeys");
	exec_name_list.push_back("Server");
	exec_name_list.push_back("SetAttr");
	exec_name_list.push_back("Shell");
	exec_name_list.push_back("Spc");
	exec_name_list.push_back("Stop");
	exec_name_list.push_back("Str");
	exec_name_list.push_back("Switch");
	exec_name_list.push_back("Sub");
	exec_name_list.push_back("Tab");
	exec_name_list.push_back("Timer");
	exec_name_list.push_back("Unlock");
	exec_name_list.push_back("Val");
	exec_name_list.push_back("Wend");
	exec_name_list.push_back("While");
	exec_name_list.push_back("Width");
	exec_name_list.push_back("With");
	exec_name_list.push_back("Write");

	math_func_list.push_back("Abs");
	math_func_list.push_back("Exp");
	math_func_list.push_back("Round");
	math_func_list.push_back("Rnd");
	math_func_list.push_back("Randomize");
	math_func_list.push_back("Sign");
	math_func_list.push_back("Sqrt");

	trig_func_list.push_back("Atan");
	trig_func_list.push_back("Cos");
	trig_func_list.push_back("Sin");
	trig_func_list.push_back("Tan");

	log_func_list.push_back("Log");

	cmplx_calc_list.push_back("+");
	cmplx_calc_list.push_back("-");
	cmplx_calc_list.push_back("*");
	cmplx_calc_list.push_back("/");
	cmplx_calc_list.push_back("\\");
	cmplx_calc_list.push_back("^");

	cmplx_cond_list.push_back("Case");
	cmplx_cond_list.push_back("Do");
	cmplx_cond_list.push_back("Else");
	cmplx_cond_list.push_back("ElseIf");
	cmplx_cond_list.push_back("For");
	cmplx_cond_list.push_back("If");
	cmplx_cond_list.push_back("Select");
	cmplx_cond_list.push_back("While");

	cmplx_logic_list.push_back("<>");
	cmplx_logic_list.push_back(">");
	cmplx_logic_list.push_back("<");
	cmplx_logic_list.push_back(">=");
	cmplx_logic_list.push_back("=<");
	cmplx_logic_list.push_back("And");
	cmplx_logic_list.push_back("Not");
	cmplx_logic_list.push_back("Or");
	cmplx_logic_list.push_back("Xor");
	cmplx_logic_list.push_back("AndAlso");
	cmplx_logic_list.push_back("OrElse");
	cmplx_logic_list.push_back("IsFalse");
	cmplx_logic_list.push_back("IsTrue");

	cmplx_preproc_list.push_back("#Const");
	cmplx_preproc_list.push_back("#Else");
	cmplx_preproc_list.push_back("#ElseIf");
	cmplx_preproc_list.push_back("#End");
	cmplx_preproc_list.push_back("#ExternalSource");
	cmplx_preproc_list.push_back("#If");
	cmplx_preproc_list.push_back("#Region");

	cmplx_assign_list.push_back("=");

	cmplx_cyclomatic_list.push_back("If");
	cmplx_cyclomatic_list.push_back("ElseIf");
	//cmplx_cyclomatic_list.push_back("IIf");
	cmplx_cyclomatic_list.push_back("For");
	cmplx_cyclomatic_list.push_back("While");
	cmplx_cyclomatic_list.push_back("Until");
	cmplx_cyclomatic_list.push_back("?");
	cmplx_cyclomatic_list.push_back("Catch");
	cmplx_cyclomatic_list.push_back("When");
	cmplx_cyclomatic_list.push_back("Case");

	ignore_cmplx_cyclomatic_list.push_back("End If");
	ignore_cmplx_cyclomatic_list.push_back("End While");

	cmplx_cyclomatic_logic_list.push_back("And");
	//cmplx_cyclomatic_logic_list.push_back("Not");
	cmplx_cyclomatic_logic_list.push_back("Or");
	//cmplx_cyclomatic_logic_list.push_back("Xor");
	cmplx_cyclomatic_logic_list.push_back("AndAlso");
	cmplx_cyclomatic_logic_list.push_back("OrElse");

	cmplx_cyclomatic_case_list.push_back("Case");
	cmplx_cyclomatic_switch_list.push_back("Select Case");
	cmplx_cyclomatic_default_list.push_back("Select Case");
	cmplx_cyclomatic_default_list.push_back("Case Else");
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
int CVbCounter::CountDirectiveSLOC(filemap* fmap, results* result, filemap* /*fmapBak*/)
{
	bool contd = false, trunc_flag = false;
	size_t idx, strSize;
	unsigned int cnt = 0;
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$";
	string strDirLine = "";

	for (filemap::iterator iter = fmap->begin(); iter!=fmap->end(); iter++)
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
			for(vector<string>::iterator viter = directive.begin(); viter != directive.end(); viter++)
			{
				if ((idx = iter->line.find((*viter), 0)) != string::npos && idx == 0)
				{
					contd = true;
					break;
   		        }
        	}
			if (contd)
			{
				result->directive_lines[PHY]++;
				strSize = CUtil::TruncateLine(iter->line.length(), 0, this->lsloc_truncate, trunc_flag);
				if (strSize > 0)
					strDirLine = iter->line.substr(0, strSize);
			}
 		}
		else
		{
			// continuation of a previous directive
			strSize = CUtil::TruncateLine(iter->line.length(), strDirLine.length(), this->lsloc_truncate, trunc_flag);
			if (strSize > 0)
				strDirLine += "\n" + iter->line.substr(0, strSize);
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
int CVbCounter::LanguageSpecificProcess(filemap* fmap, results* result, filemap* fmapBak)
{
	string strLSLOC = "";
    string strLSLOCBak = "";

	filemap::iterator fit, fitbak;
	string line, lineBak;
	size_t i, pos, prev_pos, strSize, tmpLoc;
	unsigned int cnt = 0;
	StringVector loopEnd;
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$";
	string special = "[]()+/-*<>=,@&~!^?:%{}";

	string tmp, tmpstr;
	bool isDataLine = false;
	bool line_continued = false;
	bool line_skipped;
	bool trunc_flag = false;
	bool new_loop = false;

	for (fit = fmap->begin(), fitbak = fmapBak->begin(); fit != fmap->end(); fit++, fitbak++)
	{ 
		line = fit->line;
		lineBak = fitbak->line;

		if (CUtil::CheckBlank(line))
			continue;
		size_t lineNumber = fit->lineNumber; 

		// delete the line number
		pos = line.find_first_of(":");
		if ((pos == 0) || (pos != string::npos && CUtil::IsInteger(line.substr(0,pos))))
		{
			line = line.substr(pos + 1);
			lineBak = lineBak.substr(pos + 1);
		}

		prev_pos = 0;

		// check for inline If..Then
		tmpstr = CUtil::TrimString(line);
		tmpLoc = CUtil::FindKeyword(tmpstr, "Then");
		if (tmpLoc != string::npos)
		{
			if (tmpLoc < tmpstr.length() - 4)
				tmpLoc += 3;
			else
				tmpLoc = string::npos;
		}

		// record nested loops
		if (print_cmplx)
		{
			new_loop = false;
			if (CUtil::FindKeyword(tmpstr, "Do") == 0)
			{
				loopEnd.push_back("Loop");
				new_loop = true;
			}
			else if (CUtil::FindKeyword(tmpstr, "For") == 0)
			{
				loopEnd.push_back("Next");
				new_loop = true;
			}
			else if (CUtil::FindKeyword(tmpstr, "While") == 0)
			{
				loopEnd.push_back("Wend");
				new_loop = true;
			}
			else if (loopEnd.size() > 0)
			{
				if (CUtil::FindKeyword(tmpstr, loopEnd.back()) == 0)
					loopEnd.pop_back();
			}
			if (new_loop)
			{
				if ((unsigned int)result->cmplx_nestloop_count.size() < loopEnd.size())
					result->cmplx_nestloop_count.push_back(1);
				else
					result->cmplx_nestloop_count[loopEnd.size()-1]++;
			}
		}

		tmp = line;
		for (i = 0; i < tmp.size(); i++)
		{
			if ((tmp[i] == ':') || (i == tmp.size() - 1) || tmpLoc != string::npos)
			{
				if (tmpLoc != string::npos) 
				{
					i = tmpLoc;
					tmpLoc = string::npos;
				}
				else
					tmpstr = CUtil::TrimString(tmp.substr(prev_pos, i - prev_pos + 1));

				// exclude SLOC defined in the exclude_keywords
				line_skipped = false;
				for (vector<string>::iterator stri = exclude_keywords.begin(); stri != exclude_keywords.end(); stri++)
				{
					if (tmpstr.compare(*stri) == 0)
					{
						line_skipped = true;
						break;
					}
				}
				if (line_skipped)
					continue;

				// exclude SLOC starting with Next, End
				for (StringVector::iterator stri = exclude_start_keywords.begin(); stri != exclude_start_keywords.end(); stri++)
				{
					if (CUtil::FindKeyword(tmpstr, *stri) == 0)
					{
						line_skipped = true;
						break;
					}
				}
				if (line_skipped)
					continue;

				strSize = CUtil::TruncateLine(i + 1 - prev_pos, strLSLOC.length(), this->lsloc_truncate, trunc_flag);
				if (strSize > 0)
				{
					strLSLOC += line.substr(prev_pos, strSize);
					strLSLOCBak += lineBak.substr(prev_pos, strSize);
				}

				line_continued = ((i == tmp.size() - 1) && tmpstr[tmpstr.length() - 1] == CONTINUATION_CHAR);				
				if (line_continued)
				{
					// drop continuation symbol
					if (strLSLOC[strLSLOC.length()-1] == '_')
					{
						strLSLOC = strLSLOC.substr(0, strLSLOC.length()-1);
						strLSLOCBak = strLSLOCBak.substr(0, strLSLOCBak.length()-1);
					}
					continue;
				}

				isDataLine = false;
				if (result->addSLOC(strLSLOCBak, lineNumber, trunc_flag))
				{
					cnt = 0;
					CUtil::CountTally(strLSLOC, data_name_list, cnt, 1, exclude, "", "", &result->data_name_count);
					if (cnt > 0)
					{
						isDataLine = true;
						result->data_lines[LOG]++;
					}
					else
					{
						if (print_cmplx)
						{
							cnt = 0;
							CUtil::CountTally(strLSLOC, exec_name_list, cnt, 1, exclude, "", "", &result->exec_name_count);
						}
						result->exec_lines[LOG]++;
					}
				}
				strLSLOCBak = "";
				strLSLOC = "";
			 	prev_pos = i + 1;

				if (tmpLoc != string::npos)
				{
					tmpLoc = string::npos;
					prev_pos++;
				}
			}

            if (special.find_first_of(tmp[i]) != string::npos)
				tmp[i] = ' ';
		}

		if (isDataLine)
			result->data_lines[PHY]++;
		else
			result->exec_lines[PHY]++;
	}
	return 1;
}

/*!
* Parses lines for function/method names.
*
* \param line line to be processed
* \param functionStack stack of functions
* \param functionName function name found
* \param functionCount function count found
*
* \return 1 if function name is found
*/
int CVbCounter::ParseFunctionName(const string &line, string &/*lastline*/,
	filemap &functionStack, string &functionName, unsigned int &functionCount)
{
	string str;
	size_t idx;
	unsigned int fcnt;

	idx = CUtil::FindKeyword(line, "Sub");
	if (idx != string::npos)
	{
		if (idx + 4 < line.length())
		{
			str = line.substr(idx + 4);
			lineElement element(++functionCount, str);
			functionStack.push_back(element);
		}
	}
	else
	{
		idx = CUtil::FindKeyword(line, "Function");
		if (idx != string::npos)
		{
			if (idx + 9 < line.length())
			{
				str = line.substr(idx + 9);
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

	idx = CUtil::FindKeyword(line, "End Sub");
	if (idx != string::npos)
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
	}
	else
	{
		idx = CUtil::FindKeyword(line, "End Function");
		if (idx != string::npos)
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
		}
	}
	return 0;
}

//! Code counter class methods for the Interactive Data Language (IDL).
/*!
* \file CIdlCounter.cpp
*
* This file contains the code counter class methods for the Interactive Data Language (IDL).
*/
#include "CIdlCounter.h"

/*!
* Constructs a CIdlCounter object.
*/
CIdlCounter::CIdlCounter()
{
    classtype = IDL;
    language_name = "IDL";
    casesensitive = false;
    //Modification: 11.2016 Ext-4
    file_extension = CUtil::getExtensionsToLanguage("IDL", file_extension);
    /*
    file_extension.push_back(".pro");
    file_extension.push_back(".sav");*/

    QuoteStart = "\"'";
    QuoteEnd = "\"'";
    ContinueLine = "&";

    LineCommentStart.push_back(";");

    directive.push_back("@");

    exec_name_list.push_back("begin");
    exec_name_list.push_back("break");
    exec_name_list.push_back("case");
    exec_name_list.push_back("common");
    exec_name_list.push_back("compile_opt"); 
    exec_name_list.push_back("continue");
    exec_name_list.push_back("else");
    exec_name_list.push_back("for");
    exec_name_list.push_back("foreach"); 
    exec_name_list.push_back("forward_function"); 
    exec_name_list.push_back("function");
    exec_name_list.push_back("goto");
    exec_name_list.push_back("if");
    exec_name_list.push_back("inherits");
    exec_name_list.push_back("on_ioerror");
    exec_name_list.push_back("pro"); 
    exec_name_list.push_back("repeat");
    exec_name_list.push_back("return"); 
    exec_name_list.push_back("switch");
    exec_name_list.push_back("while");

    math_func_list.push_back("abs");
    math_func_list.push_back("exp");
    math_func_list.push_back("factorial");
    math_func_list.push_back("fft");
    math_func_list.push_back("fix");
    math_func_list.push_back("float");
    math_func_list.push_back("max");
    math_func_list.push_back("mean");
    math_func_list.push_back("min");
    math_func_list.push_back("primes");
    math_func_list.push_back("randomu");
    math_func_list.push_back("round");
    math_func_list.push_back("sqrt");
    math_func_list.push_back("total");

    trig_func_list.push_back("acos");
    trig_func_list.push_back("asin");
    trig_func_list.push_back("atan");
    trig_func_list.push_back("cos");
    trig_func_list.push_back("cosh");
    trig_func_list.push_back("sin");
    trig_func_list.push_back("sinh");
    trig_func_list.push_back("tan");
    trig_func_list.push_back("tanh");

    log_func_list.push_back("alog");
    log_func_list.push_back("alog10");

    cmplx_calc_list.push_back("mod");
    cmplx_calc_list.push_back("^");
    cmplx_calc_list.push_back("++");
    cmplx_calc_list.push_back("+");
    cmplx_calc_list.push_back("--");
    cmplx_calc_list.push_back("-");
    cmplx_calc_list.push_back("*");
    cmplx_calc_list.push_back("/");
    cmplx_calc_list.push_back("#");
    cmplx_calc_list.push_back("##");

    cmplx_cond_list.push_back("case");
    cmplx_cond_list.push_back("else"); 
    cmplx_cond_list.push_back("for");
    cmplx_cond_list.push_back("foreach"); 
    cmplx_cond_list.push_back("if");
    cmplx_cond_list.push_back("repeat");
    cmplx_cond_list.push_back("switch");
    cmplx_cond_list.push_back("while");

    cmplx_logic_list.push_back("&&");
    cmplx_logic_list.push_back("||");
    cmplx_logic_list.push_back("~");
    cmplx_logic_list.push_back("and");
    cmplx_logic_list.push_back("not");
    cmplx_logic_list.push_back("or");
    cmplx_logic_list.push_back("xor");
    cmplx_logic_list.push_back(">");
    cmplx_logic_list.push_back("<");
    cmplx_logic_list.push_back("lt");
    cmplx_logic_list.push_back("gt");
    cmplx_logic_list.push_back("ge");
    cmplx_logic_list.push_back("le");
    cmplx_logic_list.push_back("eq");
    cmplx_logic_list.push_back("ne");
    cmplx_logic_list.push_back("##=");
    cmplx_logic_list.push_back("#=");
    cmplx_logic_list.push_back("*=");
    cmplx_logic_list.push_back("+=");
    cmplx_logic_list.push_back(".=");
    cmplx_logic_list.push_back("/=");
    cmplx_logic_list.push_back("<=");
    cmplx_logic_list.push_back(">=");
    cmplx_logic_list.push_back("^=");
    cmplx_logic_list.push_back("and=");
    cmplx_logic_list.push_back("or=");
    cmplx_logic_list.push_back("xor=");
    cmplx_logic_list.push_back(">");
    cmplx_logic_list.push_back("<");
    cmplx_logic_list.push_back("lt");
    cmplx_logic_list.push_back("gt=");
    cmplx_logic_list.push_back("ge=");
    cmplx_logic_list.push_back("le=");
    cmplx_logic_list.push_back("eq=");
    cmplx_logic_list.push_back("ne=");

    cmplx_preproc_list.push_back("@");

    cmplx_pointer_list.push_back("->");
    cmplx_pointer_list.push_back(".");

    /*cmplx_cyclomatic_list.push_back("for");
    cmplx_cyclomatic_list.push_back("foreach"); 
    cmplx_cyclomatic_list.push_back("if");
    cmplx_cyclomatic_list.push_back("repeat");
    cmplx_cyclomatic_list.push_back("while");
    cmplx_cyclomatic_list.push_back("?");*/

    cmplx_assign_list.push_back("=");

    statement_list.push_back("&&");
    statement_list.push_back("||");
    statement_list.push_back("~");
    statement_list.push_back(">");
    statement_list.push_back("<");
    statement_list.push_back("##=");
    statement_list.push_back("#=");
    statement_list.push_back("*=");
    statement_list.push_back("+=");
    statement_list.push_back(".=");
    statement_list.push_back("/=");
    statement_list.push_back("<=");
    statement_list.push_back(">=");
    statement_list.push_back("^=");
    statement_list.push_back(">");
    statement_list.push_back("<");
    statement_list.push_back(" ");
    statement_list.push_back("(");
    statement_list.push_back(")");
}

/*!
* Perform preprocessing of file lines before counting.
*
* \param fmap list of file lines
*
* \return method status
*/
int CIdlCounter::PreCountProcess(filemap* fmap)
{
    size_t i, j, llen;
    filemap::iterator fit;
    for (fit = fmap->begin(); fit != fmap->end(); fit++)
    {
        if (fit->line.empty())
            continue;

        // check for octal and hexadecimal constants (replace any single/double quotes with '$' if found)
        llen = fit->line.length();
        for (i = 0; i < llen - 1; i++)
        {
            if (fit->line[i] == '\"')
            {
                j = i + 1;
                while (isdigit(fit->line[j]) ||
                    fit->line[j] == 'A' || fit->line[j] == 'B' || fit->line[j] == 'C' || fit->line[j] == 'D' || fit->line[j] == 'E' || fit->line[j] == 'F' ||
                    fit->line[j] == 'a' || fit->line[j] == 'b' || fit->line[j] == 'c' || fit->line[j] == 'd' || fit->line[j] == 'e' || fit->line[j] == 'f')
                {
                    j++;
                    if (j >= llen) {
						break;
					}
                }
                if (j + 1 >= llen || fit->line[j] == ' ' || fit->line[j] == 'B')   // "n or "nB
				{
					fit->line[i] = '$';
				}
                else if (fit->line[j] == 'L')
                {
                    if (j + 1 >= llen || fit->line[j + 1] == ' ' || fit->line[j + 1] == 'L')        // "nL or "nLL
                    {
                        fit->line[i] = '$';
                    }
                }
                else if (fit->line[j] == 'U')
                {
                    if (j + 1 >= llen || fit->line[j + 1] == ' ')                                   // "nU
                    {
                        fit->line[i] = '$';
                    }
                    else if (fit->line[j + 1] == 'L')
                    {
                        if (j + 2 >= llen || fit->line[j + 2] == ' ' || fit->line[j + 2] == 'L')    // "nUL or "nULL
                        {
                            fit->line[i] = '$';
                        }
                    }
                }
            }
            else if (fit->line[i] == '\'')
            {
                j = i + 1;
                while (isdigit(fit->line[j]) ||
                    fit->line[j] == 'A' || fit->line[j] == 'B' || fit->line[j] == 'C' || fit->line[j] == 'D' || fit->line[j] == 'E' || fit->line[j] == 'F' ||
                    fit->line[j] == 'a' || fit->line[j] == 'b' || fit->line[j] == 'c' || fit->line[j] == 'd' || fit->line[j] == 'e' || fit->line[j] == 'f')
                {
                    j++;
                    if (j + 1 >= llen)
						break;
                }
                if (fit->line[j] == '\'')
                {

				  if(j + 1 < llen) 
				  {

						if (fit->line[j + 1] == 'O')
						{
							if (j + 2 >= llen || fit->line[j + 2] == ' ' || fit->line[j + 2] == 'S')            // 'n'O or 'n'OS
							{
								fit->line[i] = '$';
								fit->line[j] = '$';
							}
							else if (fit->line[j + 2] == 'L')
							{
								if (j + 3 >= llen || fit->line[j + 3] == ' ' || fit->line[j + 3] == 'L')        // 'n'OL or 'n'OLL
								{
									fit->line[i] = '$';
									fit->line[j] = '$';
								}
							}
							else if (fit->line[j + 2] == 'U')
							{
								if (j + 3 >= llen || fit->line[j + 3] == ' ' || fit->line[j + 3] == 'S')        // 'n'OU or 'n'OUS
								{
									fit->line[i] = '$';
									fit->line[j] = '$';
								}
								else if (fit->line[j + 3] == 'L')
								{
									if (j + 4 >= llen || fit->line[j + 4] == ' ' || fit->line[j + 4] == 'L')    // 'n'OUL or 'n'OULL
									{
										fit->line[i] = '$';
										fit->line[j] = '$';
									}
								}
							}
						}
						else if (fit->line[j + 1] == 'X')
						{
							if (j + 2 >= llen || fit->line[j + 2] == ' ' || fit->line[j + 2] == 'B' || fit->line[j + 2] == 'S') // 'n'X or 'n'XB or 'n'XS
							{
								fit->line[i] = '$';
								fit->line[j] = '$';
							}
							else if (fit->line[j + 2] == 'L')
							{
								if (j + 3 >= llen || fit->line[j + 3] == ' ' || fit->line[j + 3] == 'L')                        // 'n'XL or 'n'XLL
								{
									fit->line[i] = '$';
									fit->line[j] = '$';
								}
							}
							else if (fit->line[j + 2] == 'U')
							{
								if (j + 3 >= llen || fit->line[j + 3] == ' ' || fit->line[j + 3] == 'S')                        // 'n'XU or 'n'XUS 
								{
									fit->line[i] = '$';
									fit->line[j] = '$';
								}
								else if (fit->line[j + 3] == 'L')
								{
									if (j + 4 >= llen || fit->line[j + 4] == ' ' || fit->line[j + 4] == 'L')                    // 'n'XUL or 'n'XULL 
									{
										fit->line[i] = '$';
										fit->line[j] = '$';
									}
								}
							}
						}
				 }
				 else 
					 fit->line[i] = '$';  // replace trailing '
                }
            }
        }
    }

    return 0;
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
int CIdlCounter::CountDirectiveSLOC(filemap* fmap, results* result, filemap* fmapBak)
{
    bool trunc_flag = false;
    size_t strSize;
    unsigned int cnt = 0;
    string strDirLine = "";

    filemap::iterator itfmBak = fmapBak->begin();
    for (filemap::iterator iter = fmap->begin(); iter != fmap->end(); iter++, itfmBak++)
    {
        if (CUtil::CheckBlank(iter->line))
            continue;

        if (print_cmplx)
        {
            cnt = 0;
            CUtil::CountTally(" " + iter->line, directive, cnt, 1, "", "", "", &result->directive_count, casesensitive);
        }

        // process include statement as a directive
        if (iter->line[0] == '@')
        {
            result->directive_lines[PHY]++;

            strSize = CUtil::TruncateLine(itfmBak->line.length(), 0, this->lsloc_truncate, trunc_flag);
            if (strSize > 0)
            {
                strDirLine = itfmBak->line.substr(0, strSize);
                if (result->addSLOC(strDirLine, trunc_flag))
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
int CIdlCounter::LanguageSpecificProcess(filemap* fmap, results* result, filemap* fmapBak)
{
    bool cont_str = false;
    unsigned int openBrackets = 0;
    string strLSLOC = "";
    string strLSLOCBak = "";
    filemap::iterator fit, fitbak;
    string line, lineBak;
    StringVector loopLevel;
    unsigned int cnt = 0;
    string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_";

    for (fit = fmap->begin(), fitbak = fmapBak->begin(); fit != fmap->end(); fit++, fitbak++)
    {
        line = fit->line;

        // insert blank at the beginning (for searching keywords)
        line = ' ' + line;
        lineBak = ' ' + fitbak->line;

        // do not process blank lines
        // blank line means blank_line/comment_line/directive
        if (!CUtil::CheckBlank(line))
        {
            LSLOC(result, line, lineBak, strLSLOC, strLSLOCBak, cont_str, openBrackets, loopLevel);

            if (print_cmplx)
            {
                cnt = 0;
                CUtil::CountTally(line, exec_name_list, cnt, 1, exclude, "", "", &result->exec_name_count, casesensitive);
            }
            result->exec_lines[PHY]++;
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
* \param cont_str continue string
* \param openBrackets number of open brackets (no matching close bracket)
* \param loopLevel nested loop level
*/
void CIdlCounter::LSLOC(results* result, string line, string lineBak, string &strLSLOC, string &strLSLOCBak,
    bool &cont_str, unsigned int &openBrackets, StringVector &loopLevel)
{
    size_t start = 0, len;
    size_t i = 0, j, k, strSize;
    bool trunc_flag = false;
    string tmp, tmpBak, str;

    int inlineStatement = 0;

    // check exclusions/continuation  
    tmp = CUtil::TrimString(line);
    tmpBak = CUtil::TrimString(lineBak);
    if (CUtil::FindKeyword(tmp, "end", 0, TO_END_OF_STRING, casesensitive) == 0 ||
        CUtil::FindKeyword(tmp, "endforeach", 0, TO_END_OF_STRING, casesensitive) == 0 ||
        CUtil::FindKeyword(tmp, "endfor", 0, TO_END_OF_STRING, casesensitive) == 0 ||
        CUtil::FindKeyword(tmp, "endwhile", 0, TO_END_OF_STRING, casesensitive) == 0 ||
        CUtil::FindKeyword(tmp, "endelse", 0, TO_END_OF_STRING, casesensitive) == 0 ||
        CUtil::FindKeyword(tmp, "endif", 0, TO_END_OF_STRING, casesensitive) == 0 ||
        CUtil::FindKeyword(tmp, "endcase", 0, TO_END_OF_STRING, casesensitive) == 0 ||
        CUtil::FindKeyword(tmp, "endswitch", 0, TO_END_OF_STRING, casesensitive) == 0)
    {
        if (loopLevel.size() > 0)
            loopLevel.pop_back();
        return;
    }
    else if (CUtil::FindKeyword(tmp, "repeat", 0, TO_END_OF_STRING, casesensitive) != string::npos &&
        CUtil::FindKeyword(tmp, "begin", 0, TO_END_OF_STRING, casesensitive) != string::npos)
    {
        if (loopLevel.size() > 0)
            loopLevel.pop_back();
        return;
    }
    else if (CUtil::FindKeyword(tmp, "else", 0, TO_END_OF_STRING, casesensitive) != string::npos &&
        tmp.find_first_of(":") != std::string::npos &&
        CUtil::FindKeyword(tmp, "begin", 0, TO_END_OF_STRING, casesensitive) != string::npos)
    {
        if (loopLevel.size() > 0)
            loopLevel.pop_back();
        return;
    }
    else if (CUtil::FindKeyword(tmp, "of", 0, TO_END_OF_STRING, casesensitive) == 0 )
    {
        strLSLOC += tmp + " ";
        strLSLOCBak += tmpBak + " ";
        return;
    }
    // process nested loops
    if (print_cmplx)
    {
        if (CUtil::FindKeyword(tmp, "for", 0, TO_END_OF_STRING, casesensitive) != string::npos ||
            CUtil::FindKeyword(tmp, "while", 0, TO_END_OF_STRING, casesensitive) != string::npos ||
            CUtil::FindKeyword(tmp, "foreach", 0, TO_END_OF_STRING, casesensitive) != string::npos ||
            CUtil::FindKeyword(tmp, "goto", 0, TO_END_OF_STRING, casesensitive) != string::npos ||
            CUtil::FindKeyword(tmp, "break", 0, TO_END_OF_STRING, casesensitive) != string::npos ||
            CUtil::FindKeyword(tmp, "continue", 0, TO_END_OF_STRING, casesensitive) != string::npos ||
            CUtil::FindKeyword(tmp, "until", 0, TO_END_OF_STRING, casesensitive) != string::npos)
        {
            loopLevel.push_back("loop");

            // record nested loop level
            unsigned int loopCnt = 0;
            for (StringVector::iterator lit = loopLevel.begin(); lit < loopLevel.end(); lit++)
            {
                if ((*lit) != "")
                    loopCnt++;
            }
            if ((unsigned int)result->cmplx_nestloop_count.size() < loopCnt)
                result->cmplx_nestloop_count.push_back(1);
            else
                result->cmplx_nestloop_count[loopCnt - 1]++;
        }
        else if (CUtil::FindKeyword(tmp, "if", 0, TO_END_OF_STRING, casesensitive) != string::npos ||
                 CUtil::FindKeyword(tmp, "case", 0, TO_END_OF_STRING, casesensitive) != string::npos ||
                 CUtil::FindKeyword(tmp, "switch", 0, TO_END_OF_STRING, casesensitive) != string::npos)
                loopLevel.push_back("");
    }

	if (CUtil::FindKeyword(tmp, "do", 0, TO_END_OF_STRING, casesensitive) != string::npos &&
                CUtil::FindKeyword(tmp, "begin", 0, TO_END_OF_STRING, casesensitive) == string::npos)
	{
                strSize = CUtil::TruncateLine(tmp.length(), strLSLOC.length(), this->lsloc_truncate, trunc_flag);
                if (strSize > 0)
                {
                    strLSLOC += tmp.substr(0, strSize);
                    strLSLOCBak += tmpBak.substr(0, strSize);

                    j = CUtil::ToLower(strLSLOC).find("do");
                    if (j != string::npos)
                    {
                        result->addSLOC(strLSLOCBak.substr(0, j + 2), trunc_flag);  // sloc before and including "do"
                        result->exec_lines[LOG]++;

                        result->addSLOC(strLSLOCBak.substr(j + 2), trunc_flag);     // sloc past "do"
                        result->exec_lines[LOG]++;

                        strLSLOC = strLSLOCBak = "";
                        cont_str = false;
                        inlineStatement = 1;
                    }
                }
	}
    else if (CUtil::FindKeyword(tmp, "do", 0, TO_END_OF_STRING, casesensitive) != string::npos &&
                CUtil::FindKeyword(tmp, "begin", 0, TO_END_OF_STRING, casesensitive) != string::npos)
    {
				strSize = CUtil::TruncateLine(tmp.length(), strLSLOC.length(), this->lsloc_truncate, trunc_flag);
                if (strSize > 0)
                {
                    strLSLOC += tmp.substr(0, strSize);
                    strLSLOCBak += tmpBak.substr(0, strSize);

                    j = CUtil::ToLower(strLSLOC).find("do");
                    if (j != string::npos)
                    {
                        result->addSLOC(strLSLOCBak.substr(0, j + 2), trunc_flag);  // sloc before and including "do"
						result->exec_lines[LOG]++;

						string next_str = strLSLOCBak.substr(j + 2);  // sloc past "do" and ending after "begin"

						if (CUtil::FindKeyword(next_str, "else", 0, TO_END_OF_STRING, casesensitive) != string::npos)  
						{
							size_t found_then = CUtil::ToLower(next_str).find("then");
							if (found_then != string::npos)
							{
								result->addSLOC(next_str.substr(0, found_then + 4), trunc_flag);     // sloc before and including "then"
								result->exec_lines[LOG]++;

								size_t found_else = CUtil::ToLower(next_str).find("else");
								result->addSLOC(next_str.substr(found_then + 4, found_else- found_then -4), trunc_flag);  // sloc past "then" and before "else"
								result->exec_lines[LOG]++;

								strLSLOC = strLSLOCBak = next_str = "";
								cont_str = false;
								inlineStatement = 1;
							}
						}
						else
						{
							string next_str_1 = strLSLOCBak.substr(j + 2);
							if (CUtil::FindKeyword(next_str_1, "begin", 0, TO_END_OF_STRING, casesensitive) == string::npos)   // no "begin" 
							{
								result->addSLOC(strLSLOCBak.substr(j + 2), trunc_flag);     // sloc past "do" with no "else"
								result->exec_lines[LOG]++;
							}
							else  // "begin" exists
							{
								size_t found_begin = CUtil::ToLower(next_str_1).find("begin");
								string next_str_2 = next_str_1.substr(0, found_begin);
								if(next_str_2.find_first_not_of(' ') != std::string::npos)
								{
									// There's a non-space.
									result->addSLOC(strLSLOCBak.substr(j + 2), trunc_flag);     // sloc past "do" with no "else"
									result->exec_lines[LOG]++;
								}
							}
						}

                        strLSLOC = strLSLOCBak = "";
                        cont_str = false;
                        inlineStatement = 1;
                    }
                }
	}

    if (CUtil::FindKeyword(tmp, "do", 0, TO_END_OF_STRING, casesensitive) != string::npos &&
        CUtil::FindKeyword(tmp, "begin", 0, TO_END_OF_STRING, casesensitive) == string::npos &&
        inlineStatement != 1)
    {
        strSize = CUtil::TruncateLine(tmp.length(), strLSLOC.length(), this->lsloc_truncate, trunc_flag);
        if (strSize > 0)
        {
            strLSLOC += tmp.substr(0, strSize);
            strLSLOCBak += tmpBak.substr(0, strSize);

            j = CUtil::ToLower(strLSLOC).find("do");
            if (j != string::npos)
            {
                result->addSLOC(strLSLOCBak.substr(0, j + 2), trunc_flag);  // sloc before and including "do"
                result->exec_lines[LOG]++;

                result->addSLOC(strLSLOCBak.substr(j + 2), trunc_flag);     // sloc past "do"
                result->exec_lines[LOG]++;

                strLSLOC = strLSLOCBak = "";
                cont_str = false;
                inlineStatement = 1;
            }
        }
    }

    if (CUtil::FindKeyword(tmp, "begin", 0, TO_END_OF_STRING, casesensitive) != string::npos &&
        tmp.find_first_of(":") != std::string::npos &&
        inlineStatement != 1)
    {
        strSize = CUtil::TruncateLine(tmp.length(), strLSLOC.length(), this->lsloc_truncate, trunc_flag);
        if (strSize > 0)
        {
            strLSLOC += tmp.substr(0, strSize);
            strLSLOCBak += tmpBak.substr(0, strSize);

            j = strLSLOC.find(":");
            if (j != string::npos)
            {
                string temp_string = strLSLOCBak.substr(0, j);  // sloc before ":"
                for (vector<string>::const_iterator i = statement_list.begin(); i != statement_list.end(); ++i)
                {
                    std::size_t found = temp_string.find(*i);
                    if (found != string::npos)
                    {
                        result->addSLOC(temp_string, trunc_flag);
                        result->exec_lines[LOG]++;
                        strLSLOC = strLSLOCBak = temp_string = "";
                        cont_str = false;
                        break;
                    }
                }
                inlineStatement = 1;
            }
        }
    }
    if (CUtil::FindKeyword(tmp, "begin", 0, TO_END_OF_STRING, casesensitive) == string::npos &&
        CUtil::FindKeyword(tmp, "else", 0, TO_END_OF_STRING, casesensitive) == string::npos &&
        tmp.find_first_of(":") != std::string::npos &&
		tmp.find_first_of("?") == std::string::npos &&
        inlineStatement != 1)
    {
        strSize = CUtil::TruncateLine(tmp.length(), strLSLOC.length(), this->lsloc_truncate, trunc_flag);
        if (strSize > 0)
        {
            strLSLOC += tmp.substr(0, strSize);
            strLSLOCBak += tmpBak.substr(0, strSize);

            j = strLSLOC.find(":");
            if (j != string::npos)
            {
                string temp_string_before = strLSLOCBak.substr(0, j);   // sloc before ":"
                for (vector<string>::const_iterator i = statement_list.begin(); i != statement_list.end(); ++i)
                {
                    std::size_t found = temp_string_before.find(*i);
                    if (found != std::string::npos)
                    {
                        result->addSLOC(temp_string_before, trunc_flag);
                        result->exec_lines[LOG]++;
                        break;
                    }
                }

                result->addSLOC(strLSLOCBak.substr(j + 1), trunc_flag); // sloc after ":" to the end
                result->exec_lines[LOG]++;

                strLSLOC = strLSLOCBak = temp_string_before = "";
                cont_str = false;
                inlineStatement = 1;
            }
        }
    }
    if (CUtil::FindKeyword(tmp, "begin", 0, TO_END_OF_STRING, casesensitive) == string::npos &&
        CUtil::FindKeyword(tmp, "else", 0, TO_END_OF_STRING, casesensitive) == string::npos &&
        tmp.find_first_of(":") != std::string::npos &&
		tmp.find_first_of("?") != std::string::npos &&
        inlineStatement != 1)
    {
        strSize = CUtil::TruncateLine(tmp.length(), strLSLOC.length(), this->lsloc_truncate, trunc_flag);
        if (strSize > 0)
        {
            strLSLOC += tmp.substr(0, strSize);
            strLSLOCBak += tmpBak.substr(0, strSize);

            j = strLSLOC.find("?");
            if (j != string::npos)
            {
                string temp_string_before = strLSLOCBak.substr(0, j+1);   // sloc before "?"
                for (vector<string>::const_iterator i = statement_list.begin(); i != statement_list.end(); ++i)
                {
                    std::size_t found = temp_string_before.find(*i);
                    if (found != std::string::npos)
                    {
                        result->addSLOC(temp_string_before, trunc_flag);
                        result->exec_lines[LOG]++;
                        break;
                    }
                }

				string temp_string_after = strLSLOCBak.substr(j + 1);
				k = strLSLOC.find(":");

                result->addSLOC(strLSLOCBak.substr(j + 1, k - j - 1), trunc_flag); // sloc after "?" to ":"
                result->exec_lines[LOG]++;

                result->addSLOC(strLSLOCBak.substr(k), trunc_flag); // sloc after ":" to the end
                result->exec_lines[LOG]++;

				strLSLOC = strLSLOCBak = temp_string_before = temp_string_after = "";
				cont_str = false;
				inlineStatement = 1;


            }
        }
    }

    if (CUtil::FindKeyword(tmp, "repeat", 0, TO_END_OF_STRING, casesensitive) != string::npos &&
        CUtil::FindKeyword(tmp, "until", 0, TO_END_OF_STRING, casesensitive) != string::npos &&
        CUtil::FindKeyword(tmp, "begin", 0, TO_END_OF_STRING, casesensitive) == string::npos &&
        inlineStatement != 1)
    {
        strSize = CUtil::TruncateLine(tmp.length(), strLSLOC.length(), this->lsloc_truncate, trunc_flag);
        if (strSize > 0)
        {
            strLSLOC += tmp.substr(0, strSize);
            strLSLOCBak += tmpBak.substr(0, strSize);

            j = CUtil::ToLower(strLSLOC).find("until");
            if (j != string::npos)
            {
                result->addSLOC(strLSLOCBak.substr(0, j), trunc_flag);  // sloc before "until"
                result->exec_lines[LOG]++;

                result->addSLOC(strLSLOCBak.substr(j), trunc_flag);     // sloc from "until" to the end
                result->exec_lines[LOG]++;

                strLSLOC = strLSLOCBak = "";
                cont_str = false;
                inlineStatement = 1;
            }
        }
    }

    if (CUtil::FindKeyword(tmp, "then", 0, TO_END_OF_STRING, casesensitive) != string::npos &&
        CUtil::FindKeyword(tmp, "begin", 0, TO_END_OF_STRING, casesensitive) == string::npos &&
        CUtil::FindKeyword(tmp, "else", 0, TO_END_OF_STRING, casesensitive) == string::npos &&
        inlineStatement != 1 &&
		tmp.substr(tmp.length() - 1, 1) != "$")
    {
        strSize = CUtil::TruncateLine(tmp.length(), strLSLOC.length(), this->lsloc_truncate, trunc_flag);
        if (strSize > 0)
        {
            strLSLOC += tmp.substr(0, strSize);
            strLSLOCBak += tmpBak.substr(0, strSize);

            size_t found_then = CUtil::ToLower(strLSLOC).find("then");
            if (found_then != string::npos)
            {
                result->addSLOC(strLSLOCBak.substr(0, found_then + 4), trunc_flag);  // sloc before and including "then"
                result->exec_lines[LOG]++;

                result->addSLOC(strLSLOCBak.substr(found_then + 4), trunc_flag);     // sloc past "then"
                result->exec_lines[LOG]++;

                strLSLOC = strLSLOCBak = "";
                cont_str = false;
                inlineStatement = 1;
            }
        }
    }

    if (CUtil::FindKeyword(tmp, "then", 0, TO_END_OF_STRING, casesensitive) != string::npos &&
        CUtil::FindKeyword(tmp, "else", 0, TO_END_OF_STRING, casesensitive) != string::npos &&
        CUtil::FindKeyword(tmp, "begin", 0, TO_END_OF_STRING, casesensitive) == string::npos &&
        inlineStatement != 1)
    {
        strSize = CUtil::TruncateLine(tmp.length(), strLSLOC.length(), this->lsloc_truncate, trunc_flag);
        if (strSize > 0)
        {
            strLSLOC += tmp.substr(0, strSize);
            strLSLOCBak += tmpBak.substr(0, strSize);

            str = CUtil::ToLower(strLSLOC);
            size_t found_then = str.find("then");
            if (found_then != string::npos)
            {
                result->addSLOC(strLSLOCBak.substr(0, found_then + 4), trunc_flag);     // sloc before and including "then"
                result->exec_lines[LOG]++;

                size_t found_else = str.find("else");
                result->addSLOC(strLSLOCBak.substr(found_then + 4, found_else - found_then - 4), trunc_flag);  // sloc past "then" and before "else"
                result->exec_lines[LOG]++;

                result->addSLOC(strLSLOCBak.substr(found_else), trunc_flag);                    // sloc from "else" to the end
                result->exec_lines[LOG]++;

                strLSLOC = strLSLOCBak = "";
                cont_str = false;
                inlineStatement = 1;
            }
        }
    }

    if (CUtil::FindKeyword(tmp, "then", 0, TO_END_OF_STRING, casesensitive) != string::npos &&
        CUtil::FindKeyword(tmp, "else", 0, TO_END_OF_STRING, casesensitive) != string::npos &&
        CUtil::FindKeyword(tmp, "begin", 0, TO_END_OF_STRING, casesensitive) != string::npos &&
        inlineStatement != 1)
    {
        strSize = CUtil::TruncateLine(tmp.length(), strLSLOC.length(), this->lsloc_truncate, trunc_flag);
        if (strSize > 0)
        {
            strLSLOC += tmp.substr(0, strSize);
            strLSLOCBak += tmpBak.substr(0, strSize);

            str = CUtil::ToLower(strLSLOC);
            size_t found_then = str.find("then");
            if (found_then != string::npos)
            {
                result->addSLOC(strLSLOCBak.substr(0, found_then + 4), trunc_flag);     // sloc before and including "then"
                result->exec_lines[LOG]++;

                size_t found_else = str.find("else");
                result->addSLOC(strLSLOCBak.substr(found_then + 4, found_else- found_then -4), trunc_flag);  // sloc past "then" and before "else"
                result->exec_lines[LOG]++;

                strLSLOC = strLSLOCBak = "";
                cont_str = false;
                inlineStatement = 1;
            }
        }
    }

    // there may be more than 1 logical SLOC in this line
    while (i < line.length())
    {
        switch (line[i])
        {
        case '&': case '\n': // LSLOC terminators
		{
            if (openBrackets > 0)
            {
                i++;
                continue;
            }
            tmp = CUtil::TrimString(line.substr(start, i - start + 1));
            tmpBak = CUtil::TrimString(lineBak.substr(start, i - start + 1));

            if (cont_str && strLSLOC.length() > 0)
            {
                // check for string continuation
                if (tmp[0] == '$')
                {
                    tmp = tmp.substr(1, tmp.length() - 1);
                    tmpBak = tmpBak.substr(1, tmpBak.length() - 1);
                }
            }
            strSize = CUtil::TruncateLine(tmp.length(), strLSLOC.length(), this->lsloc_truncate, trunc_flag);
            if (strSize > 0)
            {
                strLSLOC += tmp.substr(0, strSize);
                strLSLOCBak += tmpBak.substr(0, strSize);
            }

		    std::size_t found = strLSLOCBak.find("&");

		    if (found!=std::string::npos)
		    {
				  result->addSLOC(strLSLOCBak.substr(0, found), trunc_flag);     // sloc before "&"
				  result->exec_lines[LOG]++;
		    }
			else 
			{
				result->addSLOC(strLSLOCBak, trunc_flag);  	  
				result->exec_lines[LOG]++;
			}

            strLSLOC = strLSLOCBak = "";
            cont_str = false;
            start = i + 1;
            break;
		}
        case '[': case '(': case '{':
            openBrackets++;
            break;
        case ']': case ')': case '}':
            openBrackets--;
            break;
        }
        i++;
    }

    // check for line continuation
    tmp = CUtil::TrimString(line.substr(start, i - start));
    tmpBak = CUtil::TrimString(lineBak.substr(start, i - start));
    if (tmp.length() > 3 && tmp.substr(tmp.length() - 1, 1) == "$")
    {
        // strip off trailing ($)
        tmp = tmp.substr(0, tmp.length() - 1);
        tmpBak = tmpBak.substr(0, tmpBak.length() - 1);
        len = tmp.length();

        strSize = CUtil::TruncateLine(len, strLSLOC.length(), this->lsloc_truncate, trunc_flag);
        if (strSize > 0)
        {
            if (cont_str)
            {
                strLSLOC += CUtil::TrimString(tmp.substr(0, strSize), -1);
                strLSLOCBak += CUtil::TrimString(tmpBak.substr(0, strSize), -1);
            }
            else
            {
                strLSLOC += CUtil::TrimString(tmp.substr(0, strSize)) + " ";
                strLSLOCBak += CUtil::TrimString(tmpBak.substr(0, strSize)) + " ";
            }
        }
    }
    else if (inlineStatement == 0)
    {
        strSize = CUtil::TruncateLine(tmp.length(), strLSLOC.length(), this->lsloc_truncate, trunc_flag);
        if (strSize > 0)
        {
            strLSLOC += tmp.substr(0, strSize);
            strLSLOCBak += tmpBak.substr(0, strSize);

            result->addSLOC(strLSLOCBak, trunc_flag);
            result->exec_lines[LOG]++;
            strLSLOC = strLSLOCBak = "";
            cont_str = false;
        }
    }
	inlineStatement = 0;
 
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
int CIdlCounter::ParseFunctionName(const string &line, string &/*lastline*/,
    filemap &functionStack, string &functionName, unsigned int &functionCount)
{
    string str;
    size_t idx;
    unsigned int fcnt;

    idx = CUtil::FindKeyword(line, "pro", 0, TO_END_OF_STRING, false);
    if (idx != string::npos)
    {
        if (idx + 4 < line.length())
        {
            str = line.substr(idx + 4);
            str += ",";  // append "," so that a pro/function with no inputs is counted 
            lineElement element(++functionCount, str);
            functionStack.push_back(element);
        }
    }
    else
    {
        idx = CUtil::FindKeyword(line, "function", 0, TO_END_OF_STRING, false);
        if (idx != string::npos)
        {
            if (idx + 9 < line.length())
            {
                str = line.substr(idx + 9);
                str += ","; // append "," so that a pro/function with no inputs is counted 
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

    idx = CUtil::FindKeyword(line, "end", 0, TO_END_OF_STRING, false);
    if (idx != string::npos)
    {
        str = functionStack.back().line;
        fcnt = functionStack.back().lineNumber;
        functionStack.pop_back();
        idx = str.find(",");
        if (idx != string::npos)
        {
            functionName = CUtil::ClearRedundantSpaces(str.substr(0, idx));
            functionCount = fcnt;
            return 1;
        }
    }
    return 0;
}


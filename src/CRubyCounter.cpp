//! Code counter class methods for the Ruby language.
/*!
* \file CRubyCounter.cpp
*
* This file contains the code counter class methods for the Ruby language.
*/

#include "CRubyCounter.h"

/*!
* Constructs a CRubyCounter object.
*/
CRubyCounter::CRubyCounter()
{
	classtype = RUBY;
	language_name = "Ruby";
    
    //Modification: 11.2016 Ext-4
    file_extension = CUtil::getExtensionsToLanguage("Ruby", file_extension);
	//file_extension.push_back(".rb");

	QuoteStart = "\"'%/<";
	QuoteEnd = "\"'/";
	QuoteEscapeFront = '\\';
	ContinueLine = ".,\\+-*/";

	BlockCommentStart.push_back("=begin");
	BlockCommentEnd.push_back("=end");

	LineCommentStart.push_back("#");
	delimiter = "";
	
	exec_name_list.push_back("alias");
	exec_name_list.push_back("begin");
	exec_name_list.push_back("break");
	exec_name_list.push_back("case");
	exec_name_list.push_back("catch");
	exec_name_list.push_back("collect");
	exec_name_list.push_back("continue");
	exec_name_list.push_back("default");
	exec_name_list.push_back("die");
	exec_name_list.push_back("do");
	exec_name_list.push_back("each");
	exec_name_list.push_back("else");
	exec_name_list.push_back("elsif");
	exec_name_list.push_back("end");
	exec_name_list.push_back("ensure");
	exec_name_list.push_back("exception");
	exec_name_list.push_back("exit");
	exec_name_list.push_back("for");
	exec_name_list.push_back("if");
	exec_name_list.push_back("module");
	exec_name_list.push_back("new");
	exec_name_list.push_back("next");
	exec_name_list.push_back("puts");
	exec_name_list.push_back("print");
	exec_name_list.push_back("redo");
	exec_name_list.push_back("rescue");
	exec_name_list.push_back("retry");
	exec_name_list.push_back("return");
	exec_name_list.push_back("switch");
	exec_name_list.push_back("throw");
	exec_name_list.push_back("try");
	exec_name_list.push_back("undef");
	exec_name_list.push_back("unless");
	exec_name_list.push_back("until");
	exec_name_list.push_back("when");
	exec_name_list.push_back("while");
	exec_name_list.push_back("yield");

	math_func_list.push_back("atan2");
	math_func_list.push_back("cos");
	math_func_list.push_back("exp");
	math_func_list.push_back("frexp");
	math_func_list.push_back("ldexp");
	math_func_list.push_back("rand");
	math_func_list.push_back("sin");
	math_func_list.push_back("sqrt");
	math_func_list.push_back("srand");
	math_func_list.push_back("tan");

	log_func_list.push_back("log");
	log_func_list.push_back("log10");

	cmplx_calc_list.push_back("%");
	cmplx_calc_list.push_back("+");
	cmplx_calc_list.push_back("-");
	cmplx_calc_list.push_back("*");
	cmplx_calc_list.push_back("/");
	cmplx_calc_list.push_back("**");

	cmplx_cond_list.push_back("begin");
	cmplx_cond_list.push_back("case");
	cmplx_cond_list.push_back("else");
	cmplx_cond_list.push_back("elsif");
	cmplx_cond_list.push_back("ensure");
	cmplx_cond_list.push_back("for");
	cmplx_cond_list.push_back("if");
	cmplx_cond_list.push_back("rescue");
	cmplx_cond_list.push_back("unless");
	cmplx_cond_list.push_back("until");
	cmplx_cond_list.push_back("when");
	cmplx_cond_list.push_back("while");
	
	cmplx_logic_list.push_back("&&");
	cmplx_logic_list.push_back("||");
	cmplx_logic_list.push_back("==");
	cmplx_logic_list.push_back("<=>");
	cmplx_logic_list.push_back("!");
	cmplx_logic_list.push_back("and");
	cmplx_logic_list.push_back("not");
	cmplx_logic_list.push_back("or");
	cmplx_logic_list.push_back("~");
	cmplx_logic_list.push_back(">");
	cmplx_logic_list.push_back("<");
	cmplx_logic_list.push_back(">=");
	cmplx_logic_list.push_back("=<");
	cmplx_logic_list.push_back("===");

	cmplx_assign_list.push_back("=");
	cmplx_assign_list.push_back("+=");
	cmplx_assign_list.push_back("-=");
	cmplx_assign_list.push_back("*=");
	cmplx_assign_list.push_back("/=");
	cmplx_assign_list.push_back("%=");
	cmplx_assign_list.push_back("**=");

	/* Adding Complexity count in the language */
        
	cmplx_cyclomatic_list.push_back("if");
	cmplx_cyclomatic_list.push_back("elsif");
	cmplx_cyclomatic_list.push_back("while");
	cmplx_cyclomatic_list.push_back("when");
	cmplx_cyclomatic_list.push_back("for");
	cmplx_cyclomatic_list.push_back("unless");
	cmplx_cyclomatic_list.push_back("until");
	cmplx_cyclomatic_list.push_back("each");
	cmplx_cyclomatic_list.push_back("rescue");

	cmplx_cyclomatic_logic_list.push_back("&&");
	cmplx_cyclomatic_logic_list.push_back("||");
	cmplx_cyclomatic_logic_list.push_back("and");
	cmplx_cyclomatic_logic_list.push_back("or");

	cmplx_cyclomatic_case_list.push_back("when");
	cmplx_cyclomatic_switch_list.push_back("case");
}

/*!
* Replaces quoted strings inside a string starting at idx_start with '$'.
* Handles special cases for Ruby string literals.
*
* \param strline string to be processed
* \param idx_start index of line character to start search
* \param contd specifies the quote string is continued from the previous line
* \param CurrentQuoteEnd end quote character of the current status
*
* \return method status
*/
int CRubyCounter::ReplaceQuote(string &strline, size_t &idx_start, bool &contd, char &CurrentQuoteEnd) 
{
	//%DStrD  # multiple lines allowed
	//%QDStrD  # multiple lines allowed
	//%qDStrD  # multiple lines allowed
	//<<DDStrDD  # multiple lines allowed

	// RegEx with /
	// RegEx with %rDRegExD
	const string heredoc_deli = "\"\'-ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$";
	size_t idx;	// = string::npos;
	const string openParentheses = "([{<";
	const string closedParentheses = ")]}>";

	//( [ { < have to be closed with ) ] } >
	// blank is also used as a delimiter for string literal started with %
	size_t i = 0, startpos = 0, endpos = string::npos;
	bool foundQuote = false;
	size_t strlen = strline.length();
	unsigned int paren = 1;

	if (contd)
	{
		string tstrline = CUtil::TrimString(strline, 1);
		if ((tstrline.length() == 0 && delimiter == "\n") ||	// end of string literal with the new line as delimiter, e.g., %<NL>
			delimiter == tstrline)
		{
			// end of here document
			for (i = 0; i < ContinueLine.length(); i++)
			{
				if (delimiter[0] == ContinueLine[i])
				{
					// avoid continuation
					strline = "$";
					break;
				}
			}
			contd = false;
			delimiter = "";
			idx_start = strline.length();
			return 0;
		}
		else
		{
			// replace the whole line
			strline.replace(idx_start, strlen - idx_start, strlen - idx_start, '$');
			if (strline.length() > 0)
				strline[strline.length() - 1] = ContinueLine[0];	// continue the line
			else
				strline = string(1, ContinueLine[0]);
			return 0;
		}
	} 
	else
	{
		while (i < strlen && !contd)
		{
			switch (strline[i])
			{
			case '%':
				if (!foundQuote)
				{
					foundQuote = true;
					startpos = i;
					if (i + 1 < strlen)
					{
						if (strline[i + 1] == 'Q' ||
							strline[i + 1] == 'q' ||
							strline[i + 1] == 'r' ||
							strline[i + 1] == 's' ||
							strline[i + 1] == 'W' ||
							strline[i + 1] == 'w' ||
							strline[i + 1] == 'x')
							i++;
					} 
					if (i + 1 == strlen)
					{
						contd = true;	// continued in the next line
						idx_start = i;
						delimiter = "\n";
						strline[i] = ContinueLine[0];
						return 0;
					}
					else
					{
						delimiter = string(1, strline[i + 1]);
						paren = 1;
					}
					i++;
				}					
				break;
			case '<':
				if (!foundQuote)
				{
					if (i + 2 < strlen && strline[i + 1] == '<' && strline[i + 2] != ' ' && strline[i + 2] != '\t')
					{
						// here document
						// foundQuote = true;
						// get the last delimiter specifying the end of the 'here document'

						// find the last delimiter, check for the last delimiter word not in a comment
						idx = strline.find_last_of("#");
						if (idx == string::npos)
							idx = strlen;
						idx = idx - 1;
						string tstrline = CUtil::TrimString(strline.substr(i + 2, idx + 1 - i - 2));
						
						strline[idx] = ContinueLine[0]; // indicate that the string will continue in the next line, so one SLOC is counted
						idx_start = idx + 1;

						idx = tstrline.find_last_of("<<"); // another here-doc delimiter?

						// if yes, get the last delimiter
						if (idx != string::npos)
							tstrline = tstrline.substr(idx + 1, tstrline.length());

						tstrline = CUtil::TrimString(tstrline);

						size_t i1 = 0;
						if (tstrline.length() > 2 && (tstrline.substr(0, 2) == "-\"" || tstrline.substr(0, 2) == "-'"))
						{
							i1 = 2;
							char qChar = tstrline[1];
							while (i1 < tstrline.length() && tstrline[i1] != qChar)
								i1++;
						}
						else
						{
							while (i1 < tstrline.length() && heredoc_deli.find(tstrline[i1]) != string::npos)
								i1++;
						}

						delimiter = tstrline.substr(0, i1);
						delimiter = CUtil::TrimString(delimiter);

						delimiter = CUtil::EraseString(delimiter, "\"");
						delimiter = CUtil::EraseString(delimiter, "\'");
						delimiter = CUtil::EraseString(delimiter, "-");

						contd = true;
						return 0;
					}
					else
					{
						idx_start = strline.length();
						return 1;
					}
				}
				break;
			case '/':
				if (!foundQuote)
				{
					delimiter = "/"; 
					foundQuote = true; 
					startpos = i;
				}
				else
				{
					strline.replace(startpos, i - startpos + 1, i - startpos + 1, '$');
					delimiter = "";
					foundQuote = false; 
					idx_start = i + 1;
				}
				break;
			default:
				if (foundQuote)
				{
					if (delimiter.length() == 1 && delimiter[0] == strline[i] &&
						openParentheses.find(delimiter) != string::npos)
					{
						paren++;
					}
					
					// check if the char is a closing bracket
					if ((idx = closedParentheses.find(strline[i])) != string::npos)
					{
						if (delimiter.length() > 0 && openParentheses[idx] == delimiter[0] && paren > 0)
							paren--;
						if (paren == 0)
						{
							// here we go, replace string starting from startpos?
							endpos = i;
						}
					}
					else
					{
						if (delimiter.length() > 0 && strline[i] == delimiter[0] && paren == 1)
						{
							// here we go, replace string starting from startpos
							endpos = i;
						}
					}

					if (endpos != string::npos)
					{
						foundQuote = false;
						// now, replace string from startpos to endpos
						strline.replace(startpos, endpos - startpos + 1, endpos - startpos + 1, '$');
						contd = false;
						delimiter = "";
						endpos = string::npos;
						idx_start = i + 1;
					}
				}					
				break;
			}
			i++;
			if (i > idx_start && !foundQuote)
				break;
		}
	}
	int ret = CCodeCounter::ReplaceQuote(strline, idx_start, contd, CurrentQuoteEnd);
	if (contd && !foundQuote)
	{
		strline[strline.length() - 1] = ContinueLine[0];
		if (delimiter == "")
			delimiter = CurrentQuoteEnd;
	}
	return ret;
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
int CRubyCounter::LanguageSpecificProcess(filemap* fmap, results* result, filemap* fmapBak)
{
	string strLSLOC    = "";
	string strLSLOCBak = "";

	filemap::iterator fit, fitbak;
	string line, lineBak;
	StringVector loopLevel;

	unsigned int cnt = 0;
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$";
	delimiter = "";

	for (fit = fmap->begin(), fitbak = fmapBak->begin(); fit != fmap->end(); fit++, fitbak++)
	{
		line = fit->line;
		lineBak = fitbak->line;
	
		// do not process blank lines
		// blank line means blank_line/comment_line/directive
		if (!CUtil::CheckBlank(line))
		{
			LSLOC(result, line, fit->lineNumber, lineBak, strLSLOC, strLSLOCBak);

			if (print_cmplx)
			{
				cnt = 0;
				CUtil::CountTally(" " + line, exec_name_list, cnt, 1, exclude, "", "", &result->exec_name_count);
			}

			// no data declaration in Ruby
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
*/
void CRubyCounter::LSLOC(results* result, string line, size_t lineNumber, string lineBak, string &strLSLOC, string &strLSLOCBak)
{
	const string control_modifiers[] = {"if", "for", "unless", "while", "until", "when", "close", "elsif", "else", "then"};
	static size_t control_mod_cnt = 10;
	size_t start = 0; // starting index of the working string
	size_t i, j, strSize = string::npos;
	size_t idx = string::npos;
	bool trunc_flag = false;

	string tmp = CUtil::TrimString(strLSLOC);
	string tline = CUtil::TrimString(line);
	bool line_skipped = false;

	if (tline == "end" || tline == "else" || tline == "}" || tline == "]")
	{
		if (tmp.length() > 0)
			line_skipped = true;
		else
			return;
	}

	while (start < line.length())
	{
		strSize = string::npos;

		// get first control modifier
		idx = line.find(';', start);
		i = line.find(':', start);
		if (i != string::npos && (idx == string::npos || i < idx))
		{
			// avoid processing :: and splitting ternary operator x ? y : z
			if (i >= line.length() - 1 || line[i + 1] != ':')
			{
				for (j = i - 1; j > start; j--)
				{
					if (line[j] == '?')
						break;
				}
				if (j <= start)
					idx = i;
			}
		}
		for (j = 0; j < control_mod_cnt; j++)
		{
			i = CUtil::FindKeyword(line, control_modifiers[j], start);
			if (i != string::npos && (idx == string::npos || i < idx))
			{
				if (CUtil::FindKeyword(tline, control_modifiers[j]) != 0)
					idx = i;
			}
		}

		// process modifier
		if (idx != string::npos)
		{
			if (line[idx] == ';')
			{
				strSize = CUtil::TruncateLine(idx + 1 - start, strLSLOC.length(), this->lsloc_truncate, trunc_flag);
				if (strSize > 1 && CUtil::TrimString(line.substr(start, strSize - 1)) != ";")
				{
					// only include ';' if it stands alone
					strSize--;
				}
				idx++;
			}
			else if (line[idx] == ':')
			{
				strSize = CUtil::TruncateLine(idx + 1 - start, strLSLOC.length(), this->lsloc_truncate, trunc_flag);
				idx++;
			}
			else if (line.length() >= idx + 4 && line.substr(idx, 4) == "then")
			{
				idx += 4;
				strSize = CUtil::TruncateLine(idx - start, strLSLOC.length(), this->lsloc_truncate, trunc_flag);
			}
			else
				strSize = CUtil::TruncateLine(idx - start, strLSLOC.length(), this->lsloc_truncate, trunc_flag);
		}

		if (strSize > 0 && strSize != string::npos)	// only if (idx != 0 && idx != string::npos) returns true
		{
			strLSLOC += line.substr(start, strSize);
			strLSLOCBak += lineBak.substr(start, strSize);
		}
		else if (!line_skipped)
		{
			strSize = CUtil::TruncateLine(line.length() - start, strLSLOC.length(), this->lsloc_truncate, trunc_flag);
			if (strSize > 0)
			{
				strLSLOC += line.substr(start, strSize);
				strLSLOCBak += lineBak.substr(start, strSize);
			}

			if (tline.length() > 0)
			{
				for (i = 0; i < ContinueLine.length(); i++)
				{
					if (tline.find_last_of(ContinueLine[i]) == tline.length() - 1)
					{
						// continued to the next line
						if (tline[tline.length() - 1] == '\\')
						{
							i = strLSLOC.find_last_of('\\');
							if (i != string::npos)
							{
								strLSLOC[i] = ' ';
								strLSLOCBak[i] = ' ';
							}
						}
						return;
					}
				}
			}
		}
		if (CUtil::TrimString(strLSLOC).length() == 0)
			return;

		// add SLOC
		if (result->addSLOC(strLSLOCBak, lineNumber, trunc_flag))
			result->exec_lines[LOG]++;
		strLSLOC = strLSLOCBak = "";

		if (idx != 0 && idx != string::npos)
		{
			start = idx;
			tline = CUtil::TrimString(line.substr(start));
		}
		else
			start = string::npos;
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
int CRubyCounter::ParseFunctionName(const string &line, string &/*lastline*/,
		filemap &functionStack, string &functionName, unsigned int &functionCount)
{
        string str;
        size_t idx;
	unsigned int fcnt;

        string end_marker = "end";
        size_t block_keyword_idx;
        string block_keywords[] = {"begin", "case", "if", "do", "for", "unless", "until"};
        static size_t end_marker_count;
        /* 
         * Checking for beginning of function
         */
        idx = CUtil::FindKeyword(line, "def");
        if (idx != string::npos)
        {
                if (idx + 4 < line.length())
                {
                        str = line.substr(idx + 4);
                        str = str.substr(0, CUtil::FindKeyword(str, ":"));
			lineElement element(++functionCount, str);
                        functionStack.push_back(element);
                }
        }
        else
        {
            	idx = CUtil::FindKeyword(line, "class");
            	if (idx != string::npos)
            	{
                    	if (idx + 6 < line.length())
                    	{
                        	str = line.substr(idx + 4);
                        	str = str.substr(0, CUtil::FindKeyword(str, ":"));
				lineElement element(++functionCount, str);
                        	functionStack.push_back(element);
                    	}
        	}
        }
        /* 
         * Checking for other keywords
         * That also end with block so that function does not get pooped prematurely
         */
        for (block_keyword_idx = 0; block_keyword_idx < 7; block_keyword_idx ++)
        {
            idx = CUtil::FindKeyword(line, block_keywords[block_keyword_idx]);
            if (idx != string::npos)
            {
               end_marker_count += 1;
            }
        }
        if (functionStack.empty())
        {
                return 2;
        }
        idx= CUtil::FindKeyword(line, "end");
        if (idx != string::npos)
        {
                /*
                 * This was not a function but a inside block
                 */
                if (end_marker_count > 0)
                {
                        end_marker_count -= 1;
                        if (functionStack.empty())
                        {
                            return 2;
                        }
                }
                else
                {
			str = functionStack.back().line;
			fcnt = functionStack.back().lineNumber;
                    	functionStack.pop_back();
		        functionName = CUtil::ClearRedundantSpaces(str);
			functionCount = fcnt;
		        return 1;
                }
        }
        return 0;
}

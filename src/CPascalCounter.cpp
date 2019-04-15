//! Code counter class methods for the Pascal language.
/*!
* \file CPascalCounter.cpp
*
* This file contains the code counter class methods for the Pascal language.
*/

#include "CPascalCounter.h"

/*!
* Constructs a CPascalCounter object.
*/
CPascalCounter::CPascalCounter()
{
	classtype = PASCAL;
	language_name = "Pascal";
	casesensitive = false;

	QuoteStart = "'";
	QuoteEnd = "'";
	QuoteEscapeFront = '\'';
	BlockCommentStart.push_back("(*");
	BlockCommentEnd.push_back("*)");
	BlockCommentStart.push_back("{");
	BlockCommentEnd.push_back("}");
	LineCommentStart.push_back("//");
    
    //Modification: 11.2016 Ext-4
    file_extension = CUtil::getExtensionsToLanguage("Pascal", file_extension);
    /*
	file_extension.push_back(".pas");
	file_extension.push_back(".p");
	file_extension.push_back(".pp");
	file_extension.push_back(".pa3");
	file_extension.push_back(".pa4");
	file_extension.push_back(".pa5");*/

	data_name_list.push_back("ansistring");
	data_name_list.push_back("array");
	data_name_list.push_back("boolean");
	data_name_list.push_back("byte");
	data_name_list.push_back("bytebool");
	data_name_list.push_back("cardinal");
	data_name_list.push_back("char");
	data_name_list.push_back("class");
	data_name_list.push_back("comp");
	data_name_list.push_back("complex");
	data_name_list.push_back("const");
	data_name_list.push_back("double");
	data_name_list.push_back("extended");
	data_name_list.push_back("file");
	data_name_list.push_back("integer");
	data_name_list.push_back("interface");
	data_name_list.push_back("int64");
	data_name_list.push_back("longbool");
	data_name_list.push_back("longint");
	data_name_list.push_back("longword");
	data_name_list.push_back("object");
	data_name_list.push_back("pchar");
	data_name_list.push_back("qword");
	data_name_list.push_back("real");
	data_name_list.push_back("record");
	data_name_list.push_back("set");
	data_name_list.push_back("shortint");
	data_name_list.push_back("shortstring");
	data_name_list.push_back("single");
	data_name_list.push_back("smallint");
	data_name_list.push_back("string");
	data_name_list.push_back("type");
	data_name_list.push_back("widestring");
	data_name_list.push_back("word");
	data_name_list.push_back("wordbool");

	exec_name_list.push_back("absolute");
	exec_name_list.push_back("assembler");
	exec_name_list.push_back("case");
	exec_name_list.push_back("const");
	exec_name_list.push_back("constructor");
	exec_name_list.push_back("destructor");
	exec_name_list.push_back("dispose");
	exec_name_list.push_back("downto");
	exec_name_list.push_back("else");
	exec_name_list.push_back("exit");
	exec_name_list.push_back("far");
	exec_name_list.push_back("for");
	exec_name_list.push_back("forward");
	exec_name_list.push_back("freemem");
	exec_name_list.push_back("function");
	exec_name_list.push_back("getmem");
	exec_name_list.push_back("goto");
	exec_name_list.push_back("if");
	exec_name_list.push_back("implementation");
	exec_name_list.push_back("inline");
	exec_name_list.push_back("interrupt");
	exec_name_list.push_back("label");
	exec_name_list.push_back("mark");
	exec_name_list.push_back("near");
	exec_name_list.push_back("new");
	exec_name_list.push_back("nil");
	exec_name_list.push_back("packed");
	exec_name_list.push_back("private");
	exec_name_list.push_back("procedure");
	exec_name_list.push_back("program");
	exec_name_list.push_back("protected");
	exec_name_list.push_back("public");
	exec_name_list.push_back("repeat");
	exec_name_list.push_back("unit");
	exec_name_list.push_back("uses");
	exec_name_list.push_back("var");
	exec_name_list.push_back("virtual");
	exec_name_list.push_back("while");
	exec_name_list.push_back("with");

	math_func_list.push_back("abs");
	math_func_list.push_back("arg");
	math_func_list.push_back("cmplx");
	math_func_list.push_back("dec");
	math_func_list.push_back("exp");
	math_func_list.push_back("im");
	math_func_list.push_back("inc");
	math_func_list.push_back("min");
	math_func_list.push_back("max");
	math_func_list.push_back("polar");
	math_func_list.push_back("pow");
	math_func_list.push_back("re");
	math_func_list.push_back("round");
	math_func_list.push_back("sqr");
	math_func_list.push_back("sqrt");

	trig_func_list.push_back("cos");
	trig_func_list.push_back("sin");
	trig_func_list.push_back("arccos");
	trig_func_list.push_back("arcsin");
	trig_func_list.push_back("arctan");

	log_func_list.push_back("ln");

	cmplx_calc_list.push_back("+");
	cmplx_calc_list.push_back("-");
	cmplx_calc_list.push_back("*");
	cmplx_calc_list.push_back("/");
	cmplx_calc_list.push_back("**");
	cmplx_calc_list.push_back("div");
	cmplx_calc_list.push_back("mod");

	cmplx_cond_list.push_back("case");
	cmplx_cond_list.push_back("else");
	cmplx_cond_list.push_back("for");
	cmplx_cond_list.push_back("if");
	cmplx_cond_list.push_back("repeat");
	cmplx_cond_list.push_back("while");
	cmplx_cond_list.push_back("with");

	cmplx_logic_list.push_back("=");
	cmplx_logic_list.push_back("<>"); 
	cmplx_logic_list.push_back(">");
	cmplx_logic_list.push_back("<");
	cmplx_logic_list.push_back(">=");
	cmplx_logic_list.push_back("=<");
	cmplx_logic_list.push_back("not");
	cmplx_logic_list.push_back("and");
	cmplx_logic_list.push_back("or");
	cmplx_logic_list.push_back("xor");
	cmplx_logic_list.push_back("shl");
	cmplx_logic_list.push_back("shr");

	cmplx_assign_list.push_back(":=");

	cmplx_pointer_list.push_back("^");

	//author: Anthony Zhu
	cmplx_cyclomatic_list.push_back("if");
	cmplx_cyclomatic_list.push_back("for");
	cmplx_cyclomatic_list.push_back("while");
	cmplx_cyclomatic_list.push_back("until");
	cmplx_cyclomatic_list.push_back("except");

	cmplx_cyclomatic_logic_list.push_back("and");
	cmplx_cyclomatic_logic_list.push_back("or");
	//cmplx_cyclomatic_logic_list.push_back("xor");

	cmplx_cyclomatic_case_list.push_back("case");
	//cmplx_cyclomatic_list.push_back("case"); special case, need special handler
}

/*!
* Counts the number of comment lines, removes comments, and
* replaces quoted strings by special chars, e.g., $
* All arguments are modified by the method.
* Since Pascal compiler directives are block comments starting with '$'
* this method also captures directive SLOC.
*
* \param fmap list of processed file lines
* \param result counter results
* \param fmapBak list of original file lines (same as fmap except it contains unmodified quoted strings)
*
* \return method status
*/
int CPascalCounter::CountCommentsSLOC(filemap* fmap, results* result, filemap *fmapBak)
{
	if (BlockCommentStart.empty() && LineCommentStart.empty())
		return 0;
	if (classtype == DATAFILE)
		return 0;

	bool contd = false;
	bool contd_nextline;
	int comment_type = 0;
	/*
	comment_type:
	0 : not a comment
	1 : line comment, whole line
	2 : line comment, embedded
	3 : block comment, undecided
	4 : block comment, embedded
	*/

	size_t idx_start, idx_end, comment_start;
	size_t quote_idx_start;
	string curBlckCmtStart, curBlckCmtEnd;
	char CurrentQuoteEnd = 0;
	bool quote_contd = false;
	filemap::iterator itfmBak = fmapBak->begin();
	
	string strDirLine;
	size_t strSize;
	bool isDirective = false, trunc_flag = false;

	quote_idx_start = 0;

	for (filemap::iterator iter = fmap->begin(); iter != fmap->end(); iter++, itfmBak++)
	{
		contd_nextline = false;

		quote_idx_start = 0;
		idx_start = 0;

		if (CUtil::CheckBlank(iter->line))
			continue;
		size_t lineNumber = iter->lineNumber;

		if (quote_contd)
		{
			// Replace quote until next character
			ReplaceQuote(iter->line, quote_idx_start, quote_contd, CurrentQuoteEnd);
			if (quote_contd)
				continue;
		}
		if (contd)
			comment_type = 3;

		while (!contd_nextline && idx_start < iter->line.length())
		{
			// need to handle multiple quote chars in some languages, both " and ' may be accepted
			quote_idx_start = FindQuote(iter->line, QuoteStart, quote_idx_start, QuoteEscapeFront);
			comment_start = idx_start;
			if (!contd)
				FindCommentStart(iter->line, comment_start, comment_type, curBlckCmtStart, curBlckCmtEnd);

			if (comment_start == string::npos && quote_idx_start == string::npos)
				break;

			if (comment_start != string::npos)
				idx_start = comment_start;

			// if found quote before comment, e.g., "this is quote");//comment
			if (quote_idx_start != string::npos && (comment_start == string::npos || quote_idx_start < comment_start))
			{
				ReplaceQuote(iter->line, quote_idx_start, quote_contd, CurrentQuoteEnd);
				if (quote_idx_start > idx_start)
				{
					// comment delimiter inside quote
					idx_start = quote_idx_start;
					continue;
				}
			}
			else if (comment_start != string::npos)
			{
				// comment delimiter starts first
				switch (comment_type)
				{
				case 1:	// line comment, definitely whole line
					iter->line = "";
					itfmBak->line = "";
					result->comment_lines++;
					contd_nextline = true;
					break;
				case 2:	// line comment, possibly embedded
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
				case 3:	// block comment
				case 4:
					if (contd)
						idx_end = iter->line.find(curBlckCmtEnd);
					else
					{
						idx_end = iter->line.find(curBlckCmtEnd, idx_start + curBlckCmtStart.length());

						// check whether comment is a directive (starts with '$')
						isDirective = false;
						for (size_t i = 0; i < BlockCommentStart.size(); i++)
						{
							if (iter->line.substr(idx_start, BlockCommentStart[i].length() + 1) == BlockCommentStart[i] + "$")
							{
								strDirLine = "";
								isDirective = true;
								break;
							}
						}
					}

					if (idx_end == string::npos)
					{
						if (comment_type == 3)
						{
							if (isDirective)
							{
								strSize = CUtil::TruncateLine(itfmBak->line.length(), strDirLine.length(), this->lsloc_truncate, trunc_flag);
								if (strSize > 0)
								{
									if (contd)
										strDirLine += "\n" + itfmBak->line.substr(0, strSize);
									else
										strDirLine = itfmBak->line.substr(0, strSize);
								}
								result->directive_lines[PHY]++;
							}
							else
								result->comment_lines++;
							iter->line = "";
							itfmBak->line = "";
						}
						else if (comment_type == 4)
						{
							if (isDirective)
							{
								strSize = CUtil::TruncateLine(itfmBak->line.length() - idx_start, 0, this->lsloc_truncate, trunc_flag);
								if (strSize > 0)
									strDirLine = itfmBak->line.substr(idx_start, strSize);
								result->directive_lines[PHY]++;
							}
							iter->line = iter->line.substr(0, idx_start);
							itfmBak->line = itfmBak->line.substr(0, idx_start);
							// trim trailing space
							iter->line = CUtil::TrimString(iter->line, 1);
							itfmBak->line = CUtil::TrimString(itfmBak->line, 1);
							if (!isDirective)
							{
								if (iter->line.empty())
									result->comment_lines++;	// whole line
								else
									result->e_comm_lines++;		// embedded
							}
						}
						contd = true;
						contd_nextline = true;
						break;
					}
					else
					{
						if (isDirective)
						{
							strSize = CUtil::TruncateLine(idx_end - idx_start + curBlckCmtEnd.length(), strDirLine.length(), this->lsloc_truncate, trunc_flag);
							if (strSize > 0)
							{
								if (contd)
									strDirLine += "\n" + itfmBak->line.substr(idx_start, strSize);
								else
									strDirLine = itfmBak->line.substr(idx_start, strSize);
							}
							result->directive_lines[PHY]++;
						}
						contd = false;
						iter->line.erase(idx_start, idx_end - idx_start + curBlckCmtEnd.length());
						itfmBak->line.erase(idx_start, idx_end - idx_start + curBlckCmtEnd.length());
						if (isDirective)
						{
							if (result->addSLOC(strDirLine, lineNumber, trunc_flag))
								result->directive_lines[LOG]++;
							strDirLine = "";
						}
						else
						{
							if (iter->line.empty())
								result->comment_lines++;
							else
							{
								// trim trailing space
								iter->line = CUtil::TrimString(iter->line, 1);
								itfmBak->line = CUtil::TrimString(itfmBak->line, 1);
								if (iter->line.empty())
									result->comment_lines++;	// whole line
								else
									result->e_comm_lines++;		// embedded
							}
						}

						// quote chars found may be erased as it is inside comment
						quote_idx_start = idx_start;
					}
					break;
				default:
					cout << "Error in CountCommentsSLOC()" << endl;
					break;
				}
			}
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
int CPascalCounter::LanguageSpecificProcess(filemap* fmap, results* result, filemap* fmapBak)
{
	bool		found_block			= false;
	bool		found_forifwhile	= false;
	bool		found_end			= false;
	bool		found_loop			= false;
	string		strLSLOC			= "";
	string		strLSLOCBak			= "";

	filemap::iterator fit, fitbak;
	string line, lineBak;
	StringVector loopLevel;
	unsigned int cnt = 0;
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$";

	for (fit = fmap->begin(), fitbak = fmapBak->begin(); fit != fmap->end(); fit++, fitbak++)
	{
		// insert blank at the beginning (for searching keywords)
		line = ' ' + fit->line;
		lineBak = ' ' + fitbak->line;
	
		// do not process blank lines
		// blank line means blank_line/comment_line/directive
		if (!CUtil::CheckBlank(line))
		{
			// blank line means blank_line/comment_line/directive
			// call SLOC function to detect logical SLOC and add to result
			LSLOC(result, line, fit->lineNumber, lineBak, strLSLOC, strLSLOCBak, found_block,
				found_forifwhile, found_end, found_loop, loopLevel);

			cnt = 0;
			CUtil::CountTally(line, data_name_list, cnt, 1, exclude, "", "", NULL, false);

			// need to check also if the data line continues
			if (cnt > 0)
				result->data_lines[PHY] += 1;
			else
				result->exec_lines[PHY] += 1;

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
* \param found_block found block flag
* \param found_forifwhile found for, if, or while flag
* \param found_end found end flag
* \param found_loop found loop flag
* \param loopLevel nested loop level
*/
void CPascalCounter::LSLOC(results* result, string line, size_t lineNumber, string lineBak, string &strLSLOC, string &strLSLOCBak, bool &found_block,
						   bool &found_forifwhile, bool &found_end, bool &found_loop, StringVector &loopLevel)
{
	size_t start = 0; // starting index of the working string
	size_t i, tempi, strSize;
	string templine = CUtil::TrimString(line);
	string tmp;
	bool trunc_flag = false;
	unsigned int loopCnt;
	StringVector::iterator lit;
	string keywordchars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$";

	// there may be more than 1 logical SLOC in a line
	found_end = false;
	for (i = 0; i < line.length(); i++)
	{
		if (line[i] == ';')
		{
			if (!found_end)
			{
				strSize = CUtil::TruncateLine(i - start, strLSLOC.length(), this->lsloc_truncate, trunc_flag);
				if (strSize > 0)
				{
					strLSLOC += line.substr(start, strSize);
					strLSLOCBak += lineBak.substr(start, strSize);
				}
				tmp = strLSLOC;
				FoundSLOC(result, lineNumber, strLSLOC, strLSLOCBak,
					found_block, found_forifwhile, found_end, trunc_flag);

				// record end loop for nested loop processing
				if (print_cmplx)
				{
					if (found_loop)
					{
						found_loop = false;
						loopLevel.push_back("do");

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

						loopLevel.pop_back();
					}
					else if (CUtil::FindKeyword(tmp, "end", 0, TO_END_OF_STRING, false) != string::npos ||
						CUtil::FindKeyword(tmp, "until", 0, TO_END_OF_STRING, false) != string::npos)
					{
						if (loopLevel.size() > 0)
							loopLevel.pop_back();
					}
				}
			}
			else
			{
				if (strLSLOC.size() > 0)
				{
					FoundSLOC(result, lineNumber, strLSLOC, strLSLOCBak,
						found_block, found_forifwhile, found_end, trunc_flag);
				}
				found_end = false;	// end xxx
				found_block = false;
				found_forifwhile = false;
				strLSLOC = "";
				strLSLOCBak = "";
			}
			found_loop = false;
			start = i + 1;
		}

		// if it ends in xxx, then it has already been counted, so ignore it
		tmp = "xxx " + CUtil::TrimString(line.substr(start, i + 1 - start));
		if (CUtil::FindKeyword(tmp, "end", 0, TO_END_OF_STRING, false) != string::npos)
		{
			// check for 'end,' and skip
			if ((line.length() > i + 1 && line[i + 1] == ',') || line[i] == ',')
				continue;

			found_end = true;
			found_block = false;
			found_loop = false;

			// capture SLOC
			strSize = CUtil::TruncateLine(i - 2 - start, strLSLOC.length(), this->lsloc_truncate, trunc_flag);
			if (strSize > 0)
			{
				strLSLOC += line.substr(start, strSize);
				strLSLOCBak += lineBak.substr(start, strSize);
			}
			FoundSLOC(result, lineNumber, strLSLOC, strLSLOCBak,
				found_block, found_forifwhile, found_end, trunc_flag);

			// if end is followed by a period 'end.' record SLOC if any
			if (line.length() > i + 1 && line[i + 1] == '.')
			{
				// record end loop for nested loop processing
				if (print_cmplx)
				{
					while (loopLevel.size() > 0)
						loopLevel.pop_back();
				}
				start = i + 2;
				continue;
			}
			else
			{
				// record end loop for nested loop processing
				if (print_cmplx)
				{
					if (loopLevel.size() > 0)
						loopLevel.pop_back();
				}
				start = i + 1;
			}
		}

		// continue the following processing only if line[i] is not in a middle of a word
		if (keywordchars.find(line[i]) != string::npos && i < line.length() - 1)
			continue;

		if (!found_end)
		{
			if (!found_forifwhile)
			{
				if (CUtil::FindKeyword(tmp, "for", 0 , TO_END_OF_STRING, false) != string::npos ||
					CUtil::FindKeyword(tmp, "while", 0, TO_END_OF_STRING, false) != string::npos ||
					CUtil::FindKeyword(tmp, "repeat", 0, TO_END_OF_STRING, false) != string::npos ||
					CUtil::FindKeyword(tmp, "with", 0, TO_END_OF_STRING, false) != string::npos ||
					CUtil::FindKeyword(tmp, "if", 0, TO_END_OF_STRING, false) != string::npos)
				{
					found_forifwhile = true;
				}

				if (CUtil::FindKeyword(tmp, "do", 0, TO_END_OF_STRING, false) != string::npos ||
					CUtil::FindKeyword(tmp, "repeat", 0, TO_END_OF_STRING, false) != string::npos)
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
						FoundSLOC(result, lineNumber, strLSLOC, strLSLOCBak,
							found_block, found_forifwhile, found_end, trunc_flag);
						start = i + 1;

						found_loop = true;
					}
					else
					{
						// record nested loop level
						if (print_cmplx)
						{
							loopLevel.push_back("repeat");

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
					continue;
				}
			}
			else if (CUtil::FindKeyword(tmp, "do", 0, TO_END_OF_STRING, false) != string::npos ||
				CUtil::FindKeyword(tmp, "then", 0, TO_END_OF_STRING, false) != string::npos)
			{
				// found a SLOC
				strSize = CUtil::TruncateLine(i + 1 - start, strLSLOC.length(), this->lsloc_truncate, trunc_flag);
				if (strSize > 0)
				{
					strLSLOC += line.substr(start, strSize);
					strLSLOCBak += lineBak.substr(start, strSize);
				}
				FoundSLOC(result, lineNumber, strLSLOC, strLSLOCBak,
					found_block, found_forifwhile, found_end, trunc_flag);
				start = i + 1;

				if (CUtil::FindKeyword(tmp, "do", 0, TO_END_OF_STRING, false) != string::npos)
					found_loop = true;

				continue;
			}

			// process else since no ';' is allowed before else
			if (CUtil::FindKeyword(tmp, "else", 0, TO_END_OF_STRING, false) != string::npos)
			{
				strSize = CUtil::TruncateLine(i - 4 - start, strLSLOC.length(), this->lsloc_truncate, trunc_flag);
				if (strSize > 0)
				{
					strLSLOC += line.substr(start, strSize);
					strLSLOCBak += lineBak.substr(start, strSize);
				}
				FoundSLOC(result, lineNumber, strLSLOC, strLSLOCBak,
					found_block, found_forifwhile, found_end, trunc_flag);
				strLSLOC = strLSLOCBak = "else ";
				start = i + 1;
				continue;
			}

			// process until since ';' is optional before else
			if (CUtil::FindKeyword(tmp, "until", 0, TO_END_OF_STRING, false) != string::npos)
			{
				strSize = CUtil::TruncateLine(i - 5 - start, strLSLOC.length(), this->lsloc_truncate, trunc_flag);
				if (strSize > 0)
				{
					strLSLOC += line.substr(start, strSize);
					strLSLOCBak += lineBak.substr(start, strSize);
				}
				FoundSLOC(result, lineNumber, strLSLOC, strLSLOCBak,
					found_block, found_forifwhile, found_end, trunc_flag);
				strLSLOC = strLSLOCBak = "until ";
				start = i + 1;
				continue;
			}

			if (!found_block)
			{
				if (CUtil::FindKeyword(tmp, "begin", 0, TO_END_OF_STRING, false) != string::npos ||
					CUtil::FindKeyword(tmp, "asm", 0, TO_END_OF_STRING, false) != string::npos ||
					CUtil::FindKeyword(tmp, "case",0, TO_END_OF_STRING, false) != string::npos)
				{
					found_block = true;

					// record nested loop level
					if (print_cmplx)
					{
						if (found_loop)
						{
							found_loop = false;
							loopLevel.push_back("do");

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
						else
							loopLevel.push_back("");
					}
				}
			}
			else
			{
				// only add new SLOC if 'of' is at the end of line and follows 'case', etc.
				tempi = CUtil::FindKeyword(templine, "of", 0, TO_END_OF_STRING, false);
				if (tempi == templine.length() - 2)
				{
					strSize = CUtil::TruncateLine(line.length() - start, strLSLOC.length(), this->lsloc_truncate, trunc_flag);
					if (strSize > 0)
					{
						strLSLOC += line.substr(start, strSize);
						strLSLOCBak += lineBak.substr(start, strSize);
					}
					FoundSLOC(result, lineNumber, strLSLOC, strLSLOCBak,
						found_block, found_forifwhile, found_end, trunc_flag);
					start = line.length();
					found_block = false;
					continue;
				}
			}

			// check for '= record'
			if (CUtil::FindKeyword(tmp, "= array", 0, TO_END_OF_STRING, false) != string::npos ||
				CUtil::FindKeyword(tmp, "= record", 0, TO_END_OF_STRING, false) != string::npos)
			{
				strSize = CUtil::TruncateLine(i + 1 - start, strLSLOC.length(), this->lsloc_truncate, trunc_flag);
				if (strSize > 0)
				{
					strLSLOC += line.substr(start, strSize);
					strLSLOCBak += lineBak.substr(start, strSize);
				}
				FoundSLOC(result, lineNumber, strLSLOC, strLSLOCBak,
					found_block, found_forifwhile, found_end, trunc_flag);
				start = i + 1;

				if (print_cmplx)
					loopLevel.push_back("");

				continue;
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
	}
	if (tmp == "")
	{
		found_forifwhile = false;
	}
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
* \param trunc_flag truncate lines?
*/
void CPascalCounter::FoundSLOC(results* result, size_t lineNumber, string &strLSLOC, string &strLSLOCBak, bool &found_block,
							   bool &found_forifwhile, bool &found_end, bool &trunc_flag)
{
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$";

	// add to the list for comparison purpose
	if (result->addSLOC(strLSLOCBak, lineNumber, trunc_flag))
	{
		// determine logical type, data declaration or executable
		unsigned int cnt = 0;
		CUtil::CountTally(strLSLOC, data_name_list, cnt, 1, exclude, "", "", &result->data_name_count, false);
		if (cnt > 0)
			result->data_lines[LOG] += 1;
		else
			result->exec_lines[LOG] += 1;

		// reset all variables whenever a new statement/logical SLOC is found
		strLSLOC = "";
		strLSLOCBak = "";
		found_block = false;
		found_forifwhile = false;
		found_end = false;
	}
}

/*!
 * Parses lines for function/procedure names.
 *
 * \param line line to be processed
 * \param lastline last line processed
 * \param functionStack stack of functions
 * \param functionName function name found
 *
 * \return 1 if function name is found
 * \return 0 if it is still in some function
 * \return 2 if the code line doesn't belong to any function
 */
int CPascalCounter::ParseFunctionName(const string &line, string &/*lastline*/,
		filemap &functionStack, string &functionName, unsigned int &functionCount)
{
	string str;
	size_t idx;
	unsigned int fcnt;

	idx = CUtil::FindKeyword(line, "procedure");
	if (idx != string::npos)
	{
		if (idx + 10 < line.length())
		{
			str = line.substr(idx + 10);
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

	idx = CUtil::FindKeyword(line, "end");
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
	return 0;
}

/*!
 * Counts file language complexity based on specified language keywords/characters.
 *
 * \param fmap list of processed file lines
 * \param result counter results
 *
 * \return method status
 */
int CPascalCounter::CountComplexity(filemap* fmap, results* result)
{
	if (classtype == UNKNOWN || classtype == DATAFILE)
		return 0;
	filemap::iterator fit;
	size_t idx;
	unsigned int cnt, ret, cyclomatic_cnt = 0, ignore_cyclomatic_cnt = 0, main_cyclomatic_cnt = 0, function_count = 0, cyclomatic_logic_cnt = 0, main_cyclomatic_logic_cnt = 1, cyclomatic_case_cnt = 0, main_cyclomatic_case_cnt = 1;
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

	//anthony begin
    	StringVector switch_case_key_list;
    	StringVector switch_case_stack;
    	switch_case_key_list.push_back(";");
    	//andthony end

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
		CUtil::CountTally(line, cmplx_preproc_list, cnt, 1, exclude, "", "", &result->cmplx_preproc_count, casesensitive);
		result->cmplx_preproc_lines += cnt;

		// assignments
		cnt = 0;
		CUtil::CountTally(line, cmplx_assign_list, cnt, 1, exclude, "", "", &result->cmplx_assign_count, casesensitive);
		result->cmplx_assign_lines += cnt;

		// pointers
		cnt = 0;
		// Pointers are embedded syntax so there is NO exclude string or include strings
		CUtil::CountTally(line, cmplx_pointer_list, cnt, 1, "", "", "", &result->cmplx_pointer_count, casesensitive);
		result->cmplx_pointer_lines += cnt;

		// cyclomatic complexity
		if (process_cyclomatic_complexity)
		{
			// search for cyclomatic complexity keywords
			unsigned int temp = 0;
			CUtil::CountTally(line, cmplx_cyclomatic_list, temp, 1, exclude, "", "", 0, casesensitive);
            		cyclomatic_cnt += temp;

			CUtil::CountTally(line, cmplx_cyclomatic_case_list, cyclomatic_case_cnt, 1, exclude, "", "", 0, casesensitive);
			cyclomatic_case_cnt += temp;

			//anthony 2013.11.5
            		//search for keyword "case"
            		size_t idx_case = CUtil::FindKeyword(line, "case");
            		if (idx_case != string::npos) {
                		switch_case_stack.push_back("case");
            		}
            
            		//only if switch_case_stack is not empty, we will search keyword "begin" and ";" and "end;"
            		if(!switch_case_stack.empty()){
                		size_t idx = CUtil::FindKeyword(line, "begin");
                		if (idx != string::npos) {
                    			switch_case_stack.push_back("begin");
                		}
                
                		//if the top of switch_case_stack is "case", then for each ";" we increment cyclomatic_cnt	
                		//if the top of switch_case_stack is "begin", we don't need to count ";" because each
                		//"begin...end " block counts one
                		if(switch_case_stack.back().compare("case") == 0){
                    			unsigned int semicol_cnt = 0;
                    			CUtil::CountTally(line, switch_case_key_list, semicol_cnt, 1, "end", "", "", 0, casesensitive);
                    			if(semicol_cnt != 0){
                       				cyclomatic_cnt++;
                    			}
                		}
                
                		//search for keyword "end;"
                		idx = CUtil::FindKeyword(line, "end;");

                		if (idx != string::npos) {
                    		//if the top of switch_case_stack is "begin", we increment cyclomatic_cnt because we get
                    		//one "begin...end" block, and pop out.
                   			if(switch_case_stack.back().compare("begin")==0){
                        			switch_case_stack.pop_back();
                        			cyclomatic_cnt++;
                    			}
                    		//otherwise, we reach the end of a "case" structure, we simply pop out.
                    			else{
                        			switch_case_stack.pop_back();
                    			}
                		}
            		}

			// search for keywords to exclude
			if (ignore_cmplx_cyclomatic_list.size() > 0)
				CUtil::CountTally(line, ignore_cmplx_cyclomatic_list, ignore_cyclomatic_cnt, 1, exclude, "", "", 0, casesensitive);

			// search for cyclomatic complexity logical keywords
			if (cmplx_cyclomatic_logic_list.size() > 0)
				CUtil::CountTally(line, cmplx_cyclomatic_logic_list, cyclomatic_logic_cnt, 1, exclude, "", "", 0, casesensitive);

			// parse function name if found
			ret = (unsigned)ParseFunctionName(line, lastline, function_stack, function_name, function_count);
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

				lineElement c_element(cyclomatic_case_cnt - ignore_cyclomatic_cnt + 1, function_name);
				case_map[function_count] = c_element;

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
			}
			else if (ret == 2)
			{
				if (main_cyclomatic_cnt < 1)
					main_cyclomatic_cnt = 1;	// add 1 for main function here in case no other decision points are found in main
				// some code doesn't belong to any function
				main_cyclomatic_cnt += cyclomatic_cnt - ignore_cyclomatic_cnt;
				main_cyclomatic_logic_cnt += cyclomatic_cnt - ignore_cyclomatic_cnt + cyclomatic_logic_cnt;
				main_cyclomatic_case_cnt += cyclomatic_case_cnt - ignore_cyclomatic_cnt;
				cyclomatic_cnt = ignore_cyclomatic_cnt = cyclomatic_logic_cnt = cyclomatic_case_cnt = 0;
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
					cyclomatic_case_stack.push(cyclomatic_case_cnt);
					cyclomatic_case_cnt = 0;
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
				lineElement element(cyclomatic_cnt + 1, function_name);
				lineElement n_element(cyclomatic_cnt + cyclomatic_logic_cnt + 1, function_name);
				lineElement c_element(cyclomatic_case_cnt - ignore_cyclomatic_cnt + 1, function_name);
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

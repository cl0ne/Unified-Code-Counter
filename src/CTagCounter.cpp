//! Code counter class methods for tag languages including HTML, XML, and ColdFusion.
/*!
* \file CTagCounter.cpp
*
* This file contains the code counter class methods for tag languages including HTML, XML, and ColdFusion.
*/

#include "CTagCounter.h"

/*!
* Constructs a CTagCounter object.
*/
CTagCounter::CTagCounter()
{
	casesensitive = false;
}

/*!
* Processes physical and logical lines according to language specific rules.
* NOTE: all the blank lines +
*               whole line comments
*       should have been blanked from filemap by previous processing
*       before reaching this function
*
* \param fmap list of processed file lines
* \param result counter results
* \param fmapBak list of original file lines (same as fmap except it contains unmodified quoted strings)
*
* \return method status
*/
int CTagCounter::LanguageSpecificProcess(filemap* fmap, results* result, filemap* fmapBak)
{
	unsigned int cnt = 0;
	filemap::iterator fit, fitBak;
	string line, lineBak;
	size_t lineNumber = 0;
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$";
	string strLSLOC, strLSLOCBak;
	char prev_char = 0;
	bool ok, data_continue = false;
	unsigned int phys_exec_lines = 0;
	unsigned int phys_data_lines = 0;
	unsigned int temp_lines = 0;

	size_t idx_start, quote_idx_start;
	char CurrentQuoteEnd = 0;
	bool quote_contd = false;
	QuoteStart = ">";
	QuoteEnd = "<";

	for (fit = fmap->begin(), fitBak = fmapBak->begin(); fit != fmap->end(); fit++, fitBak++)
	{
		if (!CUtil::CheckBlank(fit->line))
		{
			// replace "quotes" - string between close and open tags
			// must be processed after comments since comments start/end with same character as tag
			quote_idx_start = 0;
			idx_start = 0;
			if (quote_contd)
			{
				// replace quote until next character
				ReplaceQuote(fit->line, quote_idx_start, quote_contd, CurrentQuoteEnd);
			}
			if (!quote_contd)
			{
				while (idx_start < fit->line.length())
				{
					quote_idx_start = FindQuote(fit->line, QuoteStart, quote_idx_start, QuoteEscapeFront);

					if (quote_idx_start == string::npos)
						break;

					ReplaceQuote(fit->line, quote_idx_start, quote_contd, CurrentQuoteEnd);
					if (quote_idx_start > idx_start)
					{
						// comment delimiter inside quote
						idx_start = quote_idx_start;
						continue;
					}
				}
			}

			line = fit->line;
			lineBak = fitBak->line;
			lineNumber = fit->lineNumber;

			LSLOC(result, line, lineNumber, lineBak, strLSLOC, strLSLOCBak, prev_char,
				data_continue, temp_lines, phys_exec_lines, phys_data_lines);

			if (print_cmplx)
			{
				cnt = 0;
				CountTagTally(line, exec_name_list, cnt, 1, exclude, "", "", &result->exec_name_count, false);
			}

			result->exec_lines[PHY] += phys_exec_lines;
			phys_exec_lines = 0;

			result->data_lines[PHY] += phys_data_lines;
			phys_data_lines = 0;
		}
	}
	QuoteStart = "";
	QuoteEnd = "";

	// capture closing tag
	if (strLSLOC.length() > 0)
	{
		bool trunc_flag = false;
		if (strLSLOCBak.length() == this->lsloc_truncate)
			trunc_flag = true;
		ok = result->addSLOC(strLSLOCBak, lineNumber, trunc_flag);

		cnt = 0;
		if (data_name_list.size() > 0)
			CUtil::CountTally(strLSLOC, data_name_list, cnt, 1, exclude, "", "", &result->data_name_count);

		if (data_continue || cnt > 0)
		{
			if (ok)
				result->data_lines[LOG]++;
			result->data_lines[PHY]++;
		}
		else
		{
			if (ok)
				result->exec_lines[LOG]++;

			// since physical data lines are recorded at next LSLOC, check if first line was a data line
			if (data_name_list.size() > 0)
			{
				fit = fmap->begin();
				cnt = 0;
				CUtil::CountTally(fit->line, data_name_list, cnt, 1, exclude, "", "", NULL);
				if (cnt > 0)
					result->exec_lines[PHY]++;
			}
		}
	}
	return 0;
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
* \param prev_char previous character
* \param data_continue continuation of a data declaration line
* \param temp_lines tracks physical line count
* \param phys_exec_lines number of physical executable lines
* \param phys_data_lines number of physical data lines
*/
void CTagCounter::LSLOC(results* result, string line, size_t lineNumber, string lineBak, string &strLSLOC, string &strLSLOCBak, char &prev_char,
						bool &data_continue, unsigned int &temp_lines, unsigned int &phys_exec_lines, unsigned int &phys_data_lines)
{
	size_t start = 0;
	size_t i = 0, strSize;
	unsigned int cnt = 0;
	string tmp;
	bool trunc_flag = false;
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$";

	while (i < line.length())
	{
		if (line[i] == '<')
		{
			/*
			 * Modification Fall 2016
			 * Line below changed from: if (line.length() - 1 > i && (line[i+1] == '/' || line[i+1] == '?' || line[i+1] == '!'))
                         * to: if (line.length() - 1 > i && (line[i+1] == '/' ))  --> removed condition for ? and !
         	 	 * in order to count multiple lines beginning with <! or <? as separate SLOCS. 
			 * 	Reason for having ? or ! were not known.
			 * 	Tested Huge Collections of OpenSource HTML and XML files for SLOC counts before making this change
			 */
			if (line.length() - 1 > i && (line[i+1] == '/' )) 
            {
				i++;
				continue;
			}

			strSize = CUtil::TruncateLine(i - start, strLSLOC.length(), this->lsloc_truncate, trunc_flag);
			if (strSize > 0)
			{
				if (strLSLOC.length() > 0 && strLSLOC[strLSLOC.length()-1] != '>' && line[0] != '<')
				{
					strLSLOC += " ";
					strLSLOCBak += " ";
				}
				strLSLOC += line.substr(start, strSize);
				strLSLOCBak += lineBak.substr(start, strSize);
			}
			if (strLSLOC.length() > 0)
			{
				if (result->addSLOC(strLSLOCBak, lineNumber, trunc_flag))
				{
					cnt = 0;
					if (data_name_list.size() > 0)
						CUtil::CountTally(strLSLOC, data_name_list, cnt, 1, exclude, "", "", &result->data_name_count);

					if (data_continue || cnt > 0)
					{
						result->data_lines[LOG]++;
						phys_data_lines = temp_lines;
					}
					else
					{
						temp_lines++;
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
				start = i;
			}
		}
		else if (line[i] == '>')
		{
			// also, <> is also skipped, empty block is not counted
			if (prev_char == '/' || prev_char == '<')
				start = i + 1;
			else if (exclude_keywords.size() > 0)
			{
				// skip excluded keywords
				cnt = 0;
				CUtil::CountTally(line.substr(start, i - start), exclude_keywords, cnt, 1, exclude, "", "", NULL);
				if (cnt > 0)
					start = i + 1;
			}
		}
		i++;
	}

	tmp = CUtil::TrimString(line.substr(start, i - start));
	strSize = CUtil::TruncateLine(tmp.length(), strLSLOC.length(), this->lsloc_truncate, trunc_flag);
	if (strSize > 0)
	{
		if (strLSLOC.length() > 0 && strLSLOC[strLSLOC.length()-1] != '>' && line[0] != '<')
		{
			strLSLOC += " ";
			strLSLOCBak += " ";
		}
		strLSLOC += tmp.substr(0, strSize);
		tmp = CUtil::TrimString(lineBak.substr(start, i - start));
		strLSLOCBak += tmp.substr(0, strSize);
	}

	// verify not beginning to process a new data line
	cnt = 0;
	if (!data_continue && data_name_list.size() > 0)
	{
		CUtil::CountTally(strLSLOC, data_name_list, cnt, 1, exclude, "", "", NULL);
		if (cnt > 0)
			data_continue = true;
	}
	if (data_continue == true)
		temp_lines++;
	if (temp_lines == 0 && phys_data_lines == 0 && phys_exec_lines == 0)
		phys_exec_lines = 1; 		
}

/*!
* Counts HTML keywords in string.
*
* \param base string to search
* \param container set of keywords
* \param count number of keywords found
* \param mode allowable leading/trailing character mode (1=exclude keywords surrounded by exclude characters, 2=include only keywords preceded by include1 characters and followed by include2 characters)
* \param exclude list of characters that may not surround keywords found (if mode=1)
* \param include1 list of characters that must precede keywords found (if mode=2)
* \param include2 list of characters that must follow keywords found (if mode=2)
* \param counter_container stores list of individual keyword counts (if specified)
* \param case_sensitive language is case sensitive?
*/
void CTagCounter::CountTagTally(string base, StringVector& container, unsigned int &count, int mode, string exclude,
								string include1, string include2, UIntVector* counter_container, bool case_sensitive)
{
	base = " " + base + " ";
	string::size_type idx;
	string base1, temp;
	StringVector::iterator vit;
	UIntVector::iterator cit;
	unsigned int single_count = 0;

	if (counter_container)
		cit = counter_container->begin();

	if (case_sensitive == false)
	{
		base1 = CUtil::ToLower(base);
		for (vit = container.begin(); vit != container.end(); vit++)
			(*vit) = CUtil::ToLower((*vit));
	}
	else
		base1 = base;

	if (mode == 1)
	{
		// exclude mode
		for (vit = container.begin(); vit != container.end(); vit++)
		{
			temp = "<" + *vit;
			idx = base1.find(temp);
			while (idx != string::npos)
			{
				if ((exclude.find(base1[idx+(temp.size())] ) == string::npos) &&
					(exclude.find(base1[idx-1]) == string::npos))
				{
					count++;
					single_count++;
				}
				idx = base1.find(temp, idx + (temp.size()));
			}
			if (counter_container)
			{
				(*cit) += single_count;
				single_count = 0;
				cit++;
			}
		}
	}
	else if (mode == 2)
	{
		// include mode
		for (vit = container.begin(); vit != container.end(); vit++) 
		{
			temp = "<" + *vit;
			idx = base1.find(temp);
			while (idx != string::npos) 
			{
				if ((include1.find(base1[idx-1]) != string::npos) &&
					(include2.find(base1[idx+(temp.size())]) != string::npos))
					count++;
				idx = base1.find(temp, idx + (temp.size()));
			}
		}
	}
}

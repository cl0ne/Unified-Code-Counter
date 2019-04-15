//! Code counter class methods for Makefiles.
/*!
* \file CMakefileCounter.cpp
*
* This file contains the code counter class methods for Makefiles.
*/

#include "CMakefileCounter.h"

/*!
* Constructs a CMakefileCounter object.
*/
CMakefileCounter::CMakefileCounter()
{
	classtype = MAKEFILE;
	language_name = "Makefile";
    
    //Modification: 11.2016 Ext-4
    file_extension = CUtil::getExtensionsToLanguage("Makefile", file_extension);
    
	//file_extension.push_back(".make");
	//file_extension.push_back(".makefile");

	QuoteStart = "\"'";
	QuoteEnd = "\"'";
	LineCommentStart.push_back("#");
	
	directive.push_back("include");
	directive.push_back("-include");
	directive.push_back("sinclude");

	cmplx_assign_list.push_back("=");
	cmplx_assign_list.push_back("?=");
	cmplx_assign_list.push_back(":=");
	cmplx_assign_list.push_back("+=");

	cmplx_preproc_list.push_back("include");
	cmplx_preproc_list.push_back("-include");
	cmplx_preproc_list.push_back("sinclude");
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
int CMakefileCounter::CountDirectiveSLOC(filemap* fmap, results* result, filemap* fmapBak)
{
	bool contd = false, trunc_flag = false;
	size_t idx, strSize;
	unsigned int cnt = 0;
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$";
	string strDirLine = "";

	filemap::iterator itfmBak = fmapBak->begin();
	for (filemap::iterator iter = fmap->begin(); iter != fmap->end(); iter++, itfmBak++)
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
				// ensures the keyword stands alone
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
			// drop continuation symbol
			if (strDirLine[strDirLine.length()-1] == '\\')
				strDirLine = strDirLine.substr(0, strDirLine.length()-1);

			// if a directive or continuation of a directive (no continuation symbol found)
			if (iter->line[iter->line.length()-1] != '\\')
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
int CMakefileCounter::LanguageSpecificProcess(filemap* fmap, results* result, filemap* fmapBak)
{
	filemap::iterator fit, fitbak;
	string line, lineBak;
	string strLSLOC = "";
	string strLSLOCBak = "";
	unsigned int cnt = 0;
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$";

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
			LSLOC(result, line, fit->lineNumber, lineBak, strLSLOC, strLSLOCBak);

			if (print_cmplx)
			{
				cnt = 0;
				CUtil::CountTally(line, exec_name_list, cnt, 1, exclude, "", "", &result->exec_name_count);
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
*/
void CMakefileCounter::LSLOC(results* result, string line, size_t lineNumber, string lineBak, string &strLSLOC, string &strLSLOCBak)
{
	size_t strSize;
	bool trunc_flag = false;
	string tmp = CUtil::TrimString(line);
	//string tmpBak = CUtil::TrimString(line);
	string tmpBak = CUtil::TrimString(lineBak); //warning fix
	string tmpLower = CUtil::ToLower(tmp);

	// check for continuation and skip ending condition strings
	if (tmp[tmp.length()-1] == '\\')
	{
		strSize = CUtil::TruncateLine(tmp.length(), strLSLOC.length(), this->lsloc_truncate, trunc_flag);
		if (strSize > 1)
		{
			strLSLOC += CUtil::TrimString(tmp.substr(0, strSize - 1)) + " ";
			strLSLOCBak += CUtil::TrimString(tmpBak.substr(0, strSize - 1)) + " ";
			return;
		}
	}
	if (tmpLower == "endef" || tmpLower == "else" || tmpLower == "endif" || tmpLower == "done")
		return;

	strSize = CUtil::TruncateLine(tmp.length(), strLSLOC.length(), this->lsloc_truncate, trunc_flag);
	if (strSize > 0)
	{
		strLSLOC += tmp.substr(0, strSize);
		strLSLOCBak += tmpBak.substr(0, strSize);
	}
	if (result->addSLOC(strLSLOCBak, lineNumber, trunc_flag))
		result->exec_lines[LOG]++;
	strLSLOC = strLSLOCBak = "";
}

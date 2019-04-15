//! Code counter class methods for the cascading style sheet (CSS) language.
/*!
* \file CCssCounter.cpp
*
* This file contains the code counter class methods for the cascading style sheet (CSS) language.
*/

#include "CCssCounter.h"

/*!
* Constructs a CCssCounter object.
*/
CCssCounter::CCssCounter()
{
	classtype = CSS;
	language_name = "CSS";
    //Modification: 11.2016 Ext-4
    file_extension = CUtil::getExtensionsToLanguage("CSS", file_extension);
    /*
	file_extension.push_back(".css");*/

	BlockCommentStart.push_back("/*");
	BlockCommentEnd.push_back("*/");

	exec_name_list.push_back("azimuth");
	exec_name_list.push_back("background");
	exec_name_list.push_back("background-attachment");
	exec_name_list.push_back("background-color");
	exec_name_list.push_back("background-image");
	exec_name_list.push_back("background-repeat");
	exec_name_list.push_back("background-position");
	exec_name_list.push_back("border");
	exec_name_list.push_back("border-bottom");
	exec_name_list.push_back("border-collapse");
	exec_name_list.push_back("border-bottom-color");
	exec_name_list.push_back("border-bottom-style");
	exec_name_list.push_back("border-bottom-width");
	exec_name_list.push_back("border-left");
	exec_name_list.push_back("border-left-color");
	exec_name_list.push_back("border-left-style");
	exec_name_list.push_back("border-left-width");
	exec_name_list.push_back("border-right");
	exec_name_list.push_back("border-right-color");
	exec_name_list.push_back("border-right-style");
	exec_name_list.push_back("border-right-width");
	exec_name_list.push_back("border-spacing");
	exec_name_list.push_back("border-style");
	exec_name_list.push_back("border-top");
	exec_name_list.push_back("border-top-color");
	exec_name_list.push_back("border-top-style");
	exec_name_list.push_back("border-top-width");
	exec_name_list.push_back("border-width");
	exec_name_list.push_back("bottom");
	exec_name_list.push_back("caption-side");
	exec_name_list.push_back("clear");
	exec_name_list.push_back("clip");
	exec_name_list.push_back("content");
	exec_name_list.push_back("counter-decrement");
	exec_name_list.push_back("counter-increment");
	exec_name_list.push_back("counter-reset");
	exec_name_list.push_back("cue");
	exec_name_list.push_back("cue-after");
	exec_name_list.push_back("cue-before");
	exec_name_list.push_back("cursor");
	exec_name_list.push_back("direction");
	exec_name_list.push_back("empty-cells");
	exec_name_list.push_back("float");
	exec_name_list.push_back("font");
	exec_name_list.push_back("font-family");
	exec_name_list.push_back("font-size");
	exec_name_list.push_back("font-style");
	exec_name_list.push_back("font-variant");
	exec_name_list.push_back("font-weight");
	exec_name_list.push_back("height");
	exec_name_list.push_back("left");
	exec_name_list.push_back("letter-spacing");
	exec_name_list.push_back("line-height");
	exec_name_list.push_back("list-style");
	exec_name_list.push_back("list-style-image");
	exec_name_list.push_back("list-style-position");
	exec_name_list.push_back("list-style-type");
	exec_name_list.push_back("line-width");
	exec_name_list.push_back("margin");
	exec_name_list.push_back("margin-bottom");
	exec_name_list.push_back("margin-left");
	exec_name_list.push_back("margin-right");
	exec_name_list.push_back("margin-top");
	exec_name_list.push_back("max-height");
	exec_name_list.push_back("max-width");
	exec_name_list.push_back("min-height");
	exec_name_list.push_back("min-width");
	exec_name_list.push_back("orphans");
	exec_name_list.push_back("outline");
	exec_name_list.push_back("outline-color");
	exec_name_list.push_back("outline-style");
	exec_name_list.push_back("outline-width");
	exec_name_list.push_back("padding");
	exec_name_list.push_back("padding-bottom");
	exec_name_list.push_back("padding-left");
	exec_name_list.push_back("padding-right");
	exec_name_list.push_back("padding-top");
	exec_name_list.push_back("page-break-after");
	exec_name_list.push_back("page-break-before");
	exec_name_list.push_back("pause");
	exec_name_list.push_back("pause-after");
	exec_name_list.push_back("pitch");
	exec_name_list.push_back("play");
	exec_name_list.push_back("play-during");
	exec_name_list.push_back("pitch-range");
	exec_name_list.push_back("position");
	exec_name_list.push_back("quptes");
	exec_name_list.push_back("richness");
	exec_name_list.push_back("right");
	exec_name_list.push_back("speak");
	exec_name_list.push_back("speak-header");
	exec_name_list.push_back("speak-numeral");
	exec_name_list.push_back("speak-punctuation");
	exec_name_list.push_back("speech-rate");
	exec_name_list.push_back("stress");
	exec_name_list.push_back("table-layout");
	exec_name_list.push_back("text");
	exec_name_list.push_back("text-transform");
	exec_name_list.push_back("text-indent");
	exec_name_list.push_back("text-decoration");
	exec_name_list.push_back("text-align");
	exec_name_list.push_back("top");
	exec_name_list.push_back("unicode-bidi");
	exec_name_list.push_back("vertical-align");
	exec_name_list.push_back("visibility");
	exec_name_list.push_back("voice-family");
	exec_name_list.push_back("volume");
	exec_name_list.push_back("white-space");
	exec_name_list.push_back("width");
	exec_name_list.push_back("word-spacing");
	exec_name_list.push_back("z-index");
}

/*!
* Processes physical and logical lines according to language specific rules.
*
* \param fmap list of processed file lines
* \param result counter results
* \param fmapBak list of original file lines (same as fmap except it contains unmodified quoted strings)
*
* \return method status
*/
int CCssCounter::LanguageSpecificProcess(filemap* fmap, results* result, filemap* /*fmapBak*/) 
{
	string  exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$";
	char	prev_char				= ' ';
	string	strLSLOC				= "";
	string	line					= "";
	unsigned int phys_exec_lines	= 0;
	unsigned int phys_data_lines	= 0;
	unsigned int cnt				= 0;

	// The fmap may not contain any lines (e.g. when the file was not found).
	// If this check is not performed, when the for loop starts on the second
	// line (see +1 below) an exception will occur.  Ideally this exception
	// will be passed up to CCodeCounter::CountSLOC and then be caught in
	// MainObject::ProcessSourceList resulting in the output "Unable to count
	// file".  However this exception causes a program crash in Microsoft
	// Visual Studio 2008.  Note that this check causes the exception not to
	// occur, and therefore the "Unable to count file" message not be be
	// displayed, but the more appropriate "unable to open file" message is
	// displayed with or without this check.
	if (fmap->size() == 0)
		return 1;

	// iterating line-by-line of the file
	for (filemap::iterator iter = fmap->begin(); iter != fmap->end(); iter++)
	{
		line = iter->line;

		// checking for a blank line
		if (!CUtil::CheckBlank(line))
		{
			// LSLOC call for the current line, which is not blank and does not contain non-css keywords
			LSLOC(result, line, iter->lineNumber, strLSLOC, prev_char, phys_exec_lines, phys_data_lines);

			if (print_cmplx)
			{
				cnt = 0;
				CUtil::CountTally(iter->line, exec_name_list, cnt, 1, exclude, "", "", &result->exec_name_count);
			}

			// update physical SLOC
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
* \param strLSLOC processed logical string
* \param lastLinesLastChar last character on previous line
* \param phys_exec_lines number of physical executable lines
* \param phys_data_lines number of physical data lines
*/
void CCssCounter::LSLOC(results* result, string &line, size_t lineNumber, string &strLSLOC, char &lastLinesLastChar, 
								unsigned int &phys_exec_lines, unsigned int &phys_data_lines)
{
	size_t index = 0, strSize;
	bool trunc_flag = false;
	string tmp = CUtil::TrimString(line);

	// iterating character-by-character for the current line
	while (index < tmp.length())
	{
		// checking current character
		switch (line[index])
		{
		case ';': case '}':
			// adding the current exec strLSLOC to results
			if (result->addSLOC(strLSLOC, lineNumber, trunc_flag))
				result->exec_lines[LOG]++;

			// reinitialize strLSLOC to null
			strLSLOC = "";

			break;
		case '{':
			// adding the current data strLSLOC to results
			if (result->addSLOC(strLSLOC, lineNumber, trunc_flag))
				result->data_lines[LOG]++;
			
			// reinitialize strLSLOC to null
			strLSLOC = "";

			break;
		default:	
			// by default for other characters than '{' or ';' or '}' this case is executed
			// append character to strLSLOC
			strSize = CUtil::TruncateLine(1, strLSLOC.length(), this->lsloc_truncate, trunc_flag);
			if (strSize > 0)
				strLSLOC += tmp[index];
			break;
		}

		// incrementing index to point to next character
		index++;
	}

	if (tmp[tmp.length() - 1] == ';' ||
		(lastLinesLastChar == ';' && tmp.find_first_of('{') == string::npos && tmp.find_first_of('}') == string::npos))
		phys_exec_lines++;
	else
		phys_data_lines++;

	// extract the last line's last character
	lastLinesLastChar = tmp[tmp.length() - 1];
}

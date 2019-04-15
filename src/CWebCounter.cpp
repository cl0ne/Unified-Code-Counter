//! Code counter class methods for web languages.
/*!
* \file CWebCounter.cpp
*
* This file contains the code counter class methods for web languages.
*/

#include "UCCBeforeLibraryIncludes.h"    // Modification: 2015.12
#include <set>
#include "UCCAfterLibraryIncludes.h"     // Modification: 2015.12

#include "UCCFilesOut.h"                 // Modification: 2015.12
#include "CWebCounter.h"

/*!
* Constructs a CWebCounter object.
*/
CWebCounter::CWebCounter()
{
	classtype = WEB;

	// initialize list of web language names
	web_lang_names.push_back("PHP");
	web_lang_names.push_back("ASP");
	web_lang_names.push_back("JSP");
	web_lang_names.push_back("HTML");
	web_lang_names.push_back("XML");
	web_lang_names.push_back("ColdFusion");

	// initialize language file counters
	total_php_filesA = 0;
	total_asp_filesA = 0;
	total_jsp_filesA = 0;
	total_htm_filesA = 0;
	total_xml_filesA = 0;
	total_cfm_filesA = 0;
	total_php_filesB = 0;
	total_asp_filesB = 0;
	total_jsp_filesB = 0;
	total_htm_filesB = 0;
	total_xml_filesB = 0;
	total_cfm_filesB = 0;
	total_php_dupFilesA = 0;
	total_asp_dupFilesA = 0;
	total_jsp_dupFilesA = 0;
	total_htm_dupFilesA = 0;
	total_xml_dupFilesA = 0;
	total_cfm_dupFilesA = 0;
	total_php_dupFilesB = 0;
	total_asp_dupFilesB = 0;
	total_jsp_dupFilesB = 0;
	total_htm_dupFilesB = 0;
	total_xml_dupFilesB = 0;
	total_cfm_dupFilesB = 0;

	// initialize web language file extensions
	file_exten_htm.push_back(".html");
	file_exten_htm.push_back(".htm");
	file_exten_htm.push_back(".shtml");
	file_exten_htm.push_back(".shtm");
	file_exten_htm.push_back(".stm");
	file_exten_htm.push_back(".sht");
	file_exten_htm.push_back(".oth");
	file_exten_htm.push_back(".xhtml");
	file_exten_xml.push_back(".xml");
	file_exten_php.push_back(".php");
	file_exten_asp.push_back(".asp");
	file_exten_asp.push_back(".aspx");
	file_exten_jsp.push_back(".jsp");
	file_exten_cfm.push_back(".cfm");
	file_exten_cfm.push_back(".cfml");
	file_exten_cfm.push_back(".cfc");
	UpdateWebFileExt();

	// the space to save the separated file
	SourceFileElement tmp;
	tmp.second.file_name = "*.*htm";
	Separation.insert(map<int, SourceFileElement>::value_type(HTML				,tmp));
	tmp.second.file_name = "*.*htmphp";
	Separation.insert(map<int, SourceFileElement>::value_type(HTML_PHP			,tmp));
	tmp.second.file_name = "*.*htmjsp";
	Separation.insert(map<int, SourceFileElement>::value_type(HTML_JSP			,tmp));
	tmp.second.file_name = "*.*htmasp";
	Separation.insert(map<int, SourceFileElement>::value_type(HTML_ASP			,tmp));
	tmp.second.file_name = "*.*htmcfm";
	Separation.insert(map<int, SourceFileElement>::value_type(HTML_CFM			,tmp));
	tmp.second.file_name = "*.*xml";
	Separation.insert(map<int, SourceFileElement>::value_type(XML				,tmp));
	tmp.second.file_name = "*.*jshtm";
	Separation.insert(map<int, SourceFileElement>::value_type(JAVASCRIPT_HTML	,tmp));
	tmp.second.file_name = "*.*jsxml";
	Separation.insert(map<int, SourceFileElement>::value_type(JAVASCRIPT_XML	,tmp));
	tmp.second.file_name = "*.*jsphp";
	Separation.insert(map<int, SourceFileElement>::value_type(JAVASCRIPT_PHP	,tmp));
	tmp.second.file_name = "*.*jsjsp";
	Separation.insert(map<int, SourceFileElement>::value_type(JAVASCRIPT_JSP	,tmp));
	tmp.second.file_name = "*.*jsasps";
	Separation.insert(map<int, SourceFileElement>::value_type(JAVASCRIPT_ASP_S	,tmp));
	tmp.second.file_name = "*.*jsaspc";
	Separation.insert(map<int, SourceFileElement>::value_type(JAVASCRIPT_ASP_C	,tmp));
	tmp.second.file_name = "*.*jscfm";
	Separation.insert(map<int, SourceFileElement>::value_type(JAVASCRIPT_CFM	,tmp));
	tmp.second.file_name = "*.*php";
	Separation.insert(map<int, SourceFileElement>::value_type(PHP				,tmp));
	tmp.second.file_name = "*.*sqlcfm";
	Separation.insert(map<int, SourceFileElement>::value_type(SQL_CFM			,tmp));
	tmp.second.file_name = "*.*java";
	Separation.insert(map<int, SourceFileElement>::value_type(JAVA_JSP			,tmp));
	tmp.second.file_name = "*.*cshtm";
	Separation.insert(map<int, SourceFileElement>::value_type(CSHARP_HTML		,tmp));
	tmp.second.file_name = "*.*csxml";
	Separation.insert(map<int, SourceFileElement>::value_type(CSHARP_XML		,tmp));
	tmp.second.file_name = "*.*csasps";
	Separation.insert(map<int, SourceFileElement>::value_type(CSHARP_ASP_S		,tmp));
	tmp.second.file_name = "*.*vbshtm";
	Separation.insert(map<int, SourceFileElement>::value_type(VBS_HTML			,tmp));
	tmp.second.file_name = "*.*vbsxml";
	Separation.insert(map<int, SourceFileElement>::value_type(VBS_XML			,tmp));
	tmp.second.file_name = "*.*vbsphp";
	Separation.insert(map<int, SourceFileElement>::value_type(VBS_PHP			,tmp));
	tmp.second.file_name = "*.*vbsjsp";
	Separation.insert(map<int, SourceFileElement>::value_type(VBS_JSP			,tmp));
	tmp.second.file_name = "*.*vbsasps";
	Separation.insert(map<int, SourceFileElement>::value_type(VBS_ASP_S			,tmp));
	tmp.second.file_name = "*.*vbsaspc";
	Separation.insert(map<int, SourceFileElement>::value_type(VBS_ASP_C			,tmp));
	tmp.second.file_name = "*.*vbscfm";
	Separation.insert(map<int, SourceFileElement>::value_type(VBS_CFM			,tmp));
	tmp.second.file_name = "*.*cfm";
	Separation.insert(map<int, SourceFileElement>::value_type(COLDFUSION		,tmp));
	tmp.second.file_name = "*.*cfs";
	Separation.insert(map<int, SourceFileElement>::value_type(CFSCRIPT			,tmp));

	// the space to save the separated line
	lineElement tmp2;
	SeparatedLine.insert(map<int, lineElement>::value_type(HTML					,tmp2));
	SeparatedLine.insert(map<int, lineElement>::value_type(HTML_PHP				,tmp2));
	SeparatedLine.insert(map<int, lineElement>::value_type(HTML_JSP				,tmp2));
	SeparatedLine.insert(map<int, lineElement>::value_type(HTML_ASP				,tmp2));
	SeparatedLine.insert(map<int, lineElement>::value_type(HTML_CFM				,tmp2));
	SeparatedLine.insert(map<int, lineElement>::value_type(XML					,tmp2));
	SeparatedLine.insert(map<int, lineElement>::value_type(JAVASCRIPT_HTML		,tmp2));
	SeparatedLine.insert(map<int, lineElement>::value_type(JAVASCRIPT_XML		,tmp2));
	SeparatedLine.insert(map<int, lineElement>::value_type(JAVASCRIPT_PHP		,tmp2));
	SeparatedLine.insert(map<int, lineElement>::value_type(JAVASCRIPT_JSP		,tmp2));
	SeparatedLine.insert(map<int, lineElement>::value_type(JAVASCRIPT_ASP_S		,tmp2));
	SeparatedLine.insert(map<int, lineElement>::value_type(JAVASCRIPT_ASP_C		,tmp2));
	SeparatedLine.insert(map<int, lineElement>::value_type(JAVASCRIPT_CFM		,tmp2));
	SeparatedLine.insert(map<int, lineElement>::value_type(PHP					,tmp2));
	SeparatedLine.insert(map<int, lineElement>::value_type(SQL_CFM				,tmp2));
	SeparatedLine.insert(map<int, lineElement>::value_type(JAVA_JSP				,tmp2));
	SeparatedLine.insert(map<int, lineElement>::value_type(CSHARP_HTML			,tmp2));
	SeparatedLine.insert(map<int, lineElement>::value_type(CSHARP_XML			,tmp2));
	SeparatedLine.insert(map<int, lineElement>::value_type(CSHARP_ASP_S			,tmp2));
	SeparatedLine.insert(map<int, lineElement>::value_type(VBS_HTML				,tmp2));
	SeparatedLine.insert(map<int, lineElement>::value_type(VBS_XML				,tmp2));
	SeparatedLine.insert(map<int, lineElement>::value_type(VBS_PHP				,tmp2));
	SeparatedLine.insert(map<int, lineElement>::value_type(VBS_JSP				,tmp2));
	SeparatedLine.insert(map<int, lineElement>::value_type(VBS_ASP_S			,tmp2));
	SeparatedLine.insert(map<int, lineElement>::value_type(VBS_ASP_C			,tmp2));
	SeparatedLine.insert(map<int, lineElement>::value_type(VBS_CFM				,tmp2));
	SeparatedLine.insert(map<int, lineElement>::value_type(COLDFUSION			,tmp2));
	SeparatedLine.insert(map<int, lineElement>::value_type(CFSCRIPT				,tmp2));

	// supported script tags
	// this container contains the tags that may appear in html code to specify the script language
	TagTable.insert(map<string, int>::value_type(string("<?php")		,WEB_PHP_START));
	TagTable.insert(map<string, int>::value_type(string("<?")			,WEB_PHP_START2)); 
	TagTable.insert(map<string, int>::value_type(string("?>")			,WEB_PHP_END));
	TagTable.insert(map<string, int>::value_type(string("<script")		,WEB_SCRIPT_START));
	TagTable.insert(map<string, int>::value_type(string("</script")		,WEB_SCRIPT_END));
	TagTable.insert(map<string, int>::value_type(string("<%")			,WEB_ASP_JSP_START));
	TagTable.insert(map<string, int>::value_type(string("%>")			,WEB_ASP_JSP_END));
	TagTable.insert(map<string, int>::value_type(string("<cf")			,WEB_CFM_START));
	TagTable.insert(map<string, int>::value_type(string("</cf")			,WEB_CFM_END));

	// supported script languages
	// this container contains the strings that may appear in <script language="******">
	ScriptTable.insert(map<string, int>::value_type(string("javascript"),JAVASCRIPT));
	ScriptTable.insert(map<string, int>::value_type(string("jscript")	,JAVASCRIPT));
	ScriptTable.insert(map<string, int>::value_type(string("c#")		,CSHARP_HTML));
	ScriptTable.insert(map<string, int>::value_type(string("php")		,PHP));
	ScriptTable.insert(map<string, int>::value_type(string("vb")		,VB));
}

/*!
* Cleans up separated language file objects.
*/
void CWebCounter::CleanSeparatedFile()
{
	for (map<int, SourceFileElement>::iterator iter = Separation.begin(); iter != Separation.end(); iter++)
		iter->second.first.clear();
	while (!PreviousLanguage.empty())
		PreviousLanguage.pop();
}

/*!
* Cleans up separated language file lines.
*/
void CWebCounter::CleanSeparatedLine()
{
	for (map<int, lineElement>::iterator iter = SeparatedLine.begin(); iter != SeparatedLine.end(); iter++)
	{
		iter->second.line = "";
		iter->second.lineNumber = 0;
	}
}

/*!
* Separate web files into different languages and count these
* languages using their corresponding counters.
* Names the newly created file with its embedding file's filename.
* The filename is used in the matching function for the comparison purpose.
* new file's filename = embedding file's filename + its default filename
* NOTE: the filename contains *.* in its name.
*
* \param fmap list of file lines
* \param result counter results
*
* \return method status
*/
int CWebCounter::CountSLOC(filemap* fmap, results* result)
{
	CleanSeparatedFile();
	lineElement current_line;
	lineElement separate_line;
	size_t pos, close_pos, search_result, tempi, lng;
	char c;
	int current_language, cf_mode = -1, tag_open_mode = -1;
	int script_language_of_asp = VBS_ASP_S;
	int script_language_of_cfm = VBS_CFM;
	bool blank_line, found;
	bool first_line_of_asp = true;
	size_t preLang = INVALID_POSITION;	// used for writing to files
	string language, tempstr;
	WebType webType;

	webType = GetWebType(result->file_name);
	language_name = GetWebLangName(webType);
	switch (webType)
	{
	case WEB_ASP:
		current_language = HTML_ASP;
		break;
	case WEB_JSP:
		current_language = HTML_JSP;
		break;
	case WEB_PHP:
		current_language = HTML_PHP;
		break;
	case WEB_CFM:
		current_language = COLDFUSION;
		cf_mode = current_language;
		break;
	case WEB_XML:
		current_language = XML;
		break;
	case WEB_HTM:
		current_language = HTML;
		break;
	case WEB_UNKNOWN:
	default:
		current_language = HTML;
		break;
	}

	for (filemap::iterator iter = fmap->begin(); iter != fmap->end(); iter++)
	{
		current_line.line = iter->line;
		current_line.lineNumber = iter->lineNumber;

		if (current_line.line.length() == 0)
			blank_line = true;
		else
			blank_line = false;

		while (current_line.line.length() > 0)
		{
			search_result = CUtil::FindStringsCaseInsensitive(current_line.line, TagTable, pos, preLang);
			if (search_result != INVALID_POSITION)
				preLang = search_result;

			switch (search_result)
			{
			case WEB_PHP_START:
				{
					SeparatedLine[current_language].line += current_line.line.substr(0, pos + 5);
					PreviousLanguage.push(current_language);
					current_language = PHP;
					current_line.line = current_line.line.substr(pos + 5);
				}
				break;
			case WEB_PHP_START2:
				{
					// check for <?php since map order is by string
					tempi = 2;
					if (current_line.line.length() >= pos + 5)
					{
						tempstr = CUtil::ToLower(current_line.line);
						if (tempstr.substr(pos + 2, 3) == "php")
							tempi = 5;
					}
					SeparatedLine[current_language].line += current_line.line.substr(0, pos + tempi);
					PreviousLanguage.push(current_language);
					current_language = PHP;
					current_line.line = current_line.line.substr(pos + tempi);
				}
				break;
			case WEB_PHP_END:
				{
					// check for line termination (not required at an end tag), add semi-colon if none
					tempstr = CUtil::TrimString(current_line.line.substr(0, pos));
					if (CUtil::CheckBlank(tempstr))
					{
						filemap::iterator iter2 = Separation[current_language].first.end();
						while (CUtil::CheckBlank(tempstr))
						{
							if (iter2 == Separation[current_language].first.begin())
								break;
							iter2--;
							tempstr = CUtil::TrimString(iter2->line);
							if (iter2 == Separation[current_language].first.begin())
								break;
						}
						if (!CUtil::CheckBlank(tempstr))
						{
							c = tempstr[tempstr.length() - 1];
							if (c != ';' && c != '{' && c != '}' && c != ':')
								iter->line += ";";
						}
					}
					else
					{
						c = tempstr[tempstr.length() - 1];
						if (c != ';' && c != '{' && c != '}' && c != ':')
						{
							current_line.line.insert(pos, ";");
							pos++;
						}
					}
					if (cf_mode >= 0)
						current_language = cf_mode;
					else
					{
						current_language = PreviousLanguage.top();
						PreviousLanguage.pop();
					}
				}
				break;
			case WEB_ASP_JSP_END:
				{
					SeparatedLine[current_language].line += current_line.line.substr(0, pos);
					if (current_language == JAVA_JSP || current_language == CSHARP_ASP_S)
					{
						// special process for JSP; add semicolon at the end of line if there is no semicolon
						SeparatedLine[current_language].line = CUtil::TrimString(SeparatedLine[current_language].line);
						lng = SeparatedLine[current_language].line.length();
						if (lng > 0 && SeparatedLine[current_language].line[lng-1] != ';')
							SeparatedLine[current_language].line += ";";
					}
					if (cf_mode >= 0)
						current_language = cf_mode;
					else
					{
						current_language = PreviousLanguage.top();
						PreviousLanguage.pop();
					}

					// remove %>, next line
					current_line.line = current_line.line.substr(pos + 2);
				}
				break;
			case WEB_SCRIPT_START:
				{
					close_pos = current_line.line.find_first_of(">", pos + 7);
					SeparatedLine[current_language].line += current_line.line.substr(0, close_pos + 1);
					string language = CUtil::ToLower(current_line.line.substr(pos + 7, close_pos-pos + 7));
					if (close_pos < current_line.line.size())
						current_line.line = current_line.line.substr(close_pos + 1);
					else
						current_line.line = "";

					PreviousLanguage.push(current_language);
					current_language = (int)CUtil::FindStringsCaseInsensitive(language, ScriptTable, pos);	

					// check for <script runat="server"
					if (language.find("runat") != string::npos &&
						language.find("server") != string::npos)
					{
						if (webType == WEB_CFM)
						{
							switch (current_language)
							{
							case JAVASCRIPT:
								script_language_of_cfm = JAVASCRIPT_CFM;
								break;
							case VB:
								script_language_of_cfm = VBS_CFM;
								break;
							}
							current_language = script_language_of_cfm;
						}
						else
						{
							switch(current_language)
							{
							case JAVASCRIPT:
								script_language_of_asp = JAVASCRIPT_ASP_S;
								break;
							case VB:
								script_language_of_asp = VBS_ASP_S;
								break;
							case CSHARP_HTML:
							case CSHARP_XML:
								script_language_of_asp = CSHARP_ASP_S;
								break;
							}
							current_language = script_language_of_asp;
						}
					}
					else
					{
						// the script is run at client
						switch (current_language)
						{
						case JAVASCRIPT:
						case -1:
							if (webType == WEB_ASP)		// includes .aspx
								current_language = JAVASCRIPT_ASP_C;
							else if (webType == WEB_JSP)
								current_language = JAVASCRIPT_JSP;
							else if (webType == WEB_PHP)
								current_language = JAVASCRIPT_PHP;
							else if (webType == WEB_CFM)
								current_language = JAVASCRIPT_CFM;
							else if (webType == WEB_XML)
								current_language = JAVASCRIPT_XML;
							else
								current_language = JAVASCRIPT_HTML;
							break;
						case CSHARP_HTML:
						case CSHARP_XML:
						case PHP:
							break;
						case VB:
							if (webType == WEB_ASP)		// includes .aspx
								current_language = VBS_ASP_C;
							else if (webType == WEB_JSP)
								current_language = VBS_JSP;
							else if (webType == WEB_PHP)
								current_language = VBS_PHP;
							else if (webType == WEB_CFM)
								current_language = VBS_CFM;
							else if (webType == WEB_XML)
								current_language = VBS_XML;
							else
								current_language = VBS_HTML;
							break;
						}
					}
				}
				break;
			case WEB_SCRIPT_END:
				{
					SeparatedLine[current_language].line += current_line.line.substr(0, pos);
					// fixed an error when </script> appears w/o <script>, next two lines
					if (PreviousLanguage.empty())
						current_line.line = current_line.line.substr(pos + 9);
					else
					{
						if (cf_mode >= 0)
							current_language = cf_mode;
						else
						{
							current_language = PreviousLanguage.top();
							PreviousLanguage.pop();
						}
						close_pos = current_line.line.find_first_of(">", pos + 8);
						SeparatedLine[current_language].line += current_line.line.substr(pos, close_pos - pos + 1);
						if (close_pos < current_line.line.size())
							current_line.line = current_line.line.substr(close_pos + 1);
						else
							current_line.line = "";
					}
				}
				break;
			case WEB_ASP_JSP_START:
				{
					// removed <% from HTML page, next line, changed pos + 2 to pos
					SeparatedLine[current_language].line += current_line.line.substr(0, pos);
					PreviousLanguage.push(current_language);
					if (webType == WEB_ASP)		// includes .aspx
					{
						if (first_line_of_asp)
						{
							// the following lines within this "if" are used to 
							// check the language used in the page
							// case: <%@ Page language="???">
							tempstr = CUtil::ToLower(current_line.line);
							if (tempstr.find("@") != string::npos)
							{
								if ((tempstr.find("javascript") != string::npos) ||
									(tempstr.find("js") != string::npos))
									script_language_of_asp = JAVASCRIPT_ASP_S;
								else if ((tempstr.find("c#") != string::npos) ||
									(tempstr.find("cs") != string::npos) ||
									(tempstr.find("csharp") != string::npos))
									script_language_of_asp = CSHARP_ASP_S;
							}
							first_line_of_asp = false;
						}
						current_language = script_language_of_asp;
					}
					else if (webType == WEB_JSP)
						current_language = JAVA_JSP;
					else if (webType == WEB_PHP)
						current_language = PHP;
					else
						current_language = VBS_ASP_S;
					current_line.line = current_line.line.substr(pos + 2);
				}
				break;
			case WEB_CFM_START:
				{
					// check for <cfscript
					found = false;
					if (current_line.line.length() >= pos + 9)
					{
						tempstr = CUtil::ToLower(current_line.line);
						if (tempstr.substr(pos, 9) == "<cfscript")
						{
							found = true;
							SeparatedLine[current_language].line += current_line.line.substr(0, pos);
							close_pos = tempstr.find_first_of(">", pos + 9);
							if (close_pos != string::npos)
							{
								SeparatedLine[COLDFUSION].line += current_line.line.substr(pos, close_pos - pos + 1);
								if (close_pos < current_line.line.size())
									current_line.line = current_line.line.substr(close_pos + 1);
								else
									current_line.line = "";
								current_language = CFSCRIPT;
							}
							else
							{
								tag_open_mode = CFSCRIPT;
								SeparatedLine[COLDFUSION].line += current_line.line.substr(pos);
								current_line.line = "";
								current_language = COLDFUSION;
							}
						}
					}
					if (!found)
					{
						// check for <cfquery
						if (current_line.line.length() >= pos + 8)
						{
							tempstr = CUtil::ToLower(current_line.line);
							if (tempstr.substr(pos, 8) == "<cfquery")
							{
								found = true;
								SeparatedLine[current_language].line += current_line.line.substr(0, pos);
								close_pos = tempstr.find_first_of(">", pos + 8);
								if (close_pos != string::npos)
								{
									SeparatedLine[COLDFUSION].line += current_line.line.substr(pos, close_pos - pos + 1);
									if (close_pos < current_line.line.size())
										current_line.line = current_line.line.substr(close_pos + 1);
									else
										current_line.line = "";
									current_language = SQL_CFM;
								}
								else
								{
									tag_open_mode = SQL_CFM;
									SeparatedLine[COLDFUSION].line += current_line.line.substr(pos);
									current_line.line = "";
									current_language = COLDFUSION;
								}
							}
						}
					}
					if (!found)
					{
						SeparatedLine[current_language].line += current_line.line.substr(0, pos);
						close_pos = current_line.line.find_first_of(">", pos + 3);
						if (close_pos != string::npos)
						{
							SeparatedLine[COLDFUSION].line += current_line.line.substr(pos, close_pos - pos + 1);
							if (close_pos < current_line.line.size())
								current_line.line = current_line.line.substr(close_pos + 1);
							else
								current_line.line = "";
						}
						else
						{
							SeparatedLine[COLDFUSION].line += current_line.line.substr(pos);
							current_line.line = "";
						}
						current_language = COLDFUSION;
					}
					cf_mode = current_language;
				}
				break;
			case WEB_CFM_END:
				{
					SeparatedLine[current_language].line += current_line.line.substr(0, pos);
					close_pos = current_line.line.find_first_of(">", pos + 4);
					SeparatedLine[COLDFUSION].line += current_line.line.substr(pos, close_pos - pos + 1);
					if (close_pos < current_line.line.size())
						current_line.line = current_line.line.substr(close_pos + 1);
					else
						current_line.line = "";
					current_language = COLDFUSION;
					cf_mode = current_language;
				}
				break;
			default:
				{
					if (tag_open_mode >= 0)
					{
						// check for closure on open tag line
						pos = current_line.line.find(">");
						if (pos != string::npos)
						{
							SeparatedLine[current_language].line += current_line.line.substr(0, pos + 1);
							current_line.line = current_line.line.substr(pos + 1);
							current_language = tag_open_mode;
							cf_mode = current_language;
							tag_open_mode = -1;
						}
					}
					else if (cf_mode == COLDFUSION)
					{
						// check for switch to HTML in ColdFusion
						pos = current_line.line.find("<");
						if (pos != string::npos)
						{
							SeparatedLine[current_language].line += current_line.line.substr(0, pos);
							current_line.line = current_line.line.substr(pos);
							current_language = HTML_CFM;
							cf_mode = current_language;
						}
					}
					if (current_line.line.length() > 0)
					{
						SeparatedLine[current_language].line += current_line.line;
						SeparatedLine[current_language].lineNumber = current_line.lineNumber;
						current_line.line = "";
					}
				}
				break;
			}
		}
		
		// write into Separation
		if (blank_line)
			Separation[current_language].first.push_back(current_line);
		else
		{			
			for (map<int, lineElement>::iterator i = SeparatedLine.begin(); i != SeparatedLine.end(); i++)
			{
				if (i->second.line.length() > 0)
				{
					Separation[i->first].first.push_back(i->second);

					// name the newly created file with its embedding file's filename.
					// the filename is used in the matching function for the comparison purpose
					// new file's filename = embedding file's filename + its default filename
					// (note: the filename contains *.* in its name)
					if (Separation[i->first].second.file_name.find(result->file_name) != 0) 
					{
						tempi = Separation[i->first].second.file_name.find(EMBEDDED_FILE_PREFIX);
						Separation[i->first].second.file_name = result->file_name +
							Separation[i->first].second.file_name.substr(tempi);
					}
				}
			}
		}
		CleanSeparatedLine();
	}
	return 1;
}

/*!
* Retrieves the web language output file stream.
* Opens a new stream if it has not been opened already.
*
* \param webType file language type
* \param outputFileNamePrePend name to prepend to the output file
* \param cmd current command line string
* \param csvOutput CSV file stream? (otherwise ASCII text file)
* \param legacyOutput legacy format file stream? (otherwise standard text file)
*
* \return output file stream
*/
ofstream* CWebCounter::GetOutputStream(WebType webType, const string &outputFileNamePrePend, const string &cmd, bool csvOutput, bool legacyOutput)
{
	if (webType == WEB_PHP)
	{
		if (csvOutput)
		{
			if (!php_output_file_csv.is_open())
			{
				string ofname = outputFileNamePrePend + "PHP";
				ofname += OUTPUT_FILE_NAME_CSV;
				php_output_file_csv.open(ofname.c_str(), ofstream::out);

				if (!php_output_file_csv.is_open()) return NULL;
				PrintFileHeader(php_output_file_csv, "SLOC COUNT RESULTS", cmd);

				PrintFileHeaderLine(php_output_file_csv, "RESULTS FOR PHP FILES");
				php_output_file_csv << endl << ",,,,HTML,,JS-Clnt,,VBS-Clnt,,PHP,,," << endl;
				php_output_file_csv << "Total,Blank,Comments,,Word,Exec.,Data,Exec.,Data,Exec.,Comp.,Data,Exec.,Logical SLOC,,,,,Physical,File,Module" << endl;
				php_output_file_csv << "Lines,Lines,Whole,Embed,LOC,Instr,Decl,Instr,Decl,Instr,Direct.,Decl,Instr,HTML,JS-Clnt,VBS-Clnt,PHP,Total,SLOC,Type,Name" << endl;
			}
			return &php_output_file_csv;
		}
		else
		{
			if (!php_output_file.is_open())
			{
				string ofname = outputFileNamePrePend + "PHP";
				ofname += OUTPUT_FILE_NAME;
				php_output_file.open(ofname.c_str(), ofstream::out);

				if (!php_output_file.is_open()) return NULL;
				PrintFileHeader(php_output_file, "SLOC COUNT RESULTS", cmd);

				PrintFileHeaderLine(php_output_file, "RESULTS FOR PHP FILES");
				php_output_file << endl << "                                    |       HTML      |     JS-Clnt     |    VBS-Clnt     |           PHP           |" << endl;
				if (legacyOutput)
				{
					php_output_file << "   Total   Blank |      Comments    |    Word  Exec.  |   Data   Exec.  |   Data   Exec.  | Compiler  Data   Exec.  |                Logical SLOC                | File  Module" << endl;
					php_output_file << "   Lines   Lines |   Whole Embedded |     LOC  Instr. |   Decl.  Instr. |   Decl.  Instr. | Direct.   Decl.  Instr. |   Total      HTML JS-Clnt VBS-Clnt     PHP | Type  Name" << endl;
					php_output_file << "-----------------+------------------+-----------------+-----------------+-----------------+-------------------------+---------+----------------------------------+---------------------------" << endl;
				}
				else
				{
					php_output_file << "   Total   Blank |      Comments    |    Word  Exec.  |   Data   Exec.  |   Data   Exec.  | Compiler  Data   Exec.  |                Logical SLOC                | Physical | File  Module" << endl;
					php_output_file << "   Lines   Lines |   Whole Embedded |     LOC  Instr. |   Decl.  Instr. |   Decl.  Instr. | Direct.   Decl.  Instr. |    HTML JS-Clnt VBS-Clnt     PHP     Total |   SLOC   | Type  Name" << endl;
					php_output_file << "-----------------+------------------+-----------------+-----------------+-----------------+-------------------------+----------------------------------+---------+----------+---------------------------" << endl;
				}
			}
			return &php_output_file;
		}
	}
	else if (webType == WEB_JSP)
	{
		if (csvOutput)
		{
			if (!jsp_output_file_csv.is_open())
			{
				string ofname = outputFileNamePrePend + "JSP";
				ofname += OUTPUT_FILE_NAME_CSV;
				jsp_output_file_csv.open(ofname.c_str(), ofstream::out);

				if (!jsp_output_file_csv.is_open()) return NULL;

				PrintFileHeader(jsp_output_file_csv, "SLOC COUNT RESULTS", cmd);

				PrintFileHeaderLine(jsp_output_file_csv, "RESULTS FOR JSP FILES");
				jsp_output_file_csv << endl << ",,,,HTML,,JS-Clnt,,VBS-Clnt,,Java" << endl;
				jsp_output_file_csv << "Total,Blank,Comments,,Word,Exec.,Data,Exec.,Data,Exec.,Comp.,Data,Exec.,Logical SLOC,,,,,Physical,File,Module" << endl;
				jsp_output_file_csv << "Lines,Lines,Whole,Embed,LOC,Instr,Decl,Instr,Decl,Instr,Direct.,Decl,Instr,HTML,JS-Clnt,VBS-Clnt,Java,Total,SLOC,Type,Name" << endl;
			}
			return &jsp_output_file_csv;
		}
		else
		{
			if (!jsp_output_file.is_open())
			{
				string ofname = outputFileNamePrePend + "JSP";
				ofname += OUTPUT_FILE_NAME;
				jsp_output_file.open(ofname.c_str(), ofstream::out);

				if (!jsp_output_file.is_open()) return NULL;

				PrintFileHeader(jsp_output_file, "SLOC COUNT RESULTS", cmd);

				PrintFileHeaderLine(jsp_output_file, "RESULTS FOR JSP FILES");
				jsp_output_file << endl << "                                    |       HTML      |     JS-Clnt     |    VBS-Clnt     |          Java           |" << endl;
				if (legacyOutput)
				{
					jsp_output_file << "   Total   Blank |      Comments    |    Word  Exec.  |   Data   Exec.  |   Data   Exec.  | Compiler  Data   Exec.  |                Logical SLOC                | File  Module" << endl;
					jsp_output_file << "   Lines   Lines |   Whole Embedded |     LOC  Instr. |   Decl.  Instr. |   Decl.  Instr. | Direct.   Decl.  Instr. |   Total      HTML JS-Clnt VBS-Clnt    Java | Type  Name" << endl;
					jsp_output_file << "-----------------+------------------+-----------------+-----------------+-----------------+-------------------------+---------+----------------------------------+---------------------------" << endl;
				}
				else
				{
					jsp_output_file << "   Total   Blank |      Comments    |    Word  Exec.  |   Data   Exec.  |   Data   Exec.  | Compiler  Data   Exec.  |                Logical SLOC                | Physical | File  Module" << endl;
					jsp_output_file << "   Lines   Lines |   Whole Embedded |     LOC  Instr. |   Decl.  Instr. |   Decl.  Instr. | Direct.   Decl.  Instr. |    HTML JS-Clnt VBS-Clnt    Java     Total |   SLOC   | Type  Name" << endl;
					jsp_output_file << "-----------------+------------------+-----------------+-----------------+-----------------+-------------------------+----------------------------------+---------+----------+---------------------------" << endl;
				}
			}
			return &jsp_output_file;
		}
	}
	else if (webType == WEB_ASP)
	{
		if (csvOutput)
		{
			if (!asp_output_file_csv.is_open())
			{
				string ofname = outputFileNamePrePend + "ASP";
				ofname += OUTPUT_FILE_NAME_CSV;
				asp_output_file_csv.open(ofname.c_str(), ofstream::out);

				if (!asp_output_file_csv.is_open()) return NULL;

				PrintFileHeader(asp_output_file_csv, "SLOC COUNT RESULTS", cmd);

				PrintFileHeaderLine(asp_output_file_csv, "RESULTS FOR ASP FILES");
				asp_output_file_csv << endl << ",,,,HTML,,JS-Clnt,,VBS-Clnt,,JS-Svr,,VBS-Svr,,C#-Svr" << endl;
				asp_output_file_csv << "Total,Blank,Comments,,Word,Exec.,Data,Exec.,Data,Exec.,Data,Exec.,Data,Exec.,Data,Exec.,Logical SLOC,,,,,,,Physical,File,Module" << endl;
				asp_output_file_csv << "Lines,Lines,Whole,Embed,LOC,Instr,Decl,Instr,Decl,Instr,Decl,Instr,Decl,Instr,Decl,Instr,HTML,JS-Clnt,VBS-Clnt,JS-Svr,VBS-Svr,C#-Svr,Total,SLOC,Type,Name" << endl;
			}
			return &asp_output_file_csv;
		}
		else
		{
			if (!asp_output_file.is_open())
			{
				string ofname = outputFileNamePrePend + "ASP";
				ofname += OUTPUT_FILE_NAME;
				asp_output_file.open(ofname.c_str(), ofstream::out);

				if (!asp_output_file.is_open()) return NULL;

				PrintFileHeader(asp_output_file, "SLOC COUNT RESULTS", cmd);

				PrintFileHeaderLine(asp_output_file, "RESULTS FOR ASP FILES");
				asp_output_file << endl << "                                    |       HTML      |     JS-Clnt     |    VBS-Clnt     |      JS-Svr     |     VBS-Svr     |      C#-Svr     |" << endl;
				if (legacyOutput)
				{
					asp_output_file << "   Total   Blank |      Comments    |    Word  Exec.  |   Data   Exec.  |   Data   Exec.  |   Data   Exec.  |   Data   Exec.  |   Data   Exec.  |                        Logical SLOC                        | File  Module" << endl;
					asp_output_file << "   Lines   Lines |   Whole Embedded |     LOC  Instr. |   Decl.  Instr. |   Decl.  Instr. |   Decl.  Instr. |   Decl.  Instr. |   Decl.  Instr. |   Total      HTML JS-Clnt VBS-Clnt  JS-Svr VBS-Svr  C#-Svr | Type  Name" << endl;
					asp_output_file << "-----------------+------------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+---------+--------------------------------------------------+---------------------------" << endl;
				}
				else
				{
					asp_output_file << "   Total   Blank |      Comments    |    Word  Exec.  |   Data   Exec.  |   Data   Exec.  |   Data   Exec.  |   Data   Exec.  |   Data   Exec.  |                        Logical SLOC                        | Physical | File  Module" << endl;
					asp_output_file << "   Lines   Lines |   Whole Embedded |     LOC  Instr. |   Decl.  Instr. |   Decl.  Instr. |   Decl.  Instr. |   Decl.  Instr. |   Decl.  Instr. |    HTML JS-Clnt VBS-Clnt  JS-Svr VBS-Svr  C#-Svr     Total |   SLOC   | Type  Name" << endl;
					asp_output_file << "-----------------+------------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+--------------------------------------------------+---------+----------+---------------------------" << endl;
				}
			}
			return &asp_output_file;
		}
	}
	else if (webType == WEB_CFM)
	{
		if (csvOutput)
		{
			if (!cfm_output_file_csv.is_open())
			{
				string ofname = outputFileNamePrePend + "ColdFusion";
				ofname += OUTPUT_FILE_NAME_CSV;
				cfm_output_file_csv.open(ofname.c_str(), ofstream::out);

				if (!cfm_output_file_csv.is_open()) return NULL;

				PrintFileHeader(cfm_output_file_csv, "SLOC COUNT RESULTS", cmd);

				PrintFileHeaderLine(cfm_output_file_csv, "RESULTS FOR ColdFusion FILES");
				cfm_output_file_csv << endl << ",,,,HTML,,JS-Clnt,,VBS-Clnt,,SQL,,ColdFusion,,CFScript" << endl;
				cfm_output_file_csv << "Total,Blank,Comments,,Word,Exec.,Data,Exec.,Data,Exec.,Data,Exec.,Data,Exec.,Data,Exec.,Logical SLOC,,,,,,,Physical,File,Module" << endl;
				cfm_output_file_csv << "Lines,Lines,Whole,Embed,LOC,Instr,Decl,Instr,Decl,Instr,Decl,Instr,Decl,Instr,Decl,Instr,HTML,JS-Clnt,VBS-Clnt,SQL,ColdFusion,CFScript,Total,SLOC,Type,Name" << endl;
			}
			return &cfm_output_file_csv;
		}
		else
		{
			if (!cfm_output_file.is_open())
			{
				string ofname = outputFileNamePrePend + "ColdFusion";
				ofname += OUTPUT_FILE_NAME;
				cfm_output_file.open(ofname.c_str(), ofstream::out);

				if (!cfm_output_file.is_open()) return NULL;

				PrintFileHeader(cfm_output_file, "SLOC COUNT RESULTS", cmd);

				PrintFileHeaderLine(cfm_output_file, "RESULTS FOR ColdFusion FILES");
				cfm_output_file << endl << "                                    |       HTML      |     JS-Clnt     |    VBS-Clnt     |       SQL       |    ColdFusion   |     CFScript    |" << endl;
				if (legacyOutput)
				{
					cfm_output_file << "   Total   Blank |      Comments    |    Word  Exec.  |   Data   Exec.  |   Data   Exec.  |   Data   Exec.  |   Data   Exec.  |   Data   Exec.  |                          Logical SLOC                          | File  Module" << endl;
					cfm_output_file << "   Lines   Lines |   Whole Embedded |     LOC  Instr. |   Decl.  Instr. |   Decl.  Instr. |   Decl.  Instr. |   Decl.  Instr. |   Decl.  Instr. |   Total      HTML JS-Clnt VBS-Clnt     SQL ColdFusion CFScript | Type  Name" << endl;
					cfm_output_file << "-----------------+------------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+---------+------------------------------------------------------+---------------------------" << endl;
				}
				else
				{
					cfm_output_file << "   Total   Blank |      Comments    |    Word  Exec.  |   Data   Exec.  |   Data   Exec.  |   Data   Exec.  |   Data   Exec.  |   Data   Exec.  |                          Logical SLOC                          | Physical | File  Module" << endl;
					cfm_output_file << "   Lines   Lines |   Whole Embedded |     LOC  Instr. |   Decl.  Instr. |   Decl.  Instr. |   Decl.  Instr. |   Decl.  Instr. |   Decl.  Instr. |    HTML JS-Clnt VBS-Clnt     SQL ColdFusion CFScript     Total |   SLOC   | Type  Name" << endl;
					cfm_output_file << "-----------------+------------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+------------------------------------------------------+---------+----------+---------------------------" << endl;
				}
			}
			return &cfm_output_file;
		}
	}
	else if (webType == WEB_XML)
	{
		if (csvOutput)
		{
			if (!xml_output_file_csv.is_open())
			{
				string ofname = outputFileNamePrePend + "XML";
				ofname += OUTPUT_FILE_NAME_CSV;
				xml_output_file_csv.open(ofname.c_str(), ofstream::out);

				if (!xml_output_file_csv.is_open()) return NULL;

				PrintFileHeader(xml_output_file_csv, "SLOC COUNT RESULTS", cmd);

				PrintFileHeaderLine(xml_output_file_csv, "RESULTS FOR XML FILES");
				xml_output_file_csv << endl << ",,,,XML,,JS-Clnt,,VBS-Clnt,,C#-Clnt" << endl;
				xml_output_file_csv << "Total,Blank,Comments,,Word,Exec.,Data,Exec.,Data,Exec.,Data,Exec.,Logical SLOC,,,,,Physical,File,Module" << endl;
				xml_output_file_csv << "Lines,Lines,Whole,Embed,LOC,Instr,Decl,Instr,Decl,Instr,Decl,Instr,XML,JS-Clnt,VBS-Clnt,C#-Clnt,Total,SLOC,Type,Name" << endl;
			}
			return &xml_output_file_csv;
		}
		else
		{
			if (!xml_output_file.is_open())
			{
				string ofname = outputFileNamePrePend + "XML";
				ofname += OUTPUT_FILE_NAME;
				xml_output_file.open(ofname.c_str(), ofstream::out);

				if (!xml_output_file.is_open()) return NULL;

				PrintFileHeader(xml_output_file, "SLOC COUNT RESULTS", cmd);

				PrintFileHeaderLine(xml_output_file, "RESULTS FOR XML FILES");
				xml_output_file << endl << "                                    |       XML       |     JS-Clnt     |    VBS-Clnt     |     C#-Clnt     |" << endl;
				if (legacyOutput)
				{
					xml_output_file << "   Total   Blank |      Comments    |    Word  Exec.  |   Data   Exec.  |   Data   Exec.  |   Data   Exec.  |                Logical SLOC                | File  Module" << endl;
					xml_output_file << "   Lines   Lines |   Whole Embedded |     LOC  Instr. |   Decl.  Instr. |   Decl.  Instr. |   Decl.  Instr. |   Total       XML JS-Clnt VBS-Clnt C#-Clnt | Type  Name" << endl;
					xml_output_file << "-----------------+------------------+-----------------+-----------------+-----------------+-----------------+---------+----------------------------------+---------------------------" << endl;
				}
				else
				{
					xml_output_file << "   Total   Blank |      Comments    |    Word  Exec.  |   Data   Exec.  |   Data   Exec.  |   Data   Exec.  |                Logical SLOC                | Physical | File  Module" << endl;
					xml_output_file << "   Lines   Lines |   Whole Embedded |     LOC  Instr. |   Decl.  Instr. |   Decl.  Instr. |   Decl.  Instr. |     XML JS-Clnt VBS-Clnt C#-Clnt     Total |   SLOC   | Type  Name" << endl;
					xml_output_file << "-----------------+------------------+-----------------+-----------------+-----------------+-----------------+----------------------------------+---------+----------+---------------------------" << endl;
				}
			}
			return &xml_output_file;
		}
	}
	else // html
	{
		if (csvOutput)
		{
			if (!html_output_file_csv.is_open())
			{
				string ofname = outputFileNamePrePend + "HTML";
				ofname += OUTPUT_FILE_NAME_CSV;
				html_output_file_csv.open(ofname.c_str(), ofstream::out);

				if (!html_output_file_csv.is_open()) return NULL;

				PrintFileHeader(html_output_file_csv, "SLOC COUNT RESULTS", cmd);

				PrintFileHeaderLine(html_output_file_csv, "RESULTS FOR HTML FILES");
				html_output_file_csv << endl << ",,,,HTML,,JS-Clnt,,VBS-Clnt,,C#-Clnt" << endl;
				html_output_file_csv << "Total,Blank,Comments,,Word,Exec.,Data,Exec.,Data,Exec.,Data,Exec.,Logical SLOC,,,,,Physical,File,Module" << endl;
				html_output_file_csv << "Lines,Lines,Whole,Embed,LOC,Instr,Decl,Instr,Decl,Instr,Decl,Instr,HTML,JS-Clnt,VBS-Clnt,C#-Clnt,Total,SLOC,Type,Name" << endl;
			}
			return &html_output_file_csv;
		}
		else
		{
			if (!html_output_file.is_open())
			{
				string ofname = outputFileNamePrePend + "HTML";
				ofname += OUTPUT_FILE_NAME;
				html_output_file.open(ofname.c_str(), ofstream::out);

				if (!html_output_file.is_open()) return NULL;

				PrintFileHeader(html_output_file, "SLOC COUNT RESULTS", cmd);

				PrintFileHeaderLine(html_output_file, "RESULTS FOR HTML FILES");
				html_output_file << endl << "                                    |       HTML      |     JS-Clnt     |    VBS-Clnt     |     C#-Clnt     |" << endl;
				if (legacyOutput)
				{
					html_output_file << "   Total   Blank |      Comments    |    Word  Exec.  |   Data   Exec.  |   Data   Exec.  |   Data   Exec.  |                Logical SLOC                | File  Module" << endl;
					html_output_file << "   Lines   Lines |   Whole Embedded |     LOC  Instr. |   Decl.  Instr. |   Decl.  Instr. |   Decl.  Instr. |   Total      HTML JS-Clnt VBS-Clnt C#-Clnt | Type  Name" << endl;
					html_output_file << "-----------------+------------------+-----------------+-----------------+-----------------+-----------------+---------+----------------------------------+---------------------------" << endl;
				}
				else
				{
					html_output_file << "   Total   Blank |      Comments    |    Word  Exec.  |   Data   Exec.  |   Data   Exec.  |   Data   Exec.  |                Logical SLOC                | Physical | File  Module" << endl;
					html_output_file << "   Lines   Lines |   Whole Embedded |     LOC  Instr. |   Decl.  Instr. |   Decl.  Instr. |   Decl.  Instr. |    HTML JS-Clnt VBS-Clnt C#-Clnt     Total |   SLOC   | Type  Name" << endl;
					html_output_file << "-----------------+------------------+-----------------+-----------------+-----------------+-----------------+----------------------------------+---------+----------+---------------------------" << endl;
				}
			}
			return &html_output_file;
		}
	}
}

/*!
* Closes the web language output file streams.
*/
void CWebCounter::CloseOutputStream()
{
	if (php_output_file.is_open())
		php_output_file.close();
	if (asp_output_file.is_open())
		asp_output_file.close();
	if (jsp_output_file.is_open())
		jsp_output_file.close();
	if (html_output_file.is_open())
		html_output_file.close();
	if (xml_output_file.is_open())
		xml_output_file.close();
	if (cfm_output_file.is_open())
		cfm_output_file.close();
	if (php_output_file_csv.is_open())
		php_output_file_csv.close();
	if (asp_output_file_csv.is_open())
		asp_output_file_csv.close();
	if (jsp_output_file_csv.is_open())
		jsp_output_file_csv.close();
	if (html_output_file_csv.is_open())
		html_output_file_csv.close();
	if (xml_output_file_csv.is_open())
		xml_output_file_csv.close();
	if (cfm_output_file_csv.is_open())
		cfm_output_file_csv.close();
}

/*!
* Updates the web file extensions.
* Updates common file extensions list used in CCodeCounter
* as well as sorted file_exten_WEB used here.
*/
void CWebCounter::UpdateWebFileExt()
{
	size_t i;
	file_extension.clear();
//	file_exten_WEB.clear();

	for (i = 0; i < file_exten_php.size(); i++)
	{
		file_extension.push_back(file_exten_php[i]);
//		file_exten_WEB.push_back(file_exten_php[i]);
	}
	for (i = 0; i < file_exten_asp.size(); i++)
	{
		file_extension.push_back(file_exten_asp[i]);
//		file_exten_WEB.push_back(file_exten_asp[i]);
	}
	for (i = 0; i < file_exten_jsp.size(); i++)
	{
		file_extension.push_back(file_exten_jsp[i]);
//		file_exten_WEB.push_back(file_exten_jsp[i]);
	}
	for (i = 0; i < file_exten_htm.size(); i++)
	{
		file_extension.push_back(file_exten_htm[i]);
//		file_exten_WEB.push_back(file_exten_htm[i]);
	}
	for (i = 0; i < file_exten_xml.size(); i++)
	{
		file_extension.push_back(file_exten_xml[i]);
//		file_exten_WEB.push_back(file_exten_xml[i]);
	}
	for (i = 0; i < file_exten_cfm.size(); i++)
	{
		file_extension.push_back(file_exten_cfm[i]);
//		file_exten_WEB.push_back(file_exten_cfm[i]);
	}

/* COMMENTED OUT  Defects -- Needs more work  Randy
	// Sort file_exten_WEB and generate the related Language array
	sort( file_exten_WEB.begin(), file_exten_WEB.end() );

	// Build related Language (enum index)
	for ( i = 0; i < file_exten_WEB.size(); i++ )
	{
		WebType web_type = GetWebType( file_exten_WEB[ i ] );
	#ifdef	_DEBUG
		if ( WEB_UNKNOWN == web_type )
			printf( "\nUpdateWebFileExt found Unknown web type.\n" );
	#endif

		file_exten_WEB_Langs.push_back( web_type );
	}
*/
}


/*!
* Determines the type of web language a file is categorized under.
*
* \param fname file name
*
* \return type of web language (WEB_PHP, WEB_ASP, WEB_JSP, WEB_HTM, WEB_XML, WEB_CFM)
*/
/*  COMMENTED OUT  Defects -- needs more work.  Randy
WebType CWebCounter::GetWebTypeFast_with_Defects( const string &fname )
{
	WebType retVal = WEB_UNKNOWN;

	string file_ext = fname.substr( fname.find_last_of( "." ) );
	file_ext = CUtil::ToLower( file_ext );

	StringVector::iterator itWebExt = find( file_exten_WEB.begin(), file_exten_WEB.end(), file_ext );
	if ( file_exten_WEB.end() != itWebExt )
		retVal = (WebType)file_exten_WEB_Langs[ (unsigned)distance( file_exten_WEB.begin(), itWebExt ) ];
	else
		retVal = WEB_UNKNOWN;	// Breakpoint here if getting this return

	return retVal;
}
*/
/*!
* Determines the type of web language a file is categorized under.
*
* \param fname file name
*
* \return type of web language (WEB_PHP, WEB_ASP, WEB_JSP, WEB_HTM, WEB_XML, WEB_CFM)
*/
WebType CWebCounter::GetWebType(const string &fname)
{
	WebType retVal = WEB_UNKNOWN;

	string file_ext = fname.substr(fname.find_last_of("."));
	file_ext = CUtil::ToLower(file_ext);

	if (find(file_exten_php.begin(), file_exten_php.end(), file_ext) != file_exten_php.end())
		retVal = WEB_PHP;

	if (find(file_exten_asp.begin(), file_exten_asp.end(), file_ext) != file_exten_asp.end())
		retVal = WEB_ASP;

	if (find(file_exten_jsp.begin(), file_exten_jsp.end(), file_ext) != file_exten_jsp.end())
		retVal = WEB_JSP;

	if (find(file_exten_xml.begin(), file_exten_xml.end(), file_ext) != file_exten_xml.end())
		retVal = WEB_XML;

	if (find(file_exten_cfm.begin(), file_exten_cfm.end(), file_ext) != file_exten_cfm.end())
		retVal = WEB_CFM;

	if (find(file_exten_htm.begin(), file_exten_htm.end(), file_ext) != file_exten_htm.end())
		retVal = WEB_HTM;

	return retVal;
}

/*!
* Determines the name of a specified web language type.
*
* \param webType web language type
*
* \return name of web language (PHP, ASP, JSP, HTML, XML, ColdFusion)
*/
string CWebCounter::GetWebLangName(WebType webType)
{
	switch (webType)
	{
	case WEB_PHP:
		return("PHP");
	case WEB_ASP:
		return("ASP");
	case WEB_JSP:
		return("JSP");
	case WEB_XML:
		return("XML");
	case WEB_CFM:
		return("ColdFusion");
	case WEB_HTM:
		return("HTML");
	case WEB_UNKNOWN:
	default:
		return("HTML");
	}
}

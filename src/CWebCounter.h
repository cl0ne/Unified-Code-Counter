//! Code counter class definition for web languages.
/*!
* \file CWebCounter.h
*
* This file contains the code counter class definition for web languages.
*/

#ifndef CWebCounter_h
#define CWebCounter_h

#include "CCodeCounter.h"

enum WebType { WEB_UNKNOWN, WEB_PHP, WEB_ASP, WEB_JSP, WEB_HTM, WEB_XML, WEB_CFM };

//! Web languages code counter class.
/*!
* \class CWebCounter
*
* Defines the web languages code counter class.
*/
class CWebCounter : public CCodeCounter
{
public:
	CWebCounter();
	void CleanSeparatedFile();
	virtual int CountSLOC(filemap* fmap, results* result);
	map<int, SourceFileElement>* GetSeparationMap() { return &Separation; }
	using CCodeCounter::GetOutputStream; // warning fix
	virtual ofstream* GetOutputStream(WebType webType, const string &outputFileNamePrePend = "",
		const string &cmd = "", bool csvOutput = false, bool legacyOutput = false);
	virtual void CloseOutputStream();
	void UpdateWebFileExt();
	WebType GetWebType(const string &fname);
	static string GetWebLangName(WebType webType);

	StringVector web_lang_names;				//!< List of web language names;

	StringVector file_exten_php;				//!< File extensions for PHP
	StringVector file_exten_asp;				//!< File extensions for ASP
	StringVector file_exten_jsp;				//!< File extensions for JSP
	StringVector file_exten_htm;				//!< File extensions for HTML
	StringVector file_exten_xml;				//!< File extensions for XML
	StringVector file_exten_cfm;				//!< File extensions for ColdFusion

//	StringVector file_exten_WEB;				//!< Sorted of above for faster lookup NOT WORKING
//	UIntVector   file_exten_WEB_Langs;			//!< Related language per file_exten_WEB entry

	unsigned int total_php_filesA;				//!< Total PHP files in baseline A
	unsigned int total_asp_filesA;				//!< Total ASP files in baseline A
	unsigned int total_jsp_filesA;				//!< Total JSP files in baseline A
	unsigned int total_htm_filesA;				//!< Total HTML files in baseline A
	unsigned int total_xml_filesA;				//!< Total XML files in baseline A
	unsigned int total_cfm_filesA;				//!< Total ColdFusion files in baseline A
	unsigned int total_php_filesB;				//!< Total PHP files in baseline B
	unsigned int total_asp_filesB;				//!< Total ASP files in baseline B
	unsigned int total_jsp_filesB;				//!< Total JSP files in baseline B
	unsigned int total_htm_filesB;				//!< Total HTML files in baseline B
	unsigned int total_xml_filesB;				//!< Total XML files in baseline B
	unsigned int total_cfm_filesB;				//!< Total ColdFusion files in baseline B
	unsigned int total_php_dupFilesA;			//!< Total duplicate PHP files in baseline A
	unsigned int total_asp_dupFilesA;			//!< Total duplicate ASP files in baseline A
	unsigned int total_jsp_dupFilesA;			//!< Total duplicate JSP files in baseline A
	unsigned int total_htm_dupFilesA;			//!< Total duplicate HTML files in baseline A
	unsigned int total_xml_dupFilesA;			//!< Total duplicate XML files in baseline A
	unsigned int total_cfm_dupFilesA;			//!< Total duplicate ColdFusion files in baseline A
	unsigned int total_php_dupFilesB;			//!< Total duplicate PHP files in baseline B
	unsigned int total_asp_dupFilesB;			//!< Total duplicate ASP files in baseline B
	unsigned int total_jsp_dupFilesB;			//!< Total duplicate JSP files in baseline B
	unsigned int total_htm_dupFilesB;			//!< Total duplicate HTML files in baseline B
	unsigned int total_xml_dupFilesB;			//!< Total duplicate XML files in baseline B
	unsigned int total_cfm_dupFilesB;			//!< Total duplicate ColdFusion files in baseline B

protected:
	
	void CleanSeparatedLine();

	map<int, SourceFileElement> Separation;		//!< Separation file map
	map<int, lineElement> SeparatedLine;		//!< Separated line
	map<string, int> TagTable;					//!< Script language tags
	map<string, int> ScriptTable;				//!< Supported script languages
	stack<int> PreviousLanguage;				//!< Previous language stack

	ofstream php_output_file;					//!< PHP output file stream
	ofstream asp_output_file;					//!< ASP output file stream
	ofstream jsp_output_file;					//!< JSP output file stream
	ofstream html_output_file;					//!< HTML output file stream
	ofstream xml_output_file;					//!< XML output file stream
	ofstream cfm_output_file;					//!< ColdFusion output file stream

	ofstream php_output_file_csv;				//!< PHP output CSV file stream
	ofstream asp_output_file_csv;				//!< ASP output CSV file stream
	ofstream jsp_output_file_csv;				//!< JSP output CSV file stream
	ofstream html_output_file_csv;				//!< HTML output CSV file stream
	ofstream xml_output_file_csv;				//!< XML output CSV file stream
	ofstream cfm_output_file_csv;				//!< ColdFusion output CSV file stream

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CWebCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CWebCounter(const CWebCounter& rhs);

	// Take care of warning C4626: 'CWebCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CWebCounter operator=(const CWebCounter);
};

#endif

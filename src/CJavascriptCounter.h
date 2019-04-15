//! Code counter class definition for the JavaScript language.
/*!
* \file CJavascriptCounter.h
*
* This file contains the code counter class definition for the JavaScript language.
*/

#ifndef CJavascriptCounter_h
#define CJavascriptCounter_h

#include "CCodeCounter.h"

//! JavaScript code counter class.
/*!
* \class CJavascriptCounter
*
* Defines the JavaScript code counter class.
*/
class CJavascriptCounter : public CCodeCounter
{
public:
	CJavascriptCounter();

protected:
	virtual int ReplaceQuote(string &strline, size_t &idx_start, bool &contd, char &CurrentQuoteEnd);
	virtual int CountDirectiveSLOC(filemap* fmap, results* result, filemap* fmapBak = NULL);
	virtual int LanguageSpecificProcess(filemap* fmap, results* result, filemap* fmapBak = NULL);
	void LSLOC(results* result, string line, size_t lineNumber, string lineBak, string &strLSLOC, string &strLSLOCBak, 
		unsigned int &paren_cnt, bool &forflag, bool &found_forifwhile, bool &found_while, char &prev_char, 
		bool &data_continue, unsigned int &temp_lines, unsigned int &phys_exec_lines, unsigned int &phys_data_lines, 
		bool &inArrayDec, unsigned int &openBrackets, StringVector &loopLevel);
	int ParseFunctionName(const string &line, string &/*lastline*/, filemap &functionStack, string &functionName, 
		unsigned int &functionCount);

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CJavascriptCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CJavascriptCounter(const CJavascriptCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CJavascriptCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CJavascriptCounter operator=(const CJavascriptCounter);    // Declare without implementation
};

//! JavaScript in PHP code counter class.
/*!
* \class CJavascriptPhpCounter
*
* Defines the JavaScript in PHP code counter class.
*/
class CJavascriptPhpCounter : public CJavascriptCounter
{
public:
	CJavascriptPhpCounter();

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CJavascriptPhpCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CJavascriptPhpCounter(const CJavascriptPhpCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CJavascriptPhpCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CJavascriptPhpCounter operator=(const CJavascriptPhpCounter);    // Declare without implementation
};

//! JavaScript in HTML code counter class.
/*!
* \class CJavascriptHtmlCounter
*
* Defines the JavaScript in HTML code counter class.
*/
class CJavascriptHtmlCounter : public CJavascriptCounter
{
public:
	CJavascriptHtmlCounter();

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CJavascriptHtmlCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CJavascriptHtmlCounter(const CJavascriptHtmlCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CJavascriptHtmlCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CJavascriptHtmlCounter operator=(const CJavascriptHtmlCounter);    // Declare without implementation
};

//! JavaScript in XML code counter class.
/*!
* \class CJavascriptXmlCounter
*
* Defines the JavaScript in XML code counter class.
*/
class CJavascriptXmlCounter : public CJavascriptCounter
{
public:
	CJavascriptXmlCounter();

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CJavascriptXmlCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CJavascriptXmlCounter(const CJavascriptXmlCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CJavascriptXmlCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CJavascriptXmlCounter operator=(const CJavascriptXmlCounter);    // Declare without implementation
};

//! JavaScript in JSP code counter class.
/*!
* \class CJavascriptJspCounter
*
* Defines the JavaScript in JSP code counter class.
*/
class CJavascriptJspCounter : public CJavascriptCounter
{
public:
	CJavascriptJspCounter();

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CJavascriptJspCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CJavascriptJspCounter(const CJavascriptJspCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CJavascriptJspCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CJavascriptJspCounter operator=(const CJavascriptJspCounter);    // Declare without implementation
};

//! JavaScript in ASP server code counter class.
/*!
* \class CJavascriptAspServerCounter
*
* Defines the JavaScript in ASP server code counter class.
*/
class CJavascriptAspServerCounter : public CJavascriptCounter
{
public:
	CJavascriptAspServerCounter();

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CJavascriptAspServerCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CJavascriptAspServerCounter(const CJavascriptAspServerCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CJavascriptAspServerCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CJavascriptAspServerCounter operator=(const CJavascriptAspServerCounter);    // Declare without implementation
};

//! JavaScript in ASP client code counter class.
/*!
* \class CJavascriptAspClientCounter
*
* Defines the JavaScript in ASP client code counter class.
*/
class CJavascriptAspClientCounter : public CJavascriptCounter
{
public:
	CJavascriptAspClientCounter();

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CJavascriptAspClientCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CJavascriptAspClientCounter(const CJavascriptAspClientCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CJavascriptAspClientCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CJavascriptAspClientCounter operator=(const CJavascriptAspClientCounter);    // Declare without implementation
};

//! JavaScript in ColdFusion code counter class.
/*!
* \class CJavascriptColdFusionCounter
*
* Defines the JavaScript in ColdFusion code counter class.
*/
class CJavascriptColdFusionCounter : public CJavascriptCounter
{
public:
	CJavascriptColdFusionCounter();

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CJavascriptColdFusionCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CJavascriptColdFusionCounter(const CJavascriptColdFusionCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CJavascriptColdFusionCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CJavascriptColdFusionCounter operator=(const CJavascriptColdFusionCounter);    // Declare without implementation
};

#endif

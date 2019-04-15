//! Code counter class definition for the Ruby language.
/*!
* \file CRubyCounter.h
*
* This file contains the code counter class definition for the Ruby language.
*/

#ifndef CRubyCounter_h
#define CRubyCounter_h

#include "CCodeCounter.h"

//! Ruby code counter class.
/*!
* \class CRubyCounter
*
* Defines the Ruby code counter class.
*/
class CRubyCounter : public CCodeCounter
{
public:
	CRubyCounter();

protected:
	virtual int ReplaceQuote(string &strline, size_t &idx_start, bool &contd, char &CurrentQuoteEnd);
	virtual int LanguageSpecificProcess(filemap* fmap, results* result, filemap* fmapBak = NULL);
	void LSLOC(results* result, string line, size_t lineNumber, string lineBak, string &strLSLOC, string &strLSLOCBak);
	int ParseFunctionName(const string &line, string &lastline,
		filemap &functionStack, string &functionName, unsigned int &functionCount);

	string delimiter;	// used to store delimiter of string literals across lines

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CRubyCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CRubyCounter(const CRubyCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CRubyCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CRubyCounter operator=(const CRubyCounter);    // Declare without implementation
};

#endif

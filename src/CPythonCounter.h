//! Code counter class definition for the Python language.
/*!
* \file CPythonCounter.h
*
* This file contains the code counter class definition for the Python language.
*/

#ifndef CPythonCounter_h
#define CPythonCounter_h

#include "CCodeCounter.h"

//! Python code counter class.
/*!
* \class CPythonCounter
*
* Defines the Python code counter class.
*/
class CPythonCounter : public CCodeCounter
{
public:
	CPythonCounter();

protected:
	StringVector loop_keywords;		//!< List of keywords to indicate the beginning of a loop
	using CCodeCounter::ReplaceQuote; // fix warning
	virtual int ReplaceQuote(string &strline, size_t &idx_start, bool &contd, string &CurrentQuoteEnd);
	virtual int CountCommentsSLOC(filemap* fmap, results* result, filemap* fmapBak = NULL);
	virtual int CountDirectiveSLOC(filemap* fmap, results* result, filemap* fmapBak = NULL);
	virtual int LanguageSpecificProcess(filemap* fmap, results* result, filemap* fmapBak = NULL);
	void LSLOC(results* result, string line, size_t lineNumber, string lineBak, string &strLSLOC, string &strLSLOCBak,
		unsigned int &paren_cnt, UIntVector &loopWhiteSpace);
	using CCodeCounter::ParseFunctionName; // fix warning
	int ParseFunctionName(const string &line, string &lastline, StringVector &functionStack, string &functionName, vector<int> &condenStack, int & numWS);
    	int CountComplexity(filemap* fmap, results* result);

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CPythonCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CPythonCounter(const CPythonCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CPythonCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CPythonCounter operator=(const CPythonCounter);    // Declare without implementation
};

#endif

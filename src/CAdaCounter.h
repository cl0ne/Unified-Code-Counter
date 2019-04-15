//! Code counter class definition for the Ada language.
/*!
* \file CAdaCounter.h
*
* This file contains the code counter class definition for the Ada language.
*/

#ifndef AdaCounter_h
#define AdaCounter_h

#include "CCodeCounter.h"

//! Ada code counter class.
/*!
* \class CAdaCounter
*
* Defines the Ada code counter class.
*/
class CAdaCounter : public CCodeCounter
{
public:
	CAdaCounter();

protected:
	virtual int CountDirectiveSLOC(filemap* fmap, results* result, filemap* fmapBak = NULL);
	virtual int LanguageSpecificProcess(filemap* fmap, results* result, filemap* fmapBak = NULL);
	virtual int ReplaceQuote(string &strline, size_t &idx_start, bool &contd, char &CurrentQuoteEnd);
	void LSLOC(results* result, string line, size_t lineNumber, string lineBak, string &strLSLOC, string &strLSLOCBak, 
		unsigned int &paren_cnt, bool &forflag, bool &found_forifwhile, bool &found_while, bool &found_type, bool &found_is, bool 			&found_accept, unsigned int &loopLevel);
	void FoundSLOC(results* result, size_t lineNumber, string &strLSLOC, string &strLSLOCBak, bool &found_block, bool &found_forifwhile,
		bool &found_end, bool &found_type, bool &found_is, bool &found_accept, bool &trunc_flag);
	int ParseFunctionName(const string &line, string &lastline,
	filemap &functionStack, string &functionName, unsigned int &functionCount);

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CAdaCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CAdaCounter(const CAdaCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CAdaCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CAdaCounter operator=(const CAdaCounter);    // Declare without implementation
};

#endif

//! Code counter class definition for the Perl language.
/*!
* \file CPerlCounter.h
*
* This file contains the code counter class definition for the Perl language.
*/

#ifndef CPerlCounter_h
#define CPerlCounter_h

#include "CCodeCounter.h"

//! Perl code counter class.
/*!
* \class CPerlCounter
*
* Defines the Perl code counter class.
*/
class CPerlCounter : public CCodeCounter
{
public:
	CPerlCounter();

protected:
	virtual int PreCountProcess(filemap* fmap);
	virtual int ReplaceQuote(string &strline, size_t &idx_start, bool &contd, char &CurrentQuoteEnd);
	virtual int CountDirectiveSLOC(filemap* fmap, results* result, filemap* fmapBak = NULL);
	virtual int LanguageSpecificProcess(filemap* fmap, results* result, filemap* fmapBak = NULL);
	void LSLOC(results* result, string line, size_t lineNumber, string lineBak, string &strLSLOC, string &strLSLOCBak, 
		unsigned int &paren_cnt, bool &forflag, bool &found_forifwhile, bool &found_while, char &prev_char, 
		bool &data_continue, unsigned int &temp_lines, unsigned int &phys_exec_lines, unsigned int &phys_data_lines,
		unsigned int &openBrackets, StringVector &loopLevel);
	int ParseFunctionName(const string &line, string &lastline,
		filemap &functionStack, string &functionName, unsigned int &functionCount);

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CPerlCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CPerlCounter(const CPerlCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CPerlCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CPerlCounter operator=(const CPerlCounter);    // Declare without implementation
};

#endif

//! Code counter class definition for the PHP language.
/*!
* \file CPhpCounter.h
*
* This file contains the code counter class definition for the PHP language.
*/

#ifndef CPhpCounter_h
#define CPhpCounter_h

#include "CCodeCounter.h"

//! PHP code counter class.
/*!
* \class CPhpCounter
*
* Defines the PHP code counter class.
* NOTE: PHP variables are case sensitive, but PHP functions are case insensitive.
*/
class CPhpCounter : public CCodeCounter
{
public:
	CPhpCounter();

protected:
	StringVector exclude_loop;				//!< List of keywords to exclude for loops

	virtual int CountDirectiveSLOC(filemap* fmap, results* result, filemap* fmapBak = NULL);
	virtual int LanguageSpecificProcess(filemap* fmap, results* result, filemap* fmapBak = NULL);
	void LSLOC(results* result, string line, size_t lineNumber, string lineBak, string &strLSLOC, string &strLSLOCBak, 
		unsigned int &paren_cnt, bool &forflag, bool &found_forifwhile, bool &found_while, char &prev_char, 
		bool &data_continue, unsigned int &temp_lines, unsigned int &phys_exec_lines, unsigned int &phys_data_lines,
		bool &inArrayDec, bool &found_for, StringVector &loopLevel);
	int ParseFunctionName(const string &line, string &lastline,
		filemap &functionStack, string &functionName, unsigned int &functionCount);

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CPhpCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CPhpCounter(const CPhpCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CPhpCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CPhpCounter operator=(const CPhpCounter);    // Declare without implementation
};

#endif

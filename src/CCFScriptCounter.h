//! Code counter class definition for the CFScript language.
/*!
* \file CCFScriptCounter.h
*
* This file contains the code counter class definition for the CFScript language.
*/

#ifndef CCFScriptCounter_h
#define CCFScriptCounter_h

#include "CCodeCounter.h"

//! CFScript code counter class.
/*!
* \class CCFScriptCounter
*
* Defines the CFScript code counter class.
*/
class CCFScriptCounter : public CCodeCounter
{
public:
	CCFScriptCounter();

protected:
	virtual int LanguageSpecificProcess(filemap* fmap, results* result, filemap* fmapBak = NULL);
	void LSLOC(results* result, string line, size_t lineNumber, string lineBak, string &strLSLOC, string &strLSLOCBak, 
		unsigned int &paren_cnt, bool &forflag, bool &found_forifwhile, bool &found_while, char &prev_char, 
		bool &data_continue, unsigned int &temp_lines, unsigned int &phys_exec_lines, unsigned int &phys_data_lines, 
		bool &inArrayDec, unsigned int &openBrackets, StringVector &loopLevel);
	int ParseFunctionName(const string &line, string &lastline, filemap &functionStack, string &functionName, 
		unsigned int &functionCount);

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CCFScriptCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CCFScriptCounter(const CCFScriptCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CCFScriptCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CCFScriptCounter operator=(const CCFScriptCounter);    // Declare without implementation
};

#endif

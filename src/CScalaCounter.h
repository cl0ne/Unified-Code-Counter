//! Code counter class definition for the Scala language.
/*!
* \file CScalaCounter.h
*
* This file contains the code counter class definition for the Scala language.
*/

#ifndef CScalaCounter_h
#define CScalaCounter_h

#include "CCJavaCsScalaCounter.h"

//! Scala code counter class.
/*!
* \class CScalaCounter
*
* Defines the Scala code counter class.
*/

// Inherit from CCJavaCsScalaCounter base class to get 
// method/data interfaces to more extensive complexity support
class CScalaCounter : public CCJavaCsScalaCounter
{
public:
	// Set the language so base class constructors set values as needed
	CScalaCounter(string lang = "SCALA");

protected:
	virtual int CountDirectiveSLOC(filemap* fmap, results* result, filemap* fmapmBak = NULL);
	virtual int LanguageSpecificProcess(filemap* fmap, results* result, filemap* fmapmBak = NULL);
	void LSLOC(results* result, string line, string lineBak, string &strLSLOC, string &strLSLOCBak, unsigned int &paren_cnt,
		bool &forflag, bool &found_forifwhile, bool &found_while, char &prev_char, bool &data_continue,
		unsigned int &temp_lines, unsigned int &phys_exec_lines, unsigned int &phys_data_lines,
		bool &inArrayDec, bool &found_for, unsigned int &openBrackets, StringVector &loopLevel);
	virtual int ParseFunctionName(const string &line, string &lastline,
		filemap &functionStack, string &functionName, unsigned int &functionCount);

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CScalaCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CScalaCounter(const CScalaCounter& rhs);

	// Take care of warning C4626: 'CScalaCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CScalaCounter operator=(const CScalaCounter);
};

#endif

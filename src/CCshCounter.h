//! Code counter class definition for the C shell script language.
/*!
* \file CCshCounter.h
*
* This file contains the code counter class definition for the C shell script language.
* This also includes the Tcsh language.
*/

#ifndef CCshCounter_h
#define CCshCounter_h

#include "CCodeCounter.h"

//! C shell script code counter class.
/*!
* \class CCshCounter
*
* Defines the C shell script code counter class.
*/
class CCshCounter : public CCodeCounter
{
public:
	CCshCounter();

protected:
	virtual int PreCountProcess(filemap* fmap);
	virtual int LanguageSpecificProcess(filemap* fmap, results* result, filemap* fmapBak = NULL);
	void LSLOC(results* result, string line, size_t lineNumber, string lineBak, string &strLSLOC, string &strLSLOCBak,
		bool &data_continue, unsigned int &temp_lines, unsigned int &phys_exec_lines, unsigned int &phys_data_lines, 
		unsigned int &loopLevel);
	using CCodeCounter::ParseFunctionName; // fix warning
	int ParseFunctionName(const string & /*line*/, string & /*lastline*/, filemap & /*functionStack*/, string & /*functionName*/) { return 2; }
    	int CountComplexity(filemap* fmap, results* result);
    	string echoHelper(string line);

	StringVector continue_keywords;		//!< List of keywords to continue to next line

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CCshCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CCshCounter(const CCshCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CCshCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CCshCounter operator=(const CCshCounter);    // Declare without implementation
};

#endif

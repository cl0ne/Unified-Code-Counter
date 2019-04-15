//! Code counter class definition for the Visual Basic language.
/*!
* \file CVbCounter.h
*
* This file contains the code counter class definition for the Visual Basic language.
*/

#ifndef CVbCounter_h
#define CVbCounter_h

#include "CCodeCounter.h"

//! Visual Basic code counter class.
/*!
* \class CVbCounter
*
* Defines the Visual Basic code counter class.
*/
class CVbCounter : public CCodeCounter
{
public:
	CVbCounter();

protected:
	virtual int CountDirectiveSLOC(filemap* fmap, results* result, filemap* fmapBak = NULL);
	virtual int LanguageSpecificProcess(filemap* fmap, results* result, filemap* fmapBak = NULL);
	int ParseFunctionName(const string &line, string &lastline,
		filemap &functionStack, string &functionName, unsigned int &functionCount);

	StringVector exclude_start_keywords;		//!< SLOC lines excluded from counts starting with keywords

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CVbCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CVbCounter(const CVbCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CVbCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CVbCounter operator=(const CVbCounter);    // Declare without implementation};
};

#endif

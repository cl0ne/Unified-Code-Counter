//! Code counter class definition for Makefiles.
/*!
* \file CMakefileCounter.h
*
* This file contains the code counter class definition for Makefiles.
*/

#ifndef CMakefileCounter_h
#define CMakefileCounter_h

#include "CCodeCounter.h"

//! Makefile code counter class.
/*!
* \class CMakefileCounter
*
* Defines the Makefile code counter class.
*/
class CMakefileCounter : public CCodeCounter
{
public:
	CMakefileCounter();

protected:
	virtual int CountDirectiveSLOC(filemap* fmap, results* result, filemap* fmapBak = NULL);
	virtual int LanguageSpecificProcess(filemap* fmap, results* result, filemap* fmapBak = NULL);
	void LSLOC(results* result, string line, size_t lineNumber, string lineBak, string &strLSLOC, string &strLSLOCBak);

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CMakefileCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CMakefileCounter(const CMakefileCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CMakefileCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CMakefileCounter operator=(const CMakefileCounter);    // Declare without implementation
};

#endif

//! Code counter class definition for the Matlab language.
/*!
* \file CMatlabCounter.h
*
* This file contains the code counter class definition for the Matlab Language.
*/

#ifndef CMatlabCounter_h
#define CMatlabCounter_h

#include "CCodeCounter.h"

//! Matlab code counter class.
/*!
* \class CMatlabCounter
*
* Defines the Matlab code counter class.
*/
class CMatlabCounter : public CCodeCounter
{
public:
	CMatlabCounter();

protected:
	virtual int CountDirectiveSLOC(filemap* fmap, results* result, filemap* fmapBak = NULL);
	virtual int LanguageSpecificProcess(filemap* fmap, results* result, filemap* fmapBak = NULL);
	void LSLOC(results* result, string line, size_t lineNumber, string lineBak, string &strLSLOC, string &strLSLOCBak,
		bool &cont_str, unsigned int &openBrackets, StringVector &loopLevel);
	int ParseFunctionName(const string &line, string &lastline,
		filemap &functionStack, string &functionName, unsigned int &functionCount);

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CMatlabCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CMatlabCounter(const CMatlabCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CMatlabCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CMatlabCounter operator=(const CMatlabCounter);    // Declare without implementation
};

#endif

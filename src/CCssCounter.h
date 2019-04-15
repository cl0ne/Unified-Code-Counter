//! Code counter class definition for the cascading style sheet (CSS) language.
/*!
* \file CCssCounter.h
*
* This file contains the code counter class definition for the cascading style sheet (CSS) language.
*/

#ifndef CCssCounter_h
#define CCssCounter_h

#include "CCodeCounter.h"

//! CSS code counter class.
/*!
* \class CCssCounter
*
* Defines the CSS code counter class.
*/
class CCssCounter : public CCodeCounter
{
public:
	CCssCounter();

protected:
	virtual int LanguageSpecificProcess(filemap* fmap, results* result, filemap* fmapBak = NULL);
	void LSLOC(results* result, string &line, size_t lineNumber, string &strLSLOC, char &lastLinesLastChar,
		unsigned int &phys_exec_lines, unsigned int &phys_data_lines);

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CCssCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CCssCounter(const CCssCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CCssCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CCssCounter operator=(const CCssCounter);    // Declare without implementation
};

#endif

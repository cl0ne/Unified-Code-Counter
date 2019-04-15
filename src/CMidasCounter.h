//! Code counter class definition for the Midas macro languages.
/*!
* \file CMidasCounter.h
*
* This file contains the code counter class definition for the Midas macro languages.
*/

#ifndef CMidasCounter_h
#define CMidasCounter_h

#include "CCodeCounter.h"

//! Midas code counter class.
/*!
* \class CMidasCounter
*
* Defines the Midas code counter class.
*/
class CMidasCounter : public CCodeCounter
{
public:
	CMidasCounter();

protected:
	virtual int CountDirectiveSLOC(filemap* fmap, results* result, filemap* fmapBak = NULL);
	virtual int LanguageSpecificProcess(filemap* fmap, results* result, filemap* fmapBak = NULL);
	void LSLOC(results* result, string line, size_t lineNumber, string lineBak, string &strLSLOC, string &strLSLOCBak,
		bool &data_continue, unsigned int &temp_lines, unsigned int &phys_exec_lines,
		unsigned int &phys_data_lines, StringVector &loopEnd);
	//Modification 2016.10; USC
	int CountComplexity(filemap* fmap, results* result);

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CMidasCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CMidasCounter(const CMidasCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CMidasCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CMidasCounter operator=(const CMidasCounter);    // Declare without implementation
};

#endif

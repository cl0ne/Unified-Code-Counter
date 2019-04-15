//! Code counter class definition for the Fortran language.
/*!
* \file CFortranCounter.h
*
* This file contains the code counter class definition for the Fortran language.
* This includes F77, F90, F95, and F03 including fixed and free formats.
*/

#ifndef CFortranCounter_h
#define CFortranCounter_h

#include "CCodeCounter.h"

//! Fortran code counter class.
/*!
* \class CFortranCounter
*
* Defines the Fortran code counter class.
*/
class CFortranCounter :  public CCodeCounter
{
public:
	CFortranCounter();

protected:
	StringVector c_keywords;				//!< List of keywords starting with 'c' with flexible formats

	virtual int PreCountProcess(filemap* fmap);
	virtual int CountCommentsSLOC(filemap* fmap, results* result, filemap* fmapBak = NULL);
	virtual int CountDirectiveSLOC(filemap* fmap, results* result, filemap* fmapBak = NULL);
	virtual int LanguageSpecificProcess(filemap* fmap, results* result, filemap* fmapBak = NULL);
	void LSLOC(results* result, string line, size_t lineNumber, string lineBak, string &strLSLOC, string &strLSLOCBak,
		bool &fixed_continue, bool &data_continue, unsigned int &temp_lines, unsigned int &phys_exec_lines, 
		unsigned int &phys_data_lines, StringVector &loopEnd);
	int ParseFunctionName(const string &line, string &lastline, filemap &functionStack, string &functionName, 
		unsigned int &functionCount);

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CFortranCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CFortranCounter(const CFortranCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CFortranCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CFortranCounter operator=(const CFortranCounter);    // Declare without implementation
};

#endif

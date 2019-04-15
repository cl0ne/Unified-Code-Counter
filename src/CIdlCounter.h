//! Code counter class definition for the Interactive Data Language (IDL).
/*!
* \file CIdlCounter.h
*
* This file contains the code counter class definition for the Interactive Data Language (IDL).
*/

#ifndef CIdlCounter_h
#define CIdlCounter_h

#include "CCodeCounter.h"

//! IDL code counter class.
/*!
* \class CIdlCounter
*
* Defines the IDL code counter class.
*/
class CIdlCounter : public CCodeCounter
{
public:
    CIdlCounter();

protected:
    virtual int PreCountProcess(filemap* fmap);
    virtual int CountDirectiveSLOC(filemap* fmap, results* result, filemap* fmapBak = NULL);
    virtual int LanguageSpecificProcess(filemap* fmap, results* result, filemap* fmapBak = NULL);
    void LSLOC(results* result, string line, string lineBak, string &strLSLOC, string &strLSLOCBak,
        bool &cont_str, unsigned int &openBrackets, StringVector &loopLevel);
    int ParseFunctionName(const string &line, string &lastline,
        filemap &functionStack, string &functionName, unsigned int &functionCount);

    StringVector statement_list;

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CIdlCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CIdlCounter(const CIdlCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CIdlCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CIdlCounter operator=(const CIdlCounter);    // Declare without implementation
};

#endif

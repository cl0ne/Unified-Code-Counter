//! Code counter class definition for assembly languages
/*!
* \file CAssemblyCounter.h
*
* This file contains the code counter class definition for assembly languages.
*/

#ifndef CAssemblyCounter_h
#define CAssemblyCounter_h

#include "CCodeCounter.h"

//! Assembly code counter class.
/*!
* \class CAssemblyCounter
*
* Defines the assembly code counter class.
*/
class CAssemblyCounter : public CCodeCounter
{
public:
    CAssemblyCounter();

protected:
    virtual int PreCountProcess(filemap* /*fmap*/);
    virtual int LanguageSpecificProcess(filemap* fmap, results* result, filemap* fmapmBak = NULL);
    int SwitchSectionMode(const string &statement);     // data mode or text mode
    void FindLineCommentMarker(filemap* fmap);
    StringVector data_declaration_code;                 //!< storage mapping codes to define data declaration statements
    string statementSeparator;

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CAssemblyCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CAssemblyCounter(const CAssemblyCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CAssemblyCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CAssemblyCounter operator=(const CAssemblyCounter);    // Declare without implementation
};

#endif

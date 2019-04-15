//! Code counter class definition for the Cobol language.
/*!
* \file CCobolCounter.h
*
* This file contains the code counter class definition for the Cobol language.
*/

#ifndef CCobolCounter_h
#define CCobolCounter_h

#include "CCodeCounter.h"

//! Cobol code counter class.
/*!
* \class CCobolCounter
*
* Defines the Cobol code counter class.
*/
class CCobolCounter : public CCodeCounter
{
public:
	CCobolCounter();

protected:

	virtual int CountDirectiveSLOC( filemap* fmap, results* result, filemap* fmapBak = NULL );
	virtual int LanguageSpecificProcess( filemap* fmap, results* result, filemap* fmapBak = NULL );
//	virtual int ProcessComplexityMeasures( filemap* fmap, results* result );
	virtual int CountComplexity( filemap* fmap, results* result );
	virtual int TrimSourceLine( string& line );
	virtual int TrimSequenceNumber( string& line );
	virtual int TrimAreaA( string& line );
	virtual int TrimIndicator( string& line );

	StringVector exclude_start_keywords;            //!< SLOC lines excluded from counts starting with keywords

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CCobolCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CCobolCounter(const CCobolCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CCobolCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CCobolCounter operator=(const CCobolCounter);    // Declare without implementation
};

#endif
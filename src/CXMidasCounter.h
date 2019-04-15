//! Code counter class definition for the X-Midas macro language.
/*!
* \file CXMidasCounter.h
*
* This file contains the code counter class definition for the X-Midas macro language.
*/

#ifndef CXMidasCounter_h
#define CXMidasCounter_h

#include "CMidasCounter.h"

//! X-Midas code counter class.
/*!
* \class CXMidasCounter
*
* Defines the X-Midas code counter class.
*/
class CXMidasCounter : public CMidasCounter
{
public:
	CXMidasCounter();

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CXMidasCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CXMidasCounter(const CXMidasCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CXMidasCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CXMidasCounter operator=(const CXMidasCounter);    // Declare without implementation
};

#endif

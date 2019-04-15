//! Code counter class definition for the NeXtMidas macro language.
/*!
* \file CNeXtMidasCounter.h
*
* This file contains the code counter class definition for the NeXtMidas macro language.
*/

#ifndef CNeXtMidasCounter_h
#define CNeXtMidasCounter_h

#include "CMidasCounter.h"

//! NeXtMidas code counter class.
/*!
* \class CNeXtMidasCounter
*
* Defines the NeXtMidas code counter class.
*/
class CNeXtMidasCounter : public CMidasCounter
{
public:
	CNeXtMidasCounter();

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CNeXtMidasCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CNeXtMidasCounter(const CNeXtMidasCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CNeXtMidasCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CNeXtMidasCounter operator=(const CNeXtMidasCounter);    // Declare without implementation
};

#endif

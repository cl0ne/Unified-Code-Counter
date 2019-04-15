//! Code counter class definition for data files.
/*!
* \file CDataCounter.h
*
* This file contains the code counter class definition for data files.
*/

#ifndef CDataCounter_h
#define CDataCounter_h

#include "CCodeCounter.h"

//! Data file code counter class.
/*!
* \class CDataCounter
*
* Defines the data file code counter class.
*/
class CDataCounter : public CCodeCounter
{
public:
	CDataCounter();

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CDataCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CDataCounter(const CDataCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CDataCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CDataCounter operator=(const CDataCounter);    // Declare without implementation
};

#endif

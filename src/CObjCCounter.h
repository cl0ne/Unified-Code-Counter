//! Code counter class definition for the C/C++ languages.
/*!
* \file CCCounter.h
*
* This file contains the code counter class definition for the C/C++ languages.
*/

#ifndef CObjCCounter_h
#define CObjCCounter_h

#include "CCJavaCsScalaCounter.h"

//! Objective C code counter class.
/*!
* \class CObjCCounter
*
* Defines the Objective C code counter class.
*/

class CObjCCounter : public CCJavaCsScalaCounter
{
public:
	CObjCCounter( string lang = "OBJC" );

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CObjCCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CObjCCounter(const CObjCCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CObjCCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CObjCCounter operator=(const CObjCCounter);    // Declare without implementation
};

#endif
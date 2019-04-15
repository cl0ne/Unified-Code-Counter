//! Code counter class definition for the XML language.
/*!
* \file CXmlCounter.h
*
* This file contains the code counter class definition for the XML language.
*/

#ifndef CXmlCounter_h
#define CXmlCounter_h

#include "CTagCounter.h"

//! XML code counter class.
/*!
* \class CXmlCounter
*
* Defines the XML code counter class.
*/
class CXmlCounter : public CTagCounter
{
public:
	CXmlCounter();

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CXmlCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CXmlCounter(const CXmlCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CXmlCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CXmlCounter operator=(const CXmlCounter);    // Declare without implementation
};

#endif

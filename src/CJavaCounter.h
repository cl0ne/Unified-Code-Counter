//! Code counter class definition for the Java language.
/*!
* \file CJavaCounter.h
*
* This file contains the code counter class definition for the Java language.
*/

#ifndef CJavaCounter_h
#define CJavaCounter_h

#include "CCJavaCsScalaCounter.h"

//! Java code counter class.
/*!
* \class CJavaCounter
*
* Defines the Java code counter class.
*/
class CJavaCounter : public CCJavaCsScalaCounter
{
public:
	// Set the language so base class constructors set values as needed
	CJavaCounter( string lang = "JAVA" );
        int GetLineUntilEndOfMultistringIfAny(int curr_line_idx, string &line, filemap &fmap, map<string, unsigned int> &nonfunction_operator_counts);

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CJavaCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CJavaCounter(const CJavaCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CJavaCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CJavaCounter operator=(const CJavaCounter);    // Declare without implementation
};

//! Java in JSP code counter class.
/*!
* \class CJavaJspCounter
*
* Defines the Java in JSP code counter class.
*/
class CJavaJspCounter : public CJavaCounter
{
public:
	CJavaJspCounter();

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CJavaJspCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CJavaJspCounter(const CJavaJspCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CJavaJspCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CJavaJspCounter operator=(const CJavaJspCounter);    // Declare without implementation
};

#endif

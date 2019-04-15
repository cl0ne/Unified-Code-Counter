//! Code counter class definition for the C/C++ languages.
/*!
* \file CCCounter.h
*
* This file contains the code counter class definition for the C/C++ languages.
*/

#ifndef CCCounter_h
#define CCCounter_h

#include "CCJavaCsScalaCounter.h"

//! C/C++ code counter class.
/*!
* \class CCCounter
*
* Defines the C/C++ code counter class.
*/
class CCCounter : public CCJavaCsScalaCounter
{
public:
	// Set the language so base class constructors set values as needed
	CCCounter( string lang = "C_CPP" );

        //Modification: 2018.01 Integration starts
        bool EndsWithOpenString(const string &line);
        int GetLineUntilEndOfMultistringIfAny(int curr_line_idx, string &line, filemap &fmap, map<string, unsigned int> &nonfunction_operator_counts);

        //Modification: 2018.01 Integration ends
private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CCCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CCCounter(const CCCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CCCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CCCounter operator=(const CCCounter);    // Declare without implementation
};

#endif

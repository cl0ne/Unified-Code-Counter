//! Code counter class definition for the HTML language.
/*!
* \file CHtmlCounter.h
*
* This file contains the code counter class definition for the HTML language.
*/

#ifndef CHtmlCounter_h
#define CHtmlCounter_h

#include "CTagCounter.h"

//! HTML code counter class.
/*!
* \class CHtmlCounter
*
* Defines the HTML code counter class.
*/
class CHtmlCounter : public CTagCounter
{
public:
	CHtmlCounter();

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CHtmlCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CHtmlCounter(const CHtmlCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CHtmlCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CHtmlCounter operator=(const CHtmlCounter);    // Declare without implementation
};

//! HTML in PHP code counter class.
/*!
* \class CHtmlPhpCounter
*
* Defines the HTML in PHP code counter class.
*/
class CHtmlPhpCounter : public CHtmlCounter
{
public:
	CHtmlPhpCounter();

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CHtmlPhpCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CHtmlPhpCounter(const CHtmlPhpCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CHtmlPhpCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CHtmlPhpCounter operator=(const CHtmlPhpCounter);    // Declare without implementation
};

//! HTML in JSP code counter class.
/*!
* \class CHtmlJspCounter
*
* Defines the HTML in JSP code counter class.
*/
class CHtmlJspCounter : public CHtmlCounter
{
public:
	CHtmlJspCounter();

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CHtmlJspCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CHtmlJspCounter(const CHtmlJspCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CHtmlJspCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CHtmlJspCounter operator=(const CHtmlJspCounter);    // Declare without implementation
};

//! HTML in ASP code counter class.
/*!
* \class CHtmlAspCounter
*
* Defines the HTML in ASP code counter class.
*/
class CHtmlAspCounter : public CHtmlCounter
{
public:
	CHtmlAspCounter();

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CHtmlAspCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CHtmlAspCounter(const CHtmlAspCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CHtmlAspCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CHtmlAspCounter operator=(const CHtmlAspCounter);    // Declare without implementation
};

//! HTML in ColdFusion code counter class.
/*!
* \class CHtmlColdFusionCounter
*
* Defines the HTML in ColdFusion code counter class.
*/
class CHtmlColdFusionCounter : public CHtmlCounter
{
public:
	CHtmlColdFusionCounter();

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CHtmlColdFusionCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CHtmlColdFusionCounter(const CHtmlColdFusionCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CHtmlColdFusionCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CHtmlColdFusionCounter operator=(const CHtmlColdFusionCounter);    // Declare without implementation
};

#endif

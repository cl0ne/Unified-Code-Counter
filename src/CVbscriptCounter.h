//! Code counter class definition for the VBScript language.
/*!
* \file CVbscriptCounter.h
*
* This file contains the code counter class definition for the VBScript language.
*/

#ifndef CVbscriptCounter_h
#define CVbscriptCounter_h

#include "CVbCounter.h"

//! Visual Basic code counter class.
/*!
* \class CVbscriptCounter
*
* Defines the Visual Basic code counter class.
*/
class CVbscriptCounter : public CVbCounter
{
public:
	CVbscriptCounter();
	int PreCountProcess(filemap* fmap);

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CVbscriptCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CVbscriptCounter(const CVbscriptCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CVbscriptCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CVbscriptCounter operator=(const CVbscriptCounter);    // Declare without implementation
};

//! VBScript in PHP code counter class.
/*!
* \class CVbsPhpCounter
*
* Defines the VBScript in PHP code counter class.
*/
class CVbsPhpCounter : public CVbscriptCounter
{
public:
	CVbsPhpCounter();

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CVbsPhpCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CVbsPhpCounter(const CVbsPhpCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CVbsPhpCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CVbsPhpCounter operator=(const CVbsPhpCounter);    // Declare without implementation
};

//! VBScript in HTML code counter class.
/*!
* \class CVbsHtmlCounter
*
* Defines the VBScript in HTML code counter class.
*/
class CVbsHtmlCounter : public CVbscriptCounter
{
public:
	CVbsHtmlCounter();

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CVbsHtmlCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CVbsHtmlCounter(const CVbsHtmlCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CVbsHtmlCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CVbsHtmlCounter operator=(const CVbsHtmlCounter);    // Declare without implementation
};

//! VBScript in XML code counter class.
/*!
* \class CVbsXmlCounter
*
* Defines the VBScript in XML code counter class.
*/
class CVbsXmlCounter : public CVbscriptCounter
{
public:
	CVbsXmlCounter();

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CVbsXmlCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CVbsXmlCounter(const CVbsXmlCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CVbsXmlCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CVbsXmlCounter operator=(const CVbsXmlCounter);    // Declare without implementation
};

//! VBScript in JSP code counter class.
/*!
* \class CVbsJspCounter
*
* Defines the VBScript in JSP code counter class.
*/
class CVbsJspCounter : public CVbscriptCounter
{
public:
	CVbsJspCounter();

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CVbsJspCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CVbsJspCounter(const CVbsJspCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CVbsJspCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CVbsJspCounter operator=(const CVbsJspCounter);    // Declare without implementation
};

//! VBScript in ASP server code counter class.
/*!
* \class CVbsAspServerCounter
*
* Defines the VBScript in ASP server code counter class.
*/
class CVbsAspServerCounter : public CVbscriptCounter
{
public:
	CVbsAspServerCounter();

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CVbsAspServerCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CVbsAspServerCounter(const CVbsAspServerCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CVbsAspServerCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CVbsAspServerCounter operator=(const CVbsAspServerCounter);    // Declare without implementation
};

//! VBScript in ASP client code counter class.
/*!
* \class CVbsAspClientCounter
*
* Defines the VBScript in ASP client code counter class.
*/
class CVbsAspClientCounter : public CVbscriptCounter
{
public:
	CVbsAspClientCounter();

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CVbsAspClientCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CVbsAspClientCounter(const CVbsAspClientCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CVbsAspClientCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CVbsAspClientCounter operator=(const CVbsAspClientCounter);    // Declare without implementation
};

//! VBScript in ColdFusion code counter class.
/*!
* \class CVbsColdFusionCounter
*
* Defines the VBScript in ColdFusion code counter class.
*/
class CVbsColdFusionCounter : public CVbscriptCounter
{
public:
	CVbsColdFusionCounter();

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CVbsColdFusionCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CVbsColdFusionCounter(const CVbsColdFusionCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CVbsColdFusionCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CVbsColdFusionCounter operator=(const CVbsColdFusionCounter);    // Declare without implementation
};

#endif

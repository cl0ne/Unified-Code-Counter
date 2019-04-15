//! Code counter class definition for the C# language.
/*!
* \file CCsharpCounter.h
*
* This file contains the code counter class definition for the C# language.
*/

#ifndef CCsharpCounter_h
#define CCsharpCounter_h

#include "CCJavaCsScalaCounter.h"

//! C# code counter class.
/*!
* \class CCsharpCounter
*
* Defines the C# code counter class.
*/
class CCsharpCounter : public CCJavaCsScalaCounter
{
public:
	// Set the language so base class constructors set values as needed
	CCsharpCounter( string lang = "C_SHARP" );

protected:
	virtual int PreCountProcess(filemap* fmap);
	virtual int ReplaceQuote(string &strline, size_t &idx_start, bool &contd, char &CurrentQuoteEnd);
        bool HasOneDoubleQuote(string line);
        int GetLineUntilEndOfMultistringIfAny(int curr_line_idx, string &line, filemap &fmap, map<string, unsigned int> &nonfunction_operator_counts);

private:
	bool isVerbatim;

// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CCsharpCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CCsharpCounter(const CCsharpCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CCsharpCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CCsharpCounter operator=(const CCsharpCounter);    // Declare without implementation
};

//! C# in HTML code counter class.
/*!
* \class CCsharpHtmlCounter
*
* Defines the C# in HTML code counter class.
*/
class CCsharpHtmlCounter : public CCsharpCounter
{
public:
	CCsharpHtmlCounter();

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CCsharpHtmlCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CCsharpHtmlCounter(const CCsharpHtmlCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CCsharpHtmlCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CCsharpHtmlCounter operator=(const CCsharpHtmlCounter);    // Declare without implementation
};

//! C# in XML code counter class.
/*!
* \class CCsharpXmlCounter
*
* Defines the C# in XML code counter class.
*/
class CCsharpXmlCounter : public CCsharpCounter
{
public:
	CCsharpXmlCounter();

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CCsharpXmlCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CCsharpXmlCounter(const CCsharpXmlCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CCsharpXmlCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CCsharpXmlCounter operator=(const CCsharpXmlCounter);    // Declare without implementation
};

//! C# in ASP code counter class.
/*!
* \class CCsharpAspCounter
*
* Defines the C# in ASP code counter class.
*/
class CCsharpAspCounter : public CCsharpCounter
{
public:
	CCsharpAspCounter();

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CCsharpAspCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CCsharpAspCounter(const CCsharpAspCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CCsharpAspCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CCsharpAspCounter operator=(const CCsharpAspCounter);    // Declare without implementation
};

#endif

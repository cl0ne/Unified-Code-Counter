//! Code counter class definition for the SQL language.
/*!
* \file CSqlCounter.h
*
* This file contains the code counter class definition for the SQL language.
*/

#ifndef CSqlCounter_h
#define CSqlCounter_h

#include "CCodeCounter.h"

//! SQL code counter class.
/*!
* \class CSqlCounter
*
* Defines the SQL code counter class.
*/
class CSqlCounter : public CCodeCounter
{
public:
	CSqlCounter();

protected:
	virtual int ReplaceQuote(string &strline, size_t &idx_start, bool &contd, char &CurrentQuoteEnd);
	virtual int LanguageSpecificProcess(filemap* fmap, results* result, filemap* fmapBak = NULL);
	void LSLOC(results* result, string line, size_t lineNumber, string lineBak, string &strLSLOC, string &strLSLOCBak, 
		bool &data_continue);

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CSqlCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CSqlCounter(const CSqlCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CSqlCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CSqlCounter operator=(const CSqlCounter);    // Declare without implementation
};

//! SQL in ColdFusion code counter class.
/*!
* \class CSqlColdFusionCounter
*
* Defines the SQL in ColdFusion code counter class.
*/
class CSqlColdFusionCounter : public CSqlCounter
{
public:
	CSqlColdFusionCounter();

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CSqlColdFusionCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CSqlColdFusionCounter(const CSqlColdFusionCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CSqlColdFusionCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CSqlColdFusionCounter operator=(const CSqlColdFusionCounter);    // Declare without implementation
};

#endif

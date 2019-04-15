//! Code counter class definition for tag languages including HTML, XML, and ColdFusion.
/*!
* \file CHtmlCounter.h
*
* This file contains the code counter class definition for for tag languages including HTML, XML, and ColdFusion.
*/

#ifndef CTagCounter_h
#define CTagCounter_h

#include "CCodeCounter.h"

//! Tag language code counter class.
/*!
* \class CTagCounter
*
* Defines the tag language code counter class.
*/
class CTagCounter : public CCodeCounter
{
public:
	CTagCounter();

protected:
	virtual int PreCountProcess(filemap* /*fmap*/) { return 0; }
	virtual int LanguageSpecificProcess(filemap* fmap, results* result, filemap* fmapBak = NULL);
	void LSLOC(results* result, string line, size_t lineNumber, string lineBak, string &strLSLOC, string &strLSLOCBak, 
		char &prev_char, bool &data_continue, unsigned int &temp_lines, unsigned int &phys_exec_lines, 
		unsigned int &phys_data_lines);
	void CountTagTally(string base, StringVector& container, unsigned int &count, int mode, string exclude,
		string include1, string include2, UIntVector* counter_container = 0, bool case_sensitive = true);

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CTagCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CTagCounter(const CTagCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CTagCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CTagCounter operator=(const CTagCounter);    // Declare without implementation
};

#endif

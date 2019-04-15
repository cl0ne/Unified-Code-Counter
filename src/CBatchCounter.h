//! Code counter class definition for the Batch shell script language.
/*!
* \file CBatchCounter.h
*
* This file contains the code counter class definition for the Batch shell script language.
* This also includes the Korn shell language.
*/

#ifndef CBatchCounter_h
#define CBatchCounter_h

#include "CCodeCounter.h"

//! Batch shell script code counter class.
/*!
* \class CBatchCounter
*
* Defines the Batch shell script code counter class.
*/
class CBatchCounter : public CCodeCounter
{
public:
	CBatchCounter();

protected:
	virtual int PreCountProcess(filemap* fmap);
	virtual int LanguageSpecificProcess(filemap* fmap, results* result, filemap* fmapBak = NULL);
	void LSLOC(results* result, string line, size_t lineNumber, string lineBak, string &strLSLOC, 
		string &strLSLOCBak, bool &data_continue, unsigned int &temp_lines, unsigned int &phys_exec_lines,
		unsigned int &phys_data_lines/*, StringVector &loopLevel*/); // warning fix
	int ParseFunctionName(const string &line, string &lastline, filemap &functionStack, string &functionName, 
		unsigned int &functionCount);
	int CountComplexity(filemap* fmap, results* result);

	StringVector continue_keywords;		//!< List of keywords to continue to next line

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CBatchCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CBatchCounter(const CBatchCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CBatchCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CBatchCounter operator=(const CBatchCounter);    // Declare without implementation
};

#endif

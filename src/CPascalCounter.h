//! Code counter class definition for the Pascal language.
/*!
* \file CPascalCounter.h
*
* This file contains the code counter class definition for the Pascal language.
*/

#ifndef CPascalCounter_h
#define CPascalCounter_h

#include "CCodeCounter.h"

//! Pascal code counter class.
/*!
* \class CPascalCounter
*
* Defines the Pascal counter class.
*/
class CPascalCounter : public CCodeCounter
{
public:
	CPascalCounter();

protected:
	virtual int CountCommentsSLOC(filemap* fmap, results* result, filemap* fmapBak = NULL);
	virtual int LanguageSpecificProcess(filemap* fmap, results* result, filemap* fmapmBak = NULL);
	void LSLOC(results* result, string line, size_t lineNumber, string lineBak, string &strLSLOC, string &strLSLOCBak, 
		bool &found_block, bool &found_forifwhile, bool &found_end, bool &found_loop, StringVector &loopLevel);
	void FoundSLOC(results* result, size_t lineNumber, string &strLSLOC, string &strLSLOCBak, bool &found_block,
		bool &found_forifwhile, bool &found_end, bool &trunc_flag);
	int ParseFunctionName(const string &line, string &lastline, filemap &functionStack, string &functionName, 
		unsigned int &functionCount);
	int CountComplexity(filemap* fmap, results* result);

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CPascalCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CPascalCounter(const CPascalCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CPascalCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CPascalCounter operator=(const CPascalCounter);    // Declare without implementation
};

#endif

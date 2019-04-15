//! Code counter class definition for the Bash shell script language.
/*!
* \file CBashCounter.h
*
* This file contains the code counter class definition for the Bash shell script language.
* This also includes the Korn shell language.
*/

#ifndef CBashCounter_h
#define CBashCounter_h

#include "CCodeCounter.h"

//! Bash shell script code counter class.
/*!
* \class CBashCounter
*
* Defines the Bash shell script code counter class.
*/
class CBashCounter : public CCodeCounter
{
public:
	CBashCounter();

protected:
	virtual int PreCountProcess(filemap* fmap);
	virtual int LanguageSpecificProcess(filemap* fmap, results* result, filemap* fmapBak = NULL);
    //Added by Rujun Ma for BashCounter fix. 11.27.16
	//changes maade by Nikhila Banukumar
	using CCodeCounter::ReplaceQuote;
    virtual int ReplaceQuote(string &strline, size_t &idx_start, bool &contd, string &CurrentQuoteEnd);	
	virtual int CountCommentsSLOC(filemap* fmap, results* result, filemap* fmapBak = NULL);		
	virtual int CountDirectiveSLOC(filemap* fmap, results* result, filemap* fmapBak = NULL);
    
	void LSLOC(results* result, string line, size_t lineNumber, string lineBak, string &strLSLOC, 
		string &strLSLOCBak, bool &data_continue, unsigned int &temp_lines, unsigned int &phys_exec_lines,
		unsigned int &phys_data_lines, StringVector &loopLevel);
	int ParseFunctionName(const string &line, string &lastline, filemap &functionStack, 
		string &functionName, unsigned int &functionCount);
	StringVector continue_keywords;		//!< List of keywords to continue to next line

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CBashCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CBashCounter(const CBashCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CBashCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CBashCounter operator=(const CBashCounter);    // Declare without implementation
};

#endif

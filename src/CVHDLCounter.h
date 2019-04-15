//! Code counter class definition for the VHDL language.
/*!
* \file CVHDLCounter.h
*
* This file contains the code counter class definition for the VHDL hardware definition language (used in FPGA programming).
*/

#ifndef CVHDLCounter_h
#define CVHDLCounter_h

#include "CCodeCounter.h"

class tokenLocation
{
public:
	string			token;
	unsigned int	lineNumber;
	int				position;
};

typedef vector<tokenLocation> tokLocVect;

//! VHDL code counter class.
/*!
* \class CVHDLCounter
*
* Defines the VHDL code counter class.
*/
class CVHDLCounter : public CCodeCounter
{
public:
	CVHDLCounter();

protected:
	virtual int ReplaceQuote(string &strline, size_t &idx_start, bool &contd, char &CurrentQuoteEnd);
	virtual int CountCommentsSLOC(filemap* fmap, results* result, filemap *fmapBak);
	virtual int CountComplexity(filemap* fmap, results* result);
	virtual int CountDirectiveSLOC(filemap* fmap, results* result, filemap* fmapBak = NULL);
	virtual int LanguageSpecificProcess(filemap* fmap, results* result, filemap* fmapBak);
	void FoundSLOC(results* result, size_t lineNumber, string &strLSLOC, string &strLSLOCBak, bool &found_block, bool &found_forifwhile,
		bool &found_end, bool &found_type, bool &found_is, bool &found_unit, bool &trunc_flag, StringVector currentBlock);
	void LSLOC(results* result, string line, size_t lineNumber, string lineBak, string &strLSLOC, string &strLSLOCBak, 
		unsigned int &paren_cnt, bool &forflag, bool &found_forifwhile, bool &found_while, bool &found_type, bool &found_is, 
		bool &found_unit, unsigned int &loopLevel, vectorString &currentBlock, bool &processSignatureFound, bool &found_withSelect, 
		bool &found_whenConditional, bool &foundWait, bool &blockSignatureFound, bool &found_record);
	using CCodeCounter::ParseFunctionName; // warning fix
	int ParseFunctionName(const string &line, string &lastline, StringVector &functionStack, string &functionName);

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CVHDLCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CVHDLCounter(const CVHDLCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CVHDLCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CVHDLCounter operator=(const CVHDLCounter);    // Declare without implementation
};

#endif

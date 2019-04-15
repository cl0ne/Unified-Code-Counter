//! Code counter class definition for the Verilog language.
/*!
* \file CVerilogCounter.h
*
* This file contains the code counter class definition for the Verilog hardware definition language (used in FPGA programming).
*/

#ifndef CVerilogCounter_h
#define CVerilogCounter_h

#include "CCodeCounter.h"
#include "CVHDLCounter.h"

//! Verilog code counter class.
/*!
* \class CVerilogCounter
*
* Defines the Verilog code counter class.
*/

class CVerilogCounter : public CCodeCounter
{
public:
	CVerilogCounter();

protected:
	virtual int CountComplexity(filemap* fmap, results* result);
	virtual int CountDirectiveSLOC(filemap* fmap, results* result, filemap* fmapmBak = NULL);
	virtual int LanguageSpecificProcess(filemap* fmap, results* result, filemap* fmapmBak = NULL);
	void LSLOC(results* result, string line, size_t lineNumber, string lineBak, string &strLSLOC, string &strLSLOCBak, 
		unsigned int &paren_cnt, bool &forflag, bool &found_forifwhile, bool &found_while, char &prev_char, 
		bool &data_continue, unsigned int &temp_lines, unsigned int &phys_exec_lines, unsigned int &phys_data_lines,
		bool &found_for, StringVector &loopLevel, bool &always_flag, bool &case_flag, bool &repeat_flag);
    	using CCodeCounter::ParseFunctionName; // warning fix
    	int ParseFunctionName(const string &line, string &lastline, StringVector &functionStack, string &functionName);

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CVerilogCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CVerilogCounter(const CVerilogCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CVerilogCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CVerilogCounter operator=(const CVerilogCounter);    // Declare without implementation
};

#endif

//! Code counter class definition for the C/C++, Java, and C# languages.
/*!
* \file CCJavaCsCounter.h
*
* This file contains the code counter class definition for the C/C++, Java, and C# languages.
*/

#ifndef CCJavaCsScalaCounter_h
#define CCJavaCsScalaCounter_h

#include "CCodeCounter.h"

//! C/C++, Java, C# and Scala code counter common base class.
/*!
* \class CCJavaCsCounter
*
* Defines the C/C++, Java, C# and Scala code counter class.
*/
class CCJavaCsScalaCounter : public CCodeCounter
{
public:
	CCJavaCsScalaCounter( string lang = "LANGUAGE_NOT_SET" );

protected:
	virtual int CountDirectiveSLOC(filemap* fmap, results* result, filemap* fmapmBak = NULL);
	virtual int LanguageSpecificProcess(filemap* fmap, results* result, filemap* fmapmBak = NULL);
	void LSLOC(results* result, string line, size_t lineNumber, string lineBak, string &strLSLOC, string &strLSLOCBak,
		unsigned int &paren_cnt, bool &forflag, bool &found_forifwhile, bool &found_while, char &prev_char, bool &data_continue,
		unsigned int &temp_lines, unsigned int &phys_exec_lines, unsigned int &phys_data_lines,
		bool &inArrayDec, bool &found_for, unsigned int &openBrackets, StringVector &loopLevel);
	virtual int ParseFunctionName(const string &line, string &lastline,
		filemap &functionStack, string &functionName, unsigned int &functionCount);
        
        //Modification: 2018.01 Integration starts
        int FindIdxOfNextQuote(const string &line, int start_pos, const char &quote);
    int RemoveStringsAndChars(string &line, map<string, unsigned int> &string_char_counts, const bool &add_to_count);
        bool IsEitherFunctionDefinitionOrInvocation(string line, string &function_name);
        bool IsFunctionDefinition(int line_idx, filemap &fmap, string &function_name);
        void SumUp(map<string, map<string, unsigned int> > &add_from_counts, map<string, unsigned int> &into_counts);
        bool IsLastLineOfFunction(map<string, unsigned int> &nonfunction_operator_counts);
        int CountOperatorsAndOperands(string &line,
                                  map<string, unsigned int> &string_char_counts,
                                  map<string, unsigned int> &word_counts,
                                  map<string, unsigned int> &bool_counts,
                                  map<string, unsigned int> &number_counts,
                                  map<string, unsigned int> &function_counts,
                                  map<string, unsigned int> &nonfunction_operator_counts);
        void RemoveSymbolsExceptOpenRoundBracketOfFunction(string &line, map<string, unsigned int> &nonfunction_operator_counts, const bool &should_count);
        bool IsAlphanumericOrUnderscore(const char &c);
        virtual int GetLineUntilEndOfMultistringIfAny(int, string &, filemap &, map<string, unsigned int> &) { return -1; } //Modification: 2018.05. Integration
        virtual int FindHalsteadsVolume(filemap fmapModBak, results* result);
        //Modification: 2018.01 Integration ends

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CCJavaCsScalaCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CCJavaCsScalaCounter(const CCJavaCsScalaCounter& rhs);

	// Take care of warning C4626: 'CCJavaCsScalaCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CCJavaCsScalaCounter operator=(const CCJavaCsScalaCounter);
};

#endif

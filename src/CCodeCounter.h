//! Common code counter class for sub-classing individual languages.
/*!
* \file CCodeCounter.h
*
* This file contains the common code counter class for sub-classing individual languages.
*/

#ifndef CCodeCounter_h
#define CCodeCounter_h

#include "cc_main.h"
#include "CUtil.h"

// Enable this new approach.  Disabled for 2015.12 Release
//#define		ENABLE_1_PASS_PARSER

//! Common code counter class.
/*!
* \class CCodeCounter
*
* Defines the common code counter class.
*/
class CCodeCounter
{
public:
	CCodeCounter();
	virtual ~CCodeCounter();
	virtual void InitializeCounts();
	virtual int CountSLOC(filemap* fmap, results* result);
	bool IsSupportedFileExtension(const string &file_name);
	virtual ofstream* GetOutputStream(const string &outputFileNamePrePend = "",
		const string &cmd = "", bool csvOutput = false, bool legacyOutput = false);
	virtual void CloseOutputStream();

//!< public Data declarations
	StringVector  directive;				//!< Directive statement keywords
	StringVector  data_name_list;			//!< Data statement keywords
	StringVector  exec_name_list;			//!< Executable statement keywords
	StringVector  file_extension;			//!< File extension

	StringVector  math_func_list;			//!< Math functions
	StringVector  trig_func_list;			//!< Trigonometric functions
	StringVector  log_func_list;			//!< Logarithmic functions
	StringVector  cmplx_calc_list;			//!< Calculations (complexity)
	StringVector  cmplx_cond_list;			//!< Conditionals (complexity)
	StringVector  cmplx_logic_list;			//!< Logicals (complexity)
	
        //Modification: 2018.01 Integration starts
        StringVector three_char_operator_list;  //!< List of three character operators (for Halsteads volume calculations)
        StringVector two_char_operator_list;    //!< List of two character operators (for Halsteads volume calculations)
        StringVector one_char_operator_list;    //!< List of one character operators (for Halsteads volume calculations)
        set<string>  keyword_operators;         //!< List of reserved keyword operators (for Halsteads volume calculations)
        //Modification: 2018.01 Integration ends
  
	// Auxillary meanings keywords/operators.  PASCAL ex: IF/ELSE IF, REPEAT-UNTIL, WHILE, FOR, CASE OF, CASE, Database exception clause
	StringVector  aux_meaning_keys;			//!< Cyclomatic Complexity rings keywords/Operators, OO keywords, FP keywords
	UIntVector    aux_meaning_vals;			//!< Cyclomatic Complexity rings values, OO values, FP values
	
	StringVector  cmplx_preproc_list;		//!< Preprocessor directives (complexity)
	StringVector  cmplx_assign_list;		//!< Assignments (complexity)
	StringVector  cmplx_pointer_list;		//!< Pointers (complexity)

	StringVector  cmplx_cyclomatic_list;	//!< Cyclomatic complexity decision keywords (complexity) (C/C++: if, case, while, for, catch, ?)
	UIntVector    cmplx_cyclomatic_listVals;	//!< Cyclomatic complexity decision keywords (complexity) CC ring values, OO vals, fp vals

	StringVector  cmplx_cyclomatic_logic_list;	//!< Cyclomatic complexity logical keywords (complexity) (C/C++:
	StringVector  cmplx_cyclomatic_case_list;	//!< Cyclomatic complexity case keywords (complexity)
	StringVector  cmplx_cyclomatic_default_list;	//!< Cyclomatic complexity case default keywords (complexity)
	StringVector  cmplx_cyclomatic_switch_list;	//!< Cyclomatic complexity case switch keywords (complexity)
	StringVector  ignore_cmplx_cyclomatic_list;	//!< Cyclomatic complexity decision keywords to ignore (for example End If)
	StringVector  skip_cmplx_cyclomatic_file_extension_list;	//!< Cyclomatic complexity file extensions to skip

	UIntPairVector directive_count;			//!< Count of each directive statement keyword
	UIntPairVector data_name_count;			//!< Count of each data statement keyword
	UIntPairVector exec_name_count;			//!< Count of each executable statement keyword

	UIntPairVector math_func_count;			//!< Count of math functions
	UIntPairVector trig_func_count;			//!< Count of trigonometric functions
	UIntPairVector log_func_count;			//!< Count of logarithmic functions
	UIntPairVector cmplx_calc_count;		//!< Count of calculations
	UIntPairVector cmplx_cond_count;		//!< Count of conditionals
	UIntPairVector cmplx_logic_count;		//!< Count of logicals
	UIntPairVector cmplx_preproc_count;		//!< Count of preprocessor directives
	UIntPairVector cmplx_assign_count;		//!< Count of assignments
	UIntPairVector cmplx_pointer_count;		//!< Count of pointers

#ifdef	ENABLE_1_PASS_PARSER
	bool          use_1pass_parser;			//!< use single pass file parse approach		Modification: 2015.12
#endif

	bool          print_cmplx;				//!< Print complexity and keyword counts
	bool          cc4enable;				//!< Enable Cyclomatic Complexity 4 unique conditional clauses scanning  Modification: 2015.12
	size_t        lsloc_truncate;			//!< # of characters allowed in LSLOC for differencing (0=no truncation)
	string        language_name;			//!< Counter language name
	string        language_version;			//!< Counter language version (do not include the word version)		Modification: 2015.12
											//!< For C++ could be "2014", for Scala could be "2.11.5"

	string        language_version_authority;	//!< language version authority (either the Standards body or version creator)	Modification: 2015.12
											//!< For C++ could be "ISO C++ Standards Committee", for Scala could be "scala.org"

	ClassType	  classtype;				//!< Language class type
	unsigned int  counted_files;			//!< Number of files counted
	unsigned int  counted_dupFiles;			//!< Number of duplicate files counted
	unsigned int  total_filesA;				//!< Total number of files in baseline A
	unsigned int  total_filesB;				//!< Total number of duplicate files in baseline B
	unsigned int  total_dupFilesA;			//!< Total number of files in baseline A
	unsigned int  total_dupFilesB;			//!< Total number of duplicate files in baseline B

// Support for lightweight Stack Dump        Modified: 2015.12
//
	// These are set directly from caller before parsing starts    Modified: 2015.12
	string			parse_file_name;
	unsigned int	parse_threadIdx;

	// These are set by the Parse Exception Handler    Modified: 2015.12
	string			parse_physical_line;
	string			parse_physical_line_comments;
	string			parse_logical_line;

	// All these are set during Parsing
	unsigned long  currentPhyLine;			//!< Which Physical line in source file is being parsed, starting with 1
	unsigned long  currentLSrcLine;			//!< Which Logical Source line in source file is being parsed, starting with 1
	// Level ABOVE current Step 1 is always CountSLOC
	string		  currentStep1;				//!< What procedure is called from CountSLOC
	string		  currentStep2;				//!< What procedure or other unique code place is called from level BELOW CountSLOC
	string		  currentStep3;				//!< What step is active from level BELOW the level BELOW CountSLOC
	string		  currentStep4;				//!< What step is active from level BELOW the level BELOW/BELOW CountSLOC (if used)
	string		  currentFunc1;				//!< What procedure name at level 1 (changes slowest)
	string		  currentFunc2;				//!< What procedure name at level 2 (changes faster)
	string		  currentFunc3;				//!< What procedure name at level 3 (changes fastest)
	string		  currentFunc4;				//!< What procedure name at level 3 (changes fastest, if used)
	// Line numbers are only APPROXIMATE.  Captured from most recent use of a MACRO as below.
	long		  currentCode1;				//!< What line of UCC code is running as called from CountSLOC
	long		  currentCode2;				//!< What line of UCC code is running as called from BELOW CountSLOC
	long		  currentCode3;				//!< What line of UCC code is running as called from BELOW/BELOW CountSLOC
	long		  currentCode4;				//!< What line of UCC code is running as called from BELOW/BELOW/BELOW CountSLOC (if used)

	// Define some helper MACRO to make saving code context easier.  Support for later Stack Dump    Modified: 2015.12
	#define ENTER_1(msg)   { currentStep1 = msg; currentCode1 = __LINE__; currentFunc1 = UCC_FUNC_NAME; }
	#define ENTER_2(msg)   { currentStep2 = msg; currentCode2 = __LINE__; currentFunc2 = UCC_FUNC_NAME; }
	#define ENTER_3(msg)   { currentStep3 = msg; currentCode3 = __LINE__; currentFunc3 = UCC_FUNC_NAME; }
	#define ENTER_4(msg)   { currentStep4 = msg; currentCode4 = __LINE__; currentFunc4 = UCC_FUNC_NAME; }
	#define SAVE_TO_1(msg) { currentStep1 = msg; currentCode1 = __LINE__; }
	#define SAVE_TO_2(msg) { currentStep2 = msg; currentCode2 = __LINE__; }
	#define SAVE_TO_3(msg) { currentStep3 = msg; currentCode3 = __LINE__; }
	#define SAVE_TO_4(msg) { currentStep4 = msg; currentCode4 = __LINE__; }
    /*Made these public to use them in MainObject.cpp //Ext Team. Modification: 10/16*/
	StringVector BlockCommentStart;			//!< Block comment start character(s) (ex. /* in C++)
	StringVector BlockCommentEnd;			//!< Block comment end character(s) (ex. */ in C++)
	StringVector LineCommentStart;			//!< Single line or embedded comment character(s)

protected:
	virtual void InitializeResultsCounts(results* result);
	static size_t FindQuote(string const &strline, string const &QuoteStart, size_t idx_start, char QuoteEscapeFront);
	virtual int ReplaceQuote(string &strline, size_t &idx_start, bool &contd, char &CurrentQuoteEnd);
	virtual int PreCountProcess(filemap* /*fmap*/) { return 0; }
	int CountBlankSLOC(filemap* fmap, results* result);
	virtual int CountCommentsSLOC(filemap* fmap, results* result, filemap* fmapBak = NULL);
        virtual int FindHalsteadsVolume(filemap, results* ) { return -1; } //Modification: 2018.05. Integration

#ifdef	ENABLE_1_PASS_PARSER
//!< These support fewer passes file parsing
/*!
	* \enum ParseType
	*
	* Enumeration of types of well known values that parsing of a language could find.
	* Or types of parsing found for various reasons.
	*/
	enum ParseType {
		PARSE_UNKNOWN = -1,	// Unknown parse type, should NOT be in actual structs after parsing
		BLK_COMMENT_START,	// from BlockCommentStart,		Block comment start character(s) (ex. /* in C++)
		BLK_COMMENT_END,	// from BlockCommentEnd,		Block comment end character(s) (ex. */ in C++)
		LINE_COMMENT_START,	// from LineCommentStart,		Single line or embedded comment character(s)
		DIRECTIVE,			// from directive,				Directive (or preprocessor) statement keywords
		DATA_NAME,			// from data_name_list,			Data statement keywords
		EXEC_NAME,			// from exec_name_list,			Executable statement keywords
		MATH_FUNC,			// from math_func_list,			Math functions (not Trig or Log)
		TRIG_FUNC,			// from trig_func_list,			Trig functions
		LOG_FUNC,			// from log_func_list,			Log functions
		CMPLX_CALC,			// from cmplx_calc_list,		Calculations (complexity)
		CMPLX_COND,			// from cmplx_cond_list,		Conditionals (complexity)
		CMPLX_LOGIC,		// from cmplx_logic_list,		Logicals (complexity)
		CMPLX_PREPROC,		// from cmplx_preproc_list,		Preprocessor directives (complexity)
		CMPLX_ASSIGN,		// from cmplx_assign_list,		Assignments (complexity)
		CMPLX_POINTER,		// from cmplx_pointer_list,		Pointers (complexity)
		CMPLX_CYCLOMATIC,	// from cmplx_cyclomatic_list,	Cyclomatic complexity decision keywords (complexity)
		
		// These are found when doing first low level parsing
		IDENTIFIER,			// could be a valid identifier or resolve to a keyword type (above) in later parsing
		NUMBER_PART,		// could be part or entire number
		NUMBER_LONG,		// known to be a  Long
		NUMBER_INT,			// known to be an Integer
		NUMBER_SHORT,		// known to be a  Short
		NUMBER_FLOAT,		// known to be a  Float
		NUMBER_DOUBLE,		// known to be a  Double
		STRING_LITERAL,		// a string that begins and ends with valid Quote symbols within a line
		STRING_MULTILINE_START,	// begins with valid Quote multiline symbols, end may be on following line(s)
		STRING_MULTILINE_END,	//  ends  with valid Quote multiline symbols, start may be from previous line(s)
		OPEN_CURLY_BRACE,	// {
		CLOSE_CURLY_BRACE,	// }
		OPEN_SQUARE_BRACKET, // [
		CLOSE_SQUARE_BRACKET, // ]
		OPEN_PAREN,			// (
		CLOSE_PAREN,		// )
		LOGICAL_END,		// Statement delimiter.  C++ uses semicolon ;  Scala semicolon is usually optional
		WILDCARD,			// Match any found; in import statements (or elsewhere _ for Scala) Java uses *
		SCOPE_RESOLUTION,	// Declare the scope on the left.  C++ uses :: or . or -> (pointer later)  Scala uses .
		DELIM_COMMA,		// Comma symbol
		DLIM_BLANK,			// Blank space delimiter (Scala uses intstead of Comma for example)
		TYPE_PREFIX_COLON,	// single Colon used to prefix a Type in Scala, maybe used for other purposes?
		DOUBLE_COLON,		// 2 Colon characters used to declare class members in C++ for instance
		REFERENCE,			// C++ uses type &   Scala implies a reference?
		ADDRESS_OF			// C++ uses & expression  Not used in Scala?
	};
#endif

	// Defines for bit masks for Auxillary meanings of Keywords (must be powers of 2)
	//
	// Flag for each Cyclomatic Complexity ring to allow parser to avoid some hard coded compares
	#define		CYCLOMATIC_CC1			1
	#define		CYCLOMATIC_CC2			2
	#define		CYCLOMATIC_CC3			4
	#define		CYCLOMATIC_CC4			8
	#define		CYCLOMATIC_CC_ALL		(CYCLOMATIC_CC1|CYCLOMATIC_CC2|CYCLOMATIC_CC3|CYCLOMATIC_CC4)
	#define		CYCLOMATIC_EXCEPT_CC4	(CYCLOMATIC_CC1|CYCLOMATIC_CC2|CYCLOMATIC_CC3)
	#define		CYCLOMATIC_EXCEPT_CC3	(CYCLOMATIC_CC1|CYCLOMATIC_CC2|CYCLOMATIC_CC4)
	// Use these to check in code if needed
	#define		IsCyclomaticAny(x)		(CYCLOMATIC_CC_ALL&x)
	#define		IsCyclomatic_1(x)		(CYCLOMATIC_CC1&x)
	#define		IsCyclomatic_2(x)		(CYCLOMATIC_CC2&x)
	#define		IsCyclomatic_3(x)		(CYCLOMATIC_CC3&x)
	#define		IsCyclomatic_4(x)		(CYCLOMATIC_CC4&x)

#ifdef	ENABLE_1_PASS_PARSER
	#define		IsComplexCalc(x)		(COMPLEX_CALC&x)
	#define		IsComplexPreProc(x)		(COMPLEX_PREPROC&x)

	// TODO:  implement OO and FP metrics
	#define		IS_OBJECT_ORIENTED		64

	#define		IS_FUNCTIONAL			128

	#define		USE_EXTRA_IDX			(COMPLEX_CALC|COMPLEX_PREPROC)
	#define		UseExtraIdx(x)			(USE_EXTRA_IDX&x)

	//! Structure to contain well known parse keywords/symbols and types of a language
	/*!
	* \struct parseElement
	*
	* Defines a structure to contain info to help parsing a given language
	*/
	struct parseElement
	{
		parseElement( const unsigned int original_idx, const unsigned int aux_meanings, const unsigned int ext_idx,
			const ParseType parse_type, const string parse_keyword )
		{
			idx       = original_idx;
			extra_idx = ext_idx;
			aux       = aux_meanings;
			type      = parse_type;
			keyword   = parse_keyword;
		}
		parseElement()
		{
			idx       = 0;
			extra_idx = 0;
			aux       = 0;
			type      = PARSE_UNKNOWN;
			keyword   = "";
		}
		unsigned int idx;		//!< Index of name array that held the keyword
		unsigned int extra_idx;	//!< Extra Index of to do Keywords with multiple meanings
		unsigned int aux;		//!< Auxillary meanings such as: is Cyclomatic
		ParseType	type;		//!< Type of element
		string		keyword;	//!< language keyword or special symbol(s)
	};

	//! Vector containing a list of parseElements.
	/*!
	* \typedef ParseElementVector
	*
	* Defines a vector containing a list of parseElements.
	*/
	typedef vector<parseElement> ParseElementVector;

	unsigned int parse_alloc_count;		//!< number of elements in pe array
	parseElement * pe;					//!< pointer to array to search when parsing
	bool		pe_dup_keywords;		//!< true if duplicate keywords of different types
	unsigned int pe_keyword_max_size;	//!< size of longest keyword string of all entries in pe
	unsigned int pe_op_max_size;		//!< size of longest operator string; Calc, Logic, Assign
	unsigned int pe_max_idx;			//!< index of end +1 of parse array
	unsigned int pe_mid_idx;			//!< index of mid point of parse array
	string pe_mid_keyword;				//!< keyword value at mid point

#endif	// #ifdef	ENABLE_1_PASS_PARSER

	// Modification: 2015.12
	virtual int CountCommentsSLOCperLine( filemap::iterator iter,
										results* result,
										filemap::iterator itfmBak,
										bool & contd, 
										int & comment_type,
										bool & contd_nextline,
										size_t & idx_start,
										size_t & idx_end,
										size_t & quote_idx_start,
										size_t & comment_start,
										string & curBlckCmtStart,
										string & curBlckCmtEnd,
										bool & quote_contd,
										char & CurrentQuoteEnd );
	int FindCommentStart(string strline, size_t &idx_start, int &comment_type,
		string &curBlckCmtStart, string &curBlckCmtEnd);
	virtual int CountComplexity(filemap* fmap, results* result);
	virtual bool CanProcessCyclomaticComplexity( const results* result );		// Modification: 2015.12
	virtual int CountDirectiveSLOC(filemap* /*fmap*/, results* /*result*/, filemap* /*fmapBak = NULL*/) { return 0; }
	virtual int LanguageSpecificProcess(filemap* fmap, results* result, filemap* fmapBak = NULL);
	virtual int ParseFunctionName(const string & /*line*/, string & /*lastline*/,
		filemap & /*functionStack*/, string & /*functionName*/, unsigned int & /*functionCount*/) { return 0; }

#ifdef	ENABLE_1_PASS_PARSER
	//!< Set up some parse helpers
	int PreParseFile( const results* result );
	void SetInPe( StringVector & keys, UIntVector & vals, const bool useExtraIdx );
	void AddToParse( unsigned int	& index,
					unsigned int &	max_str_size,
					unsigned int &	empty_string_count, 
					const ParseType	type,
					StringVector	sv );
	#define NO_MATCH (-1)
	int FindInPe( const string value, unsigned int & match_count, ParseType & type_found, unsigned int & idx );
	int FindSpecialInPe( const string value, unsigned int & match_count, 
						ParseType & type_found, unsigned int & original_idx, unsigned int & size_found );
	//!< Do a reduced pass parse
	#define NO_PARSE_DONE (-2)
	int ParseFile( filemap* fmap, results* result, filemap* fmapBak );
	int ParseLine( const unsigned long cur_line, filemap::iterator iter, results* result, filemap::iterator itfmBak );
	unsigned int StrTok( const string input, const char * separators, StringVector & tokens );
	unsigned int GetTokens( const string input, const string unchanged, ParseElementVector & parse_elements );
	int UpdateKeywordCount( const ParseType type_found, const unsigned int org_idx, results* result,
							unsigned int &direct_items,
							unsigned int &data_items, 
							unsigned int &exec_items,
							unsigned int &math_items,
							unsigned int &trig_items,
							unsigned int &logarithm_items,
							unsigned int &cmplx_calc_items,
							unsigned int &cmplx_cond_items,
							unsigned int &cmplx_logic_items,
							unsigned int &cmplx_preproc_items,
							unsigned int &cmplx_assign_items,
							unsigned int &cmplx_pointer_items );
#endif	// #ifdef	ENABLE_1_PASS_PARSER

	StringVector  exclude_keywords;			//!< List of keywords to exclude from counts

	// if language supports multiple quote marks such as javascript, you can put all of them here, ex. "\"'"
	string QuoteStart;						//!< Starting quotation mark(s)
	string QuoteEnd;						//!< Ending quotation mark(s)

	bool quote_start_is_end;				//!< true if the start and end single line Quote symbols are equal 
	string QuoteMultiStart;					//!< Start of a multiline Quoted string literal
	string QuoteMultiEnd;					//!< End   of a multiline Quoted string literal
	bool quote_multi_start_is_end;			//!< true if the start and end of multi line Quote symbols are equal 

	char QuoteEscapeFront;					//!< Escape character for front quote (ex. '\' in C++)
	char QuoteEscapeRear;					//!< Escape character for rear quote
	string ContinueLine;					//!< Line continuation character(s) (ex. \\ in C++)

	bool casesensitive;						//!< Is language is case sensitive?

	ofstream output_file;					//!< Output file stream
	ofstream output_file_csv;				//!< Output CSV file stream

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CCodeCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CCodeCounter(const CCodeCounter& rhs);

	// Take care of warning C4626: 'CCodeCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CCodeCounter operator=(const CCodeCounter);
};


//! Map containing a logical Language enum and a pointer to Parser class for that language.
/*!
* \typedef CounterForEachLangType
*
* Defines a map containing a list of strings.
*/
typedef map<int, CCodeCounter*> CounterForEachLangType;

#endif

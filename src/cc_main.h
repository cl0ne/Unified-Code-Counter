//! Main class containing results.
/*!
* \file cc_main.h
*
* This file contains the main class definition containing results.
*/

#ifndef cc_main_h
#define cc_main_h

#include "UCCBeforeLibraryIncludes.h"    // Modification: 2015.12
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <utility>
#include <map>
#include <stack>
#include <set>
#include <algorithm>
#include "UCCAfterLibraryIncludes.h"    // Modification: 2015.12

using namespace std;

#define PRODUCT_REVISION "2018.07"      // Modification: 2018.07

#define PHY		0
#define LOG		1
#define CODE	0
#define DATA	1

#define MAX_LINE_LENGTH		  10485760

// Statistics about Baseline A and B
extern		string			commonPathPrefixA;
extern unsigned long		numFilesInA;
extern unsigned long long	totalFileSizesA;
extern unsigned long long	largestFileSizeA;

extern		string			commonPathPrefixB;
extern unsigned long		numFilesInB;
extern unsigned long long	totalFileSizesB;
extern unsigned long long	largestFileSizeB;

// Used to speed up Diff directory Path comparisons
extern		string			commonPathPrefixBoth;

// Clean up Baseline stats to allow multiple uses of UCC (Qt style)
void ClearBaselineStats();

/*!
* \enum ClassType
*
* Enumeration of file class types (languages).
*/
enum ClassType {
	UNKNOWN,				// Unknown file type	see CLASS_TYPE_LOWEST const int below
	DATAFILE,				// Data file type
	WEB,					// WebCounter
	ADA,					// ADA
	ASSEMBLY,				// Assembly languages
	BATCH,					// DOS_Batch
	BASH,					// Bash
	COBOL,					// COBOL
	CSH,					// C-Shell
	C_CPP,					// C/C++
	CSHARP,					// C#
	CSHARP_HTML,			// C# in HTML
	CSHARP_XML,				// C# in XML
	CSHARP_ASP_S,			// C# server side in ASP
	COLDFUSION,				// ColdFusion
	CFSCRIPT,				// ColdFusion CFScript
	CSS,					// CSS
	FORTRAN,				// Fortran
	HTML,					// HTML
	HTML_PHP,				// HTML in PHP
	HTML_JSP,				// HTML in JSP
	HTML_ASP,				// HTML in ASP
	HTML_CFM,				// HTML in ColdFusion
	IDL,                    		// IDL
	JAVA,					// Java
	JAVA_JSP,				// Java	in JSP
	JAVASCRIPT,				// JavaScript
	JAVASCRIPT_HTML,		// JavaScript in HTML
	JAVASCRIPT_XML,			// JavaScript in XML
	JAVASCRIPT_PHP,			// JavaScript in PHP
	JAVASCRIPT_JSP,			// JavaScript in JSP
	JAVASCRIPT_ASP_S,		// JavaScript server side in ASP
	JAVASCRIPT_ASP_C,		// JavaScript client side in ASP
	JAVASCRIPT_CFM,			// Javascript in ColdFusion
	MAKEFILE,				// Makefiles
	MATLAB,					// MATLAB
	NEXTMIDAS,				// NeXtMidas
	XMIDAS,					// X-Midas
	OBJC,					// Objective C - Modifacation: 2016.07
	PASCAL,					// Pascal
	PERL,					// Perl
	PHP,					// PHP
	PYTHON,					// Python
	RUBY,					// Ruby
	SCALA,					// Scala
	SQL,					// SQL
	SQL_CFM,				// SQL in ColdFusion
	VB,						// Visual Basic
	VBSCRIPT,				// VBScript
	VBS_HTML,				// VBScript in HTML
	VBS_XML,				// VBScript in XML
	VBS_PHP,				// VBScript in PHP
	VBS_JSP,				// VBScript in JSP
	VBS_ASP_S,				// VBScript server side in ASP
	VBS_ASP_C,				// VBScript client side in ASP
	VBS_CFM,				// VBScript in ColdFusion
	VERILOG,				// Verilog
	VHDL,					// VHDL
	XML						// XML			keep as reference to last, see CLASS_TYPE_HIGHEST below
};

// These 2 help with some processing where number of different class types must be known
const	int		CLASS_TYPE_LOWEST  = UNKNOWN;

// If another Language class type gets added after XML edit this !
const	int		CLASS_TYPE_HIGHEST = XML;


#define EMBEDDED_FILE_PREFIX  "*.*" // the text prepended to the temp file created for embedded code
#define DEFAULT_TRUNCATE      10000
#define DIFF_MATCH_THRESHOLD  60

#define INPUT_FILE_LIST_OLD   "filelist.dat"
#define INPUT_FILE_LIST_NAME  "fileList.txt"

// Output file names moved to UCCFilesOut.h


///////////////////////////////////////////////////////
// For WebCounter Only
///////////////////////////////////////////////////////

/*!
* \enum WebTag
*
* Enumeration of web script tags.
*/
enum WebTag {
	WEB_PHP_START,
	WEB_PHP_START2,
	WEB_PHP_END,
	WEB_SCRIPT_START,
	WEB_SCRIPT_END,
	WEB_ASP_JSP_START,
	WEB_ASP_JSP_END,
	WEB_CFM_START,
	WEB_CFM_END
};

///////////////////////////////////////////////////////
// General Data Structures
///////////////////////////////////////////////////////

//! Structure to contain a physical line of code.
/*!
* \struct lineElement
*
* Defines a structure to contain a physical line of code.
*/
struct lineElement
{
	lineElement(unsigned int num, string str)
	{
		lineNumber = num;
		line = str;
	}
	lineElement()
	{
		lineNumber = 0;
		line = "";
	}
	unsigned int lineNumber;	//!< Line number
	string line;				//!< Line text
};

//! Vector containing all physical line elements in a file.
/*!
* \typedef filemap
*
* Defines a vector containing all physical line elements in a file.
*/
typedef vector<lineElement> filemap;

//! Map containing all logical lines in a file.
/*!
* \typedef srcLineVector
*
* Defines a map containing all logical lines in a file for differencing.
*/
typedef map<string, vector<size_t> > srcLineVector;

//! Vector containing a list of strings.
/*!
* \typedef StringVector
*
* Defines a vector containing a list of strings.
*/
typedef vector<string> StringVector;

//! Vector containing a list of unsigned integers.
/*!
* \typedef UIntVector
*
* Defines a vector containing a list of unsigned integers.
*/
typedef vector<unsigned int> UIntVector;

//! Pair of unsigned integers.
/*!
* \typedef UIntPair
*
* Defines a pair of unsigned integers.
*/
typedef pair<unsigned int, unsigned int> UIntPair;

//! Vector containing a list of unsigned integer pairs.
/*!
* \typedef UIntPairVector
*
* Defines a vector containing a list of unsigned integer pairs.
*/
typedef vector<UIntPair> UIntPairVector;

//Modification: 2018.01 Integration starts
//! Maps an operator/operand to its count (number of occurrences)
/*!
* \typedef OpCountMap
*
* Defines a map that maps an operand/operator to the number of times it occurs
*/
typedef map<string, unsigned int> OpCountMap;

//! Maps a function to its OpMapCount
/*!
* \typedef FuncOpMapCount
*
* Defines a map containing the function and its operator/operand counts
*/
typedef map<string, OpCountMap> FuncOpCountMap;
//Modification: 2018.01 Integration ends

//! Class to hold results for each file.
/*!
* \class results
*
* Defines a class to hold file results.
*/
class results
{
public:
	results() { reset(); }				//!< Constructor
	results& operator= (const results&);//!< Assignment operator
	void reset();						//!< Method that resets/zeroes out everything

	// Support for reload/reprocess of a file to allow overall MUCH lower use of RAM
	void resetMetrics();				//!< Only set Metrics counts to zero, everything else left as is

	void clearSLOC();					//!< Method that frees memory used to store logical SLOC
	bool addSLOC(const string &line, bool &trunc_flag);	//!< Method to store a logical source line of code

	bool addSLOC(const string &line, const size_t line_num, bool &trunc_flag);	//!< Method to store a logical source line of code

#ifdef	_DEBUG
	// Call (for example) to make sure a long list of files is OK for Duplicate checking or Differencing
	bool isEmbeddedConsistent();
#endif

	string error_code;					//!< Error string
	unsigned int blank_lines;			//!< # of blank lines
	unsigned int comment_lines;			//!< # of whole line comment lines
	unsigned int e_comm_lines;			//!< # of embedded comment lines
	unsigned int directive_lines[2];	//!< # of compiler directive lines
	unsigned int data_lines[2];			//!< # of data declaration lines
	unsigned int exec_lines[2];			//!< # of executable lines
	unsigned int SLOC_lines[2];			//!< # of SLOC lines
	unsigned int total_lines;			//!< # of total lines
	unsigned int cmplx_math_lines;		//!< # of math function call lines
	unsigned int cmplx_trig_lines;		//!< # of trig function call lines
	unsigned int cmplx_logarithm_lines;	//!< # of logarithm function call lines
	unsigned int cmplx_calc_lines;		//!< # of calculations
	unsigned int cmplx_cond_lines;		//!< # of conditionals
	unsigned int cmplx_logic_lines;		//!< # of logical operations
	unsigned int cmplx_preproc_lines;	//!< # of preprocessor directive lines for complexity
	unsigned int cmplx_assign_lines;	//!< # of assignments
	unsigned int cmplx_pointer_lines;	//!< # of pointers
	unsigned int trunc_lines;			//!< # of truncated logical lines
	bool      e_flag;					//!< Set to true if error occurs during processing
	bool      file_name_isEmbedded;		//!< true if an Embedded file with *.* as part of the name  Modification: 2015.12
	string    file_name;				//!< File path and name
	string    file_name_only;			//!< File name only, used by Duplicate (a lot) & Differencing  Modification: 2015.12
	string    clearCaseTrailer;			//!< Saved to allow Reading source lines later than other file info  Modification: 2015.12
	int       file_type;				//!< File type (DATA, CODE)
	unsigned int file_size;             //!< Number of Physical lines in file
	streamoff    file_size_bytes;       //!< Number of bytes in physical file. 64 bits (using 32 bit VC++ compiler)  Modification: 2015.12
//    string file_hash;                   //!< Pseudo-unique hash defining file contents
	ClassType class_type;				//!< File class type

	UIntVector directive_count;			//!< Count of each directive statement keyword
	UIntVector data_name_count;			//!< Count of each data statement keyword
	UIntVector exec_name_count;			//!< Count of each executable statement keyword

	UIntVector math_func_count;			//!< Count of math functions
	UIntVector trig_func_count;			//!< Count of trigonometric functions
	UIntVector log_func_count;			//!< Count of logarithmic functions
	UIntVector cmplx_calc_count;		//!< Count of calculations
	UIntVector cmplx_cond_count;		//!< Count of conditionals
	UIntVector cmplx_logic_count;		//!< Count of logicals
	UIntVector cmplx_preproc_count;		//!< Count of preprocessor directives
	UIntVector cmplx_assign_count;		//!< Count of assignments
	UIntVector cmplx_pointer_count;		//!< Count of pointers
	UIntVector cmplx_nestloop_count;	//!< Count of nested loop levels
	filemap cmplx_cycfunct_count;		//!< Cyclomatic complexity by function
	filemap cmplx_cycfunct_CC2_count;		//!< Cyclomatic complexity by function
	filemap cmplx_cycfunct_CC3_count;		//!< Cyclomatic complexity by function
	filemap cmplx_cycfunct_CC4_count;       //!< Cyclomatic complexity by function

        //Modification: 2018.01 Integration starts
        // Halstead's volume
    FuncOpCountMap func_string_and_char_operand_counts;  //!< Counts of string and character operands by function
    FuncOpCountMap func_word_operand_counts;             //!< Counts of word operands by function (e.g. variable names)
    FuncOpCountMap func_bool_operand_counts;             //!< Counts of bool operands by function
    FuncOpCountMap func_number_operand_counts;           //!< Counts of number operands by function

    FuncOpCountMap func_function_operator_counts;        //!< Counts of function operators by function
    FuncOpCountMap func_nonfunction_operator_counts;     //!< Counts of nonfunction operators by function

    OpCountMap file_string_and_char_operand_counts;      //!< Counts of string and character operands for the whole file
    OpCountMap file_word_operand_counts;             //!< Counts of variable operands for the whole file 
    OpCountMap file_bool_operand_counts;                 //!< Counts of bool operands for the whole file
    OpCountMap file_number_operand_counts;               //!< Counts of number operands for the whole file

    OpCountMap file_function_operator_counts;            //!< Counts of function operators for the whole file
    OpCountMap file_nonfunction_operator_counts;         //!< Counts of nonfunction operators for the whole file

    map<string, double> func_halsteads_volume;           //!< Maps the functions in the file to their Halstead's volume

    double halsteads_volume;                             //!< Halstead's volume of the file

    //Modification: 2018.01 Integration ends
        
	// differencing
	bool firstDuplicate;				//!< Is this file the first (source) duplicate? (printed in main counting file)
	bool duplicate;						//!< Is this file a duplicate? (printed in Duplicates counting file)
	bool matched;						//!< Has matching file been found for differencing?
	srcLineVector mySLOCLines;			//!< Contains the actual logical lines of code for differencing
	unsigned int mySLOC_size;           //!< Number of logical lines
};

//! Pair containing a filemap and a results set.
/*!
* \typedef SourceFileElement
*
* Defines a pair containing a filemap and a results set.
*/
typedef pair<filemap, results> SourceFileElement;

//! List of source file elements.
/*!
* \typedef SourceFileList
*
* Defines a list of source file elements.
*/
typedef list<SourceFileElement> SourceFileList;

#endif

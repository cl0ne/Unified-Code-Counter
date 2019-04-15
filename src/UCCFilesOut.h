//! UCC File(s) Output procedures public declarations
/*!
* \file UCCFilesOut.h
*
* This file gives public interfaces for the majority of UCC file output.
*
* ADDED to UCC 2015_12 release by Randy Maxwell
*   Changes started on 2015_10_02
*   Changes  ended  on 2015_10_06
*
*   See comments for rationale and some implementation notes in .cpp file
*/

#ifndef UCC_FILES_OUTPUT_H
#define UCC_FILES_OUTPUT_H

#include	"CWebCounter.h"

// Various Output file names or name parts
#define OUTPUT_FILE_NAME      "_outfile.txt"
#define OUTPUT_FILE_NAME_CSV  "_outfile.csv"
#define OUTPUT_FILE_CPLX	  "outfile_cplx.txt"
#define OUTPUT_FILE_CPLX_CSV  "outfile_cplx.csv"
#define OUTPUT_FILE_SUM	      "outfile_summary.txt"
#define OUTPUT_FILE_SUM_CSV   "outfile_summary.csv"
#define OUTPUT_FILE_CYC_CPLX  "outfile_cyclomatic_cplx.txt"
#define OUTPUT_FILE_CYC_CPLX_CSV  "outfile_cyclomatic_cplx.csv"
//Added by Yash Lamba on 09/26/2017
//#define OUTPUT_FILE_HAL_VOL   "outfile_halstead_vol.txt"
//#define OUTPUT_FILE_HAL_VOL_CSV              "outfile_halstead_vol.csv"
#define OUTPUT_FILE_MAINTAIN_INDEX  "outfile_maintainability_index.txt"
#define OUTPUT_FILE_MAINTAIN_INDEX_CSV  "outfile_maintainability_index.csv"
//#define LOG_FILENAME		  "log.txt"


extern	string	outDir;			//!< Output directory


	//! Structure to contain count totals.
	/*!
	* \struct TotalValue
	*
	* Defines a structure to contain count totals.
	*/
	struct TotalValue
	{
		TotalValue()
		{
			total_line = 0;
			blank_line = 0;
			whole_comment = 0;
			embed_comment = 0;
			phy_direct = 0;
			phy_decl = 0;
			phy_instr = 0;
			log_direct = 0;
			log_decl = 0;
			log_instr = 0;
			num_of_file = 0;
		}

		unsigned int total_line;					//!< Total lines
		unsigned int blank_line;					//!< Blank lines
		unsigned int whole_comment;					//!< Whole line comments
		unsigned int embed_comment;					//!< Embedded comments
		unsigned int phy_direct;					//!< Physical directive SLOC
		unsigned int phy_decl;						//!< Physical data declaration SLOC
		unsigned int phy_instr;						//!< Physical executable instruction SLOC
		unsigned int log_direct;					//!< Logical directive SLOC
		unsigned int log_decl;						//!< Logical data declaration SLOC
		unsigned int log_instr;						//!< Logical executable instruction SLOC
		unsigned int num_of_file;					//!< Number of files
	};

	//! Map of count totals.
	/*!
	* \typedef TotalValueMap
	*
	* Defines a map of count totals.
	*/
	typedef map<int, TotalValue> TotalValueMap;


	//! Structure to contain web language count totals.
	/*!
	* \struct WebTotalValue
	*
	* Defines a structure to contain web language count totals.
	*
	* TODO: Replace the magic number name with a better named define for whatever 6 means
	*/
	#define	WEB_MAGIC_NUMBER	6
	struct WebTotalValue
	{
		WebTotalValue()
		{
			total_line = 0;
			blank_line = 0;
			whole_comment = 0;
			embed_comment = 0;
			for (int i = 0; i < WEB_MAGIC_NUMBER; i++)
			{
				phy_direct[i] = 0;
				phy_decl[i] = 0;
				phy_instr[i] = 0;
				log_direct[i] = 0;
				log_decl[i] = 0;
				log_instr[i] = 0;
			}
			num_of_file = 0;
		}

		unsigned int total_line;					//!< Total lines
		unsigned int blank_line;					//!< Blank lines
		unsigned int whole_comment;					//!< Whole line comments
		unsigned int embed_comment;					//!< Embedded comments
		unsigned int phy_direct[WEB_MAGIC_NUMBER];	//!< Physical directive SLOC
		unsigned int phy_decl[WEB_MAGIC_NUMBER];	//!< Physical data declaration SLOC
		unsigned int phy_instr[WEB_MAGIC_NUMBER];	//!< Physical executable instruction SLOC
		unsigned int log_direct[WEB_MAGIC_NUMBER];	//!< Logical directive SLOC
		unsigned int log_decl[WEB_MAGIC_NUMBER];	//!< Logical data declaration SLOC
		unsigned int log_instr[WEB_MAGIC_NUMBER];	//!< Logical executable instruction SLOC
		unsigned int num_of_file;					//!< Number of files
	};

	//! Map of web language count totals.
	/*!
	* \typedef WebTotalValueMap
	*
	* Defines a map of web language count totals.
	*/
	typedef map<WebType, WebTotalValue> WebTotalValueMap;


	//! Structure to contain all web language count totals for unified output.
	/*!
	* \struct AllWebTotalValue
	*
	* Defines a structure to contain all web language count totals.
	* If new web language is added, must update the count value here
	*  0 - HTML    1 - XML    2 - JSclnt  3 - VBSclnt
	*  4 - C#clnt  5 - JSsrv  6 - VBSsrv  7 - C#srv
	*  8 - PHP	   9 - Java  10 - SQL    11 - CF      12 - CFS
	*/
	#define WEB_LANG_COUNT	13
	struct AllWebTotalValue
	{
		AllWebTotalValue()
		{
			total_line = 0;
			blank_line = 0;
			whole_comment = 0;
			embed_comment = 0;
			for (int i = 0; i < WEB_LANG_COUNT; i++)
			{
				phy_direct[i] = 0;
				phy_decl[i] = 0;
				phy_instr[i] = 0;
				log_direct[i] = 0;
				log_decl[i] = 0;
				log_instr[i] = 0;
			}
			num_of_file = 0;
		}

		unsigned int total_line;					//!< Total lines
		unsigned int blank_line;					//!< Blank lines
		unsigned int whole_comment;					//!< Whole line comments
		unsigned int embed_comment;					//!< Embedded comments
		unsigned int phy_direct[WEB_LANG_COUNT];	//!< Physical directive SLOC
		unsigned int phy_decl[WEB_LANG_COUNT];		//!< Physical data declaration SLOC
		unsigned int phy_instr[WEB_LANG_COUNT];		//!< Physical executable instruction SLOC
		unsigned int log_direct[WEB_LANG_COUNT];	//!< Logical directive SLOC
		unsigned int log_decl[WEB_LANG_COUNT];		//!< Logical data declaration SLOC
		unsigned int log_instr[WEB_LANG_COUNT];		//!< Logical executable instruction SLOC
		unsigned int num_of_file;					//!< Number of files
	};


// Public Print (file output) utility procedures

int PrintFileHeader( ofstream &pout, const string &title, const string &cmd = "" );

int PrintFileHeaderLine( ofstream &pout, const string &line );

// Modification 2016.12
int PrintFileFooter(ofstream &pout);

int PrintCountResults( CounterForEachLangType & CounterForEachLanguage,
						const bool useListA = true, const string &outputFileNamePrePend = "",
						StringVector* filesToPrint = NULL, const bool excludeFiles = true );

int PrintTotalCountResults( CounterForEachLangType & CounterForEachLanguage,
							const bool useListA = true, const string &outputFileNamePrePend = "",
							StringVector* filesToPrint = NULL, const bool excludeFiles = true );

int PrintComplexityResults( CounterForEachLangType & CounterForEachLanguage, 
							const bool useListA = true, const string &outputFileNamePrePend = "", 
							const bool printDuplicates = false);

void PrintDuplicateSummary( const bool useListA = true, const string &outputFileNamePrePend = "" );

int PrintMaintainabilityIndex(const bool useListA, const string &outputFileNamePrePend, const bool printDuplicates);       // Modification: 2017.10


string WriteUncountedFile( const string &msg, const string &uncFile, 
						   const bool useListA, const bool csvOutput, const string outDir );

// added summer-2015 (moved from CCodeCounter.h)
// void WriteLog( const char* logString );   // write message to traceFile.txt 
void AddTraceLine( const int lineNum, const string funcName, const string line ); // add line to traceFile.txt

#endif	//	#ifndef UCC_FILES_OUTPUT_H

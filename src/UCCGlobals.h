//! UCC Global values, defines and structures
/*!
* \file UCCGlobals.h
*
* Added to UCC 2015_12 release by Randy Maxwell
*   Changes started on 2015_10_02
*   Changes  ended  on 2015_10_06
*
* This file helps to partition the UCC application to gracefully support
* multiple concurrent worker threads (not really so much here)
* looser coupling of functionality (in some ways)
*	violated class encapsulation to move some of these here
*		BUT
*	these declarations are needed by utility code that really does not
*	have to be embedded in a class (UCCFilesOut and MainObject for example)
*/

#ifndef UCC_GLOBALS_H
#define UCC_GLOBALS_H

#include	"UCCBeforeLibraryIncludes.h"
#include	<string>
#include	<vector>
#include	"UCCAfterLibraryIncludes.h"


using namespace std;

#include	"cc_main.h"


#define DEF_LANG_NAME		  "UNDEF"

// Counts of significant UCC message types
extern unsigned int		stack_dump_count;
extern unsigned int		errors_count;
extern unsigned int		warnings_count;
extern unsigned int		information_count;
extern unsigned long	uncounted_files_count;

// Set in main() but can be used by Stack Dump and any where else.
extern	time_t	timeStart;
extern	time_t	timeEnd;

// Time values filled in by Diff in order (and following Dup if wanted)
//
// File details known, each baseline has some stats, Paths known to support Diff
extern	time_t	time_end_list_built;

// Path matching for regular and Web files
extern	time_t	time_end_match_baselines;		// Differencing step only
// extern	time_t	time_end_match_baselines_web;	// Differencing step only

// List of matched file pairs: Read/Analyze/Count each then Diff and save result files
extern	time_t	time_end_process_pairs;			// Differencing step only

// Duplicate checking for baseline A.  
// NOT recommended to do with Diff for large file sets.
extern	time_t	time_end_find_duplicates;
extern	time_t	time_end_print_results;

extern	time_t	time_end_find_duplicatesB;
extern	time_t	time_end_print_resultsB;


// Time values filled in by MainObject in order.  (If declared with Diff above commented out)
//extern	time_t	time_end_list_built;
extern	time_t	time_end_files_read;
extern	time_t	time_end_files_analysis;
extern	time_t	time_end_find_duplicates;
//extern	time_t	time_end_print_results;



//
//	PUBLIC declarations of Global values which are changed from the MAIN Thread ONLY
//
extern	string		cmdLine;							//!< Executed command line string
extern  string      userHeaderFile;                     //!< Custom Header file path

extern	bool		isDiff;								//!< Has differencing been requested (-d)?
extern  bool        isFuncDiff;                         //!< Has function level differencing been requested (-funcDiff)? Modification 2017.02
// extern	bool	visualDiff;							// Visual diff to file and HTML, must explictly declare extern elsewhere

extern	bool		isDup;								//!< Is Duplicate checking wanted ? ( duplicate_threshold >= 0.0 )

extern	double		duplicate_threshold;				//!< % changed threshold for determining duplicate content

//extern	bool		traceException;						//!< flag for -trace option

// Optimizations to do more processing per unit time
extern	bool	g_process_after_read;
extern	bool	g_discard_PHY_lines_after_process;
extern	bool	g_discard_lines_after_process;			// Discard Physical and Logical source lines

// These are commented out to prevent thread code from using these (must be passed by value as arg if needed)
// extern	bool print_cmplx;								//!< Print complexity and keyword counts
// extern	bool print_csv;									//!< Print CSV report files
// extern	bool g_cc4enable;								//!< CC4 (unique if condition) scans. Print complex must be true

extern	bool print_ascii;								//!< Print ASCII text report files
extern	bool print_legacy;								//!< Print legacy formatted ASCII text report files
extern	bool print_unified;								//!< Print all counting files in a single unified file
extern  bool remove_Header;                             //!< Remove Header from output files if set to True


extern	bool g_no_warnings_to_UI;						//!< Suppress Warning messages to UI if true.  Still gets logged.

extern	bool g_no_uncounted;							//!< Suppress any Uncounted files messages or reports

extern	unsigned int  workThreadsCount;					//!< Number of helper work threads to create. May be zero if not given.

extern	unsigned int  ramLimit;							//!< Number of 100 MegaByte increments to use.  Default is 500 MB or ~ 1/2 GB.

#define		MIN_UCC_RAM_LIMIT		1
#define		DEFAULT_UCC_RAM_LIMIT	5
#define		MAX_UCC_RAM_LIMIT		5120

extern	size_t lsloc_truncate;							//!< # of characters allowed in LSLOC for differencing (0=no truncation)

extern	StringVector duplicateFilesInA1;				//!< List of duplicate file sources in baseline A
extern	StringVector duplicateFilesInA2;				//!< List of duplicate files in baseline A
extern	StringVector duplicateFilesInB1;				//!< List of duplicate file sources in baseline B
extern	StringVector duplicateFilesInB2;				//!< List of duplicate files in baseline B

extern	SourceFileList SourceFileA;						//!< List of source files in baseline A
extern	SourceFileList SourceFileB;						//!< List of source files in baseline B

extern	string	main_parse_path_file;					//!< Path and file name being parsed if only single thread  2016

#endif	// #ifndef UCC_GLOBALS_H

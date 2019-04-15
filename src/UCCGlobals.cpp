//! UCC Global values and structures
/*!
* \file UCCGlobals.cpp
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

#include "UCCBeforeLibraryIncludes.h"
#include <string>
#include "UCCAfterLibraryIncludes.h"

using namespace std;


#include	"cc_main.h"
#include	"UserIF.h"
#include	"UCCGlobals.h"

//
//	Declare single instance of Global variables accessed elsewhere
//
unsigned int	stack_dump_count      = 0;
unsigned int	errors_count          = 0;
unsigned int	warnings_count        = 0;
unsigned int	information_count     = 0;
unsigned long	uncounted_files_count = 0L;

// Declare some variables used by other code to get Timing of various steps

// These are set in main() but kept available for other code like Stack Dump
time_t	timeStart = 0;
time_t	timeEnd = 0;

	// These are in order of when Time is set for them
//
//		DIFF order
//
time_t	time_end_list_built = 0;

// Path matching for regular and Web files
time_t	time_end_match_baselines = 0;			// Differencing step only
// time_t	time_end_match_baselines_web = 0;		// Differencing step only		No longer used?

// List of matched file pairs: Read/Analyze/Count each then Diff and save result files
time_t	time_end_process_pairs = 0;				// Differencing step only

// Duplicate checking for baseline A.  
// NOT recommended to do with Diff for large file sets.
time_t	time_end_find_duplicates = 0;
time_t	time_end_print_results = 0;

// Duplicate checking for baseline B.  
// NOT recommended to do with Diff for large file sets.
time_t	time_end_find_duplicatesB = 0;
time_t	time_end_print_resultsB = 0;

// ------------------------------------------------------------------
//
//		Time values without DIFF may also have Duplicate checking
//

// time_t	time_end_list_built;	as above

time_t	time_end_files_read = 0;
time_t	time_end_files_analysis = 0;

// time_t	time_end_find_duplicates;	as above
// time_t	time_end_print_results;		as above



//
//	These are changed from the MAIN Thread ONLY
//

string		cmdLine;						//!< Executed command line string
string userHeaderFile;                      //!< Custom Header file path 

bool		isDiff = false;					//!< Has differencing been requested (-d)?
bool           isFuncDiff = false;                             //!< Has function level differencing been requested (-funcDiff)? Modification 2017.02
bool		visualDiff = false;				//!< Print visual diff

bool		isDup = true;					//!< Is Duplicate checking wanted ? ( duplicate_threshold >= 0.0 )

double		duplicate_threshold = 0;		//!< % changed threshold for determining duplicate content

//bool	traceException = false;				//!< flag for -trace option

// Optimizations to do more processing per unit time
bool	g_process_after_read = false;
bool	g_discard_PHY_lines_after_process = false;
bool	g_discard_lines_after_process = false;

bool print_cmplx;								//!< Print complexity and keyword counts
bool g_cc4enable = false;						//!< CC4 (unique if condition) scans. Print complex must be true   Modification: 2015.12
bool print_csv;									//!< Print CSV report files
bool print_ascii;								//!< Print ASCII text report files
bool print_legacy;								//!< Print legacy formatted ASCII text report files
bool print_unified;								//!< Print all counting files in a single unified file
bool remove_Header;                             //!< Remove Header from output files if set to True


bool g_no_warnings_to_UI = false;				//!< Suppress Warning messages to UI if true.  Warnings are still logged.

bool g_no_uncounted = false;					//!< Suppress any Uncounted files messages or reports

unsigned int  workThreadsCount = 0;				//!< Number of helper work threads to create. May be zero if not given.

unsigned int  ramLimit = DEFAULT_UCC_RAM_LIMIT;	//!< Number of 100 MegaByte increments to use.  Default is 5 x 100 MB.

size_t lsloc_truncate;							//!< # of characters allowed in LSLOC for differencing (0=no truncation)

StringVector duplicateFilesInA1;				//!< List of duplicate file sources in baseline A
StringVector duplicateFilesInA2;				//!< List of duplicate files in baseline A
StringVector duplicateFilesInB1;				//!< List of duplicate file sources in baseline B
StringVector duplicateFilesInB2;				//!< List of duplicate files in baseline B

SourceFileList SourceFileA;						//!< List of source files in baseline A
SourceFileList SourceFileB;						//!< List of source files in baseline B

UserIF *userIF;									//!< User interface for presenting messages/progress to user

string	main_parse_path_file;					//!< Path and file name being parsed if only single thread  2016

//! UCC Thread support
/*!
* \file UCCThread.h
*
* This file encapsulates thread implementation details and dependencies
* so that the rest of UCC is relatively unchanged.
*
* ADDED to UCC 2015_12 release by Randy Maxwell
*   Changes started on 2015_10_02
*   Changes  ended  on 2015_11_22
*   Addition of Multithreading performance enhancement feature
*		Example: UCC -threads 4 (to have 4 worker threads)
*/

#ifndef UCC_THREAD_PUBLIC_H
#define UCC_THREAD_PUBLIC_H

#include "cc_main.h"
#include "CCodeCounter.h"

// It is really bad practice to let multiple threads access the UI.
// In this case only the Main thread which created the UI will have access.
//
// This supports shared code inside UCCThread.cpp that can also be called 
// to run when no extra worker threads were created.
#include "UserIF.h"

// Exception and Stack Dump support
#include "UCCExceptDump.h"

//
//	Uncomment this to get 2 or more worker threads supported.  Requires Qt or Boost libarary.
//
#ifdef	QTGUI
	// Threads always available with Qt
    #define		ENABLE_THREADS
#else
	// Available if Boost thread library installed, compiled, .h included and linked.
	//#define		ENABLE_THREADS
#endif

// OK even if not compiled with thread support
unsigned int	GetCPU_count();

//! Structure to contain an Error message and what to do with it.
/*!
* \struct errMsgStruct
*
* Allow Error handling/presentation to be decoupled from Thread execution.
*/
struct errMsgStruct
{
	errMsgStruct(const string &Err, bool LogOnly = false, unsigned int PreNL = 0, unsigned int PostNL = 1, unsigned int ThreadIdx = 0)
	{
		threadIdx = ThreadIdx;
		err       = Err;
		logOnly   = LogOnly;
		preNL     = PreNL;
		postNL    = PostNL;
	}
	errMsgStruct()
	{
		// Set defaults almost as if called
		threadIdx = 0;
		err       = "";
		logOnly   = false;
		preNL     = 0;
		postNL    = 1;
	}
	unsigned int	threadIdx;		//!< Zero based index of the thread that found the error
									//!< 0 for main thread and also for 1st created worker thread
	string			err;
	bool			logOnly;
	unsigned int	preNL;			//!< Number of NewLines to insert Before the Error
	unsigned int	postNL;			//!< Number of NewLines to insert After  the Error
};

//! Vector containing Error messages and what to do with them.
/*!
* \typedef ErrMsgStructVector
*
* Defines Error messages that are processed later 
* from the main thread and not the thread that found the Error.
*/
typedef vector<errMsgStruct> ErrMsgStructVector;

//! Structure to contain info about an Uncounted File and what to do with it.
/*!
* \struct uncountedFileStruct
*
* Allow Uncounted File handling/presentation to be decoupled from Thread execution.
*/
struct uncountedFileStruct
{
	uncountedFileStruct(const string &Msg, const string &UncFile, const string &OutDir, bool UseListA = true, bool CsvOutput = false, unsigned int ThreadIdx = 0)
	{
		threadIdx = ThreadIdx;
		msg       = Msg;
		uncFile   = UncFile;
		useListA  = UseListA;
		csvOutput = CsvOutput;
		outDir    = OutDir;
	}
	uncountedFileStruct()
	{
		// Set defaults almost as if called
		threadIdx = 0;
		msg       = "";
		uncFile   = "";
		useListA  = true;
		csvOutput = false;
		outDir    = "";
	}
	unsigned int	threadIdx;		//!< Zero based index of the thread that found the Uncounted File
									//!< 0 for main thread and also for 1st created worker thread
	string			msg;
	string			uncFile;
	bool			useListA;
	bool			csvOutput;		//!< 
	string			outDir;
};

//! Vector containing information on Uncounted Files and what to do with them.
/*!
* \typedef UncountedFileStructVector
*
* Defines Uncounted Files info structs that are processed later 
* from the main thread and not the thread that found them.
*/
typedef vector<uncountedFileStruct> UncountedFileStructVector;


//
//		These are the Public interfaces for UCC threads.
//
#ifdef	_DEBUG
	// Allow -threads 1 to be valid for DEBUG builds as then some debugging is MUCH easier.
	#define MIN_UCC_THREAD_COUNT	1
#else
	#define MIN_UCC_THREAD_COUNT	2
#endif
	#define MAX_UCC_THREAD_COUNT	(MIN_UCC_THREAD_COUNT+78)
	#define MAIN_THREAD_INDEX		(MAX_UCC_THREAD_COUNT+1)

	#define	THREADS_STARTED_OK	1

	// Minimum thread sleep time in milliseconds
	// Amount of actual time spent asleep is OS/HW and workload dependant.
	// Below is about 1/100 of a second
	#define		MIN_SLEEP_TIME		10
	#define		ONE_SECOND		1000

// Give up the rest of the time slice this thread has and
// Put the currently executing thread to sleep for approximately the time wanted
void ThreadSleep( const unsigned int milliSeconds = MIN_SLEEP_TIME );

/*!
*			StartWorkerThreads
* Set up and Start a number of worker threads.
* Any threads started will be waiting for either work to do or to exit thread if wanted.
*
* \param num_to_start	IN/OUT		ref to number of worker threads wanted to start
* \param message		IN/OUT		ref to message giving details for number actually started
*
* \returns							THREADS_STARTED_OK if number of threads wanted was started OK
*							or		positive number >= 2 giving number of threads actually started
*										message has INFO details
*							or		negative number giving Severe unrecoverable Error code
*										message has ERROR details
*
*	It is the calling code's responsibility to check for Errors.
*
* Recommended actions for Severe unrecoverable Errors: 
*		Log the ERROR with all given details
*		Continue running in single thread mode without any worker threads
*
* Recommended actions for the number of threads actually started being lower than wanted:
*		Log an INFO message about wanted and actual worker thread count
*		Go ahead and use the availble (smaller number) of worker threads
*/
int StartWorkerThreads( CounterForEachLangType * pCounterForEachLanguage, 
						UserIF			*		userIF,
						const unsigned int num_to_start, string & message );


/*!
*			FinishWorkerThreads
* Finish any running worker threads.
* This may block for a few seconds while worker threads exit if they were busy.
*/
void FinishWorkerThreads();

// Propagate back the results from recently finished worker threads
void CombineThreadResults();

#ifdef	ENABLE_THREADS
	// Get a pointer to a Thread's struct of Language parser class instance pointers.
	// Called only from Main thread AFTER the worker Thread is created but BEFORE it is busy.
	CounterForEachLangType * GetThread_pCounterForEachLanguage( const unsigned int threadIdx );

	// Return the number of threads working
	unsigned int NumThreadsBusy( unsigned long & count_done, 
							const unsigned int	sleep_milliseconds = 100, 
							const bool			combine_results_from_threads = true );
    // 2016   Return the path/file being parsed by a Thread		
	void GetThreadParseFile( const unsigned int threadIdx, string & path_file );
#endif

// Allow reading physical file lines now when other file info was found before.
// This will also do follow on processing after reading.
// Web files are read before as the Web implementation expands file list.
// Clearcase file name was set up but unless Web, source lines not read.
int ReadFileLines( SourceFileElement		*			pSrcFileEl,
					const	unsigned int				threadIdx,
					UserIF					*			userIF,
					const		bool					readWebFiles,
					const		bool					onlyPhysical,
					const		bool					clearCaseFile,
					CounterForEachLangType		&		CounterForEachLanguage,
					const		bool					print_cmplx,
					const		bool					print_csv,
					const		bool					useListA,
					SourceFileList			*			mySourceFileA,
					SourceFileList			*			mySourceFileB,
					string						&		myErrList,
					string								outDir,
					ErrMsgStructVector			&		err_msgs,
					UncountedFileStructVector	&		unc_files,
					const		bool					process_after_read,
					const		bool					discard_PHY_lines_after_process,
					const		bool					discard_lines_after_process,
					const		bool					no_warnings_to_UI );

// ReadFilesInList
// returns number of Errors found
int ReadFilesInList( const unsigned int					threadIdx,
					const		bool					noRead,
					const		bool					noWeb,
					UserIF					*			userIF,
					CounterForEachLangType		&		CounterForEachLanguage,
					const		bool					print_cmplx,
					const		bool					print_csv,
					std::vector<std::string>::iterator itStart, 
					std::vector<std::string>::iterator itEnd,
					const		bool					useListA,
					const		bool					clearCaseFile,
					SourceFileList			*			mySrcFileList,
					SourceFileList			*			mySourceFileA,
					SourceFileList			*			mySourceFileB,
					string						&		errList,
					string								outDir,
					ErrMsgStructVector			&		err_msgs,
					UncountedFileStructVector	&		unc_files,
					const		bool					process_after_read,
					const		bool					discard_PHY_lines_after_process,
					const		bool					discard_lines_after_process,
					const		bool					no_warnings_to_UI,
					const		bool					no_uncounted );

// Analyze and do Counts for a single Source File
void ProcessSourceListFile( const unsigned int				threadIdx, 
							UserIF						*	userIF, 
							CounterForEachLangType		&	CounterForEachLanguage,
							CCodeCounter				**	ppCounter,
							SourceFileList::iterator		i,
							SourceFileList				*	mySourceFileA,
							SourceFileList				*	mySourceFileB,
							unsigned int				&	num_inserted,
							ErrMsgStructVector			&	err_msgs,
							UncountedFileStructVector	&	unc_files,
							string						&	errList,
							const string				&	parse_file_name,
							const bool						print_csv,
							const bool						print_cmplx,
							const bool						useListA );

#ifdef	ENABLE_THREADS
	// Divide a List of Files among multiple Threads and start them working
	int ReadFilesThreads( const unsigned long		num_files,	// number of Files in the list
						const		bool			noRead,		// true to NOT read in file data
						const		bool			noWeb,		// true to NOT read in Web files
						UserIF					*	userIF,
						const		bool			print_cmplx,
						const		bool			print_csv,
						StringVector			&	inFileVector,	// File list to divide
						const		bool			useListA,
						const		bool			clearCaseFile,
						string						outDir,
						const		bool			process_after_read,
						const		bool			discard_PHY_lines_after_process,
						const		bool			discard_lines_after_process,
						const		bool			no_warnings_to_UI,
						const		bool			no_uncounted );
#endif

/*!
*			ThreadAddFileToRead		COMMENTED OUT
* Request a thread to read in the Physical lines of a given named file.
* This will return usually before the thread finishes reading.  It is NOT synchronous.
*
* This is not efficient for doing many files but there may be a case later
* on other tasks in UCC not currently multithreaded that could use this.
*
*
* Call with an empty file name means the caller will wait until 
* ALL file reading THREADS HAVE FINISHED reading all requested files.
*
* \param fileName			IN		Name of a file to request reading.  If empty name, this waits as above.
* \param trim_line			IN		True to trim whitespace from a line else false
* \param pSrcFileElement	IN/OUT	Pointer to a SourceFileElement structure to change
* \param error_count		IN/OUT	Reference to int to tell about prior errors
*
* \returns	number of Requests that are pending (includes Requests already assigned to threads)
*			The actual number of pending requests MAY be higher than the return value
*/
/*
	unsigned int ThreadAddFileToRead( const string fileName, const bool trim_line, 
									SourceFileElement * pSrcFileElement,
									unsigned int & error_count );
*/

#endif	//	END		#ifndef	UCC_THREAD_PUBLIC_H

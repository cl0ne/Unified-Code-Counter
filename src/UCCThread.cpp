//! UCC multiple work Threads support implementation
/*!
* \file UCCThread.cpp
*
* This file encapsulates thread implementation details and dependencies
* so that the rest of UCC is relatively unchanged.
*
* Added to UCC 2015_12 release by Randy Maxwell
*   Changes started on 2015_10_02
*   Changes  ended  on 2015_11_11
*   Addition of Multithreading performance enhancement 
*
*		Example: UCC -threads 4 (and other UCC args)
*
* The cross platform thread feature and some related interfaces are from Boost C++ library (Thread)
* See the file BOOST_LICENSE_1_0.txt in this directory or at
* http://www.boost.org/LICENSE_1_0.txt
*
* Only the information and prototypes in UCCThread.h are Public.
* Treat the contents of this file as PRIVATE and do not casually move to other files.
* All items with prv_ prefix should stay as PRIVATE unless you have a very good reason.
*
*     DO NOT  include UCCGlobals.h here!   Even indirectly!
* Thread code directly using Globals really hurts performance (False Sharing)!
* Some helpers called only by the Main thread use Globals (which is OK).
*
* See comments at the end of this file for some Design and Performance details.
* See UCC_Multithreading_Notes.doc for older Design and Performance details.
*/

// Get Public Thread interfaces.  This MUST be first for this file.
#include "UCCThread.h"

#include	"UCCBeforeLibraryIncludes.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <ctime>
#include <string.h>		// 2016 Fix early setup compile Error so later Link Error with Boost is easy to see

using namespace std;

#ifdef	WIN32
	#define		ENABLE_EXCEPTION_CLAUSES
	#if (_MSC_VER >= 1600)		// Visual Studio 2010 or higher (or equivalent)
		#define		CLAUSE_SYSTEM_ERROR_OK
	#endif
#endif

#ifdef DARWIN		// MacIntosh OSX
	#define		ENABLE_EXCEPTION_CLAUSES
#endif

#ifdef	ENABLE_THREADS			// See UCCThread.h
#ifdef  QTGUI
    #include    <QObject>
	#include	<QThread>
	#include	<QMutex>
    //#include	<QSemaphore>

    #include    "../gui/UCCWorker.h"

	// Helper class derived from QThread
    WorkerThread * g_WorkerThreads[ MAX_UCC_THREAD_COUNT ];

	// Helper class derived from QObject to interface with WorkerThread
    UCCWorker    * g_UCC_Workers[ MAX_UCC_THREAD_COUNT ];

#else

	#include <boost/thread/thread.hpp>
	#include <boost/thread/mutex.hpp>

	// Enable this to try out Boost cross platform Memory Mapped files
	// Initial testing showed that Memory Mapped files were SLOWER
	// Of course this is just one way to implement.
	// Chosen to maintain same interface APIs as existing code before.
	// Likely there is a better way that would also require some
	// changes to code use of interface APIs.
	// Also note that currently Boost and Qt do NOT get used together,
	// Either Boost or Qt but not both.
	//
	// This is left available if you want to experiment...
	// #define		USE_MEMORY_MAPPED_FILES

	#ifdef		USE_MEMORY_MAPPED_FILES

		//#include <boost/interprocess/file_mapping.hpp>
		//#include <boost/interprocess/mapped_region.hpp>
		#include <boost/iostreams/stream.hpp>
		#include <boost/iostreams/device/mapped_file.hpp>

		// Tried using boost/interprocess for Semaphore but had side effect 
		// on Windows OS where expected blocking of work thread 
		// still had polling in the Interprocess library code going on.
		// After reading the Boost code the polling behavior is clear (and unwanted in this case).
		//
		// TODO: Provide feedback to boost Threads and Interprocess library maintainers:
		// 1) It would be nice if there was a constructor arg or define 
		// to allow use of native semaphores (especially on Windows)
		//
		// 2) IF implementing 1) is problematic perhaps some approach like that used by
		// the cross platform Semaphore library
		// 
		//
		// #include <boost/interprocess/sync/interprocess_semaphore.hpp>

		// using namespace boost::interprocess;
		using namespace boost::iostreams;
	#endif	// #ifdef		USE_MEMORY_MAPPED_FILES

	using namespace boost;
	using namespace boost::this_thread;
#endif

// Get cross platform Semaphore support for Boost and Qt thread control
	#include "sema.h"
#endif

#include	"UCCAfterLibraryIncludes.h"

#include "CUtil.h"
#include "LangUtils.h"
#include "UCCFilesOut.h"
#include "DiffTool.h"
#include "CmpMngr.h"

extern DiffTool * pDiffTool;

// This is declared in UCCGlobals.h  BUT do NOT include UCCGlobals.h here!  So declared here as well.		
extern	string	main_parse_path_file;		// 2016

// Number of installed CPU cores
unsigned int	num_HW_cores = 1;

//
//		LOCAL		Prototypes
//

//	Do a list of Matched Pairs to Diff (run from Main or worker thread)
int DiffPairsInList( const unsigned int					threadIdx,
					const		double					match_threshold,
					UserIF					*			userIF,
					bool						&		printDup,
					CounterForEachLangType		&		CounterForEachLanguage,
					const		bool					print_cmplx,
					const		bool					print_csv,
					DiffTool::MatchingType::iterator	myI,		// matchedFilesList, matching file pairs and differencing results 
					DiffTool::MatchingType::iterator	itEnd,
					const		bool					clearCaseFile,
					SourceFileList			*			mySourceFileA,
					SourceFileList			*			mySourceFileB,
					string						&		myErrList,
					string								outDir,
					ErrMsgStructVector			&		err_msgs,
					UncountedFileStructVector	&		unc_files,
					const		bool					discard_PHY_lines_after_process,
					const		bool					discard_lines_after_process,
					const		bool					no_warnings_to_UI,
					unsigned int				&		dup_addedLines,
					unsigned int				&		dup_deletedLines,
					unsigned int				&		dup_modifiedLines,
					unsigned int				&		dup_unmodifiedLines,
					unsigned int				&		total_addedLines,
					unsigned int				&		total_deletedLines,
					unsigned int				&		total_modifiedLines,
					unsigned int				&		total_unmodifiedLines );

// Check a list of Languages for Duplicate files.
// Each Language has 2 or more files.
int DupCheckLanguages( unsigned int					threadIdx, 
						vector<struct langTypeArrayHeader> * langs,
						ErrMsgStructVector		*	err_msgs,
						UncountedFileStructVector * unc_files,
						string					*	myErrList,
						const double				dup_threshold, 
						StringVector			*	dupList1, 
						StringVector			*	dupList2, 
						const bool					checkMatch, 
						const bool					useListA,
						UserIF					*	userIF,
						const bool					clearCaseFile,
						CounterForEachLangType	&	CounterForEachLanguage,
						const bool					print_cmplx,
						const bool					print_csv,
						SourceFileList			*	mySourceFileA,
						SourceFileList			*	mySourceFileB );

// Check for Duplicates of a given file
bool FindDuplicateFor( CmpMngr					*	pDiffManager,
						struct filePtrFlags		*	pPtrFlags, 
						const unsigned int			numPtrs, 
						const unsigned int			i_idx,
						ErrMsgStructVector		*	err_msgs,
						UncountedFileStructVector * unc_files,
						string					*	myErrList,
						const double				dup_threshold, 
						StringVector			*	dupList1, 
						StringVector			*	dupList2, 
						const bool					checkMatch, 
						const bool					useListA,
						UserIF					*	userIF,
						const bool					clearCaseFile,
						CounterForEachLangType	&	CounterForEachLanguage,
						const bool					print_cmplx,
						const bool					print_csv,
						SourceFileList			*	mySourceFileA,
						SourceFileList			*	mySourceFileB );

// Compare source lines between 2 files
void CompareForDuplicate(srcLineVector * firstFile, srcLineVector * secondFile,
						unsigned int &changedLines, unsigned int &totalLines,
						CmpMngr * pDiffManager );


// USE_LESS_MEM_ON_COPY  Enabled as default.
// Disable this IF you are sure UCC threads have enough Memory 
// to do somewhat Faster but use MUCH more temporary Memory while copying.
//
// Tradeoffs
// Enabled, several MB to GB or more of temporary memory is not needed (depends on Data size).
// Disabled, you reduce runtime by a some seconds (depends on Data size) at a greater risk of running out of memory.
//
// Disabled USE_LESS_MEM_ON_COPY Example: Single thread needs 4 GB to build the file list.
// Using 2 threads:  Each thread needs 2 GB.  When ready to copy back result for first thread,
// there would be a need for an extra 2 GB or 6 GB total at one time for success to advance to next step.
#define		USE_LESS_MEM_ON_COPY


////////////////////////////////////////////////////////////////////////
//
//		P R I V A T E		D A T A		a n d		P R O C E D U R E S
//
///////////////////////////////////////////////////////////////////////

// Keep what is defined here from polluting other namespaces
// This is commented out for now as using VC++ debugger is harder with this enabled
//namespace _prv_UCC_THREAD_NAMESPACE_
//{


// For worker threads this Saves the Error info
// Main thread this handles Error without saving as struct
// void AddError(const string &err, bool logOnly = false, int preNL = 0, int postNL = 1);
void _prv_SaveOrAddError( const unsigned int threadIdx, UserIF * userIF,
						ErrMsgStructVector & my_err_msgs,
						const string & err, const bool logOnly = false, 
						const bool no_warnings_to_UI = false,
						const unsigned int preNL = 0, const unsigned int postNL = 1 )
{
	bool	myLogOnly = logOnly;
	if ( false == myLogOnly )
	{
		if ( true == no_warnings_to_UI )	
		{
			if ( err.find( "Warning:" ) != string::npos )
			{
				// Found a Warning.  Are there also Error messages ?
				if ( ( err.find( "ERROR:" ) == string::npos )
				  && ( err.find( "Error:" ) == string::npos ) )
				{
					// No.  So set logging only
					myLogOnly = true;
				}
				else
				{
					// At least 1 Error message, make sure it shows on the console
					myLogOnly = false;
				}
			}
		}
	}
	if ( threadIdx < MAX_UCC_THREAD_COUNT )
	{
		// Save in a struct for later output
		errMsgStruct		err_msg;
		err_msg.threadIdx = threadIdx;
		err_msg.err       = err;
		err_msg.logOnly   = myLogOnly;
		err_msg.preNL     = preNL;
		err_msg.postNL    = postNL;
		my_err_msgs.push_back( err_msg );
	}
	else
	{
		userIF->AddError( err, myLogOnly, (int)preNL, (int)postNL );
	}
}


#ifdef		ENABLE_THREADS

	/*!
	* \enum ThreadState
	*
	* Enumeration of the states of a single worker thread.
	* Only a worker thread may WRITE these states.
	*/
	enum ThreadState {
		THREAD_WAITING_FOR_WORK = 1,	// Initial state and also after completing some work
		THREAD_WORKING,					// Actively doing some wanted work
		THREAD_HAS_EXITED,				// Normal Exit.  LAST value written just before a thread exits
		THREAD_RESTART_NEEDED,			// LAST value written just before a thread exits
	};

	/*!
	* \enum ThreadActionsWanted
	*
	* Enumeration of the actions of a worker thread wanted by the main thread.
	* Only the main thread may WRITE these actions that are wanted.
	*/
	enum ThreadActionsWanted {
		NO_WORK_YET = 10,		// No work for the thread yet
		GET_TO_WORK,			// Worker thread has enough data to do some work
		STOP_WORKING,			// Stop working even if not finished with work (User Cancelled?)
		EXIT_NOW,				// Ask the worker thread to exit
	};

	//! Define a Structure to encapsulate the status of a worker thread
	/*!
	* \struct _prv_threadStatus
	*
	* This also allows very controlled access by both the main thread and the owning worker thread
	*/
	struct _prv_threadStatus
	{
		// There is 1 access rule: You can not change what I WRITE (you just have READ ONLY access).
		// A value has only 1 WRITER associated with it (either main thread or worker thread side).
		// Ownership (who can WRITE) is part of the entity name (as a prefix) main_ or work_ (or thread_)
//#ifdef  QTGUI
//        QSemaphore                  *       pQSem;
//#else
		Semaphore					*		pSem;				//!< Semaphore used to prevent polling
//#endif
		CounterForEachLangType		*		pCounterForEachLanguage;
		UserIF						*		userIF;

		volatile	ThreadState				work_ThreadState;	//!< State of this worker thread
		volatile	ThreadActionsWanted		main_WantsAction;	//!< Action that main thread wants
					unsigned int			thread_idx;			//!< Index in array for this worker
//					boost::thread::id		threadId;			//!< thread ID found after thread created

//			PUT Data elements HERE that are available and somewhat current if LOW Memory condition hits.
		vector<struct langTypeArrayHeader>	langs;
//
	// Info about specific work the thread is doing
	// These change FREQUENTLY (each time the thread gives UI new information)
		volatile	unsigned int			thread_work_count_done;

// These are NOT accessed when doing UI updates but should be available if LOW Memory
		// Running totals while a thread is doing DIFF between pairs of files
					unsigned int			total_addedLines;		//!< Total lines added
					unsigned int			total_deletedLines;		//!< Total lines deleted
					unsigned int			total_modifiedLines;	//!< Total lines modified
					unsigned int			total_unmodifiedLines;	//!< Total lines unmodified
					unsigned int			dup_addedLines;			//!< Total duplicate lines added
					unsigned int			dup_deletedLines;		//!< Total duplicate lines deleted
					unsigned int			dup_modifiedLines;		//!< Total duplicate lines modified
					unsigned int			dup_unmodifiedLines;	//!< Total duplicate lines unmodified

					bool					printDup;				//!< Print duplicates (only true if unmatched duplicates exist)

		// Cache for Error messages, Uncounted File info, etc. so multiple threads don't call to UI
		ErrMsgStructVector					thread_err_msgs;	//!< OUT Used if needed for any from thread_idx >= 0
		UncountedFileStructVector			thread_unc_files;	//!< OUT Used if needed for any from thread_idx >= 0

		// Info flag about a thread having started
		volatile	bool					thread_has_started;

		// These change only when the thread has finished work.
		volatile	bool					thread_work_done;	//!< true when just completed some work
		volatile	bool					thread_had_error;	//!< true if there was an error

		// Possible Optimizations to use
					bool					process_after_read;	//!< After Reading a File into memory, process it
					bool					discard_PHY_lines_after_process;	//!< After Processing, del PSLOC only
					bool					discard_lines_after_process;		//!< After Processing, del PSLOC and LSLOC

		// User controls
					bool					no_warnings_to_UI;
					bool					no_uncounted;

		// Program Logic controls
					bool					is_File_Work;		// true to do a single or List of Files, false to DIFF

		// WORK: Read a List of Files (variable names same as rest of code)
		std::vector<std::string>::iterator	itStart;
		std::vector<std::string>::iterator	itEnd;
					bool					noRead;
					bool					noWeb;
					bool					print_cmplx;
					bool					print_csv;
					bool					useListA;
					bool					clearCaseFile;
					SourceFileList	*		pThreadSrcFileList;	// pointer to SourceFileList on a thread's local stack
				//	SourceFileList			SourceFileA;		//!< List of source files in baseline A
				//	SourceFileList			SourceFileB;		//!< List of source files in baseline B
					string					outDir;
					string					errList;			// Used by Main thread only

		// WORK: DIFF a List of Pairs of Files
		DiffTool::MatchingType::iterator	itDStart;
		DiffTool::MatchingType::iterator	itDEnd;
					double					match_threshold;

		// WORK: Duplicate checking a list of files of a Language
					double					dup_threshold;
					StringVector	*		dupList1;
					StringVector	*		dupList2;
					bool					check_match;

		// WORK: Read Physical lines of a single file
					bool					main_trim_leading;		//!< true is default, false for Fortran and Python
                    // 2016 Current file being processed		
					string					current_path_file;
	};

	void	_prv_Init_threadStatus( _prv_threadStatus & rThStatus )
	{
//#ifdef  QTGUI
//        rThStatus.pQSem = NULL;
//#else
		rThStatus.pSem = NULL;						//!< Semaphore will be created later with new
//#endif
		rThStatus.pCounterForEachLanguage = NULL;

		// No good way to initialize threadId here


		// Initialize values so when a worker thread takes ownership
		// the worker thread is in a predictable state.
		rThStatus.work_ThreadState	= THREAD_WAITING_FOR_WORK;
		rThStatus.main_WantsAction	= NO_WORK_YET;
		rThStatus.thread_idx		= MAX_UCC_THREAD_COUNT + 1;		//!< Start with invalid index

		// This is checked periodically for ongoing thread status to show counts or % done
		rThStatus.thread_work_count_done = 0;

		// running counts when doing DIFF
		rThStatus.total_addedLines   = 0;		//!< Total lines added
		rThStatus.total_deletedLines = 0;		//!< Total lines deleted
		rThStatus.total_modifiedLines = 0;		//!< Total lines modified
		rThStatus.total_unmodifiedLines = 0;	//!< Total lines unmodified
		rThStatus.dup_addedLines = 0;			//!< Total duplicate lines added
		rThStatus.dup_deletedLines = 0;			//!< Total duplicate lines deleted
		rThStatus.dup_modifiedLines = 0;		//!< Total duplicate lines modified
		rThStatus.dup_unmodifiedLines = 0;		//!< Total duplicate lines unmodified

		rThStatus.printDup = false;

		rThStatus.thread_has_started = false;

		rThStatus.thread_work_done = false;
		rThStatus.thread_had_error = false;

		// Possible Optimizations to use
		rThStatus.process_after_read              = false;
		rThStatus.discard_PHY_lines_after_process = false;
		rThStatus.discard_lines_after_process     = false;

		// User controls
		rThStatus.no_warnings_to_UI = false;
		rThStatus.no_uncounted      = false;

		// Program Logic controls
		rThStatus.is_File_Work     = false;				// true to do a single or List of Files, false to DIFF

		rThStatus.userIF           = NULL;
		// No good way to initialize itStart or itEnd here
		rThStatus.noRead           = false;
		rThStatus.noWeb            = false;
		rThStatus.print_cmplx      = false;
		rThStatus.print_csv        = false;
		rThStatus.useListA         = true;
		rThStatus.clearCaseFile    = false;
		rThStatus.pThreadSrcFileList = NULL;
	//	rThStatus.SourceFileA.resize( 0 );
	//	rThStatus.SourceFileB.resize( 0 );
		rThStatus.outDir           = "";
		rThStatus.thread_err_msgs.resize( 0 );
		rThStatus.thread_unc_files.resize( 0 );

		// Duplicate checking a list of files of a Language
		rThStatus.dup_threshold = 0;
		rThStatus.dupList1 = NULL;
		rThStatus.dupList2 = NULL;
		rThStatus.check_match = false;

		rThStatus.main_trim_leading = true;
        rThStatus.current_path_file = "";		// 2016
	}


////////////////////////////////////////////////////////////////////////////////
//
// Declare some Global values (inside this .cpp file) that have a lifetime
// Longer than any given public procedure.
//
	// Number of threads assigned to work
	unsigned int	g_assigned_count = 0;

    // Used to prevent (hopefully very rare) race conditions between these threads.
#ifdef  QTGUI
	QMutex		g_exclusive_Mutex;
#else
	boost::mutex g_exclusive_Mutex;
#endif

// Create empty arrays of needed long lived values
	_prv_threadStatus	g_threads_status[ MAX_UCC_THREAD_COUNT ];
	unsigned int		g_threads_status_in_use = 0;

	CounterForEachLangType * g_MainThread_CounterForEachLanguage = NULL;

	UserIF		*		g_userIF = NULL;

	// May not be needed ?
	string				g_threads_file_names[ MAX_UCC_THREAD_COUNT ];

    SourceFileElement * g_threads_pSrcFileElement[ MAX_UCC_THREAD_COUNT ];

	// Thread ID of main thread that calls into public procedures
//	boost::thread::id 	g_main_thread_id;

//================================================================================
//
//			Thread		Functions
//
//================================================================================
void prv_WorkThread_Function( const unsigned int thr_array_idx );	// Prototype

void prv_WorkThread_Function( const unsigned int thr_array_idx )
{
	if ( thr_array_idx >= MAX_UCC_THREAD_COUNT )
	{
		// SEVERE ERROR of thread interface
		cout << endl << "Error: prv_WorkThread_Function called with invalid thr_array_idx" << endl;
		//throw std::runtime_error("Error: prv_WorkThread_Function called with invalid thr_array_idx");
		return;
	}

//		Very	Important:
//
// Minimize use of outside variables from inside a thread.
// NO use of Globals from thread code (only OK for main thread).  Maybe before any loops start.
// This helps prevent "False Sharing" which can seriously slow performance
// and defeat the whole purpose of using threads.
//
// So create variables here on the Thread's stack where possible

	// Exceptions within this thread
	unsigned int	thread_exception_type = EXCEPTION_DID_NOT_HAPPEN;
	string	thread_exception_msg;

	// Make a pointer to a thread status struct 
	// so when values in the struct change later this code will see them.
	_prv_threadStatus * pStatus = &(g_threads_status[ thr_array_idx ]);

	bool	need_to_wait		= true;
	bool	have_done_work		= false;
	bool	done_with_thread	= false;

	// Variables for Reading Physical lines
	string			srcFileName;
	string			oneline;
	// lineElement	element();	// do NOT use by itself, causes MEMORY LEAKS see below for better way

	// Declare these here so can be sent along even if in LOW Memory condition
	bool			printDup = false;
	unsigned int	dup_addedLines;
	unsigned int	dup_deletedLines;
	unsigned int	dup_modifiedLines;
	unsigned int	dup_unmodifiedLines;
	unsigned int	total_addedLines;
	unsigned int	total_deletedLines;
	unsigned int	total_modifiedLines;
	unsigned int	total_unmodifiedLines;

// Declare here so can be Released later
	CounterForEachLangType		myCounterForEachLanguage;
	SourceFileList				mySourceFileA;
	SourceFileList				mySourceFileB;
	ErrMsgStructVector			err_msgs;
	UncountedFileStructVector	unc_files;

// Protect main thread and other threads from any exceptions
try 
{
	SourceFileList			*	mySrcFileList = &mySourceFileA;
//	SourceFileList			*	pDestFileList = NULL;


/*		CRITICAL NOTE for THREAD SAFETY:
While debugging on Win 7.1 using Visual Studio C++ 2010 Express 
(Expect this is common with other Windows OS and VS versions, perhaps other OS/compilers as well.)

Looking at the code in Init_CounterForEachLanguage there is no explicit 
(or implicit that I can tell) use of Global variables = 
which is where a lot of threaded code has problems.

The thread initialization code (2 threads) deadlocked trying to acquire a Mutex.  ? ! ?
Implemented several layers down from the new operator in Microsoft code.
Debugger shows where one Thread blocked on a new for HTML class 
and another Thread blocked at at a different new for a different Language class.

Remember that all of the Language classes inherit from the CCodeCounter class.
So that even if the new is being done for apparently 2 different classes; underneath
there will also be new calls to create an instance of CCodeCounter class for each.

So the underlying Microsoft code detected that Init_CounterForEachLanguage was being
called by one thread when the OS thread scheduler switched to the other thread 
thereby allowing the race condition to the underlying Microsoft code Mutex in question. 

By extension, any time the same classes are created dynamically by 2 or more threads
	-- EVEN IF the memory (stack of each thread) is different 
		from where the Class instances will be created --
there is a potential for extra excitement.

TO help reduce the excitement, try to minimize the creation of classes at runtime. 
For example a LOT of std::string classes are return values in UCC code.
	Each time a temporary string is created and then destroyed.
	Consider adding an arg that is a reference to an already existing string 
	for the called function to change as a way to return instead.
	Semantically the 2 approaches are nearly identical but using a reference 
	(or Pointer) to an existing instance of a class 
	is more efficient of both dynamic memory and CPU resources.
This also helps performance.

Careful reading of the code that is run by threads here will show the above approach.
Of couse, I may have missed a few spots...
*/

// Acquire a Mutex to prevent race condition that may deadlock on underlying C++ library Mutex
	g_exclusive_Mutex.lock();

	Init_CounterForEachLanguage( myCounterForEachLanguage );

	ResetCounterCounts( myCounterForEachLanguage );

	SetCounterOptions( myCounterForEachLanguage );

	pStatus->pCounterForEachLanguage = &( myCounterForEachLanguage );

	g_exclusive_Mutex.unlock();

	pStatus->thread_has_started = true;

	// Should have just been created with no work to do
	do
	{
		need_to_wait = false;

		if ( EXIT_NOW == pStatus->main_WantsAction )
		{
			done_with_thread = true;
			break;
		}

		if ( ( NO_WORK_YET  == pStatus->main_WantsAction )
		  || ( STOP_WORKING == pStatus->main_WantsAction ) )
		{
			pStatus->work_ThreadState = THREAD_WAITING_FOR_WORK;
			if ( STOP_WORKING == pStatus->main_WantsAction )
				have_done_work = false;
			
			need_to_wait = true;
		}

		if ( ( GET_TO_WORK == pStatus->main_WantsAction )
		  && ( ( 0 == srcFileName.size() ) && ( NULL == pStatus->userIF ) ) )
		{
			need_to_wait = true;
		}

		if ( true == need_to_wait )
		{
			// Wait on a semaphore until there is some change to wanted action
//#ifdef  QTGUI
//            pStatus->pQSem->acquire();
//#else
			pStatus->pSem->wait();
//#endif
			continue;
		}
        		
		pStatus->current_path_file = "";		// 2016
        
		srcFileName = g_threads_file_names[ thr_array_idx ];

		if ( ( GET_TO_WORK == pStatus->main_WantsAction )
			&& ( pStatus->langs.size() ) )
		{
			// Dup Checking time !
			pStatus->work_ThreadState = THREAD_WORKING;
			pStatus->thread_work_count_done = 0;
			pStatus->thread_work_done = false;
			pStatus->thread_had_error = false;

			// Set up local variables that will get used during Dup Checking
			mySourceFileA.resize( 0 );
			mySourceFileB.resize( 0 );
			pStatus->thread_err_msgs.resize( 0 );
			pStatus->thread_unc_files.resize( 0 );
			pStatus->errList.resize( 0 );

			int error_count =
			DupCheckLanguages( thr_array_idx, 
					&(pStatus->langs),
					&(pStatus->thread_err_msgs),
					&(pStatus->thread_unc_files),
					&(pStatus->errList),
					pStatus->dup_threshold, 
					pStatus->dupList1, 
					pStatus->dupList2, 
					pStatus->check_match, 
					pStatus->useListA,
					pStatus->userIF,
					pStatus->clearCaseFile,
					myCounterForEachLanguage,
					pStatus->print_cmplx,
					pStatus->print_csv,
					&mySourceFileA,
					&mySourceFileB );

			if ( ( thr_array_idx ) 
				&& ( error_count != (int)err_msgs.size() ) )
			{
				// Different number of Error counts got returned here than got Saved... strange
			#ifdef	_DEBUG
			//	printf( "\nDupCheckLanguages returned %d errors while err_msgs.size() has %d entries.\n",
			//			error_count, (int)err_msgs.size() );
			#endif
			}

			pStatus->thread_work_done = true;
			have_done_work = true;
			g_threads_file_names[ thr_array_idx ] = "";
			pStatus->userIF = NULL;

			//printf("\n    Dup check thread finished\n");

			pStatus->work_ThreadState = THREAD_WAITING_FOR_WORK;
		}
		else
		if ( ( GET_TO_WORK == pStatus->main_WantsAction )
		  && ( false == pStatus->is_File_Work ) )
		{
			// DIFF time !
			pStatus->work_ThreadState = THREAD_WORKING;
			pStatus->thread_work_count_done = 0;
			pStatus->thread_work_done = false;
			pStatus->thread_had_error = false;

			// Set up local variables that will get used during DIFF
			dup_addedLines = 0;
			dup_deletedLines = 0;
			dup_modifiedLines = 0;;
			dup_unmodifiedLines = 0;;
			total_addedLines = 0;
			total_deletedLines = 0;
			total_modifiedLines = 0;
			total_unmodifiedLines = 0;

			bool discard_lines_after_process = true;
			extern	bool isDup;
			if ( isDup )
				discard_lines_after_process = false;

			int error_count = DiffPairsInList( thr_array_idx,
										pStatus->match_threshold,
										pStatus->userIF,
										printDup,
										myCounterForEachLanguage,
										pStatus->print_cmplx,
										pStatus->print_csv,
										pStatus->itDStart,		// matchedFilesList, matching file pairs and differencing results 
										pStatus->itDEnd,
										pStatus->clearCaseFile,
										&mySourceFileA,
										&mySourceFileB,
										pStatus->errList,
										pStatus->outDir,
										err_msgs,
										unc_files,
										true,					// pStatus->discard_PHY_lines_after_process,
										discard_lines_after_process,
										pStatus->no_warnings_to_UI,
										dup_addedLines,
										dup_deletedLines,
										dup_modifiedLines,
										dup_unmodifiedLines,
										total_addedLines,
										total_deletedLines,
										total_modifiedLines,
										total_unmodifiedLines );

			// printf("\nReturn from DiffPairsInList\n");

			if ( ( thr_array_idx ) 
				&& ( error_count != (int)err_msgs.size() ) )
			{
				// Different number of Error counts got returned here than got Saved... strange
			#ifdef	_DEBUG
			//	printf( "\nDiffPairsInList returned %d errors while err_msgs.size() has %d entries.\n",
			//			error_count, (int)err_msgs.size() );
			#endif
			}

			if ( true == printDup )
				pStatus->printDup = true;

			pStatus->total_addedLines      += total_addedLines;
			pStatus->total_deletedLines    += total_deletedLines;
			pStatus->total_modifiedLines   += total_modifiedLines;
			pStatus->total_unmodifiedLines += total_unmodifiedLines;
			pStatus->dup_addedLines        += dup_addedLines;
			pStatus->dup_deletedLines      += dup_deletedLines;
			pStatus->dup_modifiedLines     += dup_modifiedLines;
			pStatus->dup_unmodifiedLines   += dup_unmodifiedLines;

			// These are typically MUCH smaller than the File List structs, so use Faster approach.
			// Change this if you want to be even more conservative of temporary Memory use.

			//printf("\n   err_msgs size is: %ld", (long)err_msgs.size() );
			pStatus->thread_err_msgs = err_msgs;
			err_msgs.resize( 0 );

			//printf("\n   unc_files size is: %ld", (long)unc_files.size() );
			pStatus->thread_unc_files = unc_files;
			unc_files.resize( 0 );

			pStatus->thread_work_done = true;
			have_done_work = true;
			g_threads_file_names[ thr_array_idx ] = "";
			pStatus->userIF = NULL;

			//printf("\n    Diff thread finished\n");

			pStatus->work_ThreadState = THREAD_WAITING_FOR_WORK;
		}
		else
		if ( ( GET_TO_WORK == pStatus->main_WantsAction )
		  && ( ( srcFileName.size() ) || ( NULL != pStatus->userIF ) ) )
		{
			// WORK ... hooray !
			//
			// Keep going here until work is done or 
			// STOP_WORKING or EXIT_NOW is wanted  TODO: Add checks for these 2

			pStatus->work_ThreadState = THREAD_WORKING;
			pStatus->thread_work_count_done = 0;
			pStatus->thread_work_done = false;
			pStatus->thread_had_error = false;
			
			SourceFileElement * pSrcFile = g_threads_pSrcFileElement[ thr_array_idx ];
			srcFileName = g_threads_file_names[ thr_array_idx ];

			while ( false == pStatus->thread_work_done )
			{
				if ( NULL != pStatus->userIF )
				{
					// Call helper to Read a List of Files
					// Optimizations to do other processing after Reading are likely

					// Must use internal Lists (on this thread's stack!)
					mySrcFileList = (pStatus->useListA) ? &(mySourceFileA): &(mySourceFileB);
					mySrcFileList->resize( 0 );
					err_msgs.resize( 0 );
					unc_files.resize( 0 );
					pStatus->errList.resize( 0 );

					// Set up to Copy back results
					//pDestFileList = (pStatus->useListA) ? &(pStatus->SourceFileA): &(pStatus->SourceFileB);
					//pDestFileList->resize( 0 );		// Empty destination in struct

					int error_count = ReadFilesInList( thr_array_idx, 
													pStatus->noRead,
													pStatus->noWeb,
													pStatus->userIF, 
													myCounterForEachLanguage, 
													pStatus->print_cmplx, pStatus->print_csv,
													pStatus->itStart, pStatus->itEnd, pStatus->useListA, 
													pStatus->clearCaseFile, 
													mySrcFileList,		// Destination of Files read into memory
													&mySourceFileA,		// Destination if using A
													&mySourceFileB,		// Destination if using B
													pStatus->errList,
													pStatus->outDir,
													err_msgs, unc_files,
													pStatus->process_after_read,
													pStatus->discard_PHY_lines_after_process,
													pStatus->discard_lines_after_process,
													pStatus->no_warnings_to_UI,
													pStatus->no_uncounted );

					if ( ( thr_array_idx ) 
					  && ( error_count != (int)err_msgs.size() ) )
					{
						// Different number of Error counts got returned here than got Saved... strange
					#ifdef	_DEBUG
					//	printf( "\nReadFilesInList returned %d errors while err_msgs.size() has %d entries.\n",
					//			error_count, (int)err_msgs.size() );
					#endif
					}

					// Avoid extra copying of data around, just point to it
					pStatus->pThreadSrcFileList = mySrcFileList;

/**  Commented out as not needed but is an example of working code.  Better example in CombingThreadResults.
					// Copy back results to struct
					// 
				#ifdef	USE_LESS_MEM_ON_COPY
					// Minimize use of temporary memory.  
					// This can save several MB or GB or more depending on size of File list and Files
					// Which can make the difference between success with Threads or not.
					// So this is the default approach.
					unsigned int num_to_copy = mySrcFileList->size();
					for ( unsigned int j = 0; j < num_to_copy; j++ )
					{
						pDestFileList->push_back( *(mySrcFileList->begin()) );	// Append element to end of list
						mySrcFileList->pop_front();		// Get rid of element just copied
					}
				#else
					// Use Faster but temporarily uses much MORE memory approach
					pDestFileList->insert( pDestFileList->begin(), mySrcFileList->begin(), mySrcFileList->end() );
				#endif
					mySrcFileList->resize( 0 );	// Release memory in thread stack
**/
					// These are typically MUCH smaller than the File List structs, so use Faster approach.
					// Change this as above if you want to be even more conservative of temporary Memory use.
					pStatus->thread_err_msgs = err_msgs;
					err_msgs.resize( 0 );
					pStatus->thread_unc_files = unc_files;
					unc_files.resize( 0 );

					pStatus->thread_work_done = true;
					have_done_work = true;
					g_threads_file_names[ thr_array_idx ] = "";
					pStatus->userIF = NULL;
					pStatus->work_ThreadState = THREAD_WAITING_FOR_WORK;
					break;
				}

		/////////////////////////////////////////////////////////////////////////////

				// This section supports a thread processing a single file. 
				if ( srcFileName.size() )
				{
					// WORK: Read the Physical lines from a source file
					//
					// Open the file
					ifstream fr( srcFileName.c_str(), ios::in );
					if (!fr.is_open())
					{
						pStatus->thread_had_error = true;
						pSrcFile->second.e_flag = true;
						pSrcFile->second.error_code = "Unable to open file";
						pStatus->thread_work_done = true;
						have_done_work = true;
						g_threads_file_names[ thr_array_idx ] = "";
						pStatus->work_ThreadState = THREAD_WAITING_FOR_WORK;
						continue;
					}

					if ( ( EXIT_NOW     == pStatus->main_WantsAction )
					  || ( STOP_WORKING == pStatus->main_WantsAction ) )
					{
						pStatus->thread_work_done = true;
						have_done_work = false;		// to use longer sleep time
						g_threads_file_names[ thr_array_idx ] = "";
						fr.clear();
						fr.close();
						pStatus->work_ThreadState = THREAD_WAITING_FOR_WORK;
						continue;
					}

					// Read the Physical lines
					// File opened OK, read each Physical line into memory
					unsigned int lineNum = 0;
					bool lineTooLong = false;

					while (fr.good() || fr.eof())
					{
						getline(fr, oneline);
						if ((!fr.good() && !fr.eof()) || (fr.eof() && oneline.empty()))
							break;

						if (oneline.size() > MAX_LINE_LENGTH)
						{
							lineTooLong = true;
							break;
						}
						lineNum++;
						if ( pStatus->main_trim_leading )
							oneline = CUtil::TrimString( oneline );		// trim Leading and Trailing
						else
							oneline = CUtil::TrimString( oneline, 1 );	// trim Trailing only
						if ( oneline.size() )
							oneline = CUtil::ReplaceSmartQuotes( oneline );

						// Use lineElement inside the call to push_back to prevent memory leaks
						// This will show up for a Debug build but may not show for a Release build
						// as (I am guessing) the code Optimization will move the call inside as below.
						pSrcFile->first.push_back( lineElement( lineNum, oneline ) );

						if (!fr.good())
							break;

						// Check after each line if main thread wants worker to stop
						if ( ( EXIT_NOW     == pStatus->main_WantsAction )
						  || ( STOP_WORKING == pStatus->main_WantsAction ) )
						{
							pStatus->thread_work_done = true;
							have_done_work = false;		// to use longer sleep time
							g_threads_file_names[ thr_array_idx ] = "";
							// fr.clear(); done below
							// fr.close(); done below
							// rStatus.work_ThreadState = THREAD_WAITING_FOR_WORK;
							break;
						}
					}
					fr.clear();
					fr.close();

					if (lineTooLong)
					{
						pStatus->thread_had_error = true;
						pSrcFile->second.e_flag = true;
						pSrcFile->second.error_code = "Line too long";

						// Fall through
					}

					pStatus->thread_work_done = true;
					g_threads_file_names[ thr_array_idx ] = "";
					pStatus->work_ThreadState = THREAD_WAITING_FOR_WORK;
					break;
				}	//	END		Read Physical lines from source file
				else
				{
					// No name given
					pStatus->thread_work_done = true;
					pStatus->main_WantsAction = NO_WORK_YET;
					pStatus->work_ThreadState = THREAD_WAITING_FOR_WORK;
				}

				//
				// Check for another kind of work
				// 

			}
		}	//	END of WORK for thread
	
	}
	while ( false == done_with_thread );
}
	// Handle any Exceptions to get as much context info as possible
	// (in a cross platform way).
	// On Windows for example there is an another entire set 
	// (a LOT more than those listed below)
	// of Windows OS related Exceptions that may be thrown.
	// Perhaps the system_error handler below will cover them OK.
	//
	// Idea for this chain of Exception handlers was from cppreference.com docs.
	// More Exception types were found in <stdexcept> module.
	// If you find any that do not compile please just comment out those.
catch(const std::overflow_error& e) 
{
	// this executes if above throws std::overflow_error (same type rule)
	thread_exception_type = EXCEPTION_OVERFLOW_ERROR;
	thread_exception_msg = e.what();
}
catch(const std::underflow_error& e)
{
	// this executes if above throws std::underflow_error (base class rule)
	thread_exception_type = EXCEPTION_UNDERFLOW_ERROR;
	thread_exception_msg = e.what();
}
#ifdef ENABLE_EXCEPTION_CLAUSES
catch(const std::range_error& e)
{
	// this executes if above throws std::range_error
	thread_exception_type = EXCEPTION_RANGE_ERROR;
	thread_exception_msg = e.what();
}
#ifdef CLAUSE_SYSTEM_ERROR_OK
	catch(const std::system_error& e)
	{
		// this executes if above throws std::system_error
		thread_exception_type = EXCEPTION_SYSTEM_ERROR;
		thread_exception_msg = e.what();
	}
#endif
#endif	// #ifdef ENABLE_EXCEPTION_CLAUSES
catch(const std::runtime_error& e) 
{
	// this executes if above throws std::runtime_error (base class rule)
	thread_exception_type = EXCEPTION_RUNTIME_ERROR;
	thread_exception_msg = e.what();
} 
catch(const std::domain_error& e) 
{
	// this executes if above throws std::domain_error (base class rule)
	thread_exception_type = EXCEPTION_DOMAIN_ERROR;
	thread_exception_msg = e.what();
} 
catch(const std::length_error& e) 
{
	// this executes if above throws std::length_error (base class rule)
	thread_exception_type = EXCEPTION_LENGTH_ERROR;
	thread_exception_msg = e.what();
} 	
catch(const std::invalid_argument& e) 
{
	// this executes if above throws std::invalid_argument (base class rule)
	thread_exception_type = EXCEPTION_INVALID_ARG_ERROR;
	thread_exception_msg = e.what();
} 		
catch(const std::out_of_range& e) 
{
	// this executes if above throws std::out_of_range (base class rule)
	thread_exception_type = EXCEPTION_OUT_OF_RANGE_ERROR;
	thread_exception_msg = e.what();
} 
catch(const std::logic_error& e) 
{
	// this executes if above throws std::logic_error (base class rule)
	thread_exception_type = EXCEPTION_LOGIC_ERROR;
	thread_exception_msg = e.what();
} 
catch(const std::bad_alloc& e)
{										// LOW on RAM memory ! ! !
	// this executes if above throws std::bad_alloc (base class rule)
	thread_exception_type = EXCEPTION_BAD_ALLOC;
	thread_exception_msg = e.what();
	FirstExceptionMessage( EXCEPTION_HANDLER_THREAD, thr_array_idx, thread_exception_type, thread_exception_msg );
}
catch(const std::exception& e) 
{
	// this executes if above throws std::exception (base class rule)
	thread_exception_type = EXCEPTION_STD_EXCEPTION;
	thread_exception_msg = e.what();
	if ( thread_exception_msg.compare( "bad allocation" ) == 0 )
	{
		// LOW on RAM memory ! ! !
		thread_exception_type = EXCEPTION_BAD_ALLOC;
		FirstExceptionMessage( EXCEPTION_HANDLER_THREAD, thr_array_idx, thread_exception_type, thread_exception_msg );
	}
} 
catch(...) 
{
	// this executes if above throws std::string or int or any other unrelated type
	// Some kind of severe error that prevented normal finish
	thread_exception_type = EXCEPTION_NOT_SPECIFIC;
	thread_exception_msg = "";
}

bool	thread_restart_needed = false;
if ( EXCEPTION_DID_NOT_HAPPEN != thread_exception_type )
{
	// Recover from Exceptions in a thread.
	// Hopefully it is pretty unusual for UCC to have a Thread and NOT a Parser Exception.
	// So instead of restructuring the code, just fall through after this 
	// and let the Main thread restart this thread again TODO: ...
	thread_restart_needed = true;

	string	dump;
	StackDump( dump, EXCEPTION_HANDLER_THREAD, thr_array_idx, thread_exception_type, thread_exception_msg );
	_prv_SaveOrAddError( thr_array_idx, pStatus->userIF, pStatus->thread_err_msgs, dump, false );	// Show ERROR on console as well
	pStatus->errList += dump;

	g_exclusive_Mutex.unlock();		// OK to call even if not locked
}

//		Release any resources held by this Thread and clear pointers that live outside the thread
//
	// Release in reverse order of allocation
	if ( EXCEPTION_BAD_ALLOC != thread_exception_type )
	{
		// OK to copy as not LOW Memory
		if ( unc_files.size() )
			pStatus->thread_unc_files.assign( unc_files.begin(), unc_files.end() );

		if ( err_msgs.size() )
			pStatus->thread_err_msgs.assign( err_msgs.begin(), err_msgs.end() );
	}
	unc_files.resize( 0 );
	err_msgs.resize( 0 );

	mySourceFileB.resize( 0 );
	mySourceFileA.resize( 0 );

	pStatus->userIF = (UserIF *)NULL;
	
	pStatus->pCounterForEachLanguage = (CounterForEachLangType *)NULL;
	for (map<int, CCodeCounter*>::iterator iter = myCounterForEachLanguage.begin(); iter != myCounterForEachLanguage.end(); iter++)
	{
		delete iter->second;	// Free the parser class instances
		iter->second = 0;
	}
	myCounterForEachLanguage.clear();
    
    pStatus->current_path_file = "";		// 2016
    
	if ( thread_restart_needed )
		pStatus->work_ThreadState = THREAD_RESTART_NEEDED;
	else
		pStatus->work_ThreadState = THREAD_HAS_EXITED;

    //warning fix 11.25.16. DO NOT USE THIS VARIABLE AFTER THIS.
    (void) have_done_work;
	return;
}


// Increment work done counter to support UI updating
// This should be called as FEW times as POSSIBLE to minimize effects of "False sharing" here.
//
// Return the wanted Action from the main thread
int _prv_IncrementWorkDone( const unsigned int threadIdx, const unsigned int amount )
{
	int		retVal = 0;

	if ( threadIdx < MAX_UCC_THREAD_COUNT )
	{
		g_threads_status[ threadIdx ].thread_work_count_done += amount;
		
		// See if main thread has a different action wanted
		retVal = g_threads_status[ threadIdx ].main_WantsAction;
	}

	return retVal;
}


#endif		//	#ifdef		ENABLE_THREADS


// For worker threads this Saves the Uncounted File info
// Main thread this handles it without saving as struct
void _prv_SaveOrWriteUncountedFile( const unsigned int threadIdx, UserIF * userIF,
									UncountedFileStructVector & my_unc_files,
									string msg, string fileName, 
									const bool useListA, const bool print_csv,
									string outDir, const bool no_uncounted )
{
	if ( true == no_uncounted )
		return;

	if ( threadIdx < MAX_UCC_THREAD_COUNT )
	{
		// Save in a struct for later output
		uncountedFileStruct		unc_file;
		unc_file.threadIdx = threadIdx;
		unc_file.msg       = msg;
		unc_file.uncFile   = fileName;
		unc_file.useListA  = useListA;
		unc_file.csvOutput = print_csv;
		unc_file.outDir    = outDir;
		my_unc_files.push_back( unc_file );
	}
	else
	{
		string writeErr = WriteUncountedFile( msg, fileName, useListA, print_csv, outDir );
		if ( ! writeErr.empty() )
			userIF->AddError( writeErr );
	}
}

#ifdef	ENABLE_THREADS
// Divide a List of Files or a List of Pairs of files to Diff
// among multiple threads and start them working.
int 
_prv_ListToThreads( const		bool		isFileList,
					const	unsigned long	num_in_list,
					const		bool		noRead,
					const		bool		noWeb,
					UserIF				*	userIF,
					const		bool		print_cmplx,
					const		bool		print_csv,
					StringVector		*	inFileVector,	// File list to divide
					DiffTool			*	pDiff,			// DiffTool data members: Pairs list, diff & dup counters...
					const		bool		useListA,
					const		bool		clearCaseFile,
					string					outDir,
					const		bool		process_after_read,
					const		bool		discard_PHY_lines_after_process,
					const		bool		discard_lines_after_process,
					const		bool		no_warnings_to_UI,
					const		bool		no_uncounted )
{
#ifdef	_DEBUG
	bool	unexpected_state = false;
#endif
	int		error_count = 0;

	// Add a Request to Read a List of Files for each thread
	//				OR
	// Add a Request to DIFF a List of file Pairs for each thread
	// Each thread should be ready for work.
	//
	// It is OK to have fewer files than available threads.
	// If so, there may be threads left blocked and waiting for work.
	//

// Determine how many list elements (Files or Pairs of Files) to give to each Thread
	unsigned long	num_to_assign  = num_in_list;
	unsigned long	num_per_thread = num_in_list / (unsigned long)g_threads_status_in_use;
	if ( 0 == num_per_thread )
		num_per_thread = 1;

	unsigned long	isAssigned[ MAX_UCC_THREAD_COUNT ];
	memset( isAssigned, 0, sizeof( isAssigned ) );

	unsigned long	toAssign[ MAX_UCC_THREAD_COUNT ];
	memset( toAssign, 0, sizeof( toAssign ) );

	unsigned int	num_threads_to_assign = 0;
	unsigned long	thisAssign = num_per_thread;
	for ( unsigned int j = 0; j < g_threads_status_in_use; j++ )
	{
		if ( ( j == g_threads_status_in_use - 1 ) 
		  || ( num_to_assign < thisAssign ) )
			thisAssign = num_to_assign;

		toAssign[ j ] = thisAssign;
		num_threads_to_assign++;
		num_to_assign -= thisAssign;
		if ( num_to_assign <= 0 )
			break;
	}

	std::vector<std::string>::iterator itFileStart, itFileEnd;
	std::vector<std::string>::iterator iFStart, iFEnd;

	DiffTool::MatchingType::iterator itDiffStart, itDiffEnd;
	DiffTool::MatchingType::iterator iDStart, iDEnd;

	if ( isFileList )
	{
		itFileStart = inFileVector->begin();
		itFileEnd   = inFileVector->end();
	}
	else
	{
		itDiffStart = pDiff->matchedFilesList.begin();
		itDiffEnd   = pDiff->matchedFilesList.end();
	}

	do
	{
		g_assigned_count = 0;

		// Find a thread ready for Work
		for ( unsigned int j = 0; j < g_threads_status_in_use; j++ )
		{
			if ( EXIT_NOW == g_threads_status[ j ].main_WantsAction )
			{
		#ifdef	_DEBUG
				// Unexpected combination of controls got here
				if ( false == unexpected_state )
				{
					unexpected_state = true;
					printf( "\nERROR: Unexpcected state from combination of thread/Main thread values!\n" );
				}
		#endif
				continue;		// Should NOT get here
			}

			if ( THREAD_WAITING_FOR_WORK != g_threads_status[ j ].work_ThreadState )
				continue;

			if ( isAssigned[ j ] ) 
			{
				g_assigned_count++;
				continue;
			}

			if ( 0 == toAssign[ j ] )
				continue;

			if ( g_threads_status[ j ].thread_had_error )
			{
				error_count = 1;	// some error processing a file
				// Error details are found in the itFileElement first and second elements

			}
			g_threads_file_names[ j ].clear();

			g_threads_status[ j ].noRead                          = noRead;
			g_threads_status[ j ].noWeb                           = noWeb;
			g_threads_status[ j ].print_cmplx                     = print_cmplx;
			g_threads_status[ j ].print_csv                       = print_csv;
			g_threads_status[ j ].process_after_read              = process_after_read;
			g_threads_status[ j ].discard_PHY_lines_after_process = discard_PHY_lines_after_process;
			g_threads_status[ j ].discard_lines_after_process     = discard_lines_after_process;
			g_threads_status[ j ].no_warnings_to_UI               = no_warnings_to_UI;
			g_threads_status[ j ].no_uncounted                    = no_uncounted;

			if ( isFileList )
				iFStart = itFileStart;
			else
				iDStart = itDiffStart;

			if ( j )
			{
				if ( j < g_threads_status_in_use - 1 )
				{
					if ( isFileList )
					{
						advance( iFStart, j * ( num_per_thread ) );
						iFEnd = iFStart;
						advance( iFEnd, toAssign[ j ] );
					}
					else
					{
						advance( iDStart, j * ( num_per_thread ) );
						iDEnd = iDStart;
						advance( iDEnd, toAssign[ j ] );
					}
				}
				else
				{
					if ( isFileList )
					{
						iFEnd = itFileEnd;
						iFStart = iFEnd;
						iFStart--;
						advance( iFStart, - (int)( toAssign[ j ] - 1 ) );
					}
					else
					{
						iDEnd = itDiffEnd;
						iDStart = iDEnd;
						iDStart--;
						advance( iDStart, - (int)( toAssign[ j ] - 1 ) );
					}
				}
			}
			else
			{
				// This is only valid for thread index 0
				// g_threads_status[ 0 ].pThreadSrcFileList = mySrcFileList;
				if ( isFileList )
				{
					iFEnd = iFStart;
					advance( iFEnd, toAssign[ j ] );
				}
				else
				{
					iDEnd = iDStart;
					advance( iDEnd, toAssign[ j ] );
				}
			}

			if ( isFileList )
			{
				g_threads_status[ j ].itStart   = iFStart;
				g_threads_status[ j ].itEnd     = iFEnd;
				g_threads_status[ j ].is_File_Work = true;
			}
			else
			{
				g_threads_status[ j ].itDStart  = iDStart;
				g_threads_status[ j ].itDEnd    = iDEnd;
				g_threads_status[ j ].is_File_Work = false;
				g_threads_status[ j ].match_threshold = pDiff->GetMatchThreshold();

				// Zero counts for Diff and Dup
				g_threads_status[ j ].total_addedLines = 0;
				g_threads_status[ j ].total_deletedLines = 0;
				g_threads_status[ j ].total_modifiedLines = 0;
				g_threads_status[ j ].total_unmodifiedLines = 0;

				g_threads_status[ j ].dup_addedLines = 0;
				g_threads_status[ j ].dup_deletedLines = 0;
				g_threads_status[ j ].dup_modifiedLines = 0;
				g_threads_status[ j ].dup_unmodifiedLines = 0;
			}

			g_threads_status[ j ].useListA      = useListA;
			g_threads_status[ j ].clearCaseFile = clearCaseFile;
			
			g_threads_status[ j ].outDir        = outDir;

			g_threads_status[ j ].userIF        = userIF;	// thread checks this for != NULL
			g_threads_status[ j ].main_WantsAction = GET_TO_WORK;

            // Toggle semaphore to wake thread and then yield remaining time slice
			g_threads_status[ j ].pSem->signal();
#ifdef  QTGUI
//            g_threads_status[ j ].pQSem->release();
            QThread::yieldCurrentThread();
#else
            boost::this_thread::yield();	// Allow assigned thread (and maybe others) to run
#endif



			isAssigned[ j ] = j + 1;
			g_assigned_count++;
			if ( g_assigned_count == num_threads_to_assign )
				break;

		}	//	END		for ( unsigned int j = 0; j < g_threads_status_in_use; j++ )

		if ( g_assigned_count < num_threads_to_assign )
        {
#ifdef  QTGUI
            QThread::yieldCurrentThread();
#else
			boost::this_thread::yield();	//ThreadSleep( 100 );
#endif
        }

	} while ( g_assigned_count < num_threads_to_assign );

	return error_count;
}
#endif

//}	//	END		namespace _prv_UCC_THREAD_NAMESPACE_


////////////////////////////////////////////////////////////////////////
//
//			P U B L I C		P R O C E D U R E S
//
///////////////////////////////////////////////////////////////////////

//using namespace _prv_UCC_THREAD_NAMESPACE_;

// Return number of HW cores available.  May be lower than actual if threads not enabled.
unsigned int GetCPU_count()
{
	num_HW_cores = 1;

#ifdef	ENABLE_THREADS
	#ifdef  QTGUI
		num_HW_cores = QThread::idealThreadCount();
	#else
		num_HW_cores = thread::hardware_concurrency();
	#endif
#endif

	return num_HW_cores;
}

#ifdef	ENABLE_THREADS
// Give up the rest of the time slice this thread has and
// Put the currently executing thread to sleep for approximately the time wanted
void ThreadSleep( const unsigned int milliSeconds )
{
	//boost::this_thread::yield();

	unsigned int sleep_amount = milliSeconds;

	if ( sleep_amount < MIN_SLEEP_TIME )
		sleep_amount = MIN_SLEEP_TIME;

	// Sleep for a given amount.  Actual time could be somewhat longer.
#ifdef  QTGUI
    QThread::msleep( (unsigned long)sleep_amount );
#else
	sleep( boost::posix_time::milliseconds( sleep_amount ) );
#endif

	// boost::this_thread::yield() could be called instead (just not from here)
	// to give up rest of current time slice of the running thread
}
#endif


// Read a source file and Parse it if wanted
void ReadSrcFile( const unsigned int					threadIdx,
					int							&		error_count,
					CCodeCounter			*			pCounter,
					filemap						&		fmap,
					results						&		r,
					const		bool					noRead,
					const		bool					readWebFiles,
					const		bool					onlyPhysical,  // just Read Physical lines
					UserIF					*			userIF,
					CounterForEachLangType		&		CounterForEachLanguage,
					const		bool					print_cmplx,
					const		bool					print_csv,
					const		bool					useListA,
					const		string					fileName,
					const		string					clearCaseCroppedFile,
					const		bool					clearCaseFile,
					SourceFileElement		*			pSrcFileEl,
					SourceFileList			*			pSrcFileList,
					SourceFileList			*			mySourceFileA,
					SourceFileList			*			mySourceFileB,
					string						&		myErrList,
					string								outDir,
					ErrMsgStructVector			&		err_msgs,
					UncountedFileStructVector	&		unc_files,
					const		bool					process_after_read,
					const		bool					discard_PHY_lines_after_process,
					const		bool					discard_lines_after_process,
					const		bool					no_warnings_to_UI )
{
	ClassType	fileclass = r.class_type;
	CWebCounter *webCounter;		// Modification: 2011.05
	WebType		webType;

//	string		fileName, clearCaseCroppedFile;

	bool			OK_to_process = false;
	bool			lineTooLong;
	bool			isErr = false;		// Modification: 2011.05
	unsigned int	lineNum = 0;		// Modification: 2009.01
	unsigned int	num_inserted = 0;
	string			oneline;

	SourceFileList	*	mySrcFileList = pSrcFileList;
	SourceFileList		srcFileListTemp;

	bool	readThisFile = ! noRead;

	if ( WEB == fileclass )
		readThisFile = readWebFiles;

	if ( readThisFile && !onlyPhysical )
	{
		// Update the total number of files and the number of files of each type
		// Because of using a separate instance of CounterForEachLanguage per thread these
		// changes must be accumulated back later to the main thread's instance as well.
		if (useListA)
			pCounter->total_filesA++;
		else
			pCounter->total_filesB++;
        
/*Enable this to TEST Signal handling showing file(s) being parsed		
#define	TEST_SIGSEG	
        
#ifdef	TEST_SIGSEG		
		bool	force_Signal = false;
		if ( ( MAIN_THREAD_INDEX == threadIdx ) && ( 200 < pCounter->total_filesA ) )	// Force an access violation for Testing		
			force_Signal = true;		
		else		
		if ( (threadIdx + 1) * 200 < pCounter->total_filesA )	// Force an access violation for Testing with multiple threads		
			force_Signal = true;		
		if ( true == force_Signal )		
		{		
			int * pInt = NULL;		
			int k = *pInt;			// Use of NULL pointer  SIGSEGV on Windows		
			printf( "Kablooie ! ! ! %d", k );	// This print NEVER happens...		
		}		
#endif*/
		
		// deal with every specific kind of WEB
		if (fileclass == WEB)
		{
			// get web file class
			webCounter = (CWebCounter *)pCounter;
			webType = webCounter->GetWebType(clearCaseFile ? clearCaseCroppedFile : fileName);
			if (webType == WEB_PHP)
			{
				if (useListA)
					webCounter->total_php_filesA++;
				else
					webCounter->total_php_filesB++;
			}
			else if (webType == WEB_ASP)
			{
				if (useListA)
					webCounter->total_asp_filesA++;
				else
					webCounter->total_asp_filesB++;
			}
			else if (webType == WEB_JSP)    // Modification: 2011.10
			{
				if (useListA)
					webCounter->total_jsp_filesA++;
				else
					webCounter->total_jsp_filesB++;
			}
			else if (webType == WEB_XML)
			{
				if (useListA)
					webCounter->total_xml_filesA++;
				else
					webCounter->total_xml_filesB++;
			}
			else if (webType == WEB_CFM)
			{
				if (useListA)
					webCounter->total_cfm_filesA++;
				else
					webCounter->total_cfm_filesB++;
			}
			else
			{
				if (useListA)
					webCounter->total_htm_filesA++;
				else
					webCounter->total_htm_filesB++;
			}
		}
	}

	if ( 0 == fileName.size() )    // Keep from nesting code below deeper.  Modification: 2015.12
		return;                  // Modification: 2015.12

	// Read in a file
	// Set up decision variable used when processing each Physical line.
	bool	trim_leading = true;
	if ( ( fileclass == FORTRAN ) 
		|| ( fileclass == PYTHON ) )
		trim_leading = false;	// column location and/or white space is significant

#ifdef	USE_MEMORY_MAPPED_FILES
	// Set up to Read using a Memory Mapped file.
	// Should be faster as it could avoid 1 or more copy operations by OS.
	// However testing this implementation on Windows 7.1 showed it was SLOWER by a fair amount.  
	// Experiment with it if you want.

	// Create a read only Memory Mapped file interface with stream semantics
	// The way this library works is that we only need to create 1 instance here.
	// Cleanup happens when this procedure returns and the class object instance goes out of scope.
	boost::iostreams::stream<boost::iostreams::mapped_file_source> fr;

	// Use the Memory Mapped file interface created earlier to open the file
	// Open the Memory Mapped source (read only) file
	fr.open( fileName.c_str() );
#else
	// Open the file
	ifstream fr( fileName.c_str(), ios::in ); // Modification: 2013.04
#endif
	if (!fr.is_open())  // Modification: 2009.01
	{
		r.e_flag = true;
		r.error_code = "Unable to open file";
		error_count++;
		string err = "Error: ";
		err += r.error_code;
		err += " (";
		err += fileName;
		err += ")";
		if (!isErr)
		{
			_prv_SaveOrAddError( threadIdx, userIF, err_msgs, err, false, 1 );
			isErr = true;
		}
		else
			_prv_SaveOrAddError( threadIdx, userIF, err_msgs, err );
		_prv_SaveOrWriteUncountedFile( threadIdx, userIF, unc_files, "Not Readable", fileName, useListA, print_csv, outDir, false ); // Modification: 2011.10  2015.12
	}
	else
	{
		// Get Physical file size (Bytes)     Modification: 2015.12
		if ( 0 == r.file_size_bytes )
		{
			fr.seekg( 0, std::ios_base::end );
			r.file_size_bytes = fr.tellg();
			fr.seekg( 0 );
		}

		OK_to_process = true;

		if ( pSrcFileEl && !onlyPhysical )
			pSrcFileEl->second.resetMetrics();		// Start over with the metrics

		// File opened OK, read each Physical line into memory
		lineNum = 0;
		lineTooLong = false;

		if ( readThisFile )
		{
			// save every valid line
			while (fr.good() || fr.eof())
			{
				getline(fr, oneline);
				if ((!fr.good() && !fr.eof()) || (fr.eof() && oneline.empty()))
					break;

				lineNum++;  // Modification: 2011.05  Keep line number as counting (1 ...) instead of (0 ...)    Modification: 2015.12
				if (oneline.size() > MAX_LINE_LENGTH)
				{
					lineTooLong = true;
					break;
				}

				if ( trim_leading )
					oneline = CUtil::TrimString( oneline );		// trim Leading and Trailing
				// TODO: uncomment this and test with Fortran before having part of code.
				// else // A little testing with Python is OK but do not expect any Python problems.
				//	oneline = CUtil::TrimString( oneline, 1 );	// trim Trailing only

				if ( oneline.size() )
					oneline = CUtil::ReplaceSmartQuotes(oneline);    // Modification: 2011.10
				
				// Moved element(lineNum, oneline); as lineElement  
				// inside call below to prevent Debug build memory leaks.
				fmap.push_back( lineElement( lineNum, oneline ) );    // Modification: 2011.05  2015.12
				if (!fr.good())                                       // Modification: 2011.10
					break;
			}
		}
		else
			OK_to_process = false;

		fr.clear();
		fr.close();

		if (lineTooLong)    // Modification: 2011.05
		{
			r.e_flag = true;
			r.error_code = "Line too long";
			error_count++;
			string err = "Error: ";
			err += r.error_code;
			err += ", file skipped (";
			err += fileName;
			err += ")";
			if (!isErr)
			{
				_prv_SaveOrAddError( threadIdx, userIF, err_msgs, err, false, 1 );
				isErr = true;
			}
			else
				_prv_SaveOrAddError( threadIdx, userIF, err_msgs, err );
			fmap.resize( 0 );  // don't bother processing the file if an error is just going to be reported.
					
			// Add the line number that was too long to the message
			string tmp;
			IntToStr( (int)lineNum, tmp );
			err = "Excessive Line Length on Line " + tmp;
			_prv_SaveOrWriteUncountedFile( threadIdx, userIF, unc_files, err, fileName, useListA, print_csv, outDir, false );

			// Allow this to set up empty settings that will show as Zero counts in results files.
			OK_to_process = true;
		}
		else if ( readThisFile && ( fmap.size() == 0 ) )
		{
			bool handleZeroSize = true;
			if ( fileclass == PYTHON )
			{
				// Python has a common practice to declare various packages/modules 
				// using just the existence of a special file name.  
				// If so then don't warn about it.
				string fileNameOnly = CUtil::ExtractFilename( fileName );
				if ( fileNameOnly.compare( "__init__.py" ) == 0 )
					handleZeroSize = false;
			}

			if ( true == handleZeroSize )
			{
				bool	logOnly = false;
				if ( true == no_warnings_to_UI )
					logOnly = true;

				string err = "Warning: File is empty (";
				err += fileName;
				err += ")";
				if (!isErr)
				{
					_prv_SaveOrAddError( threadIdx, userIF, err_msgs, err, logOnly, no_warnings_to_UI, 1 );
					isErr = true;
				}
				else
					_prv_SaveOrAddError( threadIdx, userIF, err_msgs, err, logOnly, no_warnings_to_UI );
			}

			// Allow this to set up empty settings that will show as Zero counts in results files.
			OK_to_process = true;
		}
	}

	if ( readThisFile )
		// save the number of lines as the file size
		r.file_size = lineNum;

	if ( onlyPhysical )
		return;			// got the Physical lines loaded into RAM now

	// Avoid unnecessary calls to ExtractFilename later on
	if ( ! discard_lines_after_process )
		r.file_name_only = CUtil::ExtractFilename( r.file_name );

	// depending on which source file we are on...
	if ( NULL != mySrcFileList )
		mySrcFileList->push_back( SourceFileElement( fmap, r ) );    // Modification: 2013.04
	else
	{
		// Use temporary source file list as caller does NOT give a list to change.
		// This works OK only if NOT Web files.
		mySrcFileList = &srcFileListTemp;
		mySrcFileList->push_back( *pSrcFileEl );
	}

	if ( process_after_read && OK_to_process )
	{
		// set iterator to file entry just appended at end
		SourceFileList::iterator i = mySrcFileList->end();
		i--;
		ProcessSourceListFile( threadIdx, userIF, CounterForEachLanguage, 
							&pCounter, i, mySourceFileA, mySourceFileB, 
							num_inserted,
							err_msgs, unc_files,
							myErrList, 
							(*i).second.file_name,
							print_csv, print_cmplx, useListA );
		if ( NULL != pSrcFileEl )
			*pSrcFileEl = (*i);
		if ( num_inserted )
		{
			SourceFileList::iterator itPos = i;
			itPos++;
			for ( unsigned int k = 0; k < num_inserted; k++ )
			{
				ProcessSourceListFile( threadIdx, userIF, CounterForEachLanguage, 
							&pCounter, itPos, mySourceFileA, mySourceFileB, 
							num_inserted,
							err_msgs, unc_files,
							myErrList, 
							(*itPos).second.file_name,
							print_csv, print_cmplx, useListA );

				SourceFileElement & element = (*itPos);
				if ( discard_lines_after_process )
				{
					// Better to release in reverse order of allocation
					element.second.mySLOCLines.clear();	// Empty the vector of Logical source lines
					element.first.resize( 0 );			// Empty the file map of Physical source lines
				}
				if ( discard_PHY_lines_after_process )
					element.first.resize( 0 );			// Empty the file map of Physical source lines

				itPos++;
				if ( itPos == mySrcFileList->end() )
					break;
			}

			SourceFileElement & element = (*i);
			if ( discard_lines_after_process )
			{
				// Better to release in reverse order of allocation
				element.second.mySLOCLines.clear();	// Empty the vector of Logical source lines
				element.first.resize( 0 );			// Empty the file map of Physical source lines
			}
			if ( discard_PHY_lines_after_process )
				element.first.resize( 0 );			// Empty the file map of Physical source lines

			num_inserted = 0;
		}
	}

	return;
}


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
					const		bool					no_warnings_to_UI )
{
	int					error_count = 0;

	CCodeCounter	*	pCounter = CounterForEachLanguage[ pSrcFileEl->second.class_type ];
	string				fileName = pSrcFileEl->second.file_name;
	string				clearCaseCroppedFile = "";
	
	if ( clearCaseFile )
	{
		clearCaseCroppedFile = pSrcFileEl->second.file_name;
		fileName = clearCaseCroppedFile + pSrcFileEl->second.clearCaseTrailer;
	}

	ReadSrcFile( threadIdx,
				error_count,
				pCounter,
				pSrcFileEl->first,				// filemap
				pSrcFileEl->second,				// results
				false,							// noRead
				readWebFiles,
				onlyPhysical,
				userIF,
				CounterForEachLanguage,
				print_cmplx,
				print_csv,
				useListA,
				fileName,
				clearCaseCroppedFile,
				clearCaseFile,
				pSrcFileEl,
				NULL,							// mySrcFileList  
				mySourceFileA,
				mySourceFileB,
				myErrList,
				outDir,
				err_msgs,
				unc_files,
				process_after_read,
				discard_PHY_lines_after_process,
				discard_lines_after_process,
				no_warnings_to_UI );

	return error_count;
}



/*!
* 1.Function Description:
*	 DIFF a list of Matched Pairs.
*	 Reads file Physical lines to RAM and Analyze/Count if needed.
*    Likely to release RAM memory for file Physical (maybe Logical) source lines after DIFF.
*    Updates percent done on UI as this step runs.
*    Return number of Errors found.
*    Safe to call this from multiple threads.
*    Refactored to:
		 support late Reading of Physical lines / release after DIFF to save RAM
*        support thread extra args, error handling, ease of reading the code!
*
* 2.Parameters:
*    threadIdx               IN      index of calling Thread.
*    userIF
*    CounterForEachLanguage  IN/OUT  reference to struct with pointers to Language class instances
*
*    inputFileVector: list of files to count (may be empty if inputFileList is given)
*    inputFileList: file containing list of files to count (if any)
*    useListA: use file list A? (otherwise use list B)
*
*    Returns	number of Errors found
*
* 3.Creation Time And Owner:
*	 Version: 2011.05  Created as DiffTool::ProcessPairs
*    Revised: 2015.12  Moved here out of ProcessPairs and refactor for Thread use.
*/
int DiffPairsInList( const unsigned int					threadIdx,
					const		double					match_threshold,
					UserIF					*			userIF,
					bool						&		printDup,
					CounterForEachLangType		&		CounterForEachLanguage,
					const		bool					print_cmplx,
					const		bool					print_csv,
					DiffTool::MatchingType::iterator	myI,		// matchedFilesList, matching file pairs and differencing results 
					DiffTool::MatchingType::iterator	itEnd,
					const		bool					clearCaseFile,
					SourceFileList			*			mySourceFileA,
					SourceFileList			*			mySourceFileB,
					string						&		myErrList,
					string								outDir,
					ErrMsgStructVector			&		err_msgs,
					UncountedFileStructVector	&		unc_files,
					const		bool					discard_PHY_lines_after_process,
					const		bool					discard_lines_after_process,
					const		bool					no_warnings_to_UI,
					unsigned int				&		dup_addedLines,
					unsigned int				&		dup_deletedLines,
					unsigned int				&		dup_modifiedLines,
					unsigned int				&		dup_unmodifiedLines,
					unsigned int				&		total_addedLines,
					unsigned int				&		total_deletedLines,
					unsigned int				&		total_modifiedLines,
					unsigned int				&		total_unmodifiedLines )
{
	int		error_count = 0;

	// Modification: 2014.08
	CmpMngr *pDiffManager = NULL;
	if ( MAIN_THREAD_INDEX == threadIdx )
	{
		// OK to use globals from Main thread
		extern bool visualDiff;
		if ( visualDiff )
			pDiffManager = new CmpMngr( &(pDiffTool->outfile_file_dump) );
		else
			pDiffManager = new CmpMngr();
	}
	else
		pDiffManager = new CmpMngr();

	srcLineVector* myNullFile = new srcLineVector();
	srcLineVector* firstFile;
	srcLineVector* secondFile;

	string current_file_name;
	typedef vector< pair<bool, DiffTool::MatchingType::iterator> > WebFilesVector;
	WebFilesVector web_files_vector;
	string web_file_name, base_file_name;
	size_t web_marker;
	bool unmatchedDup, found;
	SourceFileElement * pElement = NULL;

	unsigned long	count_done = 0L;
	unsigned long	prev_count_done = 0L;
	unsigned long	num_in_loop = (unsigned long)distance( myI, itEnd );

	// Only need to update when percent changes so will do twice as often
	unsigned int	inc_amount = num_in_loop / 200L;
	if ( 0 == inc_amount )
		inc_amount = 1;
	unsigned int	UI_count_down = inc_amount;
#define		UI_DIFF_UPDATE_COUNT	5
	string diffDetails;

//		BIG Loop to process all the Matched Pairs
//
	for ( ; myI != itEnd; myI++ )
	{
		count_done++;

		// each source file elements results object has a mySLOCLines object with the SLOC to be diffed
		// check for unmatched duplicates
		firstFile = myNullFile;
		secondFile = myNullFile;
		unmatchedDup = false;
		web_file_name = "";

		if ((*myI).second.first != NULL)
		{
			// Some files do not produce any Logical source lines of code
			// File may have been parsed but need to check for evidence of a parse
			if ( ( (*myI).second.first->second.file_size_bytes )
				&& ( false == (*myI).second.first->second.e_flag )
			  && ( 0 == (*myI).second.first->second.mySLOCLines.size() ) 
			  && ( 0 == (*myI).second.first->second.comment_lines )
			  && ( 0 == (*myI).second.first->second.e_comm_lines )
			  && ( 0 == (*myI).second.first->second.blank_lines ) )
			{
				bool readThisFile = true;
				if ( ( true == (*myI).second.first->second.file_name_isEmbedded )
				  || ( WEB == (*myI).second.first->second.class_type ) )
					readThisFile = false;	// Logical source lines are expanded to follow on Web sep file(s)

				if ( readThisFile )
				{
					// Read and process this file and keep everything until after DIFF !
					int errors = 
					ReadFileLines( (*myI).second.first, threadIdx, userIF, 
								false,				// read Web files
								false,				// not only Physical lines
								clearCaseFile, 
								CounterForEachLanguage, print_cmplx, print_csv, 
								true,				// use list A
								mySourceFileA, 
								mySourceFileB,
								myErrList,
								outDir,
								err_msgs,
								unc_files,
								true,				// process_after_read
											// These will be discarded as needed after DIFF
								false,				// discard_PHY_lines_after_process
								false,				// discard_lines_after_process
								no_warnings_to_UI );
					if ( errors )
					{
					#ifdef	_DEBUG
						printf( "Error: DiffPairsInList %d errors from ReadFileLines for %s\n",
							errors, (*myI).second.first->second.file_name.c_str() );
					#endif
						// Error message already handled from ReadFileLines
						// TODO: Put message in MatchedPairs that this pair was not processed
						goto FreeDiffBuffers;	// Can't do Diff for this pair
					}
				/*
					#ifdef	_DEBUG
					if ( 0 == (*myI).second.first->second.mySLOCLines.size() )
					{
						if ( WEB != myI->second.first->second.class_type )
						{
							// This may be OK.  For instance .h files parsed by C/C++ do not generate any logical source lines
							// Or .cpp or other source files that are only blank lines and comments.
							printf( "Information: DiffPairsInList  ReadFileLines did not add source lines for %s\n",
								(*myI).second.first->second.file_name.c_str() );
						}
					}
					#endif
				*/
				}
			}

			firstFile = &(*myI).second.first->second.mySLOCLines;

			// keep track of classes of type WEB
			current_file_name = myI->second.first->second.file_name;
			if (myI->second.first->second.class_type == WEB)
				web_file_name = current_file_name;

			if ((*myI).second.first->second.duplicate || (*myI).second.first->second.firstDuplicate)
				unmatchedDup = true;
		}
		if ((*myI).second.second != NULL)
		{
			if ( ( (*myI).second.second->second.file_size_bytes )
				&& ( false == (*myI).second.second->second.e_flag )
			  && ( 0 == (*myI).second.second->second.mySLOCLines.size() ) 
			  && ( 0 == (*myI).second.second->second.comment_lines )
			  && ( 0 == (*myI).second.second->second.e_comm_lines )
			  && ( 0 == (*myI).second.second->second.blank_lines ) )
			{
				bool readThisFile = true;
				if ( ( true == (*myI).second.second->second.file_name_isEmbedded )
				  || ( WEB == (*myI).second.second->second.class_type ) )
					readThisFile = false;	// Logical source lines are expanded to follow on Web sep file(s)

				if ( readThisFile )
				{
					// Read and process this file
					int errors = 
					ReadFileLines( (*myI).second.second, threadIdx, userIF, 
								false,				// read Web files
								false,				// not only Physical lines
								clearCaseFile, 
								CounterForEachLanguage, print_cmplx, print_csv, 
								false,				// use list A
								mySourceFileA, 
								mySourceFileB,
								myErrList,
								outDir,
								err_msgs,
								unc_files,
								true,				// process_after_read
											// These will be discarded as needed after DIFF
								false,				// discard_PHY_lines_after_process
								false,				// discard_lines_after_process
								no_warnings_to_UI );
					if ( errors )
					{
					#ifdef	_DEBUG
						printf( "Error: DiffPairsInList %d errors from ReadFileLines for %s\n",
							errors, (*myI).second.second->second.file_name.c_str() );
					#endif
						// Error message already handled from ReadFileLines
						// TODO: Put message in MatchedPairs that this pair was not processed
						goto FreeDiffBuffers;	// Can't do Diff for this pair
					}
				/*
					#ifdef	_DEBUG
					if ( 0 == (*myI).second.second->second.mySLOCLines.size() )
					{
						if ( WEB != (*myI).second.second->second.class_type )
						{
							// This may be OK.  For instance .h files parsed by C/C++ do not generate any logical source lines
							// Or .cpp or other source files that are only blank lines and comments.
							printf( "Information: DiffPairsInList  ReadFileLines did not add source lines for %s\n",
								(*myI).second.second->second.file_name.c_str() );
						}
					}
					#endif
				*/
				}
			}
			secondFile = &(*myI).second.second->second.mySLOCLines;

			// keep track of classes of type WEB
			current_file_name = myI->second.second->second.file_name;
			if (myI->second.second->second.class_type == WEB)
				web_file_name = current_file_name;	// always overwrite with the second file if it exists

			if ((*myI).second.second->second.duplicate || (*myI).second.second->second.firstDuplicate)
			{
				if ((*myI).second.first == NULL)
					unmatchedDup = true;
				else
					unmatchedDup = false;
			}
			else
				unmatchedDup = false;
		}

		// this makes sure that if one of the files was unmatched it will just compare it against an empty set
		diffDetails = pDiffManager->Compare(firstFile, secondFile, match_threshold);

		// Use pointer to diff tool class to use from potential thread code  Modification: 2015.12
		//Only output modified files in outfile_file_dump  Modification: 2016.10
		if (!diffDetails.empty() && MAIN_THREAD_INDEX == threadIdx)
		{
			if (pDiffTool->outfile_file_dump)
			{
				if ((*myI).second.first == NULL) {
					pDiffTool->outfile_file_dump << "NA" << endl;
				}
				else {
					pDiffTool->outfile_file_dump << (*myI).second.first->second.file_name << endl;
				}
				if ((*myI).second.second == NULL) {
					pDiffTool->outfile_file_dump << "NA" << endl;
				}
				else {
					pDiffTool->outfile_file_dump << (*myI).second.second->second.file_name << endl;
				}
				pDiffTool->outfile_file_dump << diffDetails << endl;
				pDiffTool->outfile_file_dump.flush();	// Save in case of LOW Memory
			}
		}

		// this makes sure that if one of the files was unmatched it will just compare it against an empty set
		//pDiffManager->Compare(firstFile, secondFile, match_threshold);

		// if the file is a class of type WEB, keep a list
		if (web_file_name.size() > 0)
		{
			// always use the WEB file status, not subsets
			web_files_vector.push_back(pair<bool, DiffTool::MatchingType::iterator>(unmatchedDup, myI));
		}
		else
		{
			// check to see if file is a web subset
			web_marker = current_file_name.find(EMBEDDED_FILE_PREFIX);
			if (web_marker != string::npos)
			{
				// if it is a web subset, search through the captured WEB files
				// assumes the web files are populated prior to the web subsets
				found = false;
				for (WebFilesVector::iterator web_iter = web_files_vector.begin();
					web_iter != web_files_vector.end(); ++web_iter)
				{
					base_file_name = current_file_name.substr(0, web_marker);
					if (web_iter->second->second.second != NULL)
					{
						if (base_file_name.compare(web_iter->second->second.second->second.file_name) == 0)
							found = true;
					}
					if (!found && web_iter->second->second.first != NULL)
					{
						if (base_file_name.compare(web_iter->second->second.first->second.file_name) == 0)
							found = true;
					}
					if (found)
					{
						web_iter->second->first.addedLines += pDiffManager->nAddedLines;
						web_iter->second->first.deletedLines += pDiffManager->nDeletedLines;
						web_iter->second->first.modifiedLines += pDiffManager->nChangedLines;
						web_iter->second->first.unmodifiedLines += pDiffManager->nNochangedLines;
						unmatchedDup = web_iter->first;
						break;
					}
				}
			}
		}

		if (unmatchedDup)
		{
			printDup = true;
			dup_addedLines += (*myI).first.addedLines = pDiffManager->nAddedLines;
			dup_deletedLines += (*myI).first.deletedLines = pDiffManager->nDeletedLines;
			dup_modifiedLines += (*myI).first.modifiedLines = pDiffManager->nChangedLines;
			dup_unmodifiedLines += (*myI).first.unmodifiedLines = pDiffManager->nNochangedLines;
		}
		else
		{
			total_addedLines += (*myI).first.addedLines = pDiffManager->nAddedLines;
			total_deletedLines += (*myI).first.deletedLines = pDiffManager->nDeletedLines;
			total_modifiedLines += (*myI).first.modifiedLines = pDiffManager->nChangedLines;
			total_unmodifiedLines += (*myI).first.unmodifiedLines = pDiffManager->nNochangedLines;
		}

FreeDiffBuffers:
		// Dup checking requires only the number of Physical lines (Not contents)
		// and the detailed Logical SLOC, so...
		// IF Dup checking after DIFF, AND the Logical lines RAM buffer is released
		// this is going to hurt Dup checking performance.
		// This NEEDS to be clearly Explained in Docs and recommended to User
		// to do Dup checks on 1 Baseline file set at a time for larger file sets.
		//
		// Better to release in reverse order of allocation
		// Release RAM buffers used for File B source lines
		pElement = (*myI).second.second;
		if ( NULL != pElement )
		{
			if ( discard_lines_after_process )
			{
				// TODO: Add code here to release extra RAM buffers used by WEB embedded "files"  IT MUST BE FAST.
				pElement->second.mySLOCLines.clear();	// Empty the vector of Logical source lines
				pElement->first.resize( 0 );			// Empty the file map of Physical source lines
			}
			else
			if ( discard_PHY_lines_after_process )
				pElement->first.resize( 0 );			// Empty the file map of Physical source lines
		}

		// Release RAM buffers used for File A source lines
		pElement = (*myI).second.first;
		if ( NULL != pElement )
		{
			if ( discard_lines_after_process )
			{
				// TODO: Add code here to release extra RAM buffers used by WEB embedded "files"  IT MUST BE FAST.
				pElement->second.mySLOCLines.clear();	// Empty the vector of Logical source lines
				pElement->first.resize( 0 );			// Empty the file map of Physical source lines
			}
			else
			if ( discard_PHY_lines_after_process )
				pElement->first.resize( 0 );			// Empty the file map of Physical source lines
		}

		// Update UI as needed
		if ( MAIN_THREAD_INDEX != threadIdx )
		{
			UI_count_down--;
			if ( ( 0 == UI_count_down ) 
			  || ( prev_count_done + UI_DIFF_UPDATE_COUNT <= count_done ) )
			{
			#ifdef	ENABLE_THREADS
				// Save work progress for later display
				int wanted_action = _prv_IncrementWorkDone( threadIdx, 
										(unsigned int)( count_done - prev_count_done ) );

				if ( ( STOP_WORKING == wanted_action )
				  || ( EXIT_NOW     == wanted_action ) )
				{
					break;	// Don't do any more files.  Get out.
				}
			#endif
				UI_count_down = inc_amount;
				prev_count_done = count_done;
			}
		}
		else
		{
			// Running from Main thread, update UI if needed
			UI_count_down--;
			if ( ( 0 == UI_count_down ) 
			  || ( prev_count_done + UI_DIFF_UPDATE_COUNT <= count_done ) )
			{
			#ifdef	QTGUI
				// Percent changed so update UI
				int percent_done = ( 100 * count_done / num_in_loop );
				userIF->updateProgress( "", false, percent_done, num_in_loop );
			#else
				// Just keep erasing and redrawing a text area with updated count
				string cntStr;
				LongToStr( (long)count_done, cntStr );
				cout << "\b\b\b\b\b\b\b\b\b\b" << cntStr;		// 10 backspace + new count
				for ( int i = 0 ; i < 10 - (int)cntStr.size(); i++ )
					cout << " ";	// fill at right to be easier next time
				cout << flush;
			#endif
				UI_count_down = inc_amount;
				prev_count_done = count_done;
			}
			// processing Canceled ?
			if ( userIF != NULL )
			{
				bool has_cancelled = userIF->isCanceled();
				if ( has_cancelled )
				{
					break;	// Stop Looping and return
				}
			}
		}
	}

	delete myNullFile;
	delete pDiffManager;

	return error_count;
}


// Duplicate Checking for a list of 1 or more Languages
// Each Language has an array of pointers to structs in a baseline file set
// This is safe to call from multiple threads
// 
int DupCheckLanguages( unsigned int					threadIdx, 
						vector<struct langTypeArrayHeader> * langs,
						ErrMsgStructVector		*	err_msgs,
						UncountedFileStructVector * unc_files,
						string					*	myErrList,
						const double				dup_threshold, 
						StringVector			*	dupList1, 
						StringVector			*	dupList2, 
						const bool					checkMatch, 
						const bool					useListA,
						UserIF					*	userIF,
						const bool					clearCaseFile,
						CounterForEachLangType	&	CounterForEachLanguage,
						const bool					print_cmplx,
						const bool					print_csv,
						SourceFileList			*	mySourceFileA,
						SourceFileList			*	mySourceFileB )
{
	int		error_count = 0;

	// Update UI while finding Duplicates
	
#ifdef	QTGUI
	// Percent counting approach is NOT good enough.  TODO: Change Qt UI to use counts as well
	int				percent_done = 0;
    unsigned long	num_in_list  = 2 * (*langs)[0].num_pointers;
#endif

	unsigned long	count_done        = 0L;
#ifdef          ENABLE_THREADS   //Modification: 2018.01, removing warnings
	unsigned long	prev_count_done   = 0L;
#endif
	long			seconds_from_last = 0L;
	bool			update_needed = false;
	time_t			last_time = 0;
	time_t			current_time = 0;
	string			cntStr;

// These are used for thread behavior
#define		UI_REFRESH_INTERVAL		3		// In seconds, do NOT make less than 2
#define		UI_DUP_COUNT_INTERVAL	20		// Show counts every 20 or every Refresh interval seconds

	int		UI_count_down = UI_DUP_COUNT_INTERVAL;

	unsigned int	num_languages = langs->size();

// Outer LOOP to process various Language classes starting with largest
//
	unsigned int	num_duplicates = 0;
	struct filePtrFlags * pPtrFlags = NULL;

	CmpMngr		myDiffManager;		// Only need 1 instance (per thread) of this class to be created

	int	web_lang_idx = -1;			// not found state
	unsigned int positions_size = 0;

	for ( unsigned int lang_idx = 0; lang_idx < (unsigned)num_languages; lang_idx++ )
	{
		positions_size = (*langs)[ lang_idx ].num_pointers;
		if ( positions_size <= 1 )
			break;		// Too small to have Duplicates, langs is sorted by num_pointers so we are done.

		if ( WEB == (*langs)[ lang_idx ].array_Lang )
			web_lang_idx = (int)lang_idx;

		pPtrFlags = (*langs)[ lang_idx ].pFilePtrs;

// This LOOP will show the first part.  Changed from % done in text mode to updated count
// This updates REALLY slow when doing larger file sets.  That is why counts are used.
//
		time( &last_time );
		current_time = last_time;
		positions_size--;		// decrement so that there is 1 position left for the j iterator
		for ( unsigned int i_index = 0; i_index < positions_size; i_index++ )
		{
			count_done++;
			UI_count_down--;

			if ( pPtrFlags[ i_index ].flags )
			{
				// This position is not valid anymore.  Embedded or Duplicate flags were set here.
				continue;
			}

			bool dup_found = 
			FindDuplicateFor( &myDiffManager, 
							pPtrFlags, 
							positions_size + 1, 
							i_index, 
							err_msgs,
							unc_files,
							myErrList,
							dup_threshold, 
							dupList1, 
							dupList2, 
							checkMatch, 
							useListA,
							userIF,
							clearCaseFile,
							CounterForEachLanguage,
							print_cmplx,
							print_csv,
							mySourceFileA,
							mySourceFileB );

			if ( dup_found )
				num_duplicates++;

			update_needed = false;
			if ( 0 >= UI_count_down )
				update_needed = true;
			else
			{
				time( &current_time );
				seconds_from_last = (long)( difftime( current_time, last_time ) + 0.5 );

				if ( seconds_from_last >= UI_REFRESH_INTERVAL )
					update_needed = true;
			}

			if ( update_needed )
			{
				if ( MAIN_THREAD_INDEX == threadIdx )
				{
				#ifdef	QTGUI
					// Percent changed so update UI
					percent_done = (int)( ( count_done * 100 ) / num_in_list );
					userIF->updateProgress("", false, percent_done, (unsigned int)num_in_list );
				#else
					// Just keep erasing and redrawing a text area with updated count
					LongToStr( (long)count_done, cntStr );
					cout << "\b\b\b\b\b\b\b\b\b\b" << cntStr;		// 10 backspace + new count
					for ( int i = 0 ; i < 10 - (int)cntStr.size(); i++ )
						cout << " ";	// fill at right to be easier next time
					cout << flush;
				#endif
					UI_count_down = UI_DUP_COUNT_INTERVAL;
					last_time = current_time;

					// Check for Cancel
					if ( userIF->isCanceled() )
					{
						FreeLangsHeaders( langs );
						return error_count;
					}
				}
				else
				{
					// Worker thread
				#ifdef	ENABLE_THREADS
					// Save work progress for later display
					int wanted_action = _prv_IncrementWorkDone( threadIdx, 
											(unsigned int)( count_done - prev_count_done ) );

					if ( ( STOP_WORKING == wanted_action )
					  || ( EXIT_NOW     == wanted_action ) )
					{
						FreeLangsHeaders( langs );
						return error_count;	// Don't do any more files.  Get out.
					}
				#endif
					//prev_count_done = count_done; //Modification: 2018.01 ; Not Used
				}
			}
		}
	}	// End looping through various Languages to do


// This LOOP will show the remaining counts (faster than above loop)
//
// update duplicate Embedded files
	if ( web_lang_idx >= 0 )
	{
		positions_size = (*langs)[ (unsigned)web_lang_idx ].num_pointers;
		if ( positions_size <= 1 )
		{
			// Too small to have Duplicates
			web_lang_idx = -1;		// Avoid processing again
		}
	}

	if ( web_lang_idx >= 0 )
	{
		pPtrFlags = (*langs)[ (unsigned)web_lang_idx ].pFilePtrs;
		SourceFileElement * pWeb = NULL;
		SourceFileElement * pEmbed = NULL;

		UI_count_down = UI_DUP_COUNT_INTERVAL;
		time( &last_time );
		current_time = last_time;
		for ( unsigned int web_idx = 0; web_idx < positions_size; web_idx++ )
		{
			count_done++;
			UI_count_down--;

			pWeb = pPtrFlags[ web_idx ].pSrcFile;

			if ( ( pWeb->second.class_type == WEB ) 
			  && ( pWeb->second.duplicate || pWeb->second.firstDuplicate ) )
			{
				for ( unsigned int embed_idx = web_idx + 1; embed_idx < positions_size; embed_idx++ )
				{
					pEmbed = pPtrFlags[ embed_idx ].pSrcFile;
					if ( pEmbed->second.file_name_isEmbedded == true )    // Modified: 2015.12
					{
						if ( pWeb->second.duplicate )
							pEmbed->second.duplicate = true;
						else
							pEmbed->second.firstDuplicate = true;

						count_done++;
						UI_count_down--;
					}
					else
						break;
				}
			}

			update_needed = false;
			if ( 0 >= UI_count_down )
				update_needed = true;
			else
			{
				time( &current_time );
				seconds_from_last = (long)( difftime( current_time, last_time ) + 0.5 );

				if ( seconds_from_last >= UI_REFRESH_INTERVAL )
					update_needed = true;
			}

			if ( update_needed )
			{
				if ( MAIN_THREAD_INDEX == threadIdx )
				{
				#ifdef	QTGUI
					// Percent changed so update UI
					percent_done = (int)( ( count_done * 100 ) / num_in_list );
					userIF->updateProgress("", false, percent_done, (unsigned int)num_in_list );
				#else
					// Just keep erasing and redrawing a text area with updated count
					LongToStr( (long)count_done, cntStr );
					cout << "\b\b\b\b\b\b\b\b\b\b" << cntStr;		// 10 backspace + new count
					for ( int i = 0 ; i < 10 - (int)cntStr.size(); i++ )
						cout << " ";	// fill at right to be easier next time
					cout << flush;
				#endif
					UI_count_down = UI_DUP_COUNT_INTERVAL;
					last_time = current_time;

					// Check for Cancel
					if ( userIF->isCanceled() )
					{
						FreeLangsHeaders( langs );
						return error_count;
					}
				}
				else
				{
					// Worker thread
				#ifdef	ENABLE_THREADS
					// Save work progress for later display
					int wanted_action = _prv_IncrementWorkDone( threadIdx, 
											(unsigned int)( count_done - prev_count_done ) );

					if ( ( STOP_WORKING == wanted_action )
					  || ( EXIT_NOW     == wanted_action ) )
					{
						FreeLangsHeaders( langs );
						return error_count;	// Don't do any more files.  Get out.
					}
				#endif
					//prev_count_done = count_done; //Modification: 2018.01 ; Not Used
				}
			}
		}

		web_lang_idx = -1;		// Avoid processing again

	}	//	END of doing Web 

	if ( MAIN_THREAD_INDEX == threadIdx )
	{
		// Release memory used
		FreeLangsHeaders( langs );
	}
	//Warning fix 11.25.16. DO NOT USE THIS VARIABLE AFTER THIS.
	//(void)prev_count_done;

	return	error_count;
}


/*!
* 1.Function Description:
*	 Searches for duplicates in an array of file pointers of a given Language 
*
* Precondition: 2 or more pointers in array
*
* 2.Parameters:
*    myDiffManager:   pointer to a Compare helper class instance for detailed file compares
*    pPtrFlags:       array of pointers to source files of a given Language
*    numPtrs:         number of pointers in pPtrFlags
*    i_idx:           pPtrFlags i index, j index will be incremented from here
*    dupList1:        first (source) duplicate files list
*    dupList2:        duplicate files list
*    checkMatch:      check for matching files (applies only when differencing)
*    use list A:      Duplicate checking is for Baseline A or B
*
* 3.Creation Time And Owner:
*	 Version: 2011.05
*    Revised: 2015.12  Uses file_size (Physical Line count). Refactored for faster Performance after profiling.
*/
bool FindDuplicateFor( CmpMngr					*	pDiffManager,
						struct filePtrFlags		*	pPtrFlags, 
						const unsigned int			numPtrs, 
						const unsigned int			i_idx,
						ErrMsgStructVector		*	err_msgs,
						UncountedFileStructVector * unc_files,
						string					*	myErrList,
						const double				dup_threshold, 
						StringVector			*	dupList1, 
						StringVector			*	dupList2, 
						const bool					checkMatch, 
						const bool					useListA,
						UserIF					*	userIF,
						const bool					clearCaseFile,
						CounterForEachLangType	&	CounterForEachLanguage,
						const bool					print_cmplx,
						const bool					print_csv,
						SourceFileList			*	mySourceFileA,
						SourceFileList			*	mySourceFileB )
{
	vector<pair<SourceFileElement*, SourceFileElement*> > matchedFiles;			// Modification: 2011.05
	vector<pair<SourceFileElement*, SourceFileElement*> >::iterator iMatchEnd;

	vector<SourceFileElement*> dup_files_found;		// Used to release RAM before return

	SourceFileElement* nullElement = NULL;

	unsigned int	load_count = 0;


	size_t dupCnt = 0;
	bool foundDup = false, recDup = true, filesMatched;
	int filenameMatched;

	size_t sizeF1, sizeF2;
	unsigned int	changed_lines, total_lines;
	double			pctcheck, pct_change;
	bool found;
	srcLineVector emptyFile;

//      Modified: 2015.12
	SourceFileElement *	i = pPtrFlags[ i_idx ].pSrcFile;
	SourceFileElement *	j = NULL;
	
//Modification: 2018.01 ; Not Used	
//      bool	myCheckMatch = checkMatch;	// WARNING myCheckMatch gets assigned to below ? Possible logic ERROR ?

	// Set up instances on local stack of values that do NOT change in the LOOP
	// Migrate invariant expressions outside or simplify expressions within a loop.
	// Anything related to i
	string			i_second_file_name_only      = i->second.file_name_only;
	unsigned int	i_second_file_name_only_size = i->second.file_name_only.size();
	unsigned int	i_first_size                 = i->first.size();
	int				i_second_file_type           = i->second.file_type;
	unsigned int	i_second_file_size           = i->second.file_size;	// Number of Physical lines in the file
	streamoff		i_second_file_size_bytes     = i->second.file_size_bytes;
	ClassType		i_second_class_type          = i->second.class_type;
	bool			isWeb = false;
	if ( WEB == i_second_class_type )
		isWeb = true;

	// File extensions must match even if file names do not
	string i_file_extension, j_file_extension;
	size_t idx = i_second_file_name_only.find_last_of( "." );
	if ( idx != string::npos )
		i_file_extension = i_second_file_name_only.substr( idx );

	vector<lineElement>::iterator i_first_begin  = i->first.begin();
	vector<lineElement>::iterator i_first_end    = i->first.end();

	// Values that are set 1 time in the LOOP and used 2 or more times
	unsigned int	j_first_size = 0;
	unsigned int	j_second_file_size = 0;
	vector<lineElement>::iterator j_first_end;


//	LOOP	Outer loop to look for Duplicates
	unsigned int	j_idx = 0;
//	unsigned int	startPos_size = startSize;
	unsigned int	flags;

	for ( j_idx = i_idx + 1; j_idx < numPtrs; j_idx++ )
	{
		// START Precondition checks; see also FindDuplicateFiles
		flags = pPtrFlags[ j_idx ].flags;
		if ( flags )
			continue;	// Either Embedded or a Duplicate

		j = pPtrFlags[ j_idx ].pSrcFile;

		// Only call compare if sizes are same
		filenameMatched = 2;	// Start with file names NOT matched 

		// For exact match, checking string Size before string Compare is faster (profiling)
		if ( i_second_file_name_only_size == (*j).second.file_name_only.size() )
			filenameMatched = i_second_file_name_only.compare( j->second.file_name_only );

		j_first_size       = (*j).first.size();
		j_second_file_size = (*j).second.file_size;
		if ( i_second_file_size != j_second_file_size &&	// Physical lines in files, WEB files have buffers elsewhere
			filenameMatched != 0 )
		{
			// two files have different number of lines and different filename
			continue;
		}
		// END	Precondition checks

		// if files have same name, do a diff and mark as duplicates if logical SLOC change % is below threshold
		filesMatched = false;
		if (filenameMatched == 0 && (i_second_file_type != DATA || (i_second_file_size < 1 && j_second_file_size < 1)))		// Modification: 2011.10  2015.12
		{
			// match empty files with same name
			if (i_second_file_size < 1 && j_second_file_size < 1)
				filesMatched = true;
			else
			{
				// each source file elements results object has a mySLOCLines object with the SLOC to be diffed
				changed_lines = total_lines = 0;
				pct_change = 0.0;
				sizeF1 = 0;
				sizeF2 = 0;

				// for web languages, diff each of the embedded files
				if ( isWeb )
				{
					// find all matches for i embedded files in j
					SourceFileElement * i1 = i;
					SourceFileElement * j1 = j;
					unsigned int i1_idx = i_idx;
					unsigned int j1_idx = j_idx;
					for ( i1_idx++; i1_idx < numPtrs; i1_idx++ )
					{
						i1 = pPtrFlags[ i1_idx ].pSrcFile;
						if ( i1->second.file_name_isEmbedded == false )    // Modified: 2015.12
							break;		// No more to check
						
						found = false;
						for ( j1_idx++; j1_idx < numPtrs; j1_idx++ )
						{
							j1 = pPtrFlags[ j1_idx ].pSrcFile;
							if ( j1->second.file_name_isEmbedded == false )    // Modified: 20115.12
								break;		// No more to check
							
							if (i1->second.file_name_only.compare(j1->second.file_name_only) == 0)
							{
								found = true;
								matchedFiles.push_back(make_pair(&(*i1), &(*j1)));
								iMatchEnd = matchedFiles.end();
								sizeF1 += i1->second.mySLOCLines.size();
								sizeF2 += j1->second.mySLOCLines.size();
							}
						}
						if (!found)
						{
							sizeF1 += i1->second.mySLOCLines.size();
							matchedFiles.push_back(make_pair(&(*i1), nullElement));
							iMatchEnd = matchedFiles.end();
						}
					}

					// find all unmatched j embedded files
					j1_idx = j_idx;
					for ( j1_idx++; j1_idx < numPtrs; j1_idx++ )
					{
						j1 = pPtrFlags[ j1_idx ].pSrcFile;
						if ( j1->second.file_name_isEmbedded == false )    // Modified: 2015.12
							break;		// No more to check

						found = false;
						i1_idx = i_idx;
						for ( i1_idx++; i1_idx < numPtrs; i1_idx++ )
						{
							i1 = pPtrFlags[ i1_idx ].pSrcFile;
							if ( i1->second.file_name_isEmbedded == false )    // Modified: 2015.12
								break;		// No more to check

							if ( i1->second.class_type != j1->second.class_type )
								continue;	// No Duplicate checking if not same file class

							if (i1->second.file_name_only.compare(j1->second.file_name_only) == 0) 
							{
								found = true;
								break;
							}
						}
						if (!found)
						{
							sizeF2 += j1->second.mySLOCLines.size();
							matchedFiles.push_back(make_pair(nullElement, &(*j1)));
							iMatchEnd = matchedFiles.end();
						}
					}
					
					if (sizeF1 > sizeF2)
						pctcheck = 100 * (double)(sizeF1 - sizeF2) / sizeF1;
					else
						pctcheck = 100 * (double)(sizeF2 - sizeF1) / sizeF2;

					// perform comparison only if the change percent (pctcheck) is not greater than threshold
					if ( pctcheck <= dup_threshold )
					{
						vector<pair<SourceFileElement*, SourceFileElement*> >::iterator ii = matchedFiles.begin();
						while ( ii != iMatchEnd )
						{
							if (ii->first == nullElement)
							{								
								// don't need to compare the empty file to compute the information
								changed_lines += ii->second->second.mySLOCLines.size(); // all lines deleted
							} 
							else if (ii->second == nullElement)
							{
								// don't need to compare the empty file to compute the information
								changed_lines += ii->first->second.mySLOCLines.size();
								total_lines += ii->first->second.mySLOCLines.size();								
							} 
							else
								CompareForDuplicate( &(ii->first->second.mySLOCLines), &(ii->second->second.mySLOCLines), changed_lines, total_lines, pDiffManager );

							ii++;
						}
					}
					else
						continue;
				}
				else
				{
					// only compare if the chance of duplicates is high
					sizeF1 = (*i).second.mySLOCLines.size();
					sizeF2 = (*j).second.mySLOCLines.size();
					if (sizeF1 > sizeF2)
						pctcheck = 100.0 * (double)(sizeF1 - sizeF2) / (double)sizeF1;
					else
						pctcheck = 100.0 * (double)(sizeF2 - sizeF1) / (double)sizeF2;

					// perform comparison only if the change percent (pctcheck) is not greater than threshold
					if (pctcheck <= dup_threshold)
						CompareForDuplicate( &((*i).second.mySLOCLines), &((*j).second.mySLOCLines), changed_lines, total_lines, pDiffManager ); 
					else
						continue;
				}

				if ( changed_lines )
					pct_change =  100.0 * (double)changed_lines / (double)total_lines;

				if ( pct_change <= dup_threshold )
					filesMatched = true;
			}
		}
		else
		{
			// File names are different, compare Physical lines for identical duplicate
			// File extension should be the same
			if ( MAKEFILE != i_second_class_type )	// Make files can have .mak or . as extension
			{
				idx = (*j).second.file_name_only.find_last_of( "." );
				if ( idx != string::npos )
					j_file_extension = (*j).second.file_name_only.substr( idx );
				else
					j_file_extension = "";

				// Better than using class type as both .HTML and .XML are Web class but can not be Duplicates
				if ( i_file_extension.compare( j_file_extension ) )
					continue;
			}

			// Check file size in Bytes
			long bytes_size_diff = labs( (long)(i_second_file_size_bytes - (*j).second.file_size_bytes) );
			if ( bytes_size_diff > 64L )	// allow only a little difference in byte sizes
				continue;	// Not same file size

			// Check number of Physical text lines
			if ( ( i_second_file_size != j_second_file_size )
			  || ( ( i_second_file_size < 1 ) || ( j_second_file_size < 1 ) ) )
			{
				// don't match files with different Physical line counts or empty files with different names
				continue;
			}

			// See if need to Read the Physical lines, no need to do processing after Read
			if ( 0 == i_first_size )
			{
				load_count++;
				int errors = 
					ReadFileLines( &(*i), MAIN_THREAD_INDEX, userIF, 
								true,					// read Web files
								true,					// only read Physical lines
								clearCaseFile, 
								CounterForEachLanguage, print_cmplx, print_csv, 
								useListA,
								mySourceFileA, 
								mySourceFileB,
								*myErrList,
								outDir,
								*err_msgs,
								*unc_files,
								false,			// process_after_read
											// These will be discarded as needed after DUP
								false,				// discard_PHY_lines_after_process
								false,				// discard_lines_after_process
								true );				// no_warnings_to_UI
				if ( errors )
				{
					continue;
				}
			}

			if ( 0 == j_first_size )
			{
				load_count++;
				int errors = 
					ReadFileLines( &(*j), MAIN_THREAD_INDEX, userIF, 
								true,					// read Web files
								true,					// only read Physical lines
								clearCaseFile, 
								CounterForEachLanguage, print_cmplx, print_csv, 
								useListA,
								mySourceFileA, 
								mySourceFileB,
								*myErrList,
								outDir,
								*err_msgs,
								*unc_files,
								false,			// process_after_read
											// These will be discarded as needed after DUP
								false,				// discard_PHY_lines_after_process
								false,				// discard_lines_after_process
								true );				// no_warnings_to_UI
				if ( errors )
				{
					continue;
				}
			}

			// Get values from just loaded Physical lines
			i_first_begin = (*i).first.begin();
			i_first_end   = (*i).first.end();
			i_first_size  = (*i).first.size();
			j_first_size  = (*j).first.size();

			// note: two files have the same number of lines
			vector<lineElement>::iterator baseLine = i_first_begin;
			vector<lineElement>::iterator compareLine = (*j).first.begin();
			j_first_end = (*j).first.end();
			while ( baseLine != i_first_end && compareLine != j_first_end )
			{
				if ((*baseLine).line.compare((*compareLine).line) != 0)
					break;
				baseLine++;
				compareLine++;
			}
			if ( baseLine == i_first_end && compareLine == j_first_end )
				filesMatched = true;
		}	// END section File names are different

		if (filesMatched)
		{
			// check whether a comparison match exists
			recDup = true;
			if (checkMatch)
			{
				/*if ((*i).second.matched)
					myCheckMatch = false; *///Modification: 2018.01 ; Not Used
				if ((*j).second.matched)
				{
					// change previously set first duplicate (if necessary)
					if (foundDup)
					{
						(*i).second.firstDuplicate = false;
						size_t dupList1_size = dupList1->size();
						for (size_t n = dupList1_size - dupCnt; n < dupList1_size; n++)
							(*dupList1)[n] = (*j).second.file_name;
					}

					// switch first duplicate for one with a match
					recDup = false;
					//myCheckMatch = false; //Modification: 2018.01 ; Not Used
					(*j).second.firstDuplicate = true;

					// Update j flags
					pPtrFlags[ j_idx ].flags |= IS_FIRST_DUP;

					(*i).second.duplicate = true;		// No need to update i_idx ].flags as will not be visited
					dupList1->push_back((*j).second.file_name);

					// Save pointer to struct for later RAM release
					dup_files_found.push_back( &(*j) );

					// No longer valid Start position
				//	pStartPos[ j_idx ] = 0;
				
					dupList2->push_back((*i).second.file_name);
					dupCnt++;
					i = j;
				}
			}

			if (recDup)
			{
				// add pair to duplicate list
				(*i).second.firstDuplicate = true;
			//	i_flags |= IS_FIRST_DUP;			// Not going to change i_flags as will never visit it again
				(*j).second.duplicate = true;

				// Update j flags
				pPtrFlags[ j_idx ].flags |= IS_DUP;

				dupList1->push_back((*i).second.file_name);
				dupList2->push_back((*j).second.file_name);

				// Save pointer to struct for later RAM release
				dup_files_found.push_back( &(*j) );

				// No longer valid Start position
			//	pStartPos[ j_idx ] = 0;

				dupCnt++;
			}
			foundDup = true;
		}
	}

	// Better to release RAM in reverse order of allocation
	//
	// Go through list of found Duplicates with changed firstDuplicate or duplicate flags
	// Release each as no longer need for detailed comarison of Physical or Logical source lines
	vector<SourceFileElement*>::iterator itDupEnd = dup_files_found.end();
	for ( vector<SourceFileElement*>::iterator it = dup_files_found.begin(); it != itDupEnd; it++ )
	{
		(*it)->second.mySLOCLines.clear();
		(*it)->first.resize( 0 );
	}

	// Release RAM buffers for  i
	(*i).second.mySLOCLines.clear();	// Empty the vector of Logical source lines
	(*i).first.resize( 0 );

	if ( load_count )
	{
#ifdef	_DEBUG
		// cout << endl << "   Files loaded: " << load_count << endl << flush;
#endif
	}
    //Warning fix 11.25.16. DO NOT USE THIS VARIABLE AFTER THIS.
    //(void)myCheckMatch;

	return foundDup;
}

/*!
* 1.Function Description:
*	 Compares two files and adds to the changed/total lines for Duplicate processing.
*
* 2.Parameters:
*    firstFile: first file to compare
*    secondFile: second file to compare
*    changedLines: accumulated number of changed lines
*    totalLines: accumulated total number of lines
*
* 3.Creation Time And Owner:
*	 Version: 2011.05
*    Revised: 2015.12  Moved to support multiple threads
*/
void CompareForDuplicate(srcLineVector * firstFile, srcLineVector * secondFile,
						unsigned int &changedLines, unsigned int &totalLines,
						CmpMngr * pDiffManager )
{
	srcLineVector ff = *firstFile;
	srcLineVector sf = *secondFile;

	pDiffManager->Compare( &ff, &sf, DIFF_MATCH_THRESHOLD );
	changedLines += pDiffManager->nAddedLines + pDiffManager->nDeletedLines + pDiffManager->nChangedLines;
	totalLines += pDiffManager->nDeletedLines + pDiffManager->nChangedLines + pDiffManager->nNochangedLines;
}




#ifdef	ENABLE_THREADS
// Helper to start Threads processing Diff Pairs in parallel
// This is called by the Main thread.
//
int DiffPairsThreads( DiffTool				*	pDiff,
					UserIF					*	userIF,
					const	bool				print_cmplx,
					const	bool				print_csv,
					const	bool				clearCaseFile,
					string						outDir,
					const	bool				discard_PHY_lines_after_process,
					const	bool				discard_lines_after_process,
					const	bool				no_warnings_to_UI )
{
	int
	error_count = _prv_ListToThreads( false,			// DIFF list flag
								(unsigned long)pDiff->matchedFilesList.size(),
								false,					// noRead
								false,					// noWeb
								userIF,
								print_cmplx,
								print_csv,
								NULL,					// File list to divide
								pDiff,					// DiffTool data members: Pairs list, diff & dup counters...
								true,					// useListA  no matter for DIFF
								clearCaseFile,
								outDir,
								true,					// process_after_read
								discard_PHY_lines_after_process,
								discard_lines_after_process,
								no_warnings_to_UI,
								false );				//no_uncounted 

	return	error_count;
}


// Start Threads processing Duplicate checking for more than 1 Language in parallel
// This is called by the Main thread.
//
// Partition number of files to do (using number of pointers to the file structs)
// among the available threads.
//
// Preconditions:
// Availble threads ready and blocked waiting for work.
// Languages sorted with Most files to do first.
// Number of files >= 2 per Language.
//
void DupCheckingThreads( vector<struct langTypeArrayHeader> * langs,
						const double				dup_threshold, 
						const bool					checkMatch, 
						const bool					useListA,
						UserIF					*	userIF,
						const bool					clearCaseFile,
						const bool					print_cmplx,
						const bool					print_csv )
{
	unsigned int	threads_used = 0;
	unsigned int	work_count[ MAX_UCC_THREAD_COUNT ];
	memset( work_count, 0, sizeof( work_count ) );
	unsigned int	smallest_count = 0;
	unsigned int	smallest_index = 0;

	// Assign work items to each available thread.
	// Try to keep from overloading any single thread
	for ( unsigned int j = 0; j < langs->size(); j++ )
	{
		// Find thread with Smallest count to do
		smallest_count = work_count[ 0 ];
		smallest_index = 0;
		for ( unsigned int k = 0; k < g_threads_status_in_use; k++ )
		{
			if ( work_count[ k ] < smallest_count )
			{
				smallest_count = work_count[ k ];
				smallest_index = k;
			}
		}

		work_count[ smallest_index ] += (*langs)[ j ].num_pointers;

		// Copy to thread struct
		g_threads_status[ smallest_index ].langs.push_back( (*langs)[ j ] );

		if ( threads_used < g_threads_status_in_use )
		{
			if ( g_assigned_count < g_threads_status_in_use )
				g_assigned_count++;
			threads_used++;
		}
	}

	// Start the threads working
	// No need to set characteristics for other Thread jobs (Read/Process or Diff)
	// As thread procedure will check for this type first
	for ( unsigned int j = 0; j < threads_used; j++ )
	{
		g_threads_status[ j ].dup_threshold = dup_threshold;
		g_threads_status[ j ].dupList1      = new StringVector;
		g_threads_status[ j ].dupList2      = new StringVector;
		g_threads_status[ j ].check_match   = checkMatch;
		g_threads_status[ j ].useListA      = useListA;
		g_threads_status[ j ].clearCaseFile = clearCaseFile;
		g_threads_status[ j ].print_cmplx   = print_cmplx;
		g_threads_status[ j ].print_csv     = print_csv;
		g_threads_status[ j ].userIF        = userIF;	// thread checks this for != NULL
		g_threads_status[ j ].main_WantsAction = GET_TO_WORK;

		// Toggle semaphore to wake thread and then yield remaining time slice
		g_threads_status[ j ].pSem->signal();
#ifdef  QTGUI
//		g_threads_status[ j ].pQSem->release();
		QThread::yieldCurrentThread();
#else
		boost::this_thread::yield();	// Allow assigned thread (and maybe others) to run
#endif
	}

	return;
}
#endif		//	ENABLE_THREADS

/*!
* 1.Function Description:
*	 Given a list of Files, Open and Read each file in the list into memory.
*    Close file and then may do other processing AFTER each file is Read.
*    May release memory used to store file source lines after processing.
*    Updates percent done on UI as this step runs.
*    Return number of Errors found.
*    Safe to call this from multiple threads.
*    Refactored extensively to:
*        support thread extra args, error handling, ease of reading the code!
*    So consider ALL this procedure 2015.12 even though kept older where reasonable.
*
* 2.Parameters:
*    threadIdx               IN      idex of calling Thread.
*    userIF
*    CounterForEachLanguage  IN/OUT  reference to struct with pointers to Language class instances
*
*    inputFileVector: list of files to count (may be empty if inputFileList is given)
*    inputFileList: file containing list of files to count (if any)
*    useListA: use file list A? (otherwise use list B)
*
*    Returns	number of Errors found
*
* 3.Creation Time And Owner:
*	 Version: 2009.01  Created as last part of ReadAllFiles (after file list built).
*    Revised: 2015.12  Moved here out of ReadAllFiles and extensive refactoring.
*/
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
					string						&		myErrList,
					string								outDir,
					ErrMsgStructVector			&		err_msgs,
					UncountedFileStructVector	&		unc_files,
					const		bool					process_after_read,
					const		bool					discard_PHY_lines_after_process,
					const		bool					discard_lines_after_process,
					const		bool					no_warnings_to_UI,
					const		bool					no_uncounted )
{
	int				error_count = 0;	// return value
	CCodeCounter *	pCounter = NULL;

	filemap		fmap;
	results		r;

//	bool			OK_to_process = false; //Modification: 2018.01 ; Not Used
//	string			oneline;

	ClassType	fileclass;

	string		fileName, clearCaseCroppedFile;

// LOOP through the given file list
	//bool			readThisFile = ! noRead; //Modification: 2018.01 ; Not Used
	bool			readWebFiles = ! noWeb;
	bool			skip_file_process = false;
	bool			time_based_update = false;
	time_t			last_time, current_time;
	long			seconds_from_last = 0L;
	string			cntStr;
	if ( true == noRead )
		readWebFiles = true;

#define		UI_READ_DONE_AMOUNT		20
#define		UI_REFRESH_INTERVAL		3		// Do NOT make less than 2
	unsigned long	count_done = 0;
#ifdef          ENABLE_THREADS   //Modification: 2018.01, removing warnings
	unsigned long	prev_count_done = 0;
#endif
	unsigned long	num_in_loop = (unsigned long)distance( itStart, itEnd );

	// Percent approach: Only need to update when percent changes so will do twice as often
	int		inc_amount = (int)( num_in_loop / 200L );
	if ( 0 == inc_amount )
		inc_amount = 1;		// There are less than 200 to do so Percent approach is OK

	if ( UI_READ_DONE_AMOUNT < inc_amount )
	{
		// There are more than  200 x UI_READ_DONE_AMOUNT to do.
		time_based_update = true;
		inc_amount = UI_READ_DONE_AMOUNT;
	}
	int		UI_count_down = inc_amount;

	// loop through all the listed or extracted file names
	std::vector<std::string>::iterator itVectorData = itStart;
	time( &last_time );
	current_time = last_time;
	for ( ; itVectorData != itEnd;  itVectorData++ )
	{
		count_done++;
		UI_count_down--;
		skip_file_process = false;

		//OK_to_process = false; //Modification: 2018.01 ; Not Used
		//readThisFile = ! noRead; //Modification: 2018.01 ; Not Used
		fileName = *(itVectorData);
		fileName = CUtil::TrimString( fileName );
		r.file_name = fileName;
		if ( clearCaseFile )
		{
			// handle the @@ from ClearCase
			clearCaseCroppedFile = CUtil::ConvertClearCaseFile( fileName, r.clearCaseTrailer );
			
			// if the filename was modified by ClearCase, use the cropped name
			r.file_name = clearCaseCroppedFile;
		}
		fileclass = DecideLanguage( CounterForEachLanguage, &(pCounter), print_cmplx, r.file_name, false );
		if (fileclass == UNKNOWN)
		{
			_prv_SaveOrWriteUncountedFile( threadIdx, userIF, unc_files, "Unknown File Extension", fileName, useListA, print_csv, outDir, no_uncounted );
			
			// See if UI needs an update
			if ( time_based_update )
			{
				time( &current_time );
				seconds_from_last = (long)( difftime( current_time, last_time ) + 0.5 );

				if ( seconds_from_last < UI_REFRESH_INTERVAL )
					continue;
			}
			else
			{
				if ( 0 < UI_count_down )
					continue;
			}

			skip_file_process = true;	// Skip to get to UI update
		}
		
		if ( false == skip_file_process )
		{
			// Save so if Errors later file is properly handled
			r.class_type = fileclass;
			r.file_type = (pCounter->classtype == UNKNOWN || pCounter->classtype == DATAFILE) ? DATA : CODE;
			r.e_flag = false;		// Modification: 2011.05
			r.error_code = "";		// Modification: 2009.01
			r.file_size_bytes = 0;  // Modification: 2015.12
			fmap.resize( 0 );

			// Call thread safe helper to read the file if wanted
			ReadSrcFile( threadIdx, error_count, pCounter, fmap, r, noRead, readWebFiles, 
						false,				// onlyPhysical
						userIF,
						CounterForEachLanguage, print_cmplx, print_csv, useListA,
						fileName, clearCaseCroppedFile,
						clearCaseFile, 
						NULL,				// not using a single SourceFileElement
						mySrcFileList, mySourceFileA, mySourceFileB, myErrList,
						outDir, err_msgs, unc_files, 
						process_after_read, 
						discard_PHY_lines_after_process,
						discard_lines_after_process,
						no_warnings_to_UI );
		}

		// See if need to Update UI or if User has Cancelled
		bool	update_needed = skip_file_process;
		if ( ! update_needed )
		{
			// See if UI needs an update
			if ( time_based_update )
			{
				time( &current_time );
				seconds_from_last = (long)( difftime( current_time, last_time ) + 0.5 );

				if ( seconds_from_last >= UI_REFRESH_INTERVAL )
					update_needed = true;
			}
			else
			{
				if ( 0 >= UI_count_down )
					update_needed = true;
			}
		}

		if ( threadIdx != MAIN_THREAD_INDEX )
		{
			if ( update_needed )
			{
			#ifdef		ENABLE_THREADS
				// Save work progress for later display
				int wanted_action = _prv_IncrementWorkDone( threadIdx, 
										(unsigned int)(count_done - prev_count_done ) );

				if ( ( STOP_WORKING == wanted_action )
				  || ( EXIT_NOW     == wanted_action ) )
				{
					break;	// Don't do any more files.  Get out.
				}
			#endif
				UI_count_down = inc_amount;
				//prev_count_done = count_done; //Modification: 2018.01. Not Used.
				last_time = current_time;
			}
		}
		else
		{
			// Running from Main thread, update UI if needed
			if ( update_needed )
			{
			#ifdef	QTGUI
				// Percent changed so update UI
				int percent_done = (int)( ( count_done * 100 ) / num_in_loop );
				userIF->updateProgress( "", false, percent_done, num_in_loop );
			#else
				// Just keep erasing and redrawing a text area with updated count
				LongToStr( (long)count_done, cntStr );
				cout << "\b\b\b\b\b\b\b\b\b\b" << cntStr;
				for ( int i = 0 ; i < 10 - (int)cntStr.size(); i++ )
					cout << " ";		// fill at right to be easier next time
				cout << flush;			// Needed otherwise won't display for Release build !
			#endif
				UI_count_down = inc_amount;
				//prev_count_done = count_done; //Modification: 2018.01 ; Not Used
				last_time = current_time;
			}
			// processing Canceled ?
			if ( userIF != NULL )
			{
				bool has_cancelled = userIF->isCanceled();
				if ( has_cancelled )
				{
					break;	// Stop Looping and return
				}
			}
		}
	}		//	END		LOOP through the given file list

    //Warning fix 11.25.16. DO NOT USE THIS VARIABLE AFTER THIS.
  //  (void) OK_to_process;
    //Warning fix 11.25.16. DO NOT USE THIS VARIABLE AFTER THIS.
   // (void) readThisFile;
	//Warning fix 11.25.16.DO NOT USE THIS VARIABLE AFTER THIS.
	//(void) prev_count_done;

	return error_count;
}


/*!
* 1.Function Description:
*	 Process a file from source file list and call relevant counter for each file.
*    Analyze and do Counts for a single Source File.
*    Safe to call this from multiple threads.
*
* 2.Parameters: (see below)
*
* 3.Creation Time And Owner:
*	 Version: 2009.01  Created as code inside loop of ProcessSourceList
*    Revised: 2015.12  Moved here to share with thread helpers
*/
void ProcessSourceListFile( const unsigned int threadIdx, UserIF * userIF, 
							CounterForEachLangType & CounterForEachLanguage,
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
							const bool						useListA )
{
	CCodeCounter	*	pCounter = NULL;
	*ppCounter = pCounter;

	size_t				embFile = string::npos;

	// Exceptions within this File Analysis & Counter upper level Caller
	unsigned int	analyze_exception_type = EXCEPTION_DID_NOT_HAPPEN;
	string			analyze_exception_msg;
    
    // 2016   Save  Path and File  in case a Signal happens		
	if ( MAIN_THREAD_INDEX == threadIdx )		
		main_parse_path_file = parse_file_name;		
#ifdef		ENABLE_THREADS		
	else		
	{		
		if ( threadIdx < g_threads_status_in_use )		
		{		
			g_threads_status[ threadIdx ].current_path_file = parse_file_name;		
		}		
		else		
		{		
			// Possible Invalid Thread Index given as input. May be due to thread start/stop operations		
			// Not considered a Defect here unless there is more evidence		
			main_parse_path_file = "?";		
		}		
	}		
#endif		// END  2016   Save  Path and File  in case a Signal happens		
    
    
	// handle any exception that may occur un-handled in the counting functions
	try     // Modification: 2011.05
	{
		embFile = i->second.file_name.find( EMBEDDED_FILE_PREFIX );	// Get position for later use

/*  COMMENTED OUT  Replaced with Stack Dump feature instead

		// create output file for trace exception

		if ( traceException )
		{
			extern string outDirectory; // directory for output_traceFile.txt

			outDirectory = outDir + "outfile_traceFile.txt";
			FILE* pFile = fopen( outDirectory.c_str(), "w" );
			// log the file name
			string temp = "Last file processed: " + i->second.file_name + "\n*******************************\n";
			fprintf( pFile,"%s", temp.c_str() );
			temp = "Line No, Function Name, Line Being Processed\n*******************************\n";
			fprintf( pFile,"%s", temp.c_str() );
			fclose( pFile );
		} 
********************/

		// examines the file and sets counter to the correct language counter
		DecideLanguage( CounterForEachLanguage, &(pCounter), print_cmplx, i->second.file_name );
		*ppCounter = pCounter;

		// Set values needed (if it happens) for a Stack Dump    Modification: 2015.12
		pCounter->parse_file_name = parse_file_name;
		pCounter->parse_threadIdx = threadIdx;

		i->second.file_type = (pCounter->classtype == UNKNOWN || pCounter->classtype == DATAFILE) ? DATA : CODE;
		i->second.class_type = pCounter->classtype;

		// Clear the metrics as they will get rebuilt.
		i->second.resetMetrics();

		// Parse and Analyze the details in source lines of the file
		pCounter->CountSLOC( &(i->first), &(i->second) );  // Modification: 2011.10

		if (i->second.trunc_lines > 0)
		{
			stringstream ss;
			ss << "Warning: Truncated ";
			ss << i->second.trunc_lines;
			ss << " line(s) in file (";
			if (embFile != string::npos)
				ss << i->second.file_name.substr(0, embFile);
			else
				ss << i->second.file_name;
			ss << ")";
			string err = ss.str();
			errList += err + "\n";
			_prv_SaveOrAddError( threadIdx, userIF, err_msgs, err, true );	// Log only
		}

		// if webcounter, insert the separation file into list
		if (pCounter->classtype == WEB)
		{
			SourceFileList::iterator pos = i;

			pos++;				// This sometimes gives an invalid iterator.

			// Make sure pos is valid
			/*
			if ( useListA )
			{
				if ( pos == mySourceFileA->end() )
					pos--;
			}
			else
			{
				if ( pos == mySourceFileB->end() )
					pos--;
			}
			*/
	
			map<int, SourceFileElement>* smap = ((CWebCounter*)pCounter)->GetSeparationMap();
			for (map<int, SourceFileElement>::iterator iter = smap->begin(); iter != smap->end(); iter++)   // Modification: 2013.04
			{
				if (iter->second.first.size() > 0)
				{
					// Set Embedded flag when inserting a file into the List of Files
					// This helps later processing avoid calling find() to see if this is an Embedded file 
					// Set File name only string to help later processing
					size_t tempi = iter->second.second.file_name.find( EMBEDDED_FILE_PREFIX );
					if ( tempi != string::npos )
					{
						iter->second.second.file_name_isEmbedded = true;
						iter->second.second.file_name_only = CUtil::ExtractFilename( iter->second.second.file_name.substr( 0, tempi ) );
					}
					else
					{
						iter->second.second.file_name_isEmbedded = false;
						iter->second.second.file_name_only = CUtil::ExtractFilename( iter->second.second.file_name );
					}

					if ( useListA )
						mySourceFileA->insert( pos, iter->second );
					else
						mySourceFileB->insert( pos, iter->second );

					num_inserted++;
				}
			}
		}
	}
	catch(const std::overflow_error& e) 
	{
		// this executes if above throws std::overflow_error (same type rule)
		analyze_exception_type = EXCEPTION_OVERFLOW_ERROR;
		analyze_exception_msg = e.what();
	}
	catch(const std::underflow_error& e)
	{
		// this executes if above throws std::underflow_error (base class rule)
		analyze_exception_type = EXCEPTION_UNDERFLOW_ERROR;
		analyze_exception_msg = e.what();
	}
	catch(const std::range_error& e)
	{
		// this executes if above throws std::range_error
		analyze_exception_type = EXCEPTION_RANGE_ERROR;
		analyze_exception_msg = e.what();
	}
#ifdef CLAUSE_SYSTEM_ERROR_OK
		catch(const std::system_error& e)
		{
			// this executes if above throws std::system_error
			analyze_exception_type = EXCEPTION_SYSTEM_ERROR;
			analyze_exception_msg = e.what();
		}
#endif
	catch(const std::runtime_error& e) 
	{
		// this executes if above throws std::runtime_error (base class rule)
		analyze_exception_type = EXCEPTION_RUNTIME_ERROR;
		analyze_exception_msg = e.what();
	} 
	catch(const std::domain_error& e) 
	{
		// this executes if above throws std::domain_error (base class rule)
		analyze_exception_type = EXCEPTION_DOMAIN_ERROR;
		analyze_exception_msg = e.what();
	} 
	catch(const std::length_error& e) 
	{
		// this executes if above throws std::length_error (base class rule)
		analyze_exception_type = EXCEPTION_LENGTH_ERROR;
		analyze_exception_msg = e.what();
	} 	
	catch(const std::invalid_argument& e) 
	{
		// this executes if above throws std::invalid_argument (base class rule)
		analyze_exception_type = EXCEPTION_INVALID_ARG_ERROR;
		analyze_exception_msg = e.what();
	} 		
	catch(const std::out_of_range& e) 
	{
		// this executes if above throws std::out_of_range (base class rule)
		analyze_exception_type = EXCEPTION_OUT_OF_RANGE_ERROR;
		analyze_exception_msg = e.what();
	} 
	catch(const std::logic_error& e) 
	{
		// this executes if above throws std::logic_error (base class rule)
		analyze_exception_type = EXCEPTION_LOGIC_ERROR;
		analyze_exception_msg = e.what();
	} 
	catch(const std::bad_alloc& e)
	{										// LOW on RAM memory ! ! !
		// this executes if above throws std::bad_alloc (base class rule)
		analyze_exception_type = EXCEPTION_BAD_ALLOC;
		analyze_exception_msg = e.what();
		FirstExceptionMessage( EXCEPTION_HANDLER_PARSER, threadIdx, analyze_exception_type, analyze_exception_msg, pCounter );
	}
	catch(const std::exception& e) 
	{
		// this executes if above throws std::exception (base class rule)
		analyze_exception_type = EXCEPTION_STD_EXCEPTION;
		analyze_exception_msg = e.what();
		if ( analyze_exception_msg.compare( "bad allocation" ) == 0 )
		{
			// LOW on RAM memory ! ! !
			analyze_exception_type = EXCEPTION_BAD_ALLOC;
			FirstExceptionMessage( EXCEPTION_HANDLER_PARSER, threadIdx, analyze_exception_type, analyze_exception_msg, pCounter );
		}	
	} 
	catch(...) // Modification: 2011.05
	{
		// this executes if above throws std::string or int or any other unrelated type
		// Some kind of severe error that prevented normal finish
		analyze_exception_type = EXCEPTION_NOT_SPECIFIC;
		analyze_exception_msg = " Error: a general exception occurred.";
	}

	if ( EXCEPTION_DID_NOT_HAPPEN != analyze_exception_type )
	{
		// Set values formatted by StackDump
		if ( pCounter != NULL )
		{
			// Get the Physical & Logical lines that were most recent
			if ( ( pCounter->currentPhyLine >= (*i).first.size() )
			  && ( (*i).first.size() > 0 ) )
				pCounter->currentPhyLine = (*i).first.size() - 1;

			if ( pCounter->currentPhyLine < (*i).first.size() )
			{
				pCounter->parse_physical_line = (*i).first[ pCounter->currentPhyLine - 1 ].line;
				
				// TODO: Fill in with intact comments if wanted
				string tempNum;
				LongToStr( (long)pCounter->currentPhyLine, tempNum );
				pCounter->parse_physical_line_comments = "How to get the full comments info from (*i).first ?   Physical line number is: " + tempNum;
			}

			if ( pCounter->currentLSrcLine < (*i).second.mySLOCLines.size() )
			{
				// TODO: Fill in with Logical source line
				// pCounter->parse_logical_line = (*i).second.mySLOCLines.begin();
				string tempNum;
				LongToStr( (long)pCounter->currentLSrcLine, tempNum );
				pCounter->parse_logical_line = "How to get the info from (*i).second.mySLOCLines ?\n   Logical line number is: " + tempNum;
			}
		}

		string err = "Error: Unable to count file (";
		if (embFile != string::npos)
			err += i->second.file_name.substr(0, embFile);
		else
			err += i->second.file_name;
		err += ")";

		if ( EXCEPTION_BAD_ALLOC != analyze_exception_type )
		{
			// NOT a Low RAM memory condition
			// To be compatabile with 2014 show the Error on console
			_prv_SaveOrAddError( threadIdx, userIF, err_msgs, err, false );

			// To keep Error accounting correct decrement counter
			extern unsigned int	errors_count;
			errors_count--;
		}

		// Append message about File name to existing Exception info message
		err += "\n" + analyze_exception_msg;

		errList += err + "\n";
		//userIF->AddError(err, true);
		bool doLogOnly = true;
		if ( EXCEPTION_BAD_ALLOC == analyze_exception_type )
			doLogOnly = false;

		_prv_SaveOrAddError( threadIdx, userIF, err_msgs, err, doLogOnly );

		//WriteUncountedFile("Unhandled Counting Error", i->second.file_name, useListA, print_csv);
		_prv_SaveOrWriteUncountedFile( threadIdx, userIF, unc_files,
									"Unhandled Counting Error", i->second.file_name, 
									useListA, print_csv, outDir, false );

		string	dump;
		StackDump( dump, EXCEPTION_HANDLER_PARSER, threadIdx, 
					analyze_exception_type, analyze_exception_msg,
					pCounter );
		_prv_SaveOrAddError( threadIdx, userIF, err_msgs, dump, doLogOnly );  // Do NOT Show ERROR on console

#ifdef	_DEBUG
		// Show on console for DEBUG builds
		if ( true == doLogOnly )
			printf( "%s\n", dump.c_str() );
#endif
		errList += dump;
	}

	return;
}

#ifdef	ENABLE_THREADS
// Divide a List of Files among multiple threads and start them working
int ReadFilesThreads( const unsigned long	num_files,
					const		bool		noRead,
					const		bool		noWeb,
					UserIF				*	userIF,
					const		bool		print_cmplx,
					const		bool		print_csv,
					StringVector		&	inFileVector,	// Entire list to divide
					const		bool		useListA,
					const		bool		clearCaseFile,
					string					outDir,
					const		bool		process_after_read,
					const		bool		discard_PHY_lines_after_process,
					const		bool		discard_lines_after_process,
					const		bool		no_warnings_to_UI,
					const		bool		no_uncounted )
{

	return _prv_ListToThreads( true,			// File list flag
								num_files,
								noRead,
								noWeb,
								userIF,
								print_cmplx,
								print_csv,
								&inFileVector,	// File list to divide
								NULL,			// DIFF Pairs list, etc.
								useListA,
								clearCaseFile,
								outDir,
								process_after_read,
								discard_PHY_lines_after_process,
								discard_lines_after_process,
								no_warnings_to_UI,
								no_uncounted );

}
#endif

/*!
*			StartWorkerThreads
* Set up and Start a number of worker threads.
* Any threads started will be waiting for either work to do or to exit thread if wanted.
*
* \param num_to_start	IN			number of worker threads wanted to start
* \param message		IN/OUT		ref to message giving details for number actually started
*
* \returns							1 if number of threads wanted was started OK
*							or		positive number >= 2 giving number of threads actually started
*							or		negative number giving Severe unrecoverable Error code
*
*	It is the calling code's responsibility to check for Errors.
*
* Recommended actions for Severe unrecoverable Errors: 
*		Log the ERROR with all given details (found in message)
*		Continue running in single thread mode without any worker threads
*
* Recommended actions for the number of threads actually started being lower than wanted:
*		Log an INFO message about wanted and actual worker thread count
*		Go ahead and use the availble (smaller number) of worker threads
*/
int StartWorkerThreads( CounterForEachLangType * pCounterForEachLanguage, 
						UserIF				*	userIF,
						const unsigned int num_to_start, string & message )
{
	int				retVal = THREADS_STARTED_OK;

#ifdef	ENABLE_THREADS

// TODO: Shrink the size of _prv_threadStatus struct!  ALIGN with HW CPU Cache Line Size! 64 bytes on AMD CPU
//
//		Captured output using VC++ 2010 Express 32 bit compiler
// Sizes (bytes): Thread status struct: 212, ptr: 4, int: 4, enum: 4, bool: 1, char 1
//
#ifdef	_DEBUG_THREAD_STRUCT_SIZE_n_ALIGNMENT
	
	enum SomeEnum {
		SILLY_ENUM = 1,
	};
	SomeEnum	an_enum = SILLY_ENUM;

	unsigned int	thread_struct_size = sizeof( g_threads_status[ 0 ] );
	printf( "\nSizes (bytes): Thread status struct: %d, ptr: %d, int: %d, enum: %d, bool: %d, char %d\n", 
			thread_struct_size, sizeof( char * ), sizeof( int ), sizeof( an_enum ), sizeof( bool ), sizeof( char ) );
#endif

	int				error_code = 0;
	unsigned int	started_thread_count = 0;
	unsigned int	num_wanted = num_to_start;

	message = "";

	// Save the calling (main) thread id
//	g_main_thread_id = get_id();

	g_userIF = userIF;
	g_MainThread_CounterForEachLanguage = pCounterForEachLanguage;

	if ( num_wanted < MIN_UCC_THREAD_COUNT )
	{
		num_wanted = MIN_UCC_THREAD_COUNT;
		message = "Information: Number of threads wanted is too few.  Using 2 threads.\n";
	}

	if ( num_wanted > MAX_UCC_THREAD_COUNT )
	{
        if ( num_wanted <= 3 * num_HW_cores )	// Seems like a practical limit is 3 times number of cores available
			message = "Information: Number of threads wanted is more than #define MAX_UCC_THREAD_COUNT in UCCThread.h\nEdit to be a larger number if you wish.  Using ";
		else
			message = "Information: Number of threads wanted is too many.  Using ";
		num_wanted = MAX_UCC_THREAD_COUNT;
		string tmp;
		IntToStr( MAX_UCC_THREAD_COUNT, tmp );
		message += tmp;
		message += " threads.\n";
	}

	// Set up array of thread status structs
	if ( g_threads_status_in_use )
		FinishWorkerThreads();

	_prv_threadStatus		one_thread_status;
	_prv_Init_threadStatus( one_thread_status );

	try
	{
		for ( unsigned int j = 0; j < num_wanted; j++ )
		{
			one_thread_status.thread_idx = j;
			
            // Create a Semaphore class per thread to block a worker thread when not doing work.
            // Initial value of semaphore will block the worker thread when it calls wait/acquire.
//#ifdef  QTGUI
//            one_thread_status.pQSem = new QSemaphore( 0 );
//#else
			one_thread_status.pSem = new Semaphore( 0 );
//#endif
			g_threads_status[ j ] = one_thread_status;

			g_threads_file_names[ j ]      = "";
			g_threads_pSrcFileElement[ j ] = NULL;

		// Create a thread for each struct
#ifdef  QTGUI
            // Create a long lived UCC Worker Qt Object
            UCCWorker * pUCCWorkerObj = new UCCWorker();

            // Save the pointer to the Qt Object instance for later cleanup
            g_UCC_Workers[ j ] = pUCCWorkerObj;

            // Call to UCC procedure it will block until the Semaphore is released
            WorkerThread * pWorkerThread = g_UCC_Workers[ j ]->startWorkInAThread( j );

            // Save the pointer to the thread class instance for later cleanup
            g_WorkerThreads[ j ] = pWorkerThread;
#else
			boost::thread myThread( prv_WorkThread_Function, j );
//			g_threads_status[ j ].threadId = myThread.get_id();
#endif
			g_threads_status_in_use++;

			// Now  the boost::thread class myThread will go out of scope.
			// When the boost::thread class that represents a thread of
			// execution is destroyed the thread becomes detached. 
			// Once a thread is detached, it will continue executing until 
			// the invocation of the function or callable object supplied on 
			// construction has completed, or the program is terminated. 
			//
			//		OR 
			// the way done here, when the main thread tells it to Exit.

			started_thread_count++;

			// Pause the calling (main) thread so the created threads start OK
#ifdef  QTGUI
            QThread::yieldCurrentThread();
#else
			boost::this_thread::yield();
#endif
			g_exclusive_Mutex.lock();
			g_exclusive_Mutex.unlock();
		}

		// Sleep main thread for a bit
		ThreadSleep( ONE_SECOND );

		g_exclusive_Mutex.lock();
		g_exclusive_Mutex.unlock();

		unsigned int thread_started_OK_count;
		do
		{
			thread_started_OK_count = 0;
			for ( unsigned int j = 0; j < started_thread_count; j++ )
			{
				if ( true == g_threads_status[ j ].thread_has_started )
					thread_started_OK_count++;
			}

			if ( thread_started_OK_count < started_thread_count )
			{
#ifdef  QTGUI
                QThread::yieldCurrentThread();
#else
				boost::this_thread::yield();
#endif
				g_exclusive_Mutex.lock();
				g_exclusive_Mutex.unlock();
			}

		} while ( thread_started_OK_count < started_thread_count );
	}
	catch (...)
	{
		string tmp;
		if ( started_thread_count >= MIN_UCC_THREAD_COUNT )
		{
			// Got enough threads started to work with
			message += "Information: Number of threads started ";
			IntToStr( (int)started_thread_count, tmp );
			message += tmp;
			message += " is less than number wanted ";
			IntToStr( (int)num_to_start, tmp );
			message += tmp;
			message += "\n";
		}
		else
		{
			error_code = -1;	// Severe problem creating a thread
			if ( message.size() )
				message += "\n";
			message += "Error: Unable to start even ";
			IntToStr( MIN_UCC_THREAD_COUNT, tmp );
			message += tmp;
			message += " threads.";

			if ( started_thread_count )
			{
				FinishWorkerThreads();
				started_thread_count = 0;
			}
		}

		g_exclusive_Mutex.unlock();
	}	//	END		catch

	if ( num_wanted > started_thread_count )
	{
		if ( error_code )
		{
			retVal = error_code;
			if ( 0 < started_thread_count )
				FinishWorkerThreads();
		}
		else
			retVal = (int) started_thread_count;
	}
#else
	// Threads NOT enabled.  Tell User.  Reference unused args to avoid compiler warnings.
	if ( NULL == pCounterForEachLanguage )
		message = "NULL pointer for pCounterForEachLanguage";	// Bogus message not used.
	
	if ( NULL == userIF )
		message = "NULL pointer for userIF";	// Bogus message not used.
	
	if ( 0 == num_to_start )
		message = "number to start is 0";	// Bogus message not used.

	message = "Information: worker Threads not compiled.  Edit UCCThread.h  See ENABLE_THREADS\n";
	retVal = -2;
#endif
	return retVal;
}

#ifdef	ENABLE_THREADS
// Get a pointer to a Thread's struct of Language parser class instance pointers.
// Called only from Main thread AFTER the worker Thread is created but BEFORE it is busy.
CounterForEachLangType * GetThread_pCounterForEachLanguage( const unsigned int threadIdx )
{
	CounterForEachLangType * retVal = NULL;

	if ( threadIdx < g_threads_status_in_use )
		retVal = g_threads_status[ threadIdx ].pCounterForEachLanguage;

	return retVal;
}
#endif

/*!
*			FinishWorkerThreads
* Finish any running worker threads.
* This may block for a few seconds while worker threads exit if they were busy.
*/	
void FinishWorkerThreads()
{
#ifdef	ENABLE_THREADS
	
	// Tell all active Threads to finish and return.
	for ( unsigned int j = 0; j < g_threads_status_in_use; j++ )
	{
		g_threads_status[ j ].main_WantsAction = EXIT_NOW;
		
		// Toggle semaphore to wake thread if it was waiting
		// Give up rest of calling (main) thread time slice and let worker threads run and exit
		g_threads_status[ j ].pSem->signal();
#ifdef  QTGUI
        QThread::yieldCurrentThread();
#else
		boost::this_thread::yield();
#endif
	}

	// Check all threads to have status of THREAD_HAS_EXITED or THREAD_RESTART_NEEDED
	unsigned int threads_to_check = g_threads_status_in_use;
	while ( threads_to_check )
	{
		for ( unsigned int j = 0; j < g_threads_status_in_use; j++ )
		{
			ThreadState		threadState = g_threads_status[ j ].work_ThreadState;
			if ( ( THREAD_HAS_EXITED     != threadState )
			  && ( THREAD_RESTART_NEEDED != threadState ) )
			{
				// Give up rest of calling (main) thread time slice and let worker threads run and exit
				ThreadSleep( 100 );
			}
			else
				threads_to_check--;
		}

		// Is there 1 or more threads not yet Exited?  Ensure they get checked again.
		if ( threads_to_check )
			threads_to_check = g_threads_status_in_use;
	}


    for ( unsigned int j = 0; j < g_threads_status_in_use; j++ )
    {
#ifdef  QTGUI
        // Stop the Qt Threads
        g_WorkerThreads[ j ]->exit();

        // Free all Qt Objects
        delete g_WorkerThreads[ j ];
        g_WorkerThreads[ j ] = (WorkerThread *)NULL;

        delete g_UCC_Workers[ j ];
        g_UCC_Workers[ j ] = (UCCWorker *)NULL;
#endif
		// Free resources allocated
		delete g_threads_status[ j ].pSem;
		g_threads_status[ j ].pSem = (Semaphore *)NULL;

		g_threads_status[ j ].pCounterForEachLanguage = NULL;	// Freed by thread
    }


    g_threads_status_in_use = 0;

#endif
}


// Propagate back the results from recently finished worker threads
// Thread Context: Called from Main thread.
//
// Because this runs from the Main thread after worker threads finish,
// some Global variables are used without worry about poor performance.
void CombineThreadResults()
{
#ifdef	ENABLE_THREADS
	if ( 0 == g_assigned_count )
		return;		// already done

	// Send back the combined results of Reading a List of Files
	// and perhaps doing Analysis and Counting as well...
	bool	my_useListA   = g_threads_status[ 0 ].useListA;
	bool	was_File_Work = g_threads_status[ 0 ].is_File_Work;
	string	writeErr;
				
// OK to use Globals here as running in Main thread.
	bool	isDup	= false;

	if ( g_threads_status[ 0 ].langs.size() )
	{
		// Duplicate checking
		isDup = true;
		extern	StringVector	duplicateFilesInA1, duplicateFilesInA2;
		extern	StringVector	duplicateFilesInB1, duplicateFilesInB2;

		// Destination
		StringVector * pdup1 = &duplicateFilesInA1;
		StringVector * pdup2 = &duplicateFilesInA2;
		if ( false == my_useListA )
		{
			pdup1 = &duplicateFilesInB1;
			pdup2 = &duplicateFilesInB2;
		}

		for ( unsigned int j = 0; j < g_assigned_count; j++ )
		{
			if ( g_threads_status[ j ].langs.size() )
			{
				// Release langs and array(s) of structs
				for ( unsigned int k = 0; k < g_threads_status[ j ].langs.size(); k++ )
				{
					free( (void *)( g_threads_status[ j ].langs[ k ].pFilePtrs ) );
					g_threads_status[ j ].langs[ k ].pFilePtrs = NULL;
				}
				g_threads_status[ j ].langs.resize( 0 );

				// Copy back
				pdup1->insert( pdup1->end(), g_threads_status[ j ].dupList1->begin(), g_threads_status[ j ].dupList1->end() );

				// Clean up
				g_threads_status[ j ].dupList1->resize( 0 );
				delete (g_threads_status[ j ].dupList1);
				g_threads_status[ j ].dupList1 = NULL;

				// Copy back
				pdup2->insert( pdup2->end(), g_threads_status[ j ].dupList2->begin(), g_threads_status[ j ].dupList2->end() );

				// Clean up
				g_threads_status[ j ].dupList2->resize( 0 );
				delete (g_threads_status[ j ].dupList2);
				g_threads_status[ j ].dupList2 = NULL;
			}
		}

		// Fall through to get Error messages done
	}
	else
	{
		// File processing or Differencing + file processing
		extern	SourceFileList SourceFileA;
		extern	SourceFileList SourceFileB;
		SourceFileList & rDestFiles = (my_useListA) ? SourceFileA: SourceFileB;
		
		CounterForEachLangType::iterator itDestStart = g_MainThread_CounterForEachLanguage->begin();
		CounterForEachLangType::iterator itDestEnd   = g_MainThread_CounterForEachLanguage->end();
		

		for ( unsigned int j = 0; j < g_assigned_count; j++ )
		{
			// Copy back results to Main thread structures in MainObject, Globals, DiffTool
			_prv_threadStatus &		rThreadStatus = g_threads_status[ j ];

			if ( was_File_Work )
			{
				SourceFileList * pSrcFiles = g_threads_status[ j ].pThreadSrcFileList;

				// Append to existing list

			#ifdef	USE_LESS_MEM_ON_COPY
				// Minimize use of temporary memory.  
				// This can save several MB or GB or more depending on size of File list and Files
				// Which can make the difference between success with Threads or not.
				// So this is the default approach.
				unsigned int num_to_copy = pSrcFiles->size();

				if ( num_to_copy )
				{
					// Determine a chunk of File results to copy / erase at a time.
					// This is biased to work with a large number of Files but limit use of temporary memory.
					// For example if doing 50,000 files this would do 100 chunk copies instead of 20.
					unsigned int chunk_size = num_to_copy / 20;
					if ( chunk_size > 500 )
						chunk_size = 500;
					if ( 0 == chunk_size )
						chunk_size = num_to_copy;
			
					list<SourceFileElement>::iterator itSrcPos1;
					list<SourceFileElement>::iterator itSrcPos2;
					while ( num_to_copy )
					{
						itSrcPos1 = pSrcFiles->begin();
						itSrcPos2 = itSrcPos1;
						advance( itSrcPos2, chunk_size );
						rDestFiles.insert( rDestFiles.end(), itSrcPos1, itSrcPos2 );
						pSrcFiles->erase( itSrcPos1, itSrcPos2 );

						num_to_copy -= chunk_size;
						if ( chunk_size > num_to_copy )
							chunk_size = num_to_copy;
					}
					/*
					// This works but is TOO SLOW if doing thousands of File results
					for ( unsigned int k = 0; k < num_to_copy; k++ )
					{
						rDestFiles.push_back( *(pSrcFiles->begin()) );	// Append element to end of list
						pSrcFiles->pop_front();						// Get rid of element just copied
					} */
				#else
					// Use Faster but temporarily uses much MORE memory approach
					rDestFiles.insert( rDestFiles.end(), pSrcFiles->begin(), pSrcFiles->end() );
				#endif
					pSrcFiles->resize( 0 );  // Release memory from thread struct
				}
			}
			else
			{
				// DIFF results from multiple threads
				if ( true == rThreadStatus.printDup )
					pDiffTool->printDup = true;

				pDiffTool->total_addedLines      += rThreadStatus.total_addedLines;
				pDiffTool->total_deletedLines    += rThreadStatus.total_deletedLines;
				pDiffTool->total_modifiedLines   += rThreadStatus.total_modifiedLines;
				pDiffTool->total_unmodifiedLines += rThreadStatus.total_unmodifiedLines;
				pDiffTool->dup_addedLines        += rThreadStatus.dup_addedLines;
				pDiffTool->dup_deletedLines      += rThreadStatus.dup_deletedLines;
				pDiffTool->dup_modifiedLines     += rThreadStatus.dup_modifiedLines;
				pDiffTool->dup_unmodifiedLines   += rThreadStatus.dup_unmodifiedLines;
			}
		}
	//Warning fix 11.25.16. DO NOT USE THESE VARIABLES AFTER THIS.
	(void)itDestStart;
	(void)itDestEnd;
	}

	// Send back Errors, Uncounted files, Messages
	bool	any_messages_shown = false;
	ErrMsgStructVector::iterator		itErr;
	UncountedFileStructVector::iterator	itUnc;

	for ( unsigned int j = 0; j < g_assigned_count; j++ )
	{
		_prv_threadStatus &		rThreadStatus = g_threads_status[ j ];

		// Handle list of Errors
		for ( itErr = rThreadStatus.thread_err_msgs.begin(); itErr != rThreadStatus.thread_err_msgs.end(); ++itErr )
		{
			errMsgStruct & err_msg = *(itErr);
			// err_msg.threadIdx = threadIdx;
			if ( err_msg.err.size() )	// Don't do empty messages.  So main thread can empty messages before thread finishes.
			{
				g_userIF->AddError( err_msg.err, err_msg.logOnly, (int)err_msg.preNL, (int)err_msg.postNL );
				if ( false == err_msg.logOnly )
					any_messages_shown = true;
			}
		}
		rThreadStatus.thread_err_msgs.resize( 0 );

		// Handle list of Uncounted Files
		for ( itUnc = rThreadStatus.thread_unc_files.begin(); itUnc != rThreadStatus.thread_unc_files.end(); itUnc++ )
		{
			uncountedFileStruct & unc_file = *(itUnc);
			if ( unc_file.msg.size() )
			{
				writeErr = WriteUncountedFile( unc_file.msg, unc_file.uncFile, 
											unc_file.useListA, unc_file.csvOutput, unc_file.outDir );
				if ( writeErr.size() )
				{
					g_userIF->AddError( writeErr );
					any_messages_shown = true;
				}
			}
		}
		rThreadStatus.thread_unc_files.resize( 0 );

		if ( rThreadStatus.errList.size() )
		{
			// Show Errors wanted on the UI found by this thread (skip trailing NewLine)
			bool msg_shown = g_userIF->updateProgress( rThreadStatus.errList, false );
			if ( true == msg_shown )
				any_messages_shown = true;
			rThreadStatus.errList.clear();
		}

		if ( ! isDup )
		{
			extern bool g_process_after_read;
			if ( g_process_after_read )
			{
				// Update main thread from each thread's CounterForEachLangType local data
				AddFromOtherLangCounters( g_MainThread_CounterForEachLanguage,
										rThreadStatus.pCounterForEachLanguage,
										my_useListA );
			}
		}
	}

	if ( any_messages_shown )
		cout << endl;	// Put in a trailing NewLine after the console messages

	// All results from threads handled, get ready for next request
	g_assigned_count = 0;

#endif		// #ifdef	ENABLE_THREADS
}

#ifdef	ENABLE_THREADS
// Return the number of threads working.
// Thread Context: Called from the Main thread
unsigned int NumThreadsBusy( unsigned long & count_done, 
							const unsigned int sleep_milliseconds,
							const bool combine_results_from_threads )
{
	unsigned int	busy_count = 0;
	count_done = 0L;

	for ( unsigned int j = 0; j < g_threads_status_in_use; j++ )
	{
		if ( THREAD_WORKING == g_threads_status[ j ].work_ThreadState )
			busy_count++;
		
		count_done += (unsigned long)(g_threads_status[ j ].thread_work_count_done);
	}

	if ( 0 == busy_count )
	{
		if ( combine_results_from_threads )
		{
			// At this point process any Error messages, Uncounted Files, etc.
			CombineThreadResults();
		}
	}
	else
		ThreadSleep( sleep_milliseconds );

	return busy_count;
}
// Return the path/file being parsed by a Thread		
void GetThreadParseFile( const unsigned int threadIdx, string & path_file )		
{		
	path_file = "";		
	if ( threadIdx < g_threads_status_in_use )		
		path_file = g_threads_status[ threadIdx ].current_path_file;		
	return;
}
#endif


/*!
*			ThreadAddFileToRead
* Request a thread to read in the Physical lines of a given named file.
* This will return usually before the thread finishes reading.  It is NOT synchronous.
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

/*  COMMENTED OUT.  May be useful sometime.
unsigned int ThreadAddFileToRead( const string fileName, const bool trim_line, 
								SourceFileElement * pSrcFileElement,
								unsigned int & error_count )
{
	unsigned int	retVal = 0;		// Number of pending Requests
	bool			wait_for_NOT_working = false;
	unsigned int	NOT_working_count = 0;

	if ( 0 == fileName.size() )
	{
		wait_for_NOT_working = true;
	}
	
	// Add a file read request for a thread that is ready
	//		OR
	// Wait for all threads to NOT be working
	bool done = false;
	do
	{
		NOT_working_count = 0;

		// Find a thread ready for Work
		for ( unsigned int j = 0; j < g_threads_status_in_use; j++ )
		{
			if ( EXIT_NOW == g_threads_status[ j ].main_WantsAction )
			{
				continue;		// Should NOT get here
			}

			if ( wait_for_NOT_working )
			{
				//  See if the thread is NOT working
				if ( THREAD_WORKING != g_threads_status[ j ].work_ThreadState )
				{
					NOT_working_count++;
					if ( THREAD_HAS_EXITED != g_threads_status[ j ].work_ThreadState )
					{
						// Set the wanted state.  No need to toggle semaphore.
						g_threads_status[ j ].main_WantsAction = NO_WORK_YET;
					}
				}
				
				// boost::this_thread::yield();	//ThreadSleep( 100 );
				continue;
			}
			else
			{
				// Add a file read request for a thread that is ready
				if ( THREAD_WAITING_FOR_WORK != g_threads_status[ j ].work_ThreadState )
					continue;
				
				if ( g_threads_file_names[ j ].size() )
					continue;	// Thread was given a file to read but not given time

				if ( g_threads_status[ j ].thread_had_error )
				{
					error_count = 1;	// some error processing a file
					// Error details are found in the itFileElement first and second elements

				}

				g_threads_pSrcFileElement[ j ]         = pSrcFileElement;
				g_threads_status[ j ].main_trim_line   = trim_line;
				g_threads_file_names[ j ]              = fileName;
				g_threads_status[ j ].main_WantsAction = GET_TO_WORK;

				// Toggle semaphore to wake thread
				g_threads_status[ j ].pSem->signal();

				retVal = 1;		// There will be at least 1 pending request
				done = true;
				boost::this_thread::yield();	// Allow assigned thread (and maybe others) to run
				break;
			}
		}	//	END		for ( unsigned int j = 0; j < g_threads_status_in_use; j++ )

		if ( g_threads_status_in_use == NOT_working_count )
			done = true;

		if ( false == done )
			boost::this_thread::yield();	//ThreadSleep( 100 );

	} while ( false == done );

	return retVal;
}
*/


/*
------------------------------------------------------------------------------
	TOP LEVEL DESIGN  Multiple Thread feature

Primary Design Goal:
	Improve performance ==> less processing time.

Multiple threads: (appropriate Use case)

	There are many files to process and the overall processing time
	takes several minutes (your definition of several) to complete.

Multiple threads: (doing this may give poor results)

	If the processing time without extra threads is less than a minute 
	you may not see less time but rather see a small increase in time. 

	User specifies too many threads for the underlying OS/HW to support.
	Could be where too many threads are competing for
	CPU, disk resources, memory, etc.  Increased time could result.

Secondary Design Goals:
	Correctness ! ! !
		Produce results as correctly as single threaded implementation.
		Constrain ways for things to go wrong with extra threads (see below).

	Preserve existing single threaded performance
		Try to have minimal impact on existing UCC implementation.

2 DATA POINTS:  (Use cases where Multiple threads may help)

Built Optimized version of UCC_2013_04 for these measurements.
These were gathered on an older Windows 7.1 laptop with AMD 64 bit dual core CPU

TEST 1:
	Linux version 3.11.6  all directories/files using Original version
34,341 C source files plus various other files
Test with Optimized build took 1,884 seconds and CPU use was around 60% to 90%
1,884 seconds = 31.4 minutes
	This is an average of over 18.2 files processed per second

UCC -nowarnings -threads 2 -dir "C:\Linux\Stable_3_11_6\linux-3.11.6" 
-outdir  "C:\Utilities\TEST\UCC\Files_OUT" -ascii
2 Threads started.
Building list of source files...
Read, Analyze & Count keywords in files..DONE
Looking for duplicate files..............DONE
Generating results to files..............DONE
 36,876 files for 52.8 files processed per second

     Processing  Step     :  seconds
------------------------- : --------
 Build file list          :    17
Read, Analyze & Count     :   582
 Find duplicates          :    92
  Generate results files  :     8
               Total time : 11 minutes 39 seconds

TEST 2:
	SMALLER part of Linux (just the arch) directory tree

Original version Optimized build: CPU use was around 60% to 90%
2 minutes 50 seconds or about 73.0 files processed per second

	Using 2 Threads to do Linux arch dir
UCC -threads 2 -dir "C:\Linux\Stable_3_11_6\linux-3.11.6\arch" 
-outdir "C:\Utilities\TEST\UCC\Files_OUT" -ascii
2 Threads started.
Building list of source files...
Read, Analyze & Count keywords in files..DONE
Looking for duplicate files..............DONE
Generating results to files..............DONE
 12418 files for 208.8 files processed per second

     Processing  Step     :  seconds
------------------------- : --------
 Build file list          :     1
Read, Analyze & Count     :    41
 Find duplicates          :    15
  Generate results files  :     2
               Total time :    59 seconds

Overall speed is close to 3x of Original.
Most of the improvement is from changes to Duplicate checking code.
But using 2 threads is definitely faster than original as well.

I have found a fair variation in the time to first build the list of files.
I am guessing this is related to OS file caching.
I usually run the same test 2 times and use the times from the 2nd run.
This allows the OS file caching to work and gives a lower overall time.
This way may not be realistic for Users running UCC 1 time for various directories.

------------------------------------------------------------------------------
	ANALYSIS of Unified Code Count implementation to support Multiple threads

Writing to console from multiple threads:
cout Use:
	Good news is that MainObject.cpp mostly uses cout for Help output
	other uses of cout are for RARE Error messages from CCodeCounter.cpp and a
	few of the Language specific parsing modules.  
	Decision: 
		Leave unchanged except for a few places in MainObject

Messages/Logging Revised:
	Because the total list of files to process is split among N threads:
	there is likely some various messages that each thread may create (Warnings mostly).
	To preserve the same order of messages as if running single threaded, each message
	is saved in a structure with the text and some decision support flags.
	After the threads finish work, the message structures are used to call expected
	message/logging helpers as if in single thread mode.
	Downside to this approach is that messages from threads are no longer "real time"
	Upside is that same order of messages is presented (just a little later).

	The code could be changed so when the main thread wakes up to check for all threads
	finishing, it could also start processing the structures from just the first thread.
	This would give a pseudo "real time" feel but ONLY for messages from the first thread.

How Threads update the UI % completion
	Multiple threads updating the UI directly is NOT recommended (unless UI is designed that way).
	
	Threads calculate the count down number of files to process to show 1/2 of 1 % progress changed.
	When the count down reaches Zero, a thread will update a count in a shared structure 
	that the main thread a fraction of a second later will read.  This is a classic example of
	"False Sharing" which can be a big decrease on multithreaded performance.  So I tried to
	minimize the number of counts done updates by a thread while still giving decent UI update info.
*/

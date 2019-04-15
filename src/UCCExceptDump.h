//! UCC Exception and Stack Dump support
/*!
* \file UCCExceptDump.h
*
* This file has public Exception and Stack Dump characteristics
* so that the rest of UCC is relatively unchanged.
*
* ADDED to UCC 2015_12 release by Randy Maxwell
*   Changes started on 2015_10_11
*   Changes  ended  on 2015_10_1
*   Addition of Stack Dump feature (mostly after an Exception)
*
*/

#ifndef UCC_EXCEPT_DUMP_H
#define UCC_EXCEPT_DUMP_H

// Define the general location of code of the Exception handler.  
// This helps with the strategy of how to handle the Exception.
#define EXCEPTION_HANDLER_APP			1
#define EXCEPTION_HANDLER_THREAD		2
#define EXCEPTION_HANDLER_PARSER		3

// Define various Exceptions types.
// Use Defines instead of Enum so can be assigned to int type.
// See main.cpp for how these are caught/used as overall UCC exit value.
// See UCCThread.cpp for exceptions from threads and from a Language Parser.
//
#define	EXCEPTION_DID_NOT_HAPPEN		0		// 0 for backwards compatability
#define	EXCEPTION_NOT_SPECIFIC			1		// 1 for backwards compatability

// Keep this set to first specific
#define	FIRST_SPECIFIC_EXCEPTION	(EXCEPTION_UNDERFLOW_ERROR)

#define	EXCEPTION_UNDERFLOW_ERROR		(EXCEPTION_NOT_SPECIFIC+1)
#define EXCEPTION_OVERFLOW_ERROR		(EXCEPTION_NOT_SPECIFIC+2)
#define	EXCEPTION_RANGE_ERROR			(EXCEPTION_NOT_SPECIFIC+3)
#define	EXCEPTION_SYSTEM_ERROR			(EXCEPTION_NOT_SPECIFIC+4)
#define	EXCEPTION_RUNTIME_ERROR			(EXCEPTION_NOT_SPECIFIC+5)
#define EXCEPTION_DOMAIN_ERROR			(EXCEPTION_NOT_SPECIFIC+6)
#define	EXCEPTION_LENGTH_ERROR			(EXCEPTION_NOT_SPECIFIC+7)
#define	EXCEPTION_INVALID_ARG_ERROR		(EXCEPTION_NOT_SPECIFIC+8)
#define	EXCEPTION_OUT_OF_RANGE_ERROR	(EXCEPTION_NOT_SPECIFIC+9)
#define	EXCEPTION_LOGIC_ERROR			(EXCEPTION_NOT_SPECIFIC+10)
#define EXCEPTION_BAD_ALLOC				(EXCEPTION_NOT_SPECIFIC+11)
#define	EXCEPTION_STD_EXCEPTION			(EXCEPTION_NOT_SPECIFIC+12)

// Keep this set to end of above list
#define HIGHEST_EXCEPTION			(EXCEPTION_STD_EXCEPTION)


// Call 1 time at UCC start
void Init_StackDump();

// Get some info about Baseline A or B
void ShowFileSetStats( const bool useListA = true, string * pString = NULL );

// THINK LOW Memory condition!  Call this early to get at least some information and suggestion to User.
void FirstExceptionMessage( const unsigned int	context   = EXCEPTION_HANDLER_APP,
							const unsigned int  threadIdx = 0,		// use MAIN_THREAD_INDEX or actual threadIdx
							const unsigned int	exType    = EXCEPTION_DID_NOT_HAPPEN, 
							const string		ex_what   = "",
							const CCodeCounter * pCounter = NULL );	// Only need this for Parser dumps);

// MAY NOT GIVE GOOD RESULTS IF LOW MEMORY exception!  
//    Call FirstExceptionMessage for LOW Memory.
// Call this anytime.  
// Useful as part of Exception handling.
void StackDump( string & dumpStr, 
				const unsigned int	context   = EXCEPTION_HANDLER_APP,
				const unsigned int  threadIdx = 0,		// use MAIN_THREAD_INDEX or actual threadIdx
				const unsigned int	exType    = EXCEPTION_DID_NOT_HAPPEN, 
				const string		ex_what   = "",
				const CCodeCounter * pCounter = NULL );	// Only need this for Parser dumps

// This does a quick? survey of the Heap giving number of blocks and total committed sizes
// Implemented for Windows
void	HeapInUse( int & errorRet, unsigned int & heap_count, unsigned long & block_count, 
				unsigned long long & total_sizes, bool & validate, const bool in_use = true );


// Declare a class for most platforms Stack Traces
class KxStackTrace
{
public :
/* Modification: 2016.01; USC
*   Fixed cygwin compilation error w/ exception handling */
#if defined(WIN32) || defined(CYGWIN)
	void	Win32_KxStackTrace();
#else
	void	POSIX_KxStackTrace();
#endif

	unsigned int	stack_trace_count;
};


#endif		//	UCC_EXCEPT_DUMP_H
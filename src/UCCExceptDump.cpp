//! UCC Exception and Stack Dump support
//Includes call stack symbol printing support for g++ ubuntu. Addition 11.27.16.
/*!

* \file UCCExceptDump.cpp

*

* This file encapsulates Exception and Stack Dump implementation details and dependencies

* so that the rest of UCC is relatively unchanged.

*

* ADDED to UCC 2015_12 release by Randy Maxwell

*   Changes started on 2015_10_11

*   Changes  ended  on 2015_10_1

*   Addition of Stack Dump feature (mostly used after an Exception)

*

* Most Exciting Exceptions:

*   Low Memory Available:          (RAM or Disk)  Set what is accessed here before Exception.

*   REALLY Unexpected conditions:  Invalid instruction, Floating Point, Access Violation ...

*                                    Same as for Low Memory

*

* Other Exceptions:

*   Provide enough info so a User and/or programmer running UCC with compiler/Debugger 

*   gets some details of WHERE in the UCC code doing WHAT and other info from Exception.

*

* Implementation Notes at the end...

*

* Resources:

*   Generate Stack Traces on Crash Portably in C++  page

*

*

*/



#include "UCCThread.h"	// Needed to get enable threads define



#include "UCCBeforeLibraryIncludes.h"    // Modification: 2015.12



/* Modification: 2016.01; USC

*   Fixed cygwin compilation error w/ exception handling */

#define		MAX_CALL_FRAMES		128
#define UNUSED(x) (void)(x) //Modification: 2018.05


// Utilities to support checking the Heap.  Implemented for Windows

#if defined(WIN32) || defined(CYGWIN)

	#define		WIN32_LEAN_AND_MEAN

	#include	<Windows.h>

	#undef		WIN32_LEAN_AND_MEAN



	// Windows defines PASCAL as a legacy call interface 

	// so undefine it here so we can use PASCAL as a Language name

	#ifdef	PASCAL

		#undef	PASCAL

	#endif

#else

	#include	<execinfo.h>

	void printStackTrace( FILE *out = stderr, unsigned int max_frames = MAX_CALL_FRAMES - 1);

#endif



// Add support for another OS here if wanted





#include <stdio.h>

#include <signal.h>



#include <string>		// Note for LOW RAM condition use of std::string is NOT wise



#include <ctime>

#include "UCCAfterLibraryIncludes.h"     // Modification: 2015.12



#include "UCCGlobals.h"

#include "UserIF.h"

#include "UCCExceptDump.h"



//

//		EXTERN		variables	Procedures

//

extern	time_t		timeStart;		// When UCC started



extern	UserIF	*	userIF;





// Format a Time msg

extern void	ShowTiming( string & time_str, 

					const time_t timeStart, const time_t timeEnd, 

					const bool show_total_only, 

					const bool doDiff, const bool doDups );



//

// Declare Public access variables

//





// 

//	PRIVATE variables that are useful to inspect anytime (from Debugger)

//



#ifdef	ENABLE_THREADS

	// Used to prevent (hopefully very rare) race conditions between these threads.

	// Here get a UCC mutex (thin layer over whatever mutex) from UCCThread.cpp helper if needed



#endif



bool			isNativeStackDumpAvailable	= false;

unsigned int	exceptionHandlerLocation	= EXCEPTION_HANDLER_APP;

unsigned int	exception_type				= EXCEPTION_DID_NOT_HAPPEN;

unsigned int	exception_thread_index		= MAIN_THREAD_INDEX;

string			exception_msg;

string			exception_what;

string			prev_dump_msg;



//

// Parser specific

//

CCodeCounter *  pCodeParser					= NULL;

string			parserName;



// Full file path/name

string			fileParsed;



unsigned int	fileParsedLineCount			= 0;	// total physical lines in the file

unsigned int	parseLineNumber				= 0;	// Physical line number that was being parsed (1 ...)

string			parseLineData;

string			parseLineDataBackup;				// No changes to original data for this line







// These are similar to what you see in CCodeCounter.h .cpp





//================================================================================

//

//		Cross		Platform		Better than our own Stack Dump in details

//

//		May be some Platforms not covered by this approach that Stack Dump does

//

//		"Generate Stack Traces on Crash Portably in C++"

//================================================================================



// See UCCExceptDump.h for KxStackTrace declaration

KxStackTrace	g_KxStackTrace;



/* Modification: 2016.01; USC

*   Fixed cygwin compilation error w/ exception handling */

#if defined(WIN32) || defined(CYGWIN)

void abortHandler( int signum )

#else

void abortHandler( int signum, siginfo_t* si, void * unused )

#endif

{

	// associate each signal with a signal name string.

	const char * name = NULL;
	//Modification: 2018.01 : Unused variable warning in GUI
#if defined(WIN32) || defined(CYGWIN)
    //do nothing
#else
    UNUSED(unused);
	UNUSED(si);
#endif

	switch ( signum )

	{

		case SIGABRT: name = "SIGABRT" ; break ;

		case SIGSEGV: name = "SIGSEGV" ; break ;

/* Modification: 2016.01; USC

*   Fixed cygwin compilation error w/ exception handling */

#if defined(UNIX) && !defined(CYGWIN)

		case SIGBUS: name = "SIGBUS" ; break ;

#endif

		case SIGILL: name = "SIGILL" ; break ;

		case SIGFPE: name = "SIGFPE" ; break ;

	}

	// Notify the user which signal was caught. We use printf, because this is the

	// most basic output function. Once you get a crash, it is possible that more

	// complex output systems like streams and the like may be corrupted. So we

	// make the most basic call possible to the lowest level, most

	// standard print function.

	if ( name )

		fprintf ( stderr, "Caught signal %d(%s)\n" , signum, name );

	else

		fprintf ( stderr, "Caught signal %d\n" , signum );

// 2016 Added to fix sometimes no path/file showing if a Signal happened

	//

	// Output 1 file being parsed if only Main thread, else short list of 1 file per thread

	// Actual file causing Signal if more than 1 is not determined here but this helps to isolate.

	if ( 0 == workThreadsCount )

	{

		fprintf ( stderr, "File being parsed:  %s\n", main_parse_path_file.c_str() );

	}

#ifdef	ENABLE_THREADS

	else

	{

		extern unsigned int	g_assigned_count;	// Set by UCCThread.cpp

		string path_file;

		fprintf ( stderr, " Files being parsed:\n" );

		for ( unsigned int i = 0; i < g_assigned_count; i++ )

		{

			// Output 1 file per thread

			GetThreadParseFile( i, path_file );

			if ( 0 != path_file.length() )

				fprintf ( stderr, "%s\n", path_file.c_str() );

		}

	}

#endif		// END of 2016 addition to do path/file being parsed if a Signal happened



	// Dump a stack trace.	Will do Windows later.  Randy

/* Modification: 2016.01; USC

*   Fixed cygwin compilation error w/ exception handling */

#if defined(WIN32) || defined(CYGWIN)

	// StackDump();

#else

	printStackTrace();

#endif

	// If you caught one of the above signals, it is likely you just

	// want to quit your program right now.

	exit ( signum );

}



/*

The signal() function is a posix standard and is supported in

Windows, Linux and OSX. When a given signal is generated it

will call the handler function in the second argument. You can

trap any operating system signal. In this case we are trapping

the most important signals that are generated when the OS

detects an execution problem.

SIGABRT is generated when the program calls the abort()

function, such as when an assert() triggers.

SIGSEGV and SIGBUS are generated when the program makes

an illegal memory access, such as reading unaligned memory,

dereferencing a NULL pointer, reading memory out of bounds

etc. SIGBUS is not supported under Windows.

SIGILL is generated when the program tries to execute a

malformed instruction. This happens when the execution

pointer starts reading non-program data, or when a pointer to

a function is corrupted.

SIGFPE is generated when executing an illegal floating point

instruction, most commonly division by zero or floating point

overflow.

To get a list of signals supported by your operating system run

the following command:



$ kill -l

1) SIGHUP 

2) SIGINT 

3) SIGQUIT 

4) SIGILL 

5) SIGTRAP

6) SIGABRT 

7) SIGEMT 

8) SIGFPE 

9) SIGKILL 

10) SIGBUS

11) SIGSEGV 

12) SIGSYS 

13) SIGPIPE 

14) SIGALRM 

15) SIGTERM

16) SIGURG 

17) SIGSTOP 

18) SIGTSTP 

19) SIGCONT 

20) SIGCHLD

21) SIGTTIN 

22) SIGTTOU 

23) SIGIO 

24) SIGXCPU 

25) SIGXFSZ

26) SIGVTALRM 

27) SIGPROF 

28) SIGWINCH 

29) SIGPWR 

30) SIGUSR1

*/



/* Modification: 2016.01; USC

*   Fixed cygwin compilation error w/ exception handling */

#if defined(WIN32) || defined(CYGWIN)

void KxStackTrace::Win32_KxStackTrace()

{

	signal ( SIGABRT, abortHandler );

	signal ( SIGSEGV, abortHandler );

	signal ( SIGILL, abortHandler );

	signal ( SIGFPE, abortHandler );

}

#endif



/*

	Using SigAction on POSIX Systems

	The example above will work on POSIX and some Windows

	systems. On any reasonably modern POSIX platform you can

	use the newer sigaction() function. This does the same thing

	but provides additional information about the signal, and a

	little more control over which signals you handle.

*/

/* Modification: 2016.01; USC

*   Fixed cygwin compilation error w/ exception handling */

#if defined(UNIX) && !defined(CYGWIN)

void KxStackTrace::POSIX_KxStackTrace()

{

/*

	We won't use the extra information in our code here, but with

	sigaction() you can find out stuff like the process id and user

	id of the sending process ( e.g. if a kill signal was sent from

	another process ). And you can get the address of the code that

	generated the signal, in the case of a crash. Its nice to know

	you have options.

*/

	struct sigaction sa;



	sa.sa_flags = SA_SIGINFO;

	sa.sa_sigaction = abortHandler;



	sigemptyset( &sa.sa_mask );



	sigaction( SIGABRT, &sa, NULL );

	sigaction( SIGSEGV, &sa, NULL );

	sigaction( SIGBUS, &sa, NULL );

	sigaction( SIGILL, &sa, NULL );

	sigaction( SIGFPE, &sa, NULL );

	sigaction( SIGPIPE, &sa, NULL );

}

#endif



/*

Printing a Basic Stack Trace for Linux and OsX



	On Linux and its Posix variants ( like OsX ), you will want to

	use backtrace() and backtrace_symbols() . The data is

	returned in buffers allocated by malloc() . To be clean you

	will want to free them before you exit.

	backtrace() returns the raw stack trace information, as an

	array of stack frames. If the length is zero then it is likely the

	stack was corrupt, and you are done. If you get some stack

	frames then you can turn them into a somewhat readable form

	using backtrace_symbols()



	Demanging GCC C++ Symbols

	We're almost done. We just need to make the POSIX output

	more readable. Demangling gcc symbols is easy:



	You have to allocate the buffer for the unmangled name

yourself. In the above example we just grab a small buffer of

the stack. The entire function is below. It has a bunch of

uninteresting string parsing code that parses out the mangled

names, passes them to __cxa_demangle() , and prints them out.

It does it slightly differently for Linux and OsX.

*/



/* Modification: 2016.01; USC

*   Fixed cygwin compilation error w/ exception handling */
/* Modification: 2016.11; USC
 Changed code to print function names in the call trace.*/

#if defined(UNIX) && !defined(CYGWIN)

#include <execinfo.h>

#include <errno.h>
#include <cstring>

#include <iostream>

#include <cxxabi.h>

#include <dlfcn.h>

void printStackTrace( FILE *out, unsigned int max_frames )

{

	fprintf ( out, "stack trace:\n" );
	//Modification: 2018.05: Unused argument warning in GUI
    UNUSED(max_frames);



	// storage array for stack trace address data

	void * addrlist[ MAX_CALL_FRAMES + 2 ];



	// retrieve current stack addresses

	unsigned int addrlen = backtrace( addrlist, sizeof ( addrlist ) / sizeof ( void * ));

	if ( addrlen == 0 )

	{

		fprintf ( out, "No stack trace available.\n" );

		return ;

	}
	
	char ** symbollist = backtrace_symbols( addrlist, addrlen );
	
	#ifdef DARWIN
	

	size_t funcnamesize = 1024;

	char funcname[1024];

	// iterate over the returned symbol lines. skip the first, it is the

	// address of this function.

	for ( unsigned int i = 4; i < addrlen; i++ )

	{

		char * begin_name = NULL;

		char * begin_offset = NULL;

		char * end_offset = NULL;

		// find parentheses and +address offset surrounding the mangled name

		// OSX style stack trace

		for ( char *p = symbollist[i]; *p; ++p )

		{

			if (( *p == '_' ) && ( *(p-1) == ' ' ))

				begin_name = p-1;

			else if ( *p == '+' )

				begin_offset = p-1;

		}



		if ( begin_name && begin_offset && ( begin_name < begin_offset ))

		{

			*begin_name++ = '\0' ;

			*begin_offset++ = '\0' ;

			// mangled name is now in [begin_name, begin_offset) and caller

			// offset in [begin_offset, end_offset). now apply

			// __cxa_demangle():

			int status;

			char * ret = abi::__cxa_demangle( begin_name, &funcname[0], &funcnamesize, &status );

			if ( status == 0 )

			{

				strcpy( funcname, ret ); // use possibly realloc()-ed string

				fprintf ( out, " %-30s %-40s %s\n" , symbollist[i], funcname, begin_offset );

			} else {

				// demangling failed. Output function name as a C function with

				// no arguments.

				fprintf ( out, " %-30s %-38s() %s\n" , symbollist[i], begin_name, begin_offset );

			}
		}

		//#else // !DARWIN - but is posix

			// not OSX style

			// ./module(function+0x15c) [0x8048a6d]
			//Addition 2016.11
			// resolve addresses into strings containing "filename(function+address)",

			// Actually it will be ## program address function + offset

			// this array must be free()-ed

			//Removed this code in 11.26.2016 USC FALL

			/*for ( char *p = symbollist[i]; *p; ++p )

			{

				if ( *p == '(' )

					begin_name = p;

				else if ( *p == '+' )

					begin_offset = p;

				else if ( *p == ')' && ( begin_offset || begin_name ))

					end_offset = p;

			}



			if ( begin_name && end_offset && ( begin_name < end_offset ))

			{

				*begin_name++ = '\0' ;

				*end_offset++ = '\0' ;

				if ( begin_offset )

					*begin_offset++ = '\0' ;

				// mangled name is now in [begin_name, begin_offset) and caller

				// offset in [begin_offset, end_offset). now apply

				// __cxa_demangle():

				int status = 0;

				char * ret = abi::__cxa_demangle( begin_name, funcname, &funcnamesize, &status );

				char * fname = begin_name;

				if ( status == 0 )

					fname = ret;

				if ( begin_offset )

				{

					fprintf ( out, " %-30s ( %-40s + %-6s) %s\n" , symbollist[i], fname, begin_offset, end_offset );

				} else {

					fprintf ( out, " %-30s ( %-40s %-6s) %s\n" , symbollist[i], fname, "" , end_offset );

				}

			#endif // !DARWIN - but is posix

			} else {

				// couldn't parse the line print the whole line.

				fprintf (out, " %-40s\n" , symbollist[i]);

			}*/

	}
	#else

	for (unsigned int i = 1; i < addrlen - 1; i++) {

		char * begin_name = NULL;

		char * begin_offset = NULL;

		char * end_offset = NULL;

		int j = 0;

		char * func_namestr = (char*)malloc(sizeof(char) * (strlen(symbollist[i]) + 1 ));

		char * freeptr = func_namestr;

		for ( char *p = symbollist[i]; *p; ++p )

		{	

			if ( *p == '(' )

				begin_name = p;

			else if ( *p == '+' )

				begin_offset = p;

			else if ( *p == ')' && ( begin_offset || begin_name ))

				end_offset = p;

			if ( (begin_name == NULL) ^ (begin_offset == NULL)) {

				func_namestr[j++] = *p;

			}

		}


		func_namestr[j] = '\0';

		func_namestr = func_namestr+1;


		if ( begin_name && end_offset && ( begin_name < end_offset ))

		{

			*begin_name++ = '\0' ;

			*end_offset++ = '\0' ;

			if ( begin_offset )

				*begin_offset++ = '\0' ;

			// mangled name is now in [begin_name, begin_offset) and caller

			// offset in [begin_offset, end_offset). now apply

			// __cxa_demangle():

			int status = 0;

			char * ret = abi::__cxa_demangle( func_namestr, NULL, NULL, &status );

			char * fname = begin_name;

			if ( status == 0 ){

				char* name;

				name = strtok(ret, "(");

				fname = name;

			}

			if ( begin_offset && strlen(fname) > 0 )

			{

				fprintf ( out, " %-30s ( %-40s + %-6s) %s\n" , symbollist[i], fname, begin_offset, end_offset );

			} else {

				fprintf ( out, " %-30s ( %-40s %-6s) %s\n" , symbollist[i], fname, "" , end_offset );

			}

		} else {

		// couldn't parse the line print the whole line.

			fprintf (out, " %-40s\n" , symbollist[i]);
		}				

		free(freeptr);	

	}

	#endif
	


	free (symbollist);
	


}

#endif



/*

	On Linux the output looks something like this:



kxf_test: src/posix/kxf_pathos.cpp:824: virtual bool

KxfPath::listFiles(

KxVector&, const KxfPath&, bool) const: Assertion '0' failed.

Caught signal 6 (SIGABRT)

stack trace:

/lib/libc.so.6 ( abort + 0x180 ) [0x7f1b698770]

/lib/libc.so.6 ( __assert_fail + 0xf1 ) [0x7f1b698762b1]

./bin/linux-x86_64/debug/kxf_test ( ) [0x435a5d]

./bin/linux-x86_64/debug/kxf_test ( testPath() + 0x29b4)

[0x415748]

./bin/linux-x86_64/debug/kxf_test ( doTest(char const*, void

(*)()) + 0xc8 ) [0x424ce9]

./bin/linux-x86_64/debug/kxf_test ( main + 0xe4 )

[0x424e42]

/lib/libc.so.6 ( __libc_start_main + 0xfd ) [0x7f1b69869c4d]

./bin/linux-x86_64/debug/kxf_test ( ) [0x412cd9]

*/



/*

Windows

Getting and printing the stack trace on Windows is a

nightmare. To start with the functions needed to dump the

stack are not available by default, but must be loaded in using

dbghelp.dll or imagehlp.dll . The stack dumping function

itself has changed from StackWalk to StackWalk64 . Finding

your symbols themselves is complex.

I use a prebuilt stack dumping class called StackWalker,

modified somewhat to remove stuff I don’t need.

You could use the StackWalker unmodified, and derive from

that class to change the output. I just simplified the code to

make the changes I need. If you use StackWalker, then

dumping a stack trace becomes pretty easy:

*/



// #include "StackWalker.h"

/*  COMMENTED OUT FOR NOW, Randy



Unlike the POSIX version, this one will print your c++ stack

symbols demangled by default, which is nice.



static inline void printStackTrace( FILE *out = stderr )

{

	fprintf ( out, "stack trace:\n" );

	StackWalker sw;

	sw.ShowCallstack();

}

*/



/*

	Example Windows Stack Dump



Caught signal 22 (SIGABRT)

stack trace:

f:\ dd \vctools\crt_bld\self_x86\crt\src

\abort.c (74): abort

f:\ dd \vctools\crt_bld\self_x86\crt\src

\assert.c (336): _wassert

c:\ users \rafael\desktop\milk2\kxf\src\win32

\kxf_pathos.cpp (740): KxfPath::listFiles

c:\ users \rafael\desktop\milk2\kxf\src\kxf_test

\test_path.cpp (213): testPath

c:\ users \rafael\desktop\milk2\kxf\src\kxf_test

\kxf_test.cpp (25): doTest

c:\ users \rafael\desktop\milk2\kxf\src\kxf_test

\kxf_test.cpp (55): main

f:\ dd \vctools\crt_bld\self_x86\crt\src

\crt0.c (278): __tmainCRTStartup

f:\ dd \vctools\crt_bld\self_x86\crt\src

\crt0.c (189): mainCRTStartup

75A2339A (kernel32): (filename not

available): BaseThreadInitThunk

77679EF2 (ntdll): (filename not

available): RtlInitializeExceptionChain

77679EC5 (ntdll): (filename not

available): RtlInitializeExceptionChain

*/



//================================================================================

//

//		Exception		Message			Handler

//

//================================================================================



/*!

* 1. Function Description: 

*    Sets a string as a multiline Exception information message 

*

* 2. Parameters:

*    outMsg:	IN/OUT	reference to string to set with Exception info

*    context:	IN		where in UCC code is the Stack Dump wanted

*    threadIdx:	IN		which thread wanted the Stack Dump

*    exType:	IN		what kind of Exception

*    ex_what:	IN		some text provided by code that threw the Exception

*

* 3. Creation Time and Owner: 

*    Version 2015.12

*/

void FormatExceptionMsg( string & outMsg, const unsigned int context, const unsigned int threadIdx, 

						const unsigned int exType, const string ex_what )

{

	

	outMsg = "Error: there was a";

	switch( exType )

	{

	case EXCEPTION_DID_NOT_HAPPEN:

		// Could be Stack Dump for any reason but an Exception.

		outMsg = "";

		break;

	case EXCEPTION_OVERFLOW_ERROR: 

		// this executes if   std::overflow_error (same type rule)

		outMsg += "n Overflow";

		break;

	case EXCEPTION_UNDERFLOW_ERROR:

		// this executes if   std::underflow_error (base class rule)

		outMsg += "n Underflow";

		break;

	case EXCEPTION_RANGE_ERROR:

		// this executes if   std::range_error

		outMsg += " Range";

		break;

	case EXCEPTION_SYSTEM_ERROR:

		// this executes if   std::system_error

		outMsg += " System";

		break;

	case EXCEPTION_RUNTIME_ERROR:

		// this executes if   std::runtime_error (base class rule)

		outMsg += " Runtime";

		break;

	case EXCEPTION_DOMAIN_ERROR:

		// this executes if   std::domain_error (base class rule)

		outMsg += EXCEPTION_DOMAIN_ERROR;

		exception_msg = " Domain";

		break;

	case EXCEPTION_LENGTH_ERROR:

		// this executes if   std::length_error (base class rule)

		outMsg += " Length";

		break;

	case EXCEPTION_INVALID_ARG_ERROR:

		// this executes if   std::invalid_argument (base class rule)

		outMsg += "n Invalid Argument";

		break;

	case EXCEPTION_OUT_OF_RANGE_ERROR:

		// this executes if   std::out_of_range (base class rule)

		outMsg += "n Out of Range";

		break;

	case EXCEPTION_LOGIC_ERROR:

		// this executes if   std::logic_error (base class rule)

		outMsg += " Logic";

		break;

	case EXCEPTION_BAD_ALLOC:

		// this executes if   std::bad_alloc (base class rule)

		outMsg += " Bad allocation.  LOW Memory! ";

		break;

	case EXCEPTION_STD_EXCEPTION:

		// this executes if   std::exception (base class rule)

		outMsg += " std namespace";

		break;

	case EXCEPTION_NOT_SPECIFIC:

		// this executes if above throws std::string or int or any other unrelated type

		// Some kind of severe error that prevented normal finish

		if ( EXCEPTION_HANDLER_APP == context )

			outMsg = "Error: a general exception occurred";

		else

		{



		}

		break;

	default:

		outMsg = "Internal Error: Unknown exception type";

		return;

	}



	if ( FIRST_SPECIFIC_EXCEPTION <= exType )

		outMsg += " error exception";



	if ( EXCEPTION_HANDLER_THREAD == context )

	{

		if ( MAX_UCC_THREAD_COUNT > threadIdx )

		{

			string num;

			IntToStr( (int)threadIdx, num );

			outMsg += " in thread " + num + "\n";

		}

		else

			outMsg += " in the Main thread.\n";

	}

	else if ( EXCEPTION_HANDLER_APP == context )

	{



		outMsg += ".  Please try again.\n";

	}

	else

	{

		// Parser !

		outMsg += ".\n";

	}



	outMsg += ex_what + "\n";

}





//================================================================================

//

//		UCC		Stack	Dump		Will work on any compiler/platform

//

//	3 Downsides:

//		Not as detailed or accurate as Native approach

//		Requires calling some helper macros in language parsers or other places

//		VERY slight decrease in performance due to a few extra string operations

//

//================================================================================



void FormatStackCalls( string & result, const CCodeCounter * pCounter )

{

	string	lineNumStr1, lineNumStr2, lineNumStr3, lineNumStr4;

	IntToStr( pCounter->currentCode1, lineNumStr1 );

	IntToStr( pCounter->currentCode2, lineNumStr2 );

	IntToStr( pCounter->currentCode3, lineNumStr3 );

	IntToStr( pCounter->currentCode4, lineNumStr4 );



	result += "\nin: " + pCounter->currentFunc1 + " call to " + pCounter->currentStep1 + " after line " + lineNumStr1;

	

	if ( pCounter->currentFunc1.compare( pCounter->currentFunc2 ) == 0 )

		return;

	result += "\nin: " + pCounter->currentFunc2 + " call to " + pCounter->currentStep2 + " after line " + lineNumStr2;

	

	if ( pCounter->currentFunc2.compare( pCounter->currentFunc3 ) == 0 )

		return;

	result += "\nin: " + pCounter->currentFunc3 + " call to " + pCounter->currentStep3 + " after line " + lineNumStr3;

	

	if ( pCounter->currentFunc3.compare( pCounter->currentFunc4 ) == 0 )

		return;

	result += "\nin: " + pCounter->currentFunc4 + " call to " + pCounter->currentStep4 + " after line " + lineNumStr4;

	

}



/*!

* 1. Function Description: 

*    Sets a string with a possibly LONG formatted Stack Dump multiline message

*

* 2. Parameters:

*    dumpStr:	IN/OUT	reference to string to set

*    context:	IN		where in UCC code is the Stack Dump wanted

*    threadIdx:	IN		which thread wanted the Stack Dump

*    exType:	IN		what kind of Exception

*    ex_what:	IN		some text provided by code that threw the Exception

*    pCounter:	IN		pointer to base class of Parser that had the Exception

*    useNative:	IN		use Native platform support for Stack Dump

*

* 3. Creation Time and Owner: 

*    Version 2015.12

*/

void FormatStackDump( string & stackStr, const unsigned int context, 

					const unsigned int threadIdx, const unsigned int exType, 

					const string ex_what, const CCodeCounter * pCounter,

					const bool useNative )

{

        //Modification: 2018.01: isLowMemory Not used
	//bool	isLowMemory = false;
	UNUSED(exType); //Modification: 2018.05: Unused variable warning in GUI

    if (ex_what == " "){
        //DO NOTHING. Warning fix 11.28.16
    }

	// Check for the REALLY Exciting Exceptions

        //Modification: 2018.01: isLowMemory Not used
	/*if ( exType == EXCEPTION_BAD_ALLOC )

		isLowMemory = true;
        */



	stackStr = "!!! UCC Stack Dump ...\n";



	time_t	timeNow;

	time( &timeNow );



	// double	total_seconds = difftime( timeNow, timeStart ) + 0.5;

	string	runningTime;

	ShowTiming( runningTime, timeStart, timeNow, true, false, false );



	stackStr += "Command line:\n" + cmdLine + "\n";

	stackStr += runningTime + "\n";



	if ( isNativeStackDumpAvailable && useNative )

	{

		// Call Native support here



		// for now fall through

	}



	string	threadIdxStr;

	IntToStr( (int)threadIdx, threadIdxStr );





	string	phyLineNumStr;

	string	logLineNumStr;



	switch( context )

	{

		case EXCEPTION_HANDLER_APP:



			break;

		case EXCEPTION_HANDLER_THREAD:

			// TODO:  Add some details for Thread context Stack Dumps





			break;

		case EXCEPTION_HANDLER_PARSER:

			stackStr += pCounter->language_name;

			stackStr += "  parser is running from ";

			if ( MAIN_THREAD_INDEX == threadIdx )

				stackStr += "the Main thread.\n";

			else

			{

				stackStr += "Thread " + threadIdxStr + "\n";

			}

			stackStr += "File being parsed:\n";

			stackStr += pCounter->parse_file_name + "\n";

			

			LongToStr( (long)pCounter->currentPhyLine, phyLineNumStr );

			stackStr += "Physical line " + phyLineNumStr + " being parsed:\n";

			stackStr += pCounter->parse_physical_line + "\n";

			stackStr += pCounter->parse_physical_line_comments + "\n";

			

			LongToStr( (long)pCounter->currentLSrcLine, logLineNumStr );

			stackStr += "Logical (source) line " + logLineNumStr + " being parsed:\n";

			stackStr += pCounter->parse_logical_line + "\n";

			stackStr += "UCC  " + pCounter->language_name + "  parser code (approximate) Stack Dump:\n";

			stackStr += "CCodeCounter::CountSLOC  (base class method)";

			

			FormatStackCalls( stackStr, pCounter );



			break;

		default:

			stackStr += "Stack Dump interface error.  Invalid context";

			break;

	}



	// Get a fast? check of the Heap  Only works for Windows now.

	int				errorRet;

	//bool			call_again = true; //Modification: 2018.01, call_again not used

	bool			validate = true;

	unsigned int	heap_count = 0;

	unsigned long	start_block_count = 0;

	unsigned long long	start_total_sizes = 0L;



	HeapInUse( errorRet, heap_count, start_block_count, start_total_sizes, validate );

        //Modification: 2018.01 call_again not used. 
	/*if ( ( errorRet == 1 ) || ( errorRet < 0 ) )

		call_again = false;
        */ 



	if ( start_block_count > 1 )

	{

		// Got some valid? Heap stats.

		string	heapStr = "\nHeap Memory in Use:\n";

		string	heapCountStr;

		IntToStr( (int)heap_count, heapCountStr );

		string	blkCountStr;

		LongToStr( (long)start_block_count, blkCountStr );

		string	totalMemStr;

		LongLongToStr( (long long)start_total_sizes, totalMemStr );

		heapStr += "There are " + heapCountStr + " Heaps.\n";

        heapStr += "There are " + blkCountStr + " blocks for a total of " + totalMemStr + " Bytes of Memory.\n";

		stackStr += heapStr;

	}

    //Warning Fixes 11.28.16. DO NOT USE THESE VARIABLES AFTER THIS.
    //(void) call_again;
    //(void) isLowMemory;

	return;

}





/*!

* 1. Function Description: 

*    Sets a string with a possibly LONG formatted Stack Dump multiline message

*

* 2. Parameters:

*    dumpStr:	IN/OUT	reference to string to set

*    context:	IN		where in UCC code is the Stack Dump wanted

*    threadIdx:	IN		which thread wanted the Stack Dump

*    exType:	IN		what kind of Exception

*    ex_what:	IN		some text provided by code that threw the Exception

*    pCounter:	IN		pointer to base class of Parser that had the Exception

*

* 3. Creation Time and Owner: 

*    Version 2015.12

*/

void _prv_StackDump( string & dumpStr, const unsigned int context, 

				const unsigned int threadIdx, const unsigned int exType, 

				const string ex_what, const CCodeCounter * pCounter )

{

	bool	isLowMemory = false;



	// Check for the REALLY Exciting Exceptions

	if ( EXCEPTION_BAD_ALLOC == exType )

		isLowMemory = true;



	if ( EXCEPTION_STD_EXCEPTION == exType )

	{

		// STD library Exception

		if ( ex_what.compare( "bad allocation" ) == 0 )

			isLowMemory = true;



	}



	if ( isLowMemory )

	{

		// Put out at least 1 message in case LOW MEMORY or some other severe condition.

		printf( "\n     S t a c k    D u m p :   type %d  msg: %s\n", exType, ex_what.c_str() );

	}

	else

	{

		// Do NOT call FirstExceptionMessage from here if LOW Memory condition.

		FirstExceptionMessage( context, threadIdx, exType, ex_what, pCounter );

	}



	dumpStr = "";



	string	threadIdxStr;

	IntToStr( (int)threadIdx, threadIdxStr );



	string exFormMsg;

	FormatExceptionMsg( exFormMsg, context, threadIdx, exType, ex_what );



	bool	dumpAllowed = true;

	switch( context )

	{

		case EXCEPTION_HANDLER_APP:

			dumpStr = "UCC is doing a Stack Dump because:\n";

			dumpStr += exFormMsg;

			break;

		case EXCEPTION_HANDLER_THREAD:

			dumpStr = "UCC Thread ";

			dumpStr += threadIdxStr;

			dumpStr += " is doing a Stack Dump because:\n";

			dumpStr += exFormMsg;

			break;

		case EXCEPTION_HANDLER_PARSER:

			dumpStr = "UCC  ";

			dumpStr += pCounter->language_name;

			dumpStr += "  parser is doing a Stack Dump because:\n";

			dumpStr += exFormMsg;

			break;

		default:

			dumpStr = "Stack Dump interface error.  Invalid context";

			dumpAllowed = false;

			break;

	}



	string	stackStr;

	if ( dumpAllowed )

	{

		FormatStackDump( stackStr, context, threadIdx, exType, ex_what, pCounter, true );

		dumpStr += stackStr;

	}



	prev_dump_msg = dumpStr;

	return;

}





// THINK LOW Memory condition!  This is called internally early to get at least some information and suggestion to User.

void _prv_ExceptionMessage( const unsigned int	context,

							const unsigned int  threadIdx,		// use MAIN_THREAD_INDEX or actual threadIdx

							const unsigned int	exType, 

							const string		ex_what,

							const CCodeCounter * pCounter )		// Only need this for Parser dumps

{

	bool	isLowMemory = false;

	bool	mustExit = false;



	string	tempMsg;



	// Check for the REALLY Exciting Exceptions

	if ( exType == EXCEPTION_BAD_ALLOC )

		isLowMemory = true;



	if ( exType == EXCEPTION_STD_EXCEPTION )

	{

		// STD library Exception

		if ( ex_what.compare( "bad allocation" ) == 0 )

			isLowMemory = true;



	}



// Could be this was called from an Exception handler already.

// So catch any Exceptions here quietly.  These are not the original problem's location.

try

{

	if ( false == isLowMemory )

		return;



	printf( "\n   LOW on RAM exception " );

	if ( EXCEPTION_HANDLER_PARSER == context )

	{

		printf( "while parsing:\n%s\n", 

			pCounter->parse_file_name.c_str() );

	}

	else 

	if ( EXCEPTION_HANDLER_THREAD == context )

	{

		printf( "from a Thread.\n" );

	}

	else

	{

		printf( "from UCC application level.\n" );

	}



	// 





	// Output using predefined strings to help prevent possible recursive LOW Memory exceptions.

	// Also use C style code rather than C++ objects to reduce Memory use here!

	if ( isDiff && duplicate_threshold >= 0.0 )

	{

		printf( "\n\nUCC is LOW on Memory running Difference with Duplication checking.\nYou may try Differencing alone and do separate Duplicate checking for just 1 Baseline at a time.\n" );

	}

	else if ( duplicate_threshold >= 0.0 )

	{

		printf( "\n\nUCC is LOW on Memory and running Duplicate checking.\nYou can try a smaller list of files to process.\n" );

	}

	else if ( workThreadsCount >= MIN_UCC_THREAD_COUNT )

	{

		printf( "\n\nUCC is LOW on Memory and running with Threads.\nYou can try using fewer or NO Threads.\nOr you can keep Threads but have a smaller list of files to process.\n" );

	}

	else

		printf( "\n\nUCC is LOW on Memory.\nYou can try a smaller list of files to process.\n" );



	// Show the total number of files, Largest file, total sizes of all files.

	// Exactly where we are with processing those may be tricky to get here.

	if ( isDiff )

	{

		printf( "Baseline A has  %ld  files\nBaseline B has  %ld  files\n   Total files  %ld\n", 

				(long)numFilesInA, (long)numFilesInB, (long)numFilesInA + (long)numFilesInB );

		printf( "Baseline A largest file is  %lld  Bytes\nBaseline B largest file is  %lld  Bytes\n", 

				(long long)largestFileSizeA, (long long)largestFileSizeB );

	//	printf( "If all files were loaded into RAM (as previous UCC versions did):\n" );

		printf( "Baseline A total sizes all files  %lld  Bytes\nBaseline B total sizes all files  %lld  Bytes.\n               Total for both is  %lld\n", 

			(long long)totalFileSizesA, (long long)totalFileSizesB, (long long)totalFileSizesA + (long long)totalFileSizesB );

	}

	else

	{

		ShowFileSetStats( true );

	}





	int				errorRet;

	bool			validate = true;

	unsigned int	heap_count = 0;

	unsigned long	start_block_count = 0L;

	unsigned long long	start_total_sizes = 0L;



	HeapInUse( errorRet, heap_count, start_block_count, start_total_sizes, validate );



	if ( start_block_count > 1 )

	{

		// Got some valid? Heap stats.

		string	heapStr = "\nHeap Memory in Use:\n";

		string	heapCountStr;

		IntToStr( (int)heap_count, heapCountStr );

		string	blkCountStr;

		LongToStr( (long)start_block_count, blkCountStr );

		string	totalMemStr;

		LongLongToStr( (long long)start_total_sizes, totalMemStr );

		heapStr += "There are " + heapCountStr + " Heaps.\n";

		heapStr += "There are " + blkCountStr + " blocks for a total of " + totalMemStr + " Bytes of Memory.\n";



		cout << heapStr;

		userIF->AddError( heapStr, true );

	}



	// Try calling a Stack Dump here.  May not be able to finish.

	string dump;

	_prv_StackDump( dump, context, threadIdx, exType, ex_what, pCounter );

	userIF->AddError( dump, true );

}

// Could be this was called from an Exception handler already.

// So catch any Exceptions here quietly.  These are not the original problem's location.

catch(const std::bad_alloc& e)		

{									// LOW on RAM memory ! ! !

	// this executes if above throws std::bad_alloc (base class rule)

	//retVal = EXCEPTION_BAD_ALLOC;

	tempMsg = e.what();

	mustExit = true;

} 

catch(const std::exception& e) 

{

	// this executes if above throws std::exception (base class rule)

	//retVal = EXCEPTION_STD_EXCEPTION;

	tempMsg = e.what();

	mustExit = true;

} 

catch(...) 

{

	// this executes if above throws std::string or int or any other unrelated type

	//retVal = EXCEPTION_NOT_SPECIFIC;		// Some kind of severe error that prevented normal finish

	tempMsg = "";

	mustExit = true;

}



	if ( mustExit )

	{

		printf( "\nGot another Exception handling this Exception.  Exiting now!\n" );

		exit( (int)exType );

	}



	return;

}





static	bool	processingException = false;



// THINK LOW Memory condition!  Call this early to get at least some information and suggestion to User.

void FirstExceptionMessage( const unsigned int	context,

							const unsigned int  threadIdx,		// use MAIN_THREAD_INDEX or actual threadIdx

							const unsigned int	exType, 

							const string		ex_what,

							const CCodeCounter * pCounter )		// Only need this for Parser dumps

{

	// THINK possible multiple LOW Memory exceptions.

	// The first exception is processed.  

	// Any other exceptions that happens during processing of first are ignored.



	if ( true == processingException )

		return;		// Sorry this was NOT the first!

	else

		processingException = true;



	_prv_ExceptionMessage( context,

							threadIdx,		// MAIN_THREAD_INDEX or actual threadIdx

							exType, 

							ex_what,

							pCounter );



	processingException = false;

	return;

}





/*!

* 1. Function Description: 

*    Sets a string with a possibly LONG formatted Stack Dump multiline message

*    Public interface to rest of UCC so avoids handling more than 1 Exception at a time.

*

* 2. Parameters:

*    dumpStr:	IN/OUT	reference to string to set

*    context:	IN		where in UCC code is the Stack Dump wanted

*    threadIdx:	IN		which thread wanted the Stack Dump

*    exType:	IN		what kind of Exception

*    ex_what:	IN		some text provided by code that threw the Exception

*    pCounter:	IN		pointer to base class of Parser that had the Exception

*

* 3. Creation Time and Owner: 

*    Version 2015.12

*/

void StackDump( string & dumpStr, const unsigned int context, 

				const unsigned int threadIdx, const unsigned int exType, 

				const string ex_what, const CCodeCounter * pCounter )

{

	stack_dump_count++;



	// Public interface to rest of UCC so avoid more than 1 Exception at a time

	if ( true == processingException )

		return;

	else

		processingException = true;



	_prv_StackDump( dumpStr, 

					context, 

					threadIdx,

					exType, 

					ex_what,

					pCounter );



	processingException = false;



	return;

}



// Get some info about Baseline A or B

//

// This works 2 ways:

//   IF Low Memory uses printf to console as using more std::string calls is not advised.

//   If not Low Memory then grows a provided string.

//

void ShowFileSetStats( const bool useListA, string * pString )

{

	long numFiles         = (long)numFilesInA;

	long long largestFile = (long long)largestFileSizeA;

	long long totalSizes  = (long long)totalFileSizesA;

	if ( ! useListA )

	{

		numFiles    = (long)numFilesInB;

		largestFile = (long long)largestFileSizeB;

		totalSizes  = (long long)totalFileSizesB;

	}



	if ( NULL == pString )

	{

		// LOW Memory condition so avoid std::string calls as that is likely in trouble.

		printf( "File list has  %ld  files.\n", numFiles );

		printf( "Largest file is  %lld  Bytes.\n", largestFile );

		// printf( "If all files were loaded into RAM (as previous UCC versions did):\n" );

		printf( "Total of all files size is  %lld  Bytes.\n", totalSizes );

	}

	else

	{

		// OK to grow the provided string

		string	msg, numBuf;

		LongToStr( numFiles, numBuf );

		msg += "File list has  " + numBuf +"  files.\n";

			

		LongLongToStr( largestFile, numBuf );

		msg += "Largest file is  " + numBuf + "  Bytes.\n";



		// printf( "If all files were loaded into RAM (as previous UCC versions did):\n" );



		LongLongToStr( totalSizes, numBuf );

		msg += "Total of all files size is  " + numBuf + "  Bytes.\n";

		

		*pString += msg;

	}

}





//================================================================================

//

//			Platform		Specific

//

//================================================================================



//void NativeStackDump()

//{



/* Modification: 2016.01; USC

*   Fixed cygwin compilation error w/ exception handling */

#if defined(UNIX) && !defined(CYGWIN)



// Sample to support clang not sure about g++

void printStackTrace() 

{

	void *returnAddresses[500];



	// use backtrace API

	int depth = backtrace( returnAddresses, sizeof returnAddresses / sizeof *returnAddresses );



	printf( "stack depth = %d\n", depth );



	char **symbols = backtrace_symbols( returnAddresses, depth );



	for ( int i = 0; i < depth; ++i ) 

	{

		printf( "%s\n", symbols[i] );

	}



	free( symbols );

}

#endif		//	UNIX





/*

#ifdef	WIN32

LONG	WINAPI	unhandledExceptionFilter( EXCEPTION_POINTERS * exInfo )

{

	exInfo;





	return EXCEPTION_CONTINUE_SEARCH;	// Not reached

}







#endif

*/





/*!

* 1. Function Description: 

*    Initialize support for Stack Dump.

*		This determines if there is native Stack Dump support (OS or compiler)

*

* 2. Parameters:

*

* 3. Creation Time and Owner: 

*    Version 2015.12

*/

void Init_StackDump()

{

	// Reset stact trace count and 

	// install appropriate signal handler if available

	g_KxStackTrace.stack_trace_count = 0;

/* Modification: 2016.01; USC

*   Fixed cygwin compilation error w/ exception handling */	

#if defined(WIN32) || defined(CYGWIN)

	g_KxStackTrace.Win32_KxStackTrace();

#else

	g_KxStackTrace.POSIX_KxStackTrace();

#endif



	stack_dump_count           = 0;

	isNativeStackDumpAvailable = false;



	ClearSummaryMsgCounts();



/* Modification: 2016.01; USC

*   Fixed cygwin compilation error w/ exception handling */

#if defined(WIN32) || defined(CYGWIN)



	// Some exceptions bypass the cross platform catch blocks elsewhere

	// So use a native Windows exception handler to get control

//	SetUnhandledExceptionFilter( unhandledExceptionFilter );

	



#endif



// Uncomment this to do a quick test of the Signal handler

//#define		BAD_CODE



#ifdef		BAD_CODE



	int * pInt = NULL;

	int k = *pInt;			// Use of NULL pointer  SIGSEGV on Windows



	int num = 1;

	int denom = 0;

	int j = num / denom;	// Should be SIGFPE  divide by Zero or some such



#endif



	return;

}







// This does a quick? survey of the Heap(s) giving number of blocks and total committed sizes

// Implemented for Windows

void	HeapInUse( int & errorRet, unsigned int & heap_count, unsigned long & block_count, unsigned long long & total_sizes, 

					bool & validate, const bool in_use )

{

/* Modification: 2016.01; USC

*   Fixed cygwin compilation error w/ exception handling */

#if defined(WIN32) || defined(CYGWIN)

	block_count = 0;

	total_sizes = 0L;



	bool				is_valid = true;

	unsigned int		region_count = 0;

	unsigned int		uncommitted_count = 0;

	PROCESS_HEAP_ENTRY	heapEntry;



	DWORD		num_of_heaps  = GetProcessHeaps( 0, NULL );

	heap_count = (unsigned int) num_of_heaps;

	HANDLE *	pHeapsHandles = (HANDLE *)malloc( sizeof( HANDLE ) * num_of_heaps );

	if ( NULL == pHeapsHandles )

	{

		cout << "\nERROR: HeapInUse unable to Allocate array of Handles\n";

		if ( validate )

			validate = false;

		errorRet = -1;

		return;

	}



	DWORD	num_of_heaps2 = GetProcessHeaps( num_of_heaps, pHeapsHandles );

	if ( num_of_heaps2 != num_of_heaps )

	{

		// The below approach will NOT work, get out

		cout << "\nERROR: HeapInUse logical ERROR.\n";

		free( pHeapsHandles );

		if ( validate )

			validate = false;

		errorRet = -2;

		return;

	}



	BOOL	walk_result;

	HANDLE	hHeap;

	for ( DWORD j = 0; j < num_of_heaps; j++ )

	{

		memset( &heapEntry, 0, sizeof( heapEntry ) );

		hHeap = pHeapsHandles[ j ];

		BOOL lock_result = HeapLock( hHeap );

		if ( FALSE == lock_result )

		{

			cout << "\nERROR: HeapInUse unexpected ERROR: unable to Lock a Heap, set INVALID and skipping\n";

			is_valid = false;

			continue;

		}



		if ( validate )

		{

			if ( HeapValidate( hHeap, 0, 0 ) == 0 )

			{

				cout << "\nERROR: HeapInUse unexpected result.  Heap is INVALID.  Corruption ?\n";

				is_valid = false;



				// For now fall through and try to get other info

			}

		}



		do		// Get Heap details

		{

			walk_result = HeapWalk( hHeap, &heapEntry );

			if ( TRUE == walk_result )

			{

				if ( heapEntry.wFlags & PROCESS_HEAP_ENTRY_BUSY )

				{

					block_count++;

					if ( in_use )

						total_sizes += (unsigned long long)( heapEntry.cbData );

				}

				else if ( heapEntry.wFlags & PROCESS_HEAP_REGION )

				{

					region_count++;

					block_count++;

					// No need to go through the Region linked list

					total_sizes += (unsigned long long)( heapEntry.Region.dwCommittedSize );

				}

				else if ( heapEntry.wFlags & PROCESS_HEAP_UNCOMMITTED_RANGE ) 

				{

					// Uncommitted range

					uncommitted_count++;

				}

				else 

				{

					// Block

					block_count++;

				}



				if ( false == in_use )

					total_sizes += (unsigned long long)( heapEntry.cbData );

			}

		} while ( TRUE == walk_result );



		HeapUnlock( hHeap );

	}

		

	free( pHeapsHandles );



	if ( validate )

		validate = is_valid;



	errorRet = 0;



#else	// Add support for another OS here if wanted



	errorRet = 1;		// NOT IMPLEMENTED
	UNUSED(in_use); //Modification: 2018.05 : Unused variable warning in GUI



	// Assign defaults but not even realistic.

	heap_count  = 0;

	block_count = 1;

	total_sizes = 2;

	validate = false;



#endif



	return;

}





/*

Implementation Notes:



This module was born out of need to get SOME improvement for UCC severe error handling.



C++ Exceptions



Signals



Stack Dumps

Generic (works on any platform but not as accurate as Platform specific)



Mostly cross platform (Linux, Mac, Windows)



Heap use summary

Windows

Platform Specific







*/

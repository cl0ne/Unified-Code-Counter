//! The main procedure.  OS entry point.
/*!
* \file main.cpp
*
* This file contains the UCC main procedure OS entry point 
* when doing console style builds and not for Qt style builds.
*
* Changed for UCC 2015_12 release by Randy Maxwell
*	Much more detailed Exception handlers to help find defects
*	Support to get and display Time used of various steps and show files processed / second
*	DEBUG build to easily capture parts of the output as wanted and do Heap checks (Windows)
*/

// Must NOT be a Qt GUI style build
#ifndef QTGUI

#include "UCCBeforeLibraryIncludes.h"    // Modification: 2015.12
#include <cstring>
#include <ctime>
#include <iostream>
// #include <stdexcept>
#include "UCCAfterLibraryIncludes.h"     // Modification: 2015.12

#include "UCCGlobals.h"                  // Modification: 2015_12
#include "UCCFilesOut.h"                 // Modification: 2015_12
#include "DiffTool.h"

// Enable checking various C++ Exception conditions
// Set up this define for your OS and perhaps compiler and linker?
// Windows and Mac can compile but not Linux.  Perhaps another library needed?
#if (defined(_WIN32) && !defined(__MINGW32__))
	#define		ENABLE_EXCEPTION_CLAUSES
	#if (_MSC_VER >= 1600)		// Visual Studio 2010 or higher (or equivalent)
		#define		CLAUSE_SYSTEM_ERROR_OK
	#endif
#endif

#ifdef DARWIN		// MacIntosh OSX
	#define		ENABLE_EXCEPTION_CLAUSES
#endif


/*!
* 1. Function Description:
* Main function.  This is the entry point from the OS.
* This will show final Time used by various steps (if wanted).
* Detailed C++ Exception handling to give some clues to a problem.
* DEBUG support to allow checking for memory leaks (Windows) and console output capture.
*
* 2. Parameters:
* \param argc number of arguments
* \param argv argument list
* \return 0 if no serious errors else error number
*
* 3. Creatation Time: 
*	  Version ?
*     Revised 2015.12   Better Exception/Stack Dump, show Time used, Files/second and Debug 
*/
int main(int argc, char *argv[])
{
//          ALL below until try block are       // Modification: 2015.12
//
	int				retVal = EXCEPTION_DID_NOT_HAPPEN;	// 0 for backwards compatability
	string			exception_msg;
	bool			doDiff = false;
	bool			doDups = false;
	double			duplicate_threshold_used = 0.0;
	unsigned long	files_A_count = 0;
	unsigned long	files_B_count = 0;
        isFuncDiff = false; //Modification 2017.02

	Init_StackDump();		// Modification: 2015.12
	CUtil::InitToLower();	// Initialize to lower helper array

#ifdef _DEBUG                // Modification: 2015.12
	// Wait for User input 
	// to allow Debugger to be attached to this running process
	//
	// Suggest checking for Memory Leaks running Debug build...
	// This is also a good place to note the amount of memory allocated here.
	char keyBuffer[256];

	printf( "Here is where you can attach a Debugger." );
	printf( "\nHit Enter key to continue running in UCC main()\n" );

	//char * pChar = gets( keyBuffer );
        /* Modification: 2017.02; USC
         *   Fixed VS2015 compilation errors */
        char * pChar = gets_s( keyBuffer );
#endif

	// Now that UCC is running without any User input 
	// capture the start of the total running time

	// Change these as you wish
	bool	show_any_times = true;
	bool	show_total_only = false;

	InitTimes();

	try
	{
		bool	showUsage = false;
		string	helpOption;

		// determine whether differencing functionality is needed
		string myArg;
		for (int i = 0; i < argc; i++)
		{
			myArg = argv[i];
			myArg = CUtil::ToLower( myArg );
			if (myArg == "-d")
			{
				doDiff = true;
				break;
			}
			else if (myArg == "?" || myArg == "-help" || myArg == "-h" || myArg == "/?")
			{
				showUsage = true;

				// check whether a help option has been specified
				if (argc > i + 1)
				{
					// set usage option help and Exit
					helpOption = argv[ i + 1 ];
				}
				// Fall through
			}
		}
                //Modification 2017.02
                for (int i = 0; i < argc; i++)
                {
                        myArg = argv[i];
                        if(myArg == "-funcDiff")
                        {
                                isFuncDiff = true;
                                break;
                        }
                }
		if (doDiff)
		{
			// run the DiffTool to include differencing
			DiffTool diffTool;
			diffTool.diffToolProcess(argc, argv);
			duplicate_threshold_used = diffTool.GetDuplicateThreshold();    // Modification: 2015.12

			// Make sure worker Threads are done.  Could be half done due to LOW Memory.
			FinishWorkerThreads();                                          // Modification: 2015.12

			files_A_count = SourceFileA.size();                             // Modification: 2015.12
			CountPhysicalFiles( SourceFileA, files_A_count );               // Modification: 2015.12

			files_B_count = SourceFileB.size();                             // Modification: 2015.12
			CountPhysicalFiles( SourceFileB, files_B_count );               // Modification: 2015.12
                        //Modification 2017.02
                        //Runs function level differencing
                        if(isFuncDiff)
                        {
                            SourceFileA.resize(0);
                            SourceFileB.resize(0);
                            DiffTool funcDiffTool;
                            funcDiffTool.funcDiffProcess(argc, argv);
                        }
		}
		else
		{
			// if no differencing functionality required, just call the UCC
			MainObject mainObject;
			if ( showUsage )
			{
				// Show either specific usage Help or general usage Help and Exit
				if ( helpOption.size() )
					mainObject.ShowUsage( helpOption );
				else
					mainObject.ShowUsage( "" );
			}
			mainObject.MainProcess(argc, argv);
			duplicate_threshold_used = mainObject.GetDuplicateThreshold();  // Modification: 2015.12

			files_A_count = SourceFileA.size();                             // Modification: 2015.12
			CountPhysicalFiles( SourceFileA, files_A_count );               // Modification: 2015.12
		}
	}
//    ALL below here to end of this file are  Modification: 2015.12
//
	// Handle any Exceptions to get as much context info as possible
	// (in a cross platform way).
	// On Windows for example there is an another entire set 
	// (a LOT more than those listed below)
	// of Windows OS related Exceptions that may be thrown.
	// Perhaps the system_error handler below will cover them OK.
	//
	// Idea for this chain of Exception handlers was from cppreference.com docs.
	// More Exception types were found in <stdexcept> module.
	// If you find any that dont compile please just comment out those.
#ifdef	ENABLE_EXCEPTION_CLAUSES
	catch(const std::overflow_error& e) 
	{
		// this executes if above throws std::overflow_error (same type rule)
		retVal = EXCEPTION_OVERFLOW_ERROR;
		exception_msg = e.what();
	}
	catch(const std::underflow_error& e)
	{
		// this executes if above throws std::underflow_error (base class rule)
		retVal = EXCEPTION_UNDERFLOW_ERROR;
		exception_msg = e.what();
	}
	catch(const std::range_error& e)
	{
		// this executes if above throws std::range_error
		retVal = EXCEPTION_RANGE_ERROR;
		exception_msg = e.what();
	}
#ifdef	CLAUSE_SYSTEM_ERROR_OK
	catch(const std::system_error& e)
	{
		// this executes if above throws std::system_error
		retVal = EXCEPTION_SYSTEM_ERROR;
		exception_msg = e.what();
	}
#endif
	catch(const std::runtime_error& e) 
	{
		// this executes if above throws std::runtime_error (base class rule)
		retVal = EXCEPTION_RUNTIME_ERROR;
		exception_msg = e.what();
	} 
	catch(const std::domain_error& e) 
	{
		// this executes if above throws std::domain_error (base class rule)
		retVal = EXCEPTION_DOMAIN_ERROR;
		exception_msg = e.what();
	} 
	catch(const std::length_error& e) 
	{
		// this executes if above throws std::length_error (base class rule)
		retVal = EXCEPTION_LENGTH_ERROR;
		exception_msg = e.what();
	} 	
	catch(const std::invalid_argument& e) 
	{
		// this executes if above throws std::invalid_argument (base class rule)
		retVal = EXCEPTION_INVALID_ARG_ERROR;
		exception_msg = e.what();
	} 
	catch(const std::out_of_range& e) 
	{
		// this executes if above throws std::out_of_range (base class rule)
		retVal = EXCEPTION_OUT_OF_RANGE_ERROR;
		exception_msg = e.what();
	} 
	catch(const std::logic_error& e) 
	{
		// this executes if above throws std::logic_error (base class rule)
		retVal = EXCEPTION_LOGIC_ERROR;
		exception_msg = e.what();
	} 
	catch(const std::bad_alloc& e)
	{										// LOW on RAM memory ! ! !
		// this executes if above throws std::bad_alloc (base class rule)
		retVal = EXCEPTION_BAD_ALLOC;
		exception_msg = e.what();
		FirstExceptionMessage( EXCEPTION_HANDLER_APP, MAIN_THREAD_INDEX, (unsigned int)retVal, exception_msg );
	} 
	catch(const std::exception& e) 
	{
		// this executes if above throws std::exception (base class rule)
		retVal = EXCEPTION_STD_EXCEPTION;
		exception_msg = e.what();
		if ( exception_msg.compare( "bad allocation" ) == 0 )
		{
			// LOW on RAM memory ! ! !
			FirstExceptionMessage( EXCEPTION_HANDLER_APP, MAIN_THREAD_INDEX, (unsigned int)retVal, exception_msg );
		}
	}
#endif		// ENABLE_EXCEPTION_CLAUSES
	catch(...) 
	{
		// this executes if above throws std::string or int or any other unrelated type
		retVal = EXCEPTION_NOT_SPECIFIC;		// Some kind of severe error that prevented normal finish
		exception_msg = "no exception details available";
	}

	if ( retVal )	// Non Zero if an Exception was caught
	{
		string	dump;
		unsigned int ex_type = (unsigned int)retVal;
		StackDump( dump, EXCEPTION_HANDLER_APP, MAIN_THREAD_INDEX, ex_type, exception_msg );
		printf( "\n%s\n", dump.c_str() );


		if ( workThreadsCount >= MIN_UCC_THREAD_COUNT )
		{
			printf( "Because %d extra threads were also running, it is possible that\n", workThreadsCount );
			printf( "this Exception happened due to a side effect from running extra threads.\n" );
			printf( "As a workaround you should try using fewer or no threads\n" );
		}
	}

	// Get the Time to show
	time( &timeEnd );
	double	total_seconds = difftime( timeEnd, timeStart ) + 0.5;

	string	time_str;
	char buf[MSG_STR_BUF_SIZE];

	// Use more Secure C library APIs if available
#ifdef	_MSC_VER
	strcpy_s( buf, MSG_STR_BUF_SIZE * sizeof(char), "" );
#else
	// Use less Secure original C library APIs
	strcpy( buf, "" );
#endif

	if ( doDiff )
	{
		TimeMsg( buf, time_str, " A Files: ", (long)files_A_count, "  ", false );
		TimeMsg( buf, time_str, " B Files: ", (long)files_B_count, "", false );
	}
	else
		TimeMsg( buf, time_str, " ", (long)files_A_count, " files", false );
	
	if ( 1.0 > total_seconds )
		total_seconds = 1.0;		// AVOID divide by Zero

// Use more Secure C library APIs if available
#ifdef	_MSC_VER
	strcpy_s( buf, MSG_STR_BUF_SIZE * sizeof(char), "" );
	sprintf_s( buf, MSG_STR_BUF_SIZE * sizeof(char), " for %.1f files processed per second\n", 
				( (double)( files_A_count + files_B_count )/total_seconds ) + .05 );
#else
	// Use less Secure original C library APIs
	strcpy( buf, "" );
	sprintf( buf, " for %.1f files processed per second\n", 
				( (double)( files_A_count + files_B_count )/total_seconds ) + .05 );
#endif
	time_str += buf;

	if ( duplicate_threshold_used >= 0.0 )
		doDups = true;

	// show the Times of various steps and total elapsed Time if wanted
	if ( true == show_any_times )
		ShowTiming( time_str, timeStart, timeEnd, show_total_only, doDiff, doDups );

	// Put in summary counts of message types of interest
	string count;
	IntToStr( (int)stack_dump_count, count );
	time_str += "\n";
	time_str += "    Stack Dumps:  ";
	time_str += count;
	time_str += "\n";

	IntToStr( (int)errors_count, count );
	time_str += "         Errors:  ";
	time_str += count;
	time_str += "\n";

	IntToStr( (int)warnings_count, count );
	time_str += "       Warnings:  ";
	time_str += count;
	time_str += "\n";

	IntToStr( (int)information_count, count );
	time_str += "    Information:  ";
	time_str += count;
	time_str += "\n";

	LongToStr( (long)uncounted_files_count, count );
	time_str += "Uncounted Files:  ";
	time_str += count;
	time_str += "\n";

	cout << time_str;
		
	// Write the Times to a file
	SavePerformanceStats( time_str );

#ifdef	_DEBUG
// Show some internal profiling that could be followed up.  For example:
//	extern unsigned int calls_FindModifiedLines;

//	printf( "\nFindModifiedLines was called %d times.\n", calls_FindModifiedLines );
#endif

#ifdef	_DEBUG
	// Wait for User input
	// This prevents the console window (running on Windows) from closing 
	// and therefore not allowing the User to know the total running time
	//
	// Suggest checking for Memory Leaks running Debug build...
	// This is also a good place to compare the amount of memory allocated
	// on startup with what remains allocated here. See calls to HeapInUse
	printf( "\nFinished.  Hit Enter key and this will exit.\n" );

	char keyBuffer2[256];

	//pChar = gets( keyBuffer2 );
        /* Modification: 2017.02; USC
         *   Fixed VS2015 compilation errors */
        pChar = gets_s( keyBuffer2 );
#endif

	return retVal;
}

#endif		//	END		#ifndef QTGUI

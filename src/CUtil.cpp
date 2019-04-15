//! Utility class static methods.
/*!
* \file CUtil.cpp
*
* This file contains the utility class static methods.
*/

/* Modification: 2016.01; USC
*   Fixed mingw 4.9.1 compilation errors */
#include <time.h>
#include "UCCBeforeLibraryIncludes.h"    // Modification: 2015.12
#include <algorithm>
#include <cstring>
#include <iomanip>   // Modification: 2015.12
#include <sstream>   // Modification: 2009.01
#include <string>    // Modification: 2015.12

using namespace std;

#ifdef UNIX          // Modification: 2009.01
	#include <dirent.h>
	#include <sys/stat.h>
        #include <unistd.h>  //Modification: 2018.01
#else
	#include <direct.h>
	#include <io.h>
#endif

/******************  NOT  USED
#ifndef NO32BIT      // Modification: 2015.12

	// ensure unsigned 32-bit integer is available for SHA-256 algorithm
	// if compile fails, no standard 32-bit integer exists
	#include <climits>
	#if UINT_MAX == 0xffffffff
		typedef unsigned int uint32_t;
	#elif ULONG_MAX == 0xffffffff
		typedef unsigned long uint32_t;
	#else
		// only exists if C++11 standard (ISO/IEC 14882:2011)
		#include <cstdint>
	#endif

	// macros to shift and rotate 32-bit word the specified number of bits to the right
	#define ShiftRight32Bit(word, bits) (((bits) >= 32) ? 0 : ((word) >> (bits)))
	#define RotateRight32Bit(word, bits) (((word) >> (bits)) | ((word) << (32 - (bits))))

#endif               // Modification: 2009.01
*********  END  OF  NOT  USED ******/

#include "UCCAfterLibraryIncludes.h"    // Modification: 2015.12

#include "UCCGlobals.h"                 // Modification: 2015.12
#include "UCCFilesOut.h"                // Modification: 2015.12
#include "CUtil.h"


// Convert an int to a std::string
void IntToStr( const int val, string & result )
{
	char buf[64];

// Use more Secure C library APIs if available
#ifdef	_MSC_VER
	// Microsoft C++ compiler.  This MAY not be available on very old versions
	// _itoa_s( val, buf, 10 );   // itoa is not standard C so not available on Mac for example
	sprintf_s( buf, sizeof( buf )/sizeof( buf[0] ), "%d", val ); 
#else
	// Use less Secure original C library API
	sprintf( buf, "%d", val );
#endif

	result = buf;
}

// Convert a long to a std::string
void LongToStr( const long val, string & result )
{
	char buf[64];

// Use more Secure C library APIs if available
#ifdef	_MSC_VER
	// Microsoft C++ compiler.  This MAY not be available on very old versions
	// _ltoa_s( val, buf, 10 );   // ltoa is not standard C
	sprintf_s( buf, sizeof( buf )/sizeof( buf[0] ), "%ld", val );
#else
	// Use less Secure original C library API
	sprintf( buf, "%ld", val );
#endif

	result = buf;
}

// Convert a long long to a std::string
void LongLongToStr( const long long val, string & result )
{
	char buf[128];

// Use more Secure C library APIs if available
#ifdef	_MSC_VER
	// Microsoft C++ compiler.  This MAY not be available on very old versions
	// _ltoa_s( val, buf, 10 );   // ltoa is not standard C
	sprintf_s( buf, sizeof( buf )/sizeof( buf[0] ), "%lld", val );
#else
	// Use less Secure original C library API
	sprintf( buf, "%lld", val );
#endif

	result = buf;
}

// Convert a float to a std::string with a number of decimal places
void FloatToStr( const float val, string & result, const unsigned int dec_precision )
{
	char	buf[128];

	if ( 1 == dec_precision )
	{
	// Use more Secure C library APIs if available
#ifdef	_MSC_VER
		// Microsoft C++ compiler.  This MAY not be available on very old versions
		// _ltoa_s( val, buf, 10 );   // ltoa is not standard C
		sprintf_s( buf, sizeof( buf )/sizeof( buf[0] ), "%.1f", val );
#else
		// Use less Secure original C library API
		sprintf( buf, "%.1f", val );
#endif
		result = buf;
	}
	else
		result = "";
}


// Count only Physical files and do NOT count embedded "extension files"
void CountPhysicalFiles( SourceFileList & fileList, unsigned long & phyFileCount )
{
	unsigned long	foundCount = 0L;

	SourceFileList::iterator itEnd = fileList.end();
	for ( SourceFileList::iterator it = fileList.begin(); it != itEnd; it++ )
	{
		if ( (*it).second.file_name_isEmbedded )
			continue;

		foundCount++;
	}

	phyFileCount = foundCount;
}


// Helper called to clear totals of various message types
void ClearSummaryMsgCounts()
{
    stack_dump_count = 0;
    errors_count = 0;
    warnings_count = 0;
    information_count = 0;
    uncounted_files_count = 0L;
}

// Initialize Time values of various processing steps.
void InitTimes()
{
	time( &timeStart );

// Set all the time values to at least the start time.
	// Removed  time_end_match_baselines_web = from below  Randy
	timeEnd = time_end_files_analysis = time_end_files_read = time_end_print_resultsB \
		= time_end_find_duplicatesB = time_end_print_results = time_end_find_duplicates \
		= time_end_process_pairs = time_end_match_baselines \
		= time_end_list_built = timeStart;
}


/*!
* 1. Function Description:
*    Helper to format a message string
*
* 2. Parameters:
* \Local define STR_BUF_SIZE  IN      gives size of passed in character buffer
* \param        pBuf          IN/OUT  pointer to a character buffer to use when formatting
* \param        time_str      IN/OUT  string to append newly formatted character buffer
* \param        before        IN      string to show before the number
* \param        count         IN      number to display if >= 0
* \param        after         IN      string to show after the number
* \param        fixed_width   IN      use fixed width for number or not
*
* 3.Creation Time And Owner:
*	 Version: 2015.12
*/
void TimeMsg( char *pBuf, string & time_str, const char * before, 
				const long count, const char * after, const bool fixed_width )
{
	// Use more Secure C library APIs if available
#ifdef	_MSC_VER
	strcpy_s( pBuf, MSG_STR_BUF_SIZE * sizeof(char), "" );
	if ( count >= 0 )
	{
		if ( fixed_width )
			sprintf_s( pBuf, MSG_STR_BUF_SIZE * sizeof(char), "%s%6ld%s", before, count, after );
		else
			sprintf_s( pBuf, MSG_STR_BUF_SIZE * sizeof(char), "%s%ld%s", before, count, after );
	}
	else
		sprintf_s( pBuf, MSG_STR_BUF_SIZE * sizeof(char), "%s%s", before, after );
#else
	// Use less Secure original C library APIs
	strcpy( pBuf, "" );
	if ( count >= 0 )
	{
		if ( fixed_width )
			sprintf( pBuf, "%s%6ld%s", before, count, after );
		else
			sprintf( pBuf, "%s%ld%s", before, count, after );
	}
	else
		sprintf( pBuf, "%s%s", before, after );
#endif

	time_str += pBuf;
}


/*!
* 1. Function Description:
*    Show Times to perform various steps
*    Don't call this if you do not want any Times shown
*
* 2. Parameters:
* \Global      g_process_after_read  IN  Decision variable for exact sequence of steps
* \Globals     time_end_xxx          IN  xxx represents names given above
* \Local define STR_BUF_SIZE     IN      gives size of character buffer to use
* \param        time_str         IN/OUT  string to append newly formatted character buffer
* \param        timeStart        IN      Time when this activity started
* \param        timeEnd          IN      Time when this activity ended
* \param        show_total_only  IN      Decision variable for amount of Time details
* \param        doDiff           IN      Decision variable to tell if Differencing was done
* \param        doDups           IN      Decision variable to tell if Duplicate checks were done
*
* 3.Creation Time And Owner:
*	 Version: 2015.12
*    Randy Maxwell
*/
void	ShowTiming( string & time_str, 
					const time_t timeStart, const time_t timeEnd, 
					const bool show_total_only, 
					const bool doDiff, const bool doDups )
{
	long	diffSeconds;
	char	buf[MSG_STR_BUF_SIZE];

	if ( false == show_total_only )
	{
		TimeMsg( buf, time_str, "\n     Processing  Step     :  seconds\n", -1, "" );
		TimeMsg( buf, time_str,   "------------------------- : --------\n", -1, "" );

		diffSeconds = (long)( difftime( time_end_list_built, timeStart ) + 0.5 );	// round to nearest second
		TimeMsg( buf, time_str,   " Build file list          : ", diffSeconds, "\n" );

		if ( doDiff )
		{
			diffSeconds = (long)( difftime( time_end_match_baselines, time_end_list_built ) + 0.5 );
			TimeMsg( buf, time_str, " Match baselines          : ", diffSeconds, "\n" );
		
		/*    NOT USED but don't want to delete from code yet.  Randy
			diffSeconds = (long)( difftime( time_end_match_baselines_web, time_end_match_baselines ) + 0.5 );
			TimeMsg( buf, time_str, " Match Web baselines      : ", diffSeconds, "\n" );
		*/
			// Was  time_end_process_pairs, time_end_match_baselines_web
			diffSeconds = (long)( difftime( time_end_process_pairs, time_end_match_baselines ) + 0.5 );
			TimeMsg( buf, time_str, " Files comparison         : ", diffSeconds, "\n" );

			if ( doDups )
			{
			diffSeconds = (long)( difftime( time_end_find_duplicates, time_end_process_pairs ) + 0.5 );
			TimeMsg( buf, time_str, "   Find A duplicates      : ", diffSeconds, "\n" );
			}
			else
				time_end_find_duplicates = time_end_process_pairs;

			diffSeconds = (long)( difftime( time_end_print_results, time_end_find_duplicates ) + 0.5 );
			TimeMsg( buf, time_str, " Generate results A files : ", diffSeconds, "\n" );

			if ( doDups )
			{
			diffSeconds = (long)( difftime( time_end_find_duplicatesB, time_end_print_results ) + 0.5 );
			TimeMsg( buf, time_str, "   Find B duplicates      : ", diffSeconds, "\n" );
			}
			else
				time_end_find_duplicatesB = time_end_print_results;

			diffSeconds = (long)( difftime( time_end_print_resultsB, time_end_find_duplicatesB ) + 0.5 );
			TimeMsg( buf, time_str, " Generate results B files : ", diffSeconds, "\n" );
		}
		else
		{
			// NOT Diff
			// Show the Read and Analyze times
			if ( ! g_process_after_read )
			{
				diffSeconds = (long)( difftime( time_end_files_read, time_end_list_built ) + 0.5 );
				TimeMsg( buf, time_str, "  Reading files           : ", diffSeconds, "\n" );

				diffSeconds = (int)( difftime( time_end_files_analysis, time_end_files_read ) + 0.5 );
				TimeMsg( buf, time_str, "  Analyze files           : ", diffSeconds, "\n" );
			}
			else
			{	// Reading with Analysis following immediately on per file basis
				// instead of Reading going through list then start Analyze through list.
				diffSeconds = (long)( difftime( time_end_files_analysis, time_end_list_built ) + 0.5 );
				TimeMsg( buf, time_str, "Read, Analyze & Count     : ", diffSeconds, "\n" );
			}

			if ( doDups )
			{
				diffSeconds = (long)( difftime( time_end_find_duplicates, time_end_files_analysis ) + 0.5 );
				TimeMsg( buf, time_str, " Find duplicates          : ", diffSeconds, "\n" );
			}
			else
				time_end_find_duplicates = time_end_files_analysis;

			diffSeconds = (long)( difftime( time_end_print_results, time_end_find_duplicates ) + 0.5 );
			TimeMsg( buf, time_str, "  Generate results files  : ", diffSeconds, "\n" );

		} //	END  else  NOT  Diff
	}	//	END		! show_total_only

	// Show the elapsed time
	diffSeconds = (long)( difftime( timeEnd, timeStart ) + 0.5 );
	if ( diffSeconds > 60 )
	{
		if ( diffSeconds >= 120 )
		{
			// Variable minutes
		TimeMsg( buf, time_str, "               Total time : ", (diffSeconds / 60), " minutes", false );
			// seconds
			TimeMsg( buf, time_str, " ", (diffSeconds % 60), " seconds\n", false );
		}
		else
		{
		TimeMsg( buf, time_str, "               Total time : 1 minute ", (diffSeconds % 60), " seconds\n", false );
		}
	}
	else
		TimeMsg( buf, time_str, "               Total time : ", diffSeconds, " seconds\n" );

	return;
}


void SavePerformanceStats( const string time_str )
{
	time_t myTime;
	struct tm *myLocalTime;
	time(&myTime);
#if defined UNIX || defined MINGW
	myLocalTime = localtime(&myTime);
#else
	struct tm myLT;
	localtime_s(&myLT, &myTime);
	myLocalTime = &myLT;
#endif
	char s[64];
	strftime( s, sizeof(s), "/UCC_Performance__%m%d%Y_%I%M%S.txt", myLocalTime );

	string path_name;
	if ( outDir.length() )
		path_name = outDir + s;
	else
	{
		// use current directory
		path_name = s;
		path_name = "." + path_name;
	}

	FILE* pFile = NULL;

	// Use more Secure C library APIs if available
	#ifdef	_MSC_VER
		fopen_s( &pFile, path_name.c_str(), "a" );
	#else
		pFile = fopen( path_name.c_str(), "a" );
	#endif
	
	if ( NULL != pFile )
	{
		fprintf( pFile, "%s\n", time_str.c_str() );
		fprintf( pFile, "\n%s\n", cmdLine.c_str() );
		fclose( pFile );
	}
}

//
//	ALL BELOW ARE TIGHTLY BOUND AS CUtil CLASS Member Functions.  Not really needed to be like this.
//
//	There is a form of protection via a class type namespace.  Because all these are static; see .h file
//	there is no protection via implementation hiding as all these are also public.
//

/*/*Modification: 11.2016 Ext-4 Starts*/
void CUtil::allocate() {

	std::vector<std::string> ada_vec;
	ada_vec.push_back(".ada"); ada_vec.push_back(".a"); ada_vec.push_back(".adb"); ada_vec.push_back(".ads");
	dict["Ada"] = ada_vec;
	dictCopy["Ada"] = ada_vec;

	std::vector<std::string> assembly_vec;
	assembly_vec.push_back(".asm"); assembly_vec.push_back(".s"); assembly_vec.push_back(".asm.ppc");
	dict["Assembly"] = assembly_vec;
	dictCopy["Assembly"] = assembly_vec;

	std::vector<std::string> dos_vec;
	dos_vec.push_back(".bat");
	dict["DOS_Batch"] = dos_vec;
	dictCopy["DOS_Batch"] = dos_vec;

	std::vector<std::string> bash_vec;
	bash_vec.push_back(".sh"); bash_vec.push_back(".ksh");
	dict["Bash"] = bash_vec;
	dictCopy["Bash"] = bash_vec;

	std::vector<std::string> cobol_vec;
	cobol_vec.push_back(".cbl"); cobol_vec.push_back(".cob"); cobol_vec.push_back(".cpy");
	dict["COBOL"] = cobol_vec;
	dictCopy["COBOL"] = cobol_vec;

	std::vector<std::string> coldfu_vec;
	coldfu_vec.push_back(".*cfm");
	dict["ColdFusion"] = coldfu_vec;
	dictCopy["ColdFusion"] = coldfu_vec;
    
    std::vector<std::string> cfs_vec;
	cfs_vec.push_back(".cfs");
	dict["CFScript"] = cfs_vec;
	dictCopy["CFScript"] = cfs_vec;

	std::vector<std::string> cshell_vec;
	cshell_vec.push_back(".csh"); cshell_vec.push_back(".tcsh");
	dict["C-Shell"] = cshell_vec;
	dictCopy["C-Shell"] = cshell_vec;

	std::vector<std::string> ccpp_vec;
	ccpp_vec.push_back(".cpp"); ccpp_vec.push_back(".c"); ccpp_vec.push_back(".cc"); ccpp_vec.push_back(".cxx"); ccpp_vec.push_back(".inl");
	ccpp_vec.push_back(".h"); ccpp_vec.push_back(".hh"); ccpp_vec.push_back(".hpp"); ccpp_vec.push_back(".hxx"); ccpp_vec.push_back(".inc");
	dict["C_CPP"] = ccpp_vec;
	dictCopy["C_CPP"] = ccpp_vec;

	std::vector<std::string> csharp_vec;
	csharp_vec.push_back(".cs");
	dict["C#"] = csharp_vec;
	dictCopy["C#"] = csharp_vec;

	std::vector<std::string> css_vec;
	css_vec.push_back(".css");
	dict["CSS"] = css_vec;
	dictCopy["CSS"] = css_vec;

	std::vector<std::string> fortran_vec;
	fortran_vec.push_back(".f");	fortran_vec.push_back(".for");   fortran_vec.push_back(".f77");   fortran_vec.push_back(".f90");   fortran_vec.push_back(".f95");
	fortran_vec.push_back(".f03"); fortran_vec.push_back(".hpf");
	dict["Fortran"] = fortran_vec;
	dictCopy["Fortran"] = fortran_vec;

	std::vector<std::string> html_vec;
	html_vec.push_back(".*htm");
	dict["HTML"] = html_vec;
	dictCopy["HTML"] = html_vec;

	std::vector<std::string> idl_vec;
	idl_vec.push_back(".pro"); idl_vec.push_back(".sav");
	dict["IDL"] = idl_vec;
	dictCopy["IDL"] = idl_vec;

	std::vector<std::string> java_vec;
	java_vec.push_back(".java");
	dict["Java"] = java_vec;
	dictCopy["Java"] = java_vec;

	std::vector<std::string> javascript_vec;
	javascript_vec.push_back(".js");
	dict["JavaScript"] = javascript_vec;
	dictCopy["JavaScript"] = javascript_vec;

	std::vector<std::string> makefile_vec;
	makefile_vec.push_back(".make");   makefile_vec.push_back(".makefile");
	dict["Makefile"] = makefile_vec;
	dictCopy["Makefile"] = makefile_vec;

	std::vector<std::string> matlab_vec;
	matlab_vec.push_back(".m");
	dict["MATLAB"] = matlab_vec;
	dictCopy["MATLAB"] = matlab_vec;

	std::vector<std::string> nextmidas_vec;
	nextmidas_vec.push_back(".mm");
	dict["NeXtMidas"] = nextmidas_vec;
	dictCopy["NeXtMidas"] = nextmidas_vec;

	std::vector<std::string> xmidas_vec;
	xmidas_vec.push_back(".txt");
	dict["X-Midas"] = xmidas_vec;
	dictCopy["X-Midas"] = xmidas_vec;

	std::vector<std::string> pascal_vec;
	pascal_vec.push_back(".pas"); pascal_vec.push_back(".p"); pascal_vec.push_back(".pp"); pascal_vec.push_back(".pa3"); pascal_vec.push_back(".pa4"); 					pascal_vec.push_back(".pa5");
	dict["Pascal"] = pascal_vec;
	dictCopy["Pascal"] = pascal_vec;

	std::vector<std::string> perl_vec;
	perl_vec.push_back(".pl"); perl_vec.push_back(".pm");
	dict["Perl"] = perl_vec;
	dictCopy["Perl"] = perl_vec;

	std::vector<std::string> php_vec;
	php_vec.push_back(".*php");
	dict["PHP"] = php_vec;
	dictCopy["PHP"] = php_vec;


	std::vector<std::string> python_vec;
	python_vec.push_back(".py");
	dict["Python"] = python_vec;
	dictCopy["Python"] = python_vec;


	std::vector<std::string> ruby_vec;
	ruby_vec.push_back(".rb");
	dict["Ruby"] = ruby_vec;
	dictCopy["Ruby"] = ruby_vec;

	std::vector<std::string> scala_vec;
	scala_vec.push_back(".scala");
	dict["Scala"] = scala_vec;
	dictCopy["Scala"] = scala_vec;


	std::vector<std::string> sql_vec;
	sql_vec.push_back(".sql");
	dict["SQL"] = sql_vec;
	dictCopy["SQL"] = sql_vec;

	std::vector<std::string> vb_vec;
	vb_vec.push_back(".vb"); vb_vec.push_back(".frm"); vb_vec.push_back(".mod"); vb_vec.push_back(".cls"); vb_vec.push_back(".bas");
	dict["Visual_Basic"] = vb_vec;
	dictCopy["Visual_Basic"] = vb_vec;

	std::vector<std::string> vbscript_vec;
	vbscript_vec.push_back(".vbs");
	dict["VBScript"] = vbscript_vec;
	dictCopy["VBScript"] = vbscript_vec;

	std::vector<std::string> verilog_vec;
	verilog_vec.push_back(".v"); verilog_vec.push_back(".sv"); verilog_vec.push_back(".svi"); verilog_vec.push_back(".vlib"); verilog_vec.push_back(".svlib"); 				verilog_vec.push_back(".vh"); verilog_vec.push_back(".svh");
	dict["Verilog"] = verilog_vec;
	dictCopy["Verilog"] = verilog_vec;

	std::vector<std::string> vhdl_vec;
	vhdl_vec.push_back(".vhd"); vhdl_vec.push_back(".vhdl");
	dict["VHDL"] = vhdl_vec;
	dictCopy["VHDL"] = vhdl_vec;

	std::vector<std::string> xml_vec;
	xml_vec.push_back(".*xml");
	dict["XML"] = xml_vec;
	dictCopy["XML"] = xml_vec;

}
StringVector CUtil::getExtensionsToLanguage(string lang, StringVector fileExtension)
{
	std::vector<string> myvector = dict[lang];
	vector<string>::iterator it;
	//StringVector fileExtension;
	for (it = myvector.begin(); it < myvector.end(); it++)
	{
		fileExtension.push_back(*it);
	}
	return fileExtension;
}
/*Modification: 11.2016 Ext-4 ends*/

/*!
* 1. Function Description: 
*    Returns a string without leading/trailing spaces or tabs.
*    Return trimmed string
*
* 2. Parameters:
*    str：original string
*    mode：trim mode (-1=left, 0=both, 1=right)
*
* 3. Creation Time and Owner: 
*    Version 2009.01
*/
string CUtil::TrimString(const string &str, int mode) // Modification: 2011.05
{
    size_t idx;
    string str1 = str;  // Modification: 2011.10
	bool done = false;  // Modification: 2015.12
    if (mode <= 0)      // Modification: 2011.05
    {
        idx = str1.find_first_not_of(" \t\n\r\f");
        if (idx != string::npos)
            str1 = str1.substr(idx);
        else
		{
			// Nothing but whitespace in the string
            str1 = "";
			done = true;	// no need to check other direction  Modification: 2015.12
		}
    }
    if ( ( false == done )  // Modification: 2015.12
	  && ( mode >= 0 ) )
    {
        idx = str1.find_last_not_of(" \t\n\r\f");
        if (idx != string::npos)
            str1 = str1.substr(0, idx + 1);
        else
            str1 = "";
    }
    return str1;        // Modification: 2011.10
}

/*!
* 1. Function Description: 
*    Erases all strings specified by erasedstr in srcstr.
*    Return modified string
*
* 2. Parameters:
*    srcstr： original string
*    erasedstr： substring to be erased
*
* 3. Creation Time and Owner:
*    Version 2011.05
*/
string CUtil::EraseString(const string &srcstr, const string &erasedstr)
{
    size_t idx = 0;
    string srcstr1 = srcstr;                                     // Modification: 2011.10
    while ((idx = srcstr1.find(erasedstr, idx)) != string::npos) // Modification: 2011.05
    {
        srcstr1.erase(idx, erasedstr.length());
    }
    return srcstr1;
}

/*!
* 1. Function Description: Initialize an array of lower case chars used by ToLower.  
*    Performance improvement (significant, some tests show ~7% less time use by entire UCC).
*    Valid for ASCII style characters only (values from 0 to 255).  
*    NOT for UTF or Unicode character sets.
*
* 2. Parameters:
* \Globals	lowerChars	char array is initialized
*    none, fills in Global lowerChars array
*
* 3. Creation Time and Owner: 
*    Version 2015.10.02
*/
unsigned char	lowerChars[256];
static bool		useLibraryToLower = false;
void CUtil::InitToLower()
{
	// Call expensive tolower library call just to fill in array
    // Modification: Spring 2016 - 'j' can be unsinged int to avoid warning
	for ( unsigned int j = 0; j < ( sizeof( lowerChars ) / sizeof( lowerChars[0] ) ); j++ )
	{
		lowerChars[ j ] = (unsigned char)( (unsigned int)( tolower( j ) ) );
	}

	string test = "A";
	size_t  temp = 1;
	if ( sizeof( test[ 0 ] ) != temp )
		useLibraryToLower = true;
}

/*!
* 1. Function Description: Returns a string in lower case.
*    Return lower case string
*
* 2. Parameters:
*    string_to_lower：original string
* \Globals	lowerChars	char array is used to greatly improve performance
*
* 3. Creation Time and Owner: 
*    Version 2009.01
*    Revised 2015.12  Use array lookup instead of calling C library API
*/
string CUtil::ToLower(const string &string_to_lower)
{
	string string_to_lower2 = string_to_lower;    // Modification: 2011.10
	size_t	size = string_to_lower2.size();       // avoid calling size() as part of loop
	if ( false == useLibraryToLower )
	{
		for ( size_t i = 0; i < size; i++ )       // Modification: 2009.01
		{
			// Use array instead of much slower tolower library call
			string_to_lower2[ i ] = (char)lowerChars[ TL_ARR( string_to_lower2[ i ] ) ]; // Modification: 2015.10.02
		}
	}
	else
	{
		// SOME characters are greater than 255 so do the lower 255 quickly and others as library call
		for ( size_t i = 0; i < size; i++ )       // Modification: 2009.01
		{
			if ( (unsigned int)( string_to_lower2[ i ] ) <= 255 )
			{
				// Use array instead of much slower tolower library call
				string_to_lower2[ i ] = (char)lowerChars[ TL_ARR( string_to_lower2[ i ] ) ]; // Modification: 2015.10.02
			}
			else
			{
				// Use C library for this character
				string_to_lower2[ i ] = (char)tolower( (int)string_to_lower2[ i ] );
			}
		}
	}
    return string_to_lower2;
}

/*!
* 1. Function Description: 
*    Checks whether the string passed is blank.
*    Return true if it's a blank string
*
* 2. Parameters:
*    str：original string
* 
* 3. Creation Time and Owner: 
*    Version 2009.01     
*/
bool CUtil::CheckBlank(const string &str)
{
    string::size_type idx;
    idx = str.find_first_not_of("\n\t\r\f ");   // searches for chars other than specified
    if (idx != string::npos)
        return false;
    else
        return true;
}

/*!
* 1. Function Description: 
*    Checks whether the string passed is an integer.
*    Return true if it's an integer string
* 
* 2. Parameters:
*    str：original string
*
* 3. Creation Time and Owner: 
*    Version 2009.01 
*/
bool CUtil::IsInteger(const string &str)
{
    string str1 = TrimString(str);                               // Modification: 2011.10
    if (str1.find_first_not_of("0123456789") != string::npos)    // Modification: 2009.01
        return false;
    else
        return true;
}

/*!
* 1. Function Description: 
*    Finds the first appearance of each element of table's first value.
*    Return second value of the map
*
* 2. Parameters:
*    target：string
*    table：map table
*    pos：position of string
*    preLang：previous language (for web languages)
*
* 3. Creation time and Owner: 
*    Version 2009.01
*/
size_t CUtil::FindStringsCaseInsensitive(const string &target, map<string, int> &table, size_t &pos, size_t preLang)
{
    string target1 = ToLower(target);
    size_t tmp_pos;
    pos = string::npos;
    size_t ret = INVALID_POSITION;                                                      // Modification: 2011.05
    for (map<string, int>::iterator iter = table.begin(); iter != table.end(); iter++)  // Modification: 2009.01
    {
        tmp_pos = target1.find(iter->first);
        if (tmp_pos != string::npos && (pos == string::npos || pos > tmp_pos))
        {
            // handle special case "width=100%>"
            ret = static_cast<size_t>( iter->second );
            if ((ret == WEB_ASP_JSP_END && preLang != WEB_ASP_JSP_START) ||
                (ret == WEB_PHP_END && (preLang != WEB_PHP_START && preLang != WEB_PHP_START2)))
                ret = INVALID_POSITION;                                                 // Modification: 2011.05
            pos = tmp_pos;                                                              // Modification: 2009.01
        }
    }
    return ret;
}

/*!
* 1. Function Description:
*    Finds the position of the "target" char in "source" string.
*    Starting from "start_idx" but ignoring escape chars,
*    for example, '\n' is different from 'n' in "this is not nothing"
*    Return position of target character
*
* 2. Parameters:
*    source：source string
*    target：target character
*    start_idx：index to start search
*    escape：escape character to ignore
*
* 3. Creation time and Owner: 
*    Version 2009.01
*/
size_t CUtil::FindCharAvoidEscape(const string &source, char target, size_t start_idx, char escape)
{
    size_t idx, i;
    if (start_idx >= source.length())
        return string::npos;
    idx = start_idx;
    while (idx < source.length())
    {
        idx = source.find(target, idx);
        if (idx == string::npos)
            return idx;

        for (i = 1; i <= idx; i++)// Modification: 2015.12
        {
            // trace back to the previous char ex. "adfd\\\\"
            if (source[idx - i] != escape) break;
        }
        if (i % 2 != 0)
        {
            // case \\", avoid case something\"
            break;
        }
        idx++;
    }
    if (idx >= source.length())  // Modification: 2011.10
        return string::npos;
    return idx;                  // Modification: 2009.01
}

/*!
* 1. Function Description: 
*    Finds the (possibly) beginning part of 2 strings that are the same.
*    Removes the common beginning part from each string if wanted.
*    Return the string holding the common (to each given string) prefix.
*
* 2. Parameters:
*    str：   string  IN/OUT  reference to string that may share a prefix with str2
*    str2：  string  IN/OUT  reference to string that may share a prefix with str
*    prefix: string  IN/OUT  reference to string that is the found common prefix
*    remove: bool    IN      true will remove the prefix characters from both strings
*
* 3. Creation time and Owner: 
*    Version 2015.12
*/
void CUtil::FindCommonPrefix( string & str, string & str2, string & prefix, const bool remove )
{
	string	prefixNew;		// Prevent changing prefix until known

	unsigned int	prefixSize = 0;
	unsigned int	count = str.size();
	if ( count > str2.size() )
		count = str2.size();

	for ( unsigned int i = 0; i < count; i++ )
	{
		// This does Exact (case sensitive) match
		if ( str[i] == str2[i] )
		{
			prefixNew += str[i];
			prefixSize++;
		}
		else
			break;
	}

	if ( remove && prefixSize )
	{
		str  =  str.substr( prefixSize );
		str2 = str2.substr( prefixSize );
	}
	prefix = prefixNew;
}

/*!
* 1. Function Description: 
*    Finds the (possibly) beginning part of 2 strings that are the same.
*    Removes the common beginning part from each string if wanted.
*    Return the string holding the common (to each given string) prefix.
*
* 2. Parameters:
*    str：   string  IN/OUT  reference to string that may share a prefix with str2
*    str2：  string  IN/OUT  reference to string that may share a prefix with str
*    prefix: string  IN/OUT  reference to string that is the found common prefix
*    remove: bool    IN      true will remove the prefix characters from both strings
*
* 3. Creation time and Owner: 
*    Version 2015.12
*/
void CUtil::FindCommonSuffix( string & str, string & str2, string & suffix, const bool remove )
{
	string suffixNew;		// Prevent changing suffix until known

	unsigned int	size  = str.size();
	unsigned int	size2 = str2.size();
	unsigned int	count = size;
	if ( count > size2 )
		count = size2;

	if ( 0 == count )
	{
		suffix = "";
		return;
	}

	unsigned int	suffixSize = 0;
	unsigned int	index  = size  - 1;
	unsigned int	index2 = size2 - 1;

	for ( unsigned int i = 0; i < count; i++ )
	{
		// Go from END of strings
		if ( str[index] == str2[index2] )
		{
			suffixNew = str[index] + suffixNew;
			suffixSize++;
			index--;
			index2--;
		}
		else
			break;
	}

	if ( remove && suffixSize )
	{
		str  =  str.substr( 0, size  - suffixSize );
		str2 = str2.substr( 0, size2 - suffixSize );
	}
	suffix = suffixNew;
}

/*!
* 1. Function Description: 
*    Finds the keyword in the string starting from start to end.
*    Return index of keyword in string
*
* 2. Parameters:
*    str：string
*    keyword：keyword to find
*    start：starting index for search
*    end：ending index for search
*    case_sensitive: is case sensitive?
*
* 3. Creation time and Owner: 
*    Version 2009.01
*/
size_t CUtil::FindKeyword(const string &str, const string &keyword, size_t start, size_t end, bool case_sensitive)// Modification: 2013.04
{
// Modification: 2009.01
#define SPECIAL_CHARS   " \t;[]()+/-*<>=,&~!^?:%{}|"  
    size_t kw_length = keyword.length();
    size_t idx, i = start;
    string str1 = str;
    string keyword1 = keyword;      // Modification: 2014.08
    if (end == TO_END_OF_STRING)    // Modification: 2009.01
        end = str1.length() - 1;    // inclusive

    if (!case_sensitive)
    {
        str1 = CUtil::ToLower(str1);
        keyword1 = CUtil::ToLower(keyword1);
    }

    while (i <= end)
    {
        idx = str1.find(keyword1, i);
        if (idx != string::npos && idx + kw_length - 1 <= end)
        {
            if ((idx == 0 || strchr(SPECIAL_CHARS, str1[idx - 1]) != NULL) &&
                (idx + kw_length >= str1.length() ||
                strchr(SPECIAL_CHARS, str1[idx + kw_length]) != NULL))
            {
                // the keyword stands alone or surrounded by special chars
                return idx;
            }
        }
        else
        {
            // cannot find the keyword in str
            break;
        }
        i = idx + 1;    // keyword found but not stands alone nor surrounded by special chars
    }

    return string::npos;    //not found
#undef SPECIAL_CHARS
}

/*!
* 1. Function Description:
*    Counts keywords in string.
*
* 2. Parameters:
*    base: string to search
*    container: set of keywords
*    count: is incremented by the number of keywords found and not changed if none found
*    mode: allowable leading/trailing character mode (1=exclude keywords surrounded by exclude characters, 2=include only keywords preceded by include1 characters and followed by include2 characters)
*    exclude: list of characters that may not surround keywords found (if mode=1)
*    include1: list of characters that must precede keywords found (if mode=2)
*    include2: list of characters that must follow keywords found (if mode=2)
*    counter_container: stores list of individual keyword counts (if specified)
*    case_sensitive: language is case sensitive?
*
* 3. Creation time and Owner:
*    Version 2009.01
*/
void CUtil::CountTally(const string &base, StringVector &container, unsigned int &count, int mode, const string &exclude,
    const string &include1, const string &include2, UIntVector* counter_container, bool case_sensitive) // Modification: 2015.12
{
	// Precondition: Must have at least 1 keyword
	if ( container.size() == 0 )
		return;					// Nothing to look for.  Modification 2015.12

    // Modification: 2009.01
    string::size_type idx;
    string base1;
    StringVector::iterator vit;
    UIntVector::iterator cit;
    unsigned int single_count = 0; // Modification: 2011.05
    base1 = " " + base + " ";

    if (counter_container)         // Modification: 2009.01
        cit = counter_container->begin();

    if (case_sensitive == false)
    {
        base1 = CUtil::ToLower(base1);
        for (vit = container.begin(); vit != container.end(); vit++)
            (*vit) = CUtil::ToLower((*vit));
    }

    if (mode == 1)
    {
        // exclude mode
        for (vit = container.begin(); vit != container.end(); vit++)
        {
            idx = base1.find((*vit));
            while (idx != string::npos)
            {
                if ((exclude.find(base1[idx + ((*vit).size())]) == string::npos) && // Modification: 2015.12
                    (exclude.find(base1[idx - 1]) == string::npos))
                {
                    count++;
                    single_count++;
                }
                idx = base1.find((*vit), idx + ((*vit).size()));                    // Modification: 2009.01
            }
            if (counter_container)
            {
                (*cit) += single_count;
                single_count = 0;
                cit++;
            }
        }
    }
    else if (mode == 2)
    {
        // include mode
        for (vit = container.begin(); vit != container.end(); vit++)
        {
            idx = base1.find((*vit));
            while (idx != string::npos)
            {
                if ((include1.find(base1[idx - 1]) != string::npos) &&              // Modification: 2015.12
                    (include2.find(base1[idx + ((*vit).size())]) != string::npos))
                    count++;
                idx = base1.find((*vit), idx + ((*vit).size()));                    // Modification: 2009.01
            }
        }
    }
}

/*!
* 1. Function Description:
*    Extracts the filename (without path) from the filepath.
*    ex. abc\xyz.cpp --> xyz.cpp
*    Return file name
*
* 2. Parameters:
*    filepath: file path
*
* 3. Creation time and Owner:
*    Version 2009.01
*/
string CUtil::ExtractFilename(const string &filepath)
{
    string filename = filepath;  // Modification: 2013.04
    // Modification: 2015.12
    size_t idx = string::npos;
#ifdef UNIX
    size_t idxW = string::npos;                 // Windows
#else
    size_t idxW = filename.find_last_of("\\");  // Windows
#endif
    size_t idxU = filename.find_last_of("/");   // Unix
    if (idxW != string::npos && idxU != string::npos)
    {
        if (idxW > idxU)
            idx = idxW;
        else
            idx = idxU;
    }
    else if (idxW != string::npos)
        idx = idxW;
    else if (idxU != string::npos)
        idx = idxU;
    if (idx != string::npos)
        return filename.substr(idx + 1);
    else
        return filename;
}

/*!
* 1. Function Description:
*    Extracts the path (without name) from the filepath.
*    ex. abc\xyz.cpp --> abc
*    Return path
*
* 2. Parameters:
*    filepath: file path
*
* 3. Creation time and Owner:
*    Version 2015.12
*/
string CUtil::ExtractFilepath(const string &filepath)
{
    string path = filepath;
    size_t idx = string::npos;
#ifdef UNIX
    size_t idxW = string::npos;                 // Windows
#else
    size_t idxW = path.find_last_of("\\");  // Windows
#endif
    size_t idxU = path.find_last_of("/");   // Unix
    if (idxW != string::npos && idxU != string::npos)
    {
        if (idxW > idxU)
            idx = idxW;
        else
            idx = idxU;
    }
    else if (idxW != string::npos)
        idx = idxW;
    else if (idxU != string::npos)
        idx = idxU;
    if (idx != string::npos)
    {
        if (idx < 1)
            return path.substr(0, 1);
        else
            return path.substr(0, idx);
    }
    else
        return "";
}


// Helper used for smarter Estimates to help with RAM usage.
/*!
* 1. Function Description:
*    Helper used for smarter Estimates to help with RAM usage.
*    Do NOT use the size returned for buffer allocation !
*    Returns size of the file whose path/name is given.

*  Precondition:
*    It is expected but not required that the file exists.
*
* 2. Parameters:
*    file: path/name of file whose Size is wanted
*
* 3. Creation time and Owner:
*    Version 2015.12
*/
unsigned long long CUtil::GetFileSizeInBytes( const string file )
{
	unsigned long long file_size_bytes = 0L;

	// Research Note: 
	// Below is not guarenteed to give to the Byte accurate sizes.
	// This is not a problem as we just want to uses sizes for Estimates.
	// Some security scanning programs may flag this as INSECURE.
	// This is only INSECURE if we use the return value directly 
	// for buffer allocation (malloc for example).  This is not the case.

	FILE * fp = NULL;

#ifdef	_MSC_VER
	// Use more Secure C library API
	errno_t ret  = fopen_s( &fp, file.c_str(), "r" );
	if ( 0 != ret )
		fp = NULL;
#else
	// Use older less Secure C library API
	fp = fopen( file.c_str(), "r" );
#endif

	if ( fp != NULL )
	{
		if ( 0 == fseek( fp, 0, SEEK_END ) )
			file_size_bytes = (unsigned long long)ftell( fp );
		fclose( fp );
	}

	return file_size_bytes;
}

/*!
* 1. Function Description:
*    For a given directory name, extract all the files from that directory as well as
*    from all its sub-directories and store the filenames in the fileList vector.
*    Returns:
*       1 if path exists and is a directory
*       0 if not a directory
*      -1 if User cancelled		Caller MUST check all the return values!
*
*    This may be called more than 1 time to accumulate information.
*    This is support for building a List of Files for a given Baseline file set.
*
*    Pre Conditions:
*       Before the first call here ALL of OUT parameters were initialized.
*
*    Post Conditions:
*       Caller MUST check for User Cancel before normal processing
*
* 2. Parameters:
*    folder:            IN/OUT  folder to list (may have whitespace trimmed)
*    fileExtList:       IN      list of file extensions to search
*    fileList:          IN/OUT  list of files in folder
*    symLinks:          IN      follow Unix links?
*    totalFileSizes:    IN/OUT  sum of all file sizes in Bytes
*    largestFileSize:   IN/OUT  largest found file size in Bytes
*    numFiles:          IN/OUT  number of files in the list
*    commonPathPrefix:  IN/OUT  beginning part of full path that is the same among all files
*
* 3. Creation time and Owner:
*    Version 2009.01
*    Revised 2015.12  Get number of files, total file sizes (Bytes) and size of largest file and common Path prefix.
*/
int CUtil::ListAllFiles(string & folder, 
                         const StringVector &fileExtList, 
                         StringVector &fileList,				// Appended to here
                         const bool symLinks,
                         unsigned long long & totalFileSizes,	// Added to here
                         unsigned long long & largestFileSize,	// Updated here
                         unsigned long & numFiles,				// Incremented here
                         string & commonPathPrefix )			// Updated here
{
    StringVector tmpList;               // Modification: 2011.10
    string file;
    size_t i, n;

    folder = CUtil::TrimString(folder); // Modification: 2011.05

#ifdef UNIX                             // Modification: 2011.10
    // skip links if user specified
    struct stat inodeData;
    if (!symLinks && (lstat(folder.c_str(), &inodeData) < 0 || S_ISLNK(inodeData.st_mode)))
        return 0;
#endif

    // process folder
    if (!GetFileList(tmpList, folder, symLinks))
        return 0;
    unsigned long long fileSizeBytes;
    bool               extensionFiltering = true;
    if ( fileExtList.at(0) == "*.*" || fileExtList.at(0) == "*" )
        extensionFiltering = false;

#define		UI_GET_FILE_LIST_COUNT	200
    unsigned long	prev_numFiles = numFiles;
    string			cntStr;

#ifndef		QTGUI
	cout << "..........";		// Set an area to be overwritten
#endif

	int		retVal = 1;

    // read through tmpList and get the names of all the files in the directory mentioned
    for (n = 0; n < tmpList.size(); n++)
    {
        file = tmpList.at(n);

        // if no-extension filtering, each file is pushed into the fileList
        if ( false == extensionFiltering )
        {
            fileList.push_back(file);
			fileSizeBytes = GetFileSizeInBytes( file );
			if ( largestFileSize < fileSizeBytes )
				largestFileSize = fileSizeBytes;
			totalFileSizes += fileSizeBytes;
			if ( 0L == numFiles )
				commonPathPrefix = file;
			numFiles++;
			FindCommonPrefix( commonPathPrefix, file, commonPathPrefix, false );
        }
        else
        {
            // for each extension, if file extension matches with the extension, the file is pushed into the fileList
            for (i = 0; i < fileExtList.size(); i++)
            {
                if (MatchFilename(ExtractFilename(file), fileExtList.at(i)))
                {
                    fileList.push_back(file);
					numFiles++;
					fileSizeBytes = GetFileSizeInBytes( file );
					if ( largestFileSize < fileSizeBytes )
						largestFileSize = fileSizeBytes;
					totalFileSizes += fileSizeBytes;
					if ( 0L == numFiles )
						commonPathPrefix = file;
					numFiles++;
					FindCommonPrefix( commonPathPrefix, file, commonPathPrefix, false );
                }
            }
        }

		if ( prev_numFiles + UI_GET_FILE_LIST_COUNT <= numFiles )
		{
		#ifndef		QTGUI
			LongToStr( (long)numFiles, cntStr );
			cout << "\b\b\b\b\b\b\b\b\b\b" << cntStr;		// 10 backspace + new count
			for ( int i = 0 ; i < 10 - (int)cntStr.size(); i++ )
				cout << " ";	// fill at right to be easier next time
			cout << flush;
		#endif
			prev_numFiles = numFiles;

		#ifdef	QTGUI
			// Check to see if User has cancelled only for Qt
			extern	bool execCanceled;
			if ( true == execCanceled )
			{
                retVal = -1;		// Caller MUST check for Cancel from this value
				printf( "\a" );		// Make a noise
				break;
			}
		#endif
		}
    }

	// Just erase the last 10 characters
	cout << "\b\b\b\b\b\b\b\b\b\b          \b\b\b\b\b\b\b\b\b\b";

    tmpList.resize( 0 );                    // Modification: 2009.01
    return retVal;
}

/*!
* 1. Function Description:
*    For a given path, this method lists all files, directories and 
*    sub-directories it contains and stores the filenames in the fileList vector.
*    Return path exists and is a directory
*
* 2. Parameters:
*    fileList: list of files in folder
*    path: folder's path to list
*    symLinks: follow Unix links?
*
* 3. Creation time and Owner:
*    Version 2011.05
*/
bool CUtil::GetFileList(StringVector &fileList, const string &path, bool symLinks) // Modification: 2011.10
{
    string fullPath;
#ifdef UNIX                                                   // Modification: 2011.05
    DIR *dir;
    struct dirent *fileRead;
    struct stat inodeData;

    // opening the given path
    dir = opendir(path.c_str());

    // If the dir doesn't exist
    if (dir == NULL)
        return(false);                                        // Modification: 2011.10

    // each file is processed until the last one
    while ((fileRead = readdir(dir)) != NULL)                 // Modification: 2011.05
    {
        // '.' & '..' are omitted
        if ((strcmp(fileRead->d_name, ".") != 0) && (strcmp(fileRead->d_name, "..") != 0))
        {
            // fullPath contains the path + the file name.
            fullPath = path + '/' + fileRead->d_name;
            if (symLinks)
            {
                if (stat(fullPath.c_str(), &inodeData) >= 0)
                {
                    // for each file, store the fullPath into the ofstream
                    if (!S_ISDIR(inodeData.st_mode))          // Modification: 2011.10
                        fileList.push_back(fullPath);
                    else
                    {
                        // for each directory, its file list is obtained
                        GetFileList(fileList, fullPath, symLinks);
                    }
                }                                             // Modification: 2011.05
            }
            else
            {
                if (lstat(fullPath.c_str(), &inodeData) >= 0)
                {
                    // for each file, store the fullPath into the ofstream
                    if (!S_ISLNK(inodeData.st_mode))
                    {
                        if (!S_ISDIR(inodeData.st_mode))
                            fileList.push_back(fullPath);
                        else
                        {
                            // for each directory, its file list is obtained
                            GetFileList(fileList, fullPath, symLinks);
                        }
                    }
                }
            }
        }
    }
    // close the directory
    closedir(dir);
#else
    struct _finddata_t c_file;
    ptrdiff_t hFile;
    string findPath = path + "\\*.*";

    // the first file is obtained
    hFile = _findfirst(findPath.c_str(), &c_file);

    // If the dir doesn't exist
    if (hFile == -1)
        return(false);

    // each file is processed until the last one
    while (_findnext(hFile, &c_file) == 0)
    {
        // for each file (not a directory (_A_SUBDIR), store its name into the fileList
        fullPath = path + "\\" + c_file.name;
        if (!(c_file.attrib & _A_SUBDIR))                                             // Modification: 2011.10
            fileList.push_back(fullPath);
        else if ((strcmp(".", c_file.name) != 0) && (strcmp("..", c_file.name) != 0)) // Modification: 2011.05
        {
            // for each directory, except '.' and '..', its file list is obtained
            GetFileList(fileList, fullPath, symLinks);
        }
    }
    // close the directory
    _findclose(hFile);
#endif
    return(true);
}

/*!
* 1. Function Description:
*    For a given filename, this method checks whether the file matches
*    a given match string containing wildcards (*) and placeholders (?).
*    Return filename matches pattern
*
* 2. Parameters:
*    filename: filename to be checked
*    matchstr: string pattern to match
*
* 3. Creation time and Owner:
*    Version 2011.10
*/
bool CUtil::MatchFilename(const string &filename, const string &matchstr)
{
    unsigned int i, j, k, f, m, fl, ml, s, e, sl, lim;

    fl = filename.length(); // Modification: 2013.04
    ml = matchstr.length();
    if (ml == 0)                 // Modification: 2011.10
        return(fl == 0);
    if (fl == 0)
        return(false);

    f = 0;
    for (m = 0; m < ml; m++)
    {
        if (matchstr[m] == '?')
        {
            f++;
            if (f > fl)
                return(false);
            continue;
        }
        else if (matchstr[m] == '*')
        {
            // search for next non-wild card character
            s = m + 1;
            while (s < ml && matchstr[s] == '*')
                s++;
            if (s >= ml)
                break;
            e = s + 1;
            while (e < ml && matchstr[e] != '*')
                e++;
            sl = e - s;
            lim = fl - sl - f;
            if (e >= ml)
            {
                // check the end of the filename
                if (fl - f < sl)
                    return(false);
                for (j = fl - sl, k = s; j < fl; j++, k++)
                {
                    if (matchstr[k] != '?')
                    {
#ifdef UNIX
                        // case-sensitive match
                        if (matchstr[k] != filename[j])
                            break;
#else
						// TODO:  Windows server installs (and perhaps workstations)
						// allow Case Sensitive file systems.  
						// While processing file paths each file system 
						// can be queried if Case Sensitive.  For NOW, do as below
						//
                        // case-insensitive match
                        if ( lowerChars[ TL_ARR( matchstr[k] ) ] != lowerChars[ TL_ARR( filename[j] ) ] )
                            break;
#endif
                    }
                }
                return(j >= fl);
            }
            for (i = 0; i <= lim; i++)
            {
                for (j = f + i, k = s; j < f + i + sl; j++, k++)
                {
                    if (matchstr[k] != '?')
                    {
#ifdef UNIX
                        // case-sensitive match
                        if (matchstr[k] != filename[j])
                            break;
#else
                        // case-insensitive match
                        if ( lowerChars[ TL_ARR( matchstr[k] ) ] != lowerChars[ TL_ARR( filename[j] ) ] )
                            break;
#endif
                    }
                }
                if (j >= f + i + sl)
                {
                    f += i + sl;
                    break;
                }
            }
            if (i > lim)
                return(false);
            m = e - 1;
            continue;
        }
#ifdef UNIX
        // case-sensitive match
        if (matchstr[m] != filename[f])
            return(false);
#else
        // case-insensitive match
        if ( lowerChars[ TL_ARR( matchstr[m] ) ] != lowerChars[ TL_ARR( filename[f] ) ] )
            return(false);
#endif
        f++;
    }
    if (f < fl && matchstr[ml - 1] != '*')
        return(false);
    return(true);
}

/*!
*1. Function Description:
*    For a given path string, whis method replace the path string with
*    current file folder path.
*    Return file status
*   
*2. Parameters:
*   path: path string what will be replaced with current path
*
*3. Creation time and Owner:
*       Version 2017.02
*       Version 2018.01
*/
int CUtil::GetPath(string &path)
{
        char buffer[MAX_PATH_SIZE];
#ifdef UNIX
        if(getcwd(buffer, MAX_PATH_SIZE) == NULL)
    {
        return 0;  
    }
#else
    if(_getcwd(buffer, MAX_PATH_SIZE) == NULL)
    {
        return 0;
    }
#endif
    path.assign(buffer);
    return 1;
}


/*!
* 1. Function Description:
*    For a given path, this method creates the specified directory path
*    including all required sub-directories.
*    Return file status
*
* 2. Parameters:
*    path: path to create
*
* 3. Creation time and Owner:
*    Version 2011.05
*/
int CUtil::MkPath(const string &path)
{
    size_t i = 1;
    string tpath;
#ifdef UNIX
    struct stat st;
    if (stat(path.c_str(), &st) != 0)
    {
        while (i < path.size())
        {
            if (path[i] == '/')
            {
                tpath = path.substr(0, i);
                if (stat(tpath.c_str(), &st) != 0)
                {
                    if (mkdir(tpath.c_str(), 0777) != 0)
                        return 0;
                }
            }
            i++;
        }
        if (mkdir(path.c_str(), 0777) != 0)
            return 0;
    }
    string tempfile = path + "/___temp.dat";
#else
    if (_access(path.c_str(), 0) != 0)
    {
        while (i < path.size())
        {
            if ((path[i] == '\\' || path[i] == '/') && path[i - 1] != ':') // Modification: 2015.12
            {
                tpath = path.substr(0, i);
                if (_access(tpath.c_str(), 0) != 0)
                {
                    if (_mkdir(tpath.c_str()) != 0)
                        return 0;
                }
            }
            i++;
        }
        if (_mkdir(path.c_str()) != 0)
            return 0;
    }
    string tempfile = path + "\\___temp.dat";
#endif

    // attempt to write a temporary file to the directory
    ofstream outfile;
    outfile.open(tempfile.c_str(), ofstream::out);
    outfile.close();
    if (outfile.fail())
    {
        // file could not be opened
        return 0;
    }
    else
    {
        // delete the temporary file
        remove(tempfile.c_str());
    }
    return 1;
}

/*!             
* 1. Function Description:              
*    For a given path, this method delete all directory and its content.                
*    Return file status         
*               
* 2. Parameters:                
*    path: path to delete               
*               
* 3. Creation time and Owner:           
*    Version 2017.02            
*/
int CUtil::RmPath(const string &path)
{
    string tpath;
#ifdef UNIX             
    DIR *d = opendir(path.c_str());
    size_t path_len = strlen(path.c_str());
    int r = -1;
    if (d)              
    {    
         struct dirent *p;     
         r = 0;
         while (!r && (p=readdir(d)))     
         {              
                int r2 = -1;
                char *buf;              
                size_t len;      
                /* Skip the names "." and ".." as we don't want to recurse on them. */       
                if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))         
                        continue;
                len = path_len + strlen(p->d_name) + 2;
                buf = (char *) malloc(len);
                snprintf(buf, len, "%s/%s", path.c_str(), p->d_name);
                if (buf)
                {                
                        r2 = unlink(buf);
                        free(buf);
                }
                r = r2;
         }
         closedir(d);
    }
    if (!r)
        r = rmdir(path.c_str());
    if (r!=0)
        return 0;
#else
    struct _finddata_t c_file;
    ptrdiff_t hFile;
    string findPath = path + "\\*.*";

    // the first file is obtained
    hFile = _findfirst(findPath.c_str(), &c_file);

    // If the dir doesn't exist
    if (hFile == -1)
        return(false);

    int r = 0;
    string fullPath;
    // each file is processed until the last one
    while (!r && _findnext(hFile, &c_file) == 0)
    {
        int r2 = -1;
        // Skip the names "." and ".." as we don't want to recurse on them
        if (!(strcmp(".", c_file.name)) || !(strcmp("..", c_file.name)))
                continue;
        fullPath = path + "\\" + c_file.name;

        r = _unlink(fullPath.c_str());
    }
    // close the directory
    _findclose(hFile);

    if (!r)
        r = _rmdir(path.c_str());
        if (r!=0)
        return 0;
#endif
    return 1;
}

/*!
* 1. Function Description:
*    Removes extra text added after @@ by ClearCase.
*    The original implementation was provided by NGC.
*    Return modified file name
*    Set trailer to what is removed to later make original file name
*
* 2. Parameters:
*    fileName:          file name
*    clearCaseTrailer:  part of given file name removed here
*
* 3. Creation time and Owner:
*    Version 2011.05
*    Revised 2015.12  Sets trailer added by ClearCase to make original name
*/
string CUtil::ConvertClearCaseFile( const string &fileName, string & clearCaseTrailer )
{
	clearCaseTrailer = "";              // Modification: 2015.12

    // remove @@ and anything after
    string fileName1 = fileName;        // Modification: 2011.10
    size_t loc = fileName1.rfind("@@"); // Modification: 2011.05
    if (loc != string::npos)
	{
		clearCaseTrailer = fileName1.substr( loc ); // Modification: 2015.12
        return fileName1.erase(loc);
	}

	return fileName1;
}

/*!
* 1. Function Description:
*    Checks for line truncation.
*    Return size of string to keep after truncation
*
* 2. Parameters:
*    length: length of current line
*    totalLength: length of current SLOC
*    truncate: allowable number of characters per SLOC
*    trunc_flag: line truncated?
*
* 3. Creation time and Owner:
*    Version 2011.05
*/
size_t CUtil::TruncateLine(size_t length, size_t totalLength, size_t truncate, bool &trunc_flag)
{
    if (truncate == 0)
    {
        trunc_flag = false;
        return(length);
    }
    else if (totalLength >= truncate)
    {
        trunc_flag = true;
        return(0);
    }
    else if (totalLength + length <= truncate)
    {
        trunc_flag = false;
        return(length);
    }
    else
    {
        trunc_flag = true;
        return(truncate - totalLength);
    }
}

/*!
* 1. Function Description:
*    Clear redundant/unnecessary white spaces in a string.
*    Return new string
*
* 2. Parameters:
*    str: string to be processed
*
* 3. Creation time and Owner:
*    Version 2011.05
*/
string CUtil::ClearRedundantSpaces(const string &str)
{
#define SPECIAL_CHARS " \t;[]()+/-*<>=,&~!^?:%{}|"
    size_t len = str.length();
    size_t idx = 0;
    size_t idx_new = 0;
    string str_new(len, '\0');

    for (idx = 0; idx < len; idx++)
    {
        if (str[idx] == ' ' || str[idx] == '\t')                // Modification: 2011.10
        {
            if (idx == 0 || idx + 1 == len
                || strchr(SPECIAL_CHARS, str[idx - 1]) != NULL
                || strchr(SPECIAL_CHARS, str[idx + 1]) != NULL)
            {
                continue;
            }
        }
        if (str[idx] == '\t')                                   // Modification: 2013.04
            str_new[idx_new++] = ' ';
        else
            str_new[idx_new++] = str[idx];
    }
    str_new.resize(idx_new);                                    // Modification: 2011.05

    return str_new;
#undef SPECIAL_CHARS
}

/*!
* 1. Function Description:
*    Returns a string without smart quotes.
*    Return updated string
*
* 2. Parameters:
*    str: original string
*
* 3. Creation time and Owner:
*    Version 2011.10
*    Revised 2015.12  Compiles clean with MS Visual C++ -Wall
*/

// MAke the change to do the below in-place
string CUtil::ReplaceSmartQuotes( const string &str1)
{
    string str = str1;
    std::replace( str.begin(), str.end(), static_cast<char>(static_cast<unsigned char>(145)), '\'' );
    std::replace( str.begin(), str.end(), static_cast<char>(static_cast<unsigned char>(146)), '\'' );
    std::replace( str.begin(), str.end(), static_cast<char>(static_cast<unsigned char>(147)), '\"' );
    std::replace( str.begin(), str.end(), static_cast<char>(static_cast<unsigned char>(148)), '\"' );
    return str;
}


/*!
* 1. Function Description:
*    Checks whether the string starts with a specified string.
*    Return true if the character sequence represented by startsWith is a prefix of the substring of str
*
* 2. Parameters:
*    str: original string
*    startsWith: check string
*    case_sensitive: is case sensitive?
*
* 3. Creation time and Owner:
*    Version 2015.12
*/
bool CUtil::StartsWith(const string &str, const string &startsWith, bool case_sensitive)
{
    size_t strlen = str.length();
    size_t startslen = startsWith.length();

    if (strlen < 1 || strlen < startslen)
        return false;
    else if (startslen < 1)
        return true;

    string str1 = str;
    string startsWith1 = startsWith;
    if (!case_sensitive)
    {
        str1 = CUtil::ToLower(str1);
        startsWith1 = CUtil::ToLower(startsWith1);
    }
    return (str1.substr(0, startslen) == startsWith1) ? true : false;
}

/*!
* 1. Function Description:
*    Checks whether the string ends with a specified string.
*    Return true if the character sequence represented by endsWith is a suffix of the substring of str
*
* 2. Parameters:
*    str: original string
*    startsWith: check string
*    case_sensitive: is case sensitive?
*
* 3. Creation time and Owner:
*    Version 2015.12
*/
bool CUtil::EndsWith(const string &str, const string &endsWith, bool case_sensitive)
{
    size_t strlen = str.length();
    size_t endslen = endsWith.length();

    if (strlen < 1 || strlen < endslen)
        return false;
    else if (endslen < 1)
        return true;

    string str1 = str;
    string endsWith1 = endsWith;
    if (!case_sensitive)
    {
        str1 = CUtil::ToLower(str1);
        endsWith1 = CUtil::ToLower(endsWith1);
    }
    return (str1.substr(strlen - endslen) == endsWith1) ? true : false;
}

/*!
* 1. Function Description:
*    Splits a string by a specified delimiter.
*    Return string vector of tokens or if there is nothing to split or delimiter is empty, the original string is returned as a token and is the only token
*
* 2. Parameters:
*    stringToSplit: string to split
*    delimiter: string delimiter
*
* 3. Creation time and Owner:
*    Version 2015.12
*/
StringVector CUtil::Split(const string &stringToSplit, const string &delimiter)
{
    StringVector tokens;    // tokens contain 1 or more strings that have been split
    size_t start = 0, end = 0;

    if (delimiter.empty())
    {
        tokens.push_back(stringToSplit);
        return tokens;
    }

    while (end != string::npos)
    {
        end = stringToSplit.find(delimiter, start);

        // if at end, use length=maxLength, else use length=end-start
        string token = stringToSplit.substr(start, (end == string::npos) ? string::npos : end - start);
        if (token.length() > 0)
            tokens.push_back(token);

        // if at end, use start=maxSize, else use start=end+delimiter
        start = (end > (string::npos - delimiter.size())) ? string::npos : end + delimiter.size();
    }
    if (tokens.size() < 1)
        tokens.push_back(stringToSplit);    // if delimiter not found, return stringToSplit

    return tokens;
}

/*!
* 1. Function Description:
*    Replaces all instances of string within a string.
*    Return new string
*
* 2. Parameters:
*    originalStr: original string
*    toBeReplaced: string to be replaced
*    replaceWith: string to replace with
*
* 3. Creation time and Owner:
*    Version 2015.12
*/
string CUtil::ReplaceWith(const string &originalStr, const string &toBeReplaced, const string &replaceWith)
{
    string str = originalStr;
    size_t i = str.find(toBeReplaced);
    while (i != string::npos)
    {
        str.replace(i, toBeReplaced.length(), replaceWith);
        i = str.find(toBeReplaced);
    }
    return str;
}

//#ifndef NO32BIT	// NOT USED
/*!
* 1. Function Description:
*    Generates an almost-unique 256-bit (32-byte) hash signature for a text string (SHA-256).
*    Return 256-bit hash
*
* 2. Parameters:
*    fileBuffer: buffer containing file text (512-bit chunks)
*
* 3. Creation time and Owner:
*    Version 2015.12
*/
/************  NOT  USED
string CHash::CalcSHA256(vector<vector<unsigned char> > fileBuffer)
{
//  SHA-256
//    1) All variables are 32-bit unsigned integers and addition is calculated modulo 2^32
//    2) For each round, there is one round constant k[i] and one entry in the message
//    schedule array w[i], 0 <= i <= 63
//    3) The compression function uses 8 working variables, a through h
//    4) Big-endian convention is used when expressing the constants,
//    and when parsing message block data from bytes to words, for example,
//    the first word of the input message "abc" after padding is 0x61626380

//    Based on the pseudocode for SHA-256 found at
//    http://en.wikipedia.org/wiki/SHA-2

//    Information on secure hash standards found at
//    http://csrc.nist.gov/publications/fips/fips180-4/fips-180-4.pdf

//    More information on SHA-256 found at
//    http://www.movable-type.co.uk/scripts/sha256.html
//

    // initialize hash values
    // (first 32 bits of the fractional parts of the square roots of the first 8 primes 2..19):
    uint32_t h0 = 0x6a09e667;
    uint32_t h1 = 0xbb67ae85;
    uint32_t h2 = 0x3c6ef372;
    uint32_t h3 = 0xa54ff53a;
    uint32_t h4 = 0x510e527f;
    uint32_t h5 = 0x9b05688c;
    uint32_t h6 = 0x1f83d9ab;
    uint32_t h7 = 0x5be0cd19;

    // initialize array of round constants
    // (first 32 bits of the fractional parts of the cube roots of the first 64 primes 2..311):
    const uint32_t k[64] = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2 };

    uint32_t a, b, c, d, e, f, g, h;
    uint32_t s0, s1, temp1, temp2, ch, maj;
    uint32_t w[64];
    size_t L, K, TL;

    int si;
    size_t i, j, l;
    unsigned char c0 = 0x00;
    unsigned char c1 = 0x80;
    unsigned char cb;
    stringstream st_h0, st_h1, st_h2, st_h3, st_h4, st_h5, st_h6, st_h7;
    string hash;

    if (fileBuffer.size() < 1)
        fileBuffer.push_back(vector<unsigned char>());

    // preprocessing
    L = fileBuffer[fileBuffer.size() - 1].size() * 8;
    TL = L + ((fileBuffer.size() - 1) * 512);
    if (L >= 448)
    {
        // append the bit '1' to the message (8-bit char with padded '0' = 0x80)
        if (L < 512)
            fileBuffer[fileBuffer.size() - 1].push_back(c1);

        // append '0' bytes to the message until the end
        if (L < 511)
        {
            for (i = L + 8; i < 512; i += 8)
                fileBuffer[fileBuffer.size() - 1].push_back(c0);
        }

        // append another 512-bit buffer to hold the message length
        fileBuffer.push_back(vector<unsigned char>());
    }

    // append the bit '1' to the message (8-bit char with padded '0' = 0x80) if not done already
    if (L < 448 || L == 512)
        fileBuffer[fileBuffer.size() - 1].push_back(c1);

    // append '0' bytes to the message until the end
    L = fileBuffer[fileBuffer.size() - 1].size() * 8;   // includes byte 0x80
    K = 448 - L;
    i = 0;
    while (i < K)
    {
        fileBuffer[fileBuffer.size() - 1].push_back(c0);
        i += 8;
    }

    // append length of message (without the '1' bit or padding), in bits, as 64-bit big-endian integer
	int		sizeT_size = sizeof( size_t ); // declaring this variable avoids constant expression warning
    if ( sizeT_size >= 8 )
    {
        for (si = 7; si >= 0; si--)
        {
            cb = (TL >> si * 8) & 0xff;
            fileBuffer[fileBuffer.size() - 1].push_back(cb);
        }
    }
    else
    {
        for (si = 3; si >= 0; si--)
        {
            cb = TL & 0x00;
            fileBuffer[fileBuffer.size() - 1].push_back(cb);
        }
        for (si = 3; si >= 0; si--)
        {
            cb = (TL >> si * 8) & 0xff;
            fileBuffer[fileBuffer.size() - 1].push_back(cb);
        }
    }

    // process the message in successive 512-bit chunks
    for (i = 0; i < fileBuffer.size(); i++)
    {
        // the initial values in w[0..63] don't matter, so zero them here
        for (j = 0; j < 64; j++)
            w[j] = 0;

        // break chunk into sixteen 32-bit big-endian words w[0..15]
        for (j = 0, l = 0; j < 64; j += 4, l++)
            w[l] = (uint32_t)(fileBuffer[i][j] << 24 | fileBuffer[i][j + 1] << 16 | fileBuffer[i][j + 2] << 8 | fileBuffer[i][j + 3]);

        // extend the sixteen 32-bit words into sixty-four 32-bit words
        for (j = 16; j < 64; j++)
        {
            s0 = RotateRight32Bit(w[j - 15], 7) ^ RotateRight32Bit(w[j - 15], 18) ^ ShiftRight32Bit(w[j - 15], 3);
            s1 = RotateRight32Bit(w[j - 2], 17) ^ RotateRight32Bit(w[j - 2], 19) ^ ShiftRight32Bit(w[j - 2], 10);
            w[j] = (w[j - 16] + s0 + w[j - 7] + s1) & 0xffffffff;
        }

        // initialize hash value for this chunk
        a = h0;
        b = h1;
        c = h2;
        d = h3;
        e = h4;
        f = h5;
        g = h6;
        h = h7;

        // main loop
        for (j = 0; j < 64; j++)
        {
            s1 = RotateRight32Bit(e, 6) ^ RotateRight32Bit(e, 11) ^ RotateRight32Bit(e, 25);
            ch = (e & f) ^ ((~e) & g);
            temp1 = h + s1 + ch + k[j] + w[j];
            s0 = RotateRight32Bit(a, 2) ^ RotateRight32Bit(a, 13) ^ RotateRight32Bit(a, 22);
            maj = (a & b) ^ (a & c) ^ (b & c);
            temp2 = s0 + maj;

            h = g;
            g = f;
            f = e;
            e = (d + temp1) & 0xffffffff;
            d = c;
            c = b;
            b = a;
            a = (temp1 + temp2) & 0xffffffff;
        }

        // add this chunk's hash to result so far
        h0 = (h0 + a) & 0xffffffff;
        h1 = (h1 + b) & 0xffffffff;
        h2 = (h2 + c) & 0xffffffff;
        h3 = (h3 + d) & 0xffffffff;
        h4 = (h4 + e) & 0xffffffff;
        h5 = (h5 + f) & 0xffffffff;
        h6 = (h6 + g) & 0xffffffff;
        h7 = (h7 + h) & 0xffffffff;
    }

    // produce the final hash value (big-endian)
    st_h0 << hex << h0;
    st_h1 << hex << h1;
    st_h2 << hex << h2;
    st_h3 << hex << h3;
    st_h4 << hex << h4;
    st_h5 << hex << h5;
    st_h6 << hex << h6;
    st_h7 << hex << h7;
    hash = st_h0.str() + st_h1.str() + st_h2.str() + st_h3.str() + st_h4.str() + st_h5.str() + st_h6.str() + st_h7.str();

    return hash;
}
*********   END OF NOT USED *******/
//#endif		// NOT USED

int CUtil::CountNestedNum(string &combine){
    //"a==1&&b==1 return 2"
    int res=0;
    for( unsigned int i = 0; i < combine.size(); i++ )
	{
        if(combine[i]=='&')
            res++;
    }
    return res/2+1;
}

vector<string> CUtil::SplitByConcat(string &statement, string concat){
    vector<string> result;
    string temp_statement = statement;
    unsigned int idx;
    string left;
    string right;
    while(temp_statement.find(concat) != string::npos){
        idx = temp_statement.find(concat);
        left = temp_statement.substr(0, idx);
        right = temp_statement.substr(idx + concat.length(), string::npos);
        result.push_back(left);
        temp_statement = temp_statement.substr(idx + concat.length(), string::npos);
    }
    result.push_back(right);
    return result;
}

size_t CUtil::ConcatAndDup(string &cc4_valid_if, set<string> &nested_set){
    set<string>::iterator it;
    string temp;
    if(nested_set.count(cc4_valid_if) == 1){
        return (size_t)CountNestedNum( cc4_valid_if );
    }

    for(it=nested_set.begin(); it!=nested_set.end(); it++){
        temp= *it;
        if(temp.find(cc4_valid_if)!= string::npos)
            return (size_t)CountNestedNum( cc4_valid_if );
        if(cc4_valid_if.find(temp)!=string::npos)
            return (size_t)CountNestedNum( temp );
    }

    return 0;
}

void CUtil::ConcatOrReorder(set<string> &string_set, string &statement){

    if(statement.find("||")==string::npos)
        return;
    vector<string> original = SplitByConcat(statement, "||");
    //make unique
    sort( original.begin(), original.end() );
    original.erase( unique( original.begin(), original.end() ), original.end() );

    vector<string> permute;
    string output;
    if( original.size() <= 1 ) //no || in statement
        return;

    set<int> visited;
    ConcatOrReorderDFS( original, 0, visited, output, permute );
    for( unsigned int i = 0; i < permute.size(); i++ )
	{
        if ( string_set.count( permute[i] ) == 1 )
		{
            statement = permute[i];
            return;
        }
    }
    statement=permute[0];
    return;
}

/*!
* 1. Function Description:
*    This takes a DFS (whatever that is) and does a concat(enation) or Reorder
*    Strongly suggest someone review these comments and improve.
*
* 2. Parameters:
*    original		IN/OUT  reference to a string holding some DFS stuff ?
*    step			IN		integer representing some aspect of some DFS Reorder or Concat ?
*    visited		IN/OUT  integers representing the visited states ?
*    output			IN/OUT  result of ?
*    permute		IN/OUT  some change to do or that was done ?
*
* 3. Creation time and Owner:
*    Version ????.??
*    Revused 2015.12  Removed recursion to decrease use of RAM memory and run faster!
*/

struct concatOrReorderDFSCallStruct 
{
	int		step;	// - parameter input
	unsigned int i;	// - local variable that will be used
					// after returning from the function call
	int		place;	// - Since there is process needed to be done
					// after recursive call. (Sixth rule)
};

void CUtil::ConcatOrReorderDFS( vector<string> &original, const int step, set<int> &visited, string &output, vector<string> &permute )
{
//   NEW Approach without Recursion.  NEEDS more work!

	concatOrReorderDFSCallStruct	currentCall;
	currentCall.step  = step;	// Start with given value
	currentCall.i     = 0;
	currentCall.place = 0;		// Start with the code before the first simulated return

	// Stack to handle int step value that changes.  Other args are references and not needed in stack.
	stack<concatOrReorderDFSCallStruct> noRecursionStk;
	noRecursionStk.push( currentCall );	// Save initial value

	while ( !noRecursionStk.empty() )
	{
		// Entry point from either outside initial call 
		// or inside simulated recursive calls
		currentCall = noRecursionStk.top();
		noRecursionStk.pop();
		switch( currentCall.place )
		{
		default:
		case 0:
			if ( currentCall.step == (int)original.size() )
			{
				permute.push_back( output.substr( 2, string::npos ) );
			
				// Simulate a return
				// return;
				continue;
			}
			// Allow to fall through
		case 1:
		loopTop:
			for ( unsigned int i = currentCall.i; i < original.size(); i++ )
			{
				if( visited.find( (int)i ) == visited.end() )
				{
					visited.insert( i );
					output = output + "||" + original[i];

					// Simulate a recursive call
					// ConcatOrReorderDFS( original, step + 1, visited, output, permute );
					//
					// currentCall needed after returning from the recursive call
					currentCall.place = 2;
					currentCall.i     = i;
					noRecursionStk.push( currentCall );

					// Create a new struct for calling itself
					concatOrReorderDFSCallStruct	newCallStruct;

					// Set parameters as when calling itself
					newCallStruct.step = currentCall.step + 1;
					
					// Set values as initial values since it will start from the
					// beginning of the function (while loop)
					newCallStruct.i = 0;
					newCallStruct.place = 0;	// give the initial place

					noRecursionStk.push( newCallStruct );
					continue;		// to while loop to simulate the recursive call

					// These are done below as case 2
					// output.resize( output.size() - original[i].size() - 2 );
					// visited.erase( (int)i );
				}
			}
			break;
		case 2:
			// Do code after simulated recursive call

			output.resize( output.size() - original[ currentCall.i ].size() - 2 );
			visited.erase( (int)currentCall.i );

			// Must increment currentCall.i as coming from top of loop, not inside anymore
			currentCall.i++;
			goto loopTop;
			break;
		}
	}

	return;		// Really done here...

/*
	// RECURSIVE and Hostile to RAM and SLOWER version
	if( step == (int)(original.size()) )
	{
		permute.push_back( output.substr( 2, string::npos ) );
		return;
	}
	for( unsigned int i = 0; i < original.size(); i++ )
	{
		if( visited.find( (int)i ) == visited.end() )
		{
			visited.insert( i );
			output = output + "||" + original[i];

			ConcatOrReorderDFS( original, step + 1, visited, output, permute );

			output.resize( output.size() - original[i].size() - 2 );
			visited.erase( (int)i );
		}
	}
	return;
*/
}

size_t CUtil::NestedIfDup(string &cc4_valid_if, stack<string> &cc4_parent_stack, stack<set<string> > &cyclomatic_distinct_cond_stack, set<string> &nested_set){

	if(cyclomatic_distinct_cond_stack.size()==0 || cc4_parent_stack.size()==0){ //fix a bug that sometimes the stack may be empty (CC4 Integration 2015 Summer)
		return 0;
	}

	//cc4_valid_if && cc4_parent_stack.top()
    set<string> temp_set = cyclomatic_distinct_cond_stack.top();
    set<string>::iterator it;
    string parent = cc4_parent_stack.top();
    size_t dup_counter=0;

    if(cyclomatic_distinct_cond_stack.size()==1){
        string combine = parent + "&&" + cc4_valid_if;

        if(nested_set.find(combine)!=nested_set.end()){
            AddTraceLine(__LINE__, UCC_FUNC_NAME, "CC4: already counted the dup");
            return 0;

        }
        nested_set.insert(combine);
        std::ostringstream ss;
        ss << "CC4: insert " << combine << " to nested_set";
        AddTraceLine(__LINE__, UCC_FUNC_NAME, ss.str());

        if(temp_set.find(combine)!=temp_set.end()){
            //can find a && dup, counter && number
            return (size_t)CountNestedNum( combine );
        }
        return 0;


    }

    cyclomatic_distinct_cond_stack.pop();
    cc4_parent_stack.pop();
    string new_cc4_valid_if = parent+"&&"+cc4_valid_if;

	// RECURSIVE CALL... What were you THINKING ! ! !
    dup_counter = NestedIfDup( new_cc4_valid_if, cc4_parent_stack, cyclomatic_distinct_cond_stack, nested_set );

    cyclomatic_distinct_cond_stack.push(temp_set);
    cc4_parent_stack.push(parent);

    return dup_counter;
}


// Below is an offensively RECURSIVE implementation
//
// Of couse no meaningful comments to start with (my code is so great you will enjoy reading EVERY line)
void CUtil::SemanticFormat(string &statement)
{
    size_t idx;
    string left;
    string right;
    string eq = "==", ne = "!=", eqT = "==1", eqF = "==0", concat_op_and = "&&", concat_op_or="||";

    // Modification: Spring 2016 - Below variables are changed from 'unsigned int' to 'size_t' to 
    // avoid warningi(comparisonof constant and unsigned int is always true) on clang++ compiler.
    size_t eq_len = eq.length();
    size_t ne_len = ne.length();
    size_t eq_pos = (size_t)(-1), ne_pos = (size_t)(-1);
    size_t tnum_len = string("1").length();
    size_t tstr_len = string("true").length();
    size_t fnum_len = string("0").length();
    size_t fstr_len = string("false").length();

    statement.erase(remove_if(statement.begin(), statement.end(), ::isspace), statement.end());

    // For loop handling: Currently "for" keyword is searched and upon encounter, is stripped from the statement
    // and the rest of the statement is recorded as a condition. We need to record specific condition within a
    // for loop, not the entire statement.
    if(statement.find(";", 0) != string::npos) {
    	unsigned int firstSC = statement.find(";", 0);
    	if(statement.find(";", firstSC + 1) != string::npos) {
    		unsigned int secondSC = statement.find(";", firstSC + 1);
    		if(firstSC != secondSC + 1) {
    			string condFOR = statement.substr(firstSC + 1, secondSC - firstSC - 1);
    			//distinct_cond_set.erase(it);
    			//it = distinct_cond_set.find(temp);
    			statement = condFOR;
    			//distinct_cond_set.insert(condFOR);
    		}
    	}
    }

	if ( statement.size() == 0 )	// Modified: 2015.12
		return;

    if(statement.find(concat_op_and) == string::npos && statement.find(concat_op_or) == string::npos){

        if(statement[0]=='!'){
            eq_pos = statement.find(eq);
            ne_pos = statement.find(ne);
            if( eq_pos != (string::npos) ) // Modification: Spring 2016 - npos is of type size_t
			{
                if(statement.substr(eq_pos + eq_len, tnum_len) == "1" || statement.substr(eq_pos + eq_len, tstr_len) == "true") {
                    statement = statement.substr(1, string::npos) += eqF;
                } else if(statement.substr(eq_pos + eq_len, fnum_len) == "0" || statement.substr(eq_pos + eq_len, fstr_len) == "false") {
                    statement = statement.substr(1, string::npos) += eqT;
                }
            } 
			else if( ne_pos != (string::npos) ) // Modification: Spring 2016 - npos is of type size_t
			{
                if(statement.substr(ne_pos + ne_len, tnum_len) == "1" || statement.substr(ne_pos + ne_len, tstr_len) == "true") {
                    statement = statement.substr(1, string::npos) += eqT;
                } else if(statement.substr(ne_pos + ne_len, fnum_len) == "0" || statement.substr(ne_pos + ne_len, fstr_len) == "false") {
                    statement = statement.substr(1, string::npos) += eqF;
                }
            } else {
                statement = statement.substr(1, string::npos) += eqF;
            }
            return;
        }

        if (statement.find(eq) == string::npos && statement.find(ne) == string::npos){
            statement = statement + eqT;
            return ;
        }

        idx = statement.find(eq);
        if (idx != string::npos){

            left = statement.substr(0, idx);
            right = statement.substr(idx + eq_len, string::npos);

            if(left=="true" || left=="1") {
                statement = right + eqT;
            } else if(right=="true" || right=="1") {
                statement = left + eqT;
            }
            if(left=="false" || left=="0") {
                statement = right + eqF;
            } else if(right=="false" || right=="0"){
                statement = left + eqF;
            }
            return;
        }

        idx = statement.find(ne);
        if (idx != string::npos){

            left = statement.substr(0, idx);
            right = statement.substr(idx + ne_len, string::npos);

            if(left=="true" || left=="1") {
                statement = right + eqF;
            } else if (right=="true" || right=="1") {
                statement = left + eqF;
            }
            if(left=="false" || left=="0") {
                statement = right + eqT;
            } else if (right=="false" || right=="0"){
                statement = left + eqT;
            }
            return;
        }

    }else{
        // for &&
        if(statement.find(concat_op_and) != string::npos){
            string temp_statement = statement;
            statement = "";
            while(temp_statement.find(concat_op_and) != string::npos){
                idx = temp_statement.find(concat_op_and);
                left = temp_statement.substr(0, idx);
                right = temp_statement.substr(idx + concat_op_and.length(), string::npos);


                SemanticFormat(left);

                statement= statement + left + concat_op_and;
                temp_statement = temp_statement.substr(idx + concat_op_and.length(), string::npos);
            }


            SemanticFormat(right);

            statement = statement + right;
        }

        // for ||
        if(statement.find(concat_op_or) != string::npos){
            string temp_statement = statement;
            statement = "";
            while(temp_statement.find(concat_op_or) != string::npos){
                idx = temp_statement.find(concat_op_or);
                left = temp_statement.substr(0, idx);
                right = temp_statement.substr(idx + concat_op_or.length(), string::npos);


                SemanticFormat(left);

                statement= statement + left + concat_op_or;
                temp_statement = temp_statement.substr(idx + concat_op_or.length(), string::npos);
            }


            SemanticFormat(right);

            statement = statement + right;
        }

        return;
    }
}

size_t CUtil::SemanticDeduplicate(string &cc4_valid_if, stack<string> &cc4_parent_stack, stack<set<string> > &cyclomatic_distinct_cond_stack, set<string> &nested_set)
{
    if(cc4_valid_if!=""){
        std::ostringstream ss;

        ss << "CC4: cc4_valid_if " << cc4_valid_if;
        AddTraceLine(__LINE__, UCC_FUNC_NAME, ss.str());
        ss.str("");
        ss.clear();
        if(cc4_valid_if.find("&&") == string::npos){
            if(cc4_parent_stack.size()!=0){
                        // && comes first and nested if comes later.
                return CUtil::NestedIfDup(cc4_valid_if, cc4_parent_stack, cyclomatic_distinct_cond_stack, nested_set);
            }
        }else{
            if(cc4_valid_if.find("&&") != string::npos){
                ss << "CC4: get&&: " << cc4_valid_if;
                AddTraceLine(__LINE__, UCC_FUNC_NAME, ss.str());
                        // this is the case that nested comes first and && comes later
                return CUtil::ConcatAndDup(cc4_valid_if, nested_set);
            }
        }
    }
    return 0;
}

void CUtil::CountDistinctCond(string &valid_statement, const string &base, StringVector &container, unsigned int &count, int mode, const string &exclude,
                              const string &include1, const string &include2, set<string> &distinct_cond_set, UIntVector* counter_container, bool case_sensitive)
{
    string::size_type idx, left_bracket_idx;
    string base1;
    StringVector::iterator vit;
    UIntVector::iterator cit;
    unsigned int single_count = 0;
    base1 = " " + base + " ";
    valid_statement="";


    if (counter_container)
        cit = counter_container->begin();

    if (case_sensitive == false)
    {
        base1 = CUtil::ToLower(base1);
        for (vit = container.begin(); vit != container.end(); vit++)
            (*vit) = CUtil::ToLower((*vit));
    }

    unsigned int	index = (unsigned int)(-1); // keep unsigned to match length and ...

    if (mode == 1)
    {
        // exclude mode
        for (vit = container.begin(); vit != container.end(); vit++)
        {
            idx = base1.find((*vit));
            while (idx != string::npos)
            {
                if ((exclude.find(base1[idx+((*vit).size())]) == string::npos) &&
                    (exclude.find(base1[idx-1]) == string::npos))
                {
                    count++;
                    single_count++;
                    left_bracket_idx = base1.find("(", idx);
                    if (left_bracket_idx != string::npos)
                    {
                        index = left_bracket_idx + 1;
                        int level = 1;
                        while(index < base1.length() && level > 0)
                        {
                            if (base1[index] == '(') ++level;
                            if (base1[index] == ')') --level;
                            index++;
                        }
                        if (index < base1.length()){

                            string temp = base1.substr(left_bracket_idx+1, index - left_bracket_idx-2);
							if ( temp.size() > 0 )
							{
								CUtil::SemanticFormat( temp );

								CUtil::ConcatOrReorder( distinct_cond_set, temp ); // only check order
								distinct_cond_set.insert( temp );
								valid_statement = temp;
							}
                        }
                    }
                    else
                    {
                        index = idx + (*vit).size();
                    }
                }
                else
                {
                    index = idx + (*vit).size();
                }
                idx = base1.find((*vit), index);
            }
            if (counter_container)
            {
                (*cit) += single_count;
                single_count = 0;
                cit++;
            }
        }
    }
    else if (mode == 2)
    {
        // include mode
        for (vit = container.begin(); vit != container.end(); vit++)
        {
            idx = base1.find((*vit));
            while (idx != string::npos)
            {
                if ((include1.find(base1[idx-1]) != string::npos) &&
                    (include2.find(base1[idx+((*vit).size())]) != string::npos))
                    count++;
                idx = base1.find((*vit), idx + ((*vit).size()));
            }
        }
    }
}

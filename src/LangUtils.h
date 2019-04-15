//! Language aware Utility structures and procedures.
/*!
* \file LangUtils.h
*
* This file contains support procedures that can be used by:
* the main thread or by worker threads
*
* Secondary goal is to move some code out of the very large MainObject.cpp file
*
* ADDED to UCC 2015_12 release by Randy Maxwell
*   Changes started on 2015_10_02
*   Changes  ended  on 2015_10_16
*/

#ifndef LANG_UTILS_PUBLIC_H
#define LANG_UTILS_PUBLIC_H

// List of all known Language specific parsers
//
// After you add an include for another language here:
//   add in cc_main.h ClassType enum
//   add in LangUtils.cpp Init_CounterForEachLanguage
//
#include "CCodeCounter.h"
#include "CAdaCounter.h"
#include "CAssemblyCounter.h"
#include "CBatchCounter.h"
#include "CBashCounter.h"
#include "CCobolCounter.h"			// Modified: 2015.12
#include "CCCounter.h"
#include "CCFScriptCounter.h"
#include "CColdFusionCounter.h"
#include "CCsharpCounter.h"
#include "CCshCounter.h"
#include "CCssCounter.h"
#include "CDataCounter.h"
#include "CFortranCounter.h"
#include "CHtmlCounter.h"
#include "CIdlCounter.h"
#include "CJavaCounter.h"
#include "CJavascriptCounter.h"
#include "CMakefileCounter.h"
#include "CMatlabCounter.h"
#include "CNeXtMidasCounter.h"
#include "CObjCCounter.h"           // Modified: 2016.07
#include "CPascalCounter.h"
#include "CPerlCounter.h"
#include "CPhpCounter.h"
#include "CPythonCounter.h"
#include "CRubyCounter.h"
#include "CScalaCounter.h"			// Modified: 2015.12
#include "CSqlCounter.h"
#include "CVbCounter.h"
#include "CVbscriptCounter.h"
#include "CVerilogCounter.h"
#include "CVHDLCounter.h"
#include "CWebCounter.h"
#include "CXMidasCounter.h"
#include "CXmlCounter.h"


int Init_CounterForEachLanguage( CounterForEachLangType & CounterForEachLanguage );

/*!
* Resets all count lists (e.g., directive_count, data_name_count, etc.).
*/
void ResetCounterCounts( CounterForEachLangType & eachLangCounters );

/*!
* Sets the Physical line size Truncation limit.  
* Get each Language's file extensions in a string if wanted.
*/
void SetCounterOptions( CounterForEachLangType & eachLangCounters );

// Returns the related Language specific parsing class for a given file name
ClassType DecideLanguage( CounterForEachLangType & CounterForEachLanguage,
						CCodeCounter** counter,
						const bool print_cmplx,
						const string &file_name, 
						const bool setCounter = true );

string GetLanguageName( CounterForEachLangType & CounterForEachLanguage, 
						const ClassType class_type, const string &file_name );

/*!
* Updates count lists (e.g., directive_count, data_name_count, etc.) based on file counts.
*
* \param useListA use file list A? (otherwise use list B)
*/
void UpdateCounterCounts( CounterForEachLangType & eachLangCounters,
						SourceFileList * mySourceFile,
						const bool useListA = true, const bool resetCounts = true );


/*!
* AddFromOtherLangCounters add counts to Main thread structure from Threads
*
* \SideEffects	This will clear Source counters to zero after accumulation
*
* \param	pDestLangCounters	IN/OUT	pointer to Language classes whose info needs updating
* \param	pSrcLangCounters	IN/OUT	pointer to Language classes that has new info
* \param	my_useListA			IN		List A counters (otherwise counters for List B)
*/
void AddFromOtherLangCounters( CounterForEachLangType * destLangCounters,
							CounterForEachLangType * srcLangCounters,
							const bool useListA = true );

// Changes a given string to have a list of Language names and file Extensions (1 Language per line)
void GetLanguagesAndExtensions( CounterForEachLangType & eachLangCounters, string & langExtensions );

// Changes a given string to have a sorted list of File Extensions with related Language names (1 file Extension per line)
void GetExtensionsAndLanguages( CounterForEachLangType & eachLangCounters, string & extensionsAndLangs, bool & multipleLangs );

// Helper to clean up 
void FreeLangsHeaders( vector<struct langTypeArrayHeader> * pLangs );

	//! Structure to contain information about each file in a given file list
	/*!
	* \struct langTypeArrayHeader
	*
	* Defines a structure that is: a pointer to source file element and flags used to speed up Duplicate checking
	*/
// Flags used in below struct.  Must be powers of 2.
#define		IS_EMBEDDED		1
#define		IS_FIRST_DUP	2
#define		IS_DUP			4
#define		IS_ANY_DUP		(IS_FIRST_DUP|IS_DUP)

	struct filePtrFlags
	{
		filePtrFlags()
		{
			pSrcFile = NULL;
			flags    = 0;
		}
		SourceFileElement * pSrcFile;		//!< Address of a file element
		unsigned int		flags;			//!< bit flags that apply to the file, see above.
	};

	//! Structure to contain information to follow an array of pointers
	/*!
	* \struct langTypeArrayHeader
	*
	* Defines a header structure to contain info about an array of pointers of a given Language (class type)
	*/
	struct langTypeArrayHeader
	{
		langTypeArrayHeader()
		{
			pFilePtrs    = NULL;
			num_pointers = 0;
			array_Lang   = UNKNOWN;
		}
		struct filePtrFlags	*	pFilePtrs;		//!< Address of first element
		unsigned int			num_pointers;	//!< Number of pointers in the pointer array
		unsigned int			array_Lang;		//!< This Array points to what Language file structures 
	};


// Get the Start positions used to speed up Duplicate checking
int GetStartPositions( vector<struct langTypeArrayHeader> * pLangs, const bool useListA );

#endif		//	END		#ifndef		LANG_UTILS_PUBLIC_H

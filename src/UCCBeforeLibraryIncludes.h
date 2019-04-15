//! This has support to allow -Wall compiles (currently for Visual C++)
/*!
* \file UCCBeforeLibraryIncludes.h
*
* NEW file added to UCC
* 2015.10.02
* by Randy Maxwell
*
* To support -Wall compiling:
* this file MUST be BEFORE any library file include
*
*     Background:
* There has been ongoing problems with UCC compiling cleanly using Visual C++ Warning level 4 -W4
*
* After a little experimentation this approach was found to enable -Wall Visual C++ compiles as:
*
*   (This below sequence is the order of includes for any given UCC .cpp source file.)
* #include "UCCBeforeLibraryIncludes.h"
*
* The Warning level is reduced to W3 before including various compiler, std and 3rd party libraries.
*   ...
*   #includes for compiler, std and 3rd party libraries happen here
*   ...
* This prevents getting -Wall warnings from components that will not be changed by UCC maintainers.
*
*   Next include the file that complements this one:
* #include "UCCAfterLibraryIncludes.h"
*
* Which restores the command line setting of -W4 or Wall which allows stricter checking of UCC sources.
*
*   Now the various UCC header files and the rest of the .cpp code follow, 
*   all compiled with stricter checking.
*
*   WHY -Wall compiling using Visual C++ is important.
*   --------------------------------------------------
* -Wall with Visual C++ shows many MORE warnings than -w4 or lower levels of strict checking.
* Microsoft has a list of Warnings that are OFF by default UNLESS -Wall is used!
* This is a questionable implementation choice by Microsoft especially since there is NO pragma support
* to enable -Wall selectively.  You must enable knowing exact warning message number(s).
* Of course the exact warning message set of numbers vary depending on the compiler version.
*	NOTE: 
* some of the off by default messages are for known C++ best practices found in Effective C++ books ! ! !
*
* So the extra Warnings and especially the conditions being checked is a VERY strong motivation.  
* Even if this approach seems a little hacky, 
* the benefits of having the compiler give warnings cannot be overstated.
*
* Implementation Notes:
*   NO include guard for this!
* This is a rare instance of an include file that should NOT have the include guard (commented out below).
* Reasons for most include guards are for efficiency or to prevent unwanted duplicate definitions.
* Neither reason applies here.
*
* For example see  cc_main.h 
* where std includes need to have this include BEFORE them
* and have include for UCCAfterLibraryIncludes.h  AFTER them.
*
*   DRY Principle:
* The fragment below did not have to be in a separate file 
* but as it will be needed in various .cpp files and some .h files
* it is best to follow the principle of "Don't Repeat Yourself" (DRY) and place it here.
*/

// NOT using include guard !
// #ifndef UCCBeforeLibraryIncludes_H
// #define UCCBeforeLibraryIncludes_H

// If Microsoft compiler then set Warning level to 3.  This is less strict than -Wall or 4.
// See comments above for correct usage and reasons why.
#ifdef _MSC_VER
	#pragma warning( push, 3 )
	#pragma warning( disable: 4710 ) // disable: function not inlined
	#pragma warning( disable: 4711 ) // disable: function 'x' selected for automatic inline expansion
	#pragma warning( disable: 4738 ) // disable: storing 32-bit float result in memory, possible loss of performance
#endif

// #endif	// NOT using include guard !
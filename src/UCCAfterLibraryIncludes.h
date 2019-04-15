//! This has support to allow -Wall compiles (currently for Visual C++)
/*!
* \file UCCAfterLibraryIncludes.h
*
* NEW file added to UCC
* 2015.10.02
* by Randy Maxwell
*
* To support -Wall compiling:
* this file MUST be AFTER UCCMustBeFirstInclude.h was included
*
* this file MUST be AFTER compiler, std and 3rd party library includes
*
* this file MUST be BEFORE any UCC source .h files are included
*
*     Background:
* Read  UCCMustBeFirstInclude.h  for important details.
*/

// NOT using include guard !
// #ifndef UCCBeforeOtherUCCIncludes_H
// #define UCCBeforeOtherUCCIncludes_H

// If Microsoft compiler restore previous warning settings (could be stricter W4 or -Wall)
#ifdef _MSC_VER
	#pragma warning( pop )
	// using -Wall still get the below from various Microsoft headers so explicitly disable
	#pragma warning( disable: 4514 ) // disable: unreferenced inline function has been removed
	#pragma warning( disable: 4710 ) // disable: 'x' function not inlined
	#pragma warning( disable: 4711 ) // disable: function 'x' selected for automatic inline expansion

	// Warning from packing our structures. 'n' can be '1' '2' '3' or '4'?  struct name and element are given in msg
	#pragma warning( disable: 4820 ) // 'n' bytes padding added after data member 'struct_name::element_name'

	// Disable this rather than change code as change will not be cross platform/cross compiler
	#pragma warning( disable: 4571 ) // Informational: catch(...) semantics changed since Visual C++ 7.1; structured exceptions (SEH) are no longer caught

	// Windows defines PASCAL as a legacy call interface 
	// so undefine it here so we can use PASCAL as a Language name
	#ifdef	PASCAL
		#undef	PASCAL
	#endif
#endif

// #endif // NOT using include guard !
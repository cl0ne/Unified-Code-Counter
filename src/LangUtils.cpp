//! Language container structure aware Utility procedures.
/*!
* \file LangUtils.cpp
*
* This file contains support procedures that can be used by:
*   the main thread or by worker threads.
*
* Secondary goal is move code out of the very large MainObject.cpp class and file
*
* ADDED to UCC 2015_12 release by Randy Maxwell
*   Changes started on 2015_10_03
*   Changes  ended  on 2015_10_06
*/

#include "UCCGlobals.h"
#include "UserIF.h"
#include "LangUtils.h"

extern UserIF *userIF;


// Prototypes
//
bool CompareByNum( const struct langTypeArrayHeader & a, const struct langTypeArrayHeader & b );


/*!
* 1.Function Description:
*	 Creates 1 of each detailed Language parser class.
*    Inserts pointer to each newly created class into passed in structure.
*    See note in UCCThreads.cpp for WHY this can be exciting to call from multiple threads!
*
* 2.Parameters:
*    CounterForEachLanguage   IN/OUT   reference to struct holding pointers to Language specific parser classes
*
* 3.Creation Time And Owner:
*	 Version: 2015.12
*/
int Init_CounterForEachLanguage( CounterForEachLangType & CounterForEachLanguage )
{
	int retVal = 0;

    CCodeCounter* tmp;          // Modifacation: 2009.01

    tmp = new CCodeCounter; // Modifacation: 2009.01
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(UNKNOWN, tmp));

    tmp = new CDataCounter; // Modifacation: 2011.10
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(DATAFILE, tmp));

    tmp = new CWebCounter;  // Modifacation: 2009.01
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(WEB, tmp));

    tmp = new CAdaCounter;  // Modifacation: 2009.01
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(ADA, tmp));

    tmp = new CAssemblyCounter;
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(ASSEMBLY, tmp));

    tmp = new CBatchCounter;    // Modifacation: 2014.08
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(BATCH,tmp));

    tmp = new CBashCounter; // Modifacation: 2011.05
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(BASH,tmp));

    tmp = new CCobolCounter; // Modification: 2015.12
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(COBOL,tmp));

	tmp = new CCshCounter;  // Modifacation: 2011.05
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(CSH, tmp));

    tmp = new CCCounter;    // Modifacation: 2009.01
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(C_CPP, tmp));

    tmp = new CCsharpCounter;   // Modifacation: 2009.01
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(CSHARP, tmp));

    tmp = new CCsharpHtmlCounter;   // Modifacation: 2009.01
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(CSHARP_HTML, tmp));

    tmp = new CCsharpXmlCounter;    // Modifacation: 2011.10
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(CSHARP_XML, tmp));

    tmp = new CCsharpAspCounter;    // Modifacation: 2009.01
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(CSHARP_ASP_S, tmp));

    tmp = new CColdFusionCounter;   // Modifacation: 2011.05
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(COLDFUSION, tmp));

    tmp = new CCFScriptCounter; // Modifacation: 2011.05
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(CFSCRIPT, tmp));

    tmp = new CCssCounter;  // Modifacation: 2009.01
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(CSS, tmp));

    tmp = new CFortranCounter;  // Modifacation: 2011.05
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(FORTRAN, tmp));

    tmp = new CHtmlCounter; // Modifacation: 2009.01
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(HTML, tmp));

    tmp = new CHtmlPhpCounter;  // Modifacation: 2009.01
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(HTML_PHP, tmp));

    tmp = new CHtmlJspCounter;  // Modifacation: 2009.01
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(HTML_JSP, tmp));

    tmp = new CHtmlAspCounter;  // Modifacation: 2009.01
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(HTML_ASP, tmp));

    tmp = new CHtmlColdFusionCounter;   // Modifacation: 2011.05
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(HTML_CFM, tmp));

    tmp = new CIdlCounter;
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(IDL, tmp));

    tmp = new CJavaCounter; // Modifacation: 2009.01
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(JAVA, tmp));

    tmp = new CJavaJspCounter;  // Modifacation: 2009.01
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(JAVA_JSP, tmp));

    tmp = new CJavascriptCounter;   // Modifacation: 2009.01
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(JAVASCRIPT, tmp));

    tmp = new CJavascriptHtmlCounter;   // Modifacation: 2009.01
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(JAVASCRIPT_HTML, tmp));

    tmp = new CJavascriptXmlCounter;    // Modifacation: 2011.10
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(JAVASCRIPT_XML, tmp));

    tmp = new CJavascriptPhpCounter;    // Modifacation: 2009.01
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(JAVASCRIPT_PHP, tmp));

    tmp = new CJavascriptJspCounter;    // Modifacation: 2009.01
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(JAVASCRIPT_JSP, tmp));

    tmp = new CJavascriptAspServerCounter;  // Modifacation: 2009.01
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(JAVASCRIPT_ASP_S, tmp));

    tmp = new CJavascriptAspClientCounter;  // Modifacation: 2009.01
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(JAVASCRIPT_ASP_C, tmp));

    tmp = new CJavascriptColdFusionCounter; // Modifacation: 2011.05
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(JAVASCRIPT_CFM, tmp));

    tmp = new CMakefileCounter; // Modifacation: 2013.04
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(MAKEFILE, tmp));

    tmp = new CMatlabCounter;   // Modifacation: 2013.04
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(MATLAB, tmp));

    tmp = new CNeXtMidasCounter;    // Modifacation: 2011.05
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(NEXTMIDAS, tmp));

    tmp = new CXMidasCounter;   // Modifacation: 2011.05
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(XMIDAS, tmp));

    tmp = new CObjCCounter;   // Modifacation: 2016.07
	CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(OBJC, tmp));

    tmp = new CPascalCounter;   // Modifacation: 2011.10
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(PASCAL, tmp));

    tmp = new CPerlCounter; // Modifacation: 2009.01
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(PERL, tmp));

    tmp = new CPhpCounter;  // Modifacation: 2009.01
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(PHP, tmp));

    tmp = new CPythonCounter;   // Modifacation: 2011.05
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(PYTHON, tmp));

    tmp = new CRubyCounter; // Modifacation: 2011.10
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(RUBY, tmp));

    tmp = new CScalaCounter;  // Modification: 2015.12
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(SCALA, tmp));

    tmp = new CSqlCounter;  // Modifacation: 2009.01
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(SQL, tmp));

    tmp = new CSqlColdFusionCounter;    // Modifacation: 2011.05
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(SQL_CFM, tmp));

    tmp = new CVbCounter;   // Modifacation: 2009.01
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(VB, tmp));

    tmp = new CVbscriptCounter; // Modifacation: 2011.10
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(VBSCRIPT, tmp));

    tmp = new CVbsHtmlCounter;  // Modifacation: 2009.01
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(VBS_HTML, tmp));

    tmp = new CVbsXmlCounter;   // Modifacation: 2011.10
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(VBS_XML, tmp));

    tmp = new CVbsPhpCounter;   // Modifacation: 2009.01
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(VBS_PHP, tmp));

    tmp = new CVbsJspCounter;   // Modifacation: 2009.01
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(VBS_JSP, tmp));

    tmp = new CVbsAspServerCounter; // Modifacation: 2009.01
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(VBS_ASP_S, tmp));

    tmp = new CVbsAspClientCounter; // Modifacation: 2009.01
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(VBS_ASP_C, tmp));

    tmp = new CVbsColdFusionCounter;    // Modifacation: 2011.05
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(VBS_CFM, tmp));

    tmp = new CVerilogCounter;  // Modifacation: 2013.04
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(VERILOG, tmp));

    tmp = new CVHDLCounter; // Modifacation: 2013.04
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(VHDL, tmp));

    tmp = new CXmlCounter;  // Modifacation: 2011.10
    CounterForEachLanguage.insert(map<int, CCodeCounter*>::value_type(XML, tmp));

	return retVal;
}


/*!
* 1.Function Description:
*	 Resets all count lists (e.g., directive_count, data_name_count, etc.).
*
* 2.Parameters:
* Global: g_cc4enable   IN       controls if Cyclomatic Complexity 4 unique condition metric is enabled
*    eachLangCounters   IN/OUT   reference to struct holding pointers to Language specific parser classes
*
* 3.Creation Time And Owner:
*	 Version: 2011.05
*    Revised: 2015.12   to allow use by Threads and also allow setting values from Globals
*/
void ResetCounterCounts( CounterForEachLangType & eachLangCounters )
{
	CounterForEachLangType::iterator iter = eachLangCounters.begin();
	CounterForEachLangType::iterator end  = eachLangCounters.end();

	for ( ; iter != end; iter++ )
	{
		iter->second->InitializeCounts();

		// OK to use Globals here as only 1 thread at a time can call this
		extern	bool	g_cc4enable;

		iter->second->cc4enable = g_cc4enable;
	}
}


/*!
* 1.Function Description:
*    Sets the Physical line size Truncation limit for each Language type
*
* 2.Parameters:
*    eachLangCounters   IN/OUT   reference to struct holding pointers to Language specific parser classes
*
* 3.Creation Time And Owner:
*	 Version: 2013.04
*    Rewritten to allow use by Threads  2015.12
*/
void SetCounterOptions( CounterForEachLangType & eachLangCounters )
{
	if (lsloc_truncate != DEFAULT_TRUNCATE)
	{
		CounterForEachLangType::iterator iter = eachLangCounters.begin();
		CounterForEachLangType::iterator end  = eachLangCounters.end();
		for ( ; iter != end; iter++ )		
			iter->second->lsloc_truncate = lsloc_truncate;
	}
}


/*!
* 1.Function Description:
*	 Determines which counter can be used for a file.
*    Return value is language class type.
* SideEffects:	This may set print_cmplx flag in a pointed to Code counter class instance
*
* 2.Parameters:
*    CounterForEachLanguage	IN		structure holding Language counter class pointers
*    ppCounter				IN/OUT	pointer to Code counter class pointer (thread support)
*    print_cmplx			IN		true if doing complexity metrics
*    file_name				IN		file name
*    setCounter				IN		If true and print_cmplx is true then sets Language class instance print_cmplx true
*
* 3.Creation Time And Owner:
*	 Version: 2011.05
*    Revised: 2015.12  Support calling safely from multiple Threads, side effect noted
*/
ClassType DecideLanguage( CounterForEachLangType & CounterForEachLanguage,
						CCodeCounter** ppCounter,
						const bool print_cmplx,
						const string &file_name, 
						const bool setCounter )
{
	bool	found = false; // Modification: 2011.05
	CCodeCounter* pCounter = NULL; 

	// iterate all the counters to find the proper one.
	for (map<int, CCodeCounter*>::iterator iter = CounterForEachLanguage.begin(); iter != CounterForEachLanguage.end(); iter++)
	{
		if (iter->second->IsSupportedFileExtension(file_name))
		{
			found = true;
			pCounter = iter->second;
			if (setCounter && print_cmplx)
				pCounter->print_cmplx = true;    // Modification: 2013.04 // SIDE EFFECT: Set here and used by PrintXxx procedures later.
			*ppCounter = pCounter;
			break;
		}
	}
	// if it is not found, assign a default 'unknown' counter
	if (!found)		// Modification: 2011.10
	{
		pCounter = CounterForEachLanguage[UNKNOWN];
		*ppCounter = pCounter;
	}
	return pCounter->classtype;
}


/*!
* 1.Function Description:
*	 Retrieves the language name for a file.
*    Return language name
*
* 2.Parameters:
*    CounterForEachLanguage	IN		structure holding Language counter class pointers
*    class_type             IN      file classification type
*    file_name              IN      file name
*
* 3.Creation Time And Owner:
*	 Version: 2011.05
*    Revised: 2015.12   Safe to use by multiple threads, refactored to have single exit.
*/
string GetLanguageName( CounterForEachLangType & CounterForEachLanguage, 
						const ClassType class_type, const string &file_name ) 
{
	// WEB has many sub-types
	string classTypeFound = DEF_LANG_NAME;

	if ( ( class_type == WEB ) 
	  && ( file_name.length() > 0 ) )    // Modification: 2011.05
	{
		CWebCounter *webCounter = (CWebCounter*)CounterForEachLanguage[WEB];
		WebType webType = webCounter->GetWebType( file_name );		// Modification: 2011.10
		classTypeFound = webCounter->GetWebLangName( webType );		// Modification: 2011.10
	}
	else
	{
		if ( class_type < (int)CounterForEachLanguage.size() )   // Modification: 2013.04
			classTypeFound = CounterForEachLanguage[class_type]->language_name;
	}
		
	return classTypeFound;
}


/*!
* 1.Function Description:
*	 Updates count lists (e.g., directive_count, data_name_count, etc.) based on file counts.
*
* 2.Parameters:
*    eachLangCounters   IN/OUT   reference to struct holding pointers to Language specific parser classes
*    useListA           IN       use file list A? (otherwise use list B)
*    resetCounts        IN       true will also reset all counts
*
* 3.Creation Time And Owner:
*	 Version 2013.04
*    Revised 2015.12  to allow use by Threads
*/
void UpdateCounterCounts( CounterForEachLangType & eachLangCounters,
						SourceFileList * mySourceFile,
						const bool useListA, const bool resetCounts )
{
	int i;
	ClassType class_type;   // Modification: 2011.10
	WebType webType;
	SourceFileList::iterator its;
	StringVector::iterator icnts, icnte;
	UIntPairVector::iterator icntc;
	UIntVector::iterator icnt;
	CCodeCounter* codeCounter;
	CWebCounter* webCounter;    // Modification: 2011.10

	if ( resetCounts )                             // Modification: 2015.12
		ResetCounterCounts( eachLangCounters );    // Modification: 2011.05  2015.12

	size_t		counts_possible = 0;               // Modification: 2015.12

	for (its = mySourceFile->begin(); its != mySourceFile->end(); its++)
	{
		class_type = its->second.class_type;
		codeCounter = eachLangCounters[class_type];    // Modification: 2015.12
		for (i = 0; i < 12; i++)
		{
			switch (i)
			{
			case 0:
				icnts = codeCounter->directive.begin();
				icnte = codeCounter->directive.end();
				icntc = codeCounter->directive_count.begin();
				counts_possible = its->second.directive_count.size();
				icnt = its->second.directive_count.begin();
				break;
			case 1:
				icnts = codeCounter->data_name_list.begin();
				icnte = codeCounter->data_name_list.end();
				icntc = codeCounter->data_name_count.begin();
				counts_possible = its->second.data_name_count.size();
				icnt = its->second.data_name_count.begin();
				break;
			case 2:
				icnts = codeCounter->exec_name_list.begin();
				icnte = codeCounter->exec_name_list.end();
				icntc = codeCounter->exec_name_count.begin();
				counts_possible = its->second.exec_name_count.size();
				icnt = its->second.exec_name_count.begin();
				break;
			case 3:
				icnts = codeCounter->math_func_list.begin();
				icnte = codeCounter->math_func_list.end();
				icntc = codeCounter->math_func_count.begin();
				counts_possible = its->second.math_func_count.size();
				icnt = its->second.math_func_count.begin();
				break;
			case 4:
				icnts = codeCounter->trig_func_list.begin();
				icnte = codeCounter->trig_func_list.end();
				icntc = codeCounter->trig_func_count.begin();
				counts_possible = its->second.trig_func_count.size();
				icnt = its->second.trig_func_count.begin();
				break;
			case 5:
				icnts = codeCounter->log_func_list.begin();
				icnte = codeCounter->log_func_list.end();
				icntc = codeCounter->log_func_count.begin();
				counts_possible = its->second.log_func_count.size();
				icnt = its->second.log_func_count.begin();
				break;
			case 6:
				icnts = codeCounter->cmplx_calc_list.begin();
				icnte = codeCounter->cmplx_calc_list.end();
				icntc = codeCounter->cmplx_calc_count.begin();
				counts_possible = its->second.cmplx_calc_count.size();
				icnt = its->second.cmplx_calc_count.begin();
				break;
			case 7:
				icnts = codeCounter->cmplx_cond_list.begin();
				icnte = codeCounter->cmplx_cond_list.end();
				icntc = codeCounter->cmplx_cond_count.begin();
				counts_possible = its->second.cmplx_cond_count.size();
				icnt = its->second.cmplx_cond_count.begin();
				break;
			case 8:
				icnts = codeCounter->cmplx_logic_list.begin();
				icnte = codeCounter->cmplx_logic_list.end();
				icntc = codeCounter->cmplx_logic_count.begin();
				counts_possible = its->second.cmplx_logic_count.size();
				icnt = its->second.cmplx_logic_count.begin();
				break;
			case 9:
				icnts = codeCounter->cmplx_preproc_list.begin();
				icnte = codeCounter->cmplx_preproc_list.end();
				icntc = codeCounter->cmplx_preproc_count.begin();
				counts_possible = its->second.cmplx_preproc_count.size();
				icnt = its->second.cmplx_preproc_count.begin();
				break;
			case 10:
				icnts = codeCounter->cmplx_assign_list.begin();
				icnte = codeCounter->cmplx_assign_list.end();
				icntc = codeCounter->cmplx_assign_count.begin();
				counts_possible = its->second.cmplx_assign_count.size();
				icnt = its->second.cmplx_assign_count.begin();
				break;
            case 11:                                    // Modification: 2011.05
				icnts = codeCounter->cmplx_pointer_list.begin();
				icnte = codeCounter->cmplx_pointer_list.end();
				icntc = codeCounter->cmplx_pointer_count.begin();
				counts_possible = its->second.cmplx_pointer_count.size();
				icnt = its->second.cmplx_pointer_count.begin();
				break;
			}
			if ( counts_possible )
			{
				while (icnts != icnte)  // Modification: 2011.05
				{
					if (its->second.duplicate)
						(*icntc).second += (*icnt);
					else
						(*icntc).first += (*icnt);
					icnts++;
					icnt++;
					icntc++;
				}
			}
		}
		if (its->second.duplicate)
		{
			codeCounter->counted_dupFiles++;

            if (useListA)   // Modification: 2011.10
				codeCounter->total_dupFilesA++;
			else
				codeCounter->total_dupFilesB++;

			if (class_type == WEB)
			{
				// get web file class
				webCounter = (CWebCounter *)codeCounter;
				webType = webCounter->GetWebType(its->second.file_name);
				if (webType == WEB_PHP)
				{
					if (useListA)
						webCounter->total_php_dupFilesA++;
					else
						webCounter->total_php_dupFilesB++;
				}
				else if (webType == WEB_ASP)
				{
					if (useListA)
						webCounter->total_asp_dupFilesA++;
					else
						webCounter->total_asp_dupFilesB++;
				}
				else if (webType == WEB_JSP)
				{
					if (useListA)
						webCounter->total_jsp_dupFilesA++;
					else
						webCounter->total_jsp_dupFilesB++;
				}
				else if (webType == WEB_XML)
				{
					if (useListA)
						webCounter->total_xml_dupFilesA++;
					else
						webCounter->total_xml_dupFilesB++;
				}
				else if (webType == WEB_CFM)
				{
					if (useListA)
						webCounter->total_cfm_dupFilesA++;
					else
						webCounter->total_cfm_dupFilesB++;
				}
				else
				{
					if (useListA)
						webCounter->total_htm_dupFilesA++;
					else
						webCounter->total_htm_dupFilesB++;
				}
			}
		}
        else    // Modification: 2013.04
			codeCounter->counted_files++;
	}
}


/*!
* AddFromPairVector add counts to Main thread Counter instance vectors from Threads
* Local procedure
*
* \SideEffects	This will release RAM used for Source counters when done
*
* \param destVect reference to Destination base class vector instance holding counts
* \param srcVect  reference to   Source    base class vector instance holding counts
*/
void	AddFromPairVector( UIntPairVector & destVect, UIntPairVector & srcVect )
{
	unsigned int size = srcVect.size();
	for ( unsigned int k = 0; k < size; k++ )
	{
		destVect[ k ].first  += srcVect[ k ].first;
		destVect[ k ].second += srcVect[ k ].second;
	}

	srcVect.resize( 0 );
}


/*!
* AddFromACounter add counts to Main thread Counter instance from Threads
* Local procedure
*
* \param pDestCounter pointer to Destination base class Code counter
* \param pSrcCounter  pointer to   Source    base class Code counter
*/
void	AddFromACounter( CCodeCounter * pDestCounter, CCodeCounter * pSrcCounter )
{
	AddFromPairVector( pDestCounter->directive_count,     pSrcCounter->directive_count     );
	AddFromPairVector( pDestCounter->data_name_count,     pSrcCounter->data_name_count     );
	AddFromPairVector( pDestCounter->exec_name_count,     pSrcCounter->exec_name_count     );

	AddFromPairVector( pDestCounter->math_func_count,     pSrcCounter->math_func_count     );
	AddFromPairVector( pDestCounter->trig_func_count,     pSrcCounter->trig_func_count     );
	AddFromPairVector( pDestCounter->log_func_count,      pSrcCounter->log_func_count      );

	AddFromPairVector( pDestCounter->cmplx_calc_count,    pSrcCounter->cmplx_calc_count    );
	AddFromPairVector( pDestCounter->cmplx_cond_count,    pSrcCounter->cmplx_cond_count    );
	AddFromPairVector( pDestCounter->cmplx_logic_count,   pSrcCounter->cmplx_logic_count   );
	AddFromPairVector( pDestCounter->cmplx_preproc_count, pSrcCounter->cmplx_preproc_count );
	AddFromPairVector( pDestCounter->cmplx_assign_count,  pSrcCounter->cmplx_assign_count  );
	AddFromPairVector( pDestCounter->cmplx_pointer_count, pSrcCounter->cmplx_pointer_count );
}


/*!
* AddFromOtherLangCounters add counts to Main thread structure from Threads
*
* \SideEffects	This will clear Source counters to zero after accumulation
*
* \param	pDestLangCounters	IN/OUT	pointer to Language classes whose info needs updating
* \param	pSrcLangCounters	IN/OUT	pointer to Language classes that has new info
* \param	my_useListA			IN		List A counters (otherwise counters for List B)
*/
void AddFromOtherLangCounters( CounterForEachLangType * pDestLangCounters,
							CounterForEachLangType * pSrcLangCounters,
							const bool my_useListA )
{
	// Update main thread from each thread's CounterForEachLangType local data
	CounterForEachLangType::iterator itSrc     = pSrcLangCounters->begin();
	CounterForEachLangType::iterator itDest    = pDestLangCounters->begin();
	CounterForEachLangType::iterator itDestEnd = pDestLangCounters->end();

	CCodeCounter *	pSrcCodeCounter  = NULL;
	CCodeCounter *	pDestCodeCounter = NULL;
	CWebCounter *	webCounterSrc    = NULL;
	CWebCounter *	webCounterDest   = NULL;
	ClassType		classType        = WEB;

	bool	accumulate_from_web_counters = false;

	for ( ; itDest != itDestEnd; itDest++, itSrc++ )
	{
		pSrcCodeCounter  = (*itSrc).second;
		pDestCodeCounter = (*itDest).second;

		// It is possible not all Language classes in all Threads got called by DecideLanguage
		// So only set to true if Source was true else leave alone
		if ( pSrcCodeCounter->print_cmplx )
			pDestCodeCounter->print_cmplx = true;
		
		// Update values changed by the threaded code or Counters called from threaded code.
		// Update the Web classes values that are separate from CCodeCounter class
		classType = pDestCodeCounter->classtype;
		if (  WEB == classType )
		{
			// Add from a WEB counter which has private counts separate from Code counter base
			accumulate_from_web_counters = true;
			webCounterSrc  = (CWebCounter *)pSrcCodeCounter;
			webCounterDest = (CWebCounter *)pDestCodeCounter;
		}
		else
			accumulate_from_web_counters = false;

		if ( my_useListA )
		{
			pDestCodeCounter->total_filesA += pSrcCodeCounter->total_filesA;
			pSrcCodeCounter->total_filesA = 0;

			if ( accumulate_from_web_counters )
			{
				webCounterDest->total_php_filesA += webCounterSrc->total_php_filesA;
				webCounterSrc->total_php_filesA = 0;

				webCounterDest->total_asp_filesA += webCounterSrc->total_asp_filesA;
				webCounterSrc->total_asp_filesA = 0;

				webCounterDest->total_jsp_filesA += webCounterSrc->total_jsp_filesA;
				webCounterSrc->total_jsp_filesA = 0;

				webCounterDest->total_xml_filesA += webCounterSrc->total_xml_filesA;
				webCounterSrc->total_xml_filesA = 0;

				webCounterDest->total_cfm_filesA += webCounterSrc->total_cfm_filesA;
				webCounterSrc->total_cfm_filesA = 0;

				webCounterDest->total_htm_filesA += webCounterSrc->total_htm_filesA;
				webCounterSrc->total_htm_filesA = 0;
			}
		}
		else
		{
			pDestCodeCounter->total_filesB += pSrcCodeCounter->total_filesB;
			pSrcCodeCounter->total_filesB = 0;

			if ( accumulate_from_web_counters )
			{
				webCounterDest->total_php_filesB += webCounterSrc->total_php_filesB;
				webCounterSrc->total_php_filesB = 0;

				webCounterDest->total_asp_filesB += webCounterSrc->total_asp_filesB;
				webCounterSrc->total_asp_filesB = 0;

				webCounterDest->total_jsp_filesB += webCounterSrc->total_jsp_filesB;
				webCounterSrc->total_jsp_filesB = 0;

				webCounterDest->total_xml_filesB += webCounterSrc->total_xml_filesB;
				webCounterSrc->total_xml_filesB = 0;

				webCounterDest->total_cfm_filesB += webCounterSrc->total_cfm_filesB;
				webCounterSrc->total_cfm_filesB = 0;

				webCounterDest->total_htm_filesB += webCounterSrc->total_htm_filesB;
				webCounterSrc->total_htm_filesB = 0;
			}
		}
		AddFromACounter( pDestCodeCounter, pSrcCodeCounter );
	}
}


/*!
* 1.Function Description:
*	 Changes a given string to have a list of Language names and file Extensions (1 Language per line)
*
* 2.Parameters:
*    eachLangCounters   IN      reference to struct holding pointers to Language specific parser classes
*    langExtensions     IN/OUT  reference to string to hold Language names and file Extensions
*
* 3.Creation Time And Owner:
*	 Version 2015.12
*/
void GetLanguagesAndExtensions( CounterForEachLangType & eachLangCounters, string & langExtensions )
{
	CounterForEachLangType::iterator iter = eachLangCounters.begin();
	CounterForEachLangType::iterator end  = eachLangCounters.end();

	for ( ; iter != end; iter++ )
	{
		unsigned int	count_to_do = iter->second->file_extension.size();

		if ( iter->second->language_name == "UNDEF" )
		{
		#ifdef	_DEBUG
			if ( count_to_do )
			{
				// Special case the WEB
				if ( WEB == iter->second->classtype )
					langExtensions += "WEB= ";
			}
		#else
			// Don't show the User this for Relase builds as not changeable using -extfile
			continue;
		#endif
		}
		else
			langExtensions += iter->second->language_name + "= ";

		StringVector::iterator itExt = iter->second->file_extension.begin();
		for ( ; itExt != iter->second->file_extension.end(); itExt++ )
		{
			langExtensions += *itExt;
			count_to_do--;
			if ( count_to_do )
				langExtensions += ", ";
		}
		langExtensions += "\n";
	}
	return;
}


/*!
* 1.Function Description:
*	 Changes a given string to have a sorted list of File Extensions with related Language names (1 file Extension per line)
*
* 2.Parameters:
*    eachLangCounters    IN      reference to struct holding pointers to Language specific parser classes
*    extensionsAndLangs  IN/OUT  reference to string to hold file Extensions and related Language names
*
* 3.Creation Time And Owner:
*	 Version 2015.12
*/
void GetExtensionsAndLanguages( CounterForEachLangType & eachLangCounters, string & extensionsAndLangs, bool & multipleLangs )
{
	CounterForEachLangType::iterator iter = eachLangCounters.begin();
	CounterForEachLangType::iterator end  = eachLangCounters.end();

	StringVector extensions;
	string		oneLine;
	for ( ; iter != end; iter++ )
	{
		if ( iter->second->language_name == "UNDEF" )
			continue;	// skip 1st placeholder and Web as User can't change them

		StringVector::iterator itExt = iter->second->file_extension.begin();
		for ( ; itExt != iter->second->file_extension.end(); itExt++ )
		{
			oneLine = *itExt + " " + iter->second->language_name;
			extensions.push_back( oneLine );
		}
	}

	sort( extensions.begin(), extensions.end() );

	multipleLangs = false;
	string	temp, tempLang, previousExtension, previousLang;

	StringVector::iterator itExt = extensions.begin();
	for ( ; itExt != extensions.end(); itExt++ )
	{
		temp = *itExt;
		unsigned int position = temp.find_first_of(" ");
		if ( position == (unsigned int)string::npos )
		{
			// A really strange ERROR happened
			userIF->AddError( "Error: GetExtensionsAndLanguages created wrong string", false, 1, 1 );
			break;
		}
		tempLang = temp.substr( position );
		temp = temp.substr( 0, position );
		if ( previousExtension.compare( temp ) == 0 )
		{
			multipleLangs = true;	// MORE than 1 Language uses the same File Extension !
			char buf[512];
		#ifdef _MSC_VER
			// Use more Secure C library API
			sprintf_s( buf, sizeof( buf ),
		#else
			// Use older less Secure C library API
			sprintf( buf, 
		#endif
			"Warning: %s file extension is assigned to both %s and %s but only 1 language will be used.",
			temp.c_str(), previousLang.c_str(), tempLang.c_str() );
			string err = buf;
			userIF->AddError( err, false, 1, 1 );
		}
		previousExtension = temp;
		previousLang = tempLang;
		extensionsAndLangs += *itExt + "\n";
	}
	return;
}


// Helper to clean up 
void FreeLangsHeaders( vector<struct langTypeArrayHeader> * pLangs )
{
	// Release langs and array(s) of structs
	for ( unsigned int k = 0; k < pLangs->size(); k++ )
	{
		if ( NULL != (*pLangs)[k].pFilePtrs )
		{
			free( (void *)( (*pLangs)[k].pFilePtrs ) );
			(*pLangs)[k].pFilePtrs = NULL;
		}
		(*pLangs)[k].num_pointers = 0;
	}
	pLangs->resize( 0 );
}

// Sort helper used below
bool CompareByNum( const struct langTypeArrayHeader & a, const struct langTypeArrayHeader & b )
{
	// Largest is first
	return a.num_pointers > b.num_pointers;
}

// Get the Start positions used to speed up Duplicate checking
// Each different Language (class_type) will have 1 header struct
// Each header struct pointer points to array of SourceFileElement pointer and flags structs
// Each pointer and flags struct is about a single entry of SourceFileA or B list.
// Semantics of Duplicate checking are slightly changed from earlier versions of UCC.
// Now there is no way to have Duplicates whose Language classes are not the same.
// There are other cases where older UCC would have as Duplicate an XML and HTML if similar enough.
// Both are "WEB" class but semantics are not equal so now not possible to be Duplicates.
//
// Returns: Number of Language classes found 
//   or 0 if no files to do 
//   or -1 or -2 if a memory allocation error happened
int GetStartPositions( vector<struct langTypeArrayHeader> * pLangs, const bool useListA )
{
	int	retCount = 0;

	// Result structs
	pLangs->resize( 0 );

	// refer to A or B list
	SourceFileList & mySrcFile = SourceFileA;
	if ( false == useListA )
		mySrcFile = SourceFileB;

	unsigned int	size = (unsigned int)mySrcFile.size();
	if ( 0 == size )
	{
		return 0;
	}

	pLangs->resize( (unsigned int)(CLASS_TYPE_HIGHEST - CLASS_TYPE_LOWEST + 1) );

// Separate the various Languages (class types) into an array per Language with a header struct
//
	unsigned int	class_idx = 0;
	unsigned int	flag = 0;
	unsigned int	num_of_lang_classes = 0;
	bool			isEmbedded = false;

	// Use pointer semantics
	SourceFileList::iterator itSrc = mySrcFile.begin();
	for ( unsigned int idx = 0; idx < size; idx++, itSrc++ )
	{
		class_idx = (unsigned int)(*itSrc).second.class_type;
		
		// Embedded "files" are part of WEB.
		flag = 0;
		isEmbedded = (*itSrc).second.file_name_isEmbedded;
		if ( isEmbedded )
		{
			class_idx = (unsigned int)WEB;
			flag = IS_EMBEDDED;
		}

		// Allocate a C style array of structs if needed
		if ( NULL == (*pLangs)[ class_idx ].pFilePtrs )
		{
			// Allocate enough to hold rest of files including this one
			// Will get sized down before return
			(*pLangs)[ class_idx ].pFilePtrs = (filePtrFlags *)calloc( (size_t)(size - idx + 1), sizeof(struct filePtrFlags) );
			if ( NULL == (*pLangs)[ class_idx ].pFilePtrs )
			{
				userIF->AddError( "\nUnable to allocate memory to support Differencing", false );
				FreeLangsHeaders( pLangs );
				retCount = -1;
				break;
			}
			num_of_lang_classes++;
			(*pLangs)[ class_idx ].array_Lang = class_idx;
			(*pLangs)[ class_idx ].num_pointers = 0;
		}

		(*pLangs)[ class_idx ].pFilePtrs[ (*pLangs)[ class_idx ].num_pointers ].pSrcFile = &(*itSrc);
		(*pLangs)[ class_idx ].pFilePtrs[ (*pLangs)[ class_idx ].num_pointers ].flags = flag;
		(*pLangs)[ class_idx ].num_pointers++;
	}

	if ( 0 == retCount )
	{
		// Remove any entries for a Language of 1 or fewer pointers as no Duplicates are possible
		// Shrink size of memory to that actually used
		for ( unsigned int j = 0; j < pLangs->size(); j++ )
		{
			if ( NULL != (*pLangs)[ j ].pFilePtrs )
			{
				if ( (*pLangs)[ j ].num_pointers <= 1 )
				{
					// No Duplicates are possible
					free( (void *)( (*pLangs)[ j ].pFilePtrs ) );
					(*pLangs)[ j ].pFilePtrs = NULL;
					(*pLangs)[ j ].num_pointers = 0;
				}
				else
				{
					(*pLangs)[ j ].pFilePtrs = (struct filePtrFlags *)
										realloc( (void *)( (*pLangs)[ j ].pFilePtrs ), 
											( sizeof(struct filePtrFlags) * (size_t)((*pLangs)[ j ].num_pointers)) );
			
					// Check for unlikely realloc error when shrinking a block
					if ( NULL == (*pLangs)[ j ].pFilePtrs )
					{
						userIF->AddError( "\nUnable to reallocate memory to support Differencing", false );
						FreeLangsHeaders( pLangs );
						retCount = -2;
						break;
					}
				}
			}
			else
			{
				// Make sure structures are consistent
				(*pLangs)[ j ].num_pointers = 0;
			}
		}
	}

	if ( 0 == retCount )
	{
		retCount = (int)num_of_lang_classes;

		// Sort by num_pointers with Most pointers first
		sort( pLangs->begin(), pLangs->end(), CompareByNum );

		// Remove all at end after sort that have Zero number of pointers
		for ( unsigned int j = 0; j < pLangs->size(); j++ )
		{
			if ( 0 == (*pLangs)[ j ].num_pointers )
			{
				pLangs->resize( j );
				break;
			}
		}
	}

	return retCount;
}

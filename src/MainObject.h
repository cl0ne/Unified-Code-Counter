//! Main counting object class definition.
/*!
* \file MainObject.h
*
* This file contains the main counting object class definition.
*/

#ifndef MainObject_h
#define MainObject_h

// Get class defs for various Language parsers and support utils
#include "LangUtils.h"    // Modification: 2015.12

#include "UserIF.h"
#include "UCCThread.h"    // Modification: 2015.12
#include "CmpMngr.h"      // Modification: 2015.12

// Provide declaration
extern	bool	visualDiff;

//! Main counting object class.
/*!
* \class MainObject
*
* Defines the main counting object class.
*/
class MainObject
{
public:
	MainObject();
	~MainObject();

	// Must have a single point to check for User Cancelling so threads can be stopped if needed.
	bool HasUserCancelled();           // Modification: 2015.12

	// Helpers to support reporting times of various steps or when doing DIFF
	double GetDuplicateThreshold();        // Modification: 2015.12
	double GetMatchThreshold();            // Modification: 2015.12

	int MainProcess(int argc, char *argv[]);

	//string GetLanguageName(ClassType class_type, const string &file_name = "");

#ifdef	QTGUI
	void GetLanguageExtensionMap(multimap<const string, string> *lang_ext_map);
#endif

    void ShowUsage( const string &option = "", bool do_exit = true, string *outMsg = NULL );
#ifdef QTGUI
	void ConnectUserIF(QWidget *parent = 0);
#endif

protected:
	//! List of source file elements.
	/*!
	* \typedef SourceFileList
	*
	* Defines a list of source file elements.
	*/
	typedef list<SourceFileElement> SourceFileList;

	//! Structure to contain count totals.
	/*!
	* \struct TotalValue
	*
	* Defines a structure to contain count totals.
	*/
	struct TotalValue
	{
		TotalValue()
		{
			total_line = 0;
			blank_line = 0;
			whole_comment = 0;
			embed_comment = 0;
			phy_direct = 0;
			phy_decl = 0;
			phy_instr = 0;
			log_direct = 0;
			log_decl = 0;
			log_instr = 0;
			num_of_file = 0;
		}

		unsigned int total_line;					//!< Total lines
		unsigned int blank_line;					//!< Blank lines
		unsigned int whole_comment;					//!< Whole line comments
		unsigned int embed_comment;					//!< Embedded comments
		unsigned int phy_direct;					//!< Physical directive SLOC
		unsigned int phy_decl;						//!< Physical data declaration SLOC
		unsigned int phy_instr;						//!< Physical executable instruction SLOC
		unsigned int log_direct;					//!< Logical directive SLOC
		unsigned int log_decl;						//!< Logical data declaration SLOC
		unsigned int log_instr;						//!< Logical executable instruction SLOC
		unsigned int num_of_file;					//!< Number of files
	};

	//! Structure to contain web language count totals.
	/*!
	* \struct WebTotalValue
	*
	* Defines a structure to contain web language count totals.
	*/
	struct WebTotalValue
	{
		WebTotalValue()
		{
			total_line = 0;
			blank_line = 0;
			whole_comment = 0;
			embed_comment = 0;
			for (int i = 0; i < 6; i++)
			{
				phy_direct[i] = 0;
				phy_decl[i] = 0;
				phy_instr[i] = 0;
				log_direct[i] = 0;
				log_decl[i] = 0;
				log_instr[i] = 0;
			}
			num_of_file = 0;
		}

		unsigned int total_line;					//!< Total lines
		unsigned int blank_line;					//!< Blank lines
		unsigned int whole_comment;					//!< Whole line comments
		unsigned int embed_comment;					//!< Embedded comments
		unsigned int phy_direct[6];					//!< Physical directive SLOC
		unsigned int phy_decl[6];					//!< Physical data declaration SLOC
		unsigned int phy_instr[6];					//!< Physical executable instruction SLOC
		unsigned int log_direct[6];					//!< Logical directive SLOC
		unsigned int log_decl[6];					//!< Logical data declaration SLOC
		unsigned int log_instr[6];					//!< Logical executable instruction SLOC
		unsigned int num_of_file;					//!< Number of files
	};

	//! Structure to contain all web language count totals for unified output.
	/*!
	* \struct AllWebTotalValue
	*
	* Defines a structure to contain all web language count totals.
	* If new web language is added, must update the count value here
	*  0 - HTML    1 - XML    2 - JSclnt  3 - VBSclnt
	*  4 - C#clnt  5 - JSsrv  6 - VBSsrv  7 - C#srv
	*  8 - PHP	   9 - Java  10 - SQL    11 - CF      12 - CFS
	*/
	struct AllWebTotalValue
	{
		AllWebTotalValue()
		{
			total_line = 0;
			blank_line = 0;
			whole_comment = 0;
			embed_comment = 0;
			for (int i = 0; i < 13; i++)
			{
				phy_direct[i] = 0;
				phy_decl[i] = 0;
				phy_instr[i] = 0;
				log_direct[i] = 0;
				log_decl[i] = 0;
				log_instr[i] = 0;
			}
			num_of_file = 0;
		}

		unsigned int total_line;					//!< Total lines
		unsigned int blank_line;					//!< Blank lines
		unsigned int whole_comment;					//!< Whole line comments
		unsigned int embed_comment;					//!< Embedded comments
		unsigned int phy_direct[13];				//!< Physical directive SLOC
		unsigned int phy_decl[13];					//!< Physical data declaration SLOC
		unsigned int phy_instr[13];					//!< Physical executable instruction SLOC
		unsigned int log_direct[13];				//!< Logical directive SLOC
		unsigned int log_decl[13];					//!< Logical data declaration SLOC
		unsigned int log_instr[13];					//!< Logical executable instruction SLOC
		unsigned int num_of_file;					//!< Number of files
	};

	//! Map of count totals.
	/*!
	* \typedef TotalValueMap
	*
	* Defines a map of count totals.
	*/
	typedef map<int, TotalValue> TotalValueMap;

	//! Map of web language count totals.
	/*!
	* \typedef WebTotalValueMap
	*
	* Defines a map of web language count totals.
	*/
	typedef map<WebType, WebTotalValue> WebTotalValueMap;

	int ParseCommandLine(int argc, char *argv[]);
	void StartThreads( string & start_result_msg );    // Modified: 2015.12
	int ReadAllFiles(StringVector &inputFileVector, const string &inputFileList = INPUT_FILE_LIST_NAME, const bool useListA = true);

//    COMMENTED OUT  ReadFile as it was NOT called and similar code already part of 2015_10_07 changes
//	int ReadFile(SourceFileElement *sourceFile, string *errList = NULL, bool useListA = true);  2015_10_07

//	void UnloadFile(SourceFileElement *sourceFile);
//	void ProcessSourceList(bool useListA = true);
	bool IsSupportedFileExtension(const string &file_name);
	void FindDuplicateFiles(SourceFileList * fileList, StringVector * dupList1, StringVector * dupList2, 
							const bool checkMatch = false, const bool useListA = true );
//	bool FindDuplicateFor( CmpMngr * pDiffManager, struct filePtrFlags * pPtrFlags, 
//							const unsigned int numPtrs, const unsigned int i_idx, 
//							StringVector * dupList1, StringVector * dupList2, const bool checkMatch, const bool useListA );
//	void CompareForDuplicate(srcLineVector * firstFile, srcLineVector * secondFile, unsigned int &changedLines, unsigned int &totalLines, CmpMngr * pDiffManager );
	void ReadUserExtMapping(const string &extMapFile);
	void CreateExtMap();

	UserIF *userIF;									//!< User interface for presenting messages/progress to user

//      Moved to UCCGlobals.cpp .h    Modified: 2015.12
//	StringVector duplicateFilesInA1;				//!< List of duplicate file sources in baseline A
//	StringVector duplicateFilesInA2;				//!< List of duplicate files in baseline A
//	StringVector duplicateFilesInB1;				//!< List of duplicate file sources in baseline B
//	StringVector duplicateFilesInB2;				//!< List of duplicate files in baseline B
//
//	SourceFileList SourceFileA;						//!< List of source files in baseline A
//	SourceFileList SourceFileB;						//!< List of source files in baseline B

	string BaselineFileName1;                       //!< Baseline file name 1
    	string BaselineFileName2;                       //!< Baseline file name 2
    	string BaselineFilePath1;                       //!< Baseline file path 1
   	 string BaselineFilePath2;                       //!< Baseline file path 2

	string dirnameA;								//!< Directory name for baseline A
	string dirnameB;								//!< Directory name for baseline B
	string userExtMapFile;							//!< User extension map file

	StringVector listFilesToBeSearched;				//!< List of options for files to be searched
	size_t lsloc_truncate;							//!< # of characters allowed in LSLOC for differencing (0=no truncation)

	bool use_CommandLine;							//!< Read file names from command line specifications or from input files
	double match_threshold;							//!< % threshold for matching, if greater then added/deleted instead of modified

//  Moved these to UCCGlobals.h .cpp    Modified: 2015.12
//	bool print_cmplx;								//!< Print complexity and keyword counts
//	bool print_csv;									//!< Print CSV report files
//	bool print_ascii;								//!< Print ASCII text report files
//	bool print_legacy;								//!< Print legacy formatted ASCII text report files
//	bool print_unified;								//!< Print all counting files in a single unified file

	bool clearCaseFile;								//!< Target files are extracted from the ClearCase CM
	bool followSymLinks;							//!< Unix symbolic links are followed to their linked files/dirs
        bool doFuncDiff;                                //!< Enables function level differencing. Modified: 2017.02

	string	beginLangsExtens;						//!< Original list of Language names & related file extensions, 1 language per line

	map<int, CCodeCounter*> CounterForEachLanguage;	//!< List of code counters for each language
	map<string, string> LanguageExtensionMap;		//!< List of languages and their extensions

	CCodeCounter* counter;							//!< Single language code counter
};

#endif

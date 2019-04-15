//! Differencing tool class definition.
/*!
* \file DiffTool.h
*
* This file contains the differencing tool class definition.
*/

#ifndef DIFFTOOL_H
#define DIFFTOOL_H

#include "MainObject.h"
#include "CmpMngr.h"
#include "CmpMngrHtml.h"

#include "UCCBeforeLibraryIncludes.h"    // Modification: 2015.12
#include <algorithm>
#include <ctime>
#include "UCCAfterLibraryIncludes.h"     // Modification: 2015.12

// Differencing functionality defines
#define BASELINE_INF1 "fileListA.txt"
#define BASELINE_INF2 "fileListB.txt"
#define DIFF_OUTFILE "outfile_diff_results.txt"
#define DIFF_OUTFILE_CSV "outfile_diff_results.csv"
#define FUNC_DIFF_OUTFILE "outfile_func_diff_results.txt" //Modification: 2017.2
#define FUNC_DIFF_OUTFILE_CSV "outfile_func_diff_results.csv" //Modification: 2017.2
#define MATCH_PAIRS_OUTFILE "MatchedPairs.txt"
#define MATCH_PAIRS_OUTFILE_CSV "MatchedPairs.csv"
#define FILE_DUMP "diff_dump.txt"
#define HTML_DIFF "highlighted_diff.html"

#define MAX_MISMATCH_COST	10000

#define BAR_S	"-"

//! Differencing tool class.
/*!
* \class DiffTool
*
* Defines the differencing tool class.
*/
class DiffTool: public MainObject
{
public:
	DiffTool();
	~DiffTool();

	int diffToolProcess(int argc, char *argv[]);
        int funcDiffProcess(int argc, char *argv[]); //Modification: 2017.2

	//! Differencing results class.
	/*!
	* \class resultStruct
	*
	* Defines the differencing results class.
	*/
	class resultStruct
	{
	public:
		unsigned int addedLines;			//!< Lines added
		unsigned int deletedLines;			//!< Lines deleted
		unsigned int modifiedLines;			//!< Lines modified
		unsigned int unmodifiedLines;		//!< Lines unmodified

		resultStruct() { addedLines = deletedLines = modifiedLines = unmodifiedLines = 0; }
	};

	//! Pair of matching source files.
	/*!
	* \typedef MatchedFilePair
	*
	* Defines a pair of matching source files.
	*/
	typedef pair<SourceFileElement *, SourceFileElement *> MatchedFilePair;

	//! Vector of matching file pairs and results.
	/*!
	* \typedef MatchingType
	*
	* Defines a vector of matching file pairs and results.
	*/
	
	typedef vector<pair<resultStruct, MatchedFilePair> > MatchingType;

	MatchingType matchedFilesList;		//!< List of matching file pairs and differencing results

	unsigned int total_addedLines;		//!< Total lines added
	unsigned int total_deletedLines;	//!< Total lines deleted
	unsigned int total_modifiedLines;	//!< Total lines modified
	unsigned int total_unmodifiedLines;	//!< Total lines unmodified
	unsigned int dup_addedLines;		//!< Total duplicate lines added
	unsigned int dup_deletedLines;		//!< Total duplicate lines deleted
	unsigned int dup_modifiedLines;		//!< Total duplicate lines modified
	unsigned int dup_unmodifiedLines;	//!< Total duplicate lines unmodified
	bool printDup;						//!< Print duplicates (only true if unmatched duplicates exist)
	ofstream outfile_file_dump;         //!< Differencing results text file stream for paired lines info

private:
	int ReadAllDiffFiles();
	void MatchBaseLines( const string commonPathBetweenBaselines, const bool webSepFilesOnly = false );
	unsigned int CompareFilePaths( const string &path1, const string &path2 );
	void ProcessDiffSourceLists();
    void ProcessDiffResults();
	void ProcessPairs();
	void PrintMatchedPairs();
	void PrintDiffResults();
        void PrintFuncDiffResults();  //Modification: 2017.2
        void printFuncDiffResultsHeader();  //Modification: 2017.2

	//! Pair of file name to source file element
	/*!
	* \typedef FileNamePair
	*
	* Defines a pair for file name and source file element.  Used for matching files
	*/
	typedef pair<string, SourceFileElement *> FileNamePair;

	//! Structure to contain file element preference value.
	/*!
	* \struct PreferenceStruct
	*
	* Defines a structure to contain file element preference value.
	*/
	struct PreferenceStruct
	{
		SourceFileElement *fileElement;	//!< File element
		int value;						//!< File preference value
	};

	//! Structure to sort the preference vector in descending order.
	/*!
	* \struct CustomCMP
	*
	* Defines a structure to sort the preference vector in descending order.
	*/
	struct CustomCMP
	{
		bool operator() (const PreferenceStruct &a, const PreferenceStruct &b) const
		{
			return a.value < b.value;
        }
	};

	//! Vector of file preferences.
	/*!
	* \typedef MatchingType
	*
	* Defines a vector of file preferences.
	*/
	typedef vector<PreferenceStruct> PreferenceMapType;

	//! Map of file elements and preferences.
	/*!
	* \typedef BaselinePreferenceMapType
	*
	* Defines a map of file elements and preferences.
	*/
	typedef map<SourceFileElement *, PreferenceMapType *> BaselinePreferenceMapType;

	//! Map of file element pairs.
	/*!
	* \typedef BaselineFileMapType
	*
	* Defines a map of file element pairs.
	*/
	typedef map<SourceFileElement *, SourceFileElement *> BaselineFileMapType;

	//! Multimap of file elements, sorted by file name
	/*!
	* \typedef SortedPreferenceMapType
	*
	* Defines a multimap of file elements and names
	*/
	typedef multimap<string, SourceFileElement*> SortedPreferenceMapType;

	ofstream outfile_diff_results;		//!< Differencing results text file stream
	ofstream outfile_diff_csv;			//!< Differencing results CSV file stream
	ofstream dup_outfile_diff_results;  //!< Differencing results text file stream for duplicates
	ofstream dup_outfile_diff_csv;		//!< Differencing results CSV file stream for duplicates
//    ofstream outfile_file_dump;         //!< Differencing results text file stream for paired lines info
    ifstream infile_file_dump;          //!< instream for dump_file eval. fstream is included in CmpMngrHtml.h
    ofstream html_diff_stream;          //!< HTML file stram for highlighted diff output

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'DiffTool' : copy constructor could not be generated because a base class copy constructor is inaccessible
	DiffTool(const DiffTool& rhs);

	// Take care of warning C4626: 'DiffTool' : assignment operator could not be generated because a base class assignment operator is inaccessible
	DiffTool operator=(const DiffTool);
};

#endif

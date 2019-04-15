//! Differencing tool class methods.
/*!
* \file DiffTool.cpp
*
* This file contains the differencing tool class methods.
*/

#include "UCCGlobals.h"      // Modification: 2015.12
#include "DiffTool.h"
#include "CUtil.h"
#include "UCCFilesOut.h"     // Modification: 2015.12
#include "FunctionParser.h"  //Modification: 2017.2

using namespace std;

extern	UserIF *	userIF;			//!< User interface for presenting messages/progress to user   Modification: 2015.12
extern	bool		print_cmplx;	//!< Print complexity and keyword counts                       Modification: 2015.12
extern	bool		print_csv;		//!< Print CSV report files                                    Modification: 2015.12


#ifdef	ENABLE_THREADS
// Helper to have Threads process Diff Pairs in parallel
extern 
int DiffPairsThreads( DiffTool				*	pDiff,
					UserIF					*	userIF,
					const	bool				print_cmplx,
					const	bool				print_csv,
					const	bool				clearCaseFile,
					string						outDir,
					const	bool				discard_PHY_lines_after_process,
					const	bool				discard_lines_after_process,
					const	bool				no_warnings_to_UI );
#endif

//	Do a list of Matched Pairs to Diff (run from Main or worker thread)
int DiffPairsInList( const unsigned int					threadIdx,
					const		double					match_threshold,
					UserIF					*			userIF,
					bool						&		printDup,
					CounterForEachLangType		&		CounterForEachLanguage,
					const		bool					print_cmplx,
					const		bool					print_csv,
					DiffTool::MatchingType::iterator	myI,		// matchedFilesList, matching file pairs and differencing results 
					DiffTool::MatchingType::iterator	itEnd,
					const		bool					clearCaseFile,
					SourceFileList			*			mySourceFileA,
					SourceFileList			*			mySourceFileB,
					string						&		myErrList,
					string								outDir,
					ErrMsgStructVector			&		err_msgs,
					UncountedFileStructVector	&		unc_files,
					const		bool					discard_PHY_lines_after_process,
					const		bool					discard_lines_after_process,
					const		bool					no_warnings_to_UI,
					unsigned int				&		dup_addedLines,
					unsigned int				&		dup_deletedLines,
					unsigned int				&		dup_modifiedLines,
					unsigned int				&		dup_unmodifiedLines,
					unsigned int				&		total_addedLines,
					unsigned int				&		total_deletedLines,
					unsigned int				&		total_modifiedLines,
					unsigned int				&		total_unmodifiedLines );


// Allow access to instance of DiffTool class for Thread setup/combine results
DiffTool	*	pDiffTool = NULL;


/*
* 1. Function Description: 
*    Constructs a DiffTool object.
*	 
* 2. Parameters:
*
* 3. Creation Time and Owner: 
*	 Version 2007.07
*/
DiffTool::DiffTool()
{
	// this is the summary count information
	isDiff = true;

        //Modification 2017.2
        doFuncDiff = false;

	// Modification: 2007.07
	total_addedLines = total_deletedLines = total_modifiedLines = total_unmodifiedLines = 0;
	dup_addedLines = dup_deletedLines = dup_modifiedLines = dup_unmodifiedLines = 0;
	
	// Modification: 2011.05
	printDup = false;

	pDiffTool = this;	// Modification: 2015.12
}


// Destructor  2015.12
DiffTool::~DiffTool()
{
	// Release RAM used by matchedFilesList
	matchedFilesList.resize( 0 );

	pDiffTool = NULL;
}


/*
* 1. Function Description: 
*    DiffTool process to replace MainProcess when differencing.
*    Return function status
*
* 2. Parameters: 
*    argc： number of arguments
*    argv： argument list
*
* 3. Creation Time and Owner: 
*	 Version 2011.05
*    Revised 2015.12  Refactored to: use Threads, better show % done in UI, capture Time used
*/
int DiffTool::diffToolProcess(int argc, char *argv[])
{
/*	Code is done here to best support DIFF of large file sets.

Major features most affecting Memory and Speed performance here:
	Diff:  Any use of Diff will run code here.
	Diff + Duplicate checking:  Here and Dup code in MainObject.
		Because Dup checks can be done for a single Baseline,
		Dup code here for 2 Baselines is set to save memory but will likely
			NOT perform as well (speed, maybe Memory use) as a single Baseline.
Priorities:
	0)	Favor DIFF RAM use/speed over DUP for reasons above.
	1)	Limit the MAXIMUM RAM Memory In Use as much as easily done:
	So loading all the file buffers into RAM first is not the best way.
	Compared to earlier versions of UCC, the overall strategy is to delay
	loading RAM buffers for the source lines of any file until details of
	those source lines are really needed.  Release the buffer contents as
	soon as possible is also done.  This greatly helps DIFF to use less
	RAM at any given time but there is a likely performance hit for Dup as
	some files may need to be loaded into RAM buffers again.

	2)	Allow use of Threads to speed up processing where feasible.
	Especially when a List of some work needs to be done, the List can be
	split between worker threads (assuming working on items or part of the
	List can be independant from other items or parts).  Allow the Threads
	to do as much follow on processing as feasible.

	DIFF implementation characteristics:
	Because Dup potentially also goes through ALL of a Baseline's files:
		Dup processing here is done AFTER DIFF is finished and all
		DIFF related reports are done.  IF UCC runs into a LOW Memory 
		condition (which is more likely now with Dup) THEN at least all
		the DIFF results have finished and been saved.

Differencing Process (Reduce RAM memory use, respectable speed):
1.  Parse options, User sets -threads 2 or more
2.  Create worker Threads if 2 or more wanted.  Threads start as blocked.
3.  Get info of files to work on (build 2 Baseline file lists: A and B)
		RAM buffers for any file's source lines are NOT loaded now.
3.1)     File list is vector of strings of each file path/name
			IF not running worker Threads do steps below from Main thread.
3.2)     File list for A sent to Threads to fill in UCC list A structs.
3.3)     File list for B sent to Threads to fill in UCC list B structs.
4.  Match file pairs; 1 from A, 1 from B (put in Matched Pairs List).
		First time skips Web files, second time does Web files.
	(Typical project characteristics):
		Over  50% is Matching that only needs file Paths and Names.
		Below 50% is UnMatched and requires source lines from each file.
		User can examime the MatchedPairs.txt or .csv file to find their
		actual UnMatched %.  2 examples were 33% and 4.5% where 33% was
		was years between Baselines and therefore more changes included.
		Overall RAM memory Savings = 100% - UnMatched % (worst case)
		IF the project has an extremely scrambled file path structure
			where no pairs of files to DIFF can be found easily,
			THEN the Overall Savings would drop.
5.  Output the MatchedPairs.txt (or .csv) file.
6.  Repeat step 4 one time doing Web separation files.
7.  Send Matched Pairs List to be split and done by worker Threads in parallel.
8.  DO: for a Matched Pair in list (1/N part of entire list if N Threads)
		Read the source lines into a RAM buffer for the 2 files.
		Analyze each file's source lines: Count Keywords, Cyclomatic Complexity ...
		DIFF between the 2 files going line by line as needed (SLOC).
			Save DIFF results: in file structs, Visual DIFF, ...
		Release the RAM source line buffers of the 2 files.
	WHILE: there are more Matched Pairs loop back to DO
9.  Threads finish each part of entire list.  Threads block for next command.

ALL the steps below are single threaded running on Main thread.
10. Destroy worker threads if created.
		Main thread gathers worker threads' results.
		Thread destruction Releases some RAM.
			Helps later Duplicate checking (if wanted).
				(Duplicate checks here Not recommended for large file sets)
11. Output all DIFF specific results.
		IF no DUP checking 
			THEN output all other results.
			DONE !

Duplicate checks if wanted: (not recommended here as better without DIFF)
	Duplicate checking: Exhaustive search starting at first file in list 
		check forward in list to last file using duplicate Threshold.
	Because of the Exhaustive search and use of data structures when searching,
	partitioning Duplicate checking to run on 2 or more threads is quite messy.
	There is likely a way to thread partition but not yet found/implemented.

12. Identify Duplicate files within Baseline A.
		Release RAM buffers in Baseline A when not being used.
			This may lead to poor processing speed performance as some
			files may be loaded more than 1 time (Exhaustive search).
13. Output Baseline A Duplicate results.
14. Identify Duplicate files within Baseline B.
		Release RAM buffers as above.
15. Output Baseline B Duplicate results.
16. Output all counting results data
17. Output all remaining results data

The above steps were reordered to:
	1) Limit total RAM in use and 2) run on Threads.
Output to files especially should be done single threaded from Main thread.
*/

	// create a user interface object
	// Modification: 2013.04
	if (userIF == NULL)
		userIF = new UserIF();

	// handle input file lists
	BaselineFileName1 = BASELINE_INF1;
	BaselineFileName2 = BASELINE_INF2;

	// parse the command line input
	// Modification: 2011.10
	if (!ParseCommandLine(argc, argv))
		ShowUsage();    // this will Exit

	SetCounterOptions( CounterForEachLanguage );    // Modification: 2015.12

// add output directory if using QTGUI
// Modification: 2013.04
#ifdef QTGUI
	if (outDir != "")
	{
		BaselineFileName1 = outDir + BASELINE_INF1;
		BaselineFileName2 = outDir + BASELINE_INF2;
	}
#endif
	if ( HasUserCancelled() )
		return 0;

	// Enable Optimizations.  Start threads if wanted.    Modified: 2015.12
	// Threads must be started (if wanted) before file Extension maps are done.
	string	start_threads_result_msg;
	StartThreads( start_threads_result_msg );
	if ( start_threads_result_msg.size() )
		userIF->updateProgress( start_threads_result_msg, false );

	// generate user-defined language extension map
	// Modification: 2011.05
	if (userExtMapFile.length() != 0)
		ReadUserExtMapping(userExtMapFile);

	if ( HasUserCancelled() )
		return 0;
	
	// Get file details List for DIFF use.  Only Web file source lines are in RAM buffers here.
	// Modification: 2011.05
	if ( !ReadAllDiffFiles() ) // Set time_end_list_built & Shows 2 msgs, 1 per Baseline  Modification: 2015.12
		return 0;

	if ( HasUserCancelled() )
		return 0;

	// match files in BaselineA to BaselineB (does not include web separation files)
	// Modification: 2013.04
	userIF->updateProgress("Performing files matching.........................", false);    // Modification: 2015.12

	// Modification: 2011.05
	MatchBaseLines( commonPathPrefixBoth );		// Extra info for faster Path matching    Modification: 2015.12

	// Include this small time as part of Match Baselines	0 to 1 second (rounded)
	PrintMatchedPairs();	// Print time included as part of Match Baselines

	time( &time_end_match_baselines );    // Modification: 2015.12

	// Web files were buffered in RAM when list of File details was built.
	// So below is NOT needed and actually can cause a memory access fault (Seg Fault on Linux)
	// In some cases if number of HTML files is not same in both baselines.
/*
	if (CounterForEachLanguage[WEB]->total_filesA > 0 ||
		CounterForEachLanguage[WEB]->total_filesB > 0)
	{
		// perform matching on web separation files
		//userIF->updateProgress("Performing Web files matching.....................", false);    // Modification: 2015.12
		//MatchBaseLines( commonPathPrefixBoth, true );

		// Modification: 2013.04
	#ifndef QTGUI
		//userIF->updateProgress("DONE");    // Modification: 2015.12
	#endif

		time( &time_end_match_baselines_web );    // Modification: 2015.12
	}
	else
		time_end_match_baselines_web = time_end_match_baselines;    // Modification: 2015.12
*/

	if ( HasUserCancelled() )    // Modification: 2013.04 2015.12
		return 0;

	// compare the matched files in BaselineA to BaselineB
	// perform matching on web separation files Modification: these do not exist when initially matched
	// Modification: 2013.04
	userIF->updateProgress("Performing files comparison.......................", false);

	// Read/Analyze/Count keywords for a pair of files then Diff between
	ProcessPairs();		// Recommended to Run on worker Threads
	
	// Modification: 2013.04
//#ifndef QTGUI
//	userIF->updateProgress("\b\b\b\bDONE");    // Modification: 2015.12
//#endif

	// print the comparison results
	userIF->updateProgress("Saving Diff results to files..................", false);

	// Modification: 2011.05
	PrintDiffResults();

	// Release Matched Files list as no longer needed
	matchedFilesList.resize( 0 );

	// Include small time to save Diff results files
	time( &time_end_process_pairs );        // Modification: 2015.12

	// Modification: 2013.04
#ifndef QTGUI
	userIF->updateProgress("DONE");         // Modification: 2015.12
#endif

	if ( HasUserCancelled() )
		return 0;

	if (duplicate_threshold >= 0.0)         // Modification: 2015.12
	{
		time( &time_end_process_pairs );    // Modification: 2015.12

		// This will show % done
		userIF->updateProgress("Looking for duplicate files in Baseline-A ........", false);
		FindDuplicateFiles( &SourceFileA, &duplicateFilesInA1, &duplicateFilesInA2, true, true );    // Modification: 2011.05

		if ( HasUserCancelled() )    // Modification: 2013.04 2015.12
			return 0;

		// Include this small time as part of A Duplicates  0 to 1 second (rounded)
		UpdateCounterCounts( CounterForEachLanguage, &SourceFileA, true, true );    // Modification: 2015.12
		time( &time_end_find_duplicates );
	}
	else         // Modification: 2015.12
	{
		// Include this small time as part of Process Pairs  0 to 1 second (rounded)
		UpdateCounterCounts( CounterForEachLanguage, &SourceFileA, true, true );    // Modification: 2015.12
		time( &time_end_process_pairs );
	}            // Modification: 2015.12

// print the counting results for Baseline A (SLOC counts and complexity counts)
	if (print_unified)
		PrintTotalCountResults( CounterForEachLanguage, true, "Baseline-A-", &duplicateFilesInA2 );    // Modification: 2015.12
	else
		PrintCountResults( CounterForEachLanguage, true, "Baseline-A-", &duplicateFilesInA2 );    // Modification: 2015.12

	if (print_cmplx)
		PrintComplexityResults( CounterForEachLanguage, true, "Baseline-A-" );    // Modification: 2015.12

	if (duplicate_threshold >= 0.0)
	{
		if (print_unified)
			PrintTotalCountResults( CounterForEachLanguage, true, "Duplicates-A-", &duplicateFilesInA2, false );    // Modification: 2015.12
		else
			PrintCountResults( CounterForEachLanguage, true, "Duplicates-A-", &duplicateFilesInA2, false );    // Modification: 2015.12

		if (print_cmplx)
			PrintComplexityResults( CounterForEachLanguage, true, "Duplicates-A-", true );    // Modification: 2015.12

		PrintDuplicateSummary(true, "Duplicates-A-");
	}

	if ( duplicate_threshold >= 0.0 )    // Modification: 2015.12
	{
		time( &time_end_print_results );    // Modification: 2015.12

		// This will show % done
		userIF->updateProgress("Looking for duplicate files in Baseline-B ........", false);
		FindDuplicateFiles( &SourceFileB, &duplicateFilesInB1, &duplicateFilesInB2, true, false );    // Modification: 2011.05

		if ( HasUserCancelled() )    // Modification: 2013.04 2015.12
			return 0;

		// Include this small time as part of B Duplicates	0 to 1 second (rounded)
		UpdateCounterCounts( CounterForEachLanguage, &SourceFileB, false, true );
		time( &time_end_find_duplicatesB );    // Modification: 2015.12
	}
	else      // Modification: 2015.12
	{
		// Include this small time as part of print results A	0 to 1 second (rounded)
		UpdateCounterCounts( CounterForEachLanguage, &SourceFileB, false, true );
		time( &time_end_print_results );
	}        // Modification: 2015.12

// print the counting results for Baseline B (SLOC counts and complexity counts)
	if (print_unified)
		PrintTotalCountResults( CounterForEachLanguage, false, "Baseline-B-", &duplicateFilesInB2 );    // Modification: 2015.12
	else
		PrintCountResults( CounterForEachLanguage, false, "Baseline-B-", &duplicateFilesInB2 );    // Modification: 2015.12

	if (print_cmplx)
		PrintComplexityResults( CounterForEachLanguage, false, "Baseline-B-" );    // Modification: 2015.12

	if (duplicate_threshold >= 0.0)
	{
		if (print_unified)
			PrintTotalCountResults( CounterForEachLanguage, false, "Duplicates-B-", &duplicateFilesInB2, false );    // Modification: 2015.12
		else
			PrintCountResults( CounterForEachLanguage, false, "Duplicates-B-", &duplicateFilesInB2, false );    // Modification: 2015.12

		if (print_cmplx)
			PrintComplexityResults( CounterForEachLanguage, false, "Duplicates-B-", true );    // Modification: 2015.12

		PrintDuplicateSummary(false, "Duplicates-B-");
	}

	time( &time_end_print_resultsB );    // Modification: 2015.12

	return 1;
}

/*
* 1. Function Description:
*    Performs function level differencing
*
* 2. Parameters:
*    argc： number of arguments
*    argv： argument list
*
* 3. Creation Time and Owner:
*        Version 2017.2
*/
int DiffTool::funcDiffProcess(int argc, char *argv[])
{
    doFuncDiff = true;

    if (userIF == NULL)
        userIF = new UserIF();

    BaselineFileName1 = BASELINE_INF1;
    BaselineFileName2 = BASELINE_INF2;

    if (!ParseCommandLine(argc, argv))
        ShowUsage();

    SetCounterOptions(CounterForEachLanguage);
#ifdef QTGUI
    if (outDir != "")
        {
                BaselineFileName1 = outDir + BASELINE_INF1;
                BaselineFileName2 = outDir + BASELINE_INF2;
        }
#endif

    if (HasUserCancelled())
        return 0;

    string      start_threads_result_msg;
    StartThreads(start_threads_result_msg);
    if (start_threads_result_msg.size())
        userIF->updateProgress(start_threads_result_msg, false);

    if (HasUserCancelled())
        return 0;

    if (!ReadAllDiffFiles())
        return 0;

    if (HasUserCancelled())
        return 0;

    MatchBaseLines(commonPathPrefixBoth);
        userIF->updateProgress("Performing function level differencing........", false);

    //Modified: 2017.02 
    //If it is commond line with -dir option, create temp directory in dirnameA and dirnameB
    //Else, create temp directory in current work directory
    string tempDirA;
        string tempDirB;

    if(use_CommandLine) {
        tempDirA = dirnameA;
        tempDirB = dirnameB;
        }
        else {
                if(CUtil::GetPath(tempDirA)==0)
                {
                        string err = "Unable to get current work directory";
            userIF->SetErrorFile("");
            userIF->AddError(err);
            return 0;
                }
                if(CUtil::GetPath(tempDirB)==0)
                {
                        string err = "Unable to get current work directory";
            userIF->SetErrorFile("");
            userIF->AddError(err);
            return 0;
                }
        }

    MatchingType tempMatchedFileList(matchedFilesList);
    string fileA, fileB;
    string tempPathA,tempPathB;
    list<pair<pair<string, string> , ClassType> > listOfPairs;

    printFuncDiffResultsHeader();

    for (MatchingType::iterator myIt = tempMatchedFileList.begin(); myIt != tempMatchedFileList.end(); myIt++)
    {
        if ((*myIt).second.first == NULL)
        {
            fileA = "NA";
        }
        else
        {
            if ((*myIt).second.first->second.file_name_isEmbedded)
                continue;

            fileA = (*myIt).second.first->second.file_name;
        }

        if ((*myIt).second.second == NULL)
        {
            fileB = "NA";
        }
        else
        {
            if ((*myIt).second.second->second.file_name_isEmbedded)
                continue;

            fileB = (*myIt).second.second->second.file_name;
        }

        if (fileA.compare("NA")!=0)
        {
                listOfPairs.push_back(make_pair(make_pair(fileA, fileB), (*myIt).second.first->second.class_type));
        }
        else if (fileB.compare("NA")!=0)
        {
                listOfPairs.push_back(make_pair(make_pair(fileA, fileB), (*myIt).second.second->second.class_type));
        }
    }

    for(list<pair<pair<string, string> , ClassType> >::iterator myIt = listOfPairs.begin(); myIt != listOfPairs.end(); myIt++)
    {
        fileA = (*myIt).first.first;
        fileB = (*myIt).first.second;

        tempPathA = tempDirA+"/tempA";
        tempPathB = tempDirB+"/tempB";

        if (CUtil::MkPath(tempPathA) == 0)
        {
            string err = "Unable to create temporary output directory (";
            err += tempPathA;
            err += ")";
            userIF->SetErrorFile("");
            userIF->AddError(err);
            return 0;
        }
        if (CUtil::MkPath(tempPathB) == 0)
        {
            string err = "Unable to create temporary output directory (";
            err += tempPathB;
            err += ")";
            userIF->SetErrorFile("");
            userIF->AddError(err);
            return 0;
        }

        ClassType classTypeOfFile = (*myIt).second;
        FunctionParser functionParser;

        if (fileA.compare("NA")!=0)
        {
                functionParser.callParser(fileA, tempPathA, classTypeOfFile);
        }
        if (fileB.compare("NA")!=0)
        {
                classTypeOfFile = (*myIt).second;
                functionParser.callParser(fileB, tempPathB, classTypeOfFile);
        }

        dirnameA = tempPathA;
        dirnameB = tempPathB;

        total_addedLines = total_deletedLines = total_modifiedLines = total_unmodifiedLines = 0;

        matchedFilesList.resize(0);
        SourceFileA.resize(0);
        SourceFileB.resize(0);
        ReadAllDiffFiles();
        MatchBaseLines(commonPathPrefixBoth);
        ProcessPairs();
        if (print_ascii || print_legacy)
        {
            outfile_diff_results << "File Name A: ";
            outfile_diff_results << fileA << "  ";
            outfile_diff_results.width(3);
            outfile_diff_results << "| ";
            outfile_diff_results << "File Name B: ";
            outfile_diff_results << fileB;
            outfile_diff_results.width(3);
            outfile_diff_results << "| " << endl;

        }
        if(print_csv)
        {
            outfile_diff_csv << endl;
            outfile_diff_csv << "File Name A:" << "," << fileA << ",";
            outfile_diff_csv << "File Name B:" << "," << fileB << endl;
        }

        PrintFuncDiffResults();

        if (CUtil::RmPath(tempPathA)==0)
        {
                string err = "Unable to delete directory (";
                err += tempPathA;
                err += ")";
                userIF->SetErrorFile("");
                userIF->AddError(err);
                return 0;
        }
        if (CUtil::RmPath(tempPathB)==0)
        {
                string err = "Unable to delete directory (";
                err += tempPathB;
                err += ")";
                userIF->SetErrorFile("");
                userIF->AddError(err);
                return 0;
        }

        dirnameA = tempDirA;
        dirnameB = tempDirB;

        fileA = fileB = "";
    }

    //Close function level differencing output file
    if (print_ascii || print_legacy)
        outfile_diff_results.close();

    if (print_csv)
        outfile_diff_csv.close();

    //close dump file stream, otherwise infile_file_dump will be buggy.
    outfile_file_dump.close();

#ifndef QTGUI
    userIF->updateProgress("DONE");
#endif

    return 1;
}

/*
* 1. Function Description: 
*    Diff needs some file details for both baselines.
*      Reading physical lines into RAM buffers (and Analysis/Counting) happens later.
*    Return method status of 1 if OK else 0
*
* 2. Parameters:
*      various DiffTool class data members
*
*    Design:
*      UI messages about Reading files are no longer needed here.
*        See comments at start of diffToolProcess.
*
* 3. Creation Time and Owner: 
*	 Version 2011.05
*    Revised 2015.12  DIFF uses less RAM. Updates UI while getting file details.
*/
int DiffTool::ReadAllDiffFiles()
{
	int		retVal = 1;						// Modified: 2015.12

	// Make a list of input source files
        if(!doFuncDiff) //Modified: 2017.2
                userIF->updateProgress( "Building list of source files...\n", false );    // Modified: 2015.12

	// flag to indicate if the files are to be read from fileList<A|B>.txt
	// or from the parameters mentioned at command line
	// Modification: 2011.05
	vector<string> listFileNamesA;
	vector<string> listFileNamesB;
	if (use_CommandLine)
	{
		// Modification: 2011.10
		int listAllResult =
		CUtil::ListAllFiles( dirnameA, listFilesToBeSearched, listFileNamesA, followSymLinks,
							totalFileSizesA, largestFileSizeA, numFilesInA, commonPathPrefixA );

		if ( listAllResult >= 0 )
		{
//		userIF->updateProgress( ".......", false );			// Modified: 2015.12

		CUtil::ListAllFiles( dirnameB, listFilesToBeSearched, listFileNamesB, followSymLinks,
							totalFileSizesB, largestFileSizeB, numFilesInB, commonPathPrefixB );
		
		// Modification: 2011.05
		BaselineFileName1 = BaselineFileName2 = "";
		}
	}

//	userIF->updateProgress( ".......DONE\n", false );    // Modified: 2015.12

// Modified: 2015.12
	time( &time_end_list_built );

	if ( HasUserCancelled() )
		return 0;

	// Get information about source files for each baseline, build SourceFileA list.
	// Reading physical lines into RAM buffers is later.
	// Duplicate checking if wanted (not recommended for large file sets) follows Diff.
	retVal = ReadAllFiles( listFileNamesA, BaselineFileName1, true );

	// Free possibly large vector of file names, now in SourceFileA    Modified: 2015.12
	listFileNamesA.resize( 0 );    // Modified: 2015.12
	
	// for DIFF only the file list was built
	time( &time_end_list_built );

	if ( 1 == retVal )
	{
		// Get information about source files for baseline B, build SourceFileB list.
		retVal = ReadAllFiles( listFileNamesB, BaselineFileName2, false );
		
		// for DIFF only the file list was built
		time( &time_end_list_built );
	}

	// Free possibly large vector of file names, now in SourceFileB    Modified: 2015.12
	listFileNamesB.resize( 0 );    // Modified: 2015.12
	
	
	return retVal;
}

/*
* 1. Function Description: 
*     Matches each file between the two baselines mostly using file Paths for best possible match. Including
*    a. Build a preference list for each file. 
*          Preference list is an ordered list of files in the other baseline in order of the quality of the match
*    b. Run the Gale-Shapley algorithm to create a matching
*    c. Files that are unmatched will be paired with an empty file to be compared to
*    Two files are matched if and only if they have the same sort name (file name only part is same).
*
* 2. Parameters: 
*    webSepFilesOnly: whether only match web separation files
*	 
* 3. Creation Time and Owner: 
*	 Version 2011.05
*	 Revised 2015.12  Delayed File Read, Refactored before CompareFilePaths call for much faster Path compares, show count done
*/
void DiffTool::MatchBaseLines( const string commonPathBetweenBaselines, const bool webSepFilesOnly )
{
	// Quickly removing any common Path prefix is good.
	unsigned int commonABprefixSize = commonPathBetweenBaselines.size();

	// list of files each with a preferences list
	// Modification: 2011.05
	BaselinePreferenceMapType BaseAPrefs, BaseBPrefs;

	// go through the SourceFileA list and compare each to SourceFileB to generate a preference
	// BaselineA, but first check if it is empty
	if (SourceFileA.size() == 0) 
	{
		for (SourceFileList::iterator j = SourceFileB.begin(); j != SourceFileB.end(); j++) 
			BaseBPrefs[&(*j)] = NULL;
	}

	// create the multimaps that will be used
	// the key for the map will be the file name
	// Modification: 2013.04
	SortedPreferenceMapType mapA;
	SortedPreferenceMapType mapB;

	// build multimap of all the file names for A
	for (SourceFileList::iterator i = SourceFileA.begin(); i != SourceFileA.end(); i++)
	{
		if ( webSepFilesOnly && ( (*i).second.file_name_isEmbedded == false ) )   // Modification: 2015.12
			continue;

		mapA.insert(FileNamePair((*i).second.file_name_only, &(*i)));   // Modified: 2015.12
	}

	// build multimap of all the file names for B
	for (SourceFileList::iterator i = SourceFileB.begin(); i != SourceFileB.end(); i++)
	{
		if ( webSepFilesOnly && ( (*i).second.file_name_isEmbedded == false ) )   // Modification: 2015.12
			continue;

		mapB.insert(FileNamePair((*i).second.file_name_only, &(*i)));   // Modified: 2015.12
	}

	// Modification: 2011.05
	PreferenceMapType *myAPrefs, *myBPrefs;
	BaselinePreferenceMapType::iterator sfBIterator;
	PreferenceStruct myAPS, myBPS;
	size_t	file_name_only_size = 0;		// Modification: 2015.12
	int		ourPreference = MAX_MISMATCH_COST;
	
// LOOP 1	Get the distances (lower is better) between file path pairs have same File Name
//
	// Let the User know something is going on...			Modified: 2015.12
	unsigned int	percent_done = 0;
	unsigned int	prev_percent_done = 0;
	unsigned long	loop_count_done = 0L;
	unsigned long	count_done = 0L;
	unsigned long	prev_count_done = 0L;
	string			cntStr;
#define		UI_MATCH_UPDATE_COUNT	30
	
	// set this to be 0 to 33 % of total change User will see
	unsigned int	num_in_list = mapA.size() * 3;	// Doing 3 Loops so this is 1/3

#ifndef		QTGUI
	cout << "          ";
#endif

	// Modification: 2013.04
	for (SortedPreferenceMapType::iterator i = mapA.begin(); i != mapA.end(); i++)
	{
		loop_count_done++;
		count_done++;

		if ( webSepFilesOnly && ( (*i).second->second.file_name_isEmbedded == false ) )  // Modification: 2015.12
			continue;

		myBPrefs = new PreferenceMapType;

		// BaselineB
		SortedPreferenceMapType::iterator stop_point = mapB.upper_bound((*i).second->second.file_name_only);    // Modified: 2015.12
		for (SortedPreferenceMapType::iterator j = mapB.lower_bound((*i).second->second.file_name_only); j != stop_point; j++)    // Modified: 2015.12
		{
			if ( webSepFilesOnly && ( (*j).second->second.file_name_isEmbedded == false ) )   // Modification: 2015.12
				continue;

			// need to check if this file has a preference list, if so use it, if not create it
			// Modification: 2011.05
			sfBIterator = BaseBPrefs.find((*j).second);
			if (sfBIterator != BaseBPrefs.end())
			{
				// already exists, use it
				myAPrefs = (*sfBIterator).second;
			}
			else
			{
				// does not exist, create one
				myAPrefs = new PreferenceMapType;
			}

			// Compare this file Path to the file Path from A
			// Path compare Precondition: File Name part must match.  Modification: 2015.12
			bool	preference_set = false;
			if ( webSepFilesOnly )
			{
				// The saved file name only is the physical file name.
				// Need to compare using the extra *.*htm at the end of embedded "file" name.
				// This prevents false matches of different embedded languages from similar physical file.
				string embFile_name_only_I = CUtil::ExtractFilename( (*i).second->second.file_name );
				string embFile_name_only_J = CUtil::ExtractFilename( (*j).second->second.file_name );
				// For equality, Faster to check size before calling .compare
				if ( ( embFile_name_only_I.size() != embFile_name_only_J.size() )
				  || ( embFile_name_only_I.compare( embFile_name_only_J ) ) )
				{
					// File Names different so no need to call CompareFilePaths
					ourPreference = MAX_MISMATCH_COST;
					preference_set = true;
				}
				else
					file_name_only_size = embFile_name_only_I.size();
			}
			else
			{
				// For equality, Faster to check size before calling .compare
				file_name_only_size = (*i).second->second.file_name_only.size();
				if ( ( file_name_only_size != (*j).second->second.file_name_only.size() )
				  || ( (*i).second->second.file_name_only.compare( (*j).second->second.file_name_only ) ) )
				{
					// File Names different so no need to call CompareFilePaths
					ourPreference = MAX_MISMATCH_COST;
					preference_set = true;
				}
			}

			if ( !preference_set )
			{
				// Compare this file Path to the file Path from A
				// Satisfy the Preconditions:  Remove at the least the identical file Name parts
				string path_I = (*i).second->second.file_name.substr( commonABprefixSize, (*i).second->second.file_name.size() 
																	- file_name_only_size - commonABprefixSize );
				string path_J = (*j).second->second.file_name.substr( commonABprefixSize, (*j).second->second.file_name.size() 
																	- file_name_only_size - commonABprefixSize );
				// Remove the Common Prefix
				string prefix;
				CUtil::FindCommonPrefix( path_I, path_J, prefix, true );

				// Remove the Common Suffix
				string suffix;
				CUtil::FindCommonSuffix( path_I, path_J, suffix, true );

				// Check for trivial case for Levenshtein Distance; either string is empty
				if ( path_I.size() == 0 )
					ourPreference = (int)(path_J.size());
				else if ( path_J.size() == 0 )
					ourPreference = (int)(path_I.size());
				else
					ourPreference = (int)CompareFilePaths( path_I, path_J );
			}

			// update preference lists for both files
			myBPS.fileElement = (*j).second;
			myBPS.value = ourPreference;
			myAPS.fileElement = (*i).second;
			myAPS.value = ourPreference;
			myBPrefs->push_back(myBPS);
			myAPrefs->push_back(myAPS);

			// make sure its sorted by preference value
			sort(myAPrefs->begin(), myAPrefs->end(), CustomCMP());

			// save the new values for the BaseBPrefs
			BaseBPrefs[(*j).second] = myAPrefs;
		}

		// make sure its sorted by preference value
		sort(myBPrefs->begin(), myBPrefs->end(), CustomCMP());

		// save the new values for the BaseAPrefs
		BaseAPrefs[(*i).second] = myBPrefs;

		if ( HasUserCancelled() )
			return;

		percent_done = ( loop_count_done * 100 ) / num_in_list;
		if ( ( percent_done != prev_percent_done )
		  || ( prev_count_done + UI_MATCH_UPDATE_COUNT <= count_done ) )
		{
		#ifdef	QTGUI
			// Percent changed so update UI
			userIF->updateProgress( "", false, (int)percent_done, num_in_list );
		#else
			// Just keep erasing and redrawing a text area with updated count
			LongToStr( (long)count_done, cntStr );
			cout << "\b\b\b\b\b\b\b\b\b\b" << cntStr;		// 10 backspace + new count
			for ( int i = 0 ; i < 10 - (int)cntStr.size(); i++ )
				cout << " ";	// fill at right to be easier next time
			cout << flush;
		#endif
			prev_percent_done = percent_done;
			prev_count_done = count_done;
		}
	}	//	END		LOOP 1

	// add empty preference lists for unmatched B files
	for (SortedPreferenceMapType::iterator j = mapB.begin(); j != mapB.end(); j++)
	{
		if ( webSepFilesOnly && ( (*j).second->second.file_name_isEmbedded == false ) )   // Modification: 2015.12
			continue;

		// need to check if this file has a preference list, if so use it, if not create it
		sfBIterator = BaseBPrefs.find((*j).second);
		if (sfBIterator == BaseBPrefs.end())
		{
			// does not exist, create one
			myBPrefs = new PreferenceMapType;

			// save the new values for the BaseBPrefs
			BaseBPrefs[(*j).second] = myBPrefs;
		}
	}

	// at this point we have a complete sorted preference list for all pairs
	// we will want to match preferences that have the smallest value as it is actually the value of optimal alignment
	// now run the Gale-Shapley algorithm
	// Modification: 2011.05
	list<SourceFileElement *> FreeBaseAList;
	map<SourceFileElement *, PreferenceMapType::iterator> NextSelectionMapBaseA;
	BaselineFileMapType BaseAMatches, BaseBMatches;
	list<SourceFileElement *>::iterator myAFile;
	PreferenceMapType::iterator myBFile;
	for (BaselinePreferenceMapType::iterator myI = BaseAPrefs.begin(); myI != BaseAPrefs.end(); myI++)
	{
		BaseAMatches[(*myI).first] = NULL;
		FreeBaseAList.push_back((*myI).first);
		NextSelectionMapBaseA.insert(make_pair((*myI).first, (*myI).second->begin()));
	}
	
	// do this for completeness later on
	for (BaselinePreferenceMapType::iterator myI = BaseBPrefs.begin(); myI != BaseBPrefs.end(); myI++)
		BaseBMatches[(*myI).first] = NULL;

	BaselineFileMapType::iterator mLocation;
	SourceFileElement *myAFilePrime;
	bool foundAPrimeFirst;

//	LOOP	2   Try to find matches for BaselineA unmatched files
//
	// Let the User know something is going on...			Modified: 2015.12
	percent_done = 0;	
	prev_percent_done = 0;
	loop_count_done = 0;
	
	// set this to be 33 to 66 % of total change User will see
	num_in_list = FreeBaseAList.size() * 3;  // Doing 3 Loops so this is another 1/3

	while (!FreeBaseAList.empty())
	{
		loop_count_done++;
		count_done++;

		// find a file in BaselineA that is unmatched and has not tried to match against all of files in BaselineB
		myAFile = FreeBaseAList.begin();
		myBFile = NextSelectionMapBaseA[(*myAFile)];
		if (myBFile == BaseAPrefs[(*myAFile)]->end())
		{
			// it has checked all and is not a good match, match to NULL
			BaseAMatches[(*myAFile)] = NULL;

			// remove it from the "Free" list
			FreeBaseAList.pop_front();
			continue;
		}

		// if BFile is unmatched, then pair them...
		mLocation = BaseBMatches.find((*myBFile).fileElement);
		if (mLocation == BaseBMatches.end() || (*mLocation).second == NULL)
		{
			// not matched up, pair it!
			// if the filename is the same, match them
			if ((*myBFile).fileElement->second.file_name_only.compare(    // Modified: 2015.12
				(*myAFile)->second.file_name_only) == 0)    // Modified: 2015.12
			{
				BaseAMatches[(*myAFile)] = (*myBFile).fileElement;
				BaseBMatches[(*myBFile).fileElement] = (*myAFile);
			}
			else
			{
				// otherwise, match them to NULL
				BaseAMatches[(*myAFile)] = NULL;
				BaseBMatches[(*myBFile).fileElement] = NULL;
			}

			// now increase NextSelection for A as it has now examined this preference file entry
			NextSelectionMapBaseA[(*myAFile)] = ++myBFile;

			// remove this A file from free list
			FreeBaseAList.pop_front();
		}
		else
		{
			// this preference is already matched, check stability
			// this is the file in A that B is matched to right now
			myAFilePrime = (*mLocation).second;

			// check to see if FileB prefers AFile over AFilePrime
			foundAPrimeFirst = false;
			for (PreferenceMapType::iterator myI = BaseBPrefs[(*myBFile).fileElement]->begin();
				myI != BaseBPrefs[(*myBFile).fileElement]->end(); myI++)
			{
				if ((*myI).fileElement == myAFilePrime)
				{
					// found the Prime
					foundAPrimeFirst = true;
					break;
				}
				else if ((*myI).fileElement == (*myAFile))
				{
					// found the AFile
					break;
				}
			}
			if (foundAPrimeFirst)
			{
				// file APrime and FileB are a better match, keep it that way
				// now FileA remains unmatched but increment its next selection
				NextSelectionMapBaseA[(*myAFile)] = ++myBFile;

				//now check to see if it has checked all the files, if so match it with NULL
				if (myBFile == BaseAPrefs[(*myAFile)]->end())
				{
					// it has checked all and is not a good match, match to NULL
					BaseAMatches[(*myAFile)] = NULL;

					// remove it from the "Free" list
					FreeBaseAList.pop_front();
				}
			}
			else
			{
				// FileB and FileA are better matches....
				// free FileAPrime or match it to NULL
				BaseAMatches[myAFilePrime] = NULL;

				// now check to see if it has checked all the files
				if (NextSelectionMapBaseA[myAFilePrime] != BaseAPrefs[myAFilePrime]->end())
				{
					// has not checked all, put back on free list
					FreeBaseAList.push_back(myAFilePrime);
				}

				// if FileA and FileB have the same name, match them
				if ( (*myBFile).fileElement->second.file_name_only.compare(    // Modified: 2015.12
					 (*myAFile)->second.file_name_only) == 0 )    // Modified: 2015.12
				{
					// match FileA and FileB
					BaseAMatches[(*myAFile)] = (*myBFile).fileElement;
					BaseBMatches[(*myBFile).fileElement] = (*myAFile);
				}
				else
				{
					// match FileA and FileB to NULL
					BaseAMatches[(*myAFile)] = NULL;
					BaseBMatches[(*myBFile).fileElement] = NULL;
				}
				NextSelectionMapBaseA[(*myAFile)] = ++myBFile;

				// remove this AFile from free list.
				FreeBaseAList.pop_front();				
			}
		}

		if ( HasUserCancelled() )
			return;

		percent_done = ( loop_count_done * 100 ) / num_in_list;
		if ( ( percent_done != prev_percent_done )
		  || ( prev_count_done + UI_MATCH_UPDATE_COUNT <= count_done ) )
		{
		#ifdef	QTGUI
			// Percent changed so update UI.
			userIF->updateProgress( "", false, (int)(percent_done + 33), num_in_list );
		#else
			// Just keep erasing and redrawing a text area with updated count
			LongToStr( (long)count_done, cntStr );
			cout << "\b\b\b\b\b\b\b\b\b\b" << cntStr;		// 10 backspace + new count
			for ( int i = 0 ; i < 10 - (int)cntStr.size(); i++ )
				cout << " ";	// fill at right to be easier next time
			cout << flush;
		#endif
			prev_percent_done = percent_done;
			prev_count_done = count_done;
		}
	}	//	END		LOOP 2

//		LOOP  3		Comparing Lines in detail
//
	// Let the User know something is going on...			Modified: 2015.12
	percent_done = 0;	
	prev_percent_done = 0;
	loop_count_done = 0;
	
	// set this to be 33 to 66 % of total change User will see
	num_in_list = BaseAMatches.size() * 3;  // Doing 3 Loops so this is last 1/3

	// Create class instances outside loop to prevent needless creation/destruction
	string						myErrList;
	ErrMsgStructVector			err_msgs;
	UncountedFileStructVector	unc_files;

	// now attempt to match unmatched files by checking content
	vector<lineElement>::iterator baseLine, compareLine;
	for (BaselineFileMapType::iterator i = BaseAMatches.begin(); i != BaseAMatches.end(); i++)
	{
		loop_count_done++;
		count_done++;
		if ((*i).second == NULL)
		{
			for (BaselineFileMapType::iterator j = BaseBMatches.begin(); j != BaseBMatches.end(); j++)
			{
				if ((*j).second == NULL)
				{
					SourceFileElement* fileA = (*i).first;
					SourceFileElement* fileB = (*j).first;
					if ( fileA->second.file_size_bytes < 1 || fileB->second.file_size_bytes < 1 )
						continue;	//don't match empty file with different names
					
					// Don't try to read as a physical file embedded or WEB files (already read)
					if ( !fileA->second.file_name_isEmbedded && ( WEB != fileA->second.class_type ) )
					{
						// Make sure file gets buffered and processed before detailed compares
						if ( 0 == fileA->first.size() )
						{
							int errors = 
							ReadFileLines( fileA, MAIN_THREAD_INDEX, userIF, 
										false,				// read Web files
										false,				// not only Physical lines
										clearCaseFile, 
										CounterForEachLanguage, print_cmplx, print_csv, 
										true, // use list A
										&SourceFileA, 
										&SourceFileB,
										myErrList,
										outDir,
										err_msgs,
										unc_files,
										g_process_after_read,
										g_discard_PHY_lines_after_process,
										g_discard_lines_after_process,
										g_no_warnings_to_UI );
							if ( errors )
							{
							#ifdef	_DEBUG
								printf( "Error: MatchBaseLines %d errors from ReadFileLines for %s\n",
									errors, fileA->second.file_name.c_str() );
							#endif
								// Error message already handled as this is the Main thread
								continue;
							}
						}
					}

					// Don't try to read as a physical file embedded or WEB files (already read)
					if ( !fileB->second.file_name_isEmbedded && ( WEB != fileB->second.class_type ) )
					{
						if ( 0 == fileB->first.size() )
						{
							int errors = 
							ReadFileLines( fileB, MAIN_THREAD_INDEX, userIF, 
										false,				// read Web files
										false,				// not only Physical lines
										clearCaseFile, 
										CounterForEachLanguage, print_cmplx, print_csv, 
										false, // use list A
										&SourceFileA, 
										&SourceFileB,
										myErrList,
										outDir,
										err_msgs,
										unc_files,
										g_process_after_read,
										g_discard_PHY_lines_after_process,
										g_discard_lines_after_process,
										g_no_warnings_to_UI );
							if ( errors )
							{
							#ifdef	_DEBUG
								printf( "Error: MatchBaseLines %d errors from ReadFileLines for %s\n",
									errors, fileB->second.file_name.c_str() );
							#endif
								// Error message already handled as this is the Main thread
								continue;
							}
						}
					}

					if (fileA->first.size() == fileB->first.size())
					{
						if (fileA->first.size() < 1 || fileB->first.size() < 1)
							continue;	//don't match empty file with different names

						baseLine = fileA->first.begin();
						compareLine = fileB->first.begin();
						while (baseLine != fileA->first.end() && compareLine != fileB->first.end())
						{
							// This goes line by line and quits if not exactly equal physical lines
							if ((*baseLine).line.compare((*compareLine).line) != 0)
								break;
							baseLine++;
							compareLine++;
						}
						if (baseLine == fileA->first.end() && compareLine == fileB->first.end())
						{
							// Files match !
							(*i).second = fileB;
							(*j).second = fileA;
						}
					}
				}
			}
		}

		if ( HasUserCancelled() )
			return;

		percent_done = ( loop_count_done * 100 ) / num_in_list;
		if ( ( percent_done != prev_percent_done )
		  || ( prev_count_done + UI_MATCH_UPDATE_COUNT <= count_done ) )
		{
		#ifdef	QTGUI
			// Percent changed so update UI.  Add 66 percent as this is 3rd loop.
			userIF->updateProgress( "", false, (int)(percent_done + 66), num_in_list );
		#else
			// Just keep erasing and redrawing a text area with updated count
			LongToStr( (long)count_done, cntStr );
			cout << "\b\b\b\b\b\b\b\b\b\b" << cntStr;		// 10 backspace + new count
			for ( int i = 0 ; i < 10 - (int)cntStr.size(); i++ )
				cout << " ";	// fill at right to be easier next time
			cout << flush;
		#endif
			prev_percent_done = percent_done;
			prev_count_done = count_done;
		}
	}

	/* 
	*  now we have our matches in BaseAMatches
	*  now go through all files in A, if matched create pair, if unmatched pair with NULL
	*  then go through B, if matched, ignore Modification: already added from A, if unmatched pair with NULL
	*  process ordering based on input lists
	*/
	for (SourceFileList::iterator i = SourceFileA.begin(); i != SourceFileA.end(); i++)
	{
		if ( webSepFilesOnly && ( (*i).second.file_name_isEmbedded == false ) )    // Modification: 2015.12
			continue;

		// get the corresponding match
		mLocation = BaseAMatches.find(&(*i));
		if (mLocation != BaseAMatches.end())
		{
			resultStruct myResults;
			matchedFilesList.push_back(make_pair(myResults, (*mLocation)));
			if (mLocation->second != NULL)
				(*i).second.matched = true;
		}
	}

	for (SourceFileList::iterator i = SourceFileB.begin(); i != SourceFileB.end(); i++)
	{
		if ( webSepFilesOnly && ( (*i).second.file_name_isEmbedded == false ) )    // Modification: 2015.12
			continue;

		// get the corresponding match
		mLocation = BaseBMatches.find(&(*i));
		if (mLocation != BaseBMatches.end())
		{
			if (mLocation->second == NULL)
			{
				resultStruct myResults;
				MatchedFilePair myPair = make_pair((*mLocation).second, (*mLocation).first);
				matchedFilesList.push_back(make_pair(myResults, myPair));
			}
			else
				(*i).second.matched = true;
		}
	}

	// at this point we have the complete match set in matchedFilesList
	for (BaselinePreferenceMapType::iterator myI = BaseAPrefs.begin(); myI != BaseAPrefs.end(); myI++)
		delete (*myI).second;
	for (BaselinePreferenceMapType::iterator myI = BaseBPrefs.begin(); myI != BaseBPrefs.end(); myI++)
		delete (*myI).second;

#ifdef QTGUI
	// Call 1 more time to show User 100% completion (this does not depend on following code).
	userIF->updateProgress( "", false, 100, 1 );
#else
	// Just erase the last 10 characters
	cout << "\b\b\b\b\b\b\b\b\b\b          \b\b\b\b\b\b\b\b\b\b";
        if(!doFuncDiff) //Modification: 2017.2
                cout << "\b\b\b\bDONE\n";               // And to adjust for not using calls to update progress
#endif
}

/*
* 1. Function Description: 
*    Calculates the Levenshtein distance between the two file Paths. 
*	 Diff will only compare two files that have the same sort name (excluding the Path).
*    Return case-sensitive Levenshtein distance between file Paths
*
*  Precondition:  (Critical to avoiding inefficient run times)
*    Each Path string MUST have 1 or more characters.
*    Calling code  MUST  remove AT LEAST the known matching ending (file Name part)!
*    Calling code SHOULD remove the Common PREFIX of the two Paths.
*    Calling code SHOULD remove the Common SUFFIX of the two Paths.
*
*  References:
*    This gives a faster and less use of memory approach.
*    Wikipedia article: Levenshtein distance
*       http://en.wikipedia.org/wiki/Levenshtein_distance
*
*    A better C++ approach (that is used below) was found at 
*    Rosetta Code:  Levenshtein distance  C++ (modified to not use iterators)
*
*
*  Performance:
*    Profiling the Difference feature shows this as a primary place to optimize.
*    Rules: Avoid use of complex std containers if possible.  Avoid use of Iterators.
*      Using C style approach will still likely be faster than C++ style OO approach.
*
* 2. Parameters: 
*    file1: original file
*    file2: comparison file
*
* 3. Creation Time and Owner: 
*	 Version 2011.05
*    Revised 2015.12  Renamed & Completely Rewritten for much better Performance (time & memory)
*/
unsigned int DiffTool::CompareFilePaths( const string &s1, const string &s2 )
{
	/* 
	*  This function compares the file Paths without the file Names, not File contents!
	*  Compute the Levenshtein distance between the two file Paths
	*  comparison is always case-sensitive, even in Windows
	*/

// See: Rosetta Code:  Levenshtein distance  C++ implementation
	const unsigned int	m = (unsigned int)s1.size();
	const unsigned int	n = (unsigned int)s2.size();

	// Trivial case: already checked for as part of Precondition
	// if( m == 0 )
	//     return n;
	// if( n == 0 )
	//    return m;

	unsigned int *costs = new unsigned int[n + 1];
	for( unsigned int k = 0; k <= n; k++ )
		costs[k] = k;

	unsigned int	corner;
	unsigned int	upper;
	unsigned int	t;
	unsigned int	j;
	unsigned int	i = 0;
	char			s1_it1;

	for ( unsigned int it1 = 0; it1 != m; ++it1, ++i )
	{
		costs[ 0 ] = i + 1;
		corner = i;
		j = 0;
		s1_it1 = s1[ it1 ];

		for ( unsigned int it2 = 0; it2 != n; ++it2, ++j )
		{
			upper = costs[ j + 1 ];
			if ( s1_it1 == s2[ it2 ] )
			{
				costs[ j + 1 ] = corner;
			}
			else
			{
				t = ( upper < corner ? upper : corner );
				costs[ j + 1 ] = ( costs[ j ] < t ? costs[ j ] : t ) + 1;
			}
			corner = upper;
		}
	}
	unsigned int result = costs[ n ];
	delete [] costs;
	return result;

/*     PREVIOUS and seriously SLOW implementation  COMMENTED OUT
	// Modification: 2011.05
	int delta = 1;
	int mismatchCost = 1;

	int** B = new int*[(int)file1.size()+1];

	// initialize B[i,0] = i*delta
	for (int i = 0; i <= (int)file1.size(); i++)
	{
		B[i] = new int[2];
		B[i][0] = i * delta;
	}
	
	for (int j = 1; j <= (int)file2.size(); j++)
	{
		B[0][1] = j * delta;
		// Modification: 2011.10
		for (int i = 1; i <= (int)file1.size(); i++)
		{
			int ourMatchCost = (file1[i-1] == file2[j-1]) ? 0 : mismatchCost;
			B[i][1] = min(ourMatchCost + B[i-1][0],
				min(delta + B[i-1][1], delta + B[i][0]));
		}
		// Modification: 2013.04
		for (int i = 0; i <= (int)file1.size(); i++)
			B[i][0] = B[i][1];
	}

	// the optimal alignment value is in B[file1.size(),1]
	int rValue = B[(int)file1.size()][1];
	for (int i = 0; i <= (int)file1.size(); i++)
		delete[] B[i];
	delete[] B;

	return rValue;
*/
}

/*
* 1. Function Description: 
*    Executes the differencing comparison on a List of mapped pairs. 
*    Assumes that the SLOC lines have been put int the mySLOCLines object in the results object of each file.
*	 
* 2. Parameters:
*      DiffTool::matchedFilesList  IN   List of Pairs of files Matched for detailed Diff
*    MainObject::visualDiff        IN   true if presentation of Diff details is wanted (HTML output)
*
* 3. Creation Time and Owner: 
*	 Version 2011.05
*	 Revised 2015.12  Threads! Process PHY lines just in time, Shows running count
*/
void DiffTool::ProcessPairs()
{
	string	errList;            // Modification: 2015.12

    // output differences between baselines in diff_dump.txt if visualdiff switch is set
    // Modification: 2014.08s
    if ( visualDiff ) 
	{
        string fName = outDir + FILE_DUMP;
        outfile_file_dump.open( fName.c_str(), ofstream::out );
    }

	// Let the User know something is going on...			Modified: 2015.12
	string			cntStr;

#ifndef	QTGUI
	cout << "..........";  // set up a small area / 10 blanks / to update on top of
#endif

	// Create needed args
	string						myErrList;
	ErrMsgStructVector			err_msgs;
	UncountedFileStructVector	unc_files;
	MatchingType::iterator myI = matchedFilesList.begin();
	MatchingType::iterator itEnd = matchedFilesList.end();

//
// List of file Pairs to Diff is ready.  Send to Thread helper for worker threads if wanted.
//
//  NOTE: To have thread safe code, more arguments need to be passed to thread code.
//  Thread arguments may be pass by value of Global flags or other variables.
//  Threads use args on their local stacks to avoid False Sharing performance problem.

	// Set up to either call thread helper or just run single threaded.

#ifdef	ENABLE_THREADS
        //Modification: 2018.01 : Moved count_done inside ENABLE_THREADS to avoid compilation warnings
	unsigned long	count_done = 0;
	if ( workThreadsCount >= MIN_UCC_THREAD_COUNT )
	{
	#ifdef	QTGUI
		unsigned int	num_in_list = matchedFilesList.size();
	#endif
		DiffPairsThreads( this, userIF, print_cmplx, print_csv, 
								clearCaseFile, outDir, 
								g_discard_PHY_lines_after_process,
								g_discard_lines_after_process,
								g_no_warnings_to_UI );

		// Main thread waits until Differences are finished.
		unsigned int	num_busy = 1;
		unsigned long	prev_count_done = 0L;

		do 
		{
	#ifdef	QTGUI
			ThreadSleep( 250 );		// wake up about 4 times per second
	#else
			ThreadSleep( 1000 );	// wake up about 1 time per second
	#endif
			// Returns count of threads working 
			// and will set count_done (how many Pairs) were Diffed so far
			// Don't automatically call CombineThreadResults here as UI must update first
			num_busy = NumThreadsBusy( count_done, 100, false );

			if ( num_busy )
			{
				if ( count_done != prev_count_done )
				{
			#ifdef	QTGUI
					// Percent changed so update Qt UI
					int percent_done = (int)( ( count_done * 100 ) / num_in_list );
					userIF->updateProgress( "", false, percent_done, (unsigned int)num_in_list );
			#else
					// Just keep erasing and redrawing a text area with updated count
					LongToStr( (long)count_done, cntStr );
					cout << "\b\b\b\b\b\b\b\b\b\b" << cntStr;		// 10 backspace + new count
					for ( int i = 0 ; i < 10 - (int)cntStr.size(); i++ )
						cout << " ";	// fill at right to be easier next time
					cout << flush;
			#endif
					prev_count_done = count_done;
				}
			}

			if ( HasUserCancelled() )
				break;

		} while ( num_busy );
	}
	else
#endif
	{
		// Not using extra worker threads
		// Still uses optimizations
		DiffPairsInList( MAIN_THREAD_INDEX,
										match_threshold,
										userIF,
										printDup,
										CounterForEachLanguage,
										print_cmplx,
										print_csv,
										myI,		// matchedFilesList, matching file pairs and differencing results 
										itEnd,
										clearCaseFile,
										&SourceFileA,
										&SourceFileB,
										myErrList,
										outDir,
										err_msgs,
										unc_files,
										true,		// g_discard_PHY_lines_after_process,
										g_discard_lines_after_process,
										g_no_warnings_to_UI,
										dup_addedLines,
										dup_deletedLines,
										dup_modifiedLines,
										dup_unmodifiedLines,
										total_addedLines,
										total_deletedLines,
										total_modifiedLines,
										total_unmodifiedLines );

		// Here the error_count is for info purposes.  
		// Single thread mode mostly does the Error reporting as expected.
		//count_done = matchedFilesList.size(); //Modification: 2018.01 count_done not used anymore
	}

#ifdef	QTGUI
	// Call 1 more time to show User 100% completion (this does not depend on following code).
	userIF->updateProgress( "", false, 100, 1 );
#else
	// Just erase the last 10 characters
	cout << "\b\b\b\b\b\b\b\b\b\b          \b\b\b\b\b\b\b\b\b\b";
        if(!doFuncDiff) //Modification: 2017.2
                cout << "\b\b\b\bDONE\n";               // And to adjust for not using calls to update progress
#endif

#ifdef	ENABLE_THREADS
	if ( workThreadsCount >= MIN_UCC_THREAD_COUNT )
	{
		CombineThreadResults();
		// At this point any Errors => log or UI messages, Uncounted Files, UI messages
		// were handled along with the results of Reading in the List of Files
	}
#endif

	if ( errList.size() )
		userIF->updateProgress( errList );
    //Warning fix 11.25.16. DO NOT USE THIS VARIABLE AFTER THIS.
    //(void)error_count;
    //Warning fix 11.25.16. DO NOT USE THIS VARIABLE AFTER THIS.
    //(void)count_done;
	return;
}

/*!
* 1. Function Description: 
*    Print the matching pairs of files.
*
* 2. Parameters:
*
* 3. Creation Time and Owner: 
*	 Version 2011.05
*/
void DiffTool::PrintMatchedPairs()
{
	// Modification: 2011.05
	ofstream pairFile, pairFileCSV;
    //Prepare to print ASCII text file
	if (print_ascii || print_legacy)
	{
		string fName = outDir + MATCH_PAIRS_OUTFILE;
		pairFile.open(fName.c_str(), ofstream::out);
        //File open failed
		if (!pairFile.is_open())
		{
			string err = "Error: Failed to open matched file pairs summary output file (";
			err += MATCH_PAIRS_OUTFILE;
			err += ")";
			// Modification: 2013.04
			userIF->AddError(err);
			return;
		}

        //Print the header of the file and set output format.
        // Modification: 2011.10
		PrintFileHeader(pairFile, "MATCHED FILE PAIRS", cmdLine);
		
		// Modification: 2011.05
		pairFile.setf(ofstream::left);
		pairFile.width(45);
		pairFile << "Baseline A";
		pairFile.unsetf(ofstream::left);
		pairFile.width(5);
		pairFile << "  |  ";
		pairFile.width(3);
		pairFile.setf(ofstream::left);
		pairFile.width(45);
		pairFile << "Baseline B";
		pairFile << endl;
		for (int y = 0; y < 90; y++)
			pairFile << "-";
	}

    //Prepare to print the file in CSV format
    if (print_csv)
	{
        string fName = outDir + MATCH_PAIRS_OUTFILE_CSV;
		pairFileCSV.open(fName.c_str(), ofstream::out);
        //File open failed
		if (!pairFileCSV.is_open())
		{
			string err = "Error: Failed to open matched file pairs summary output file (";
			err += MATCH_PAIRS_OUTFILE_CSV;
			err += ")";
			// Modification: 2013.04
			userIF->AddError(err);
			return;
		}
		// Modification: 2011.10
		PrintFileHeader(pairFileCSV, "MATCHED FILE PAIRS", cmdLine);
		// Modification: 2011.05
		pairFileCSV << "Baseline A,Baseline B" << endl;
	}

	// print the results of matching pairs in the file in formatted manner
	string fileA, fileB;

    // Traverse the matchedFilesList
	for (MatchingType::iterator myI = matchedFilesList.begin(); myI != matchedFilesList.end(); myI++)
	{
		if ((*myI).second.first == NULL)
			fileA = "NA";
		else
		{
			// Do not output any embedded file names !		Modification: 2015.12
			if ( (*myI).second.first->second.file_name_isEmbedded )
				continue;

			fileA = (*myI).second.first->second.file_name;
		}

		if ((*myI).second.second == NULL)
			fileB = "NA";
		else
		{
			// Do not output any embedded file names !		Modification: 2015.12
			if ( (*myI).second.second->second.file_name_isEmbedded )
				continue;

			fileB = (*myI).second.second->second.file_name;
		}

        // ASCII format
        if (print_ascii || print_legacy)
		{
			pairFile << endl;
			pairFile.setf(ofstream::left);
			pairFile.width(45);
			pairFile << fileA;
			pairFile.unsetf(ofstream::left);
			pairFile.width(5);
			pairFile << "  |  ";
			pairFile.setf(ofstream::left);
			pairFile.width(45);
			pairFile << fileB;
		}
        // CSV format
		if (print_csv)
			pairFileCSV << fileA << "," << fileB << endl;
	}
	if (print_ascii || print_legacy) {
		PrintFileFooter(pairFile);
		pairFile.close();
	}
	if (print_csv) {
		PrintFileFooter(pairFileCSV);
		pairFileCSV.close();
	}
		
}

/*!
* 1. Function Description: 
*    Prints the results of the file differencing.
*
* 2. Parameters:
*
* 3. Creation Time and Owner: 
*	 Version 2011.05
*/
void DiffTool::PrintDiffResults()
{
	// open the diff results output file
    //ASCII format
    // Modification: 2011.05
	if (print_ascii || print_legacy)
	{
		string fName = outDir + DIFF_OUTFILE;
		outfile_diff_results.open(fName.c_str(), ofstream::out);
        //File open failed
		if (!outfile_diff_results.is_open())
		{
			string err = "Error: Failed to open diff results output file (";
			err += DIFF_OUTFILE;
			err += ")";
			// Modification: 2013.04
			userIF->AddError(err);
			return;
		}
        //Open file for print duplicates
        // Modification: 2011.05
		if (printDup)
		{
			fName = outDir + "Duplicates-";
			fName += DIFF_OUTFILE;
			dup_outfile_diff_results.open(fName.c_str(), ofstream::out);
            //File open failed
			if (!dup_outfile_diff_results.is_open())
			{
				string err = "Error: Failed to open duplicates diff results output file (";
				err += fName;
				err += ")";
				// Modification: 2013.04
				userIF->AddError(err);
				return;
			}
		}
	}
    //CSV format
    // Modification: 2011.05
	if (print_csv)
	{
		string fName = outDir + DIFF_OUTFILE_CSV;
		outfile_diff_csv.open(fName.c_str(), ofstream::out);
        //File open failed
		if (!outfile_diff_csv.is_open())
		{
			string err = "Error: Failed to open diff results output file (";
			err += DIFF_OUTFILE_CSV;
			err += ")";
			// Modification: 2013.04
			userIF->AddError(err);
			return;
		}
        //Open file for print duplicates
        // Modification: 2011.05
		if (printDup)
		{
			fName = outDir + "Duplicates-";
			fName += DIFF_OUTFILE_CSV;
			dup_outfile_diff_csv.open(fName.c_str(), ofstream::out);
            //File open failed
			if (!dup_outfile_diff_csv.is_open())
			{
				string err = "Error: Failed to open duplicates diff results output file (";
				err += fName;
				err += ")";
				// Modification: 2013.04
				userIF->AddError(err);
				return;
			}
		}
	}

	// print the diff results header.
	// Modification: 2011.05
	string myCats[] = {"New", "Deleted", "Modified", "Unmodified", "Module"};
	int i, y, numCats = 5;

    //ASCII format header
	if (print_ascii || print_legacy)
	{
		// Modification: 2011.10
		PrintFileHeader(outfile_diff_results, "SOURCE CODE DIFFERENTIAL RESULTS", cmdLine);
        //Print the title of each column
        for (i = 0; i < numCats; i++)
		{
			outfile_diff_results.setf(ofstream::left);
			outfile_diff_results.width(15);
			outfile_diff_results << myCats[i];
			outfile_diff_results.unsetf(ofstream::left);
			if (i + 1 < numCats)
			{
				outfile_diff_results.width(3);
				outfile_diff_results << " | ";
			}
		}
		outfile_diff_results << endl;
		for (i = 0; i < numCats - 1; i++)
		{
			outfile_diff_results.setf(ofstream::left);
			outfile_diff_results.width(15);
			outfile_diff_results << "Lines";
			outfile_diff_results.unsetf(ofstream::left);
			outfile_diff_results.width(3);
			outfile_diff_results << " | ";
		}
		outfile_diff_results.setf(ofstream::left);
		outfile_diff_results.width(15);
		outfile_diff_results << "Name";
		outfile_diff_results.unsetf(ofstream::left);
		outfile_diff_results << endl;
		for (y = 0; y < 120; y++)
			outfile_diff_results << BAR_S;
		outfile_diff_results << endl;


        //Print the header duplicates
		if (printDup)
		{
			// Modification: 2011.10
			PrintFileHeader(dup_outfile_diff_results, "SOURCE CODE DIFFERENTIAL RESULTS", cmdLine);
			// Modification: 2011.05
			for (i = 0; i < numCats; i++)
			{
				dup_outfile_diff_results.setf(ofstream::left);
				dup_outfile_diff_results.width(15);
				dup_outfile_diff_results << myCats[i];
				dup_outfile_diff_results.unsetf(ofstream::left);
				if (i + 1 < numCats)
				{
					dup_outfile_diff_results.width(3);
					dup_outfile_diff_results << " | ";
				}
			}
			dup_outfile_diff_results << endl;
			for (i = 0; i < numCats - 1; i++)
			{
				dup_outfile_diff_results.setf(ofstream::left);
				dup_outfile_diff_results.width(15);
				dup_outfile_diff_results << "Lines";
				dup_outfile_diff_results.unsetf(ofstream::left);
				dup_outfile_diff_results.width(3);
				dup_outfile_diff_results << " | ";
			}
			dup_outfile_diff_results.setf(ofstream::left);
			dup_outfile_diff_results.width(15);
			dup_outfile_diff_results << "Name";
			dup_outfile_diff_results.unsetf(ofstream::left);
			dup_outfile_diff_results << endl;
			for (y = 0; y < 120; y++)
				dup_outfile_diff_results << BAR_S;
			dup_outfile_diff_results << endl;
		}
	}

    // CVS format
	if (print_csv)
	{
		// Modification: 2011.10
		PrintFileHeader(outfile_diff_csv, "SOURCE CODE DIFFERENTIAL RESULTS", cmdLine);
		// Modification: 2011.05
		outfile_diff_csv << "New Lines,Deleted Lines,Modified Lines,Unmodified Lines,Modification Type,Language,Module A,Module B" << endl;
		if (printDup)
		{
			// Modification: 2011.10
			PrintFileHeader(dup_outfile_diff_csv, "SOURCE CODE DIFFERENTIAL RESULTS", cmdLine);
			// Modification: 2011.05
			dup_outfile_diff_csv << "New Lines,Deleted Lines,Modified Lines,Unmodified Lines,Modification Type,Language,Module A,Module B" << endl;
		}
	}

	// print pair results
	resultStruct *myResults;
	string filenameA, filenameB, lang, modType;
	bool unmatchedDup;
	lang = DEF_LANG_NAME;
    //Iterate the matchedFileList
	for (MatchingType::iterator myI = matchedFilesList.begin(); myI != matchedFilesList.end(); myI++)
	{
		// select the filename, choose the baselineA unless this one was only in baselineB
		filenameA = ((*myI).second.first != NULL) ? (*myI).second.first->second.file_name : "NA";
		filenameB = ((*myI).second.second != NULL) ? (*myI).second.second->second.file_name : "NA";

		if (((*myI).first.addedLines == 0 && (*myI).first.deletedLines == 0 &&
			 (*myI).first.modifiedLines == 0 && (*myI).first.unmodifiedLines == 0) ||
			 filenameA.find(EMBEDDED_FILE_PREFIX) != string::npos ||
			 filenameB.find(EMBEDDED_FILE_PREFIX) != string::npos)
		{
			// do not print out if the file is empty or file not supported
			// only print file if it is the actual file (not having *.* in the filename)
			continue;
		}
		else
		{
			if ((*myI).second.first != NULL)
				lang = GetLanguageName( CounterForEachLanguage, (*myI).second.first->second.class_type, (*myI).second.first->second.file_name );      // Modification: 2015.12
			else
				lang = GetLanguageName( CounterForEachLanguage, (*myI).second.second->second.class_type, (*myI).second.second->second.file_name );    // Modification: 2015.12
		}

		unmatchedDup = false;
		if ((*myI).second.first != NULL)
		{
			if ((*myI).second.first->second.duplicate || (*myI).second.first->second.firstDuplicate)
				unmatchedDup = true;
		}
		if ((*myI).second.second != NULL)
		{
			if ((*myI).second.second->second.duplicate || (*myI).second.second->second.firstDuplicate)
			{
				if ((*myI).second.first == NULL)
					unmatchedDup = true;
				else
					unmatchedDup = false;
			}
			else
				unmatchedDup = false;
		}

		// print pair results
		myResults = &((*myI).first);
        // No duplicate
		if (!unmatchedDup)
		{
			if (print_ascii || print_legacy)
			{
				outfile_diff_results.setf(ofstream::left);
				outfile_diff_results.width(15);
				outfile_diff_results << myResults->addedLines;

				outfile_diff_results.unsetf(ofstream::left);
				outfile_diff_results.width(3);
				outfile_diff_results << "| ";
				outfile_diff_results.setf(ofstream::left);
				outfile_diff_results.width(15);
				outfile_diff_results << myResults->deletedLines;	// modified from addedLines

				outfile_diff_results.unsetf(ofstream::left);
				outfile_diff_results.width(3);
				outfile_diff_results << "| ";
				outfile_diff_results.setf(ofstream::left);
				outfile_diff_results.width(15);
				outfile_diff_results << myResults->modifiedLines;

				outfile_diff_results.unsetf(ofstream::left);
				outfile_diff_results.width(3);
				outfile_diff_results << "| ";
				outfile_diff_results.setf(ofstream::left);
				outfile_diff_results.width(15);
				outfile_diff_results << myResults->unmodifiedLines;

				outfile_diff_results.unsetf(ofstream::left);
				outfile_diff_results.width(3);
				outfile_diff_results << "| ";
				outfile_diff_results.setf(ofstream::left);
				outfile_diff_results.width(15);

				outfile_diff_results.setf(ofstream::left);
				outfile_diff_results.width(15);
				if (filenameB.compare("NA") != 0)
					outfile_diff_results << filenameB;
				else
					outfile_diff_results << filenameA;

				outfile_diff_results.unsetf(ofstream::left);
				outfile_diff_results << endl;
			}
			if (print_csv)
			{
				outfile_diff_csv << myResults->addedLines;
				outfile_diff_csv << "," << myResults->deletedLines;
				outfile_diff_csv << "," << myResults->modifiedLines;
				outfile_diff_csv << "," << myResults->unmodifiedLines;

				if (myResults->deletedLines == 0 && myResults->unmodifiedLines == 0 && myResults->modifiedLines == 0)
					modType = "Add";
				else if (myResults->addedLines == 0 && myResults->unmodifiedLines == 0 && myResults->modifiedLines == 0)
					modType = "Del";
				else if (myResults->addedLines == 0 && myResults->deletedLines == 0 && myResults->modifiedLines == 0)
					modType = "Unmod";
				else
					modType = "Mod";

				outfile_diff_csv << "," << modType;
				outfile_diff_csv << ",\"" << lang << "\"";
				outfile_diff_csv << ",\"" + filenameA + "\",\"" + filenameB + "\"";
				outfile_diff_csv << endl;
			}
		}
        else //With duplicate
		{
            if (print_ascii || print_legacy) //ASCII format
			{
				dup_outfile_diff_results.setf(ofstream::left);
				dup_outfile_diff_results.width(15);
				dup_outfile_diff_results << myResults->addedLines;

				dup_outfile_diff_results.unsetf(ofstream::left);
				dup_outfile_diff_results.width(3);
				dup_outfile_diff_results << "| ";
				dup_outfile_diff_results.setf(ofstream::left);
				dup_outfile_diff_results.width(15);
				dup_outfile_diff_results << myResults->deletedLines;	// modified from addedLines

				dup_outfile_diff_results.unsetf(ofstream::left);
				dup_outfile_diff_results.width(3);
				dup_outfile_diff_results << "| ";
				dup_outfile_diff_results.setf(ofstream::left);
				dup_outfile_diff_results.width(15);
				dup_outfile_diff_results << myResults->modifiedLines;

				dup_outfile_diff_results.unsetf(ofstream::left);
				dup_outfile_diff_results.width(3);
				dup_outfile_diff_results << "| ";
				dup_outfile_diff_results.setf(ofstream::left);
				dup_outfile_diff_results.width(15);
				dup_outfile_diff_results << myResults->unmodifiedLines;

				dup_outfile_diff_results.unsetf(ofstream::left);
				dup_outfile_diff_results.width(3);
				dup_outfile_diff_results << "| ";
				dup_outfile_diff_results.setf(ofstream::left);
				dup_outfile_diff_results.width(15);

				dup_outfile_diff_results.setf(ofstream::left);
				dup_outfile_diff_results.width(15);
				if (filenameB.compare("NA") != 0)
					dup_outfile_diff_results << filenameB;
				else
					dup_outfile_diff_results << filenameA;

				dup_outfile_diff_results.unsetf(ofstream::left);
				dup_outfile_diff_results << endl;
			}
            // CSV format
			if (print_csv)
			{
				dup_outfile_diff_csv << myResults->addedLines;
				dup_outfile_diff_csv << "," << myResults->deletedLines;
				dup_outfile_diff_csv << "," << myResults->modifiedLines;
				dup_outfile_diff_csv << "," << myResults->unmodifiedLines;

				if (myResults->deletedLines == 0 && myResults->unmodifiedLines == 0 && myResults->modifiedLines == 0)
					modType = "Add";
				else if (myResults->addedLines == 0 && myResults->unmodifiedLines == 0 && myResults->modifiedLines == 0)
					modType = "Del";
				else if (myResults->addedLines == 0 && myResults->deletedLines == 0 && myResults->modifiedLines == 0)
					modType = "Unmod";
				else
					modType = "Mod";

				dup_outfile_diff_csv << "," << modType;
				dup_outfile_diff_csv << ",\"" << lang << "\"";
				dup_outfile_diff_csv << ",\"" + filenameA + "\",\"" + filenameB + "\"";
				dup_outfile_diff_csv << endl;
			}
		}
	}

	// print the diff results summary
	int ti;
	numCats--;
	// ASCII format
	if (print_ascii || print_legacy)
	{
		outfile_diff_results << endl << endl;
		for (ti = 0; ti < numCats; ti++)
		{
			outfile_diff_results.setf(ofstream::left);
			outfile_diff_results.width(15);
			outfile_diff_results << "Total";
			outfile_diff_results.unsetf(ofstream::left);
			if (ti + 1 < numCats)
			{
				outfile_diff_results.width(3);
				outfile_diff_results << "| ";
			}
		}
		outfile_diff_results << endl;
		for (ti = 0; ti < numCats; ti++)
		{
			outfile_diff_results.setf(ofstream::left);
			outfile_diff_results.width(15);
			outfile_diff_results << myCats[ti];
			outfile_diff_results.unsetf(ofstream::left);
			if (ti + 1 < numCats)
			{
				outfile_diff_results.width(3);
				outfile_diff_results << "| ";
			}
		}
		outfile_diff_results << endl;
		for (ti = 0; ti < numCats; ti++)
		{
			outfile_diff_results.setf(ofstream::left);
			outfile_diff_results.width(15);
			outfile_diff_results << "Lines";
			outfile_diff_results.unsetf(ofstream::left);
			if (ti + 1 < numCats)
			{
				outfile_diff_results.width(3);
				outfile_diff_results << "| ";
			}
		}
		outfile_diff_results << endl;
		for (y = 0; y < 73; y++)
			outfile_diff_results << BAR_S;
		outfile_diff_results << endl;

		outfile_diff_results.setf(ofstream::left);
		outfile_diff_results.width(15);
		outfile_diff_results << total_addedLines;

		outfile_diff_results.unsetf(ofstream::left);  
		outfile_diff_results.width(3);
		outfile_diff_results << "| ";
		outfile_diff_results.setf(ofstream::left);
		outfile_diff_results.width(15);
		outfile_diff_results << total_deletedLines;

		outfile_diff_results.unsetf(ofstream::left);  
		outfile_diff_results.width(3);
		outfile_diff_results << "| ";
		outfile_diff_results.setf(ofstream::left);
		outfile_diff_results.width(15);
		outfile_diff_results << total_modifiedLines;

		outfile_diff_results.unsetf(ofstream::left);  
		outfile_diff_results.width(3);
		outfile_diff_results << "| ";
		outfile_diff_results.setf(ofstream::left);
		outfile_diff_results.width(15);
		outfile_diff_results << total_unmodifiedLines;

		outfile_diff_results << endl << endl;

        // Print duplicates
		if (printDup)
		{
			dup_outfile_diff_results << endl << endl;
			for (ti = 0; ti < numCats; ti++)
			{
				dup_outfile_diff_results.setf(ofstream::left);
				dup_outfile_diff_results.width(15);
				dup_outfile_diff_results << "Total";
				dup_outfile_diff_results.unsetf(ofstream::left);
				if (ti + 1 < numCats)
				{
					dup_outfile_diff_results.width(3);
					dup_outfile_diff_results << "| ";
				}
			}
			dup_outfile_diff_results << endl;
			for (ti = 0; ti < numCats; ti++)
			{
				dup_outfile_diff_results.setf(ofstream::left);
				dup_outfile_diff_results.width(15);
				dup_outfile_diff_results << myCats[ti];
				dup_outfile_diff_results.unsetf(ofstream::left);
				if ((ti + 1) < numCats)
				{
					dup_outfile_diff_results.width(3);
					dup_outfile_diff_results << "| ";
				}
			}
			dup_outfile_diff_results << endl;
			for (ti = 0; ti < numCats; ti++)
			{
				dup_outfile_diff_results.setf(ofstream::left);
				dup_outfile_diff_results.width(15);
				dup_outfile_diff_results << "Lines";
				dup_outfile_diff_results.unsetf(ofstream::left);
				if (ti + 1 < numCats)
				{
					dup_outfile_diff_results.width(3);
					dup_outfile_diff_results << "| ";
				}
			}
			dup_outfile_diff_results << endl;
			for (y = 0; y < 73; y++)
				dup_outfile_diff_results << BAR_S;
			dup_outfile_diff_results << endl;

			dup_outfile_diff_results.setf(ofstream::left);
			dup_outfile_diff_results.width(15);
			dup_outfile_diff_results << dup_addedLines;

			dup_outfile_diff_results.unsetf(ofstream::left);  
			dup_outfile_diff_results.width(3);
			dup_outfile_diff_results << "| ";
			dup_outfile_diff_results.setf(ofstream::left);
			dup_outfile_diff_results.width(15);
			dup_outfile_diff_results << dup_deletedLines;

			dup_outfile_diff_results.unsetf(ofstream::left);  
			dup_outfile_diff_results.width(3);
			dup_outfile_diff_results << "| ";
			dup_outfile_diff_results.setf(ofstream::left);
			dup_outfile_diff_results.width(15);
			dup_outfile_diff_results << dup_modifiedLines;

			dup_outfile_diff_results.unsetf(ofstream::left);  
			dup_outfile_diff_results.width(3);
			dup_outfile_diff_results << "| ";
			dup_outfile_diff_results.setf(ofstream::left);
			dup_outfile_diff_results.width(15);
			dup_outfile_diff_results << dup_unmodifiedLines;

			dup_outfile_diff_results << endl << endl;
		}
	}
    //CSV format
	if (print_csv)
	{
		outfile_diff_csv << endl << "Total New Lines,Total Deleted Lines,Total Modified Lines,Total Unmodified Lines" << endl;
		outfile_diff_csv << total_addedLines;
		outfile_diff_csv << "," << total_deletedLines;
		outfile_diff_csv << "," << total_modifiedLines;
		outfile_diff_csv << "," << total_unmodifiedLines << endl;

		if (printDup)
		{
			dup_outfile_diff_csv << endl << "Total New Lines,Total Deleted Lines,Total Modified Lines,Total Unmodified Lines" << endl;
			dup_outfile_diff_csv << dup_addedLines;
			dup_outfile_diff_csv << "," << dup_deletedLines;
			dup_outfile_diff_csv << "," << dup_modifiedLines;
			dup_outfile_diff_csv << "," << dup_unmodifiedLines << endl;
		}
	}

    //Close file
	if (print_ascii || print_legacy)
	{
		outfile_diff_results.close();
		PrintFileFooter(outfile_diff_results);
		if (printDup)
			dup_outfile_diff_results.close();
	}
	if (print_csv)
	{
		outfile_diff_csv.close();
		PrintFileFooter(outfile_diff_csv);
		if (printDup)
			dup_outfile_diff_csv.close();
	}

	//close dump file stream, otherwise infile_file_dump will be buggy.
	outfile_file_dump.close();

    // output differences between baselines in highlight_diff.html if visualdiff switch is set
    if (visualDiff) {
        string fName = outDir + FILE_DUMP;
        infile_file_dump.open(fName.c_str(), ifstream::in);
        fName = outDir + HTML_DIFF;
        html_diff_stream.open(fName.c_str(), ofstream::out);
        
        CmpMngrHtml(infile_file_dump, html_diff_stream).PrintFileSetAsHtml();
        
        infile_file_dump.close();
        html_diff_stream.close();
    }
}

/*!
* 1. Function Description:
*    Prints the header for the function level differencing output.
*
* 2. Parameters:
*
* 3. Creation Time and Owner:
*        Version 2017.2
*/
void DiffTool::printFuncDiffResultsHeader()
{
    // open the func diff results output file
    //ASCII format
    if (print_ascii || print_legacy)
    {
        string fName = outDir + FUNC_DIFF_OUTFILE;
        outfile_diff_results.open(fName.c_str(), ofstream::out);
        //File open failed
        if (!outfile_diff_results.is_open())
        {
            string err = "Error: Failed to open func diff results output file (";
            err += FUNC_DIFF_OUTFILE;
            err += ")";
            userIF->AddError(err);
            return;
        }
    }
    //CSV format
    if (print_csv)
    {
        string fName = outDir + FUNC_DIFF_OUTFILE_CSV;
        outfile_diff_csv.open(fName.c_str(), ofstream::out);
        //File open failed
        if (!outfile_diff_csv.is_open())
        {
            string err = "Error: Failed to open func diff results output file (";
            err += FUNC_DIFF_OUTFILE_CSV;
            err += ")";
            userIF->AddError(err);
            return;
        }
    }

    // print the func diff results header.
    string myCats[] = {"New", "Deleted", "Modified", "Unmodified", "Function"};
    int i, y, numCats = 5;

    //ASCII format header
    if (print_ascii || print_legacy)
    {
        PrintFileHeader(outfile_diff_results, "FUNCTION LEVEL DIFFERENCING RESULTS", cmdLine);
        //Print the title of each column
        for (i = 0; i < numCats; i++)
        {
            outfile_diff_results.setf(ofstream::left);
            outfile_diff_results.width(15);
            outfile_diff_results << myCats[i];
            outfile_diff_results.unsetf(ofstream::left);
            if (i + 1 < numCats)
            {
                outfile_diff_results.width(3);
                outfile_diff_results << " | ";
            }
        }
        outfile_diff_results << endl;
        for (i = 0; i < numCats - 1; i++)
        {
            outfile_diff_results.setf(ofstream::left);
            outfile_diff_results.width(15);
            outfile_diff_results << "Lines";
            outfile_diff_results.unsetf(ofstream::left);
            outfile_diff_results.width(3);
            outfile_diff_results << " | ";
        }
        outfile_diff_results.setf(ofstream::left);
        outfile_diff_results.width(15);
        outfile_diff_results << "Name";
        outfile_diff_results.unsetf(ofstream::left);
        outfile_diff_results << endl;
        for (y = 0; y < 120; y++)
            outfile_diff_results << BAR_S;
        outfile_diff_results << endl;
    }

    // CVS format
    if (print_csv)
    {
        PrintFileHeader(outfile_diff_csv, "FUNCTION LEVEL DIFFERENCING RESULTS", cmdLine);
        outfile_diff_csv << "New Lines,Deleted Lines,Modified Lines,Unmodified Lines,Modification Type,Language,Baseline A Methods,Baseline B Methods" << endl;
    }
}

/*!
* 1. Function Description:
*    Prints the results of the function level differencing.
*
* 2. Parameters:
*
* 3. Creation Time and Owner:
*        Version 2017.2
*/
void DiffTool::PrintFuncDiffResults()
{
    string myCats[] = {"New", "Deleted", "Modified", "Unmodified", "Module"};
    int y, numCats = 5;

    // print pair results
    resultStruct *myResults;
    string filenameA, filenameB, lang, modType;
    lang = DEF_LANG_NAME;
    //Iterate the matchedFileList
    for (MatchingType::iterator myI = matchedFilesList.begin(); myI != matchedFilesList.end(); myI++)
    {
        // select the filename, choose the baselineA unless this one was only in baselineB
        filenameA = ((*myI).second.first != NULL) ? (*myI).second.first->second.file_name : "NA";
        filenameB = ((*myI).second.second != NULL) ? (*myI).second.second->second.file_name : "NA";

        if (((*myI).first.addedLines == 0 && (*myI).first.deletedLines == 0 &&
             (*myI).first.modifiedLines == 0 && (*myI).first.unmodifiedLines == 0) ||
            filenameA.find(EMBEDDED_FILE_PREFIX) != string::npos ||
            filenameB.find(EMBEDDED_FILE_PREFIX) != string::npos)
        {
            // do not print out if the file is empty or file not supported
            // only print file if it is the actual file (not having *.* in the filename)
            continue;
        }
        else
        {
            if ((*myI).second.first != NULL)
                lang = GetLanguageName( CounterForEachLanguage, (*myI).second.first->second.class_type, (*myI).second.first->second.file_name );      // Modification: 2015.12
            else
                lang = GetLanguageName( CounterForEachLanguage, (*myI).second.second->second.class_type, (*myI).second.second->second.file_name );    // Modification: 2015.12
        }

        if (filenameA.compare("NA") != 0)
        {
                size_t last = filenameA.find_last_of('/');
                if ( last != string::npos)
                {
                        filenameA=filenameA.substr(last+1,filenameA.length()-last-1);
                }
                last = filenameA.find_last_of('\\');
                if ( last != string::npos)
                {
                        filenameA=filenameA.substr(last+1,filenameA.length()-last-1);
                }
                last = filenameA.find_last_of('.');
                if ( last != string::npos)
                {
                        filenameA=filenameA.substr(0,last);
                }
        }
        if (filenameB.compare("NA") != 0)
        {
                size_t last = filenameB.find_last_of('/');
                if ( last != string::npos)
                {
                        filenameB=filenameB.substr(last+1,filenameB.length()-last-1);
                }
                last = filenameB.find_last_of('\\');
                if ( last != string::npos)
                {
                        filenameB=filenameB.substr(last+1,filenameB.length()-last-1);
                }
                last = filenameB.find_last_of('.');
                if ( last != string::npos)
                {
                        filenameB=filenameB.substr(0,last);
                }
        }
    
                // print pair results
        myResults = &((*myI).first);
        if (print_ascii || print_legacy)
        {
                outfile_diff_results << endl;
            outfile_diff_results.setf(ofstream::left);
            outfile_diff_results.width(15);
            outfile_diff_results << myResults->addedLines;

            outfile_diff_results.unsetf(ofstream::left);
            outfile_diff_results.width(3);
            outfile_diff_results << "| ";
            outfile_diff_results.setf(ofstream::left);
            outfile_diff_results.width(15);
            outfile_diff_results << myResults->deletedLines;    // modified from addedLines

            outfile_diff_results.unsetf(ofstream::left);
            outfile_diff_results.width(3);
            outfile_diff_results << "| ";
            outfile_diff_results.setf(ofstream::left);
            outfile_diff_results.width(15);
            outfile_diff_results << myResults->modifiedLines;

            outfile_diff_results.unsetf(ofstream::left);
            outfile_diff_results.width(3);
            outfile_diff_results << "| ";
            outfile_diff_results.setf(ofstream::left);
            outfile_diff_results.width(15);
            outfile_diff_results << myResults->unmodifiedLines;

            outfile_diff_results.unsetf(ofstream::left);
            outfile_diff_results.width(3);
            outfile_diff_results << "| ";
            outfile_diff_results.setf(ofstream::left);
            outfile_diff_results.width(15);

            outfile_diff_results.setf(ofstream::left);
            outfile_diff_results.width(15);
            if (filenameB.compare("NA") != 0)
                outfile_diff_results << filenameB;
            else
                outfile_diff_results << filenameA;

            outfile_diff_results.unsetf(ofstream::left);
            outfile_diff_results << endl;
        }

        if (print_csv)
        {
            outfile_diff_csv << myResults->addedLines;
            outfile_diff_csv << "," << myResults->deletedLines;
            outfile_diff_csv << "," << myResults->modifiedLines;
            outfile_diff_csv << "," << myResults->unmodifiedLines;

            if (myResults->deletedLines == 0 && myResults->unmodifiedLines == 0 && myResults->modifiedLines == 0)
                modType = "Add";
            else if (myResults->addedLines == 0 && myResults->unmodifiedLines == 0 && myResults->modifiedLines == 0)
                modType = "Del";
            else if (myResults->addedLines == 0 && myResults->deletedLines == 0 && myResults->modifiedLines == 0)
                modType = "Unmod";
            else
                modType = "Mod";

            outfile_diff_csv << "," << modType;
            outfile_diff_csv << ",\"" << lang << "\"";
            outfile_diff_csv << ",\"" + filenameA + "\",\"" + filenameB + "\"";
            outfile_diff_csv << endl;
        }
    }

    // print the diff results summary
    int ti;
    numCats--;
    // ASCII format
    if (print_ascii || print_legacy)
    {
        outfile_diff_results << endl;
        for (ti = 0; ti < numCats; ti++)
        {
            outfile_diff_results.setf(ofstream::left);
            outfile_diff_results.width(15);
            outfile_diff_results << "Total";
            outfile_diff_results.unsetf(ofstream::left);
            if (ti + 1 < numCats)
            {
                outfile_diff_results.width(3);
                outfile_diff_results << "| ";
            }
        }
        outfile_diff_results << endl;
        for (ti = 0; ti < numCats; ti++)
        {
            outfile_diff_results.setf(ofstream::left);
            outfile_diff_results.width(15);
            outfile_diff_results << myCats[ti];
            outfile_diff_results.unsetf(ofstream::left);
            if (ti + 1 < numCats)
            {
                outfile_diff_results.width(3);
                outfile_diff_results << "| ";
            }
        }
        outfile_diff_results << endl;
        for (ti = 0; ti < numCats; ti++)
        {
            outfile_diff_results.setf(ofstream::left);
            outfile_diff_results.width(15);
            outfile_diff_results << "Lines";
            outfile_diff_results.unsetf(ofstream::left);
            if (ti + 1 < numCats)
            {
                outfile_diff_results.width(3);
                outfile_diff_results << "| ";
            }
        }
        outfile_diff_results << endl; 
        for (y = 0; y < 73; y++)
            outfile_diff_results << BAR_S;
        outfile_diff_results << endl;

        outfile_diff_results.setf(ofstream::left);
        outfile_diff_results.width(15);
        outfile_diff_results << total_addedLines;

        outfile_diff_results.unsetf(ofstream::left);
        outfile_diff_results.width(3);
        outfile_diff_results << "| ";
        outfile_diff_results.setf(ofstream::left);
        outfile_diff_results.width(15);
        outfile_diff_results << total_deletedLines;

        outfile_diff_results.unsetf(ofstream::left);
        outfile_diff_results.width(3);
        outfile_diff_results << "| ";
        outfile_diff_results.setf(ofstream::left);
        outfile_diff_results.width(15);
        outfile_diff_results << total_modifiedLines;

        outfile_diff_results.unsetf(ofstream::left);
        outfile_diff_results.width(3);
        outfile_diff_results << "| ";
        outfile_diff_results.setf(ofstream::left);
        outfile_diff_results.width(15);
        outfile_diff_results << total_unmodifiedLines;

        outfile_diff_results << endl << endl << endl;
    }
    //CSV format
    if (print_csv)
    {
        outfile_diff_csv << endl << "Total New Lines,Total Deleted Lines,Total Modified Lines,Total Unmodified Lines" << endl;
        outfile_diff_csv << total_addedLines;
        outfile_diff_csv << "," << total_deletedLines;
        outfile_diff_csv << "," << total_modifiedLines;
        outfile_diff_csv << "," << total_unmodifiedLines << endl << endl;
    }
}


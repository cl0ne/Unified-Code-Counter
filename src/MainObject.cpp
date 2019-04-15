//! Main counting object class methods.
/*!
* \file MainObject.cpp
*
* This file contains the main counting object class methods.
*
* Revised: 2015.12  Moved PrintXxx code to UCCFilesOut, some other code to LangUtils.
*                   Added here is: Thread use, Timing, Performance improvements
*/

#include "UCCBeforeLibraryIncludes.h"   // Modification: 2015_12
#include <sstream>
#include <sys/stat.h>
#include <ctime>                        // Modification: 2015_12
#include <stdexcept>
#include "UCCAfterLibraryIncludes.h"    // Modification: 2015_12

#include "UCCGlobals.h"                 // Modification: 2015_12
#include "CUtil.h"
#include "CmpMngr.h"
#include "UCCFilesOut.h"                // Modification: 2015_12
#include "MainObject.h"
#include<string.h>
// Uncomment to show each found path/file name    Modified: 2015.12
//#define		SHOW_FILES_FOUND

//
//		EXTERNAL	Data	and		Procedures
//
extern	bool	print_cmplx;	//!< Print complexity and keyword counts   Modification: 2015.12
extern	bool	print_csv;		//!< Print CSV report files                Modification: 2015.12
extern	bool	g_cc4enable;	//!< CC4 (unique if condition) scans. Print complex must be true   Modification: 2015.12

vector <string>line_delimiters;/* Modification: 2016.10; Vector to store the comment delimiters passed by the ext file */
vector <string>block_delimiters;/*Modification: 2016.10; Vector to store the comment delimiters passed by the ext file */
int DupCheckLanguages(unsigned int					threadIdx,
	vector<struct langTypeArrayHeader> * langs,
	ErrMsgStructVector		*	err_msgs,
	UncountedFileStructVector * unc_files,
	string					*	myErrList,
	const double				dup_threshold,
	StringVector			*	dupList1,
	StringVector			*	dupList2,
	const bool					checkMatch,
	const bool					useListA,
	UserIF					*	userIF,
	const bool					clearCaseFile,
	CounterForEachLangType	&	CounterForEachLanguage,
	const bool					print_cmplx,
	const bool					print_csv,
	SourceFileList			*	mySourceFileA,
	SourceFileList			*	mySourceFileB);

/*!
* 1.Function Description:
*	 Constructs a MainObject object.
*
* 2.Parameters:
*
* 3.Creation Time And Owner:
*	 Version: 2009.01
*    Revised: 2015.12  Calls Init_CounterForEachLanguage
*/
MainObject::MainObject()
{
	userIF = NULL;	// Modification: Version 2013.04
	counter = 0;	// Modification: Version 2009.01

					// Modifacation: 2011.05
	isDiff = false;				// default not differencing files
	outDir = "";				// default output directory is current directory
	duplicate_threshold = 0;	// default duplicate files threshold
	lsloc_truncate = DEFAULT_TRUNCATE;	// default LSLOC maximum characters for truncation (0=no truncation)
	print_cmplx = true;			// default print complexity and keyword counts
	g_cc4enable = false;		// default to NOT do CC4 cyclomatic complexity metric
	print_csv = true;			// default print CSV report files
	print_ascii = false;		// default don't print ASCII text report files
	print_legacy = false;		// default don't print legacy ASCII text report files
	print_unified = false;		// default don't print all counting files to a single unified file
	clearCaseFile = false;		// default don't process Clear Case file names
	followSymLinks = true;		// default follow symbolic links
	use_CommandLine = false;    // default read all files from fileList file
	workThreadsCount = 0;		// default is no worker threads    // Modification: 2015.12
 
    visualDiff = false;         // default don't create diff_dump.txt and highlighted_diff.html
	remove_Header = false;      // default don't remove header //Modification 2016.12
	// Modification: 2013.04
	match_threshold = DIFF_MATCH_THRESHOLD;	// default modified lines threshold

	int init_result = Init_CounterForEachLanguage(CounterForEachLanguage);    // Modification: 2015.12
	if (init_result)
	{
		// Handle error as best as possible.
		// Realistically an error here is VERY unlikely and previous version did the above here as well.
		// Because this is called from within a class constructor there is no return value allowed.
		cout << endl << "Error: MainObject constructor unable to init CounterForEachLanguage" << endl;
		//throw std::runtime_error("Error: MainObject constructor unable to init CounterForEachLanguage");
	}    // Modification: 2015.12
	else
	{
		// initialize the count lists
		ResetCounterCounts(CounterForEachLanguage);    // Modification: 2011.05  2015.12
	}
}

/*!
* 1.Function Description:
*	 Destroys a MainObject object.
*
* 2.Parameters:
*
* 3.Creation Time And Owner:
*	 Version: 2009.01
*    Revised: 2015.12  Stops any running threads
*/
MainObject::~MainObject()
{
	// First stop and exit Worker threads in case they were updating any shared data structures
	if (workThreadsCount >= MIN_UCC_THREAD_COUNT)    // Modified: 2015.12
		FinishWorkerThreads();

	workThreadsCount = 0;

	// Release RAM in reverse order of allocation
	duplicateFilesInB2.resize(0);
	duplicateFilesInB1.resize(0);
	duplicateFilesInA2.resize(0);
	duplicateFilesInA1.resize(0);

	SourceFileB.resize(0);
	SourceFileA.resize(0);

	ClearBaselineStats();

	for (map<int, CCodeCounter*>::iterator iter = CounterForEachLanguage.begin(); iter != CounterForEachLanguage.end(); iter++)
	{
		delete iter->second;
		iter->second = 0;
	}
	if (userIF != NULL)		// Modification:2013.04
		delete(userIF);		// Modification:2013.04
}

/*!
* 1.Function Description:
* Must have a single point to check for User Cancelling so threads can be stopped if needed.
*
* 2.Parameters:
*   (none)
*   Returns true if the User has cancelled and stops running threads
*
* 3.Creation Time And Owner:
*	 Version: 2015.12   Randy Maxwell
*/
bool MainObject::HasUserCancelled()
{
	bool	has_cancelled = false;

	if (userIF != NULL)
	{
		has_cancelled = userIF->isCanceled();
		if (has_cancelled)
		{
#ifdef	ENABLE_THREADS
			if (workThreadsCount >= MIN_UCC_THREAD_COUNT)
			{
				// Perhaps use new way to stop threads and not destroy them
				// This is a synchronous call and will NOT return until threads are stopped
				FinishWorkerThreads();
			}
#endif
			workThreadsCount = 0;
		}
	}
	return has_cancelled;
}


/*!
* 1.Function Description:
*   Helpers to support reporting times of various steps
*
* 2.Parameters:
*   (none)
*   Returns the duplicate threshold value
*
* 3.Creation Time And Owner:
*	 Version: 2015.12   Randy Maxwell
*/
double MainObject::GetDuplicateThreshold()
{
	return duplicate_threshold;
}

double MainObject::GetMatchThreshold()
{
	return match_threshold;
}

/*!
* 1.Function Description:
*	 Main counting process.
*    Return value is function status
*
* 2.Parameters:
*   argc: number of arguments
*   argv: argument list
*
* 3.Creation Time And Owner:
*	 Version: 2009.01
*    Revised: 2015.12  Threads, Timing, Performance
*/
int MainObject::MainProcess(int argc, char *argv[])
{
	/*
	Counting Process:
	1. Parse options
	2. Enable optimizations and IF -threads 2 (or more) start worker Threads
	3. Find files to work on (build a list of files to do)
	4. Read input files found into RAM memory
	5. Run counter on baseline A files (no baseline B files here, only for Diffs)
	6. IF Duplicate checking wanted: Identify duplicate files and process them
	7. Stop worker threads if started
	8. Print all results data

	Notes: Steps 3 to 6 may be implemented using worker threads.
	Currently Steps 4 and 5 can run multithreaded.
	IF
	using multiple threads
	THEN
	it makes more sense to go ahead and read the file contents
	while working from a list of files for that thread to do.
	See UCCThread.cpp
	*/

	// create a user interface object
	if (userIF == NULL) // Modification: 2011.10B
		userIF = new UserIF();

	// handle input file list
	BaselineFileName1 = INPUT_FILE_LIST_NAME;

	// parse the command line input
	if (!ParseCommandLine(argc, argv))
		ShowUsage();

	SetCounterOptions(CounterForEachLanguage);    // Modification: 2015.12

												  //Handle the input list file name in Baseline A
	string inputListFileName1 = BaselineFileName1;
#ifdef QTGUI    // Modification: 2011.10B
	if (outDir != "")
		BaselineFileName1 = outDir + INPUT_FILE_LIST_NAME;
#endif
	if (HasUserCancelled())
		return 0;

	// Enable Optimizations.  Start threads if wanted.    Modified: 2015.12
	// Threads must be started (if wanted) before file Extension maps are done.
	string	start_threads_result_msg;
	StartThreads(start_threads_result_msg);
	if (start_threads_result_msg.size())
		userIF->updateProgress(start_threads_result_msg, false);

	// generate user-defined language extension map if extfile exists
	ifstream userExtMapFileStream;
	userExtMapFileStream.open(userExtMapFile.c_str(), ifstream::in);

	if (userExtMapFileStream.is_open())
	{
		userExtMapFileStream.close();
		ReadUserExtMapping(userExtMapFile);
	}
	else if (userExtMapFile.length() != 0)
	{
		cout << "Error: Unable to open file extension mapping file: " + userExtMapFile << endl;    // Modification: 2015.12
		return 0;

	}

	// Make a list of input source files
	userIF->updateProgress("Building list of source files...");    // Modified: 2015.12

																   // read the source files
	vector<string> listFileNames;
	if (listFilesToBeSearched.size() != 0)
		CUtil::ListAllFiles(dirnameA, listFilesToBeSearched, listFileNames, followSymLinks,
			totalFileSizesA, largestFileSizeA, numFilesInA, commonPathPrefixA);

	time(&time_end_list_built);    // Modified: 2015.12

	if (HasUserCancelled())
		return 0;

	//Get complete path of the file from the command arguments
	for (int i = 0; i < argc; i++)  // Modification: 2014.08s
	{
		string arg = argv[i];
		if (arg == "-i1")
		{
			// extract the BaselineA file
			if (i + 1 > argc)
			{
				string err = "Error: -i1  Baseline A file name of file list is missing";
				userIF->AddError(err);
				return 0;
			}
			i++;

			ifstream infile;
			infile.open(argv[i], ifstream::in);
			infile.close();
			if (infile.fail())
			{
				// file did not exist
				string err = "Error: Unable to open baseline file (";
				err += argv[i];
				err += ")";
				userIF->AddError(err);
				return 0;
			}
			inputListFileName1 = string(argv[i]);
			break;
		}
	}

	if (HasUserCancelled())
		return 0;

	// Read the source files, now shows counts or % complete as it runs.    Modified: 2015.12
	// Finish building the list of files and then Estimate RAM needed for success.
	if (1 != ReadAllFiles(listFileNames, inputListFileName1))
		return 0;

	// Free possibly large vector of file names, now in SourceFileA    Modified: 2015.12
	listFileNames.clear();    // Modified: 2015.12

	time(&time_end_files_read);    // Modification: 2015.12

								   //#ifndef QTGUI       //Modification: 2011.10B
								   //	userIF->updateProgress(".......................DONE");
								   //#endif

								   /*		g_process_after_read  always happens now for MainObject processing
								   if ( ! g_process_after_read )    // Modification: 2015.12
								   {
								   // Analyze and Count keywords in files
								   userIF->updateProgress("Performing files analysis and counting.......", false);
								   ProcessSourceList();
								   if (HasUserCancelled())
								   return 0;
								   }	// Modification: 2015.12
								   */
	if (duplicate_threshold >= 0.0)
	{		// Modified: 2015.12
		time(&time_end_files_analysis);

		// This will show count done
		userIF->updateProgress("Looking for duplicate files..................", false);

		FindDuplicateFiles(&SourceFileA, &duplicateFilesInA1, &duplicateFilesInA2);
		if (HasUserCancelled())
			return 0;

		// Include this small time as part of Duplicate checks	 0 to 1 second (rounded)
		UpdateCounterCounts(CounterForEachLanguage, &SourceFileA, true, false);

		time(&time_end_find_duplicates);
	}
	else    // Modified: 2015.12
	{
		// Include this small time as part of Analysis	0 to 1 second (rounded)
		UpdateCounterCounts(CounterForEachLanguage, &SourceFileA, true, false);

		time(&time_end_files_analysis);
	}    // Modified: 2015.12

	if (HasUserCancelled())
		return 0;

	// print the counting results (SLOC counts and complexity counts)
	userIF->updateProgress("Generating results to files........", false);    // Modification: 2011.05  2015.12

	if (print_unified)
		PrintTotalCountResults(CounterForEachLanguage, true, "", &duplicateFilesInA2);    // Modified: 2015.12
	else
		PrintCountResults(CounterForEachLanguage, true, "", &duplicateFilesInA2);    // Modified: 2015.12

																					 // Show a change to keep the User happy.    Modified: 2015.12
	userIF->updateProgress("......", false);

	if (print_cmplx)
		PrintComplexityResults(CounterForEachLanguage);    // Modified: 2015.12

	if (duplicate_threshold >= 0.0)
	{
		if (print_unified)
			PrintTotalCountResults(CounterForEachLanguage, true, "Duplicates-", &duplicateFilesInA2, false);    // Modified: 2015.12
		else
			PrintCountResults(CounterForEachLanguage, true, "Duplicates-", &duplicateFilesInA2, false);    // Modified: 2015.12

		if (print_cmplx)
			PrintComplexityResults(CounterForEachLanguage, true, "Duplicates-", true);    // Modified: 2015.12
		PrintDuplicateSummary();
	}
#ifndef QTGUI
	userIF->updateProgress("DONE");     // Modified: 2015.12
#endif

	time(&time_end_print_results);    // Modified: 2015.12

	return 1;
}

/*!
* 1.Function Description:
Parses the command line arguments.Return value is method status
*
* 2.Parameters:
*    argc: number of arguments
*    argv: argument list
*
* 3.Creation Time And Owner:
*	 Version: 2009.01
*    Revised: 2015.12  threads, nowarnings, nouncounted
*/
int MainObject::ParseCommandLine(int argc, char *argv[])
{
	int i;  // Modification: 2011.10
	string arg, cmdStr;

	// capture command line and strip executable path
	arg = argv[0];
	cmdStr = CUtil::ExtractFilename(arg);
	for (i = 1; i < argc; i++)
	{
		arg = argv[i];
		if (arg.find(' ') != string::npos)	// quote strings containing spaces
			cmdStr += " \"" + arg + "\"";
		else
			cmdStr += " " + arg;
	}
	cmdLine = cmdStr;   // Modification: 2011.10

						// get the output directory first for error logging
	for (i = 1; i < argc; i++)  // Modification: 2013.04
	{
		arg = argv[i];

		if (arg == "-outdir")
		{
			// extract the output directory name
			if (i + 1 >= argc)
			{
				string err = "Error: -outdir  Directory name is missing";
				userIF->SetErrorFile("");
				userIF->AddError(err);
				return 0;
			}
			i++;
			outDir = argv[i];

			// attempt to create the specified directory (if exists, this will do nothing)
#ifdef UNIX
			// trim trailing '/' character
			if (outDir.length() > 1 && outDir[outDir.length() - 1] == '/')
				outDir = outDir.substr(0, outDir.length() - 1);
#else
			// trim trailing '\' or '/' character
			if (outDir.length() > 1 && (outDir[outDir.length() - 1] == '\\' || outDir[outDir.length() - 1] == '/'))
				outDir = outDir.substr(0, outDir.length() - 1);
#endif
			if (CUtil::MkPath(outDir) == 0)
			{
				string err = "Error: -outdir  Unable to create output directory (";
				err += outDir;
				err += ")";
				userIF->SetErrorFile("");
				userIF->AddError(err);
				return 0;
			}

			// add trailing slash and initialize the error log file path
#ifdef UNIX
			outDir.append("/");
#else
			outDir.append("\\");
#endif
			userIF->SetErrorFile(outDir);
			break;
		}
	}

	// make sure error path is set even if outDir is not
	if (outDir.empty())
		userIF->SetErrorFile("");

	for (i = 1; i < argc; i++)  // Modification: 2011.05
	{
		arg = argv[i];

		if (arg == "-v")
		{
			string err = "UCC version ";
			err += PRODUCT_REVISION;
			err += "\n";
			userIF->AddError(err);
			exit(0);
		}
		else if (arg == "-dir")
		{
			// read the two baseline directory names and the extract the names
			// of the file extensions to be searched
			if ((isDiff && i + 3 > argc) || (!isDiff && i + 2 > argc))
			{
				string err = "Error: -dir  Not enough directories given";
				userIF->AddError(err);  // Modification: 2013.04
				return 0;   // Modification: 2013.04
			}

			// extract the directory names for BaselineA and BaselineB (if applicable)
			use_CommandLine = true; // Modification: 2011.05
			i++;
			dirnameA = argv[i];
			i++;
			// Bug fix #2: add message "Error: Invalid -dir specification" if the specified 
			// -dir directory does not exist
			struct stat info;

			if ((stat(dirnameA.c_str(), &info) != 0) || !(info.st_mode & S_IFDIR))    // Modification: 2015.12
			{
				string err = "Error: -dir  Invalid Baseline A directory: (";
				err += dirnameA;
				err += ") either not found or not a directory.";
				userIF->AddError(err);
				return 0;
			}
			if (isDiff)
			{
				dirnameB = argv[i];

				// Directory B path/name must be different from A
				if (dirnameB.compare(dirnameA) == 0)
				{
					string err = "Error: -dir  Baseline B directory must be different from Baseline A";
					userIF->AddError(err);
					return 0;
				}
				i++;

				// Bug fix #2: add message "Error: Invalid -dir specification" if the specified 
				// -dir directory does not exist for the second directory as well if isDiff is set
				if ((stat(dirnameB.c_str(), &info) != 0) || !(info.st_mode & S_IFDIR))    // Modification: 2015.12
				{
					string err = "Error: -dir  Invalid Baseline B directory: (";
					err += dirnameB;
					err += ") either not found or not a directory.";
					userIF->AddError(err);
					return 0;
				}
			}

			// extract the names of the all file extensions to be searched
			for (; i < argc; i++)
			{
				// command line arguments can be mentioned in any sequence
				// search until you do not come across the end of arguments or
				// another command line argument
				arg = argv[i];
				if (arg.find_first_of('-') == 0)	// '-' indicates a new argument
					break;
				else
					listFilesToBeSearched.push_back(arg);
			}
			if (listFilesToBeSearched.size() == 0)
			{
				// add all files as the default
				listFilesToBeSearched.push_back("*.*");
			}
		}

		// Return from the loop if all arguments have been processed.
		if (i == argc)
			return 1;

		if (arg == "-i1")
		{
			/*Bug fix #11: UCC crashes when running option -i1 and -i2 without customized filename
			*                         Changed the condition from (i+1 > argc) to (i+1 >= argc) in order for it to check if customized name is provided. */

			// extract the BaselineA file
			if (i + 1 >= argc)
			{
				string err = "Error: -i1  Baseline A file name missing";
				userIF->AddError(err);
				return 0;
			}
			i++;

			ifstream infile;
			infile.open(argv[i], ifstream::in);
			infile.close();
			if (infile.fail())
			{
				// file did not exist
				string err = "Error: -i1  Unable to open Baseline A file (";
				err += argv[i];
				err += ")";
				userIF->AddError(err);
				return 0;
			}
			BaselineFileName1 = string(argv[i]);
		}
		else if (arg == "-i2" && isDiff)
		{
			/*Bug fix #11: UCC crashes when running option -i1 and -i2 without customized filename
			*                         Changed the condition from (i+1 > argc) to (i+1 >= argc) in order for it to check if customized name is provided. */

			// extract the BaselineB file
			if (i + 1 >= argc)
			{
				string err = "Error: -i2  Baseline B file name missing";
				userIF->AddError(err);  // Modification: 2013.04
				return 0;   // Modification: 2013.04
			}
			i++;    // Modification: 2011.05

			ifstream infile;
			infile.open(argv[i], ifstream::in);
			infile.close();
			if (infile.fail())
			{
				// file did not exist
				string err = "Error: -i2  Unable to open Baseline B file (";
				err += argv[i];
				err += ")";
				userIF->AddError(err);  // Modification: 2013.04
				return 0;   // Modification: 2013.04
			}
			BaselineFileName2 = string(argv[i]);
		}
		else if (arg == "-outdir")
		{
			// already processed so increment for the output directory name
			i++;
		}
		else if (arg == "-extfile")
		{
			// read the user defined language extension map
			if (i + 2 > argc)
			{
				string err = "Error: -extfile  File name missing";
				userIF->AddError(err);  // Modification: 2013.04

										// Call Help
				ShowUsage("-extfile", false);
				return 0;   // Modification: 2013.04
			}
			i++;
			userExtMapFile = argv[i];
		}
		else if (arg == "-filespecs")
		{
			//check file is present in the filepsecs
			if (i + 2 > argc)
			{
				string err = "Error: -extfile  File name missing";
				userIF->AddError(err);  // Modification: 2016.03
										// Call Help
				return 0;   // Modification: 2016.03
			}
			//read the file from the readile object
			ifstream readFile;
			const string filespec;
			//increment the argument.
			i++;
			//specify the file name in the text file
			std::ifstream infile(argv[i]);
			std::string line;
			//reading line by line
			while (std::getline(infile, line))
			{
				std::istringstream iss(line);
				string a;
				if (!(iss >> a)) { break; }// error
										   //triming the extra space
				a = CUtil::TrimString(a);
				//pushing the all the filetypes into list object
				listFilesToBeSearched.push_back(a);
			}
		}
		else if (arg == "-t")
		{
			// add message "Error: Invalid usage of -t" if option -t exsits without option -d
			if (!isDiff) {
				string err = "Error: -t  Invalid usage: -d  must be specified as well";
				userIF->AddError(err);
				return 0;
			}
			// add message "Error: Invalid usage of -t" if option -t exsits without the specified modification threshold number
			if (i + 2 > argc) {
				string err = "Error: -t  Threshold number missing";
				userIF->AddError(err);
				return 0;
			}
			else
			{
				i++;
				double myMatch = atof(argv[i]);
				if (myMatch >= 0 && myMatch <= 100) {
					match_threshold = myMatch;
				}
				else {
					string err = "Error: -t  Difference threshold given must be an integer between 0 and 100.";
					userIF->AddError(err);
					return 0;
				}
			}
		}
		// check to make sure -tdup has a value
		else if (arg == "-tdup")
		{
			if (i + 1 >= argc)
			{
				string err = "Error: -tdup  Duplicate threshold value missing. Must be between 0 and 100.";
				userIF->AddError(err);
				return 0;
			}
			else
			{
				i++;
				if (duplicate_threshold >= 0.0)
				{
					double myDup = atof(argv[i]);
					if (0.0 <= myDup && myDup <= 100.0)
					{
						duplicate_threshold = myDup;
					}
					else
					{
						string err = "Error: -tdup  Duplicate threshold given must be between 0 and 100.";
						userIF->AddError(err);
						return 0;
					}
				}
			}
		}
		else if (arg == "-nodup")
		{
			duplicate_threshold = -1.0;
		}

		// Bug fix #5 - Checking if trunc has a value
		else if (arg == "-trunc")
		{
			if (i + 1 >= argc)
			{
				string err = "Error: -trunc  Truncation value missing";
				userIF->AddError(err);
				return 0;
			}
			else
			{
				i++;
				int myTrunc = atoi(argv[i]);
				if (myTrunc >= 0)
					lsloc_truncate = (size_t)myTrunc;
				else
				{
					string err = "Error: -trunc  Truncation value must be 0 or more.";
					userIF->AddError(err);
					return 0;
				}
			}
		}
		else if (arg == "-d" && isDiff)
		{
			// ignore
		}
		else if (arg == "-cf")
		{
			// support ClearCase files
			clearCaseFile = true;
		}
		else if (arg == "-unified")
		{
			print_unified = true;
		}
		else if (arg == "-ascii")
		{
			print_csv = false;
			if (!print_legacy)
				print_ascii = true;
			else
				print_ascii = false;
		}
		else if (arg == "-legacy")
		{
			print_csv = false;
			print_ascii = false;
			print_legacy = true;
		}
		else if (arg == "-nocomplex")
		{
			print_cmplx = false;
		}
		else if (arg == "-cc4enable")
		{
			// depends on print_cmplx being true to actually work
			g_cc4enable = true;
		}
		else if (arg == "-nolinks")
		{
#ifdef UNIX // Modification: 2011.10
			// disable following symbolic links
			followSymLinks = false;
#else
			// Not supported.  Set reasonable behavior and tell User.
			// disable following symbolic links
			followSymLinks = false;
			string info = "Information: -nolinks  feature is not available.";
			userIF->AddError(info);
#endif
		}
		else if (arg == "-visualdiff") {    // Modification: 2014.08
											// enables output of visual differencing in diff_dump.txt and highlighted_diff.html
			visualDiff = true;
		}
		/*  COMMENTED OUT but kept if we want to have somethin like it later.  Stack Dump is available  2015.10.28
		else if (arg == "-trace")  // EXPERIMENTAL !  2015.10.07
		{
		// enable trace option
		traceException = true;
		}
		*/
		else if (arg == "-threads")
		{
			if (i + 1 > argc)
			{
				string err = "Error: -threads  Number of worker threads to use is missing.";
				userIF->AddError(err);
				return 0;
			}
			else
			{
				i++;
				int myThreads = 0;
#ifdef	ENABLE_THREADS
				myThreads = atoi(argv[i]);
				if (myThreads < MIN_UCC_THREAD_COUNT)
					myThreads = MIN_UCC_THREAD_COUNT;

				if (myThreads > MAX_UCC_THREAD_COUNT)
					myThreads = MAX_UCC_THREAD_COUNT;
#endif
				workThreadsCount = static_cast<unsigned int>(myThreads);
			}
		}
		else if (arg == "-ramlimit")
		{
			if (i + 1 > argc)
			{
				string info = "Information: -ramlimit  Number of 100 MegaByte increments (1 to 5120) is missing, using 500 MegaBytes.";
				userIF->AddError(info);
				ramLimit = DEFAULT_UCC_RAM_LIMIT;
			}
			else
			{
				i++;
				int myRamLimit = atoi(argv[i]);
				if (myRamLimit < MIN_UCC_RAM_LIMIT)
					myRamLimit = MIN_UCC_RAM_LIMIT;

				if (myRamLimit > MAX_UCC_RAM_LIMIT)
					myRamLimit = MAX_UCC_RAM_LIMIT;

				ramLimit = static_cast<unsigned int>(myRamLimit);
			}
		}
		else if (arg == "-nowarnings")
		{
			g_no_warnings_to_UI = true;
		}
		else if (arg == "-nouncounted")
		{
			g_no_uncounted = true;
		}
		else if (arg == "-header")   // Modification: 2016.12
		{
			// Adding options to support -header
			if (i + 1 >= argc)
			{
				string info = "Information: -header  Header file missing. Using default header for output.";
				userIF->AddError(info);
			}
			else
			{
				i++;
				userHeaderFile = argv[i];
				int retval = 0;
				struct stat info;
				retval = stat(userHeaderFile.c_str(), &info);
				// check if the suggested file is not a directory and is not empty
				if ((retval != 0) || (info.st_mode & S_IFDIR) || (info.st_size == 0))    
				{
					string info = "Information: -header Invalid header file path or the file is empty. Using default header for output.";
					userIF->AddError(info);
					userHeaderFile = "";
				}
			}
		}
		else if (arg == "-noheader")   // Modification: 2016.12
		{
			// Remove any header from the output document
			remove_Header = true; 
		}
                else if( arg == "-funcDiff" ) //Modification 2017.02
                {
                    //Ignore
                }
		else
		{
			// Let User know unable to recognize this arg
			string err = "Error: " + arg;
			err += " is not a valid UCC command line argument.";
			userIF->AddError(err);
			return 0;
		}
	}

	// Call here so some parser classes values can be set from Globals set here
	ResetCounterCounts(CounterForEachLanguage);    // Modification: 2015.12
	return 1;
}

/*!
* 1.Function Description:
*	 Shows program usage and terminates application.
*
* 2.Parameters:
*    option: usage option (default="")
*    do_exit: exit after display (default=true)
*
* 3.Creation Time And Owner:
*	 Version: 2011.10
*    Revised: 2015.12  Threads, nowarnings, nouncounted, ramlimit, cc4enable, changed to make a string
*/
void MainObject::ShowUsage(const string &option, bool do_exit, string * outMsg)
{
	string msg;
	if (option == "-v") // Modification: 2011.10
	{
		msg += "Usage: ucc -v\n\n";
		msg += " -v: Displays the current version of UCC being executed\n";
	}
        else if (option == "-d" || option == "-i1" || option == "-i2" || option == "-t" || option == "-funcDiff") //Modification: 2017.02
	{
		msg += "Usage: ucc -d [-i1 <fileListA>] [-i2 <fileListB>] [-t <#>]\n\n";
		msg += " -d: Enables the differencing function. If not specified, only the\n";
		msg += "     counting functions will execute.\n\n";
		msg += " Related Options:\n\n";
		msg += "  -i1 <fileListA>: File containing a list of files to be used as\n";
		msg += "                   Baseline A for counting or comparison if -d is specified.\n";
		msg += "                   If -i1 is not specified, the file 'fileListA.txt' will be\n";
		msg += "                   used as the default if -d is specified, and the file\n";
		msg += "                   'fileList.txt' will be used as the default without -d.\n\n";
		msg += "  -i2 <fileListB>: File containing a list of files to be be used as\n";
		msg += "                   Baseline B for comparison to Baseline A.\n";
		msg += "                   If -i2 is not specified, the file 'fileListB.txt' will be\n";
		msg += "                   used as the default if -d is specified.\n\n";
		msg += "  -t <#>:          Specifies the percentage of common characters between two\n";
		msg += "                   lines of code that are being compared in order to determine\n";
		msg += "                   whether the line is modified or replaced. If the percentage of\n";
		msg += "                   common characters between the compared lines is greater than\n";
		msg += "                   the specified threshold, the line is considered replaced and\n";
		msg += "                   will be counted as one line deleted and one line added.\n";
		msg += "                   Otherwise, it will be counted as one modified line.\n";
		msg += "                   The valid range is 0 to 100 and defaults to 60.\n";
		msg += "  -visualdiff:     Enables visual differencing. This causes differences between\n";  // Modification: 2014.08s
		msg += "                   baselines to be logged in diff_dump.txt and highlighted_diff.html\n"; // Modification: 2014.08s
		msg += "                   for future visual inspection.  Not supported if -threads used.\n";    // Modification: 2015.12
                msg += "  -funcDiff:       Enables function level differencing. This option works only when -d is enabled.\n";  //Modification: 2017.02
	}
	else if (option == "-tdup") // Modification: 2011.10
	{
		msg += "Usage: ucc -tdup <#>\n\n";
		msg += " -tdup <#>: Specifies the percentage of logical source lines of code (LSLOC)\n";
		msg += "            that have changed between two files of the same name in order to\n";
		msg += "            determine whether the files are duplicates. If the percentage of\n";
		msg += "            common LSLOC between two files is less than or equal to the\n";
		msg += "            specified threshold, the files are considered duplicates.\n";
		msg += "            This method compares LSLOC similar to the differencing function\n";
		msg += "            and ignores formatting including blank lines and comments.\n";
		msg += "            Note that files of different names may be checked for an exact\n";
		msg += "            physical match. The valid range is 0 to 100 and defaults to 0.\n";
	}
	else if (option == "-trunc")
	{
		msg += "Usage: ucc -trunc <#>\n\n";
		msg += " -trunc <#>: Specifies the maximum number of characters allowed in a logical\n";
		msg += "             source line of code (LSLOC). Any characters beyond the specified\n";
		msg += "             threshold will be truncated and ignored when compared.\n";
		msg += "             If the truncation is disabled by setting the threshold to 0\n";
		msg += "             or the threshold is set too high, very long LSLOC may significantly\n";
		msg += "             degrade performance.\n";
	}
	else if (option == "-cf")
	{
		msg += "Usage: ucc -cf\n\n";
		msg += " -cf: Indicates that the target files were retrieved from\n";
		msg += "      IBM Rational ClearCase. ClearCase appends information at the end\n";
		msg += "      of file names beginning with '@@'. Use of this option strips\n";
		msg += "      all characters after the last '@@' sequence from the file name.\n";
	}
	else if (option == "-dir")
	{
		msg += "Usage: ucc -dir <dirA> [<dirB>] <fileSpecs>\n\n";
		msg += " -dir: Specifies the directories and file types to be searched for files\n";
		msg += "       to be counted or compared. The directories dirA and dirB indicate\n";
		msg += "       the directories to be searched for each baseline. The fileSpecs indicate\n";
		msg += "       the file type specifications (typically containing search wildcards).\n";
		msg += "       The specified directories will be searched recursively.\n\n";
		msg += " Option Parameters:\n\n";
		msg += "    <dirA>:      Specifies the directory of Baseline A to be searched for files\n";
		msg += "                 to be counted or compared.\n\n";
		msg += "    <dirB>:      If the -d option is specified, this specifies the directory\n";
		msg += "                 of Baseline B to be searched for files to be compared.\n\n";
		msg += "    <fileSpecs>: One or more specification of file types to be included\n";
		msg += "                 in the file search. Each file specification is separated\n";
		msg += "                 by whitespace and typically contains search wildcards.\n";
		msg += "                 For example:\n";
		msg += "                   *.cpp *.h *.java\n";
	}
	else if (option == "-outdir")
	{
		msg += "Usage: ucc -outdir <outDir>\n\n";
		msg += " -outdir <outDir>: Specifies the directory where output files will be written.\n";
		msg += "                   If this is not specified, the output files will be written\n";
		msg += "                   to the working directory by default. This option prevents\n";
		msg += "                   overwriting output files from multiple runs and allows for\n";
		msg += "                   batch execution and output organization.\n";
	}
	else if (option == "-extfile")
	{
		msg += "Usage: ucc -extfile <extFile>\n\n";
		msg += " -extfile <extFile>: Specifies a file containing user specified file extensions\n";
		msg += "                     for any of the available language counters. Any language\n";
		msg += "                     counter specified in this file will have its associated\n";
		msg += "                     extensions replaced. If a language is specified with no\n";
		msg += "                     extensions, the language counter will be disabled.\n";
		msg += "                     The file format contains a single line entry for each\n";
		msg += "                     language. Single or multi-line comments may be included\n";
		msg += "                     with square brackets []. For example:\n\n";
		msg += "                       C_CPP = *.cpp, *.h  [C/C++ extensions]\n\n";

		// List of Language names (1 Language per line) with extensions for each language
		msg += "        Here is a list of languages with file extension(s) for each.\n";
		GetLanguagesAndExtensions(CounterForEachLanguage, beginLangsExtens);
		msg += beginLangsExtens + "\n";

#ifdef	_DEBUG
		msg += "UNDEF and WEB show for developers but can not be changed using -extfile\n\n";
#endif

		// Sorted list of Extensions (1 Extension per line) with related Language name(s)
		msg += "        Here is a list of file extensions with the language for each.\n";
		string	extensionsAndLangs;
		bool	twoLangsPerExt = false;
		GetExtensionsAndLanguages(CounterForEachLanguage, extensionsAndLangs, twoLangsPerExt);
		msg += extensionsAndLangs + "\n";
		if (twoLangsPerExt)
		{
			msg += "\nNote: The same file extension has more than 1 language above.\n";
			msg += "To remove it for those languages you do not want (for example):\n";
			msg += "Suppose you have .txt files and you don't want X-Midas to run.\n";
			msg += "In the extension file you have a line like:\n";
			msg += "X-Midas=\n";
			msg += "Which will disable X-Midas parsing of .txt files.\n";
		}

		if (outMsg)
		{
			*outMsg = msg;
			return;
		}
		else
		{
			if (userIF)
				userIF->AddError(msg);
			else
				printf("%s", msg.c_str());
		}

#ifdef	QTGUI
		return;
#else
		exit(1);	// Exit here after showing the list.
#endif

	}
	else if (option == "-unified")
	{
		msg += "Usage: ucc -unified\n\n";
		msg += " -unified: Prints language report files to a single unified report file.\n";
		msg += "           The results are written to 'TOTAL_outfile.csv' or 'TOTAL_outfile.txt'.\n";
		msg += "           In the absence of this option, results for each language are written\n";
		msg += "           to separate files.\n";
	}
	else if (option == "-ascii")
	{
		msg += "Usage: ucc -ascii\n\n";
		msg += " -ascii: Prints ASCII text (*.txt) report files instead of CSV (*.csv) files.\n";
		msg += "         The content of the ASCII format is identical to the CSV format.\n";
	}
	else if (option == "-legacy")
	{
		msg += "Usage: ucc -legacy\n\n";
		msg += " -legacy: Prints legacy formatted ASCII text report files instead of\n";
		msg += "          the current format of the CSV or text files. The purpose of this\n";
		msg += "          option is to maintain backward compatibility with some older\n";
		msg += "          UCC results post-processing software systems.\n";
	}
	else if (option == "-nodup")
	{
		msg += "Usage: ucc -nodup\n\n";
		msg += " -nodup: Prevents separate processing of duplicate files. This avoids extra\n";
		msg += "         processing time to determine the presence of duplicate files within\n";
		msg += "         each baseline. With this option, all files will be counted and reported\n";
		msg += "         together, regardless of whether they are duplicates. Otherwise, files\n";
		msg += "         within a baseline will be checked for duplicates and results will\n";
		msg += "         be reported separately. Please see the user manual for details.\n";
	}
	else if (option == "-nocomplex")
	{
		msg += "Usage: ucc -nocomplex\n\n";
		msg += " -nocomplex: Disables printing of keyword counts and processing of complexity\n";
		msg += "             metrics. This can reduce processing time and limit reports.\n";
	}
	else if (option == "-cc4enable")
	{
		msg += "Usage: ucc -cc4enable\n\n";
		msg += " -cc4enable: Enables Cyclomatic Complexity 4 unique condition clauses\n";
		msg += "             metrics. This may increase processing time.\n";
	}
	else if (option == "-nolinks")
	{
#ifdef UNIX
		msg += "Usage: ucc -nolinks\n\n";
		msg += " -nolinks: Disables following symbolic links to directories and counting\n";
		msg += "           of links to files on Unix systems. This can prevent duplicate file counts.\n";
#else
		msg += "           is not available on this platform.\n";
#endif
	}
	else if (option == "-threads")
	{
#ifdef	ENABLE_THREADS
		msg += "Usage: ucc -threads <#>\n\n";
		msg += " -threads <#>    Specify the number of work threads. Minimum is 2.\n";
		msg += "                 If not specified then no work threads are created.\n";
#else
		msg += "           is not available.  Compile UCC with Boost thread library.\n";
#endif
	}
	else if (option == "-ramlimit")
	{
		msg += "Usage: ucc -ramlimit <#>\n\n";
		msg += " -ramlimit <#>   Specify RAM limit used for estimation of success\n";
		msg += "                 in 100 MB increments. Minimum is 1\n";
		msg += "                 and maximum is 5120.  10 will be about 1 GigaByte.\n";
		msg += "                 If not specified then default is 500 MBytes RAM.\n";
		msg += "                 If  unlikely to succeed  shows, UCC will try anyway.\n";
	}
	else if (option == "-nowarnings")
	{
		msg += "Usage: ucc -nowarnings\n\n";
		msg += " -nowarnings     Disables warning messages on console.\n";
		msg += "                 Warning messages will still be logged.\n";
		msg += "                 Error messages will still show on console.\n";
	}
	else if (option == "-nouncounted")
	{
		msg += "Usage: ucc -nouncounted\n\n";
		msg += " -nouncounted    Disables reports or messages about uncounted files.\n";
	}
	else if (option == "-noheader") // Modification 2016.12
	{
		msg += "Usage: ucc -noheader\n\n";
		msg += " -noheader    Removes the default header in reports.\n";
	}
	else if (option == "-header")  // Modification 2016.12
	{
		msg += "Usage: ucc -header <filename>\n\n";
		msg += " -header <filename>    Replaces the default header in the reports with contents\n";
		msg += "                       of the <filename>\n";
	}
	else if (option == "-h" || option == "-help")
	{
		msg += "Usage: ucc -help <option>\n\n";
		msg += " -help <option>: Without a specified option, this displays the list of command\n";
		msg += "                 line options. If a command line option is specified, detailed\n";
		msg += "                 usage information for the specified option is displayed.\n";
	}
	else
	{
		msg += "Usage: ucc [-v] [-d [-i1 fileListA] [-i2 <fileListB>] [-t <#>]] [-tdup <#>]\n";
		msg += "           [-trunc <#>] [-cf] [-dir <dirA> [<dirB>] <fileSpecs>]\n";
		msg += "           [-outdir <outDir>] [-extfile <extFile>] [-unified] [-ascii] [-legacy]\n";
		msg += "          [-filespecs <filespecs>] [-outdir <outDir>] [-extfile <extFile>] [-unified] [-ascii] [-legacy]\n";
#ifdef UNIX
		msg += "           [-nodup] [-nocomplex] [-nolinks] [-help [<option>]]\n\n";
#else
		msg += "           [-nodup] [-nocomplex] [-help [<option>]]\n\n";
#endif
		msg += "Options:\n";
		msg += " -v                 Lists the current version number.\n";
		msg += " -d                 Runs the differencing function.\n";
		msg += "                      If not specified, runs the counting function.\n";
                msg += " -funcDiff          Enables function level differencing. This option works only when -d is enabled.\n";  //Modification 2017.02
		msg += " -i1 <fileListA>    Filename containing filenames in the Baseline A.\n";
		msg += " -i2 <fileListB>    Filename containing filenames in the Baseline B.\n";
		msg += " -t <#>             Specifies the threshold percentage for a modified line.\n";
		msg += "                      (DEFAULTS TO 60).\n";
		msg += " -visualdiff        Enables visual differencing.\n";
		msg += " -tdup <#>          Specifies the threshold percentage for duplicated files -\n";
		msg += "                      the maximum percent difference between two files of the\n";
		msg += "                      same name in a baseline to be considered duplicates.\n";
		msg += "                      (DEFAULTS TO 0)\n";
		msg += " -trunc <#>         Specifies the maximum number of characters allowed in a\n";
		msg += "                      logical SLOC. Additional characters will be truncated.\n";
		msg += "                      (DEFAULTS TO 10,000, use 0 for no truncation)\n";
		msg += " -cf                Indicated that target files were retrieved from ClearCase.\n";
		msg += "                      Restored the original filename before counting.\n";
		msg += " -dir               Specifies the following directories and file specifications: \n";
		msg += "      <dirA>          Name of the directory containing source files.\n";
		msg += "                        If -d is given, dirA is the directory for Baseline A.\n";
		msg += "      <dirB>          Name of the directory for Baseline B only if -d is given.\n";
		msg += "      <fileSpecs>     File specifications, wildcard chars ? * are allowed.\n";
		msg += "                        For example, *.cpp *.h\n";
		msg += " -outdir <outDir>   Specifies the directory to store the output files.\n";
		msg += " -extfile <extFile> Indicates language extension mapping filename\n";
		msg += " -unified           Prints language report files to a single unified report file.\n";
		msg += " -filespecs <filespecs> Indicates file types to be serached for in the directories\n";
		msg += " -ascii             Prints ASCII text report files instead of CSV files.\n";
		msg += " -legacy            Prints legacy formatted ASCII text report files\n";
		msg += "                      instead of the current format of the CSV or text files.\n";
		msg += " -nodup             Disables separate processing of duplicate files.\n";
		msg += " -nocomplex         Disables printing complexity reports or keyword counts.\n";
		msg += " -cc4enable         Enables Cyclomatic Complexity 4 unique condition metrics.\n";
#ifdef UNIX
		msg += " -nolinks           Disables following symbolic links to directories and files.\n";
#endif
#ifdef	ENABLE_THREADS
		msg += " -threads <#>       Specify the number of work threads. Minimum is 2.\n";
		msg += "                      If not specified then no work threads are created.\n";
#endif
		msg += " -ramlimit <#>      Specify RAM limit used for estimation of success\n";
		msg += "                      in 100 MB increments. Minimum is 1\n";
		msg += "                      and maximum is 5120.  10 will be about 1 GigaByte.\n";
		msg += "                      If not specified then default is 500 MBytes RAM.\n";
		msg += "                      If  unlikely to succeed  shows, UCC will try anyway.\n";
		msg += " -nowarnings        Disables warning messages on console.\n";
		msg += "                      Warning messages will still be logged.\n";
		msg += "                      Error messages will still show on console.\n";
		msg += " -nouncounted       Disables reports or messages about uncounted files.\n";
		msg += " -header <filename> Replaces the default header in the reports with contents\n";
		msg += "                    of the <filename>\n";
		msg += " -noheader          Removes the default header in reports.\n"; // Modification 2016.12
		msg += " -help <option>     Displays this usage or usage for a specified option.\n";
	}

	if (outMsg)
	{
		*outMsg = msg;
		return;
	}
	else
	{
        /*		
		Modification Semester: Fall 2016		
			1> Added a check to make sure userIF exist before calling the AddError function.		
			2> Below Condition prevents segmentation fault and makes "UCC help" command to work perfectly.		
	   */
		if (userIF)
			userIF->AddError(msg);
		else
			printf("%s", msg.c_str());
	}

	if (do_exit)
	{
#ifndef	QTGUI
		exit(1);
#endif
	}

	return;
}


/*!
* 1.Function Description:
*	 Read user-defined language extension map file.
*
* 2.Parameters:
*    extMapFile: language extension map file
*
* 3.Creation Time And Owner:
*	 Version: 2011.05
*/
void MainObject::ReadUserExtMapping(const string &extMapFile)
{
	ifstream readFile;  // Modification: 2011.05
	string line, str;   // Modification: 2011.10
	string language;    // Modification: 2011.05
	string extension;
	int flag;
	string token;
	bool foundc = false;
	vector <string>*comment_delimiters = NULL;/*Vector to store the comment delimiters passed by the ext file */
	readFile.open(extMapFile.c_str(), ifstream::in);    // Modification: 2011.05
														//File open success
	if (readFile.is_open())
	{

		while (readFile.good() || readFile.eof())
		{
			getline(readFile, line);    // Modification: 2011.10
										//Read failed or reach the end of the file
			if ((!readFile.good() && !readFile.eof()) || (readFile.eof() && line.empty()))
				break;
			line = CUtil::TrimString(line);
			flag = 0;

			// process embedded, whole, or multi-line comments (delimited by [])

			if (!foundc)
			{
				size_t pos1=0, pos2=0;  // Modification: 2011.10
				/*Modification: 2016.10;USC
				 *Scan for the block comment delimiter */
				char c = line.at(0);
				if (c == 'b')
				{
					if (line.at(line.size() - 1) != '}')
					{
						userIF->AddError("Error: Please check the format of the input for block comments 1",false);
						return;
					}
					comment_delimiters = &block_delimiters;

					pos1 = line.find_first_of('{');
					pos2 = line.find_last_of('}');

					/*Modification: 2016.10;USC
					 *Check for the valid input string*/
					if ((pos1 == std::string::npos) || (pos2 == std::string::npos) || line.at(pos1-1) != ':')
					{
						userIF->AddError("Error: Please check the format of the input 2",false);
						return;
					}
				}			
				/*Modification: 2016.10;USC
				 *Scan for the line comment delimiter */
				if(c == 'l') 
				{
					if (line.at(line.size() - 1) != '}')
					{
						userIF->AddError("Error: Please check the format of the input 3",false);
						return;
					}
					comment_delimiters = &line_delimiters;
					pos1 = line.find_first_of('{');
					pos2 = line.find_last_of('}');

					/*Modification: 2016.10;USC
					 *Check for the valid input string*/
					if ((pos1 == std::string::npos) || (pos2 == std::string::npos) || line.at(pos1 - 1) != ':')
					{
						userIF->AddError("Error: Please check the format of the input 4",false);
						return;
					}
				}

				
				
				if (pos1 > 0 && pos2 > 0 && pos1< line.size() && pos2<line.size() && comment_delimiters != NULL)
				{
					comment_delimiters->clear();
					string delimiter = line.substr(pos1+1, pos2-pos1-1);
					size_t i = 0; // Modification: 2018.01 : changed data type to size_t
					string *s = new string();
                    
                    while (i<delimiter.length())
					{
						if (delimiter.at(i) != ',')
						{
							char c = delimiter.at(i);
                            
                            if (c == '\\' && (i + 1)<line.size())
							{
								/*Modification: 2016.10;USC
								 *Check for the escape sequence in the input string*/
								if (delimiter.at(i + 1) == '{' || delimiter.at(i + 1) == '}')	
								{
									c = delimiter.at(i + 1);
									s->append(1, c);
									i++;
								}
							}
							else
							{
								s->append(1, c);
							}
						
						}
						else
						{
							/*Modification: 2016.10;USC
							*Add the delimiters in the specific vectors*/
							const char* z = s->c_str();
							comment_delimiters->push_back(z);
							s = new string();
						}
						i++;
					}
					if (s->size() > 0)
					{
						const char* z = s->c_str();
						comment_delimiters->push_back(z);
						s = new string();
					}

			//	foundc = true;
				}

			}			
			//add the relationship between the user-defined language and extension to the map
			if (line.size() > 0)
			{
				istringstream iss(line);
				while (getline(iss, token, '='))
				{
					if (flag == 0)
					{
						language = CUtil::TrimString(CUtil::ToLower(token));
						flag = 1;
					}
					else
					{
						extension = CUtil::TrimString(CUtil::ToLower(token));
						flag = 0;
					}
				}
				LanguageExtensionMap.insert(std::pair<std::string, std::string>(language, extension));
			}
			if (!readFile.good())
				break;

			extension = "";		// clear to prevent mixing wrong tokens to following languages
		}
	}
    //Warnings fix 11.25.16. DO NOT USE THESE VARIABLES AFTER THIS.
    //(void)pos1;
    //(void)pos2;

	readFile.close();
	CreateExtMap();
}

/*!
* 1.Function Description:
*	 Create user-defined language extension map.
*
* 2.Parameters:
*
* 3.Creation Time And Owner:
*	 Version: 2011.05
*	 Revised: 2015.12  Added support so Thread structs are changed as well.
*/
void MainObject::CreateExtMap()
{
	int i;									// Modification: 2011.10
	unsigned int	j = 0;
	string token, lang_name;					// Modification: 2011.05
	bool found;
	CCodeCounter *langCounter;
	StringVector *webExten;

	// Added support to also change Thread structs   start Modification: 2015.12
	//
	vector<CounterForEachLangType *> pCountersLangs;
	pCountersLangs.push_back(&CounterForEachLanguage);   // Main thread Lang class parser pointers struct

#ifdef	ENABLE_THREADS
	if (workThreadsCount)
	{
		for (unsigned int k = 0; k < workThreadsCount; k++)
			pCountersLangs.push_back(GetThread_pCounterForEachLanguage(k));
	}
#endif

	unsigned int updateCount = pCountersLangs.size();
	// NOT going to indent below even more, just letting you know
	for (unsigned int updateIdx = 0; updateIdx < updateCount; updateIdx++)
	{
		CounterForEachLangType * myCounterForEachLanguage = pCountersLangs[updateIdx]; // end Modification: 2015.12

																					   // Code changed below where it used to be:  CounterForEachLanguage  is now:  (*myCounterForEachLanguage)

																					   // iterate all the elements in the LanguageExtensionMap,put them in the langCounter's file_extension vector.
		CWebCounter *webCounter = (CWebCounter *)((*myCounterForEachLanguage)[WEB]);    // Modification: 2015.12
		unsigned int web_lang_names_size = (unsigned int)(webCounter->web_lang_names.size());
		bool updateWeb = false;
		for (std::map< std::string, std::string >::const_iterator iter = LanguageExtensionMap.begin();  // Modification: 2013.04
			iter != LanguageExtensionMap.end(); ++iter)
		{
			// check for a counter for the specified language
            //Modification: 11.2016 Ext-4 Starts
            string name = iter->first;                
			lang_name = CUtil::TrimString(CUtil::ToLower(name));
            ///Modification: 11.2016 Ext-4 Ends

			langCounter = NULL;
			for (i = 0; i < (int)((*myCounterForEachLanguage).size()); i++)    // Modification: 2015.12
			{
				if ((*myCounterForEachLanguage)[i]->classtype == WEB)
				{
					found = false;
					for (j = 0; j < web_lang_names_size; j++)
					{
						if (lang_name.compare(CUtil::ToLower(webCounter->web_lang_names[j])) == 0)
						{
							langCounter = (*myCounterForEachLanguage)[i];    // Modification: 2015.12
							updateWeb = true;
							found = true;
							break;
						}
					}
					if (found)
						break;
				}
				else if (lang_name.compare(CUtil::ToLower((*myCounterForEachLanguage)[i]->language_name)) == 0)    // Modification: 2015.12
				{
					langCounter = (*myCounterForEachLanguage)[i];// Modification: 2015.12


					break;
				}
			}
			if (langCounter != NULL)    // Modification: 2011.05
			{
				if (langCounter->classtype == WEB)  // discriminate each kind of web language
				{
					if (lang_name == "asp")
						webExten = &(webCounter->file_exten_asp);
					else if (lang_name == "jsp")
						webExten = &(webCounter->file_exten_jsp);
					else if (lang_name == "php")
						webExten = &(webCounter->file_exten_php);
					else if (lang_name == "coldfusion")
						webExten = &(webCounter->file_exten_cfm);
					else if (lang_name == "xml")    // Modification: 2011.10
						webExten = &(webCounter->file_exten_xml);
					else    // Modification: 2011.05
						webExten = &(webCounter->file_exten_htm);

					webExten->clear();						// optional line if existing map is to be erased
					istringstream iss(iter->second);
					while (getline(iss, token, ','))
						webExten->push_back(CUtil::TrimString(token));
				}
				else
				{
					langCounter->file_extension.clear();	// optional line if existing map is to be erased
					istringstream iss(iter->second);
                    //Modification: 11.2016 Ext-4
                    StringVector tempVector;
                    
					while (getline(iss, token, ','))
					{
						langCounter->file_extension.push_back(CUtil::TrimString(token));
                        //Modification: 11.2016 Ext-4
                        tempVector.push_back(CUtil::TrimString(token));					
					}
                    //Modification: 11.2016 Ext-4 starts
                    std::map<string, StringVector>::iterator it = dictCopy.find(name);
					if (it != dictCopy.end()) {
						dictCopy.erase(it);
					}
                    
                    dictCopy.insert(std::pair<string, StringVector>(name, tempVector));
                    //Modification: 11.2016 Ext-4 ends
                    
                    /*Modification: 2016.10;USC, 
					*Add the line delimiters to the language counter*/
                    //Modification: 2018.01: USC ; Changed data type from int to size_t
                    for (size_t i = 0;i < line_delimiters.size();i++)
					{
						/*char c = line_delimiters[i];
						string* s = new string(1, c);
						const char* z = s->c_str();*/
						langCounter->LineCommentStart.push_back(line_delimiters[i]);
					}

                    /*Modification: 2016.10;USC
					*Add the block delimiters to the language counter*/
                    size_t i = 0; //Modification: 2018.01: USC ; Changed data type from int to size_t
                    for ( i = 0; i < block_delimiters.size(); i++)
					{
						/*char c = block_delimiters[i];
						string* s = new string(1, c);
						const char* z = s->c_str();*/
						if(i%2 == 0)
						    langCounter->BlockCommentStart.push_back(block_delimiters[i]);
						else
							langCounter->BlockCommentEnd.push_back(block_delimiters[i]);
					}
					if (i % 2 != 0)
					{
						userIF->AddError("Error: Please specify the starting and ending block comments in pair", false);
						return;
					}
				}
			}
			else
			{
				string err = "Error: ";
				err += iter->first;
				err += " is not a supported language as specified in the language extension file";
			//	userIF->AddError(err, false, 1);    // Modification: 2011.10
			}
		}

		// update web extensions if changed
		if (updateWeb)  // Modification: 2011.05
			webCounter->UpdateWebFileExt();

	}    // END of loop to support all Threads    Modification: 2015.12
}


/*!
* 1.Function Description:
*	 Enable Optimizations.  Start worker Threads if wanted.
*
* POST CONDITION:  Caller must check start_result_msg
*
* 2.Parameters:
* \Global g_process_after_read            IN/OUT   Set to true so will do Counting right after a file is read
* \Global g_discard_lines_after_process   IN/OUT   Set if OK to discard the lines after detailed Counts done
* \Global workThreadsCount                IN/OUT   Try to start this number of threads.  May be changed.
*         start_result_msg:               IN/OUT   Message holding results of starting threads
* Returns: possibly not empty start_result_msg string
*
* 3.Creation Time And Owner:
*	 Version: 2015.12  Created
*/
void MainObject::StartThreads(string & start_result_msg)
{
	start_result_msg = "";

	// Set back Defaults
	g_process_after_read = false;
	g_discard_PHY_lines_after_process = false;
	g_discard_lines_after_process = false;

	// Maybe set more flags for Optimization of processing steps
	if (isDiff)
	{
		// Called from DiffTool
		// Although this is set true here, DIFF uses Delay Read approach
		// so first time file is accessed just other file details are found
		// and Reading the Physical lines into RAM happens later (on demand Reading).
		// This is critical to having minimum RAM usage for Differencing.
		g_process_after_read = true;

		// Diff downstream processing can be multithreaded ! ! !
		// Solved: see DiffTool.cpp code
		// But any cleanup of Physical or Logical source lines
		// happens after DIFF and not directly after Counting/Analysis.
		// Exact cleanup after DIFF depends on if Dup checking happens later.
	}
	else
	{
		// Called from MainObject
		g_process_after_read = true;

		if (g_process_after_read)
		{
			if (!isDup)
			{
				g_discard_lines_after_process = true;	// OK to discard, not doing Duplicate checking
			}
			else
			{	// Duplicate checking afterwards
				// g_discard_PHY_lines_after_process = true;	// Dup checks do need Physical source lines
			}
		}
	}

	if (workThreadsCount >= MIN_UCC_THREAD_COUNT)
	{
		string msg_result;	// This will hold details from starting threads

		int start_result = StartWorkerThreads(&(CounterForEachLanguage), userIF, workThreadsCount, msg_result);
		if (THREADS_STARTED_OK != start_result)
		{
			if (0 > start_result)
			{
				// There was an Error starting threads.  Add to Error message.
				msg_result += "Running without any extra threads.\n";
				workThreadsCount = 0;
			}
			else
			{
				// Number of threads started is less than wanted.
				// Continue running with these fewer threads.
				// Message already done.
				workThreadsCount = (unsigned int)start_result;
			}
		}
		else
		{
			// Message that wanted number of threads started OK.
			IntToStr((int)workThreadsCount, msg_result);
			msg_result += " Threads started.\n";
		}
		start_result_msg = msg_result;
	}
}


/*!
* 1.Function Description:
*    Finds more files to process if given an input file list.
*    May show list of Files found before detailed processing.
*    Estimates RAM needed and gives Information if over ramlimit amount, UCC still continues.
*    Display UI information about step to be in progress.
*        In called procedures:
*        Reads information about all files of a given baseline into UCC structures and maybe into RAM memory.
*        May do other processing AFTER file information per file, Counts, Complexity, Differencing,
*    Updates count done or percent done on UI as processing step runs.
*    Releases string representation of file list to save RAM after UCC structures are populated.
*    Returns  1 if OK, else 0 if unrecoverable error trying to open list file
*    Returns  2 if User cancelled
*
* 2.Parameters:
*    inputFileVector: list of files to count (may be empty if inputFileList is given)
*    inputFileList: file containing list of files to count (if any)
*    useListA: use file list A? (otherwise use list B)
*
* 3.Creation Time And Owner:
*    Version: 2009.01
*    Revised: 2015.12  Changed to update UI and use threads (if wanted) after file list is built.
*/
int MainObject::ReadAllFiles(StringVector &inputFileVector, string const &inputFileList, const bool useListA)
{
	string	errList;            // Modification: 2015.12
	filemap fmap;		// Modification: 2009.01
	results r;
	std::vector<std::string>::iterator itVectorData;
	string fileName, clearCaseCroppedFile;
	string oneline;

	unsigned long long * pTotalFileSizes = &totalFileSizesA;
	unsigned long long * pLargestFileSize = &largestFileSizeA;
	unsigned long * pNumFilesIn = &numFilesInA;
	string * pCommonPathPrefix = &commonPathPrefixA;
	if (false == useListA)
	{
		pTotalFileSizes = &totalFileSizesB;
		pLargestFileSize = &largestFileSizeB;
		pNumFilesIn = &numFilesInB;
		pCommonPathPrefix = &commonPathPrefixB;
	}


	// if the size of the vector is zero read from the files
	if (inputFileVector.size() == 0 && inputFileList.size() > 0)    // Modification: 2009.01
	{
		ifstream fl;
		fl.open(inputFileList.c_str(), ifstream::in);   // Modification: 2011.05

		if (!fl.is_open())  // Modification: 2009.01
		{
			// try old input file name
			fl.open(INPUT_FILE_LIST_OLD, ifstream::in); // Modification: 2011.05
			if (!fl.is_open())  // Modification: 2009.01
			{
                string err = "Unable to open list file  (";   // Modification: 2011.10
				err += inputFileList;   // Modification: 2009.01
				err += ")";
				userIF->AddError(err, false, 1);    // Modification: 2011.10
				return 0;
			}
		}
		fl.clear();
		fl.seekg(0, ios::beg);

		// LOOP and add more File names to List of Files to process
		StringVector searchFiles;   // Modification: 2013.04
		searchFiles.push_back("*.*");

		// Process each line, save the fileName if it is valid.
		while (fl.good() || fl.eof())   // Modification: 2009.01
		{
			getline(fl, fileName);
			if ((!fl.good() && !fl.eof()) || (fl.eof() && fileName.empty()))
				break;
			if (!CUtil::CheckBlank(fileName))
			{
				if (clearCaseFile)
				{
					inputFileVector.push_back(fileName);    // Modification: 2011.05
					unsigned long long fileSizeBytes = CUtil::GetFileSizeInBytes(fileName);
					if (*pLargestFileSize < fileSizeBytes)
						*pLargestFileSize = fileSizeBytes;
					*pTotalFileSizes += fileSizeBytes;
					if (0L == *pNumFilesIn)
						*pCommonPathPrefix = fileName;
					(*pNumFilesIn)++;
					CUtil::FindCommonPrefix(*pCommonPathPrefix, fileName, *pCommonPathPrefix, false);
				}
				else    // Modification: 2009.01
				{
					// expand if item is a directory
					int listResult = CUtil::ListAllFiles(fileName, searchFiles, inputFileVector, followSymLinks,
						*pTotalFileSizes, *pLargestFileSize, *pNumFilesIn, *pCommonPathPrefix);
					if (0 == listResult)
						inputFileVector.push_back(fileName);	// Not a directory
					else
						if (-1 == listResult)
							return 2;		// User cancelled while getting the file list
				}
			}
			if (!fl.good())
				break;

			if (HasUserCancelled())
				return 2;
		}	//	END		while adding more File names

		fl.close();
	}	//	END		if adding more to List of Files to Read

		//      ALL the rest to end of procedure is  Modified: 2015.12
		//
		//  File List is Built:
		//  Total of file Sizes, Largest File Size, number of Files, common Path prefix is known.
		//

#ifdef	SHOW_FILES_FOUND		// Modified: 2015.12

		// This will list all the files found.  Not usually the same as all that are processed.
	string	msg = "";
	if (isDiff)
	{
		msg = "Baseline A ";
		if (!useListA)
			msg = "Baseline B ";
	}

	cout << "\n\n    List of " << msg << "files found before detailed processing:\n";

	// This might take a while if scrolling several thousands of file path/names
	for (std::vector<int>::size_type i = 0; i != inputFileVector.size(); i++)
		cout << inputFileVector[i] << endl;

	cout << "    There were  " << inputFileVector.size() << "  " << msg << "files found before detailed processing.\n";
	cout << "        If the number processed is smaller then please\n        check the number of Uncounted files and files with Errors.\n\n";

#endif		// Modified: 2015.12

	//
	//  Estimate RAM needed for success
	bool	isDup = false;
	if (duplicate_threshold >= 0.0)
		isDup = true;

	unsigned long long
		ramAvailable = (unsigned long long)ramLimit * (unsigned long long)(100L * 1024L * 1024L);

	// Absolute minimum without changing how the parsers work.
	// See  CCodeCounter::CountSLOC() for details.
	unsigned long long ramNeededA, ramNeededB, minFileSizeA, minFileSizeB, structSizesB, structSizesA
		= (unsigned long long) sizeof(SourceFileElement) * (unsigned long long)numFilesInA;

	// 2.5 x size of largest file (WORST CASE is more than this!)  This is the MINIMUM.
	// The previous way would RAM buffer ALL the files, not just the largest!
	minFileSizeA = largestFileSizeA + largestFileSizeA + largestFileSizeA / 2;
	if (isDup)
		minFileSizeA += minFileSizeA;	// Must have buffers for another possibly HUGE file.

	ramNeededA = structSizesA + minFileSizeA;
	// Plus overhead of std list struct.  This is the MINIMUM estimate.
	// Due to how the std lib code arranges structs in memory, 
	// HW & OS memory alignment requirements, Heaps use, memory page sizes, etc.
	// It is VERY likely that extra RAM not accounted for here is actually required.
	ramNeededA += (unsigned long long) sizeof(SourceFileList);

	// Extreme WORST case is that largest file is last in list and is HUGE, 
	// it has no (or very few blank lines and comments),
	// so that when extracting logical SLOC yet a 3rd HUGE buffer is needed.

	bool	estimateNeeded = true;
	if (ramNeededA >= ramAvailable)
	{
		// Show User the RAM calculations.  Also save to error log.
		string	numBuf, numBuf2, numBuf3, ram_msg = "\n";
		ShowFileSetStats(useListA, &ram_msg);

		ram_msg += "Information: Unlikely to finish.\nAvailable RAM is not enough for estimated needs.\n";
		IntToStr((int)ramLimit, numBuf);
		LongLongToStr((long long)ramAvailable, numBuf2);
		ram_msg += "Available RAM given is: " + numBuf + " increments of 100 MB = " + numBuf2 + " total Bytes.\n";

		ram_msg += "MINIMUM estimated RAM needed (baseline A) is:\n";
		LongLongToStr((long long)numFilesInA, numBuf);
		LongLongToStr((long long)sizeof(SourceFileElement), numBuf2);
		LongLongToStr((long long)structSizesA, numBuf3);
		ram_msg += "Number of A files " + numBuf +
			" times size of a file structure " + numBuf2 + " = " + numBuf3 + "\n";

		LongLongToStr((long long)largestFileSizeA, numBuf);
		LongLongToStr((long long)minFileSizeA, numBuf2);
		ram_msg += "PLUS size of largest file " + numBuf;
		if (isDup)
			ram_msg += " times 5 (Duplicate checks) = ";
		else
			ram_msg += " times 2.5 = ";
		ram_msg += numBuf2 + "\n";

		LongLongToStr((long long)ramNeededA, numBuf);
		ram_msg += "Total minimum estimated RAM needed is: " + numBuf + " Bytes.\n";

		// Tell User increase RAM limit or reduce number and perhaps size of files.
		ram_msg += "Suggestions: If you really have more RAM available increase -ramlimit\n";
		ram_msg += "Very important that you or the person deploying UCC review the -ramlimit\n";
		ram_msg += "section in the UCC User Manual before making the increase.  Or you can\n";
		ram_msg += "reduce the number of files to process or even split the largest file.\n";

		userIF->AddError(ram_msg, false, 1);

		// For backwards compatability allow UCC to continue even though
		// it may be very likely to run out of RAM.
		// fall through
	}
	else
	{
		if (!isDiff)
			estimateNeeded = false;	// already done above and seems to be OK
	}

	bool	doNotRead = false;
	bool	noWeb = false;

	// Check for Diff, Dup and available RAM to give Estimate for success !
	if (isDiff)
	{
		doNotRead = true;	// Diff (and maybe Dup).  Diff will use less RAM Memory
		if (useListA)
			estimateNeeded = false;	// Wait until built Baseline B list to do Estimate
	}

	if (estimateNeeded)
	{
		// Already did estimate for Baseline A above so 1 Baseline seems OK.
		// Minimum for a Baseline: # of file structs, + 3 x size of largest (maybe drop to 2.5 x)
		// compare with ramLimit, repeat for Baseline B
		if (isDiff)
		{
			CUtil::FindCommonPrefix(commonPathPrefixA, commonPathPrefixB, commonPathPrefixBoth, false);

			// At this point we have the Baseline B number of files and largest.
			// The code is already biased to use the least amount of RAM.
			// Just do a sanity check here that still OK adding in Baseline B amounts.
			structSizesB
				= (unsigned long long) sizeof(SourceFileElement) * (unsigned long long)numFilesInB;

			// Refer to comments about Baseline A how the minimum is just that: the MINIMUM !
			minFileSizeB = largestFileSizeB + largestFileSizeB + largestFileSizeB / 2;

			if (isDup)
				minFileSizeB += minFileSizeB;	// Must have buffers for another possibly HUGE file.

			ramNeededB = structSizesB + minFileSizeB;
			ramNeededB += (unsigned long long) sizeof(SourceFileList);

			if (ramNeededA + ramNeededB >= ramAvailable)
			{
				// Show User the RAM calculations.  Also save to error log.
				string	numBuf, numBuf2, numBuf3, ram_msg = "\n";
				ShowFileSetStats(true, &ram_msg);
				ram_msg += "\n";
				ShowFileSetStats(false, &ram_msg);

				ram_msg += "Information: Difference unlikely to finish.\nAvailable RAM is not enough for estimated needs.\n";
				IntToStr((int)ramLimit, numBuf);
				LongLongToStr((long long)ramAvailable, numBuf2);
				ram_msg += "Available RAM given is: " + numBuf + " increments of 100 MB = " + numBuf2 + " total Bytes.\n";

				ram_msg += "MINIMUM estimated RAM needed (baseline A) is:\n";
				LongLongToStr((long long)numFilesInA, numBuf);
				LongLongToStr((long long)sizeof(SourceFileElement), numBuf2);
				LongLongToStr((long long)structSizesA, numBuf3);
				ram_msg += "Number of A files " + numBuf +
					" times size of a file structure " + numBuf2 + " = " + numBuf3 + "\n";

				LongLongToStr((long long)largestFileSizeA, numBuf);
				LongLongToStr((long long)minFileSizeA, numBuf2);
				ram_msg += "PLUS size of largest A file " + numBuf;
				if (isDup)
					ram_msg += " times 5 (Duplicate checks) = ";
				else
					ram_msg += " times 2.5 = ";
				ram_msg += numBuf2 + "\n";

				LongLongToStr((long long)ramNeededA, numBuf);
				ram_msg += "Total minimum A estimated RAM needed is: " + numBuf + " Bytes.\n\n";

				ram_msg += "MINIMUM estimated RAM needed (baseline B) is:\n";
				LongLongToStr((long long)numFilesInB, numBuf);
				LongLongToStr((long long)sizeof(SourceFileElement), numBuf2);
				LongLongToStr((long long)structSizesB, numBuf3);
				ram_msg += "Number of B files " + numBuf +
					" times size of a file structure " + numBuf2 + " = " + numBuf3 + "\n";

				LongLongToStr((long long)largestFileSizeB, numBuf);
				LongLongToStr((long long)minFileSizeB, numBuf2);
				ram_msg += "PLUS size of largest B file " + numBuf;
				if (isDup)
					ram_msg += " times 5 (Duplicate checks) = ";
				else
					ram_msg += " times 2.5 = ";
				ram_msg += numBuf2 + "\n";

				LongLongToStr((long long)ramNeededB, numBuf);
				ram_msg += "Total minimum     B estimated RAM needed is: " + numBuf + " Bytes.\n";
				LongLongToStr((long long)ramNeededA + (long long)ramNeededB, numBuf);
				ram_msg += "Total minimum A + B estimated RAM needed is: " + numBuf + " Bytes.\n";

				// Tell User increase RAM limit or reduce number and perhaps size of files.
				ram_msg += "      Suggestions: \n";
				if (isDup)
					ram_msg += "Recommend Duplicate checking be separated from Differencing.\n";
				ram_msg += "If you really have more RAM available increase -ramlimit\n";
				ram_msg += "Very important that you or the person deploying UCC review the\n";
				ram_msg += "-ramlimit section in the UCC User Manual before making the increase.\n";
				ram_msg += "Or you can reduce the number of files or even split the largest file.\n";

				userIF->AddError(ram_msg, false, 1);

				// For backwards compatability allow UCC to continue even though
				// it may be very likely to run out of RAM.
				// fall through
			}
		}
	}

	if (isDiff)
	{
            //Modification: 2017.02
            if (!doFuncDiff)
            {

		if (useListA)
		{
#ifdef	QTGUI
			userIF->updateProgress("Get baseline A information about source files.....", false);
#else
			cout << "Get baseline A information about source files.....";
#endif
		}
		else
		{
#ifdef	QTGUI
			userIF->updateProgress("Get baseline B information about source files.....", false);
#else
			cout << "Get baseline B information about source files.....";
#endif
		}
            }
	}
	else
	{
		if (g_process_after_read)
			userIF->updateProgress("Read, Analyze & Count keywords in files......", false);
		else
			userIF->updateProgress("Reading source files.........................", false);
	}

	//
	// List of files to work on is ready.  Send to Thread helper for worker threads if wanted.
	//
	//  NOTE: To have thread safe code, more arguments need to be passed to thread code.
	//  Thread arguments may be pass by value Global flags or other variables.
	//  Threads use args on their local stacks to avoid False Sharing performance problem.

	// Set up to either call thread helper or just run single threaded.
	SourceFileList* mySrcFileList = (useListA) ? &SourceFileA : &SourceFileB;
	std::vector<std::string>::iterator itStart = inputFileVector.begin();

#ifndef	QTGUI
	cout << "..........";  // set up a small area / 10 blanks / to update on top of
#endif

#ifdef	ENABLE_THREADS
	unsigned long	count_done = 0L; //Modification: 2018.01 moved count_done inside ENABLE_THREADS to remove warnings
	if (workThreadsCount >= MIN_UCC_THREAD_COUNT)
	{
		unsigned long	num_in_list = (unsigned long)distance(itStart, inputFileVector.end());

                //Modification: 2018.01 error_count not used
		ReadFilesThreads(inputFileVector.size(), doNotRead, noWeb,
			userIF, print_cmplx,
			print_csv, inputFileVector,
			useListA, clearCaseFile, outDir,
			g_process_after_read,
			g_discard_PHY_lines_after_process,
			g_discard_lines_after_process,
			g_no_warnings_to_UI,
			g_no_uncounted);

		// Main thread waits until File Reading (and possible other processing) is finished.
		string			cntStr;
		unsigned int	num_busy = 1;
		int				percent_done = 0;
		int				prev_percent_done = 0;
		unsigned long	prev_count_done = 0L;

		do
		{
#ifdef	QTGUI
			ThreadSleep(500);		// wake up about 2 times per second
#else
			ThreadSleep(1000);	// wake up about 1 time per second
#endif

								// Returns count of threads working 
								// and will set count_done (how many files) were processed so far
								// Don't automatically call CombineThreadResults here as UI must update first
			num_busy = NumThreadsBusy(count_done, 100, false);

			if (num_busy)
			{
				percent_done = (int)((count_done * 100L) / num_in_list);
				if ((percent_done != prev_percent_done) || (prev_count_done != count_done))
				{
#ifdef	QTGUI
					// Percent changed so update UI
					userIF->updateProgress("", false, percent_done, (unsigned int)num_in_list);
#else
					// Just keep erasing and redrawing a text area with updated count
					LongToStr((long)count_done, cntStr);
					cout << "\b\b\b\b\b\b\b\b\b\b" << cntStr;		// 10 backspace + new count
					for (int i = 0; i < 10 - (int)cntStr.size(); i++)
						cout << " ";	// fill at right to be easier next time
					cout << flush;
#endif
					prev_percent_done = percent_done;
					prev_count_done = count_done;
				}
			}

			if (HasUserCancelled())
				break;

		} while (num_busy);
	}
	else
#endif
	{
		// Not using extra worker threads
		// Still uses optimizations
		ErrMsgStructVector			err_msgs;	// Not used but needed for arg in call
		UncountedFileStructVector	unc_files;	// Not used but needed for arg in call

                //Modification: 2018.01 error_count not used
		ReadFilesInList(MAIN_THREAD_INDEX, doNotRead, noWeb,
			userIF,
			CounterForEachLanguage,
			print_cmplx,
			print_csv, itStart, inputFileVector.end(),
			useListA, clearCaseFile, mySrcFileList,
			&SourceFileA, &SourceFileB,
			errList,
			outDir,
			err_msgs, unc_files,
			g_process_after_read,
			g_discard_PHY_lines_after_process,
			g_discard_lines_after_process,
			g_no_warnings_to_UI,
			g_no_uncounted);

		// Here the error_count is for info purposes.  
		// Single thread mode mostly does the Error reporting as expected.

		//count_done = mySrcFileList->size(); //Modification: 2018.01 removed as it's not used anymore to avoid warnings
	}

#ifdef QTGUI
	// Call 1 more time to show User 100% completion (this does not depend on following code).
	userIF->updateProgress("", false, 100, 1);
#else
	// Just erase the last 10 characters
	cout << "\b\b\b\b\b\b\b\b\b\b          \b\b\b\b\b\b\b\b\b\b";
        //Modification 2017.02
        if (!doFuncDiff)
        {
            cout << "\b\b\b\bDONE\n";               // And to adjust for not using calls to update progress
        }
#endif

	if (workThreadsCount >= MIN_UCC_THREAD_COUNT)
	{
		CombineThreadResults();
		// At this point any Errors => log or UI messages, Uncounted Files, UI messages
		// were handled along with the results of Reading in the List of Files
	}

	if (errList.size())
		userIF->updateProgress(errList);

	// Free possibly large vector of file names, now in SourceFileA or B   Modified: 2015.12
	inputFileVector.resize(0);

    //warnings fix 11.25.16. DO NOT USE THESE VARIABLES AFTER THIS.
    //(void)error_count;
 	//(void)count_done;

	return 1;
}

/*!
* 1.Function Description:
*	 Process source file list and call relevant counter for each file.
*
* 2.Parameters:
*    useListA: use file list A? (otherwise use list B)
*
* 3.Creation Time And Owner:
*	 Version: 2009.01
*    Revised: 2015.12  Now calls code that is shared with thread helpers
*/
/*
void MainObject::ProcessSourceList( const bool useListA )
{
SourceFileList* mySrcFileList = (useListA) ? &SourceFileA: &SourceFileB;  // Modification: 2009.01
string errList;

ErrMsgStructVector			err_msgs;	// Not used but needed for arg in call
UncountedFileStructVector	unc_files;	// Not used but needed for arg in call

int				prev_percent = 0;
unsigned int	num_inserted = 0;
unsigned int	count_progress = 1;
unsigned int	sizeo = (unsigned int)mySrcFileList->size();

std::map<string, bool> duplicateFiles;

// iterate every souce file in the file list
SourceFileList::iterator itEnd = mySrcFileList->end();
for ( SourceFileList::iterator i = mySrcFileList->begin(); i != itEnd; i++ )
{
// Code inside ProcessSourceListFile MAY Insert entries in the List !
// The loop check above is OK as it can handle a growing List.

// Was the path/file name already processed ?
if (duplicateFiles[i->second.file_name] == false) {
// marks this file as visited so that we avoid processing
// files with the same name more than once.
//
duplicateFiles[i->second.file_name] = true;
// and fall through
}
else
continue;    // Path/file name done previously

ProcessSourceListFile( MAIN_THREAD_INDEX, userIF, CounterForEachLanguage,
&counter, i, &SourceFileA, &SourceFileB,
num_inserted,
err_msgs, unc_files,
errList, (*i).second.file_name,
print_csv, print_cmplx, useListA );

if ( HasUserCancelled() )
return;

// Moving UI call here to keep the User informed has 2 benefits:
// 1) Removes a little overhead from code path used by threads
// 2) Fixes defect where the UI would increment the % done
// BEFORE any Analysis or Counting of a file.  In the case
// of 1 file it would show 100% and then process the file.
// Having it here now reports % done more accurately.

if ( i->second.file_name_isEmbedded == false )
{
int disp = (int)( (count_progress * 100) / sizeo );
if ( prev_percent != disp )
{
userIF->updateProgress( "", false, disp, sizeo );
prev_percent = disp;
}
count_progress++;
}
}

#ifndef QTGUI
if (mySrcFileList->size() > 0)
userIF->updateProgress("\b\b\b\b", false);
userIF->updateProgress("DONE");
#endif

if (!errList.empty())
userIF->updateProgress(errList);
}
*/

/*!
* 1.Function Description:
*	 Checks whether a file has a supported extension (can be counted by UCC).Return value is  supported file extension.
*
* 2.Parameters:
*    file_name: file name
*
* 3.Creation Time And Owner:
*	 Version: 2011.05
*/
bool MainObject::IsSupportedFileExtension(const string &file_name)
{
	for (map<int, CCodeCounter*>::iterator iter = CounterForEachLanguage.begin(); iter != CounterForEachLanguage.end(); iter++)
	{
		// Call to CCodeCounter::IsSupportedFileExtension
		if (iter->second->IsSupportedFileExtension(file_name))
			return true;
	}
	return false;
}


#ifdef	QTGUI
/*!
* 1.Function Description:
*	 Retrieves the extensions for each counter language.
*
* 2.Parameters:
*    lang_ext_map: pointer to language extension map
*
* 3.Creation Time And Owner:
*	 Version 2014.08
*/
void MainObject::GetLanguageExtensionMap(multimap<const string, string> *lang_ext_map)  // Modification: 2014.08
{
	CWebCounter *webCounter = (CWebCounter *)CounterForEachLanguage[WEB];
	StringVector *ext_list;
	string lang_name;

	lang_ext_map->clear();
	// iterate the each counter and add corresponding names to the extension map
	for (map<int, CCodeCounter*>::iterator iter = CounterForEachLanguage.begin(); iter != CounterForEachLanguage.end(); iter++)
	{
		lang_name = iter->second->language_name;
		if (lang_name != "UNDEF")
		{
			if (lang_name == "ASP")
				ext_list = &(webCounter->file_exten_asp);
			else if (lang_name == "JSP")
				ext_list = &(webCounter->file_exten_jsp);
			else if (lang_name == "PHP")
				ext_list = &(webCounter->file_exten_php);
			else if (lang_name == "ColdFusion")
				ext_list = &(webCounter->file_exten_cfm);
			else if (lang_name == "XML")
				ext_list = &(webCounter->file_exten_xml);
			else if (lang_name == "HTML")
				ext_list = &(webCounter->file_exten_htm);
			else
				ext_list = &iter->second->file_extension;

			if (ext_list->size() < 1)
				lang_ext_map->insert(pair<const string, string>(lang_name, ""));
			else
			{
				for (StringVector::iterator vit = ext_list->begin(); vit != ext_list->end(); vit++)
					lang_ext_map->insert(pair<const string, string>(lang_name, (*vit)));
			}
		}
	}
}
#endif		// QTGUI


/*!
* 1.Function Description:
*	 Finds duplicate files in a baseline.
*    2015.12 Extensively refactored: Thread use, Show counts done, User Cancel, Check Memory Leaks (Debug) and Faster Performance.
*
* 2.Parameters:
*    fileList: file list
*    dupList1: first (source) duplicate files list
*    dupList2: duplicate files list
*    checkMatch: check for matching files (applies only when differencing)
*
* 3.Creation Time And Owner:
*	 Version: 2011.05
*    Revised: 2015.12  Refactored: Faster Performance, Show counts done, User Cancel and Check Memory Leaks (Debug)
*/
void MainObject::FindDuplicateFiles(SourceFileList * pFileList, StringVector * dupList1, StringVector * dupList2,
	const bool checkMatch, const bool useListA)
{				// Modification: 2015.12
#ifdef	_DEBUG
	int				errorRet;
	bool			call_again = true;
	bool			validate = true;
	unsigned int	start_heap_count = 0;
	unsigned int	end_heap_count = 0;
	unsigned long	start_block_count = 0;
	unsigned long	end_block_count = 0;
	unsigned long long	start_total_sizes = 0L;
	unsigned long long	end_total_sizes = 0L;

	HeapInUse(errorRet, start_heap_count, start_block_count, start_total_sizes, validate);
	if ((errorRet == 1) || (errorRet < 0))
		call_again = false;
#endif

	// Update UI while finding Duplicates

#ifdef	QTGUI
	// Percent counting approach is NOT good enough.  TODO: Change Qt UI to use counts as well
	int				percent_done = 0;
	unsigned long	num_in_list = 2 * pFileList->size();
#endif


	string			cntStr;

	// These are used for Main (single) thread behavior.  See UCCThread for thread behavior
#define		UI_REFRESH_INTERVAL		3		// In seconds, do NOT make less than 2
#define		UI_DUP_COUNT_INTERVAL	20		// Show counts every 20 or every Refresh interval seconds

//	SourceFileList::iterator fileList_end = pFileList->end();
	SourceFileList::iterator i;

	// Get a list of per Language starting positions for Duplicate checking, WEB has Embedded "files"
	vector<struct langTypeArrayHeader> langs;
	int num_languages = GetStartPositions(&langs, useListA);

#ifdef	_DEBUG
	string baseMsg;
	if (isDiff)
	{
		baseMsg = " Baseline A";
		if (false == useListA)
			baseMsg = " Baseline B";
	}
	// DEBUG helper but MAY be needed for Release builds at times
	// The number shown here may be smaller than total languages 
	// as languages with only 1 file are not checked for Duplicates.
	cout << "\n    There are " << num_languages << baseMsg << " Languages for Duplicate checking.\n";
#endif

	if (num_languages <= 0)
		return;

#ifdef	ENABLE_THREADS
	int		available_CPUs = 1;
	bool	use_threads = false;
	if (num_languages > 1)
	{
		// See if have threads to do Duplicate checking in parallel
		available_CPUs = (int)GetCPU_count();	// count will be 2 or more only if threads and CPUs are available
		if (available_CPUs > 1)
		{
			use_threads = true;

			// See if reasonable to have more worker threads.  About 2 x actual CPU limit seems OK.
			int wanted_thread_count = 2 * available_CPUs;
			if (wanted_thread_count > num_languages)
				wanted_thread_count = num_languages;

			if (wanted_thread_count > (int)workThreadsCount)
			{
				FinishWorkerThreads();
				string msg;
				workThreadsCount = (unsigned int)wanted_thread_count;
				StartThreads(msg);
				if (0 == workThreadsCount)
					use_threads = false;
			}
		}
	}
	else
	{
		// Only 1 Language to do for Duplicate checks.
		//
		// TODO: Implement 2 or more threads checking Duplicates on 1 Language
		// First thread starts and has highest priority to changing list entries (Dup flags)
		// Other threads have decreasing priority (allow up to 16 work threads total)
		// Each thread has a limit of positions [index] in the pointer array that can 
		// NOT go beyond without higher priority thread releasing that position.
		// 
		// For NOW:  Take the easy way out and destroy threads to release RAM
		FinishWorkerThreads();
		workThreadsCount = 0;
	}

    //Warning fix 11.25.16. DO NOT USE THIS VARIABLES AFTER THIS
    (void)use_threads;
#endif


#ifndef	QTGUI
	cout << ".........." << flush;  // set up a small area / 10 blanks / to update on top of
#endif

#ifdef	ENABLE_THREADS

	if (workThreadsCount >= MIN_UCC_THREAD_COUNT)
	{
		// Prototype
		void DupCheckingThreads(vector<struct langTypeArrayHeader> * langs,
			const double				dup_threshold,
			const bool					checkMatch,
			const bool					useListA,
			UserIF					*	userIF,
			const bool					clearCaseFile,
			const bool					print_cmplx,
			const bool					print_csv);

		unsigned long	count_done = 0;
		unsigned long	prev_count_done = 0;
		unsigned int	num_busy = 1;

		DupCheckingThreads(&langs,
			duplicate_threshold,
			checkMatch,
			useListA,
			userIF,
			clearCaseFile,
			print_cmplx,
			print_csv);

		do
		{
#ifdef	QTGUI
			ThreadSleep(250);		// wake up about 4 times per second
#else
			ThreadSleep(1000);	// wake up about 1 time per second
#endif

								// Returns count of threads working 
								// and will set count_done (how many Pairs) were Diffed so far
								// Don't automatically call CombineThreadResults here as UI must update first
			num_busy = NumThreadsBusy(count_done, 100, false);

			if (num_busy)
			{
				if (count_done != prev_count_done)
				{
#ifdef	QTGUI
					// Percent changed so update UI
					percent_done = (int)((count_done * 100L) / num_in_list);
					userIF->updateProgress("", false, percent_done, (unsigned int)num_in_list);
#else
					// Just keep erasing and redrawing a text area with updated count
					LongToStr((long)count_done, cntStr);
					cout << "\b\b\b\b\b\b\b\b\b\b" << cntStr;		// 10 backspace + new count
					for (int i = 0; i < 10 - (int)cntStr.size(); i++)
						cout << " ";	// fill at right to be easier next time
					cout << flush;
#endif
					prev_count_done = count_done;
				}
			}

			if (HasUserCancelled())
				break;

		} while (num_busy);
	}
	else
#endif
	{
		// Single threaded.  UI updates and check for Cancel happen within.
		ErrMsgStructVector			err_msgs;
		UncountedFileStructVector	unc_files;
		string						myErrList;

		int error_count =
			DupCheckLanguages(MAIN_THREAD_INDEX,
				&langs,
				&err_msgs,
				&unc_files,
				&myErrList,
				duplicate_threshold,
				dupList1,
				dupList2,
				checkMatch,
				useListA,
				userIF,
				clearCaseFile,
				CounterForEachLanguage,
				print_cmplx,
				print_csv,
				&SourceFileA,
				&SourceFileB);

		if ((int)err_msgs.size() != error_count)
		{
			// Perhaps check into why the count are not same.
			// See DupCheckLanguages...
		}
	}

#ifndef	QTGUI
	cout << "\b\b\b\b\b\b\b\b\b\b          \b\b\b\b\b\b\b\b\b\b" << flush;
#endif
        //Modification: 2017.02
        if (!doFuncDiff)
        {
	        userIF->updateProgress("\b\b\b\bDONE");
        }


	if (workThreadsCount >= MIN_UCC_THREAD_COUNT)
	{
		CombineThreadResults();
		// At this point any Errors => log or UI messages, UI messages
		// were handled along with the results of Duplicate checking
	}

#ifdef	_DEBUG
	if (call_again)
	{
		validate = true;
		HeapInUse(errorRet, end_heap_count, end_block_count, end_total_sizes, validate);
		if ((end_heap_count != start_heap_count)
			|| (end_block_count != start_block_count)
			|| (end_total_sizes != start_total_sizes))
		{
			if ((end_block_count > start_block_count)
				|| (end_total_sizes > start_total_sizes))
				cout << "\nMemory LEAK likely inside FindDuplicateFiles\n";
			else
			{
				// GOOD NEWS !  Maybe.
				cout << "\nMemory SHRINK likely inside FindDuplicateFiles\n";
			}
		}
	}
#endif
	//Warning fix 11.25.16. DO NOT USE THIS VARIABLE AFTER THIS.
	//(void)fileList_end;

	return;
}


#ifdef QTGUI
/*!
* 1.Function Description:
* 	 Connects to the user interface (only required for GUI).
*
* 2.Parameters:
*	  parent: user interface parent widget
*
* 3.Creation Time And Owner:
*	 Version: 2013.04
*/
void MainObject::ConnectUserIF(QWidget *parent)
{
	userIF = new UserIF(parent);
}
#endif


/* Added for integration.*/

/*!
* 1. Function Description:
*     Reads a file into memory.
*	   return method status
*
* 2. Parameters:
*     sourceFile: file to read
*     errList: error list pointer (if NULL, skip errors)
*     useListA: use file list A? (otherwise use list B)
*
* 3. Creation Time And Owner:
*	  Version: 2015.12
*/

/*     COMMENTED  OUT  2015_10_07   Randy Maxwell
int MainObject::ReadFile(SourceFileElement *sourceFile, string *errList, bool useListA)				// Modification: 2015.12
{
filemap *fmap = &(sourceFile->first);
results *r = &(sourceFile->second);
string oneline, alt_file_name;
bool lineTooLong, isErr = false;
unsigned int i, lineNum = 0;
vector<unsigned char> buffer;
vector<vector<unsigned char> > fileBuffer;
string hash;

fmap->clear();
ifstream fr(r->file_name.c_str(), ios::in);
if (!fr.is_open())
{
// if BaselineFilePath exists, attempt to prepend it to the file_name
if ((useListA && BaselineFilePath1 != "") || (!useListA && BaselineFilePath2 != ""))
{
#ifdef UNIX
if (r->file_name[0] != '/')
#else
if (r->file_name[0] != '/' && r->file_name[0] != '\\' && (r->file_name.length() < 3 || r->file_name[1] != ':'))
#endif
{
if (useListA)
alt_file_name = BaselineFilePath1 + r->file_name;
else
alt_file_name = BaselineFilePath2 + r->file_name;
fr.open(alt_file_name.c_str(), ios::in);
}
}
}
if (!fr.is_open())
{
if (!r->e_flag)
{
r->e_flag = true;
r->error_code = "Unable to open file";
if (errList != NULL)
{
string err = "Error: ";
err += r->error_code;
err += " (";
err += r->file_name;
err += ")";
*errList += err + "\n";
userIF->AddError(err, true);
WriteUncountedFile("Not Readable", r->file_name, useListA, print_csv);
}
isErr = true;
}
}
else
{
lineTooLong = false;
while (fr.good() || fr.eof())
{
getline(fr, oneline);
if ((!fr.good() && !fr.eof()) || (fr.eof() && oneline.empty()))
break;

if (oneline.length() > MAX_LINE_LENGTH)
{
lineTooLong = true;
break;
}
lineNum++;
if (r->class_type != FORTRAN && r->class_type != PYTHON)    // column location and/or white space is significant
oneline = CUtil::TrimString(oneline);

#ifndef NO32BIT
// start filling a char buffer up to 512 bits (64 bytes) using the string oneline here
vector<char> bytes(oneline.begin(), oneline.end());
if (lineNum > 1)
{
// add new line character
buffer.push_back('\n');
if (buffer.size() >= 64)
{
// put buffer in the struct
fileBuffer.push_back(buffer);

// clear the buffer to use again
vector<unsigned char>().swap(buffer);
}
}
i = 0;
while (i < bytes.size())
{
buffer.push_back(bytes[i]);
if (buffer.size() >= 64)
{
// put buffer in the struct
fileBuffer.push_back(buffer);

// clear the buffer to use again
vector<unsigned char>().swap(buffer);
}
i++;
}
#endif

oneline = CUtil::ReplaceSmartQuotes(oneline);
lineElement element(lineNum, oneline);
fmap->push_back(element);
if (!fr.good())
break;
}
fr.clear();
fr.close();

if (lineTooLong)
{
if (!r->e_flag)
{
r->e_flag = true;
r->error_code = "Line too long";
if (errList != NULL)
{
string err = "Error: ";
err += r->error_code;
err += ", file skipped (";
err += r->file_name;
err += ")";
*errList += err + "\n";
userIF->AddError(err, true);
WriteUncountedFile("Excessive Line Length", r->file_name, useListA, print_csv);
}
fmap->resize( 0 );  // don't bother processing the file if an error is just going to be reported
isErr = true;
}
}
else if (fmap->size() == 0)
{
if (errList != NULL)
{
string err = "Warning: File is empty (";
err += r->file_name;
err += ")";
*errList += err + "\n";
userIF->AddError(err, true);
}
isErr = true;
}
}

// save the number of lines as the file size
r->file_size = lineNum;

#ifndef NO32BIT
// generate a file hash
if (buffer.size() > 0)
fileBuffer.push_back(buffer);
hash = CHash::CalcSHA256(fileBuffer);
vector<vector<unsigned char> >().swap(fileBuffer);
r->file_hash = hash;
#endif

if (isErr)
return 0;
return 1;
}
**************  END OF COMMENTED OUT  ReadFile  ************************/

/*!
* 1. Function Description:
*    Unloads file lines and SLOC to save memory, but retains counts.
*      The WEB block only works if doing cleanup immediately after processing
*      when the pointer to a langauge parser class instance is available.
*      The lookup code in DecideLanguage called by WEB is not very efficient.
*      Not as flexible as needed elsewhere and because there is 1 + number of worker threads
*      instances the overall savings is quite small as each time the WEB class is called to parse
*      the previous is discarded anyway.  Not worth the computational cost for such small savings.
*
* 2. Parameters:
*     sourceFile: file to unload
*
* 3. Creation Time And Owner:
*	  Version: 2015.12
*/
/*****   Code is not being called
void MainObject::UnloadFile(SourceFileElement *sourceFile)			// Modification: 2015.12
{
// Better to release in reverse order of allocation
if (sourceFile->second.class_type == WEB)
{
if (counter->classtype != WEB)
{
//DecideLanguage(sourceFile->second.file_name);
DecideLanguage( CounterForEachLanguage, &(counter),
false,	// Do NOT have print_cmplx true as we just want to set up to unload, not processing
sourceFile->second.file_name );
}
if (counter->classtype == WEB)
((CWebCounter *)counter)->CleanSeparatedFile();
}
sourceFile->second.mySLOCLines.clear();
sourceFile->first.resize( 0 );
}
*/

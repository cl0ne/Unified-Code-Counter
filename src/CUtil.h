//! Utility class definition for static methods.
/*!
* \file CUtil.h
*
* This file contains the utility class definition for static methods.
*/

#ifndef CUTIL_H
#define CUTIL_H

#include "cc_main.h"

using namespace std;
#define INVALID_POSITION ((unsigned int)-1)
#define TO_END_OF_STRING INVALID_POSITION

//Modified: 2017.02 Max path size of a directory
const int MAX_PATH_SIZE = 100000;

/* Modification: 2015.09.27
*   by Randy Maxwell
*   Fix for Compile error(s) on older compilers when __func__ is not declared. */
// Set up a cross compiler define to support getting local function (procedure) name.
// First check for old versions of Microsoft compilers
#ifdef _MSC_VER
// See Microsoft site: Predefined Macros and then see when __func__ becomes supported.
// See StackOverflow: How to Detect if I'm Compiling Code With Visual Studio 2008?
	#if _MSC_VER < 1800
		// Old Microsoft compiler
		#define		UCC_FUNC_NAME	(__FUNCTION__)
	#else
		// Newer compiler
		#define		UCC_FUNC_NAME	(__func__)
	#endif
#endif

#ifndef	UCC_FUNC_NAME
	// Check for old versions of GCC
	// TODO:  Somebody add this if needed

	// Use __func__  This assumes compiler supports C++ 2011 or later?
	#define		UCC_FUNC_NAME	(__func__)
#endif

//! Vector containing a list of strings.
/*!
* \typedef vectorString
*
* Defines a vector containing a list of strings.
*/
typedef vector<string> vectorString;
/*Modification: 11.2016 Ext-4 Starts*/
static std::map<std::string, std::vector<std::string> > dict;
static std::map<std::string, std::vector<std::string> > dictCopy;
/*Modification: 11.2016 Ext-4 End*/

//
// These are utilities that must NOT be inside the CUtil class
//
void IntToStr( const int val, string & result );
void LongToStr( const long val, string & result );
void LongLongToStr( const long long val, string & result );

// Convert a float to a std::string with a number of decimal places
void FloatToStr( const float val, string & result, const unsigned int dec_precision = 1 );

void CountPhysicalFiles( SourceFileList & fileList, unsigned long & phyFileCount );

void ClearSummaryMsgCounts();

// Time message helpers
//
// Initialize Time values of various processing steps.



void	InitTimes();
#define		MSG_STR_BUF_SIZE	512
void	SavePerformanceStats( const string time_str );
void	TimeMsg( char *pBuf, string & time_str, const char * before, 
				const long count, const char * after, const bool fixed_width = true );
void	ShowTiming( string & time_str, 
					const time_t timeStart, const time_t timeEnd, 
					const bool show_total_only, 
					const bool doDiff, const bool doDups );

// Helper to make lowerChars array access reliable
#define TL_ARR(x)	((unsigned int)((unsigned char)(x)))

//! Utility class.
/*!
* \class CUtil
*
* Defines a utility class.
*/
class CUtil
{
public:
    /*Modification: 11.2016 Ext-4 Starts*/
    static void allocate();
    static StringVector getExtensionsToLanguage(string lang, StringVector fileExtension);
    /*Modification: 11.2016 Ext-4 ends*/

    static string TrimString(const string &str, int mode = 0);
    static string EraseString(const string &srcstr, const string &erasedstr);

	// InitToLower MUST be called before calling ToLower
	static void InitToLower();    // Modification: 2015.12
    static string ToLower(const string &string_to_lower);

    static bool CheckBlank(const string &str);
    static bool IsInteger(const string &str);
    static size_t FindStringsCaseInsensitive(const string &target, map<string, int> &table, size_t &pos, size_t preLang = INVALID_POSITION);
    static size_t FindCharAvoidEscape(const string &source, char target, size_t start_idx, char escape);
	static void FindCommonPrefix( string & str, string & str2, string & prefix, const bool remove = false );
	static void FindCommonSuffix( string & str, string & str2, string & suffix, const bool remove = false );
    static size_t FindKeyword(const string &str, const string &keyword, size_t start = 0, size_t end = TO_END_OF_STRING, bool case_sensitive = true);
    static void CountTally(const string &base, StringVector &container, unsigned int &count, int mode, const string &exclude,
        const string &include1, const string &include2, UIntVector *counter_container = 0, bool case_sensitive = true);

    static vector<string> SplitByConcat(string &statement, string concat);
	static int CountNestedNum(string &combine);
	static size_t SemanticDeduplicate(string &cc4_valid_if, stack<string> &cc4_parent_stack, stack<set<string> > &cyclomatic_distinct_cond_stack, set<string> &nested_set);
	static void SemanticFormat(string &statement);
	static size_t NestedIfDup(string &cc4_valid_if, stack<string> &cc4_parent_stack, stack<set<string> > &cyclomatic_distinct_cond_stack, set<string> &nested_set);
	static size_t ConcatAndDup(string &cc4_valid_if, set<string> &nested_set);
	static void ConcatOrReorderDFS(vector<string> &original, int step, set<int> &visited, string &output, vector<string> &permute);
	static void ConcatOrReorder(set<string> &string_set, string &statement);
	static void CountDistinctCond(string &valid_statement, const string &base, StringVector &constainer, unsigned int &count, int mode, const string &exclude, const string &include1, const string &include2, set<string> &set, UIntVector *counter_container = 0, bool case_sensitive = true);

    static string ExtractFilename(const string &filepath);
    static string ExtractFilepath(const string &filepath);
    
	// Helper used for smarter Estimates to help with RAM usage.
    static unsigned long long GetFileSizeInBytes( const string file );

	// Build a List of Files; of given file Extension(s) starting from a Directory; maybe follow symbolic links.
	// Update number of files, largest file size, total files size, common Path prefix shared by the files.
    static int ListAllFiles(string & folder, const StringVector &fileExtList, StringVector &fileList, const bool symLinks,
                             unsigned long long & totalFileSizes, unsigned long long & largestFileSize, 
                             unsigned long & numFiles, string & commonPathPrefix );
    static bool GetFileList(StringVector &fileList, const string &path, bool symLinks);
    static bool MatchFilename(const string &filename, const string &matchStr);
    //Modification: 2017.02
    static int GetPath(string &path);
    static int MkPath(const string &path);
    // Modification: 2017.02
    static int RmPath(const string &path);
    static string ConvertClearCaseFile( const string &fileName, string & clearCaseTrailer );
    static size_t TruncateLine(size_t length, size_t totalLength, size_t truncate, bool &trunc_flag);
    static string ClearRedundantSpaces(const string &str);
    static string ReplaceSmartQuotes(const string &str);
    static bool StartsWith(const string &str, const string &startsWith, bool case_sensitive = true);
    static bool EndsWith(const string &str, const string &endsWith, bool case_sensitive = true);
    static StringVector Split(const string &stringToSplit, const string &delimiter);
    static string ReplaceWith(const string &originalStr, const string &toBeReplaced, const string &replaceWith);
};

#ifndef NO32BIT
//! Hash generation class.
/*!
* \class CHash
*
* Defines a hash generation class.
*/
class CHash
{
public:
    static string CalcSHA256(vector<vector<unsigned char> > fileBuffer);
};
#endif

#endif
#ifdef _MSC_VER
#define __func__ __FUNCTION__
#endif

//! Compare manager class definition.
/*!
* \file CmpMngr.h
*
* This file contains the compare manager class definition.
*/

#ifndef CMPMNGR_H
#define CMPMNGR_H

#include "cc_main.h"
//#include "MainObject.h"

using namespace std;

enum MARKER {MODIFIED, UNMODIFIED, DELETED, ADDED};


class mapping_code
{
public:
	size_t	base_file_line_num;
	size_t	comp_file_line_num;
	MARKER	marker;

	mapping_code();
	mapping_code(const size_t base_file, const size_t comp_file, const MARKER mk);
	mapping_code(const mapping_code &rhs);
};


//! Compare manager class.
/*!
* \class CmpMngr
*
* Defines the compare manager class.
*/
class CmpMngr
{
public:
    CmpMngr();
    CmpMngr(ofstream *file_stream);
	string Compare(srcLineVector* baseFileMap, srcLineVector* compFileMap, const double match_threshold);

	unsigned int nAddedLines;		//!< Number of lines added
	unsigned int nDeletedLines;		//!< Number of lines deleted
	unsigned int nChangedLines;		//!< Number of lines changed
	unsigned int nNochangedLines;	//!< Number of lines unchanged

private:
	const static string MARKER_TABLE[];

	void FindModifiedLines(srcLineVector* aHm, srcLineVector* bHm);
	void FindUnmodifiedLines(srcLineVector *aHm, srcLineVector *bHm);
	bool SimilarLine( const string &baseLine, int *x1, const string &compareLine, int *x2 );    // Modified: 2015.12

	double MATCH_THRESHOLD;			//!< % threshold for matching, if greater then added/deleted instead of modified

	vector<string> base_file;
	vector<string> comp_file;
	vector<mapping_code> mapping_list;

    ofstream *file_dump_stream;

	void fill_source_code_vec(const srcLineVector *file_map, vector<string> &source_code_vec);

	string print_info(string * diff_details_str = NULL ) const;
};

//! List of strings.
/*!
* \typedef stringList
*
* Defines a list of strings.
*/
typedef list<string> stringList;

//! Map of string lists.
/*!
* \typedef stringSizeMap
*
* Defines a map of string lists.
*/
typedef map<size_t, stringList> stringSizeMap;

typedef pair<const string*, size_t> code_line_pair;

bool compare_code_line_pair(const code_line_pair &lhs, const code_line_pair &rhs);

#endif

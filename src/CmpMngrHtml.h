#ifndef CMPMNGRHTML_H
#define CMPMNGRHTML_H

#define UNIT_INDENT 15

#include "cc_main.h"
#include "MainObject.h"
#include <string>
#include <fstream>
#include <sstream>

#define LINE_TYPE_DELETED       0
#define LINE_TYPE_MODIFIED      1
#define LINE_TYPE_UNMODIFIED    2
#define LINE_TYPE_ADDED         3
#define LINE_TYPE_PHYSICAL      4

using namespace std;

enum EVAL_STATUS {EVAL_GOOD,EVAL_NA_FOUND,EVAL_BAD};

class LinesPair {
public:
    int original_line_number;
    int paired_line_number;
    int type;

    LinesPair(int, int, std::string);
    LinesPair(){}
};

class LinesPairOriginalComparator {
private:
    int m_line_no;

public:
    LinesPairOriginalComparator(int line_no)
        :m_line_no(line_no)
    {}

    bool operator()(const LinesPair& pair)
    {
        return pair.original_line_number == m_line_no;
    }
};

class LinesPairPairedComparator {
private:
    int m_line_no;

public:
    LinesPairPairedComparator(int line_no)
        :m_line_no(line_no)
    {}

    bool operator()(const LinesPair& pair)
    {
        return pair.paired_line_number == m_line_no;
    }
};

class FilePairComparisonData{
private:
    string original_filename;
    string paired_filename;
    vector<LinesPair> mapped_lines;

public:
    FilePairComparisonData(string orig_fn, string paired_fn)
        : original_filename(orig_fn)
        , paired_filename(paired_fn)
    {}
    
    void AddLinesPair(const LinesPair& lp) { mapped_lines.push_back(lp); }
    const string& OriginalFileName() const { return original_filename; }
    const string& PairedFileName() const { return paired_filename; }

    ifstream& OpenOriginalFile(ifstream& file_stream) const;
    ifstream& OpenPairedFile(ifstream& file_stream) const;
    int FindOriginalLineType(int line_no) const;
    int FindPairedLineType(int line_no) const;
};

class CmpMngrHtml
{
public:
    CmpMngrHtml(
            ifstream& dump_file_stream,
            ofstream& html_stream);
    void PrintFileSetAsHtml();

private:
    ifstream& dump_file_stream;
    ofstream& html_stream;
    vector<FilePairComparisonData> comparison_data;

    EVAL_STATUS FillMapData();
    void PrintOriginalFileAsHtml(const FilePairComparisonData& file_pair);
    void PrintPairedFileAsHtml(const FilePairComparisonData& file_pair);
    void PrintHtmlLine(string line, int line_number, string css_class);
    string EscapeHtml(const string& data);

// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CmpMngrHtml' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CmpMngrHtml(const CmpMngrHtml& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CmpMngrHtml' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CmpMngrHtml operator=(const CmpMngrHtml);    // Declare without implementation
};

#endif // CMPMNGRHTML_H

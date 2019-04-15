#include "CmpMngrHtml.h"


/* Modification: 2016.01; USC
   Updated to fix spacing for empty line breaks. Needed to support move to webengine*/
/* HTML Pre-amble for all html files generated... */
#define HTML_HEADER         "   <!DOCTYPE html>"\
                            "   <html>"\
                            "   <head>"\
                            "   <title>UCC File Difference</title>"\
                            "   <style>"\
                            "       body {"\
                            "           margin-top: 40px;"\
                            "       }"\
                            "       .filepair {"\
                            "           width: 100%;"\
                            "           display: table;"\
                            "           table-layout: fixed;"\
                            "           margin-bottom: 20px;"\
                            "       }"\
                            "       .codefile {"\
                            "           width: 50%;"\
                            "           display: table-cell;"\
                            "           margin-top: 15px;"\
                            "           border: 1px solid LightGray;"\
                            "           background-color: rgb(240, 240, 240);"\
                            "       }"\
                            "       .x-scroll {"\
                            "           overflow-x: auto;"\
                            "       }"\
                            "       .codefile-inner {"\
                            "           width: 100%;"\
                            "           display: table;"\
                            "           border-collapse: collapse;"\
                            "           margin-bottom: 10px;"\
                            "       }"\
                            "       .left {"\
                            "           border-radius: 5px 0 0 5px;"\
                            "       }"\
                            "       .right {"\
                            "           border-radius: 0 5px 5px 0;"\
                            "       }"\
                            "       .filename {"\
                            "           font-size: 20px;"\
                            "           font-weight: bold;"\
                            "           font-family: sans-serif;"\
                            "           padding: 10px 5px;"\
                            "           background: rgba(0, 102, 255, 0.5);"\
                            "           border-bottom: 1px solid LightGray;"\
                            "       }"\
                            "       .codeline {"\
                            "           line-height: 1.5;"\
                            "           vertical-align: middle;"\
                            "           unicode-bidi: embed;"\
                            "           font-family: monospace;"\
                            "           white-space: pre;"\
                            "           display: table-row;"\
                            "           border-bottom: 1px dotted LightGray;"\
                            "       }\n"\
                            "       .deleted {"\
                            "           background-color: Red;"\
                            "       }\n"\
                            "       .modified {"\
                            "           background-color: DarkSalmon;"\
                            "       }\n"\
                            "       .unmodified {"\
                            "           background-color: LightGray;"\
                            "       }\n"\
                            "       .added {"\
                            "           background-color: SpringGreen;"\
                            "       }\n"\
                            "       .physical {"\
                            "           background-color: White;"\
                            "       }\n"\
                            "       .lineno {"\
                            "           width: 40px;"\
                            "           display: inline-block;"\
                            "           padding-left: 10px;"\
                            "       }\n"\
                            "       br {"\
                            "           line-height: 1.6;"\
                            "           vertical-align: middle;"\
                            "           unicode-bidi: embed;"\
                            "           font-family: monospace;"\
                            "           white-space: pre;"\
                            "           display: table-row;"\
                            "           border-bottom: 1px dotted LightGray;"\
                            "       }"\
                            "   </style>"\
                            "   </head>"\
                            "   <body>"
#define HTML_FOOTER         "   </body>"
#define FILE_PAIR_HEADER    "   <div class=\"filepair\" >"
#define FILE_PAIR_FOOTER    "   </div>"

string line_ty_to_css_class[] = {
      "deleted"
    , "modified"
    , "unmodified"
    , "added"
    , "physical"
};

LinesPair::LinesPair(int num1, int num2, string type_str){
    original_line_number=num1;
    paired_line_number=num2;
    
    if (type_str == "DELETED") {
        type = 0;
    } else if (type_str == "MODIFIED") {
        type = 1;
    } else if (type_str == "UNMODIFIED") {
        type = 2;
    } else {
        type = 3;
    }
}

ifstream& FilePairComparisonData::OpenOriginalFile(ifstream& file_stream) const
{
    file_stream.open(original_filename.c_str(), ifstream::in);

    return file_stream;
}

ifstream& FilePairComparisonData::OpenPairedFile(ifstream& file_stream) const
{
    file_stream.open(paired_filename.c_str(), ifstream::in);

    return file_stream;
}

int FilePairComparisonData::FindOriginalLineType(int line_no) const
{
    int line_ty;

    vector<LinesPair>::const_iterator it = find_if(
        mapped_lines.begin(),
        mapped_lines.end(),
        LinesPairOriginalComparator(line_no));
    if (it == mapped_lines.end()) {
        line_ty = LINE_TYPE_PHYSICAL;
    } else {
        line_ty = it->type;
    }

    return line_ty;
}

int FilePairComparisonData::FindPairedLineType(int line_no) const
{
    int line_ty;

    vector<LinesPair>::const_iterator it = find_if(
        mapped_lines.begin(),
        mapped_lines.end(),
        LinesPairPairedComparator(line_no));
    if (it == mapped_lines.end()) {
        line_ty = LINE_TYPE_PHYSICAL;
    } else {
        line_ty = it->type;
    }

    return line_ty;
}

CmpMngrHtml::CmpMngrHtml(
        ifstream&  dump_file_st,
        ofstream& html_st)
    : dump_file_stream(dump_file_st)
    , html_stream(html_st)
{}

void CmpMngrHtml::PrintFileSetAsHtml()
{
    EVAL_STATUS my_eval_status;
    
    if (!html_stream) return;

    /*
     * HTML pre-amble. Things like scripts, css, etc. go here...
     */
    html_stream << HTML_HEADER << endl;

    while (dump_file_stream.good()) {
        my_eval_status = FillMapData();
        if (my_eval_status == EVAL_BAD || my_eval_status == EVAL_NA_FOUND) continue;
    }


    /* Loop through files... */
    for (vector<FilePairComparisonData>::iterator itFile = comparison_data.begin();
            itFile != comparison_data.end();
            ++ itFile) {
    
        html_stream << FILE_PAIR_HEADER;
        PrintOriginalFileAsHtml(*itFile);

        PrintPairedFileAsHtml(*itFile);
        html_stream << FILE_PAIR_FOOTER;
    }

    /*
     * HTML post-amble...
     */
    html_stream << HTML_FOOTER;
}

EVAL_STATUS CmpMngrHtml::FillMapData()
{
    string line;
    string type_str;
    string orig_fn, paired_fn;
    int code_lines_original;
    int code_lines_paired;
    bool existed = true;

    getline(dump_file_stream, orig_fn);
    if(orig_fn.empty()) {
        //Redundent blank line found
        return EVAL_BAD;
    }
    getline(dump_file_stream, paired_fn);
    if (paired_fn.empty()) {
        return EVAL_BAD;
    } /*else if (orig_fn == "NA" || paired_fn == "NA") {
        //One baseline file cannot be paired
        return EVAL_NA_FOUND;
    }*/

    if(orig_fn.find_first_of("*") != string::npos){
        unsigned sub_pos = orig_fn.find("*");
        orig_fn = orig_fn.substr(0,sub_pos);
    }
    if(paired_fn.find_first_of("*") != string::npos){
        unsigned sub_pos = paired_fn.find("*");
        paired_fn = paired_fn.substr(0,sub_pos);
    }

    std::vector<FilePairComparisonData>::iterator it;
    for (it = comparison_data.begin() ; it != comparison_data.end(); ++it) {
        if (it->OriginalFileName() == orig_fn) break;
    }
    if (it == comparison_data.end()) existed = false;

    if (!existed) {
        comparison_data.push_back(FilePairComparisonData(orig_fn, paired_fn));

        while(getline(dump_file_stream, line) && !line.empty()) {
            //Have to be in the loop, or stream manipulation will be buggy.
            stringstream line_stream;
            line_stream << line;
            line_stream >> code_lines_original;
            line_stream >> code_lines_paired;
            line_stream >> type_str;

            comparison_data.back().AddLinesPair(LinesPair(
                code_lines_original,
                code_lines_paired,
                type_str));
        }
    } else {
        while(getline(dump_file_stream, line) && !line.empty()) {
            //Have to be in the loop, or stream manipulation will be buggy.
            stringstream line_stream;
            line_stream << line;
            line_stream >> code_lines_original;
            line_stream >> code_lines_paired;
            line_stream >> type_str;

            it -> AddLinesPair(LinesPair(
                code_lines_original,
                code_lines_paired,
                type_str));
        }
    }

    return EVAL_GOOD;
}

void CmpMngrHtml::PrintOriginalFileAsHtml(const FilePairComparisonData& file_pair)
{
    string line;
    int line_number = 0;
    ifstream in_stream;

    /*
     * Code File pre-amble...
     */
    html_stream
        << "<div class=\"codefile left\">"
        << "    <div class=\"x-scroll\">"
        << "        <div class=\"codefile-inner\" >"
        << "            <div class=\"filename\" >" << file_pair.OriginalFileName()
        << "            </div>"
    ;

    file_pair.OpenOriginalFile(in_stream);

    while (getline(in_stream, line)) {
        ++ line_number;

        PrintHtmlLine(
            line,
            line_number,
            line_ty_to_css_class[file_pair.FindOriginalLineType(line_number)]);
    }
    
    /*
     * Code File post-amble...
     */
    html_stream
        << "        </div>"
        << "    </div>"
        << "</div>"
    ;
}
    
void CmpMngrHtml::PrintPairedFileAsHtml(const FilePairComparisonData& file_pair)
{
    string line;
    int line_number = 0;
    ifstream in_stream;

    /*
     * Code File pre-amble...
     */
    html_stream
        << "<div class=\"codefile right\">"
        << "    <div class=\"x-scroll\">"
        << "        <div class=\"codefile-inner\" >"
        << "            <div class=\"filename\" >" << file_pair.PairedFileName()
        << "            </div>"
    ;

    file_pair.OpenPairedFile(in_stream);

    while (getline(in_stream, line)) {
        ++ line_number;

        PrintHtmlLine(
            line,
            line_number,
            line_ty_to_css_class[file_pair.FindPairedLineType(line_number)]);
    }
    
    /*
     * Code File post-amble...
     */
    html_stream
        << "        </div>"
        << "    </div>"
        << "</div>"
    ;
}

void CmpMngrHtml::PrintHtmlLine(
        string line,
        int line_number,
        string css_class)
{
    if (line.empty()) {
        html_stream << "</br>" << endl;
        return;
    }

    html_stream
        << "<span class=\"codeline "
        << css_class
        << "\" >"
        << "<span class=\"lineno\" >"
        << line_number
        << "</span>"
        << EscapeHtml(line)
        << "</span>"
        << endl
    ;
}

string CmpMngrHtml::EscapeHtml(const string& data)
{
    std::string buffer;
    buffer.reserve(data.size());
    for(size_t pos = 0; pos != data.size(); ++pos) {
        switch(data[pos]) {
            case '&':  buffer.append("&amp;");       break;
            case '\"': buffer.append("&quot;");      break;
            case '\'': buffer.append("&apos;");      break;
            case '<':  buffer.append("&lt;");        break;
            case '>':  buffer.append("&gt;");        break;
            default:   buffer.append(&data[pos], 1); break;
        }
    }

    return buffer;
}

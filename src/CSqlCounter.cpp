//! Code counter class methods for the SQL language.
/*!
* \file CSqlCounter.cpp
*
* This file contains the code counter class methods for the SQL language.
*/

#include "CSqlCounter.h"
#include <stack>
#include <vector>
#include <list>
#include <iterator>
#include <map>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cstring>

/*!
* Constructs a CSqlCounter object.
*/
CSqlCounter::CSqlCounter()
{
    classtype = SQL;
    language_name = "SQL";
    casesensitive = false;
    
    //Modification: 11.2016 Ext-4
    file_extension = CUtil::getExtensionsToLanguage("SQL", file_extension);
    //file_extension.push_back(".sql");
    
    QuoteStart = "\"'";
    QuoteEnd = "\"'";
    LineCommentStart.push_back("--");

    BlockCommentStart.push_back("/*");
    BlockCommentEnd.push_back("*/");

    data_name_list.push_back("bigint");
    data_name_list.push_back("binary");
    data_name_list.push_back("bit");
    data_name_list.push_back("blob");
    data_name_list.push_back("boolean");
    data_name_list.push_back("byte");
    data_name_list.push_back("char");
    data_name_list.push_back("character");
    data_name_list.push_back("date");
    data_name_list.push_back("datetime");
    data_name_list.push_back("decimal");
    data_name_list.push_back("double");
    data_name_list.push_back("enum");
    data_name_list.push_back("float");
    data_name_list.push_back("image");
    data_name_list.push_back("int");
    data_name_list.push_back("integer");
    data_name_list.push_back("interval");
    data_name_list.push_back("long");
    data_name_list.push_back("longblob");
    data_name_list.push_back("longtext");
    data_name_list.push_back("mediumblob");
    data_name_list.push_back("mediumint");
    data_name_list.push_back("mediumtext");
    data_name_list.push_back("memo");
    data_name_list.push_back("money");
    data_name_list.push_back("nchar");
    data_name_list.push_back("ntext");
    data_name_list.push_back("nvarchar");
    data_name_list.push_back("numeric");
    data_name_list.push_back("real");
    data_name_list.push_back("single");
    data_name_list.push_back("smalldatetime");
    data_name_list.push_back("smallint");
    data_name_list.push_back("smallmoney");
    data_name_list.push_back("text");
    data_name_list.push_back("time");
    data_name_list.push_back("timestamp");
    data_name_list.push_back("tinyint");
    data_name_list.push_back("tinytext");
    data_name_list.push_back("uniqueidentifier");
    data_name_list.push_back("varbinary");
    data_name_list.push_back("varchar");
    data_name_list.push_back("year");
    //add by KK @ 11/6
    data_name_list.push_back("number");
    data_name_list.push_back("cursor");
    data_name_list.push_back("exception");


    exec_name_list.push_back("alter");
    exec_name_list.push_back("close");
    exec_name_list.push_back("comment");
    exec_name_list.push_back("commit");
    exec_name_list.push_back("create");
    exec_name_list.push_back("declare");
    exec_name_list.push_back("delete");
    exec_name_list.push_back("deny");
    exec_name_list.push_back("drop");
    exec_name_list.push_back("except");
    exec_name_list.push_back("fetch");
    exec_name_list.push_back("grant");
    exec_name_list.push_back("group by");
    exec_name_list.push_back("having");
    exec_name_list.push_back("insert");
    exec_name_list.push_back("intersect");
    exec_name_list.push_back("join");
    exec_name_list.push_back("limit");
    exec_name_list.push_back("order by");
    exec_name_list.push_back("rename");
    exec_name_list.push_back("replace");
    exec_name_list.push_back("revoke");
    exec_name_list.push_back("rollback");
    exec_name_list.push_back("select");
    exec_name_list.push_back("set");
    exec_name_list.push_back("truncate");
    exec_name_list.push_back("union");
    exec_name_list.push_back("update");
    exec_name_list.push_back("where");
    // For PL/SQL
    string exec_words[] = {"CHECK", "CONNECT", "CONTINUE", "ELSIF", "EXIT", "FOR", "GOTO", "IF", "LOCK", "LOOP", "MINUS", "RAISE", "RETURN", "START", "UNIQUE", "USE", "VIEW", "WHEN", "WHILE", "dbms_output.put_line"};
    for (unsigned int i = 0; i < sizeof(exec_words)/sizeof(exec_words[0]); i++) {
        exec_name_list.push_back(exec_words[i]  );
    }

    math_func_list.push_back("abs");
    math_func_list.push_back("avg");
    math_func_list.push_back("ceil");
    math_func_list.push_back("count");
    math_func_list.push_back("exp");
    math_func_list.push_back("floor");
    math_func_list.push_back("max");
    math_func_list.push_back("min");
    math_func_list.push_back("mod");
    math_func_list.push_back("power");
    math_func_list.push_back("round");
    math_func_list.push_back("sign");
    math_func_list.push_back("sqrt");
    math_func_list.push_back("stddev");
    math_func_list.push_back("sum");
    math_func_list.push_back("trunc");
    math_func_list.push_back("variance");

    trig_func_list.push_back("acos");
    trig_func_list.push_back("acosh");
    trig_func_list.push_back("asin");
    trig_func_list.push_back("asinh");
    trig_func_list.push_back("atan");
    trig_func_list.push_back("atan2");
    trig_func_list.push_back("atanh");
    trig_func_list.push_back("cos");
    trig_func_list.push_back("cosh");
    trig_func_list.push_back("sin");
    trig_func_list.push_back("sinh");
    trig_func_list.push_back("tan");
    trig_func_list.push_back("tanh");

    log_func_list.push_back("ln");
    log_func_list.push_back("log");

    cmplx_calc_list.push_back("+");
    cmplx_calc_list.push_back("-");
    cmplx_calc_list.push_back("*");
    cmplx_calc_list.push_back("/");

    cmplx_cond_list.push_back("except");
    cmplx_cond_list.push_back("group by");
    cmplx_cond_list.push_back("having");
    cmplx_cond_list.push_back("intersect");
    cmplx_cond_list.push_back("join");
    cmplx_cond_list.push_back("limit");
    cmplx_cond_list.push_back("order by");
    cmplx_cond_list.push_back("union");
    cmplx_cond_list.push_back("where");
    // For PL/SQL
    cmplx_cond_list.push_back("when");
    cmplx_cond_list.push_back("else");
    cmplx_cond_list.push_back("elsif");// Because pl/sql has an "end if" to mark the end of if the if in "end if" is getting counted. How to handle that.
    cmplx_cond_list.push_back("if");
    cmplx_cond_list.push_back("for"); // add for for pl/sql @ 10/30/14
    //add @ 10/23/14
    cmplx_cond_list.push_back("EXISTS");
    cmplx_cond_list.push_back("DISTINCT");
    cmplx_cond_list.push_back("ASC");
    cmplx_cond_list.push_back("DESC");

    cmplx_logic_list.push_back("=");
    cmplx_logic_list.push_back("!=");
    cmplx_logic_list.push_back("<>");
    cmplx_logic_list.push_back(">");
    cmplx_logic_list.push_back("<");
    cmplx_logic_list.push_back(">=");
    cmplx_logic_list.push_back("<=");
    cmplx_logic_list.push_back("and");
    cmplx_logic_list.push_back("or");
    cmplx_logic_list.push_back("not");
    cmplx_logic_list.push_back("like");

    cmplx_preproc_list.push_back("dictionary");

    //cmplx_assign_list.push_back("=");
    // For PL/SQL
    cmplx_assign_list.push_back(":=");
    //cmplx_assign_list.push_back("IS");


    cmplx_cyclomatic_list.push_back("if");
    cmplx_cyclomatic_list.push_back("elsif");
    cmplx_cyclomatic_list.push_back("when");
    cmplx_cyclomatic_list.push_back("while");
    cmplx_cyclomatic_list.push_back("for");

    cmplx_cyclomatic_logic_list.push_back("or");
    cmplx_cyclomatic_logic_list.push_back("and");

    cmplx_cyclomatic_case_list.push_back("when");
    cmplx_cyclomatic_switch_list.push_back("case");

    ignore_cmplx_cyclomatic_list.push_back("end if");
    ignore_cmplx_cyclomatic_list.push_back("end case");
}

/*!
* Replaces quoted strings inside a string starting at idx_start with '$'.
* Handles special cases for SQL literal strings.
*
* \param strline string to be processed
* \param idx_start index of line character to start search
* \param contd specifies the quote string is continued from the previous line
* \param CurrentQuoteEnd end quote character of the current status
*
* \return method status
*/
int CSqlCounter::ReplaceQuote(string &strline, size_t &idx_start, bool &contd, char &CurrentQuoteEnd)
{
    size_t idx = string::npos;
	bool done = false;
    while ( ! done )
    {
        idx = strline.find("''", idx_start);
        if (idx != string::npos)
            strline.replace(idx, 2, 2, '$');
        else
            break;
    }
    while ( ! done )
    {
        idx = strline.find("\"\"", idx_start);
        if (idx != string::npos)
            strline.replace(idx, 2, 2, '$');
        else
            break;
    }
    return CCodeCounter::ReplaceQuote(strline, idx_start, contd, CurrentQuoteEnd);
}

/*!
* Processes physical and logical lines according to language specific rules.
* NOTE: all the blank lines +
*               whole line comments +
*               lines with compiler directives
*       should have been blanked from filemap by previous processing
*       before reaching this function
*
* \param fmap list of processed file lines
* \param result counter results
* \param fmapBak list of original file lines (same as fmap except it contains unmodified quoted strings)
*
* \return method status
*/
int CSqlCounter::LanguageSpecificProcess(filemap* fmap, results* result, filemap* fmapBak)
{
    size_t i, j, k, ind, nextInd;
    unsigned int sloc_count, dsloc_count, lineNum;
    string stmt, stmtBak, exec_keyword, data_keyword;
    filemap::iterator fit, fitbak;

    unsigned int data_count = 0;
    unsigned int exec_count = 0;
    bool    trunc_flag      = false;
    string  exclude         = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$";
    string  line            = "";
    string  lineBak         = "";
    bool    data_continue   = false;
    string  strLSLOC        = "";
    string  strLSLOCBak     = "";
    size_t lineNumber = 0;

    vector<string> nestedSql, nestedSqlBak;
    stack<size_t> pistack, ppstack;
    size_t mapi = 0, pi, pp;

    // process physical SLOC and capture embedded SLOC
    for (fit = fmap->begin(), fitbak = fmapBak->begin(); fit != fmap->end(); fit++, fitbak++)
    {
        // insert blank at the beginning(for searching keywords)
        line = ' ' + fit->line;
        lineBak = ' ' + fitbak->line;
        lineNumber = fit->lineNumber;

        if (CUtil::CheckBlank(line))
        {
            mapi++;
            continue;
        }

        // check physical lines
        exec_count = 0;
        CUtil::CountTally(line, exec_name_list, exec_count, 1, exclude, "", "", &result->exec_name_count, false);
        data_count = 0;
        CUtil::CountTally(line, data_name_list, data_count, 1, exclude, "", "", &result->data_name_count, false);
        if (exec_count > 0 || (data_count <= 0 && !data_continue))
        {
            data_continue = false;
            result->exec_lines[PHY]++;
        }
        else
        {
            data_continue = true;
            result->data_lines[PHY]++;
        }

        for (i = 1; i < line.length(); i++)
        {
            if (line[i] == '(')
            {
                pistack.push(mapi);
                ppstack.push(i - 1);
            }
            else if (line[i] == ')' && pistack.size() > 0)
            {
                // capture embedded SLOC
                pi = pistack.top();
                pp = ppstack.top();
                pistack.pop();
                ppstack.pop();
                if (pi == mapi)
                {
                    stmt = fit->line.substr(pp, i - pp);
                    stmtBak = fitbak->line.substr(pp, i - pp);
                }
                else
                {
                    stmt = fmap->at(pi).line.substr(pp);
                    stmtBak = fmapBak->at(pi).line.substr(pp);
                    for (j = pi + 1; j < mapi; j++)
                    {
                        stmt += ' ' + fmap->at(j).line;
                        stmtBak += ' ' + fmapBak->at(j).line;
                    }
                    stmt += ' ' + fit->line.substr(0, i);
                    stmtBak += ' ' + fitbak->line.substr(0, i);
                }

                // check for executable statement keywords
                sloc_count = 0;
                dsloc_count = 0;
                CUtil::CountTally(stmt, exec_name_list, sloc_count, 1, exclude, "", "", NULL, false);
                if (sloc_count > 0)
                {
                    // extract embedded SLOC from main content
                    if (pi == mapi)
                    {
                        fit->line.erase(pp, i - pp);
                        fitbak->line.erase(pp, i - pp);
                        line.erase(pp + 1, i - pp - 1);
                        lineBak.erase(pp + 1, i - pp - 1);
                        i = pp + 1;
                    }
                    else
                    {
                        fmap->at(pi).line.erase(pp);
                        fmapBak->at(pi).line.erase(pp);
                        for (j = pi + 1; j < mapi; j++)
                        {
                            fmap->at(j).line.clear();
                            fmapBak->at(j).line.clear();
                        }
                        fit->line.erase(0, i);
                        fitbak->line.erase(0, i);
                        line.erase(1, i - 1);
                        lineBak.erase(1, i - 1);
                        i = 1;
                    }
                    stmt = CUtil::TrimString(stmt.substr(1, stmt.length() - 2));
                    stmtBak = CUtil::TrimString(stmtBak.substr(1, stmtBak.length() - 2));
                    nestedSql.push_back(stmt);
                    nestedSqlBak.push_back(stmtBak);
                }
                else
                {
                    CUtil::CountTally(stmt, data_name_list, dsloc_count, 1, exclude, "", "", NULL, false);
                    if (dsloc_count > 0)
                    {
                        // mark data keywords (not counted as LSLOC)
                        for (j = 0; j < data_name_list.size(); j++)
                        {
                            ind = 0;
                            nextInd = 0;
                            data_keyword = data_name_list.at(j);
                            while (ind != string::npos)
                            {
                                ind = CUtil::FindKeyword(stmt, data_keyword, nextInd, TO_END_OF_STRING, false);
                                if (ind != string::npos)
                                {
                                    stmt.replace(ind, 1, "$");
                                    nextInd = ind + 1;
                                }
                            }
                            ind = pp;
                            nextInd = pp;
                            if (pi == mapi)
                            {
                                while (ind != string::npos)
                                {
                                    ind = CUtil::FindKeyword(fit->line, data_keyword, nextInd, i - 1, false);
                                    if (ind != string::npos)
                                    {
                                        fit->line.replace(ind, 1, "$");
                                        nextInd = ind + 1;
                                    }
                                }
                            }
                            else
                            {
                                while (ind != string::npos)
                                {
                                    ind = CUtil::FindKeyword(fmap->at(pi).line, data_keyword, nextInd, TO_END_OF_STRING, false);
                                    if (ind != string::npos)
                                    {
                                        fmap->at(pi).line.replace(ind, 1, "$");
                                        nextInd = ind + 1;
                                    }
                                }
                                for (k = pi + 1; k < mapi; k++)
                                {
                                    ind = 0;
                                    nextInd = 0;
                                    while (ind != string::npos)
                                    {
                                        ind = CUtil::FindKeyword(fmap->at(k).line, data_keyword, nextInd, TO_END_OF_STRING, false);
                                        if (ind != string::npos)
                                        {
                                            fmap->at(k).line.replace(ind, 1, "$");
                                            nextInd = ind + 1;
                                        }
                                    }
                                }
                                ind = 0;
                                nextInd = 0;
                                while (ind != string::npos)
                                {
                                    ind = CUtil::FindKeyword(fit->line, data_keyword, nextInd, i - 1, false);
                                    if (ind != string::npos)
                                    {
                                        fit->line.replace(ind, 1, "$");
                                        nextInd = ind + 1;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        mapi++;
    }
    data_continue = false;

    // add embedded SLOC to file maps
    if (nestedSql.size() > 0)
    {
        lineNum = fmap->back().lineNumber;
        for (i = 0; i < nestedSql.size(); i++)
        {
            lineNum++;
            lineElement element(lineNum, nestedSql.at(i));
            fmap->push_back(element);
            lineElement elementBak(lineNum, nestedSqlBak.at(i));
            fmapBak->push_back(elementBak);
        }
        nestedSql.clear();
        nestedSqlBak.clear();
    }

    // process logical SLOC
    for (fit = fmap->begin(), fitbak = fmapBak->begin(); fit != fmap->end(); fit++, fitbak++)
    {
        // insert blank at the beginning(for searching keywords)
        line = ' ' + fit->line;
        lineBak = ' ' + fitbak->line;

        if (CUtil::CheckBlank(line))
            continue;
        lineNumber = fit->lineNumber;
        // process logical SLOC
        LSLOC(result, line, lineNumber, lineBak, strLSLOC, strLSLOCBak, data_continue);
    }
    if (strLSLOC.length() > 0)
    {
        if (result->addSLOC(strLSLOCBak, lineNumber, trunc_flag))
        {
            if (data_continue)
                result->data_lines[LOG]++;
            else
                result->exec_lines[LOG]++;
        }
        strLSLOC = strLSLOCBak = "";
        data_continue = false;
    }
    if (classtype == UNKNOWN || classtype == DATAFILE)
            return 0;
    // Cyclomatic complexity analysis for PL/SQL constructs starts
        size_t idx;
        string file_ext,function_name = "";;
        unsigned int  cyclomatic_cnt = 0, ignore_cyclomatic_cnt = 0, main_cyclomatic_cnt = 0, function_count = 0, cyclomatic_logic_cnt = 0, main_cyclomatic_logic_cnt = 1, cyclomatic_case_cnt = 0, main_cyclomatic_case_cnt = 1, cyclomatic_default_cnt = 0, cyclomatic_switch_cnt = 0;
        filemap function_stack;
        stack<unsigned int> cyclomatic_stack;
        map<unsigned int, lineElement> function_map;
        map<unsigned int, lineElement> logical_map;
        map<unsigned int, lineElement> case_map;
        bool process_cyclomatic_complexity = false;
        // check whether to process cyclomatic complexity
        if (cmplx_cyclomatic_list.size() > 0)
        {
            process_cyclomatic_complexity = true;
            if (skip_cmplx_cyclomatic_file_extension_list.size() > 0)
            {
                idx = result->file_name.find_last_of(".");
                if (idx != string::npos)
                {
                    file_ext = result->file_name.substr(idx);
                    file_ext = CUtil::ToLower(file_ext);
                    if (find(skip_cmplx_cyclomatic_file_extension_list.begin(), skip_cmplx_cyclomatic_file_extension_list.end(), file_ext) != skip_cmplx_cyclomatic_file_extension_list.end())
                        process_cyclomatic_complexity = false;
                }
            }
        }
        for (fit = fmap->begin(); fit != fmap->end(); fit++)
        {
            line = fit->line;

            if (CUtil::CheckBlank(line))
                continue;

            line = " " + line;

            // cyclomatic complexity
            if (process_cyclomatic_complexity)
            {
                // search for cyclomatic complexity keywords
                CUtil::CountTally(line, cmplx_cyclomatic_list, cyclomatic_cnt, 1, exclude, "", "", 0, casesensitive);

                CUtil::CountTally(line, cmplx_cyclomatic_switch_list, cyclomatic_switch_cnt, 1, exclude, "", "", 0, casesensitive);

                // search for keywords to exclude
                if (ignore_cmplx_cyclomatic_list.size() > 0)
                    CUtil::CountTally(line, ignore_cmplx_cyclomatic_list, ignore_cyclomatic_cnt, 1, exclude, "", "", 0, casesensitive);

                // search for cyclomatic complexity logical keywords
                if (cmplx_cyclomatic_logic_list.size() > 0)
                    CUtil::CountTally(line, cmplx_cyclomatic_logic_list, cyclomatic_logic_cnt, 1, exclude, "", "", 0, casesensitive);

                // search for cyclomatic complexity case keywords
                if (cmplx_cyclomatic_case_list.size() > 0)
                    CUtil::CountTally(line, cmplx_cyclomatic_case_list, cyclomatic_case_cnt, 1, exclude, "", "", 0, casesensitive);

                // search for cyclomatic complexity case default keywords
                if (cmplx_cyclomatic_default_list.size() > 0)
                    CUtil::CountTally(line, cmplx_cyclomatic_default_list, cyclomatic_default_cnt, 1, exclude, "", "", 0, casesensitive);

                if(cyclomatic_default_cnt > 0)
                {
                    cyclomatic_cnt -= cyclomatic_default_cnt;
                    cyclomatic_case_cnt -= cyclomatic_default_cnt;
                    cyclomatic_default_cnt = 0;
                }

                    if (main_cyclomatic_cnt < 1)
                          main_cyclomatic_cnt = 1;  // add 1 for main function here in case no other decision points are found in main
                // PL/SQL code doesn't belong to any function. It is treated as a single main function.
                main_cyclomatic_cnt += cyclomatic_cnt - ignore_cyclomatic_cnt;
                main_cyclomatic_logic_cnt += cyclomatic_cnt - ignore_cyclomatic_cnt + cyclomatic_logic_cnt;
                main_cyclomatic_case_cnt += cyclomatic_cnt - ignore_cyclomatic_cnt - cyclomatic_case_cnt + cyclomatic_switch_cnt;
                cyclomatic_cnt = ignore_cyclomatic_cnt = cyclomatic_logic_cnt = cyclomatic_case_cnt = cyclomatic_switch_cnt = 0;


            }
        }

        // done with a file
        if (main_cyclomatic_cnt > 0)
        {
            // add "main" code
            lineElement element(main_cyclomatic_cnt, "main");
            lineElement n_element(main_cyclomatic_logic_cnt, "main");
            lineElement c_element(main_cyclomatic_case_cnt, "main");
            function_map[0] = element;
            logical_map[0] = n_element;
            case_map[0] = c_element;
        }
        else
        {
            // finish the first function if not closed
            while (!function_stack.empty())
            {
                function_name = function_stack.back().line;
                function_count = function_stack.back().lineNumber;
                function_stack.pop_back();

                if (!function_stack.empty())
                {
                    // grab previous function from stack to continue
                    if (!cyclomatic_stack.empty())
                    {
                        cyclomatic_cnt = cyclomatic_stack.top();
                        cyclomatic_stack.pop();
                    }
                }
                else
                {
                    // capture count at end of function
                    lineElement element(cyclomatic_cnt - ignore_cyclomatic_cnt + 1, function_name);
                    lineElement n_element(cyclomatic_cnt - ignore_cyclomatic_cnt + cyclomatic_logic_cnt + 1, function_name);
                    lineElement c_element(cyclomatic_cnt - ignore_cyclomatic_cnt - cyclomatic_case_cnt + cyclomatic_switch_cnt + 1, function_name);
                    function_map[function_count] = element;
                    logical_map[function_count] = n_element;
                    case_map[function_count] = c_element;
                }
            }
        }

        // process ordered functions
        for (map<unsigned int, lineElement>::iterator it = function_map.begin(); it != function_map.end(); ++it)
            result->cmplx_cycfunct_count.push_back(it->second);
        if(cmplx_cyclomatic_logic_list.size() > 0)
        {
            for (map<unsigned int, lineElement>::iterator it = logical_map.begin(); it != logical_map.end(); ++it)
                result->cmplx_cycfunct_CC2_count.push_back(it->second);
        }
        if(cmplx_cyclomatic_case_list.size() > 0)
        {
            for (map<unsigned int, lineElement>::iterator it = case_map.begin(); it != case_map.end(); ++it)
                result->cmplx_cycfunct_CC3_count.push_back(it->second);
        }
    return 1;
}

/*!
* Extracts and stores logical lines of code.
* Determines and extract logical SLOC to place in the result variable
* using addSLOC function. Each time the addSLOC function is called,
* a new logical SLOC is added. This function assumes that the directive
* is handled before it is called.
*
* \param result counter results
* \param line processed physical line of code
* \param lineBak original physical line of code
* \param strLSLOC processed logical string
* \param strLSLOCBak original logical string
* \param data_continue continuation of a data declaration line
*/
void CSqlCounter::LSLOC(results* result, string line, size_t lineNumber, string lineBak, string &strLSLOC, string &strLSLOCBak, bool &data_continue)
{
    size_t i, ind, nextInd, startInd, endInd, strSize;
    ptrdiff_t j;
    bool trunc_flag = false, found;
    string exec_keyword, data_keyword, cmplx_assign_keyword;
    list<size_t> slocIndices, eslocIndices, dslocIndices, gslocIndices;
	
	/* Modification Fall 2016
		1> PreProcess the line 
		2> Handles the case where "var1:=1" is modified to "var1 := 1"
		3> The change is required so that LSLOC Count is as per UCC documentation 
	*/
	char *cs = (char *)malloc(sizeof(char *) * (line.length() + 1));
	memset(cs, 0, sizeof(char *) * (line.length() + 1));
    for (i=0;i<line.length();i++) {
        cs[i] = line[i];
    }
    //Modification: 2018.01, changed data type to size_t
    size_t len = strlen(cs);
    std::stringstream ss;
    std::map<size_t, std::string> cmplx_assign_positions;
    for(std::vector<std::string>::iterator it = cmplx_assign_list.begin(); it != cmplx_assign_list.end(); it ++) {
        i = 0;	
        while ((i=line.find(*it, i)) != std::string::npos) {
            memset(&cs[i], 0, (*it).length());
            cmplx_assign_positions[i] = *it;
            i += (*it).length();
        }
    }
    i = 0;
    while (i < len) {
        if (cs[i] != 0) {
            ss << &cs[i];
            i += strlen(&cs[i]);
        } else {
            if (cmplx_assign_positions.find(i) != cmplx_assign_positions.end()) 
                ss << " "<< cmplx_assign_positions[i] <<" ";
            i++;
        }
    }
    free(cs);
	line = ss.str();
	/*
		Modification Fall 2016 Ends
	*/

    // find locations of executable keywords
    for (i = 0; i < exec_name_list.size(); i++)
    {
        ind = 0;
        nextInd = 0;
        exec_keyword = exec_name_list.at(i);
        while (ind != string::npos)
        {
            ind = CUtil::FindKeyword(line, exec_keyword, nextInd, TO_END_OF_STRING, false);
            if (ind != string::npos)
            {
                // check for grant, revoke, deny
                if (exec_keyword == "grant" || exec_keyword == "revoke" || exec_keyword == "deny")
                {
                    // ignore GRANT OPTION
                    if (line.length() <= ind + 12 || CUtil::ToLower(line.substr(ind, 12)) != "grant option")
                        gslocIndices.push_back(ind);
                }

                nextInd = ind + 1;

                //For PL/SQL, when it's "end if", ignore and not count it
                if (exec_keyword == "if")
                {
                    if (ind >= 4)
                    {
                        string ifStr = CUtil::ToLower(line.substr(ind - 4, 6));
                        if (ifStr == "end if") {
                            continue;
                        }
                    }
                }

                //For PL/SQL, when it's "end loop", ignore and not count it
                if (exec_keyword == "loop")
                {
                    if (ind >= 4)
                    {
                        string ifStr = CUtil::ToLower(line.substr(ind - 4, 8));
                        if (ifStr == "end loop") {
                            continue;
                        }
                    }
                }

                //For PL/SQL, when it's "create or replace", count only once
                if (exec_keyword == "create")
                {
                    if (line.length() >= ind + 17) {
                        string crtStr = CUtil::ToLower(line.substr(ind, 17));
                        if (crtStr == "create or replace") {
                            continue;
                        }
                    }
                }

                //For PL/SQL, when it's return + data name, ignore and not count it
                if (exec_keyword == "return")
                {
                    bool flag = false;
                    int startIndex = 0;
                    int endIndex = 0;
                    for(unsigned int i = ind + 6; i<line.length(); i++) {
                        char c = line[i]; //this is your character
                        if (c != ' ') {
                            if (!flag) {
                                flag = true;
                                startIndex = (int)i;
                            }
                        }

                        if (c == ' ' || c == '('){
                            if (flag) {
                                endIndex = (int)i;
                                break;
                            }
                        }
                    }

                    if (endIndex != 0) {
                        string dataStr = line.substr( (unsigned)startIndex, (unsigned)( endIndex - startIndex ) );
                        cout << dataStr;
                        if (std::find(data_name_list.begin(), data_name_list.end(), CUtil::ToLower(dataStr)) != data_name_list.end()) {
                            continue;
                        }
                    }
                }

                slocIndices.push_back(ind);
                eslocIndices.push_back(ind);
            }
        }
    }

    // For PL/SQL, find locations of complex assignment list
    for (i = 0; i < cmplx_assign_list.size(); i++) {
        ind = 0;
        nextInd = 0;
        cmplx_assign_keyword = cmplx_assign_list.at(i);
        while (ind != string::npos)
        {
            ind = CUtil::FindKeyword(line, cmplx_assign_keyword, nextInd, TO_END_OF_STRING, false);
            if (ind != string::npos) {
                slocIndices.push_back(ind);
                eslocIndices.push_back(ind);
            }
            nextInd = ind + 1;
        }
    }



    eslocIndices.sort();
    eslocIndices.unique();

    // process grant, revoke, deny
    if (gslocIndices.size() > 0)
    {
        slocIndices.sort();
        slocIndices.unique();
    }
    while (gslocIndices.size() > 0)
    {
        ind = gslocIndices.front() + 1;
        gslocIndices.pop_front();

        // search for ON, TO, FROM until first non-exec keyword found and clear slocIndices in between
        // (ideally this check would span multiple lines, but we avoid this for now)
        if (ind < line.length())
        {
            nextInd = CUtil::FindKeyword(line, "on", ind, TO_END_OF_STRING, false);
            if (nextInd == string::npos)
            {
                nextInd = CUtil::FindKeyword(line, "to", ind, TO_END_OF_STRING, false);
                if (nextInd == string::npos)
                {
                    nextInd = CUtil::FindKeyword(line, "from", ind, TO_END_OF_STRING, false);
                    if (nextInd == string::npos)
                        nextInd = line.length();
                }
            }
            // clear any slocIndices between these values
            list<size_t>::iterator it, it2;
            for (it = eslocIndices.begin(), it2 = slocIndices.begin(); it != eslocIndices.end(); ++it, ++it2)
            {
                if (*it >= ind && *it < nextInd)
                {
                    *it = INVALID_POSITION;
                    *it2 = INVALID_POSITION;
                }
            }
            eslocIndices.remove(INVALID_POSITION);
            slocIndices.remove(INVALID_POSITION);
        }
    }

    // find locations of data keywords

    if (CUtil::FindKeyword(line, ":=", 0, TO_END_OF_STRING, false) == string::npos && CUtil::FindKeyword(line, "RETURN", 0, TO_END_OF_STRING, false) == string::npos) {
        for (i = 0; i < data_name_list.size(); i++)
        {
            ind = 0;
            nextInd = 0;
            data_keyword = data_name_list.at(i);
            while (ind != string::npos)
            {
                ind = CUtil::FindKeyword(line, data_keyword, nextInd, TO_END_OF_STRING, false);
                if (ind != string::npos)
                {
                    // try to get variable name (var name is listed before type in SQL)
                    nextInd = ind + 1;
                    found = false;
                    for ( j = (ptrdiff_t)ind - 1; j >= 0; j-- )
                    {
                        if (line[(unsigned)j] != ' ' && line[(unsigned)j] != '\t')
                            found = true;
                        else if (found && (line[(unsigned)j] == ' ' || line[(unsigned)j] == '\t' || line[(unsigned)j] == ','))
                        {
                            ind = (size_t)j + 1;
                            break;
                        }
                    }
                    if (found && j <= 0)
                        ind = 1;
                    slocIndices.push_back(ind);
                    dslocIndices.push_back(ind);
                }
            }
        }

    }

    dslocIndices.sort();
    dslocIndices.unique();

    // process total set of keywords
    slocIndices.sort();
    slocIndices.unique();
    startInd = 0;
    while (slocIndices.size() > 0)
    {
        // get current keyword index
        startInd = slocIndices.front();
        slocIndices.pop_front();
        if (slocIndices.size() > 0)
            endInd = slocIndices.front();
        else
            endInd = string::npos;

        // process continuation
        if (strLSLOCBak.length() > 0)
        {
            strSize = CUtil::TruncateLine(startInd, strLSLOC.length(), this->lsloc_truncate, trunc_flag);
            if (strSize > 0)
            {
                strLSLOC += line.substr(0, strSize);
                strLSLOCBak += lineBak.substr(0, strSize);
            }
            if (result->addSLOC(strLSLOCBak, lineNumber, trunc_flag))
            {
                if (data_continue)
                    result->data_lines[LOG]++;
                else
                    result->exec_lines[LOG]++;
            }
            strLSLOC = strLSLOCBak = "";
        }
        data_continue = false;

        // determine keyword type
        if (eslocIndices.size() > 0 && eslocIndices.front() == startInd)
            eslocIndices.pop_front();
        else
        {
            dslocIndices.pop_front();
            data_continue = true;
        }

        // process LSLOC
        if (endInd != string::npos)
        {
            strSize = CUtil::TruncateLine(endInd - startInd, 0, this->lsloc_truncate, trunc_flag);
            if (strSize > 0)
            {
                strLSLOC = line.substr(startInd, strSize);
                strLSLOCBak = lineBak.substr(startInd, strSize);
            }
            if (result->addSLOC(strLSLOCBak, lineNumber, trunc_flag))
            {
                if (data_continue)
                    result->data_lines[LOG]++;
                else
                    result->exec_lines[LOG]++;
            }
            strLSLOC = strLSLOCBak = "";
            startInd = endInd;
        }
    }

    // capture continuing LSLOC
    if (startInd < line.length())
    {
        strSize = CUtil::TruncateLine(line.length() - startInd, strLSLOC.length(), this->lsloc_truncate, trunc_flag);
        if (strSize > 0)
        {
            strLSLOC += line.substr(startInd, strSize);
            strLSLOCBak += lineBak.substr(startInd, strSize);
        }
    }
}

/*!
* Constructs a CSqlColdFusionCounter object.
*/
CSqlColdFusionCounter::CSqlColdFusionCounter()
{
    classtype = SQL_CFM;
    language_name = "SQL/ColdFusion";

    file_extension.clear();
    file_extension.push_back(".*sqlcfm");
}

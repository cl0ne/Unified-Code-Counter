//! Code counter class methods for the Cobol language.
/*!
* \file CCobolCounter.cpp
*
* This file contains the code counter class methods for the Cobol language.
*
* References:
* Wikipedia COBOL language
*
* COBOL 85 Grammer  Downloaded grammer text file   COBOL85.grm
*    Revision: 09-13-06: by Devin Cook
*
* COBOL grammar Version 0.1.1.pdf
*    1999 by Ralf Lammel
*
* Created: 2011?   (not sure when or who, not released?)  Contributors: QINGYU ZHOU, Nipun, PEI-CHEN
* Revised: 2015.12 by Randy Maxwell
*/

#include "CCobolCounter.h"

/*!
* Constructs a CCobolCounter object.
*/
CCobolCounter::CCobolCounter()
{
    classtype = COBOL;
    language_name = "COBOL";
    
    //Modification: 11.2016 Ext-4
    file_extension = CUtil::getExtensionsToLanguage("COBOL", file_extension);
    /*
    file_extension.push_back(".cbl");
    file_extension.push_back(".cob");
	file_extension.push_back(".cpy");*/

    QuoteStart = "\"'";
    QuoteEnd = "\"'";
    //casesensitive = false;

    exclude_keywords.push_back("ELSE");
    exclude_keywords.push_back("END-EVALUATE");
    exclude_keywords.push_back("END-IF");
	exclude_keywords.push_back("END-IF.");
    exclude_keywords.push_back("END-PERFORM");
    exclude_keywords.push_back("END-SEARCH");
    exclude_keywords.push_back("THEN");

    // added by Nipun
    exclude_keywords.push_back("EXCEPTION");
    exclude_keywords.push_back("ON EXCEPTION");
    exclude_keywords.push_back("NOT ON EXCEPTION");
    exclude_keywords.push_back("END-XML");
    // -- FINISHED ADDING

	sort( exclude_keywords.begin(), exclude_keywords.end() );


    exclude_start_keywords.push_back("END");
    exclude_start_keywords.push_back("STOP");


    //In COBOL, there are only Line Comments
    // and they start with either * or /
    //An asterisk (*) comment line is printed in the output listing, immediately following the last preceding line.
    //A slash (/) comment line is printed on the first line of the next page, and the current page of the output listing is ejected.
    // There are no Block comments in COBOL
    LineCommentStart.push_back("*");
    LineCommentStart.push_back("/");
    LineCommentStart.push_back("'");

    // === ADDED BY QINGYU ZHOU ===
    directive.push_back("BASIS");
    directive.push_back("DELETE");
    directive.push_back("REPLACE");
    directive.push_back("RESET");
    directive.push_back("CBL");
    directive.push_back("EJECT");
    directive.push_back("SERVICE");
    directive.push_back("LABEL");
    directive.push_back("PROCESS");
    directive.push_back("ENTER");
    directive.push_back("TITLE");
    directive.push_back("RELOAD");
    directive.push_back("CONTROL");
    directive.push_back("INSERT");
    directive.push_back("DEBUGGING");
    directive.push_back("COPY");
    directive.push_back("TRACE");
    directive.push_back("READY");
    directive.push_back("PROCEDURE");
    // === FINISHED ADDING ===

    // Added by Nipun
    directive.push_back("*CONTROL");
    directive.push_back("*CBL");
    directive.push_back("*CONTROL");
    directive.push_back("SKIP1");
    directive.push_back("SKIP2");
    directive.push_back("SKIP3");
    directive.push_back("SERVICE RELOAD");
    directive.push_back("RESET TRACE");
    // FINISHED ADDING

    directive.push_back("USE");
    directive.push_back("USING");

	// Sort for better presentation of results
	sort( directive.begin(), directive.end() );

    data_name_list.push_back("PIC");
    data_name_list.push_back("PICTURE");

	exec_name_list.push_back("ACCEPT");			// 2015.12
    exec_name_list.push_back("ACCESS");
    exec_name_list.push_back("ADVANCING");
    exec_name_list.push_back("AFTER");
    exec_name_list.push_back("ALL");
	exec_name_list.push_back("ARE");			// 2015.12
	exec_name_list.push_back("ALPHABETIC");		// 2015.12
	exec_name_list.push_back("ALPHANUMERIC");	// 2015.12
	exec_name_list.push_back("ALPHANUMERIC-EDITED");	// 2015.12
    exec_name_list.push_back("ALTER");
	exec_name_list.push_back("ALTERNATIVE");	// 2015.12
	exec_name_list.push_back("ASCENDING");		// 2015.12
    exec_name_list.push_back("ASSIGN");
    exec_name_list.push_back("AT");
    exec_name_list.push_back("AUTHOR");
	exec_name_list.push_back("BACKGROUND-COLOR");	// 2015.12
    exec_name_list.push_back("BEGIN");
    exec_name_list.push_back("BEFORE");
	exec_name_list.push_back("BINARY");			// 2015.12
	exec_name_list.push_back("BLANK");			// 2015.12
	exec_name_list.push_back("BLINK");			// 2015.12
    exec_name_list.push_back("BLOCK");
	exec_name_list.push_back("BOTTOM");			// 2015.12
    exec_name_list.push_back("BY");
    exec_name_list.push_back("CALL");
    exec_name_list.push_back("CANCEL");
	exec_name_list.push_back("CD");				// 2015.12
	exec_name_list.push_back("CF");				// 2015.12
	exec_name_list.push_back("CH");				// 2015.12
	exec_name_list.push_back("CHARACTER");		// 2015.12
	exec_name_list.push_back("CHARACTERS");		// 2015.12
    exec_name_list.push_back("CLASS");
    exec_name_list.push_back("CLASS-ID");
	exec_name_list.push_back("CLOCK-UNITS");	// 2015.12
    exec_name_list.push_back("CLOSE");
    exec_name_list.push_back("CODE-SET");
    exec_name_list.push_back("COLLATING");
	exec_name_list.push_back("COLUMN");			// 2015.12
    exec_name_list.push_back("COMMON");
    exec_name_list.push_back("COMP");
	exec_name_list.push_back("COMPUTATIONAL");	// 2015.12

	// ADD BY PEI-CHEN
	exec_name_list.push_back("COMPUTE");
	// FINISH ADDING 

    exec_name_list.push_back("CONFIGURATION");
	exec_name_list.push_back("CONSOLE");		// 2015.12
	exec_name_list.push_back("CONTAINS");		// 2015.12
    exec_name_list.push_back("CONTENT");
	exec_name_list.push_back("CONTINUE");		// 2015.12
	exec_name_list.push_back("CONTROL");		// 2015.12
	exec_name_list.push_back("CONTROLS");		// 2015.12
	exec_name_list.push_back("CORR");			// 2015.12
	exec_name_list.push_back("CORRESPONDING");	// 2015.12
	exec_name_list.push_back("COUNT");			// 2015.12
    exec_name_list.push_back("CURRENCY");
    exec_name_list.push_back("DATA");
    exec_name_list.push_back("DATE-COMPILED");
    exec_name_list.push_back("DATE-WRITTEN");
    exec_name_list.push_back("DAY");
	exec_name_list.push_back("DAY-OF-WEEK");	// 2015.12
	exec_name_list.push_back("DE");				// 2015.12
    exec_name_list.push_back("DEBUGGING");
    exec_name_list.push_back("DECIMAL-POINT");
    exec_name_list.push_back("DECLARATIVES");
    exec_name_list.push_back("DELETE");
	exec_name_list.push_back("DELIMITED");		// 2015.12
    exec_name_list.push_back("DELIMITER");
	exec_name_list.push_back("DEPENDING");		// 2015.12
	exec_name_list.push_back("DESCENDING");		// 2015.12
	exec_name_list.push_back("DISABLE");		// 2015.12
    exec_name_list.push_back("DISPLAY");
    exec_name_list.push_back("DIVISION");
	exec_name_list.push_back("DUPLICATES");		// 2015.12
	exec_name_list.push_back("ENABLE");			// 2015.12
    exec_name_list.push_back("END");
	exec_name_list.push_back("END-ADD");		// 2015.12
	exec_name_list.push_back("END-CALL");		// 2015.12
	exec_name_list.push_back("END-COMPUTE");	// 2015.12
	exec_name_list.push_back("END-DELETE");		// 2015.12
	exec_name_list.push_back("END-DIVIDE");		// 2015.12
	exec_name_list.push_back("END-EVALUATE");	// 2015.12
	exec_name_list.push_back("END-MULTIPLY");	// 2015.12
    exec_name_list.push_back("END-OF-FILE");
	exec_name_list.push_back("END-OF-PAGE");	// 2015.12
	exec_name_list.push_back("END-REWRITE");	// 2015.12
	exec_name_list.push_back("END-STRING");		// 2015.12
	exec_name_list.push_back("END-SUBTRACT");	// 2015.12
	exec_name_list.push_back("END-UNSTRING");	// 2015.12
	exec_name_list.push_back("ENVIRONMENT");
	exec_name_list.push_back("EOP");			// 2015.12
    exec_name_list.push_back("ERROR");
    exec_name_list.push_back("EVALUATE");		// 2015.12
	exec_name_list.push_back("EVERY");			// 2015.12
	exec_name_list.push_back("EXCEPTION");		// 2015.12
    exec_name_list.push_back("EXIT");
	exec_name_list.push_back("EXTEND");			// 2015.12
	exec_name_list.push_back("EXTERNAL");		// 2015.12
    exec_name_list.push_back("FACTORY");
	exec_name_list.push_back("FD");				// 2015.12
    exec_name_list.push_back("FILE");
    exec_name_list.push_back("FILE-CONTROL");
    exec_name_list.push_back("FILLER");
	exec_name_list.push_back("FINAL");			// 2015.12
    exec_name_list.push_back("FOOTING");
	exec_name_list.push_back("FOR");			// 2015.12
	exec_name_list.push_back("FOREGROUND-COLOR");	// 2015.12
    exec_name_list.push_back("FROM");
    exec_name_list.push_back("FUNCTION");
    exec_name_list.push_back("GENERATE");
	exec_name_list.push_back("GIVING");			// 2015.12
	exec_name_list.push_back("GLOBAL");			// 2015.12
    exec_name_list.push_back("GO");
    exec_name_list.push_back("GOTO");
	exec_name_list.push_back("GROUP");			// 2015.12
	exec_name_list.push_back("HEADING");		// 2015.12
	exec_name_list.push_back("HIGH-VALUE");		// 2015.12
    exec_name_list.push_back("HIGH-VALUES");
	exec_name_list.push_back("HIGHLIGHT");		// 2015.12
    exec_name_list.push_back("ID");
    exec_name_list.push_back("IDENTIFICATION");
    exec_name_list.push_back("IF");
	exec_name_list.push_back("IN");				// 2015.12
    exec_name_list.push_back("INDEXED");
	exec_name_list.push_back("INDICATE");		// 2015.12
    exec_name_list.push_back("INHERITS");
    exec_name_list.push_back("INITIAL");
    exec_name_list.push_back("INITIATE");
	exec_name_list.push_back("INPUT");			// 2015.12
    exec_name_list.push_back("INPUT-OUTPUT");
	exec_name_list.push_back("INSPECT");		// 2015.12
    exec_name_list.push_back("INSTALLATION");
	exec_name_list.push_back("INTERNAL");		// 2015.12
    exec_name_list.push_back("INVALID");
    exec_name_list.push_back("INVOKE");
    exec_name_list.push_back("IS");
	exec_name_list.push_back("I-O");			// 2015.12
    exec_name_list.push_back("I-O-CONTROL");
	exec_name_list.push_back("JUST");			// 2015.12
	exec_name_list.push_back("JUSTIFIED");		// 2015.12
    exec_name_list.push_back("KEY");
    exec_name_list.push_back("LABEL");
    exec_name_list.push_back("LABEL-OF");
	exec_name_list.push_back("LEADING");		// 2015.12
	exec_name_list.push_back("LEFT");			// 2015.12
	exec_name_list.push_back("LIMIT");			// 2015.12
	exec_name_list.push_back("LIMITS");			// 2015.12
	exec_name_list.push_back("LINAGE");			// 2015.12
    exec_name_list.push_back("LINE");
    exec_name_list.push_back("LINES");
    exec_name_list.push_back("LINKAGE");
    exec_name_list.push_back("LOCAL-STORAGE");
	exec_name_list.push_back("LOCK");			// 2015.12
	exec_name_list.push_back("LOW-VALUE");		// 2015.12
	exec_name_list.push_back("LOW-VALUES");		// 2015.12
    exec_name_list.push_back("MERGE");
	exec_name_list.push_back("MESSAGE");		// 2015.12
    exec_name_list.push_back("METHOD-ID");
    exec_name_list.push_back("MODE");
    exec_name_list.push_back("MOVE");
	exec_name_list.push_back("MULTIPLE");		// 2015.12
	exec_name_list.push_back("NATIVE");			// 2015.12
	exec_name_list.push_back("NEXT");			// 2015.12
    exec_name_list.push_back("NOT-END-OF-FILE");
	exec_name_list.push_back("NULL");			// 2015.12
	exec_name_list.push_back("NULLS");			// 2015.12
	exec_name_list.push_back("NUMBER");			// 2015.12
	exec_name_list.push_back("NUMERIC");		// 2015.12
	exec_name_list.push_back("NUMERIC-EDITED");	// 2015.12
    exec_name_list.push_back("OBJECT");
    exec_name_list.push_back("OBJECT-COMPUTER");
    exec_name_list.push_back("OCCURS");
	exec_name_list.push_back("OF");				// 2015.12
	exec_name_list.push_back("OFF");			// 2015.12
	exec_name_list.push_back("OMITTED");		// 2015.12
	exec_name_list.push_back("ON");				// 2015.12
    exec_name_list.push_back("OPEN");
	exec_name_list.push_back("ORDER");			// 2015.12
	exec_name_list.push_back("OUTPUT");			// 2015.12
    exec_name_list.push_back("ORGANIZATION");
	exec_name_list.push_back("OVERFLOW");		// 2015.12
	exec_name_list.push_back("PACKED-DECIMAL");	// 2015.12
    exec_name_list.push_back("PADDING");
	exec_name_list.push_back("PAGE");			// 2015.12
    exec_name_list.push_back("PASSWORD");
    exec_name_list.push_back("PERFORM");
	exec_name_list.push_back("PF");				// 2015.12
	exec_name_list.push_back("PH");				// 2015.12
	exec_name_list.push_back("POINTER");		// 2015.12			<<<<   Pointer like C++ ? ? ?
	exec_name_list.push_back("POSITION");		// 2015.12
	exec_name_list.push_back("PRINTING");		// 2015.12
    exec_name_list.push_back("PROCEDURE");
	exec_name_list.push_back("PROCEDURES");		// 2015.12
    exec_name_list.push_back("PROGRAM");
    exec_name_list.push_back("PROGRAM-ID");
	exec_name_list.push_back("QUOTE");			// 2015.12
	exec_name_list.push_back("QUOTES");			// 2015.12
    exec_name_list.push_back("READ");
    exec_name_list.push_back("RECORD");
	exec_name_list.push_back("RECORDS");		// 2015.12
    exec_name_list.push_back("RECORDING");
    exec_name_list.push_back("RECURSIVE");
	exec_name_list.push_back("REDEFINES");		// 2015.12
    exec_name_list.push_back("REFERENCE");
	exec_name_list.push_back("REFERENCES");		// 2015.12
    exec_name_list.push_back("RELATIVE");
    exec_name_list.push_back("RELEASE");
	exec_name_list.push_back("REMOVAL");		// 2015.12
	exec_name_list.push_back("RENAMES");		// 2015.12
	exec_name_list.push_back("REPORT");			// 2015.12
    exec_name_list.push_back("REPORTING");
	exec_name_list.push_back("RERUN");			// 2015.12
    exec_name_list.push_back("RESERVE");
	exec_name_list.push_back("RESET");			// 2015.12
    exec_name_list.push_back("RETURN");
	exec_name_list.push_back("REVERSE-VIDEO");	// 2015.12
	exec_name_list.push_back("REWIND");			// 2015.12
    exec_name_list.push_back("REWRITE");
	exec_name_list.push_back("RF");				// 2015.12
	exec_name_list.push_back("RH");				// 2015.12
	exec_name_list.push_back("RIGHT");			// 2015.12
	exec_name_list.push_back("ROUNDED");		// 2015.12
	exec_name_list.push_back("SCREEN");			// 2015.12
	exec_name_list.push_back("SD");				// 2015.12

	// ADD BY PEI-CHEN
	exec_name_list.push_back("SEARCH");
	// FINISH ADDING 

    exec_name_list.push_back("SECTION");
    exec_name_list.push_back("SECURITY");
    exec_name_list.push_back("SEGMENT-LIMIT");
    exec_name_list.push_back("SELECT");
	exec_name_list.push_back("SEPARATE");		// 2015.12
    exec_name_list.push_back("SEQUENCE");
    exec_name_list.push_back("SEQUENTIAL");
	exec_name_list.push_back("SIGN");			// 2015.12
	exec_name_list.push_back("SIZE");			// 2015.12
    exec_name_list.push_back("SORT");
	exec_name_list.push_back("SORT-MERGE");		// 2015.12
    exec_name_list.push_back("SOURCE-COMPUTER");
	exec_name_list.push_back("SPACE");			// 2015.12
    exec_name_list.push_back("SPACES");
	exec_name_list.push_back("SPECIAL-NAMES");	// 2015.12
	exec_name_list.push_back("STANDARD");		// 2015.12
	exec_name_list.push_back("STANDARD-1");		// 2015.12
	exec_name_list.push_back("STANDARD-2");		// 2015.12
    exec_name_list.push_back("START");
    exec_name_list.push_back("STATUS");
	exec_name_list.push_back("STRING");			// 2015.12
	exec_name_list.push_back("STOP");			// 2015.12
	exec_name_list.push_back("SUPPRESS");		// 2015.12
	exec_name_list.push_back("SUM");			// 2015.12
	exec_name_list.push_back("SYMBOLIC");		// 2015.12
	exec_name_list.push_back("SYNC");			// 2015.12
	exec_name_list.push_back("SYNCHRONIZED");	// 2015.12
	exec_name_list.push_back("TABLE");			// 2015.12
	exec_name_list.push_back("TABLES");			// 2015.12
	exec_name_list.push_back("TAPE");			// 2015.12
    exec_name_list.push_back("TERMINATE");
	exec_name_list.push_back("THAN");			// 2015.12
	exec_name_list.push_back("THEN");			// 2015.12
	exec_name_list.push_back("THROUGH");		// 2015.12
    exec_name_list.push_back("THRU");
    exec_name_list.push_back("TIME");
    exec_name_list.push_back("TIMES");
    exec_name_list.push_back("TO");
	exec_name_list.push_back("TOP");			// 2015.12
	exec_name_list.push_back("TRAILING");		// 2015.12
	exec_name_list.push_back("TRUE");			// 2015.12
	exec_name_list.push_back("UNDERLINE");		// 2015.12
	exec_name_list.push_back("UNSTRING");		// 2015.12
    exec_name_list.push_back("UNTIL");
	exec_name_list.push_back("UPON");			// 2015.12
    exec_name_list.push_back("USE");
	exec_name_list.push_back("USING");			// 2015.12
    exec_name_list.push_back("VALUE");
	exec_name_list.push_back("VALUES");			// 2015.12
    exec_name_list.push_back("VARYING");
    exec_name_list.push_back("WHEN");
    exec_name_list.push_back("WITH");
    exec_name_list.push_back("WORKING-STORAGE");
    exec_name_list.push_back("WRITE");

	// ADD BY PEI-CHEN
	exec_name_list.push_back("XML-CODE");
	exec_name_list.push_back("XML-EVENT");	
	exec_name_list.push_back("XML PARSE");
	exec_name_list.push_back("XML-TEXT");
	// FINISH ADDING 

	exec_name_list.push_back("ZERO");
    exec_name_list.push_back("ZEROS");
	exec_name_list.push_back("ZEROES");

	sort( exec_name_list.begin(), exec_name_list.end() );

    math_func_list.push_back("-");
    math_func_list.push_back("*");
    math_func_list.push_back("**");
    math_func_list.push_back("/");
    math_func_list.push_back("+");
    math_func_list.push_back("ADD");
    math_func_list.push_back("COMPUTE");
    math_func_list.push_back("DIVIDE");
    math_func_list.push_back("MULTIPLY");
    math_func_list.push_back("SUBTRACT");
    math_func_list.push_back("REMAINDER");

    cmplx_logic_list.push_back("<");
    cmplx_logic_list.push_back("<=");
    cmplx_logic_list.push_back("=");
    cmplx_logic_list.push_back(">");
    cmplx_logic_list.push_back(">=");
    cmplx_logic_list.push_back("EQUAL");
    cmplx_logic_list.push_back("GREATER");
    cmplx_logic_list.push_back("LESS");
    cmplx_logic_list.push_back("NOT");
    cmplx_logic_list.push_back("NOT=");
    cmplx_logic_list.push_back("OR");
        
    // added by Nipun
    cmplx_logic_list.push_back("XML GENERATE");
    cmplx_logic_list.push_back("XML PARSE");
    //---finished adding

    cmplx_cond_list.push_back("ELSE IF");
    cmplx_cond_list.push_back("EVALUATE");
    cmplx_cond_list.push_back("IF");
    cmplx_cond_list.push_back("PERFORM");
    cmplx_cond_list.push_back("SEARCH");
    cmplx_cond_list.push_back("STOP");
    cmplx_cond_list.push_back("THEN");
    cmplx_cond_list.push_back("WHEN");

    cmplx_assign_list.push_back("ACCEPT");
    cmplx_assign_list.push_back("COMPUTE");
    cmplx_assign_list.push_back("GENERATE");
    cmplx_assign_list.push_back("INITIALIZE");
    cmplx_assign_list.push_back("INSPECT");
    cmplx_assign_list.push_back("MOVE");
    cmplx_assign_list.push_back("PARSE");
    cmplx_assign_list.push_back("SET");
    cmplx_assign_list.push_back("STRING");
    cmplx_assign_list.push_back("UNSTRING");
    cmplx_assign_list.push_back("XML");


    cmplx_cyclomatic_list.push_back("ELSE IF");
    cmplx_cyclomatic_list.push_back("END-EVALUATE");
    cmplx_cyclomatic_list.push_back("END-IF");
    cmplx_cyclomatic_list.push_back("END-PERFORM");
    cmplx_cyclomatic_list.push_back("END-SEARCH");
    cmplx_cyclomatic_list.push_back("EVALUATE");
    cmplx_cyclomatic_list.push_back("IF");
    cmplx_cyclomatic_list.push_back("PERFORM");
    cmplx_cyclomatic_list.push_back("SEARCH");
    cmplx_cyclomatic_list.push_back("STOP RUN");
    cmplx_cyclomatic_list.push_back("STOP");


// These used to be in ProcessComplexityMeasures in the old 2011 (never released) COBOL parser
//
	// subtract "End If" since "If" captured within "End If" and we also have in-line "If"
	// subtract "Case Else" because it is captured by "Case" and should not be counted
	ignore_cmplx_cyclomatic_list.push_back("END-EVALUATE");
	ignore_cmplx_cyclomatic_list.push_back("END-PERFORM");
	ignore_cmplx_cyclomatic_list.push_back("END-SEARCH");
	ignore_cmplx_cyclomatic_list.push_back("END-IF");
	ignore_cmplx_cyclomatic_list.push_back("STOP RUN");


	// EVALUATE is like switch



}
/*!
* Counts directive lines of code.
*
* \param fmap list of processed file lines
* \param result counter results
* \param fmapBak list of original file lines (same as fmap except it contains unmodified quoted strings)
*
* \return method status
*/
int CCobolCounter::CountDirectiveSLOC(filemap* fmap, results* result, filemap* /* fmapBak */)
{
    bool contd = false, trunc_flag = false;
    size_t idx, strSize;
    if (classtype == DATAFILE) return 0;
    unsigned int cnt = 0;
    string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$";
    string strDirLine = "";

    for (filemap::iterator iter = fmap->begin(); iter!=fmap->end(); iter++)
    {
        if ( CUtil::CheckBlank( iter->line ) )
            continue;

		// === ADDED BY QINGYU ZHOU ===
		TrimSourceLine( iter->line );
		// === FINISHED ADDING ===

        if ( print_cmplx )
        {
            cnt = 0;
            CUtil::CountTally(" " + iter->line, directive, cnt, 1, exclude, "", "", &result->directive_count);
        }

        if (!contd)
        {
            // if not a continuation of a previous directive
            for(vector<string>::iterator viter = directive.begin(); viter != directive.end(); viter++)
            {
                if ((idx = iter->line.find((*viter), 0)) != string::npos && idx == 0)
                {
                    contd = true;
                    break;
                }
            }
            if (contd)
            {
                result->directive_lines[PHY]++;
                strSize = CUtil::TruncateLine(iter->line.length(), 0, result->trunc_lines, trunc_flag);
                if (strSize > 0)
                    strDirLine = iter->line.substr(0, strSize);
            }
        }
        else
        {
            // continuation of a previous directive
            strSize = CUtil::TruncateLine(iter->line.length(), strDirLine.length(), result->trunc_lines, trunc_flag);
            if (strSize > 0)
                strDirLine += "\n" + iter->line.substr(0, strSize);
            result->directive_lines[PHY]++;
        }
    
        if (contd)
        {
            // if a directive or continuation of a directive (no continuation symbol found)
            if (iter->line[iter->line.length()-1] != '_')
            {
                contd = false;
                result->directive_lines[LOG]++;
                result->addSLOC(strDirLine, trunc_flag);
            }
            iter->line = "";
        }
    }
    return 1;
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
int CCobolCounter::LanguageSpecificProcess(filemap* fmap, results* result, filemap* fmapBak)
{
	int		retVal = 1;
    string strLSLOC = "";
    string strLSLOCBak = "";

    filemap::iterator fit, fitbak;
    string line, lineBak;
    size_t prev_pos, strSize, tmpLoc;
    unsigned int cnt = 0;
    StringVector loopEnd;
    string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$";
    string special = "[]()+/*<>=,@&~!^?:%{}";

    string tmpstr;
    bool isDataLine = false;
//    bool line_continued = false;
    bool line_skipped;
    bool trunc_flag = false;
    bool new_loop = false;

	SAVE_TO_2( "Start CCobolCounter::CountComplexity" );
	currentPhyLine = 0;
	for (fit = fmap->begin(), fitbak = fmapBak->begin(); fit != fmap->end(); fit++, fitbak++)
    {
		currentPhyLine++;
        line = fit->line;
        if (CUtil::CheckBlank(line))
            continue;

		lineBak = fitbak->line;
        prev_pos = 0;

        // check for inline If..Then
        tmpstr = CUtil::TrimString(line);
        tmpLoc = CUtil::FindKeyword(tmpstr, "THEN");
        if (tmpLoc != string::npos)
        {
            if (tmpLoc < tmpstr.length() - 4)
                tmpLoc += 3;
            else
                tmpLoc = string::npos;
        }

        // record nested loops
        if ( print_cmplx )
        {
            new_loop = false;
            if (CUtil::FindKeyword(tmpstr, "PERFORM") == 0)
            {
                loopEnd.push_back("END-PERFORM");
                new_loop = true;
            }
            else if (loopEnd.size() > 0)
            {
                if (CUtil::FindKeyword(tmpstr, loopEnd.back()) == 0)
                    loopEnd.pop_back();
            }
            if (new_loop)
            {
                if ( result->cmplx_nestloop_count.size() < loopEnd.size() )
                    result->cmplx_nestloop_count.push_back(1);
                else
                    result->cmplx_nestloop_count[loopEnd.size()-1]++;
            }
        }

        for (size_t i = 0; i < line.size(); i++)
        {
            if ((line[i] == ':') || (i == line.size() - 1) || tmpLoc != string::npos)
            {
                if (tmpLoc != string::npos) 
                {
                    i = tmpLoc;
                    tmpLoc = string::npos;
                }
                else
                    tmpstr = CUtil::TrimString(line.substr(prev_pos, i - prev_pos + 1));

                // exclude SLOC defined in the exclude_keywords
                line_skipped = false;
                for (vector<string>::iterator stri = exclude_keywords.begin(); stri != exclude_keywords.end(); stri++)
                {
                    if (tmpstr.compare(*stri) == 0)
                    {
                        line_skipped = true;
                        break;
                    }
                }
                if (line_skipped)
                    continue;

                // exclude SLOC starting with Next, End
                for (StringVector::iterator stri = exclude_start_keywords.begin(); stri != exclude_start_keywords.end(); stri++)
                {
                    if (CUtil::FindKeyword(tmpstr, *stri) == 0)                                 
                    {
                        line_skipped = true;
                        break;
                    }
                }
                if (line_skipped)
                    continue;

                strSize = CUtil::TruncateLine(i + 1 - prev_pos, strLSLOC.length(), result->trunc_lines, trunc_flag);
                if (strSize > 0)
                {
                    strLSLOC += line.substr(prev_pos, strSize);
                    strLSLOCBak += lineBak.substr(prev_pos, strSize);
                }

                cnt = 0;
                CUtil::CountTally(strLSLOC, data_name_list, cnt, 1, exclude, "", "", &result->data_name_count);
                if (cnt > 0)
                {
                    isDataLine = true;
                    result->data_lines[LOG]++;
                }
                else
                {
                    if ( print_cmplx )
                    {
                        cnt = 0;
                        CUtil::CountTally(strLSLOC, exec_name_list, cnt, 1, exclude, "", "", &result->exec_name_count);
                    }
                    isDataLine = false;
                    result->exec_lines[LOG]++;
                }

                result->addSLOC( strLSLOCBak, trunc_flag );  // add one logical SLOC to the list
                strLSLOCBak = "";
                strLSLOC = "";

                prev_pos = i + 1;

                if (tmpLoc != string::npos)
                {
                    tmpLoc = string::npos;
                    prev_pos++;
                }
            }

            if (special.find_first_of(line[i]) != string::npos)
                line[i] = ' ';
        }

        if (isDataLine)
            result->data_lines[PHY]++;
        else
            result->exec_lines[PHY]++;
    }

/* Not needed?  Done as  ignore_cmplx_cyclomatic_list
	if ( print_cmplx )
		retVal = ProcessComplexityMeasures( fmap, result );
*/

	return retVal;
}

/*!
* Processes file language complexity measures.
*
* \param fmap list of processed file lines
* \param result counter results
*
* \return method status
*/
/*  COMMENTED OUT.  Now the keywords are in  ignore_cmplx_cyclomatic_list

int CCobolCounter::ProcessComplexityMeasures( filemap* fmap, results* result )
{
	// process Cyclomatic Complexity (CC)  from above COBOL has Cyclomatic Complexity keywords with no skip

	SAVE_TO_3( "Start CCobolCounter::ProcessComplexityMeasures" );

	result->cyclomatic_complexity = 1;
	StringVector::iterator icyc = cmplx_cyclomatic_list.begin();
	for (vector<int>::iterator icycc = result->cmplx_cyclomatic_count.begin(); icycc != result->cmplx_cyclomatic_count.end(); icycc++, icyc++)
	{
		if ((*icyc) == "END-EVALUATE" || (*icyc) == "END-PERFORM" || (*icyc) == "END-SEARCH" || (*icyc) == "END-IF" || (*icyc) == "STOP RUN")
		{
			// subtract "End If" since "If" captured within "End If" and we also have in-line "If"
			// subtract "Case Else" because it is captured by "Case" and should not be counted
			result->cyclomatic_complexity -= (*icycc);
		}
		else
			result->cyclomatic_complexity += (*icycc);
	}
	if ( result->cyclomatic_complexity < 0 )
		result->cyclomatic_complexity = 0;

    return 1;
} */


int CCobolCounter::TrimSourceLine(string& line)
{
	for (unsigned int i = 0; i < line.length() - 1; i++)
	{
		if (line.at(i) == ' ')
		{
			int countOfSpaces;
			for ( countOfSpaces = 1; i + countOfSpaces < line.length(); countOfSpaces++ )
			{
				if (line.at(i + countOfSpaces) != ' ')
					break;
			}
			line.erase( i, (unsigned)countOfSpaces - 1 );
		}
	}
	TrimSequenceNumber( line );
	if ( TrimIndicator( line ) == 1 )
			TrimAreaA( line );
	return 1;
}

int CCobolCounter::TrimSequenceNumber( string& line )
{
	if ( line.length() < 6)
			return 0;

	for (int i = 0; i < 6; i++)
	{
		if ( isdigit( line.at( (unsigned)i ) ) == false )
			return 0;
	}

	line.erase( 0, 6 );
	return 1;
}

int CCobolCounter::TrimIndicator( string& line )
{
	if ( line.length() > 0 && line.at( 0 ) == ' ' )
	{
		line.erase( 0, 1 );
		return 1;
	}

	return 0;

}

int CCobolCounter::TrimAreaA( string& line )
{
	if ( line.length() < 3 )
		return 0;

	if ( line.at(2) == ' ' && isalnum( line.at( 0 ) ) && isalnum( line.at( 1 ) ) )
		line.erase( 0, 3 );

	return 1;
}

/*!
* Counts file language complexity based on specified language keywords/characters.
*
* \param fmap list of processed file lines
* \param result counter results
*
* \return method status
*/
int CCobolCounter::CountComplexity( filemap* fmap, results* result )
{
	filemap::iterator fit;
	unsigned int cnt, cyclomatic_cnt = 0, cyclomatic_logic_cnt = 0, ignore_cyclomatic_cnt = 0;
	//int tmpc;
	string line;
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$";
	bool line_skipped = false;
//	bool process_cyclomatic_complexity = true;	// from above COBOL has Complexity keywords and no skip

	ENTER_1( "Start" );		// Due to Polymorphism change level above
//	try
//	{

	SAVE_TO_2( "Start CCobolCounter::CountComplexity" );
	currentPhyLine = 0;
	for ( fit = fmap->begin(); fit != fmap->end(); fit++ )
	{
		currentPhyLine++;
		line = fit->line;
		SAVE_TO_2( "CUtil::CheckBlank" );
		if ( CUtil::CheckBlank( line ) )
			continue;

		// exclude keywords in exclude_keywords
		line_skipped = false;
		for (StringVector::iterator stri = exclude_keywords.begin(); stri != exclude_keywords.end(); stri++)
		{
			if (CUtil::FindKeyword(line, *stri) == 0)
			{
				line_skipped = true;
				break;
			}
		}

		if ( line_skipped )
			continue;

		line = " " + line;

		// mathematical functions
		cnt = 0;
		SAVE_TO_2( "CUtil::CountTally Math" );
		CUtil::CountTally(line, math_func_list, cnt, 1, exclude, "", "", &result->math_func_count, casesensitive);
		result->cmplx_math_lines += cnt;

		// trigonometric functions
		cnt = 0;
		SAVE_TO_2( "CUtil::CountTally Trigonometric" );
		CUtil::CountTally(line, trig_func_list, cnt, 1, exclude, "", "", &result->trig_func_count, casesensitive);
		result->cmplx_trig_lines += cnt;

		// logarithmic functions
		cnt = 0;
		SAVE_TO_2( "CUtil::CountTally Logarithmic" );
		CUtil::CountTally(line, log_func_list, cnt, 1, exclude, "", "", &result->log_func_count, casesensitive);
		result->cmplx_logarithm_lines += cnt;

		// calculations
		cnt = 0;
		SAVE_TO_2( "CUtil::CountTally Calculations" );
		CUtil::CountTally(line, cmplx_calc_list, cnt, 1, exclude, "", "", &result->cmplx_calc_count, casesensitive);
		result->cmplx_calc_lines += cnt;

		// conditionals
		cnt = 0;
		SAVE_TO_2( "CUtil::CountTally Conditionals" );
		CUtil::CountTally(line, cmplx_cond_list, cnt, 1, exclude, "", "", &result->cmplx_cond_count, casesensitive);
		result->cmplx_cond_lines += cnt;

		//tmpc = (int)CUtil::FindKeyword( line, "END-IF" );
		//result->cmplx_cond_lines = 0;
		

		// logical operators
		cnt = 0;
		SAVE_TO_2( "CUtil::CountTally Logical" );
		CUtil::CountTally(line, cmplx_logic_list, cnt, 1, exclude, "", "", &result->cmplx_logic_count, casesensitive);
		result->cmplx_logic_lines += cnt;

		// preprocessor directives
		cnt = 0;
		SAVE_TO_2( "CUtil::CountTally Preprocessor" );
		CUtil::CountTally(line, cmplx_preproc_list, cnt, 1, exclude, "", "", &result->cmplx_preproc_count, casesensitive);
		result->cmplx_preproc_lines += cnt;

		// assignments
		cnt = 0;
		SAVE_TO_2( "CUtil::CountTally Assignments" );
		CUtil::CountTally(line, cmplx_assign_list, cnt, 1, exclude, "", "", &result->cmplx_assign_count, casesensitive);
		result->cmplx_assign_lines += cnt;

	/* Pointers not in COBOL
		// pointers
		cnt = 0;
		SAVE_TO_2( "CUtil::CountTally Pointers" );
		// Pointers are embedded syntax so there is NO exclude string or include strings
		CUtil::CountTally(line, cmplx_pointer_list, cnt, 1, "", "", "", &result->cmplx_pointer_count, casesensitive);
		result->cmplx_pointer_lines += cnt;
	*/

		// Cyclomatic Complexity keywords
		//cnt = 0;
		//CUtil::CountTally(line, cmplx_cyclomatic_list, cnt, 1, exclude, "", "", &result->cmplx_cyclomatic_count, casesensitive);
		// search for Cyclomatic Complexity keywords
		SAVE_TO_2( "CUtil::CountTally Cyclomatic Complexity" );
		CUtil::CountTally(line, cmplx_cyclomatic_list, cyclomatic_cnt, 1, exclude, "", "", 0, casesensitive);

		// COBOL has Cyclomatic Complexity keywords to ignore
		if ( ignore_cmplx_cyclomatic_list.size() > 0 )
		{
			SAVE_TO_2( "CUtil::CountTally exclude keywords" );
			CUtil::CountTally( line, ignore_cmplx_cyclomatic_list, ignore_cyclomatic_cnt, 1, exclude, "", "", 0, casesensitive );
		}

		// COBOL has cyclomatic complexity logical keywords
		SAVE_TO_2( "CUtil::CountTally complexity Logical" );
		CUtil::CountTally(line, cmplx_cyclomatic_logic_list, cyclomatic_logic_cnt, 1, exclude, "", "", 0, casesensitive);



	}

/*
	} 
	catch(const std::exception& e)
	{
		string	dumpInfo;
		string	what = e.what();
		StackDump( dumpInfo, EXCEPTION_HANDLER_PARSER, this->parse_threadIdx, EXCEPTION_STD_EXCEPTION, what, this );
		throw( e );
	}
*/
	return 1;
}

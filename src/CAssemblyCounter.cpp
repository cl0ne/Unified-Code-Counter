//! Code counter class methods for assembly languages.
/*!
* \file CAssemblyCounter.cpp
*
* This file contains the code counter class methods for the assembly languages.
* To-do: Globalize changeMode using struct
*/

#include "CAssemblyCounter.h"

/*!
* Constructs a CAssemblyCounter object.
*/
CAssemblyCounter::CAssemblyCounter()
{
    classtype = ASSEMBLY;
    language_name = "Assembly";
    
    //Modification: 11.2016 Ext-4 starts
    file_extension = CUtil::getExtensionsToLanguage("Assembly", file_extension);
    
    /*file_extension.push_back(".asm");
    file_extension.push_back(".s");
    file_extension.push_back(".asm.ppc");*/
    //Modification: 11.2016 Ext-4 ends

    statementSeparator = ";"; // assume it is ';' unless it conflicts with comment marker

    BlockCommentStart.push_back("/*");
    BlockCommentEnd.push_back("*/");
    ContinueLine = "\\";

    // .data & .bss section declaration storage mapping classes, which define data declaration
    // see documentation: http://publib.boulder.ibm.com/infocenter/pseries/v5r3/topic/com.ibm.aix.aixassem/doc/alangref/csect.htm#wyl350ken
    data_declaration_code.push_back("RW");
    data_declaration_code.push_back("TC0");
    data_declaration_code.push_back("TC");
    data_declaration_code.push_back("TD");
    data_declaration_code.push_back("UA");
    data_declaration_code.push_back("DS");
    data_declaration_code.push_back("BS");
    data_declaration_code.push_back("UC");
    data_declaration_code.push_back("ER");
    data_declaration_code.push_back("SD");
    data_declaration_code.push_back("LD");
    data_declaration_code.push_back("CM");
}

/*!
* Perform preprocessing of file lines before counting.
*
* \param fmap list of file lines
*
* \return method status
*/
int CAssemblyCounter::PreCountProcess(filemap* fmap)
{
    FindLineCommentMarker(fmap);
    return 0;
}

/*!
* Detect the character(s) that is used for marking a line as a comment line.
* If such a character is found, it is added to LineCommentStart vector.
* Since assembly code counter covers multiple assembly languages, there can be
* mulitple markers in the same program. 
* GNU Assembler, for instance, supports # and ; for comment lines
*
* \param fmap list of file lines
*
*/
void CAssemblyCounter::FindLineCommentMarker(filemap* fmap)
{
    StringVector markerChecklist;   // contains line comment markers that need to be checked/tested
    markerChecklist.push_back("#");
    markerChecklist.push_back(";");
    markerChecklist.push_back("|");

    LineCommentStart.clear();       // remove all existing values just in case it was set unintentionally

    /* Algorithm:
        1.	Read each line
            a.	For each comment marker, find position/index of the marker
                i.	If position is 0, a marker is found
                    1.	Save/add the marker: LineCommentStart.push_back()
                    2.	Remove the marker from the check list
                ii.	If position is > 0 && not last index && (position-1) is a space, a marker is found.
                    1.	Save/add the marker: LineCommentStart.push_back()
                    2.	Remove the marker from the check list
    */
    StringVector::iterator nextMarker;
    string line;
    size_t position;
    for (filemap::iterator fit = fmap->begin(); fit != fmap->end(); fit++)
    {
        line = fit->line;
        nextMarker = markerChecklist.begin();
        while (nextMarker != markerChecklist.end())
        {
            position = line.find(*nextMarker);
            if (position == string::npos)
            {
                nextMarker++;
                continue;
            }
            else if (position == 0)
            {
                LineCommentStart.push_back(*nextMarker);
                nextMarker = markerChecklist.erase(nextMarker);
            }
            else if (position > 0 && position != (line.length() - 1) && line.at(position - 1) == ' ')
            {
                LineCommentStart.push_back(*nextMarker);
                nextMarker = markerChecklist.erase(nextMarker);
            }
            else
                nextMarker++;
        }
    }
}

/*!
* Processes physical and logical lines according to language specific rules.
* NOTE: all the blank lines +
*               whole line comments
*       should have been blanked from filemap by previous processing
*       before reaching this function
*
* \param fmap list of processed file lines
* \param result counter results
* \param fmapBak list of original file lines (same as fmap except it contains unmodified quoted strings)
*
* \return method status
*/
int CAssemblyCounter::LanguageSpecificProcess(filemap* fmap, results* result, filemap* /*fmapBak*/)
{
    string logicalLine, nextPhysicalLine, physicalLine;
    unsigned int i;
    int changeSectionMode;
    bool isDataSection = false;
    bool isDataDeclarationLine = false; // specifies whether a given single line is the declaration of a data section (i.e. '.data')
    bool hasLineContinuation = false;
    int countPhysicalLine = 0;

    for (filemap::iterator iter = fmap->begin(); iter != fmap->end(); iter++)
    {
        // A physical line does not contain trailing or leading spaces.
        // nextPhysicalLine may contains multiple logical statements separated by a separator. 
        // nextPhysicalLine may end with a line continuation character. 
        // In this case, nextPhysicalLine will be constructed as multiple physical lines. 
        nextPhysicalLine = iter->line;

        if (!hasLineContinuation)
            physicalLine = "";

        // do not process blank lines - blank line means blank_line/comment_line
        if (!CUtil::CheckBlank(nextPhysicalLine))
        {
            countPhysicalLine++;    // keep track of the number of physical lines (blank is not a physical line)
            if (CUtil::EndsWith(nextPhysicalLine, "\\"))
            {
                physicalLine += nextPhysicalLine.substr(0, nextPhysicalLine.length() - 1) + " ";
                hasLineContinuation = true;
                continue;           // go to next physical line
            }
            else
            {
                physicalLine += nextPhysicalLine;
                hasLineContinuation = false;
            }
            isDataDeclarationLine = false;

            // since a physical line may contain multiple logical lines, check for statement separator
            StringVector tokens = CUtil::Split(physicalLine, statementSeparator);
            for (i = 0; i < tokens.size(); i++)
            {
                logicalLine = CUtil::TrimString(tokens[i], 0);  // one logical line
                changeSectionMode = SwitchSectionMode(logicalLine);
                if (changeSectionMode == 1)
                    isDataSection = false;                      // false means it is in executable section, not data section
                else if (changeSectionMode == 2)
                {
                    isDataSection = true;                       // true means it has entered a data section of the program
                    isDataDeclarationLine = true;               // declaration of a data section (i.e. '.data') itself will not be counted as data line, but counted as executable
                }

                /*
                Some Logical SLOC Counting Rules:
                1) MyLabel: instr 				1 LSLOC since instruction is present on the same line as the label
                2) MyLabel: 					0 LSLOC. Label without instruction is not an executable statement
                3) .instruction param			1 LSLOC
                4) .instruction					1 LSLOC. Some instructions do not require parameters
                5) # 							0 LSLOC since comment line. Comment lines have been taken care of in previous processing. No processing needed here
                6) ; 							0 LSLOC. Logical statement separator. Program will parse into multiple lines for processing
                7) instr						1 LSLOC. If a line passes all of the above filter/exception, it is an executable line

                Reference: http://www.ibm.com/developerworks/library/l-ppc/
                */

                // The strategy is to figure out what should be excluded from counting (these are known as counting exceptions).
                // Those lines that are not excluded will be assumed to be instructions; thus, they will be counted.

                // 1. First exception to look for is mylabel: 
                //    Rationale: label without instruction is not counted as a logical SLOC
                if (CUtil::EndsWith(logicalLine, ":", true))
                    continue;

                // 2. Do not count the following NASM statements: %endmacro, %endif, %endrep
                if (CUtil::StartsWith(logicalLine, "%end", true))
                    continue;

                // 3. Do not count the following MASM statements: endm, endp, ends, endw, end, .end, .endw
                //    and do not count the following GAS statements: .end, .endm, .endef, .endfunc, .endif, .endloop
                if (CUtil::StartsWith(logicalLine, ".end", true) || CUtil::StartsWith(logicalLine, "end", true))
                    continue;

                // anything that has passed the above exceptions is a logical SLOC that will be counted as 1 instruction
                if (isDataSection && !isDataDeclarationLine)
                    result->data_lines[LOG]++;
                else
                    result->exec_lines[LOG]++;

                bool trunc_flag = false;
                CUtil::TruncateLine(physicalLine.length(), logicalLine.length(), this->lsloc_truncate, trunc_flag);
                result->addSLOC(logicalLine, trunc_flag);   // add SLOC so that diff can be performed
            }

            if (isDataSection && !isDataDeclarationLine)
                result->data_lines[PHY] += countPhysicalLine;
            else
                result->exec_lines[PHY] += countPhysicalLine;

            countPhysicalLine = 0;
        }
    }
    return 1;
}

/*!
* Determine whether or not to switch to text mode, data mode or no change from previous setting.
*
* \param statement statement text
*
* \return 0 for no change, 1 for text section, and 2 for data section.
*/
int CAssemblyCounter::SwitchSectionMode(const string &statement)
{
    const int noChange = 0;
    const int textSection = 1;
    const int dataSection = 2;

    string lStatement = CUtil::ToLower(statement);

    // 1. Data section usually starts with .data (or its variances) or .bss, while code section starts with .text

    if (lStatement.substr(0, 13) == "section .data" || lStatement.substr(0, 12) == "section .bss" ||                            // NASM syntax for data section declaration
        lStatement.substr(0, 5) == ".data" || lStatement.substr(0, 6) == ".const" ||                                            // MASM & GAS syntax for data section
        lStatement.substr(0, 4) == ".bss" ||                                                                                    // GAS syntax here & below
        (lStatement.substr(0, 9) == ".section " &&                                                                              // find: .section name["flag"]. This is GAS syntax
            (lStatement.find("\"d\"", 10) != string::npos || lStatement.find("\'d\'", 10) != string::npos ||                    // find: "d" or 'd'
             lStatement.find("\"b\"", 10) != string::npos || lStatement.find("\'b\'", 10) != string::npos)) ||                  //    or "b" or 'b'
        lStatement.substr(0, 6) == ".rdata" || lStatement.substr(0, 6) == ".sdata" || lStatement.substr(0, 6) == ".kdata" ||    // MIPS syntax
        lStatement.substr(0, 5) == ".sbss" || lStatement.substr(0, 4) == ".lit")                                                //.lit8 and .lit4 are data sections for MIPS syntax
    {
        return dataSection;
    }
    else if (lStatement.substr(0, 13) == "section .text" ||                              // NASM syntax for executable section declaration
             lStatement.substr(0, 12) == "section .txt" ||                               // NASM syntax only
             lStatement.substr(0, 5) == ".code" ||                                       // MASM syntax only
             lStatement.substr(0, 14) == ".section .text" ||                             // GAS syntax
             lStatement.substr(0, 5) == ".text" ||                                       // GAS, PowerPC, MIPS
             lStatement.substr(0, 5) == ".init" || lStatement.substr(0, 5) == ".fini" || // MIPS syntax
             lStatement.substr(0, 6) == ".ktext")                                        // SPIM MIPS syntax
    {
        return textSection;
    }
    else if (lStatement.substr(0, 6) == ".csect")   // only apply to PowerPC
    {
        // 2. A text section and a data section can also be defined by .csect instruction.
        // Whether .csect is data or text depends on the given storage mapping classes.
        // Data section can also be defined by .csect [name][storageMappingClass], where
        // storageMappingClass is one of classes for .data or .bss.
        // See .csect documentation: http://publib.boulder.ibm.com/infocenter/pseries/v5r3/topic/com.ibm.aix.aixassem/doc/alangref/csect.htm#wyl350ken

        // since both [] and {} are supported, easier to convert {} to [] so that there is only one thing to deal with

        lStatement = CUtil::ReplaceWith(CUtil::ReplaceWith(lStatement, "{", "["), "}", "]");

        StringVector::iterator vit;
        for (vit = data_declaration_code.begin(); vit != data_declaration_code.end(); vit++)
        {
            string strToFind = "[" + CUtil::ToLower(*vit) + "]";
            if (lStatement.find(strToFind, 6) != string::npos)
                return dataSection;
        }
        return textSection; // it was a .csect, but not a data .csect. Therefore, it is assumed to be a text .csect
    }
    return noChange;        // if it is neither data nor text, mode change is NOT needed
}

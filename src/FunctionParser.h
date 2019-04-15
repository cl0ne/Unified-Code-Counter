//! FunctionParser class
/*!
* \file FunctionParser.h
*
* This file contains the function parsing class methods definition.
*/

#ifndef FUNCTIONPARSER_H
#define FUNCTIONPARSER_H

#include "cc_main.h"

using namespace std;

class FunctionParser
{
public:
    FunctionParser();
    ~FunctionParser();
    void callParser(string filePath, string dirName, ClassType classTypeOfFile);
    void pythonParser(string filePath, string dirName);
    void fortranParser(string filePath, string dirName);
    void verilogParser(string filePath, string dirName);
    void idlParser(string filePath, string dirName);
    void ccppParser(string filePath, string dirName);
    void javaParser(string filePath, string dirName);
    int numberOfSpacesAtBeginning(string& str);
    pair<pair<int, int>, string> findFunction(string input, int position, ClassType classTypeOfFile);
    pair<string, int> findClass(string input, int position, ClassType classTypeOfFile);
};



#endif // FUNCTIONPARSER_H

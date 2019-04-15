//! FunctionParser class
/*!
* \file FunctionParser.cpp
*
* This file contains the function parsing class methods.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <regex>
#include "FunctionParser.h"
#include "cc_main.h"
#include "CUtil.h"

using namespace std;

FunctionParser::FunctionParser() {}
FunctionParser::~FunctionParser() {}

/*
* 1. Function Description:
*    Call function parser as per the ClassType of file
*
* 2. Parameters:
*    filePath:          path of python source code
*	 dirName:           directory where the output files are to be stored
*    classTypeOfFile:   class type of the file
*
* 3. Creation Time and Owner:
*	 Version 2016.10
*/
void FunctionParser::callParser(string filePath, string dirName, ClassType classTypeOfFile)
{
    switch(classTypeOfFile)
    {
        case PYTHON: 	pythonParser(filePath, dirName);
                     	break;
        case FORTRAN: 	fortranParser(filePath, dirName);
        				break;
        case VERILOG:	verilogParser(filePath, dirName);
        				break;
        case IDL:		idlParser(filePath, dirName);
        				break;
        case C_CPP:   ccppParser(filePath, dirName); //Modified: 2017.03
                break;
        case JAVA:    javaParser(filePath, dirName); //Modified: 2017.04
                break;
        default:     cout<<"\n\nUNKNOWN CLASS TYPE. NO PARSER PRESENT FOR THIS CLASS TYPE!\n\n";
                     break;
    }
}

/*
* 1. Function Description: 
*    Calculate number of white spaces in the start of a string
*
* 2. Parameters: 
*    str： string
*
* 3. Creation Time and Owner: 
*	 Version 2016.10
*/
int FunctionParser::numberOfSpacesAtBeginning(string& str)
{
	size_t idx = str.find_first_not_of(" \t\n\r\f");
    if (idx != string::npos)
    	return idx;
   	else
   		return 0;
}

/*
* 1. Function Description: 
*    Parse the methods in a given python source code file and store them in different files
*
* 2. Parameters: 
*    filePath:  path of python source code
*	 dirName:	directory where the output files are to be stored
*
* 3. Creation Time and Owner: 
*	 Version 2016.10
*/
void FunctionParser::pythonParser(string filePath, string dirName)
{
	  set<string> functions; //set storing unique method names
	  map<int, string> fileMap; //map storing all lines of code against lineNumber
    vector<pair<string, int> > stackScopeIndent; //store the currently active scope and its indent

	  vector<int> lineNumberOfComments;

	  string line;
  	ifstream myfile (filePath.c_str()); //file to be read
  	int lineNumber = 1;
  	int indentLevel=0;

  	//code to read input python code and store the lines in fileMap
  	if (myfile.is_open())
  	{
  	  while ( getline (myfile,line))
  	  {
  	  	fileMap[lineNumber]=line;
  	  	lineNumber++;
  	  }
  	  myfile.close();
  	}
  	else 
  		cout << "Unable to open file" <<endl;

  	//iterating through the fileMap to identify methods in the python code
  	if (fileMap.size()>0)
  	{
  		for (map<int, string>::iterator it = fileMap.begin(); it != fileMap.end(); ++it) 
  		{
  		    string lineOfCode = it->second;
  		    string temp = CUtil::TrimString(lineOfCode);
  	    	if (temp.length()>0 && (temp.substr(0,4)).compare("def ")==0)
  	    	{
            //Python doesn't support overload, we only need to consider the function name but not parameters
            //Modification: 2017.02
  	    	  size_t first = temp.find_first_of('(');
  	    		if (first == string::npos)
  	    		{
  	    			continue;
  	    		}
  	    		else
  	    		{
  	    			//found a method definition
  	    			string function_name = temp.substr(4,(first - 4));
  	    			functions.insert(function_name);

              indentLevel = numberOfSpacesAtBeginning(lineOfCode);

              //pop inactive scopes from stack and generate temporary file name with scope
              //Modification: 2017.02
              while (stackScopeIndent.size() > 0 && stackScopeIndent.back().second >= indentLevel) 
              {
                stackScopeIndent.pop_back();
              }
              string scope = "";
              for (vector<pair<string, int> >::iterator it_stack = stackScopeIndent.begin(); it_stack != stackScopeIndent.end(); ++it_stack) 
              {
                scope += it_stack->first + ":";
              }
  	    			ofstream newMethodFile;
  	    			string nameOfMethodFile = dirName + "/" + scope + function_name + ".py";

              //keep track of currently active function and indent
              //Modification: 2017.02
              stackScopeIndent.push_back(make_pair(function_name, indentLevel));

						  newMethodFile.open (nameOfMethodFile.c_str());
						  newMethodFile << lineOfCode << endl;

  	    			//finding the method body and storing the last line number in methodBodyMap
  	    			map<int, string>::iterator it2 = it;
  	    			it2++;
  	    			int methodIndentLevel = numberOfSpacesAtBeginning(it2->second);
  	    			while(it2 != fileMap.end() && (CUtil::TrimString(it2->second).length() == 0 || methodIndentLevel > indentLevel || (((CUtil::TrimString(it2->second)).substr(0,1)).compare("#") == 0)))
  	    			{
  	    				if (CUtil::TrimString(it2->second).length() > 0)
  	    				{
  	    					newMethodFile << it2->second << endl;
  	    				}
  	    				it2++;
						    //if it2 is the end of file, do not call it2->second to avoid crash, and will break in the next iteration
						    if (it2 != fileMap.end())
  	    					methodIndentLevel = numberOfSpacesAtBeginning(it2->second);
  	    			}
  	    			newMethodFile.close();
  	    		}
  	    	}
          ////keep track of currently active class and indent
          //Modification: 2017.02
          else if (temp.length()>0 && (temp.substr(0,6)).compare("class ")==0) 
          {
            size_t first = temp.find_first_of('(');
            if (first == string::npos)
            {
              continue;
            }
            else
            {
              //found a class definition
              string class_name = temp.substr(6,(first - 6));

              indentLevel = numberOfSpacesAtBeginning(lineOfCode);
              //pop inactive scopes from stack
              //Modification: 2017.02
              while (stackScopeIndent.size() > 0 && stackScopeIndent.back().second >= indentLevel) 
              {
                stackScopeIndent.pop_back();
              }

              stackScopeIndent.push_back(make_pair(class_name, indentLevel));
            }
          }
  		}
  	}
}

/*
* 1. Function Description: 
*    Parse the methods in a given fortran source code file and store them in different files
*
* 2. Parameters: 
*    filePath:  path of fortran source code
*	 dirName:	directory where the output files are to be stored
*
* 3. Creation Time and Owner: 
*	 Version 2016.11
*/
void FunctionParser::fortranParser(string filePath, string dirName)
{
	set<string> functions; //set storing unique method names
	map<int, string> fileMap; //map storing all lines of code against lineNumber

	string line;
  	ifstream myfile (filePath.c_str()); //file to be read
  	int lineNumber = 1;

  	//code to read input fortran code and store the lines in fileMap
  	if (myfile.is_open())
  	{
  	  while ( getline (myfile,line))
  	  {
  	  	fileMap[lineNumber]=line;
  	  	lineNumber++;
  	  }
  	  myfile.close();
  	}
  	else 
  		cout << "Unable to open file" <<endl;

  	//iterating through the fileMap to identify methods in the fortran code
  	if (fileMap.size()>0)
  	{
  		for (map<int, string>::iterator it = fileMap.begin(); it != fileMap.end(); ++it) 
  		{
  		    string lineOfCode = it->second;
  		    string temp = CUtil::TrimString(lineOfCode);
  	    	if (temp.length()>0 && (CUtil::ToLower(temp.substr(0,9))).compare("function ")==0)
  	    	{
  	    	    size_t first = temp.find_last_of(')');
  	    		if (first == string::npos)
  	    		{
  	    			continue;
  	    		}
  	    		else
  	    		{
  	    			//found a method definition
  	    			string function_name = temp.substr(9,(first - 8));
  	    			functions.insert(function_name);

  	    			ofstream newMethodFile;
  	    			string nameOfMethodFile = dirName + "/" + function_name + ".f";
						newMethodFile.open (nameOfMethodFile.c_str());
						newMethodFile << lineOfCode << endl;


  	    			map<int, string>::iterator it2 = it;
  	    			it2++;
  	    			int isEndFunctionPresent = (CUtil::ToLower(CUtil::TrimString(it2->second).substr(0,12))).compare("end function");
  	    			int isEndPresent = (CUtil::ToLower(CUtil::TrimString(it2->second).substr(0,3))).compare("end");
  	    			while((CUtil::TrimString(it2->second).length() == 0 || isEndFunctionPresent != 0 || isEndPresent != 0) && it2 != fileMap.end())
  	    			{
  	    				if (CUtil::TrimString(it2->second).length() > 0)
  	    				{
  	    					newMethodFile << it2->second << endl;
  	    				}
  	    				it2++;
  	    				isEndFunctionPresent = (CUtil::ToLower(CUtil::TrimString(it2->second).substr(0,12))).compare("end function");	
  	    				isEndPresent = (CUtil::ToLower(CUtil::TrimString(it2->second).substr(0,3))).compare("end");	
  	    			}
  	    			if (isEndFunctionPresent == 0)
  	    				newMethodFile << it2->second << endl;
  	    			else if (isEndPresent == 0)
  	    				newMethodFile << it2->second << endl;
  	    			newMethodFile.close();
  	    		}
  	    	}
  		}
  	}
}

/*
* 1. Function Description: 
*    Parse the methods in a given verilog source code file and store them in different files
*
* 2. Parameters: 
*    filePath:  path of verilog source code
*	 dirName:	directory where the output files are to be stored
*
* 3. Creation Time and Owner: 
*	 Version 2016.11
*/
void FunctionParser::verilogParser(string filePath, string dirName)
{
	set<string> functions; //set storing unique method names
	map<int, string> fileMap; //map storing all lines of code against lineNumber

	string line;
  	ifstream myfile (filePath.c_str()); //file to be read
  	int lineNumber = 1;

  	//code to read input verilog code and store the lines in fileMap
  	if (myfile.is_open())
  	{
  	  while ( getline (myfile,line))
  	  {
  	  	fileMap[lineNumber]=line;
  	  	lineNumber++;
  	  }
  	  myfile.close();
  	}
  	else 
  		cout << "Unable to open file" <<endl;

  	//iterating through the fileMap to identify methods in the verilog code
  	if (fileMap.size()>0)
  	{
  		for (map<int, string>::iterator it = fileMap.begin(); it != fileMap.end(); ++it) 
  		{
  		    string lineOfCode = it->second;
  		    string temp = CUtil::TrimString(lineOfCode);
  	    	if (temp.length()>0 && (CUtil::ToLower(temp.substr(0,9))).compare("function ")==0)
  	    	{
  	    	    size_t first = temp.find_last_of(';');
  	    		if (first == string::npos)
  	    		{
  	    			continue;
  	    		}
  	    		else
  	    		{
  	    			//found a method definition
  	    			string function_name = temp.substr(9,(first - 9));
  	    			functions.insert(function_name);

  	    			ofstream newMethodFile;
  	    			string nameOfMethodFile = dirName + "/" + function_name + ".v";
						newMethodFile.open (nameOfMethodFile.c_str());
						newMethodFile << lineOfCode << endl;


  	    			map<int, string>::iterator it2 = it;
  	    			it2++;
  	    			int isEndFunctionPresent = (CUtil::ToLower(CUtil::TrimString(it2->second).substr(0,11))).compare("endfunction");
  	    			while((CUtil::TrimString(it2->second).length() == 0 || isEndFunctionPresent != 0) && it2 != fileMap.end())
  	    			{
  	    				if (CUtil::TrimString(it2->second).length() > 0)
  	    				{
  	    					newMethodFile << it2->second << endl;
  	    				}
  	    				it2++;
  	    				isEndFunctionPresent = (CUtil::ToLower(CUtil::TrimString(it2->second).substr(0,11))).compare("endfunction");	
  	    			}
  	    			if (isEndFunctionPresent == 0)
  	    				newMethodFile << it2->second << endl;
  	    			newMethodFile.close();
  	    		}
  	    	}
  		}
  	}
}

/*
* 1. Function Description: 
*    Parse the methods in a given IDL source code file and store them in different files
*
* 2. Parameters: 
*    filePath:  path of IDL source code
*	 dirName:	directory where the output files are to be stored
*
* 3. Creation Time and Owner: 
*	 Version 2016.11
*/
void FunctionParser::idlParser(string filePath, string dirName)
{
	set<string> functions; //set storing unique method names
	map<int, string> fileMap; //map storing all lines of code against lineNumber

	string line;
  	ifstream myfile (filePath.c_str()); //file to be read
  	int lineNumber = 1;

  	//code to read input IDL code and store the lines in fileMap
  	if (myfile.is_open())
  	{
  	  while ( getline (myfile,line))
  	  {
  	  	fileMap[lineNumber]=line;
  	  	lineNumber++;
  	  }
  	  myfile.close();
  	}
  	else 
  		cout << "Unable to open file" <<endl;

  	//iterating through the fileMap to identify methods in the IDL code
  	if (fileMap.size()>0)
  	{
  		for (map<int, string>::iterator it = fileMap.begin(); it != fileMap.end(); ++it) 
  		{
  		    string lineOfCode = it->second;
  		    string temp = CUtil::TrimString(lineOfCode);
  	    	if (temp.length()>0 && (CUtil::ToLower(temp.substr(0,9))).compare("function ")==0)
  	    	{
  	    	    size_t first = temp.find_first_of(',');
  	    		if (first == string::npos)
  	    		{
  	    			first = temp.find_first_of(';');
  	    			if (first == string::npos)
  	    			{
  	    				temp = temp + " ";
  	    				first = temp.find_last_of(' ');
  	    			}	
  	    		}
  	    		if (first != string::npos)
  	    		{
  	    			//found a method definition
  	    			string function_name = temp.substr(9,(first - 9));
  	    			functions.insert(function_name);

  	    			ofstream newMethodFile;
  	    			string nameOfMethodFile = dirName + "/" + function_name + ".pro";
						newMethodFile.open (nameOfMethodFile.c_str());
						newMethodFile << lineOfCode << endl;

  	    			map<int, string>::iterator it2 = it;
  	    			it2++;
  	    			int isEndFunctionPresent = (CUtil::ToLower(CUtil::TrimString(it2->second).substr(0,3))).compare("end");
  	    			while((CUtil::TrimString(it2->second).length() == 0 || isEndFunctionPresent != 0) && it2 != fileMap.end())
  	    			{
  	    				if (CUtil::TrimString(it2->second).length() > 0)
  	    				{
  	    					newMethodFile << it2->second << endl;
  	    				}
  	    				it2++;
  	    				isEndFunctionPresent = (CUtil::ToLower(CUtil::TrimString(it2->second).substr(0,3))).compare("end");	
  	    			}
  	    			if (isEndFunctionPresent == 0)
  	    				newMethodFile << it2->second << endl;
  	    			newMethodFile.close();
  	    		}
  	    	}
  	    	if (temp.length()>0 && (CUtil::ToLower(temp.substr(0,4))).compare("pro ")==0)
  	    	{
  	    	    size_t first = temp.find_first_of(',');
  	    		if (first == string::npos)
  	    		{
  	    			first = temp.find_first_of(';');
  	    			if (first == string::npos)
  	    			{
  	    				temp = temp + " ";
  	    				first = temp.find_last_of(' ');
  	    			}	
  	    		}
  	    		if (first != string::npos)
  	    		{
  	    			//found a method definition
  	    			string function_name = temp.substr(4,(first - 4));
  	    			functions.insert(function_name);

  	    			ofstream newMethodFile;
  	    			string nameOfMethodFile = dirName + "/" + function_name + ".pro";
						newMethodFile.open (nameOfMethodFile.c_str());
						newMethodFile << lineOfCode << endl;

  	    			map<int, string>::iterator it2 = it;
  	    			it2++;
  	    			int isEndFunctionPresent = (CUtil::ToLower(CUtil::TrimString(it2->second).substr(0,3))).compare("end");
  	    			while((CUtil::TrimString(it2->second).length() == 0 || isEndFunctionPresent != 0) && it2 != fileMap.end())
  	    			{
  	    				if (CUtil::TrimString(it2->second).length() > 0)
  	    				{
  	    					newMethodFile << it2->second << endl;
  	    				}
  	    				it2++;
  	    				isEndFunctionPresent = (CUtil::ToLower(CUtil::TrimString(it2->second).substr(0,3))).compare("end");	
  	    			}
  	    			if (isEndFunctionPresent == 0)
  	    				newMethodFile << it2->second << endl;
  	    			newMethodFile.close();
  	    		}
  	    	}
  		}
  	}
}

/*
* 1. Function Description: 
*    Parse the methods in a given c/cpp source code file and store them in different files
*
* 2. Parameters: 
*    filePath:  path of python source code
*    dirName: directory where the output files are to be stored
*
* 3. Creation Time and Owner: 
*  Version 2017.03
*/
void FunctionParser::ccppParser(string filePath, string dirName)
{
  set<string> functions; //set storing unique method names
  map<int, string> fileMap; //map storing all lines of code against lineNumber

  string line;
    ifstream myfile (filePath.c_str()); //file to be read
    int lineNumber = 1;

    //code to read input C_CPP code and store the lines in fileMap
    if (myfile.is_open())
    {
      while ( getline (myfile,line))
      {
        fileMap[lineNumber]=line;
        lineNumber++;
      }
      myfile.close();
    }
    else 
      cout << "Unable to open file" <<endl;
    //iterating through the fileMap to identify methods in the C_CPP code
    if (fileMap.size()>0)
    {
      map<int, string>::iterator it = fileMap.begin();
      int pointer = 0;
      vector<pair<string, int> > classStack; // a stack to store the class and brace counter information
      bool lastActivated = false;//to mark whether the last function in function stack has been activated (has met the first left brace)
      bool multiLineCommentFlag = false; //to mark whether the current code is in a /*...*/ comment


      while (it != fileMap.end()) 
      {
          string lineOfCode = it->second;
          string temp = CUtil::TrimString(lineOfCode);
          if (temp.length() == 0) {
              it ++;
              pointer = 0;
              continue;
          }

          //if it's in comments now, try to find the ending of the comment, otherwise move to the next line.
          if (multiLineCommentFlag) {
              //Modification: 2018.01, changed data type to size_t
              size_t commentEnding = lineOfCode.find("*/", pointer);
              if (commentEnding == string::npos) {
                  it ++;
                  pointer = 0;
                  continue;
              }
              else {
                  pointer = commentEnding + 2;
                  multiLineCommentFlag = false;
              }
          }

          //cout << "code: " + lineOfCode << endl;

          pair<pair<int, int>, string> funcPair = findFunction(lineOfCode, pointer, C_CPP); //try to find the definition of function, return the positions of start of entry, right parenthesis, and the entire name including paramters
          pair<string, int> classPair = findClass(lineOfCode, pointer, C_CPP); //try to find the definition of class, return the name of class and the ending character of class name

          //cout << "function: " + funcPair.second << endl;
          //cout << "class: " + classPair.first << endl;

          //process class braces until the next stop point(start of class or function) or the end of line
          int stopPos;
          if (funcPair.first.first < 0 && classPair.second < 0) {
              stopPos = lineOfCode.length();
          }
          else if (funcPair.first.first >= 0) {
              stopPos = funcPair.first.first + 1;
          }
          else {
              stopPos = classPair.second + 1;
          }

          bool lineCommentFlag = false;
          for (int i = pointer; i < stopPos; i ++) {
              if (lineOfCode[i] == '{') {
                  //Modification: 2018.01, changed data type to size_t
                  for (size_t idx = 0; idx < classStack.size(); idx ++) {
                      classStack[idx].second ++;
                  }
                  //if the lastest class hasn't met a left brace and now we met a left brace, we set it activated
                  if (classStack.size() > 0 && !lastActivated) {
                    lastActivated = true;
                  }
              }
              //if a single line comment is found
              else if (lineOfCode[i] == '/' && i+1 < stopPos && lineOfCode[i+1] == '/') {
                  lineCommentFlag = true;
                  break;
              }
              //if a multi line comment is found
              else if (lineOfCode[i] == '/' && i+1 < stopPos && lineOfCode[i+1] == '*') {
                  multiLineCommentFlag = true;
                  pointer = i+2;
                  break;
              }
              else if (lineOfCode[i] != ' ' && lineOfCode[i] != '\t' && line[i] != '\r') {
                  //if the latest class hasn't met a left brace but met a non-space character, we should remove it from the class stack
                  if (classStack.size() > 0 && !lastActivated) {
                      classStack.pop_back();
                      lastActivated = true;
                  }
                  if (lineOfCode[i] == '}') {
                      //Modification: 2018.01, changed data type to size_t
                      for (size_t idx = 0; idx < classStack.size(); idx ++) {
                          classStack[idx].second --;
                      }
                      //if a class ends
                      if (classStack.size() > 0 && classStack[classStack.size() - 1].second == 0) {
                          classStack.pop_back();
                      }
                  }
              }
          }

          //The following part of this line is commented
          if (lineCommentFlag) {
              it ++;
              pointer = 0;
              continue;
          }
          //The start of a multi line comment was found, we get into the next iteration
          if (multiLineCommentFlag) {
              continue;
          }

          //if there is only a class definition or the class definition is ahead of the function definition
          if (classPair.second >= 0 && (funcPair.first.first < 0 || classPair.second < funcPair.first.first)) {
              classStack.push_back(make_pair(classPair.first, 0));
              pointer = classPair.second + 1;
              lastActivated = false;
          }
          else if (funcPair.first.first >= 0) 
          {
              //found a function definition
              pointer = funcPair.first.first;
              //Modification: 2018.01, changed data type to size_t
              size_t pointer2 = funcPair.first.second + 1; //the character pointer for function body

              string function_name;
              //concatenate the class names in the current active class stack with colons to be the identifier of the function name
              //Modification: 2018.01, changed data type to size_t
              for (size_t i = 0; i < classStack.size(); i ++) {
                  function_name += classStack[i].first + ':';
              }
              function_name += funcPair.second;
              functions.insert(function_name);

              ofstream newMethodFile;
              string nameOfMethodFile = dirName + "/" + function_name + ".cpp";
              string functionHead = lineOfCode.substr(pointer, pointer2 - pointer); //save the function head part in order to write it in file after confirming this is actually a fucntion
              

              map<int, string>::iterator it2 = it; //the line iterator for function body
              int count = 0;//count the current braces stack
              bool startFlag = false; //a flag to show whether we have check the entry of function
              bool endFlag = false; //a flag to show whether the function has ended
              bool wrongFlag = false;//a flag to show whether this is a wrong function judgement
              bool multiLineCommentInFuncFlag = false;//a flag to show whether it's in multi line comments now in a function
              while(it2 != fileMap.end() && !endFlag)
              {
                line = it2->second;

                size_t i;
                //if it's in a multi line comment now, we should try to find the ending of comments at first, then we start running from it
                if (multiLineCommentInFuncFlag) {
                    //Modification: 2018.01, changed data type to size_t
                    size_t commentEndingInFunc = line.find("*/", pointer2);
                    if (commentEndingInFunc == string::npos) {
                        i = line.length();
                    }
                    else {
                        i = commentEndingInFunc + 2;
                        multiLineCommentInFuncFlag = false;
                    }
                }
                else {
                  for (i = pointer2; i < line.length(); i ++) {
                    //if there is // in this line, we end the traversing at once
                    if (line[i] == '/' && i+1 < line.length() && line[i+1] == '/') {
                      i = line.length();
                      break;
                    }
                    //if there is /* in this line
                    else if (line[i] == '/' && i+1 < line.length() && line[i+1] == '*') {
                      multiLineCommentInFuncFlag = true;
                      i = i+2;
                      break;
                    }
                    else if (!startFlag && line[i] != ' ' && line[i] != '\t' && line[i] != '\r' && line[i] != '{') { //if we meet a non-space character before me meet the first {, then this is not a real function
                      wrongFlag = true;
                      break;
                    }
                    else if (line[i] == '{') {
                      if (count == 0) {
                        startFlag = true;
                      }
                      count ++;
                    }
                    else if (line[i] == '}') {
                      count --;
                      if (count == 0) {
                        i ++;
                        endFlag = true;
                        break;
                      }
                    }
                  }
                }
                if (wrongFlag) { //if it is a wrong function, then we break and continue searching
                  pointer2 = i;
                  break;
                }

                if (!startFlag) { //if we haven't found the function body, all characters should be saved in functionHead
                  functionHead += line.substr(pointer2, i - pointer2);
                  if (i == line.length()) {
                    functionHead += '\n';
                  }
                }
                else {
                  if (!newMethodFile.is_open()) { //open the file and write the function head if we confirm the function body
                    newMethodFile.open (nameOfMethodFile.c_str());
                    newMethodFile << functionHead;
                  }

                  if (i - pointer2 > 0) { //save the valid characters in this line
                    newMethodFile << line.substr(pointer2, i - pointer2);
                    if (i == line.length()) {
                      newMethodFile << '\n';
                    }
                  }
                }
                pointer2 = i;
                if (pointer2 == line.length()) {
                  it2 ++;
                  pointer2 = 0;
                }
              }
              it = it2; //we can start the next search from the ending position of last time
              pointer = pointer2;
              newMethodFile.close();
          }
          else {
            it ++;
            pointer = 0;
          }
      }
    }
}

/*
* 1. Function Description: 
*    Parse the methods in a given java source code file and store them in different files
*
* 2. Parameters: 
*    filePath:  path of python source code
*    dirName: directory where the output files are to be stored
*
* 3. Creation Time and Owner: 
*  Version 2017.04
*/
void FunctionParser::javaParser(string filePath, string dirName)
{
    set<string> functions; //set storing unique method names
  map<int, string> fileMap; //map storing all lines of code against lineNumber

  string line;
    ifstream myfile (filePath.c_str()); //file to be read
    int lineNumber = 1;

    //code to read input C_CPP code and store the lines in fileMap
    if (myfile.is_open())
    {
      while ( getline (myfile,line))
      {
        fileMap[lineNumber]=line;
        lineNumber++;
      }
      myfile.close();
    }
    else 
      cout << "Unable to open file" <<endl;
    //iterating through the fileMap to identify methods in the C_CPP code
    if (fileMap.size()>0)
    {
      map<int, string>::iterator it = fileMap.begin();
      int pointer = 0;
      vector<pair<string, int> > classStack; // a stack to store the class and brace counter information
      bool lastActivated = false;//to mark whether the last function in function stack has been activated (has met the first left brace)
      bool multiLineCommentFlag = false; //to mark whether the current code is in a /*...*/ comment


      while (it != fileMap.end()) 
      {
          string lineOfCode = it->second;
          string temp = CUtil::TrimString(lineOfCode);
          if (temp.length() == 0) {
              it ++;
              pointer = 0;
              continue;
          }

          //if it's in comments now, try to find the ending of the comment, otherwise move to the next line.
          if (multiLineCommentFlag) {
              //Modification: 2018.01, changed data type to size_t
              size_t commentEnding = lineOfCode.find("*/", pointer);
              if (commentEnding == string::npos) {
                  it ++;
                  pointer = 0;
                  continue;
              }
              else {
                  pointer = commentEnding + 2;
                  multiLineCommentFlag = false;
              }
          }

          //cout << "code: " + lineOfCode << endl;

          pair<pair<int, int>, string> funcPair = findFunction(lineOfCode, pointer, JAVA); //try to find the definition of function, return the positions of start of entry, right parenthesis, and the entire name including paramters
          pair<string, int> classPair = findClass(lineOfCode, pointer, JAVA); //try to find the definition of class, return the name of class and the ending character of class name

          //cout << "function: " + funcPair.second << endl;
          //cout << "class: " + classPair.first << endl;

          //process class braces until the next stop point(start of class or function) or the end of line
          int stopPos;
          if (funcPair.first.first < 0 && classPair.second < 0) {
              stopPos = lineOfCode.length();
          }
          else if (funcPair.first.first >= 0) {
              stopPos = funcPair.first.first + 1;
          }
          else {
              stopPos = classPair.second + 1;
          }

          bool lineCommentFlag = false;
          for (int i = pointer; i < stopPos; i ++) {
              if (lineOfCode[i] == '{') {
                  //Modification: 2018.01, changed data type to size_t
                  for (size_t idx = 0; idx < classStack.size(); idx ++) {
                      classStack[idx].second ++;
                  }
                  //if the lastest class hasn't met a left brace and now we met a left brace, we set it activated
                  if (classStack.size() > 0 && !lastActivated) {
                    lastActivated = true;
                  }
              }
              //if a single line comment is found
              else if (lineOfCode[i] == '/' && i+1 < stopPos && lineOfCode[i+1] == '/') {
                  lineCommentFlag = true;
                  break;
              }
              //if a multi line comment is found
              else if (lineOfCode[i] == '/' && i+1 < stopPos && lineOfCode[i+1] == '*') {
                  multiLineCommentFlag = true;
                  pointer = i+2;
                  break;
              }
              else if (lineOfCode[i] != ' ' && lineOfCode[i] != '\t' && line[i] != '\r') {
                  //if the latest class hasn't met a left brace but met a non-space character, we should remove it from the class stack
                  if (classStack.size() > 0 && !lastActivated) {
                      classStack.pop_back();
                      lastActivated = true;
                  }
                  if (lineOfCode[i] == '}') {
                      //Modification: 2018.01, changed data type to size_t
                      for (size_t idx = 0; idx < classStack.size(); idx ++) {
                          classStack[idx].second --;
                      }
                      //if a class ends
                      if (classStack.size() > 0 && classStack[classStack.size() - 1].second == 0) {
                          classStack.pop_back();
                      }
                  }
              }
          }

          //The following part of this line is commented
          if (lineCommentFlag) {
              it ++;
              pointer = 0;
              continue;
          }
          //The start of a multi line comment was found, we get into the next iteration
          if (multiLineCommentFlag) {
              continue;
          }

          //if there is only a class definition or the class definition is ahead of the function definition
          if (classPair.second >= 0 && (funcPair.first.first < 0 || classPair.second < funcPair.first.first)) {
              classStack.push_back(make_pair(classPair.first, 0));
              pointer = classPair.second + 1;
              lastActivated = false;
          }
          else if (funcPair.first.first >= 0) 
          {
              //found a function definition
              pointer = funcPair.first.first;
              //Modification: 2018.01, changed data type to size_t
              size_t pointer2 = funcPair.first.second + 1; //the character pointer for function body

              string function_name;
              //concatenate the class names in the current active class stack with colons to be the identifier of the function name
              //Modification: 2018.01, changed data type to size_t
              for (size_t i = 0; i < classStack.size(); i ++) {
                  function_name += classStack[i].first + ':';
              }
              function_name += funcPair.second;
              functions.insert(function_name);

              ofstream newMethodFile;
              string nameOfMethodFile = dirName + "/" + function_name + ".java";
              string functionHead = lineOfCode.substr(pointer, pointer2 - pointer); //save the function head part in order to write it in file after confirming this is actually a fucntion
              

              map<int, string>::iterator it2 = it; //the line iterator for function body
              int count = 0;//count the current braces stack
              bool startFlag = false; //a flag to show whether we have check the entry of function
              bool endFlag = false; //a flag to show whether the function has ended
              bool wrongFlag = false;//a flag to show whether this is a wrong function judgement
              bool multiLineCommentInFuncFlag = false;//a flag to show whether it's in multi line comments now in a function
              while(it2 != fileMap.end() && !endFlag)
              {
                line = it2->second;

                size_t i;//Modification: 2018.01, changed data type to size_t
                //if it's in a multi line comment now, we should try to find the ending of comments at first, then we start running from it
                if (multiLineCommentInFuncFlag) {
                    //Modification: 2018.01, changed data type to size_t
                    size_t commentEndingInFunc = line.find("*/", pointer2);
                    if (commentEndingInFunc == string::npos) {
                        i = line.length();
                    }
                    else {
                        i = commentEndingInFunc + 2;
                        multiLineCommentInFuncFlag = false;
                    }
                }
                else {
                  for (i = pointer2; i < line.length(); i ++) {
                    //if there is // in this line, we end the traversing at once
                    if (line[i] == '/' && i+1 < line.length() && line[i+1] == '/') {
                      i = line.length();
                      break;
                    }
                    //if there is /* in this line
                    else if (line[i] == '/' && i+1 < line.length() && line[i+1] == '*') {
                      multiLineCommentInFuncFlag = true;
                      i = i+2;
                      break;
                    }
                    else if (!startFlag && line[i] != ' ' && line[i] != '\t' && line[i] != '\r' && line[i] != '{') { //if we meet a non-space character before me meet the first {, then this is not a real function
                      wrongFlag = true;
                      break;
                    }
                    else if (line[i] == '{') {
                      if (count == 0) {
                        startFlag = true;
                      }
                      count ++;
                    }
                    else if (line[i] == '}') {
                      count --;
                      if (count == 0) {
                        i ++;
                        endFlag = true;
                        break;
                      }
                    }
                  }
                }
                if (wrongFlag) { //if it is a wrong function, then we break and continue searching
                  pointer2 = i;
                  break;
                }

                if (!startFlag) { //if we haven't found the function body, all characters should be saved in functionHead
                  functionHead += line.substr(pointer2, i - pointer2);
                  if (i == line.length()) {
                    functionHead += '\n';
                  }
                }
                else {
                  if (!newMethodFile.is_open()) { //open the file and write the function head if we confirm the function body
                    newMethodFile.open (nameOfMethodFile.c_str());
                    newMethodFile << functionHead;
                  }

                  if (i - pointer2 > 0) { //save the valid characters in this line
                    newMethodFile << line.substr(pointer2, i - pointer2);
                    if (i == line.length()) {
                      newMethodFile << '\n';
                    }
                  }
                }
                pointer2 = i;
                if (pointer2 == line.length()) {
                  it2 ++;
                  pointer2 = 0;
                }
              }
              it = it2; //we can start the next search from the ending position of last time
              pointer = pointer2;
              newMethodFile.close();
          }
          else {
            it ++;
            pointer = 0;
          }
      }
    }
}

/*
* 1. Function Description: 
*    find the possible entry of a function in the lineOfCode starting from the index 'pointer'
*
* 2. Parameters: 
*    input: the line of code
*    position: the starting index from which we start searching
*    classTypeOfFile: class type of the file. Now only support C_CPP and JAVA.
*
* 3. Return:
*    return a pair<pair<int, int>, string>, in which contains: 
*    a)the function name as a string
*    b)the function start and end position as pair<int, int>.
*
* 4. Creation Time and Owner: 
*  Version 2017.03
*/
pair<pair<int, int>, string> FunctionParser::findFunction(string input, int position, ClassType classTypeOfFile) {
    string functionName = "";
    string s = input.substr(position);
    std::size_t commentPosition = s.find("//");
    s = s.substr(0, commentPosition);

    if(s.find("(") == string::npos || s.find(")") == string::npos) {
        return make_pair(make_pair(-1, -1), "");
    }

    //Modified: 2017.04
    string functionTypeDecorate, varType, pointerType, funcId, typeDecorate, varId, pointerAndAddress, varTypeShort, funcIdAndParameter, pattern, constructorAppendent;
    switch(classTypeOfFile) {
        case C_CPP:{ 
            functionTypeDecorate = "((public|private|protected|signed|mutable|static|unsigned|const|volatile|long|struct)\\s+)*";
            varType = "([_a-zA-Z][_a-zA-Z0-9]*::)*[_a-zA-Z][_a-zA-Z0-9]*(\\s*<\\s*([_a-zA-Z][_a-zA-Z0-9]*::)*[_a-zA-Z][_a-zA-Z0-9\\[\\]]*(\\s*<\\s*[_a-zA-Z0-9<>:,\\s\\*\\[\\]]+\\s*>)?\\s*(,\\s*([_a-zA-Z][_a-zA-Z0-9]*::)*[_a-zA-Z][_a-zA-Z0-9\\[\\]]*(\\s*<\\s*[_a-zA-Z0-9<>,:\\s\\*\\[\\]]+\\s*>)?)*\\s*>)?";
            pointerType = "[\\s\\*]+";
            funcId = "([_a-zA-Z][_a-zA-Z0-9]*::)*[_a-zA-Z][_a-zA-Z0-9]*";
            typeDecorate = "((signed|mutable|static|unsigned|const|volatile|long|struct)\\s+)*";
            varId = "[_a-zA-Z][_a-zA-Z0-9]*";
            pointerAndAddress = "[\\s\\*&]+";
            funcIdAndParameter = "[_0-9a-zA-Z:]+\\s*\\([_0-9a-zA-Z:,\\s\\*<>]*\\)";
            varTypeShort = "[_0-9a-zA-Z]+(::[_0-9a-zA-Z]+)*(\\s*<([_0-9a-zA-Z:,\\s\\*<>]+)>)?";
            constructorAppendent = "(\\s*:\\s*" + funcIdAndParameter + ")?";
            //string pattern = functionTypeDecorate + varType + pointerType + funcId + "\\s*\\(\\s*(" + typeDecorate + varType + pointerAndAddress + varId + "\\s*(,\\s*" + typeDecorate + varType + pointerAndAddress + varId + "\\s*)*|\\s*)\\s*\\)";
            pattern = functionTypeDecorate + varTypeShort + pointerType + funcIdAndParameter;
            break;
        }
        case JAVA:{
            functionTypeDecorate = "((abstract|synchronized|final|interface|public|private|protected|signed|mutable|static|unsigned|const|volatile|long)\\s+)*";
            varType = "([_a-zA-Z][_a-zA-Z0-9]*\\.)*[_a-zA-Z][_a-zA-Z0-9]*(\\s*\\[\\s*\\])?(\\s*<\\s*[_a-zA-Z][_a-zA-Z0-9\\[\\]\\.]*(\\s*<\\s*[_a-zA-Z0-9<>,\\s\\[\\]\\.]+\\s*>)?\\s*(,\\s*[_a-zA-Z][_a-zA-Z0-9\\[\\]\\.]*(\\s*<\\s*[_a-zA-Z0-9<>,\\s\\[\\]\\.]+\\s*>)?)*\\s*>)?";
            funcId = "[_a-zA-Z][_a-zA-Z0-9]*";
            varId = "[_a-zA-Z][_a-zA-Z0-9]*";
            funcIdAndParameter = "[_0-9a-zA-Z\\[\\]]+\\s*\\([_0-9a-zA-Z,\\s<>\\[\\]\\.]*\\)";
            varTypeShort = "[_0-9a-zA-Z\\.]+(\\s*\\[\\s*\\])?(\\s*<([_0-9a-zA-Z\\.,\\s\\*<>\\[\\]]+)>)?";
            constructorAppendent = "";
            pattern = functionTypeDecorate + varTypeShort + "\\s+" + funcIdAndParameter;
            break;
        }
        default:     
            cout<<"\n\nUNKNOWN CLASS TYPE. NO FINDFUNCTION PRESENT FOR THIS CLASS TYPE!\n\n";
            break;
    }
    smatch m;
    regex e1 (funcIdAndParameter + constructorAppendent);
    if (regex_search(s, m, e1) == false) {
        return make_pair(make_pair(-1, -1), "");
    }

    string tempFunc = m[0].str();
    int tempStartPosition = m.position(0);
    int startPosition;
    pair<int, int> firstEle;
    regex e2 (pattern);
    if (regex_search(s, m, e2) == false) {// Modified:2017.04 // Modified:2017.04 handle the constructor include a constructor appendent
    	startPosition = tempStartPosition;
    	firstEle.first = startPosition + position;
    	firstEle.second = startPosition - 1 + tempFunc.size() + position;
    	s = tempFunc;
    }
    else {
    	startPosition = m.position(0);
    	firstEle.first = startPosition + position;
    	firstEle.second = startPosition - 1 + m[0].str().size() + position;
    	s = m[0].str();
      switch(classTypeOfFile) {
        case C_CPP: {
          regex first (functionTypeDecorate + varType + pointerType);
          regex_search(s, m, first);
          s = m.suffix();
          break;
        }
        case JAVA: {
          regex first (functionTypeDecorate + varType + "\\s+");
          regex_search(s, m, first);
          s = m.suffix();
          break;
        }
        default:
                cout<<"\n\nUNKNOWN CLASS TYPE. NO FINDFUNCTION PRESENT FOR THIS CLASS TYPE!\n\n";
                break;
      }
    }
    switch(classTypeOfFile){ //Modified:2017.04
        case C_CPP:{
            regex second (funcId);
            regex_search(s, m, second);
            functionName = functionName + m[0].str();
            if(functionName.compare("if")==0 || functionName.compare("while")==0 || functionName.compare("for")==0 || functionName.compare("switch")==0 || functionName.compare("catch") == 0) {//Modified: 2017.04
              return make_pair(make_pair(-1, -1), "");
            }
            functionName = functionName + "(";
            pattern = typeDecorate + varType + pointerAndAddress + varId;
            vector<string> parameters;
            regex third (pattern);
            s = m.suffix();
            string typeDecorateWithoutBlank = "(signed|mutable|static|unsigned|const|volatile|long|struct)";
            regex fourth (typeDecorateWithoutBlank);
            regex fifth (varType);
            string temp = "";
            string temp2;
            string var;
            smatch mtemp;
            bool whetherLong = false;
            while (regex_search(s, m, third)) {
            	var = "";
                temp = m[0].str();
                while (regex_search(temp, mtemp, fourth) == true) {
                    temp2 = mtemp[0].str();
                    temp = mtemp.suffix();
                    if (temp2 == "long")  {
                        whetherLong = true;
                    }
                    var += temp2 + " ";
                }
                if (!whetherLong) {
                    regex_search(temp, mtemp, fifth);
                    var += mtemp[0].str();
                } else {
                    var.pop_back();
                }
                size_t start = temp.find_first_of('*'); //Modification: 2018.01, changed data type to size_t
                while (start!=string::npos) {
                    start = temp.find_first_of('*', start + 1);
                    var = var + "*";
                }
                if (temp.find_first_of('&')!=string::npos) {
                    var = var + "&";
                }
                parameters.push_back(var);
                s = m.suffix();
            }

            if (parameters.empty()) {
                functionName = functionName + ")";
            } else {
            size_t i; //Modification: 2018.01, changed data type to size_t
                for (i = 0; i < parameters.size() - 1; i++){
                    functionName = functionName + parameters[i] + ",";
                }
                functionName = functionName + parameters[i] + ")";
            }
            break;
        }
        case JAVA:{
            //Modified: 2017.04: Implement JAVA find function here
            regex second (funcId);
            regex_search(s, m, second);
            functionName = functionName + m[0].str();
            if(functionName.compare("if")==0 || functionName.compare("while")==0 || functionName.compare("for")==0 || functionName.compare("switch")==0 || functionName.compare("catch") == 0) {//Modified: 2017.04
              return make_pair(make_pair(-1, -1), "");
            }
            functionName = functionName + "(";
            pattern = varType + "\\s+" + varId;
            vector<string> parameters;
            regex third (pattern);
            s = m.suffix();
            regex fourth (varType);
            string temp = "";
            string var;
            smatch mtemp;
            while (regex_search(s, m, third)) {
                var = "";
                temp = m[0].str();
                regex_search(temp, mtemp, fourth);
                var += mtemp[0].str();
                parameters.push_back(var);
                s = m.suffix();
            }
            if (parameters.empty()) {
                functionName = functionName + ")";
            } else {
                size_t i; //Modification: 2018.01, changed data type to size_t
                for (i = 0; i < parameters.size() - 1; i++){
                    functionName = functionName + parameters[i] + ",";
                }
                functionName = functionName + parameters[i] + ")";
            }
            break;
        }
        default:
            cout<<"\n\nUNKNOWN CLASS TYPE. NO FINDFUNCTION PRESENT FOR THIS CLASS TYPE!\n\n";
            break;
    }

    return make_pair(firstEle, functionName);
}

/*
* 1. Function Description: 
*    find the possible entry of a class in the lineOfCode starting from the index 'pointer'
*
* 2. Parameters: 
*    input: the line of code
*    position: the starting index from which we start searching
*    classTypeOfFile: class type of the file. Now only support C_CPP and JAVA.
*
* 3. Return:
*    return a pair<string, int>, in which contains the class name as a string and the class end position as int.
*
* 4. Creation Time and Owner: 
*    Version 2017.04
*/
pair<string, int> FunctionParser::findClass(string input, int position, ClassType classTypeOfFile) {
    string s = input.substr(position);
    std::size_t commentPosition = s.find("//");
    s = s.substr(0, commentPosition);
    string pattern, classKeyWord, className, appendent;
    switch(classTypeOfFile){
        case C_CPP:
            classKeyWord = "class";
            className = "[_0-9a-zA-Z:]+(\\s*<[_0-9a-zA-Z:,\\*\\s<>\\[\\]]*>)?";
            appendent = "(\\s+:\\s+((public|protected|private)\\s+)?[_0-9a-zA-Z:]+(\\s*<[_0-9a-zA-Z:,\\s<>\\[\\]]*>)?(\\s*,\\s*((public|protected|private)\\s+)?[_0-9a-zA-Z:]+(\\s*<[_0-9a-zA-Z:,\\s<>\\[\\]]*>)?)*)?";
            pattern = classKeyWord + "\\s+" + className + appendent;
            break;
        case JAVA:
            classKeyWord = "class";
            className = "[_0-9a-zA-Z]+(\\s*<[_0-9a-zA-Z,\\s<>\\[\\]]*>)?";
            appendent = "(\\s+((implements\\s+[_0-9a-zA-Z\\.]+(\\s*<[_0-9a-zA-Z\\.,\\s<>\\[\\]]*>)?(\\s*,\\s*[_0-9a-zA-Z\\.]+(\\s*<[_0-9a-zA-Z\\.,\\s<>\\[\\]]*>)?)*)|(extends\\s+[_0-9a-zA-Z\\.]+(\\s*<[_0-9a-zA-Z\\.,\\s<>\\[\\]]*>)?))){0,2}";
            pattern = classKeyWord + "\\s+" + className + appendent;
            break;
        default:
            cout<<"\n\nUNKNOWN CLASS TYPE. NO FINDCLASS PRESENT FOR THIS CLASS TYPE!\n\n";
            break;
    }
    smatch m;
    
    regex e (pattern);
    
    if(regex_search(s, m, e) == false) {
        return make_pair("", -1);
    }
    
    int startPosition = m.position(0);
    int endPosition = startPosition - 1 + m[0].str().size() + position;
    
    s = m[0].str();
    regex first (classKeyWord + "\\s+" + className);
    regex_search(s, m, first);
    s = m[0].str();
    regex second (classKeyWord + "\\s+");
    regex_search(s, m, second);
    string result = m.suffix();
    return make_pair(result, endPosition);
}

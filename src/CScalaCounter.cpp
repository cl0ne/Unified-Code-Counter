//! Code counter class methods for the Scala language.
/*!
* \file CScalaCounter.cpp
*
* This file contains the code counter class methods for the Scala language.
*
* Because documents can go out of date
* there are comments about the development of this module 
* AND
* other ideas about improving UCC
* which may be found at the end of this file or other file(s) as noted.
*
* You are strongly encouraged to fully read these comments before
* making changes and also to keep the comments in step with the new code!
*
* Added to 2015.12 by Randy Maxwell
*   Changes started on 2015_10_16
*   Changes  ended  on 2015_10_24
*/

#include "CScalaCounter.h"

/*!
* Constructs a CScalaCounter object.
*/
CScalaCounter::CScalaCounter( string lang ) : CCJavaCsScalaCounter( lang )
{
#ifdef	ENABLE_1_PASS_PARSER
	// Use the new single pass parser
	use_1pass_parser = false;
#endif

	// Set the internal default parsing values.
	// These default Scala parsing values agree with Scala version and authority.
	// See comments below for support of other Scala versions from other creators.
	//
	classtype = SCALA;
	language_name = "Scala";
	language_version = "2.11.5";
	language_version_authority = "scala.org";	// creator of this version
    
    //Modification: 11.2016 Ext-4
    file_extension = CUtil::getExtensionsToLanguage("Scala", file_extension);
    
	//file_extension.push_back(".scala");

// INVARIANT ! ! !   NO  DUPLICATE  STRINGS
//
//  ALL these declarations are used to build a 1 pass parser element array in CCodeCounter.
//  Do NOT declare any duplicate keys (strings) here!
//  CCJavaCsScalaCounter will declare SEEMING duplicates that get changed to aux meanings flags.
//  We keep this way so that the multi pass parsing code will still work.
//  IF 1 pass parsing really works out well (as Randy hopes) the code can be refactored later.
//

	// Below comments give differences from the Java parse support that
	// was first used as a example to do this Scala parse support.
	//
	// Please keep these comments as there may be a significant fraction
	// of Scala developers coming from a Java background and we might want 
	// to give these details out in a document sometime.
	//

	// Compiler directive (or preprocessor) keywords
	directive.push_back("import");
	directive.push_back("package");

	// Data keywords; please see comments below about these
	//
	data_name_list.push_back("abstract");		// don't see how class inheritance is data
	data_name_list.push_back("Array");
	// data_name_list.push_back("ArrayList");	// this is Java and not Scala
	data_name_list.push_back("Boolean");		// different capitalization from Java
	data_name_list.push_back("Byte");			// different capitalization from Java
	data_name_list.push_back("Char");			// different capitalization from Java
	data_name_list.push_back("class");			// don't see how a class is data
	data_name_list.push_back("Double");			// different capitalization from Java
	data_name_list.push_back("extends");		// don't see how class inheritance is data
	data_name_list.push_back("Float");			// different capitalization from Java
	data_name_list.push_back("HashMap");
	data_name_list.push_back("HashSet");
	data_name_list.push_back("implements");		// don't see how class inheritance is data
	data_name_list.push_back("Int");			// different capitalization from Java
	// data_name_list.push_back("interface");	not in Scala
	data_name_list.push_back("LinkedHashMap");
	data_name_list.push_back("LinkedList");
	data_name_list.push_back("Long");			// different capitalization from Java
	// data_name_list.push_back("native");		not in Scala
	data_name_list.push_back("object");			// don't see how an object is data
	data_name_list.push_back("override");		// don't see how class interface is data
	data_name_list.push_back("private");		// don't see how access visibility is data
	data_name_list.push_back("protected");		// don't see how access visibility is data
	// data_name_list.push_back("public");		not in Scala, public is defaut visibility
	data_name_list.push_back("sealed");			
	data_name_list.push_back("Short");			// different capitalization from Java
	data_name_list.push_back("static");
	data_name_list.push_back("String");
	data_name_list.push_back("TreeMap");
	data_name_list.push_back("val");			// immuteable data
	data_name_list.push_back("var");			// muteable data
	data_name_list.push_back("Vector");
	// data_name_list.push_back("void");		not in Scala
	// data_name_list.push_back("volatile");	not in Scala

	// Executable keywords
	//
	exec_name_list.push_back("break");
	exec_name_list.push_back("case");
	exec_name_list.push_back("catch");
	// exec_name_list.push_back("continue");	not in Scala
	exec_name_list.push_back("def");
	// exec_name_list.push_back("default");		not in Scala
	exec_name_list.push_back("do");
	exec_name_list.push_back("else");
	exec_name_list.push_back("finally");
	exec_name_list.push_back("for");
	exec_name_list.push_back("if");	
	exec_name_list.push_back("match");			// somewhat like Java switch statement
	exec_name_list.push_back("new");
	exec_name_list.push_back("return");
	exec_name_list.push_back("super");
	// exec_name_list.push_back("switch");		not in Scala
	exec_name_list.push_back("this");
	exec_name_list.push_back("throw");
	// exec_name_list.push_back("throws");		not in Scala
	exec_name_list.push_back("try");
	exec_name_list.push_back("while");

	// Math, Trig and Log functions
	// Scala math functions are implicitly imported due to
	//		import scala._
	// being implicitly done for all Scala source files.
	//
	// So one way is that the source code would prefix all the below function names with
	//		math.
	// to declare the name of the collection for disambiguation to satisfy the compiler.
	//
	// Another way is to explicitly
	//		import scala.math._
	// to get all of them or
	//		import scala.math.cos
	// to get the cosine of an angle function for example.
	// in any case the Scala syntax is simply calling the function without the
	//		math.
	// prefix.
	// 
	math_func_list.push_back("abs");
	math_func_list.push_back("cbrt");
	math_func_list.push_back("ceil");
	// math_func_list.push_back("copySign");	not in Scala math package
	math_func_list.push_back("E");
	math_func_list.push_back("exp");
	math_func_list.push_back("expm1");
	math_func_list.push_back("floor");
	// math_func_list.push_back("getExponent");	not in Scala math package
	math_func_list.push_back("hypot");
	math_func_list.push_back("IEEEremainder");	
	math_func_list.push_back("max");
	math_func_list.push_back("min");
	// math_func_list.push_back("nextAfter");	not in Scala math package
	// math_func_list.push_back("nextUp");		not in Scala math package
	math_func_list.push_back("Pi");				// different capitalization from Java
	math_func_list.push_back("pow");
	math_func_list.push_back("random");
	math_func_list.push_back("rint");
	math_func_list.push_back("round");
	// math_func_list.push_back("scalb");		not in Scala math package
	math_func_list.push_back("signum");
	math_func_list.push_back("sqrt");
	math_func_list.push_back("toRadians");
	math_func_list.push_back("toDegrees");
	math_func_list.push_back("ulp");

	trig_func_list.push_back("acos");
	trig_func_list.push_back("asin");
	trig_func_list.push_back("atan");
	trig_func_list.push_back("atan2");
	trig_func_list.push_back("cos");
	trig_func_list.push_back("cosh");
	trig_func_list.push_back("sin");
	trig_func_list.push_back("sinh");
	trig_func_list.push_back("tan");
	trig_func_list.push_back("tanh");

	log_func_list.push_back("log");
	log_func_list.push_back("log10");
	log_func_list.push_back("log1p");


/*
Use this (needs to be enhanced for 1 pass approach)

	cmplx_cyclomatic_logic_list.push_back("||");
	cmplx_cyclomatic_logic_list.push_back("&&");

	cmplx_cyclomatic_case_list.push_back("case");
	cmplx_cyclomatic_switch_list.push_back("switch");

BELOW IS WRONG !
Do NOT add more to cmplx_cyclomatic_list ! ! !

	if ( "SCALA" == lang )
		cmplx_cyclomatic_list.push_back("match");		// switch not in Scala
	else
		cmplx_cyclomatic_list.push_back("switch");
	cmplx_cyclomatic_listVals.push_back( CYCLOMATIC_CC3 );

	// Cyclomatic Complexity ring 2: same as CC1 with 2 CC2 Operators/equivalent Keywords list.  Logical AND  OR
	aux_meaning_keys.push_back("&&");
	aux_meaning_vals.push_back( CYCLOMATIC_CC2 );

	aux_meaning_keys.push_back("||");
	aux_meaning_vals.push_back( CYCLOMATIC_CC2 );
*/

// TODO: Add more keywords/values for Object Orientation and Functional Programming metrics collection


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
int CScalaCounter::CountDirectiveSLOC(filemap* fmap, results* result, filemap* fmapBak)
{
	bool			contd      = false;
	bool			trunc_flag = false;
	size_t			idx, strSize;
	unsigned int	cnt = 0;
	string			exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$";
	string			strDirLine = "";

	filemap::iterator itfmBak = fmapBak->begin();
	currentPhyLine = 0;
	for ( filemap::iterator iter = fmap->begin(); iter != fmap->end(); iter++, itfmBak++ )
	{
		currentPhyLine++;
		if ( CUtil::CheckBlank( iter->line ) )
			continue;

		if ( print_cmplx )
		{
			cnt = 0;
			CUtil::CountTally( " " + iter->line, directive, cnt, 1, exclude, "", "", &result->directive_count );
		}

		if ( !contd )
		{
			// if not a continuation of a previous directive
			for ( vector<string>::iterator viter = directive.begin(); viter != directive.end(); viter++ )
			{
				// ensures the keyword stands alone, avoid, e.g., #ifabc
				if ( ((idx = CUtil::FindKeyword(iter->line, *viter)) != string::npos) && idx == 0 )
				{
					contd = true;
					break;
				}
			}
			if (contd)
			{
				strSize = CUtil::TruncateLine(itfmBak->line.length(), 0, this->lsloc_truncate, trunc_flag);
				if (strSize > 0)
					strDirLine = itfmBak->line.substr(0, strSize);
				result->directive_lines[PHY]++;
			}
 		}
		else
		{
			// continuation of a previous directive
			strSize = CUtil::TruncateLine(itfmBak->line.length(), strDirLine.length(), this->lsloc_truncate, trunc_flag);
			if (strSize > 0)
				strDirLine += "\n" + itfmBak->line.substr(0, strSize);
			result->directive_lines[PHY]++;
		}
	
		if (contd)
		{
			// drop continuation symbol			NO Line Continuation for SCALA
			//if (strDirLine[strDirLine.length()-1] == '\\')
			//	strDirLine = strDirLine.substr(0, strDirLine.length()-1);

			// if a directive or continuation of a directive (no continuation symbol found)
			// if (iter->line[iter->line.length()-1] != ',' && iter->line[iter->line.length()-1] != '\\')
			// There is NO continuation symbol in Scala
			if (iter->line[iter->line.length()-1] != ',' )
			{
				contd = false;
				if (result->addSLOC(strDirLine, trunc_flag))
					result->directive_lines[LOG]++;
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
int CScalaCounter::LanguageSpecificProcess( filemap* fmap, results* result, filemap* fmapBak )
{
	unsigned int	paren_count			= 0;
	bool			for_flag			= false;
	bool			found_for			= false;
	bool			found_forifwhile	= false;
	bool			found_while			= false;
	char			prev_char			= 0;
	bool			data_continue		= false;
	bool			inArrayDec			= false;
	string			strLSLOC			= "";
	string			strLSLOCBak			= "";
	unsigned int	openBrackets		= 0;

	filemap::iterator fit, fitbak;
	string line, lineBak;
	StringVector loopLevel;

	unsigned int phys_exec_lines = 0;
	unsigned int phys_data_lines = 0;
	unsigned int temp_lines = 0;
	unsigned int cnt = 0;
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$";

	for (fit = fmap->begin(), fitbak = fmapBak->begin(); fit != fmap->end(); fit++, fitbak++)
	{
		line = fit->line;

		// insert blank at the beginning (for searching keywords)
		line = ' ' + line;
		lineBak = ' ' + fitbak->line;
	
		// do not process blank lines
		// blank line means blank_line/comment_line/directive
		if ( ! CUtil::CheckBlank( line ) )
		{
			LSLOC( result, line, lineBak, strLSLOC, strLSLOCBak, paren_count, for_flag, found_forifwhile, found_while,
				prev_char, data_continue, temp_lines, phys_exec_lines, phys_data_lines, inArrayDec, found_for,
				openBrackets, loopLevel );

			if ( print_cmplx )
			{
				cnt = 0;
				CUtil::CountTally( line, exec_name_list, cnt, 1, exclude, "", "", &result->exec_name_count );
			}

			result->exec_lines[PHY] += phys_exec_lines;
			phys_exec_lines = 0;

			result->data_lines[PHY] += phys_data_lines;
			phys_data_lines = 0;
		}
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
* \param paren_cnt count of parenthesis
* \param forflag found for flag
* \param found_forifwhile found for, if, or while flag
* \param found_while found while flag
* \param prev_char previous character
* \param data_continue continuation of a data declaration line
* \param temp_lines tracks physical line count
* \param phys_exec_lines number of physical executable lines
* \param phys_data_lines number of physical data lines
* \param inArrayDec marks an array declaration
* \param found_for found for loop
* \param openBrackets number of open brackets (no matching close bracket)
* \param loopLevel nested loop level
*/
void CScalaCounter::LSLOC(results* result, string line, string lineBak, string &strLSLOC, string &strLSLOCBak, unsigned int &paren_cnt,
							bool &forflag, bool &found_forifwhile, bool &found_while, char &prev_char, bool &data_continue,
							unsigned int &temp_lines, unsigned int &phys_exec_lines, unsigned int &phys_data_lines,
							bool &inArrayDec, bool &found_for, unsigned int &openBrackets, StringVector &loopLevel)
{
	// paren_cnt is used with 'for' statement only
	size_t start = 0; //starting index of the working string
	size_t i = 0, strSize;
	bool found_do = false;
	bool found_try = false;
	bool found_else = false; 
	bool trunc_flag = false;
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$:";
	string dataExclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$:().";	// avoid double count of casts as data and executable lines (e.g. set { m_uiValue = (uint)value; }

	unsigned int cnt = 0;

	string tmp = CUtil::TrimString( strLSLOC );
	if ( tmp.size() > 0 )
	{
		// do, try
		found_do   = ( CUtil::FindKeyword(tmp, "do") != string::npos );
		found_try  = ( CUtil::FindKeyword(tmp, "try") != string::npos );
		// else is treated differently, else is included in SLOC, do and try are not
		found_else = ( CUtil::FindKeyword(tmp, "else") != string::npos );
	}

	// there may be more than 1 logical SLOC in this line
	while ( i < line.length() )
	{
		switch ( line[i] )
		{
		case ';': case '{': // LSLOC terminators
			                // ';' for normal executable or declaration statement
			                // '{' for starting a function or 'do' stmt or a block (which is counted)
			// get the previous logical mark until i-1 index is the new LSLOC
			// except 'do' precedes '{'
			// except '}' precedes ';' ??
			// do nothing inside 'for' statement
			if ( found_for == true && paren_cnt > 0 && line[i] == ';' )
				break;

			// record open bracket for nested loop processing
			if ( print_cmplx )
			{
				if ( line[i] == '{' )
				{
					openBrackets++;
					if ( (unsigned int)loopLevel.size() < openBrackets )
						loopLevel.push_back("");
				}
				else
				{
					if ( (unsigned int)loopLevel.size() > openBrackets && openBrackets > 0 )
						loopLevel.pop_back();
				}
			}

			// case 'while(...);', 'while(...) {', and '} while(...);'
			// this case is handled in case ')'
			if (found_while && found_forifwhile)
			{
				found_while = false;
				found_forifwhile = false;
				start = i + 1;
				break;
			}

			if ( line[i] == '{' )
			{
				if (prev_char == '=')
					inArrayDec = true;

				// continue until seeing ';'
				if ( inArrayDec )
					break;

				// case for(...); and if (...) {
				// these specials are handled
				if ( found_forifwhile )
				{
					found_forifwhile = false;
					start = i + 1;
					break;
				}

				// check if 'do' precedes '{'
				if ( !found_do && !found_try && !found_else )
				{
					// find for 'do' in string before tmp string
					tmp = CUtil::TrimString(line.substr(start, i - start));
					found_do = (tmp == "do");		// found 'do' statement
					found_try = (tmp == "try");		// found 'try' statement
					// same as else
					found_else = (tmp == "else");	// found 'else' statement
				}
				if (found_do || found_try || found_else)
				{
					if (found_do && print_cmplx)
					{
						if (loopLevel.size() > 0) loopLevel.pop_back();
						loopLevel.push_back("do");
					}
					found_do = false;
					found_try = false;
					if (!found_else)
					{
						// everything before 'do', 'try' are cleared
						strLSLOC = "";
						strLSLOCBak = "";
						start = i + 1;
					}
					break; // do not store '{' following 'do'
				}
			}

			// wrong, e.g., a[]={1,2,3};
			if (line[i] == ';' && prev_char == '}')
			{
				// check if in array declaration or not
				// if no, skip, otherwise, complete the SLOC containing array declaration
				if (!inArrayDec)
				{
					start = i + 1;
					break;
				}
			}

			inArrayDec = false;

			// check for empty statement (=1 LSLOC)
			if (CUtil::TrimString(line.substr(start, i + 1 - start)) == ";" && strLSLOC.length() < 1)
			{
				strLSLOC = ";";
				strLSLOCBak = ";";
			}
			else
			{
				strSize = CUtil::TruncateLine(i + 1 - start, strLSLOC.length(), this->lsloc_truncate, trunc_flag);
				if (strSize > 0)
				{
					strLSLOC += line.substr(start, strSize);
					strLSLOCBak += lineBak.substr(start, strSize);
				}
			}
			if (result->addSLOC(strLSLOCBak, trunc_flag))
			{
				cnt = 0;
				CUtil::CountTally(strLSLOC, data_name_list, cnt, 1, dataExclude, "", "", &result->data_name_count);

				temp_lines++;
				if (data_continue == true && line[i] == ';')
				{
					result->data_lines[LOG]++;
					phys_data_lines = temp_lines;
				}
				else
				{
					if (cnt > 0 && line[i] == ';')
					{
						result->data_lines[LOG]++;
						phys_data_lines = temp_lines;
					}
					else
					{
						result->exec_lines[LOG]++;
						phys_exec_lines = temp_lines;
					}
				}
			}
			else if (data_continue == true && line[i] == ';')
				phys_data_lines = temp_lines;
			else
				phys_exec_lines = temp_lines;
			data_continue = false;
			temp_lines = 0;
			strLSLOC = strLSLOCBak = "";
			start = i + 1;

			// reset some flagging parameters
			forflag = false;
			paren_cnt = 0;
			found_while = false;
			found_forifwhile = false;
			found_for = false;

			break;
		case '(':
			if (forflag)
				paren_cnt++;
			else
			{
				// handle 'for', 'while', 'if' the same way
				tmp = CUtil::TrimString(line.substr(start, i));
				if (CUtil::FindKeyword(tmp, "for") != string::npos
				//	|| CUtil::FindKeyword(tmp, "foreach") != string::npos  foreach NOT in Scala
					|| CUtil::FindKeyword(tmp, "while") != string::npos
					|| CUtil::FindKeyword(tmp, "if") != string::npos)
				{
					forflag = true;
					paren_cnt++;

					if (print_cmplx && (unsigned int)loopLevel.size() > openBrackets && openBrackets > 0)
						loopLevel.pop_back();

					if (CUtil::FindKeyword(tmp, "for") != string::npos)
					{
						if (print_cmplx)
							loopLevel.push_back("for");
						found_for = true;
					}
					else if (CUtil::FindKeyword(tmp, "while") != string::npos)
					{
						if (print_cmplx)
							loopLevel.push_back("while");
						found_while = true;
					}
					//else if (print_cmplx && CUtil::FindKeyword(tmp, "foreach") != string::npos)
					//	loopLevel.push_back("foreach");

					// record nested loop level
					if (print_cmplx)
					{
						if (CUtil::FindKeyword(tmp, "if") == string::npos)
						{
							unsigned int loopCnt = 0;
							for (StringVector::iterator lit = loopLevel.begin(); lit < loopLevel.end(); lit++)
							{
								if ((*lit) != "")
									loopCnt++;
							}
							if ((unsigned int)result->cmplx_nestloop_count.size() < loopCnt)
								result->cmplx_nestloop_count.push_back(1);
							else
								result->cmplx_nestloop_count[loopCnt-1]++;
						}
					}
				}
			}
			break;
		case ')':
			if (forflag)
			{
				if (paren_cnt > 0)
					paren_cnt--;
				if (paren_cnt == 0)
				{
					// handle 'for', 'while', 'if'
					strSize = CUtil::TruncateLine(i + 1 - start, strLSLOC.length(), this->lsloc_truncate, trunc_flag);
					if (strSize > 0)
					{
						strLSLOC += line.substr(start, strSize);
						strLSLOCBak += lineBak.substr(start, strSize);
					}
					if (result->addSLOC(strLSLOCBak, trunc_flag))
						result->exec_lines[LOG]++;
					strLSLOCBak = strLSLOC = "";
					phys_exec_lines = temp_lines;
					temp_lines = 0;
					start = i + 1;
					found_forifwhile = true;
					forflag = false;
					found_for = false;
				}
			}
			break;
		case '}':
			// skip '}' when found ';' and then '}' because '{' is counted already
			// also, {} is also skipped, counted
			if (prev_char == ';' || prev_char == '{' || prev_char == '}')
				if (!inArrayDec) start = i + 1;

			// record close bracket for nested loop processing
			if (print_cmplx)
			{
				if (openBrackets > 0)
					openBrackets--;
				if (loopLevel.size() > 0)
					loopLevel.pop_back();
			}
			break;
		}

		if (line[i] != ' ' && line[i] != '\t') 
		{
			// if ;}}} --> don't count }}} at all
			// also, if {}}} --> don't count }}} at all
			// if ( !(line[i] == '}' && (prev_char == ';' || prev_char == '{'))) // see case '}' above
			prev_char = line[i];

			// change to not found if a char appears before
			if (line[i] != ')' && found_forifwhile)
				found_forifwhile = false;
		}
		i++;
	}

	tmp = CUtil::TrimString(line.substr(start, i - start));
	strSize = CUtil::TruncateLine(tmp.length(), strLSLOC.length(), this->lsloc_truncate, trunc_flag);
	if (strSize > 0)
	{
		strLSLOC += tmp.substr(0, strSize);
		tmp = CUtil::TrimString(lineBak.substr(start, i - start));
		strLSLOCBak += tmp.substr(0, strSize);

		// drop continuation symbol			NOT USED FOR SCALA
		//if (strLSLOC[strLSLOC.length()-1] == '\\')
		//{
		//	strLSLOC = strLSLOC.substr(0, strLSLOC.length()-1);
		//	strLSLOCBak = strLSLOCBak.substr(0, strLSLOCBak.length()-1);
		//}
	}
	
	// make sure that we are not beginning to process a new data line
	cnt = 0;
	CUtil::CountTally(strLSLOC, data_name_list, cnt, 1, exclude, "", "", NULL);

	if (cnt > 0)
		data_continue = true;
	if (data_continue)
		temp_lines++;
	if (temp_lines == 0 && phys_data_lines == 0 && phys_exec_lines == 0)
		phys_exec_lines = 1;
}

/*!
* Parses lines for function/method names.
*
* \param line line to be processed
* \param lastline last line processed
* \param functionStack stack of functions
* \param functionName function name found
* \param functionCount function count found
*
* \return 1 if function name is found
*/
int CScalaCounter::ParseFunctionName(const string &line, string &lastline,
	filemap &functionStack, string &functionName, unsigned int &functionCount)
{
	string tline, str;
	size_t idx, tidx, cnt, cnt2;
	unsigned int fcnt, cyclomatic_cnt = 0;
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$";

	tline = CUtil::TrimString(line);
	idx = tline.find('{');
	if (idx != string::npos)
	{
		// check whether it is at first index, if yes then function name is at above line
		if (idx == 0)
		{
			lineElement element(++functionCount, lastline);
			functionStack.push_back(element);
			lastline.erase();
		}
		else
		{
			str = tline.substr(0, idx);
			tidx = cnt = cnt2 = 0;
			if (str[0] != '(' && str[0] != ':' && (lastline.length() < 1 || lastline[lastline.length() - 1] != ':'))
			{
				while (tidx != string::npos)
				{
					tidx = str.find('(', tidx);
					if (tidx != string::npos)
					{
						cnt++;
						tidx++;
					}
				}
				if (cnt > 0)
				{
					tidx = 0;
					while (tidx != string::npos)
					{
						tidx = str.find(')', tidx);
						if (tidx != string::npos)
						{
							cnt2++;
							tidx++;
						}
					}
				}
			}
			// make sure parentheses are closed and no parent class listed
			if ((cnt > 0 && cnt == cnt2) || (lastline.length() > 0 && lastline[lastline.length() - 1] == ';'))
				lastline = str;
			else
				lastline += " " + str;
			lineElement element(++functionCount, CUtil::TrimString(lastline));
			functionStack.push_back(element);
			lastline.erase();
		}
	}
	else if (tline.length() > 0 && tline[tline.length() - 1] != ';' &&
		lastline.length() > 0 && lastline[lastline.length() - 1] != ';')
	{
		// append until all parentheses are closed
		tidx = lastline.find('(');
		if (tidx != string::npos)
		{
			cnt = 1;
			while (tidx != string::npos)
			{
				tidx = lastline.find('(', tidx + 1);
				if (tidx != string::npos)
					cnt++;
			}
			tidx = lastline.find(')');
			while (tidx != string::npos)
			{
				cnt++;
				tidx = lastline.find(')', tidx + 1);
			}
			if (cnt % 2 != 0)
				lastline += " " + tline;
			else
				lastline = tline;
		}
		else
			lastline = tline;
	}
	else
		lastline = tline;

	idx = line.find('}');
	if (idx != string::npos && !functionStack.empty())
	{
		str = functionStack.back().line;
		fcnt = functionStack.back().lineNumber;
		functionStack.pop_back();
		idx = str.find('(');

		if (idx != string::npos)
		{
			// search for cyclomatic complexity keywords and other possible keywords
			CUtil::CountTally(str, cmplx_cyclomatic_list, cyclomatic_cnt, 1, exclude, "", "", 0, casesensitive);
			if (cyclomatic_cnt <= 0 && CUtil::FindKeyword(str, "match") == string::npos &&
				CUtil::FindKeyword(str, "try") == string::npos && CUtil::FindKeyword(str, "finally") == string::npos &&
				CUtil::FindKeyword(str, "return") == string::npos && str.find('=') == string::npos)
			{
				string myFunctionName = CUtil::ClearRedundantSpaces(str.substr(0, idx));

			#ifdef	_DEBUG
				if ( myFunctionName.size() == 0 )
				{
					// PrintCyclomaticComplexity will now skip any empty Function Names.
					// Done this way as is easier and less Risk of adding Defects from complex Cyclomatic Complexity code
					// TODO: Go through the CC code and the ParseFunctionName code and try to SAFELY improve.
				//	cout << endl << "CScalaCounter::ParseFunctionName() Empty function name allowed" << endl;
				}
			#endif
				// May have the actual arg list so look for first ( and do not copy
				size_t myIdx = myFunctionName.find( '(' );
				if ( myIdx != string::npos )
					myFunctionName = myFunctionName.substr( 0, myIdx );
				functionName = myFunctionName;
				functionCount = fcnt;
				lastline.erase();
				return 1;
			}
		}
		lastline.erase();
	}
	return 0;
}

/*! 
* How this Scala language parse module was developed:
*
* Goals:
* Support parsing of Scala language as it currently exists.
* Generate as accurate a count of Logical Source Lines Of Code LSLOC as possible (1).
* Provide for fairly easy changes in the future to support other Scala versions (2).
*
*		Problem Space exploration:
* (1) The Scala language makes use of implicit construction of statements.
* There is no Scala line continuation character as in C/C++ or Java (those use \ )
* Although a semicolon character (;) may be used to terminate a Scala statement
* the semicolon is OPTIONAL and NOT the recommended style.
* About the only time the semicolon appears is when 
* more than 1 Scala statement appears on the same Physical line in the file.
* So correctly parsing Scala Logical lines (a complete statement) is less 
* deterministic than for C/C++ or Java.  In this respect Scala resembles Ruby.
*
* (2) There is an offical document covering all Scala syntax.  
* EBNF representation can be found @ scala.org
* It seems the Scala compiler is written in Scala (about 30,000+ Lines Of Code).
* (2a) There are also other creators of Scala based language versions (not TypeSafe)
* or creators of other similar languages that started from Scala syntax.
* We might expect that there would be a fairly large overlap of keywords, 
* implied meanings, and other language features.  
* In other words there may be more similarities than differences.
* 
* (3) Other open source language parsing tools that may help here:
*		Doxygen 
* also parses various source languages but the internals are different: 
* It uses a C style front end parser, C Flex parse engine and rules, builds an 
* Abstract Syntax Tree representation and then several different output options.
* I believe Doxygen has a better approach but there may be slower performance with
* an AST building overhead (but also potentially better capture of metrics as well).
*		LEX or related tools
* Even with EBNF grammer not sure how to proceed here.
* Tried COCO /R
* Got stuck trying finish (maybe someone else can succeed)
* Looked then at PEG representation because there is direct support in Boost for PEG.
* I think going forward this makes the most sense for C++ approach.
*
*		Solution Space (near term):
* With the UCC 2015.12 release (on which these changes are based) the C/C++ Java parse
* support included the most details of complexity metrics.  So it made some sense to try
* to fit in with that approach.  Another reason to use this approach is that Scala uses
* the same // notation for single line or trailing comments and same for multiline.
*
* Please see Informal Analysis of Parse Code below for the new Alternate approach.
*
*		Solution Space (after near term):
* To better support (2 and 2a) it would be nice if the code were changed to:
* Set the default values used for parsing a language as is now done; 
* (fixed strings and flag settings directly embedded in the code) and add code 
* before parsing starts that reads OPTIONAL file(s) to change some or all of the
* detailed strings/flags.  Initial implementation wanted was to add XML parsing 
* but there is no support for XML parsing in the standard library.  So a simple
* text file format seems to be the best approach.
*
* Allowing user editable files to set detailed parse values:
*		Pros
* Users can just edit a file to support new versions 
*	or alternate language forms (in some cases).
* It may be possible for several new language parsers to be added with
* a very small effort.  (maybe Groovy, Rust, ...)
*		Cons
* The user may try to apply one or more detailed setting(s) 
* that prevent generation of valid results.
*
*   User may then contact support for UCC to get help!
*   The Help feature can be extended to cover this situation.
*   Could give example problem and solution and also refer to 
*   new -help_lang Scala (or other language) help feature.
*
* Because the default values are embedded in the code 
* a small enhancement to the Help feature:
*		-help_lang Scala (for exmaple) or
*		-help_lang All	 (for all embedded languages supported)
* may output all the built in default settings for a language in a 
* text file with the same format as used for reading those settings.
* So user has working example(s) of given language(s) parse settings.
*
*		Design Decision: User editable files to set parse values? Yes!
* So I believe that the Pros outweigh the Cons.  (Design tradeoff)
* Note that this approach may also help the Doxygen project as well
* (but that is off topic here :)
*
*		Solution Space (mid term):
* (A) Extending Complexity metrics (collection & reporting):
* Current set of metrics seem to be based only on Procedural code.
* Object Oriented developers may want to know:
*   What is the class inheritance depth?  (nesting)
*   How many methods are in a given class?  (over 20 seems to be getting messy)
*   and so on.
* Functional developers may want to know:
*   What are the higher order functions used?
*   How complex are the Lambda expressions used?
*   and so on.
* Data structure and useage details:
*   What is the structure/union complexity? (depth of nesting, number of elements, ...)
*   How are various known data structures used?  Arrays, Dictionaries, Lists, Maps, ...
*   and so on.
* Visibility or Access to Code and/or Data: (types of Encapsulation and a measure of how much)
*   Namespaces
*   Public
*   Protected
*   Private
*   Friend
*
* Add args to enable/disable: OO, Functional, Data structure metrics.
* So original performance without this collection would be as before.
* Note: Data structure metrics would require new code as currently 
* (for example in C/C++) do not even parse .h files
*
* (B) Performance improvements:  (Modified: 2015.12 Threads and others DONE)
*       Minor (maybe lots of places in the code):
* There may be several instances of temporary object creation/destruction
* that is not really needed.  Suggest using AMD CodeAnalyst or similar profiler.
*
*		Major:
* Changed to use multiple threads.  (done: see UCCThread)
* Gathering performance data running an optimized Release build shows
* the CPU and the Disk subsystems have margin for faster results.
* Added a command line arg to set the number of extra threads to create.
* Threads allow faster Reading/Counting just after Read and for Diff
* but not Duplicate detection due to how existing code works.
* Boost Thread library worked fine along with a Semaphore library.
*
* ================================================================
*				Informal Analysis of Parse Code
*
*		Existing Parsing Approach:
*		-------------------------
* Searching for Directive (preprocessor) keywords (for example)
* (in a looping Linear way)
* For each Physical line in a file
*	For each Directive keyword
*		See if the keyword is within the line
*		If found stop, else loop (possible Exhaustive Search)
*
* There are other similar loops for various other types of keywords
*
* So each Physical line in a file is parsed through more than once.
* 
* In addition there are some loops where every character in the line
* is checked for special meanings {} () [] ; \ // and so on.
*
* Consider from a Searching algorithms point of view:
* this can approximate a form of an almost Exhaustive Search 
* (or at least Linear Searches done more than once)!
*
*		Alternate Parsing Approach: 
*		--------------------------
* Ideally each Physical line in a file should be parsed just once.
* Instead of multiple passes to maybe find special keywords/symbols
* the code should just work through the Physical line once.
*
*		How to implement this new single pass parse approach?
* For each Physical line in a file:
*	Code is needed to get parts of a given Physical line so that:
*	An API like strtok would be needed (using whitespace as delimiter?)
*	Then for each piece of text found:
*	Try to pick it apart based on various special characters.
*	When no more smaller pieces can be found,
*	Search a data structure for matches of a small piece.
*	Based on the match found, set various flags, counters, etc.
*	Keep going until no more pieces of the Physical line remain.
* Loop back to parse another Physical line.
*
* What Data structure(s) are useful to support the single pass?
* 
* How about an Array of all the keywords and all the special symbols?
*	The keyword/symbol Array would be sorted so a Binary search may be used.
* 
* If the keyword/symbol Array has a match, then the Type of the match is needed.
* Consider a Type array with a 1 to 1 correspondence to the keyword array.
* The Type array would give guidance on how to proceed with parsing.
* Types may be: Math function(s), Data declarations, Directives (preprocessor), etc.
*
*
* If having 2 related Arrays seems to be awkward how about a Dictionary approach?
* 
* Either way it seems feasible to only need 1 pass through the Physical lines.
*
* It is difficult to guesstimate but I would say a 2+ times speed up is likely.
*
*		Informal Comparison of Existing and Alternate:
*		---------------------------------------------
* Existing approach is a nice modular approach where the various
* parse needs, blank line detection, handling embedded or multi-line
* comments, searching for different types of keywords, dealing with
* looping and nested code constructs have been separated out.  
* 
* It may be said that the Existing approach is like a Structured
* Programming approach.
*
* Alternate approach takes small text pieces that were isolated
* and then looks for them in a sorted Array (or Dictionary).
*
* If we consider the small text pieces as low level 'data' about
* the parsed code, then the Alternate is a Data Driven approach.
* 
*		Motivation for Alternate Parsing Approach:
*		-----------------------------------------
* OK, I admit like many other developers I can be basically lazy.
* So when doing some fairly mindless debugging to see details of
* HOW the Existing code works, I kept thinking about the strengths
* and weaknesses in use.
*
* So I could spend more time making incremental changes to a parse
* approach that I knew was correct for other languages but may not
* be a good approach for Scala (with implicit statement termination).
* 
* Also I had prior experience working on a 4GL (natural) language
* with the Alternate approach of cached Dictionary lookups.
*
* So since I am contributing this Scala support on my own time,
* it makes sense to me to try out the Alternate way.
*/

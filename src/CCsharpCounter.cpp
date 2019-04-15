//! Code counter class methods for the C# language.
/*!
* \file CCsharpCounter.cpp
*
* This file contains the code counter class methods for the C# language.
*/

#include "CCsharpCounter.h"

/*!
* Constructs a CCsharpCounter object.
*/
CCsharpCounter::CCsharpCounter( string lang ) : CCJavaCsScalaCounter( lang )
{
	classtype = CSHARP;
	language_name = "C#";

	isVerbatim = false;
    
    //Modification: 11.2016 Ext-4
    file_extension = CUtil::getExtensionsToLanguage("C#", file_extension);

//	file_extension.push_back(".cs");

	directive.push_back("#define");
	directive.push_back("#else");
	directive.push_back("#elif");
	directive.push_back("#endif");
	directive.push_back("#endregion");
	directive.push_back("#error");
	directive.push_back("#if");
	directive.push_back("#line");
	directive.push_back("#region");
	directive.push_back("#undef");
	directive.push_back("#warning");

	data_name_list.push_back("abstract");
	data_name_list.push_back("bool");
	data_name_list.push_back("byte");
	data_name_list.push_back("char");
	data_name_list.push_back("class");
	data_name_list.push_back("const");
	data_name_list.push_back("decimal");
	data_name_list.push_back("delegate");
	data_name_list.push_back("double");
	data_name_list.push_back("enum");
	data_name_list.push_back("event");
	data_name_list.push_back("explicit");
	data_name_list.push_back("extern");
	data_name_list.push_back("float");
	data_name_list.push_back("implicit");
	data_name_list.push_back("int");
	data_name_list.push_back("interface");
	data_name_list.push_back("internal");
	data_name_list.push_back("long");
	data_name_list.push_back("namespace");
	data_name_list.push_back("object");
	data_name_list.push_back("operator");
	data_name_list.push_back("override");
	data_name_list.push_back("private");
	data_name_list.push_back("protected");
	data_name_list.push_back("public");
	data_name_list.push_back("readonly");
	data_name_list.push_back("sbyte");
	data_name_list.push_back("sealed");
	data_name_list.push_back("short");
	data_name_list.push_back("static");
	data_name_list.push_back("string");
	data_name_list.push_back("struct");
	data_name_list.push_back("uint");
	data_name_list.push_back("ulong");
	data_name_list.push_back("unsafe");
	data_name_list.push_back("ushort");
	data_name_list.push_back("using");
	data_name_list.push_back("virtual");
	data_name_list.push_back("void");
	data_name_list.push_back("volatile");

	exec_name_list.push_back("as");
	exec_name_list.push_back("base");
	exec_name_list.push_back("break");
	exec_name_list.push_back("case");
	exec_name_list.push_back("catch");
	exec_name_list.push_back("checked");
	exec_name_list.push_back("continue");
	exec_name_list.push_back("default");
	exec_name_list.push_back("do");
	exec_name_list.push_back("else");
	exec_name_list.push_back("finally");
	exec_name_list.push_back("fixed");
	exec_name_list.push_back("for");
	exec_name_list.push_back("foreach");
	exec_name_list.push_back("goto");
	exec_name_list.push_back("if");
	exec_name_list.push_back("lock");
	exec_name_list.push_back("new");
	exec_name_list.push_back("return");
	exec_name_list.push_back("sizeof");
	exec_name_list.push_back("stackalloc");
	exec_name_list.push_back("switch");
	exec_name_list.push_back("this");
	exec_name_list.push_back("throw");
	exec_name_list.push_back("try");
	exec_name_list.push_back("typeof");
	exec_name_list.push_back("unchecked");
	exec_name_list.push_back("while");

	math_func_list.push_back("abs");
	math_func_list.push_back("cbrt");
	math_func_list.push_back("ceil");
	math_func_list.push_back("copysign");
	math_func_list.push_back("erf");
	math_func_list.push_back("erfc");
	math_func_list.push_back("exp");
	math_func_list.push_back("exp2");
	math_func_list.push_back("expm1");
	math_func_list.push_back("fabs");
	math_func_list.push_back("floor");
	math_func_list.push_back("fdim");
	math_func_list.push_back("fma");
	math_func_list.push_back("fmax");
	math_func_list.push_back("fmin");
	math_func_list.push_back("fmod");
	math_func_list.push_back("frexp");
	math_func_list.push_back("hypot");
	math_func_list.push_back("ilogb");
	math_func_list.push_back("ldexp");
	math_func_list.push_back("lgamma");
	math_func_list.push_back("llrint");
	math_func_list.push_back("lrint");
	math_func_list.push_back("llround");
	math_func_list.push_back("lround");
	math_func_list.push_back("modf");
	math_func_list.push_back("nan");
	math_func_list.push_back("nearbyint");
	math_func_list.push_back("nextafter");
	math_func_list.push_back("nexttoward");
	math_func_list.push_back("pow");
	math_func_list.push_back("remainder");
	math_func_list.push_back("remquo");
	math_func_list.push_back("rint");
	math_func_list.push_back("round");
	math_func_list.push_back("scalbln");
	math_func_list.push_back("scalbn");
	math_func_list.push_back("sqrt");	
	math_func_list.push_back("tgamma");
	math_func_list.push_back("trunc");

	trig_func_list.push_back("cos");
	trig_func_list.push_back("cosh");
	trig_func_list.push_back("sin");
	trig_func_list.push_back("sinh");
	trig_func_list.push_back("tan");
	trig_func_list.push_back("tanh");
	trig_func_list.push_back("acos");
	trig_func_list.push_back("acosh");
	trig_func_list.push_back("asinh");
	trig_func_list.push_back("atanh");
	trig_func_list.push_back("asin");
	trig_func_list.push_back("atan");
	trig_func_list.push_back("atan2");

	log_func_list.push_back("log");
	log_func_list.push_back("log10");
	log_func_list.push_back("log1p");
	log_func_list.push_back("log2");
	log_func_list.push_back("logb");

	cmplx_preproc_list.push_back("define");
	cmplx_preproc_list.push_back("elif");
	cmplx_preproc_list.push_back("else");
	cmplx_preproc_list.push_back("endif");
	cmplx_preproc_list.push_back("endregion");
	cmplx_preproc_list.push_back("error");
	cmplx_preproc_list.push_back("if");
	cmplx_preproc_list.push_back("import");
	cmplx_preproc_list.push_back("line");
	cmplx_preproc_list.push_back("region");
	cmplx_preproc_list.push_back("undef");
	cmplx_preproc_list.push_back("warning");

/* Below Set in CCJavaCsScalaCounter base class
	cmplx_cyclomatic_list.push_back("if");
	cmplx_cyclomatic_list.push_back("case");
	cmplx_cyclomatic_list.push_back("while");
	cmplx_cyclomatic_list.push_back("for");
	cmplx_cyclomatic_list.push_back("foreach");
	cmplx_cyclomatic_list.push_back("catch");
	cmplx_cyclomatic_list.push_back("?");

	cmplx_cyclomatic_logic_list.push_back("||");
	cmplx_cyclomatic_logic_list.push_back("&&");
	//cmplx_cyclomatic_logic_list.push_back("^");

	cmplx_cyclomatic_case_list.push_back("case");
	cmplx_cyclomatic_switch_list.push_back("switch");
*/

        //Modification: 2018.01 Integration starts
        two_char_operator_list.push_back("??");
        two_char_operator_list.push_back("->");
        two_char_operator_list.push_back("::");
        two_char_operator_list.push_back("=>");

        one_char_operator_list.push_back("@");

        for (StringVector::iterator it = cmplx_cyclomatic_list.begin(); it != cmplx_cyclomatic_list.end(); it++)
            keyword_operators.insert(*it);

        for (StringVector::iterator it = directive.begin(); it != directive.end(); it++)
            keyword_operators.insert(*it);

        for (StringVector::iterator it = data_name_list.begin(); it != data_name_list.end(); it++)
            keyword_operators.insert(*it);

        for (StringVector::iterator it = exec_name_list.begin(); it != exec_name_list.end(); it++)
            keyword_operators.insert(*it);
        //Modification: 2018.01 Integration ends
        
}

/*!
* Perform preprocessing of file lines before counting.
*
* \param fmap list of file lines
*
* \return method status
*/
int CCsharpCounter::PreCountProcess(filemap* fmap)
{
	size_t i;
	bool found;
	filemap::iterator fit;
	for (fit = fmap->begin(); fit != fmap->end(); fit++) 
	{
		if (fit->line.empty())
			continue;
		// check for parenthesis within attribute brackets [...()]
		found = false;
		for (i = 0; i < fit->line.length(); i++)
		{
			if (fit->line[i] == '[')
				found = true;
			else if (found)
			{
				if (fit->line[i] == ']')
					found = false;
				else if (fit->line[i] == '(' || fit->line[i] == ')')
					fit->line[i] = '$';
			}
		}
	}
	return 0;
}

/*!
* Replaces up to ONE quoted string inside a string starting at idx_start.
*
* \param strline string to be processed
* \param idx_start index of line character to start search
* \param contd specifies the quote string is continued from the previous line
* \param CurrentQuoteEnd end quote character of the current status
*
* \return method status
*/
int CCsharpCounter::ReplaceQuote(string &strline, size_t &idx_start, bool &contd, char &CurrentQuoteEnd)
{
	size_t idx_end, idx_quote, idx_verbatim;
	char noQuoteEscapeFront = 0x00;

	if (contd)
	{
		idx_start = 0;
		if (strline[0] == CurrentQuoteEnd)
		{
			if (!isVerbatim || (strline.length() < 2) || (strline[1] != '"'))
			{
				idx_start = 1;
				contd = false;
				return 1;
			}
		}
		else
			strline[0] = '$';
	}
	else
	{
		// accommodate C# verbatim string (e.g. @"\")
		isVerbatim = false;
		idx_verbatim = strline.find_first_of("@");
		if (idx_verbatim != string::npos && idx_verbatim + 1 == idx_start)
			isVerbatim = true;

		// handle two quote chars in some languages, both " and ' may be accepted
		idx_start = FindQuote(strline, QuoteStart, idx_start, QuoteEscapeFront);
		if (idx_start != string::npos)
		{
			idx_quote = QuoteStart.find_first_of(strline[idx_start]);
			CurrentQuoteEnd = QuoteEnd[idx_quote];
		}
		else
		{
			idx_start = strline.length();
			return 0;
		}
	}

	// accommodate C# verbatim string (e.g. @"\")
	if (isVerbatim)	// verbatim string
		idx_end = CUtil::FindCharAvoidEscape(strline, CurrentQuoteEnd, idx_start + 1, noQuoteEscapeFront);
	else
		idx_end = CUtil::FindCharAvoidEscape(strline, CurrentQuoteEnd, idx_start + 1, QuoteEscapeFront);
	if (idx_end == string::npos)
	{
		idx_end = strline.length() - 1;
		strline.replace(idx_start + 1, idx_end - idx_start, idx_end - idx_start, '$');
		contd = true;
		idx_start = idx_end + 1;
	}
	else
	{
		if ((isVerbatim && (strline.length() > idx_end + 1) && (strline[idx_end+1] == '"')) ||
			((QuoteEscapeRear) && (strline.length() > idx_end + 1) && (strline[idx_end+1] == QuoteEscapeRear)))
		{
			strline[idx_end] = '$';
			strline[idx_end+1] = '$';
		}
		else
		{
			isVerbatim = false;
			contd = false;
			strline.replace(idx_start + 1, idx_end - idx_start - 1, idx_end - idx_start - 1, '$');
			idx_start = idx_end + 1;
		}
	}
	return 1;
}

/*!
* Constructs a CCsharpHtmlCounter object.
*/
CCsharpHtmlCounter::CCsharpHtmlCounter()
{
	classtype = CSHARP_HTML;
	language_name = "C#/HTML";

	file_extension.clear();
	file_extension.push_back(".*cshtm");
}

/*!
* Constructs a CCsharpXmlCounter object.
*/
CCsharpXmlCounter::CCsharpXmlCounter()
{
	classtype = CSHARP_XML;
	language_name = "C#/XML";

	file_extension.clear();
	file_extension.push_back(".*csxml");
}

/*!
* Constructs a CCsharpAspCounter object.
*/
CCsharpAspCounter::CCsharpAspCounter()
{
	classtype = CSHARP_ASP_S;
	language_name = "C#/ASPNET";

	file_extension.clear();
	file_extension.push_back(".*csasps");
}

//Modification: 2018.01 Integration starts
bool CCsharpCounter::HasOneDoubleQuote(string line) {
        string two_doublequotes = "\"\"";

        //Modification: 2018.01, changed data type to size_t
        size_t idx = line.find(two_doublequotes);
        while (idx != string::npos) {
                line.replace(idx, 2, "  ");
                idx += 2;
                idx = line.find(two_doublequotes, idx);
        }

    return line.find("\"") != string::npos;
}

int CCsharpCounter::GetLineUntilEndOfMultistringIfAny(int current_line_idx, string &line, filemap &fmap, map<string, unsigned int> &nonfunction_operator_counts) {
        string beginning_of_raw_string = "@\"";
        //Modification: 2018.01, changed data type to size_t
        size_t idx = line.find(beginning_of_raw_string);
        size_t curr_line_idx = current_line_idx;
		(void) nonfunction_operator_counts; //Modification: 2018.01 : Unused argument
        

        // If not the beginning of a raw string then don't need to do anything
        if (idx == string::npos)
                return curr_line_idx;

        if (HasOneDoubleQuote(line.substr(idx + 2)))
                return curr_line_idx;

        string line_with_end_of_multistring_if_any = "";
    string curr_line = CUtil::TrimString(fmap[curr_line_idx].line, 0);

    do {
        line_with_end_of_multistring_if_any.append(curr_line);
        curr_line_idx++;
        curr_line = CUtil::TrimString(fmap[curr_line_idx].line, 0);
    } while (!HasOneDoubleQuote(curr_line) && curr_line_idx < fmap.size() - 2);

    line = line_with_end_of_multistring_if_any + curr_line;

    return curr_line_idx;
}

//Modification: 2018.01 Integration ends

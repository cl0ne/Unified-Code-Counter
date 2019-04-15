//! Code counter class methods for the C/C++ languages.
/*!
* \file CCCounter.cpp
*
* This file contains the code counter class methods for the C/C++ languages.
*
* Changed UCC 2015.12 release by Randy Maxwell
*   Addition of file extensions .inl .inc to support working with Linux and Boost for example
*		.cxx .hxx as comments ready for anyone wanting Legacy C++ file support
*/

#include "CCCounter.h"

/*!
* Constructs a CCCounter object.
*/
CCCounter::CCCounter( string lang ) : CCJavaCsScalaCounter( lang )
{
	classtype = C_CPP;
	language_name = "C_CPP";
    
    //Modification: 11.2016 Ext-4 starts
    file_extension = CUtil::getExtensionsToLanguage("C_CPP", file_extension);

	/*file_extension.push_back(".c");
	file_extension.push_back(".cc");
	file_extension.push_back(".cpp");
	 file_extension.push_back(".cxx");	// Legacy C++  Enable if you want
	file_extension.push_back(".inl");	// inline files for example see Boost libraries		Modification: 2015.12

	// These file types are not processed when doing Cyclomatic Complexity calculations
	file_extension.push_back(".h");
	file_extension.push_back(".hh");
	file_extension.push_back(".hpp");
	file_extension.push_back(".hxx");	// Legacy C++  Enable if you want
	file_extension.push_back(".inc");	// include files for example see Linux				Modification: 2015.12 */
    
    //Modification: 11.2016 Ext-4 ends


	directive.push_back("#define");
	directive.push_back("#dictionary");
	directive.push_back("#error");
	directive.push_back("#if");
	directive.push_back("#ifdef");
	directive.push_back("#ifndef");
	directive.push_back("#else");
	directive.push_back("#elif");
	directive.push_back("#endif");
	directive.push_back("#import");
	directive.push_back("#include");
	directive.push_back("#line");
	directive.push_back("#module");
	directive.push_back("#pragma");
	directive.push_back("#undef");
	directive.push_back("#using");
    
    directive.push_back("# define");
	directive.push_back("# dictionary");
	directive.push_back("# error");
	directive.push_back("# if");
	directive.push_back("# ifdef");
	directive.push_back("# ifndef");
	directive.push_back("# else");
	directive.push_back("# elif");
	directive.push_back("# endif");
	directive.push_back("# import");
	directive.push_back("# include");
	directive.push_back("# line");
	directive.push_back("# module");
	directive.push_back("# pragma");
	directive.push_back("# undef");
	directive.push_back("# using");

	data_name_list.push_back("asm");
	data_name_list.push_back("auto");
	data_name_list.push_back("bool");
	data_name_list.push_back("char");
	data_name_list.push_back("class");
	data_name_list.push_back("const");
	data_name_list.push_back("double");
	data_name_list.push_back("enum");
	data_name_list.push_back("explicit");
	data_name_list.push_back("extern");
	data_name_list.push_back("FILE");
	data_name_list.push_back("float");
	data_name_list.push_back("friend");
	data_name_list.push_back("inline");
	data_name_list.push_back("int");
	data_name_list.push_back("long");
	data_name_list.push_back("mutable");
	data_name_list.push_back("namespace");
	data_name_list.push_back("operator");
	data_name_list.push_back("register");
	data_name_list.push_back("short");
	data_name_list.push_back("static");
	data_name_list.push_back("string");
	data_name_list.push_back("struct");
	data_name_list.push_back("template");
	data_name_list.push_back("typedef");
	data_name_list.push_back("union");
	data_name_list.push_back("unsigned");
	data_name_list.push_back("using");
	data_name_list.push_back("virtual");
	data_name_list.push_back("void");
	data_name_list.push_back("volatile");
	data_name_list.push_back("wchar_t");

	exec_name_list.push_back("break");
	exec_name_list.push_back("case");
	exec_name_list.push_back("catch");
	exec_name_list.push_back("cerr");
	exec_name_list.push_back("cin");
	exec_name_list.push_back("clog");
	exec_name_list.push_back("const_cast");
	exec_name_list.push_back("continue");
	exec_name_list.push_back("cout");
	exec_name_list.push_back("default");
	exec_name_list.push_back("delete");
	exec_name_list.push_back("do");
	exec_name_list.push_back("dynamic_cast");
	exec_name_list.push_back("else");
	exec_name_list.push_back("entry");
	exec_name_list.push_back("for");
	exec_name_list.push_back("goto");
	exec_name_list.push_back("if");
	exec_name_list.push_back("new");
	exec_name_list.push_back("reinterpret_cast");
	exec_name_list.push_back("return");
	exec_name_list.push_back("sizeof");
	exec_name_list.push_back("stderr");
	exec_name_list.push_back("stdin");
	exec_name_list.push_back("stdout");
	exec_name_list.push_back("switch");
	exec_name_list.push_back("static_cast");
	exec_name_list.push_back("throw");
	exec_name_list.push_back("try");
	exec_name_list.push_back("typeid");
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
	math_func_list.push_back("fdim");
	math_func_list.push_back("floor");
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

	cmplx_preproc_list.push_back("#define");
	cmplx_preproc_list.push_back("#dictionary");
	cmplx_preproc_list.push_back("#elif");
	cmplx_preproc_list.push_back("#else");
	cmplx_preproc_list.push_back("#endif");
	cmplx_preproc_list.push_back("#error");
	cmplx_preproc_list.push_back("#if");
	cmplx_preproc_list.push_back("#ifdef");
	cmplx_preproc_list.push_back("#ifndef");
	cmplx_preproc_list.push_back("#import");
	cmplx_preproc_list.push_back("#include");
	cmplx_preproc_list.push_back("#line");
	cmplx_preproc_list.push_back("#module");
	cmplx_preproc_list.push_back("#pragma");
	cmplx_preproc_list.push_back("#undef");
	cmplx_preproc_list.push_back("#using");

	cmplx_pointer_list.push_back("->");

/* Below Set in CCJavaCsScalaCounter
	cmplx_cyclomatic_list.push_back("if");
	cmplx_cyclomatic_list.push_back("case");
	cmplx_cyclomatic_list.push_back("while");
	cmplx_cyclomatic_list.push_back("for");
	cmplx_cyclomatic_list.push_back("catch");
	cmplx_cyclomatic_list.push_back("?");


	cmplx_cyclomatic_logic_list.push_back("||");
	cmplx_cyclomatic_logic_list.push_back("&&");
	//cmplx_cyclomatic_logic_list.push_back("^");

	cmplx_cyclomatic_case_list.push_back("case");
	cmplx_cyclomatic_switch_list.push_back("switch");
*/

	skip_cmplx_cyclomatic_file_extension_list.push_back(".h");
	skip_cmplx_cyclomatic_file_extension_list.push_back(".hh");
	skip_cmplx_cyclomatic_file_extension_list.push_back(".hpp");
	//skip_cmplx_cyclomatic_file_extension_list.push_back(".hxx");	// Legacy C++  Enable if you want
	skip_cmplx_cyclomatic_file_extension_list.push_back(".inc");

        two_char_operator_list.push_back("->");
        two_char_operator_list.push_back("::");

        for (StringVector::iterator it = cmplx_cyclomatic_list.begin(); it != cmplx_cyclomatic_list.end(); it++)
            keyword_operators.insert(*it);

        for (StringVector::iterator it = cmplx_preproc_list.begin(); it != cmplx_preproc_list.end(); it++)
            keyword_operators.insert(*it);

        for (StringVector::iterator it = directive.begin(); it != directive.end(); it++)
            keyword_operators.insert(*it);

        for (StringVector::iterator it = data_name_list.begin(); it != data_name_list.end(); it++)
            keyword_operators.insert(*it);

        for (StringVector::iterator it = exec_name_list.begin(); it != exec_name_list.end(); it++)
            keyword_operators.insert(*it);
}

/*!
* Returns true if the line ends with '\', which is a way to
* continue a line of code in C++ (and other languages)to the next line.
*
* \param line the string to examine
* \return true if the line ends with '\' else false
*/
bool CCCounter::EndsWithOpenString(const string &line) {
    if (line.size() > 0 && line[line.size() - 1] == '\\') {
        return true;
    }
    return false;
}

/*!
* Concatenates all the physical lines belonging to a multiline string and stores
* this in the parameter 'line'. If the source line at curr_line_idx does not contain any multiline string
* then this function will store just that one source line into 'line' and return
* the original curr_line_idx that was passed into the function.
*
* \param curr_line_idx the index of the line in fmap from which to begin
* \param line the concatenation of all source lines beginning from curr_line_idx until there is no multistring
* \param fmap the source code for the file
* \param nonfunction_operator_counts the map for counting the number of times a nonfunction operator (e.g. symbolic operators such as '+' and '[')
*
* \return the index into fmap at which the first physical line, at or after curr_line_idx, does not continue to the next source line as a multiline string
*/
int CCCounter::GetLineUntilEndOfMultistringIfAny(int curr_line_idx, string &line, filemap &fmap, map<string, unsigned int> &nonfunction_operator_counts) {
    string line_with_end_of_multistring_if_any = "";
    //Modification: 2018.01 USC, changed data type to size_t
    size_t current_line_idx = curr_line_idx ; 

    string curr_line = CUtil::TrimString(fmap[current_line_idx].line, 1);
    while (EndsWithOpenString(curr_line) && current_line_idx < fmap.size() - 1) {
        line_with_end_of_multistring_if_any.append(curr_line.substr(0, curr_line.size() - 1));
        nonfunction_operator_counts["\\"]++;
        current_line_idx++;
        curr_line = CUtil::TrimString(fmap[current_line_idx].line, 1);
    }
    line = line_with_end_of_multistring_if_any + curr_line;

    return current_line_idx;
}


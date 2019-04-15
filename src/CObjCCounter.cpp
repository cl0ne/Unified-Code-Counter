//! Code counter class methods for the Objective C languages.
/*!
* \file CObjCCounter.cpp
*
* This file contains the code counter class methods for the Objective C languages.
*/

#include "CObjCCounter.h"

/*!
* Constructs a CObjCCounter object.
*/

CObjCCounter::CObjCCounter(string lang) : CCJavaCsScalaCounter(lang)
{
	classtype = OBJC;

	language_name = "OBJC";
	//file_extension.push_back(".m"); share extension with Matlab, need to specify in extfile
	//file_extension.push_back(".mm"); share extension with NeXtMidas, need to specify in extfile
	//file_extension.push_back(".h"); share extension with C_CPP, need to specify in extfile

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

	// directive.push_back("@interface"); // in Objective C, althought "@interface" should be compile direction, the interface definition should be excutable line
	// directive.push_back("@autoreleasepool"); count as excutable line now, will double count as compile direction and excutable line
	// directive.push_back("@synchronized"); count as excutable line now, will double count as compile direction and excutable line
	// directive.push_back("@selector"); // not very sure how to use it now
	directive.push_back("@implementation");
	directive.push_back("@protocol");
	directive.push_back("@optional");
	directive.push_back("@required");
	directive.push_back("@end");
	directive.push_back("@encode");
	directive.push_back("@throw");
	directive.push_back("@property");
	directive.push_back("@synthesize");



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
	data_name_list.push_back("id");
	data_name_list.push_back("BOOL");

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
	exec_name_list.push_back("print");
	exec_name_list.push_back("finally");

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

/* Below Set in CCJavaCsScalaCounter base class
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
}


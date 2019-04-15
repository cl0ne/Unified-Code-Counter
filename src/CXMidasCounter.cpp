//! Code counter class methods for the X-Midas macro language.
/*!
* \file CXMidasCounter.cpp
*
* This file contains the code counter class methods for the X-Midas macro language.
*/

#include "CXMidasCounter.h"

/*!
* Constructs a CXMidasCounter object.
*/
CXMidasCounter::CXMidasCounter()
{
	classtype = XMIDAS;
	language_name = "X-Midas";
    
    //Modification: 11.2016 Ext-4
    file_extension = CUtil::getExtensionsToLanguage("X-Midas", file_extension);
	//file_extension.push_back(".txt");

	data_name_list.push_back("local");

	exec_name_list.push_back("jump");

	math_func_list.push_back("firhlbrt");
	math_func_list.push_back("firparks");
	math_func_list.push_back("hilbert");
	math_func_list.push_back("imfft");
	math_func_list.push_back("invfft");
	math_func_list.push_back("magnitude");
	math_func_list.push_back("marray");
	math_func_list.push_back("mfft");
	math_func_list.push_back("modulo");
	math_func_list.push_back("morph");
	math_func_list.push_back("mpoly");
	math_func_list.push_back("multiply");
	math_func_list.push_back("normalize");
	math_func_list.push_back("parray");
	math_func_list.push_back("passfilt");
	math_func_list.push_back("phase");
	math_func_list.push_back("polar");
	math_func_list.push_back("polyeval");
	math_func_list.push_back("polyfit");
	math_func_list.push_back("polyphase");
	math_func_list.push_back("pulse");
	math_func_list.push_back("random");
	math_func_list.push_back("sarray");
	math_func_list.push_back("smooth");
	math_func_list.push_back("spectra");
	math_func_list.push_back("subtract");
	math_func_list.push_back("transfft");
	math_func_list.push_back("transform");
	math_func_list.push_back("transpose");

	trig_func_list.push_back("sincosine");

	log_func_list.push_back("logarithm");
}

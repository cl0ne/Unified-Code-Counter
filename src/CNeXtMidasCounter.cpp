//! Code counter class methods for the NeXtMidas macro language.
/*!
* \file CNeXtMidasCounter.cpp
*
* This file contains the code counter class methods for the NeXtMidas macro language.
*/

#include "CNeXtMidasCounter.h"

/*!
* Constructs a CNeXtMidasCounter object.
*/
CNeXtMidasCounter::CNeXtMidasCounter()
{
	classtype = NEXTMIDAS;
	language_name = "NeXtMidas";
    
    //Modification: 11.2016 Ext-4
    file_extension = CUtil::getExtensionsToLanguage("NeXtMidas", file_extension);
    
	//file_extension.push_back(".mm");

	exclude_keywords.push_back("enddo");
	exclude_keywords.push_back("endfor");

	data_name_list.push_back("global");

	exec_name_list.push_back("do");
	exec_name_list.push_back("foreach");

	math_func_list.push_back("filter");
	math_func_list.push_back("ifft");

	cmplx_cond_list.push_back("do");
	cmplx_cond_list.push_back("foreach");

	cmplx_logic_list.push_back("isTrue");
	cmplx_logic_list.push_back("isFalse");
	cmplx_logic_list.push_back("oeq");
	cmplx_logic_list.push_back("feq"); 
	cmplx_logic_list.push_back("noeq");
	cmplx_logic_list.push_back("nfeq");

    //Modification 2016.11; USC
    cmplx_cyclomatic_list.push_back("do");
    cmplx_cyclomatic_list.push_back("foreach");
}

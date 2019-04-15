//! Code counter class methods for the HTML language.
/*!
* \file CHtmlCounter.cpp
*
* This file contains the code counter class methods for the HTML language.
*/

#include "CHtmlCounter.h"

/*!
* Constructs a CHtmlCounter object.
*/
CHtmlCounter::CHtmlCounter()
{
	classtype = HTML;
	language_name = "HTML";
    
    //Modification: 11.2016 Ext-4
    file_extension = CUtil::getExtensionsToLanguage("HTML", file_extension);
    
	//file_extension.push_back(".*htm");

	BlockCommentStart.push_back("<!--");
	BlockCommentEnd.push_back("-->");

	exec_name_list.push_back("address");
	exec_name_list.push_back("applet");
	exec_name_list.push_back("area");
	exec_name_list.push_back("a");
	exec_name_list.push_back("base");
	exec_name_list.push_back("basefont");
	exec_name_list.push_back("big");
	exec_name_list.push_back("blockquote");
	exec_name_list.push_back("body");
	exec_name_list.push_back("br");
	exec_name_list.push_back("b");
	exec_name_list.push_back("caption");
	exec_name_list.push_back("center");
	exec_name_list.push_back("cite");
	exec_name_list.push_back("code");
	exec_name_list.push_back("dd");
	exec_name_list.push_back("dfn");
	exec_name_list.push_back("dir");
	exec_name_list.push_back("div");
	exec_name_list.push_back("dl");
	exec_name_list.push_back("dt");
	exec_name_list.push_back("em");
	exec_name_list.push_back("font");
	exec_name_list.push_back("form");
	exec_name_list.push_back("h1");
	exec_name_list.push_back("h2");
	exec_name_list.push_back("h3");
	exec_name_list.push_back("h4");
	exec_name_list.push_back("h5");
	exec_name_list.push_back("h6");
	exec_name_list.push_back("head");
	exec_name_list.push_back("hr");
	exec_name_list.push_back("html");
	exec_name_list.push_back("img");
	exec_name_list.push_back("input");
	exec_name_list.push_back("isindex");
	exec_name_list.push_back("i");
	exec_name_list.push_back("jsp");
	exec_name_list.push_back("kbd");
	exec_name_list.push_back("link");
	exec_name_list.push_back("li");
	exec_name_list.push_back("map");
	exec_name_list.push_back("menu");
	exec_name_list.push_back("meta");
	exec_name_list.push_back("ol");
	exec_name_list.push_back("option");
	exec_name_list.push_back("param");
	exec_name_list.push_back("pre");
	exec_name_list.push_back("p");
	exec_name_list.push_back("samp");
	exec_name_list.push_back("script");
	exec_name_list.push_back("select");
	exec_name_list.push_back("small");
	exec_name_list.push_back("span");
	exec_name_list.push_back("strike");
	exec_name_list.push_back("strong");
	exec_name_list.push_back("style");
	exec_name_list.push_back("sub");
	exec_name_list.push_back("sup");
	exec_name_list.push_back("table");
	exec_name_list.push_back("td");
	exec_name_list.push_back("textarea");
	exec_name_list.push_back("th");
	exec_name_list.push_back("title");
	exec_name_list.push_back("tr");
	exec_name_list.push_back("tt");
	exec_name_list.push_back("ul");
	exec_name_list.push_back("u");
	exec_name_list.push_back("var");
}

/*!
* Constructs a CHtmlPhpCounter object.
*/
CHtmlPhpCounter::CHtmlPhpCounter()
{
	classtype = HTML_PHP;
	language_name = "HTML/PHP";

	file_extension.clear();
	file_extension.push_back(".*htmphp");
}

/*!
* Constructs a CHtmlJspCounter object.
*/
CHtmlJspCounter::CHtmlJspCounter()
{
	classtype = HTML_JSP;
	language_name = "HTML/JSP";

	file_extension.clear();
	file_extension.push_back(".*htmjsp");
}

/*!
* Constructs a CHtmlAspCounter object.
*/
CHtmlAspCounter::CHtmlAspCounter()
{
	classtype = HTML_ASP;
	language_name = "HTML/ASP";

	file_extension.clear();
	file_extension.push_back(".*htmasp");
}

/*!
* Constructs a CHtmlColdFusionCounter object.
*/
CHtmlColdFusionCounter::CHtmlColdFusionCounter()
{
	classtype = HTML_CFM;
	language_name = "HTML/ColdFusion";

	file_extension.clear();
	file_extension.push_back(".*htmcfm");
}

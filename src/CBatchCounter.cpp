//! Code counter class methods for the DOS Batch language.
/*!
* \file CBatchCounter.cpp
*
* This file contains the code counter class methods for the DOS Batch language.
* This also includes the MS-DOS commands.*/

#include <string.h>
#include "CBatchCounter.h"
//#include "MainObject.h"

// changed calling tolower() to use lowerChars array
extern unsigned char lowerChars[256];

/*!
* Constructs a CBatchCounter object.
*/
CBatchCounter::CBatchCounter ()
{
	classtype = BATCH;
	language_name = "DOS_Batch";
    
    //Modification: 11.2016 Ext-4
    file_extension = CUtil::getExtensionsToLanguage("DOS_Batch", file_extension);
	//file_extension.push_back(".bat");

	QuoteStart = "\"'";
	QuoteEnd = "\"'";
	QuoteEscapeFront = '\\';
	ContinueLine = "^";
	
	//Comments in .bat
    	LineCommentStart.push_back("rem");
	LineCommentStart.push_back("::");

	continue_keywords.push_back("else");

	//commands in .bat
	exec_name_list.push_back("ansi.sys");
	exec_name_list.push_back("append");
	exec_name_list.push_back("arp");
	exec_name_list.push_back("assign");
	exec_name_list.push_back("assoc");
	exec_name_list.push_back("at");
	exec_name_list.push_back("atmadm");
	exec_name_list.push_back("attrib");
	exec_name_list.push_back("backup");
	exec_name_list.push_back("batch");
	exec_name_list.push_back("bootcfg");
	exec_name_list.push_back("bootsect");
	exec_name_list.push_back("break");
	exec_name_list.push_back("cacls");
	exec_name_list.push_back("call");
	exec_name_list.push_back("cd");
	exec_name_list.push_back("chcp");
	exec_name_list.push_back("chdir");
	exec_name_list.push_back("chkdsk");
	exec_name_list.push_back("chkntfs");
	exec_name_list.push_back("choice");
	exec_name_list.push_back("cipher");
	exec_name_list.push_back("cls");
	exec_name_list.push_back("cmd");
	exec_name_list.push_back("color");
	exec_name_list.push_back("command");
	exec_name_list.push_back("comp");
	exec_name_list.push_back("compact");
	exec_name_list.push_back("control");
	exec_name_list.push_back("convert");
	exec_name_list.push_back("copy");
	exec_name_list.push_back("ctty");
	exec_name_list.push_back("date");
	exec_name_list.push_back("debug");
	exec_name_list.push_back("defrag");
	exec_name_list.push_back("del");
	exec_name_list.push_back("delete");
	exec_name_list.push_back("deltree");
	exec_name_list.push_back("dir");
	exec_name_list.push_back("disable");
	exec_name_list.push_back("diskcomp");
	exec_name_list.push_back("diskcopy");
	exec_name_list.push_back("diskpart");
	exec_name_list.push_back("doskey");
	exec_name_list.push_back("dosshell");
	exec_name_list.push_back("drivparm");
	exec_name_list.push_back("echo");
	exec_name_list.push_back("edit");
	exec_name_list.push_back("edlin");
	exec_name_list.push_back("emm386");
	exec_name_list.push_back("enable");
	exec_name_list.push_back("endlocal");
	exec_name_list.push_back("erase");
	exec_name_list.push_back("exit");
	exec_name_list.push_back("expand");
	exec_name_list.push_back("extract");
	exec_name_list.push_back("fasthelp");
	exec_name_list.push_back("fc");
	exec_name_list.push_back("fdisk");
	exec_name_list.push_back("find");
	exec_name_list.push_back("findstr");
	exec_name_list.push_back("fixboot");
	exec_name_list.push_back("fixmbr");
	exec_name_list.push_back("for");
	exec_name_list.push_back("forfiles");
	exec_name_list.push_back("format");
	exec_name_list.push_back("ftp");
	exec_name_list.push_back("fType");
	exec_name_list.push_back("goto");
	exec_name_list.push_back("gpupdate");
	exec_name_list.push_back("graftabl");
	exec_name_list.push_back("help");
	exec_name_list.push_back("himem");
	exec_name_list.push_back("sys");
	exec_name_list.push_back("hostname");
	exec_name_list.push_back("if");
	exec_name_list.push_back("ifshlp");
	exec_name_list.push_back("sys");
	exec_name_list.push_back("ipconfig");
	exec_name_list.push_back("keyb");
	exec_name_list.push_back("label");
	exec_name_list.push_back("lh");
	exec_name_list.push_back("listsvc");
	exec_name_list.push_back("loadfix");
	exec_name_list.push_back("loadhigh");
	exec_name_list.push_back("lock");
	exec_name_list.push_back("logoff");
	exec_name_list.push_back("logon");
	exec_name_list.push_back("map");
	exec_name_list.push_back("md");
	exec_name_list.push_back("mem");
	exec_name_list.push_back("mkdir");
	exec_name_list.push_back("mode");
	exec_name_list.push_back("more");
	exec_name_list.push_back("move");
	exec_name_list.push_back("msav");
	exec_name_list.push_back("msbackup");
	exec_name_list.push_back("msd");
	exec_name_list.push_back("mscdex");
	exec_name_list.push_back("mwbackup");
	exec_name_list.push_back("nbtstat");
	exec_name_list.push_back("net");
	exec_name_list.push_back("netsh");
	exec_name_list.push_back("netstat");
	exec_name_list.push_back("nlsfunc");
	exec_name_list.push_back("nslookup");
	exec_name_list.push_back("path");
	exec_name_list.push_back("pathping");
	exec_name_list.push_back("pause");
	exec_name_list.push_back("ping");
	exec_name_list.push_back("popd");
	exec_name_list.push_back("power");
	exec_name_list.push_back("print");
	exec_name_list.push_back("prompt");
	exec_name_list.push_back("pushd");
	exec_name_list.push_back("qbasic");
	exec_name_list.push_back("rd");
	exec_name_list.push_back("reg");
	exec_name_list.push_back("ren");
	exec_name_list.push_back("rename");
	exec_name_list.push_back("rmdir");
	exec_name_list.push_back("robocopy");
	exec_name_list.push_back("route");
	exec_name_list.push_back("runas");
	exec_name_list.push_back("scandisk");
	exec_name_list.push_back("scanreg");
	exec_name_list.push_back("set");
	exec_name_list.push_back("setlocal");
	exec_name_list.push_back("setver");
	exec_name_list.push_back("sfc");
	exec_name_list.push_back("share");
	exec_name_list.push_back("shift");
	exec_name_list.push_back("shutdown");
	exec_name_list.push_back("smartdrv");
	exec_name_list.push_back("sort");
	exec_name_list.push_back("start");
	exec_name_list.push_back("subst");
	exec_name_list.push_back("switches");
	exec_name_list.push_back("sys");
	exec_name_list.push_back("systeminfo");
	exec_name_list.push_back("systemroot");
	exec_name_list.push_back("taskkill");
	exec_name_list.push_back("tasklist");
	exec_name_list.push_back("telnet");
	exec_name_list.push_back("time");
	exec_name_list.push_back("title");
	exec_name_list.push_back("tracert");
	exec_name_list.push_back("tree");
	exec_name_list.push_back("tskill");
	exec_name_list.push_back("Type");
	exec_name_list.push_back("undelete");
	exec_name_list.push_back("unformat");
	exec_name_list.push_back("unlock");
	exec_name_list.push_back("ver");
	exec_name_list.push_back("verify");
	exec_name_list.push_back("vol");
	exec_name_list.push_back("wmic");
	exec_name_list.push_back("xcopy");
	
	//arithmetic operations in .bat
	cmplx_calc_list.push_back("%");
	cmplx_calc_list.push_back("^");
	cmplx_calc_list.push_back("+");
	cmplx_calc_list.push_back("-");
	cmplx_calc_list.push_back("/");
	cmplx_calc_list.push_back("*");
	cmplx_calc_list.push_back("<<");
	cmplx_calc_list.push_back(">>");
	cmplx_calc_list.push_back("&");
	cmplx_calc_list.push_back("|");

	//complex commands in .bat
	cmplx_cond_list.push_back("for");
	cmplx_cond_list.push_back("if");
	cmplx_cond_list.push_back("choice");

	//logical operations in .bat
	cmplx_logic_list.push_back("&&");
	cmplx_logic_list.push_back("||");
	cmplx_logic_list.push_back("==");
	cmplx_logic_list.push_back("equ");
	cmplx_logic_list.push_back("neq");
	cmplx_logic_list.push_back("lss");
	cmplx_logic_list.push_back("leq");
	cmplx_logic_list.push_back("gtr");
	cmplx_logic_list.push_back("geq");

	cmplx_assign_list.push_back("=");

	// cyclometic complexity
	cmplx_cyclomatic_list.push_back("if");
	cmplx_cyclomatic_list.push_back("while");
	cmplx_cyclomatic_list.push_back("for");
	cmplx_cyclomatic_list.push_back("unless");

	cmplx_cyclomatic_logic_list.push_back("&&");
	cmplx_cyclomatic_logic_list.push_back("||");

	//there is no keyword "case" in DOS_Batch, so no CC3.
}

/*!
* Perform preprocessing of file lines before counting.
*
* \param fmap list of file lines
*
* \return method status
*/
int CBatchCounter::PreCountProcess(filemap* fmap)
{
	filemap::iterator fit;
	for (fit = fmap->begin(); fit != fmap->end(); fit++) 
	{
		if (fit->line.empty())
			continue;
		for (size_t i = fit->line.length() - 1; i > 0; i--)
		{
			// to lowercase
			fit->line[i]   = (char)lowerChars[ TL_ARR( fit->line[i] ) ];
			fit->line[i-1] = (char)lowerChars[ TL_ARR( fit->line[i-1] ) ];

			// replace with space
			if (fit->line[i] == '(' || fit->line[i] == ')' || fit->line[i] == '\'' || fit->line[i] == '.' || fit->line[i] == '@') {
				fit->line[i] = ' ';
			}
			if (fit->line[i-1] ==')' || fit->line[i-1] =='(' || fit->line[i-1] == '\'' || fit->line[i-1] == '.' || fit->line[i-1] == '@') {
				fit->line[i-1] = ' ';
			}
		}
		if (CUtil::CheckBlank(fit->line))
			fit->line = "$";
		
		//ignore words after keyword "echo"
		/*int idx = CUtil::FindKeyword(fit->line, "echo");*/
		size_t idx = CUtil::FindKeyword(fit->line, "echo"); // warning fix
		if(idx != string::npos) {
			for (size_t i = idx + 4; i < fit->line.length(); i++)
			{
				fit->line[i] = '$';
			}
		}
	}
	return 0;
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
int CBatchCounter::LanguageSpecificProcess(filemap* fmap, results* result, filemap* fmapBak)
{
	filemap::iterator fit, fitbak;
	string line, lineBak;

	bool data_continue = false;
	string strLSLOC = "";
	string strLSLOCBak = "";
	string str;
	unsigned int phys_exec_lines = 0;
	unsigned int phys_data_lines = 0;
	unsigned int temp_lines = 0;
	unsigned int cnt = 0; 
	//StringVector loopLevel; warning fix
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$";

	for (fit = fmap->begin(), fitbak = fmapBak->begin(); fit != fmap->end(); fit++, fitbak++)
	{
		line = fit->line;
		lineBak = fitbak->line;

		// do not process blank lines (blank_line/comment_line/directive)
		if (!CUtil::CheckBlank(line))
		{
			// process logical SLOC
			LSLOC(result, line, fit->lineNumber, lineBak, strLSLOC, strLSLOCBak, data_continue,
				temp_lines, phys_exec_lines, phys_data_lines/*, loopLevel*/); //warning fix

			if (print_cmplx)
			{
				cnt = 0;
				CUtil::CountTally(line, exec_name_list, cnt, 1, exclude, "", "", &result->exec_name_count);
			}

			// update physical SLOC lines
			result->exec_lines[PHY] += phys_exec_lines;
			phys_exec_lines = 0;

			result->data_lines[PHY] += phys_data_lines;
			phys_data_lines = 0;
		}
	}
	return 1;
}

void CBatchCounter::LSLOC(results* result, string line, size_t lineNumber, string lineBak, string &strLSLOC, string &strLSLOCBak,
						 bool &data_continue, unsigned int &temp_lines, unsigned int &phys_exec_lines,
						 unsigned int &phys_data_lines/*, StringVector &loopLevel*/)// warning fix
{
	size_t start, end;
	size_t strSize;
	bool trunc_flag = false;
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$";
	string str, spc;
	unsigned int cnt = 0;

	string tmp    = CUtil::TrimString(line);
	string tmpBak = CUtil::TrimString(lineBak);
	start = 0;
	end = tmp.length();

	// skip whole line '(' or ')'
	if (tmp == "(" || tmp == ")")
	{
		strLSLOC = strLSLOCBak = "";
		phys_exec_lines++;
		temp_lines = 0;
		return;
	}

	// there may be more than 1 logical SLOC in this line
#ifdef	_MSC_VER
	char * context = NULL;		// used for strtok_s( )  below
#else
	//
#endif

	bool has_for = false;
	bool has_for_in = false;
	while (start < tmp.length())
	{
        	end = tmp.length();
        	if (start == 0) {
            		has_for = false;
            		has_for_in = false;
        	}
        	//std::cout << start <<"start\n";

        	// check if is a label
        	bool isLabel = false;
        	for (unsigned int i = 0; tmp[i] != '\0'; i++) {
			if (tmp[i] == ' ') {
                		continue;
            		} else if (tmp[i] == ':') {
                	isLabel = true;
                	break;
            		} else {
                		break;
            		}
		}
		if(isLabel) {
			strLSLOC = strLSLOCBak = "";
			phys_exec_lines++;
			temp_lines = 0;
			return;
		} 


		char * token = NULL;

		// Allocate a char array that must be deleted later.
		char* line = new char[end - start + 1];

	#ifdef _MSC_VER
		// Use more Secure C library API
		strcpy_s( line, (end - start + 1) * sizeof(char), tmp.substr(start).c_str() );
	#else
		// Use older less Secure C library API
		strcpy( line, tmp.substr(start).c_str() );
	#endif

		// check |, &
		bool hasPipe = false;
		size_t pipePos = 0;
		bool hasAmp = false;
		size_t ampPos = 0;

		for (int i = 0; line[i] != '\0'; i++) {
			if (line[i] == '|') {
		        	if (!hasPipe) {
		            		pipePos = (size_t)i;
		       		}
		        	hasPipe = true;
		        	line[i] = ' ';
		    	}
		    	if (line[i] == '&') {
		        	if (!hasAmp) {
		            		ampPos = (size_t)i;
		        	}
		        	hasAmp = true;
		        	line[i] = ' ';
		    	}
		}
		
	#ifdef _MSC_VER
		// Use more Secure C library API
		token = strtok_s( line,  " .@:", &context );
	#else
		// Use older less Secure C library API
		token = strtok( line, " .@:" );
	#endif

		for (int i = 0; token[i] != '\0'; i++) {
			token[i] = (char)lowerChars[ TL_ARR( token[i] ) ];
		}
		if (strcmp(token, "else") == 0) 
		{
		#ifdef _MSC_VER
			// Use more Secure C library API
			token = strtok_s( NULL,  " .@:", &context );
		#else
			// Use older less Secure C library API
			token = strtok( NULL, " .@:" );
		#endif
		}

		if (token == NULL) {
			strLSLOC = strLSLOCBak = "";
			phys_exec_lines++;
			temp_lines = 0;
			delete[] line;
			return;
		}
		
		//first keyword
		//std::cout << token << "TOKEN\n";
		string firstWord(token);    
		firstWord = CUtil::ToLower(firstWord);
		bool checkFirstWord = false;
		for(std::vector<string>::iterator it = exec_name_list.begin(); it != exec_name_list.end(); ++it) {
			if (*it == firstWord) {
				checkFirstWord = true;
				break;
			}
		}
		if(!checkFirstWord) {
			strLSLOC = strLSLOCBak = "";
			phys_exec_lines++;
			temp_lines = 0;
			delete[] line;
			return;
		}

		

		//if, for, choice as first word
		bool isComplexWord = false;
		bool isCall = false;
		size_t nextKeyWordStart = (size_t)(-1);
		bool isKeyWord = false;
		bool hasElse = false;
		char * nextToken = NULL;
		size_t tmpNextKeyWordStart = (size_t)(-1);
		size_t tmpEnd = (size_t)(-1);

		for(std::vector<string>::iterator it = cmplx_cond_list.begin(); it != cmplx_cond_list.end(); ++it) {
			if (*it == firstWord) {
				isComplexWord = true;
				break;
			}
		}

		//for as first word
		if (firstWord.compare("for") == 0) {
			has_for = true;
		}
		
		
		//call as first word
		if (firstWord.compare("call") == 0) {
			isCall = true;
		}


		// go through the rest of words
	#ifdef _MSC_VER
		// Use more Secure C library API
		nextToken = strtok_s( NULL,  " .@:", &context );
	#else
		// Use older less Secure C library API
		nextToken = strtok( NULL, " .@:" );
	#endif

		while (nextToken != NULL) 
		{
			string nextWord(nextToken);
			nextWord = CUtil::ToLower(nextWord);

			if (has_for && nextWord.compare("in") == 0) 
		        	has_for_in = true;

			if (nextWord.compare("else") == 0)
				hasElse = true;

			if (!isKeyWord) 
			{
				for(std::vector<string>::iterator it2 = exec_name_list.begin(); it2 != exec_name_list.end(); ++it2) 
				{
					if (*it2 == nextWord) 
					{
						isKeyWord = true;
						tmpNextKeyWordStart = CUtil::FindKeyword(CUtil::ToLower(tmp), nextWord, start + firstWord.length(), end);
						//std::cout << tmpNextKeyWordStart << "NEXT_START\n";
						tmpEnd = tmpNextKeyWordStart; // !!!!!
						//std::cout << tmpEnd << "CURR_END\n";

						break;
					}
				}
			}
		#ifdef _MSC_VER
			// Use more Secure C library API
			nextToken = strtok_s( NULL,  " .@:", &context );
		#else
			// Use older less Secure C library API
			nextToken = strtok( NULL, " .@:" );
		#endif
		}

		if (isKeyWord) 
		{
			//std::cout << pipePos << "pipePos\n";
			if (hasPipe && pipePos >= tmpEnd) 
				hasPipe = false;

			//std::cout << ampPos << "ampPos\n";
			if (hasAmp && ampPos >= tmpEnd)
				hasAmp = false;
		} 
		else 
		{
			hasPipe = false;
			hasAmp = false;
		}

		if ((isComplexWord || isCall || hasElse || has_for_in || hasPipe || hasAmp) && isKeyWord) {  // REMEMBER TO CHANGE THE SAME CODE BELOW
		    	nextKeyWordStart = tmpNextKeyWordStart;
		    	end = tmpEnd;
		}
		

		if (tmp[0] != '^') {
		    	//std::cout << "2\n";
			strSize = CUtil::TruncateLine(end - start + 1, strLSLOC.length(), this->lsloc_truncate, trunc_flag);
			if (strSize > 0) {
				strLSLOC += CUtil::TrimString(tmp.substr(start, strSize));
				strLSLOCBak += CUtil::TrimString(tmpBak.substr(start, strSize));
			}
			start = end + 1;
		    	if ((isComplexWord || isCall || hasElse || has_for_in || hasPipe || hasAmp) && isKeyWord) { // REMEMBER TO CHANGE THE SAME CODE ABOVE
		        	start = nextKeyWordStart;
		    	}
			if (strLSLOCBak.length() > 0) { //add physical SLOC
				if (result->addSLOC(strLSLOCBak, lineNumber, trunc_flag)) { //add a logical SLOC
					cnt = 0;
					CUtil::CountTally(strLSLOC, data_name_list, cnt, 1, exclude, "", "", &result->data_name_count);

		            		temp_lines++;
					if (data_continue == true || cnt > 0) {
						result->data_lines[LOG]++;
						phys_data_lines = temp_lines;
					}
					else {
						result->exec_lines[LOG]++;
						phys_exec_lines = temp_lines;
					}
				}
				else if (data_continue == true)
					phys_data_lines = temp_lines;
				else
					phys_exec_lines = temp_lines;
			}
			data_continue = false;
			temp_lines = 0;
			strLSLOC = strLSLOCBak = "";
		}

		delete[] line;
		line = NULL;
	}
}

/*!
 * Parses lines for function/method names.
 *
 * \param line line to be processed
 * \param lastline last line processed
 * \param functionStack stack of functions
 * \param functionName function name found
 *
 * \return 1 if function name is found
 */
int CBatchCounter::ParseFunctionName(const string &line, string &lastline,
		filemap &functionStack, string &functionName, unsigned int &functionCount)
{
	/*
	 * functionStack is used to record block names encountered, it will be poped when its closing "}" is encountered
	 * cyclomatic_stack is used to record the previous cyc_count when start of block is encountered, it will be poped
	 * 		when end of block is encountered and poped value will be added to the current cyc_cnt
	 */

	string tline, str;
    //size_t i, idx, tidx, cnt;
    size_t idx; // warning fix

    tline = CUtil::TrimString(line, 0);
	// Look for label
	idx = tline.find(':');
	if(idx == 0) {
		// check if there is any function in the stack that has noot been closed
		if(!functionStack.empty()) {
			// pop start of the block from functionStack
			functionName = functionStack.back().line;
			functionCount = functionStack.back().lineNumber;
			functionStack.pop_back();
			// push current function into stack
			lineElement element(++functionCount, CUtil::ClearRedundantSpaces(tline));
			functionStack.push_back(element);
			lastline.erase();

			return 1;
		}

		str = CUtil::ClearRedundantSpaces(tline);
		lineElement element(++functionCount, str);
		functionStack.push_back(element);
		lastline.erase();
	}

	// Look for end of function
	idx = CUtil::FindKeyword(tline, "goto");
	if(idx != string::npos) {
		str = CUtil::TrimString(tline.substr(idx+4));
		idx = CUtil::FindKeyword(str, ":eof");
	}
	if(idx == 0 && !functionStack.empty()) {
		functionName = functionStack.back().line;
		functionCount = functionStack.back().lineNumber;
		functionStack.pop_back();
		lastline.erase();
		return 1;
	}

	// update lastline
	lastline = tline;

	// code that belong to "main" block
	if(functionStack.empty()) {
		return 2;
	}

	return 0;

}

/*!
* Counts file language complexity based on specified language keywords/characters.
*
* \param fmap list of processed file lines
* \param result counter results
*
* \return method status
*/
int CBatchCounter::CountComplexity(filemap* fmap, results* result)
{
	if (classtype == UNKNOWN || classtype == DATAFILE)
		return 0;
	filemap::iterator fit;
	size_t idx;
	unsigned int cnt, ret, cyclomatic_cnt = 0, ignore_cyclomatic_cnt = 0, main_cyclomatic_cnt = 0, function_count = 0, cyclomatic_logic_cnt = 0, main_cyclomatic_logic_cnt = 1;
	string line, lastline, file_ext, function_name = "";
	string exclude = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_$";
	filemap function_stack;
	stack<unsigned int> cyclomatic_stack;
	stack<unsigned int> cyclomatic_logic_stack;
	map<unsigned int, lineElement> function_map;
	map<unsigned int, lineElement> logical_map;
	bool process_cyclomatic_complexity = false;
	// check whether to process cyclomatic complexity
	if (cmplx_cyclomatic_list.size() > 0)
	{
		process_cyclomatic_complexity = true;
		if (skip_cmplx_cyclomatic_file_extension_list.size() > 0)
		{
			idx = result->file_name.find_last_of(".");
			if (idx != string::npos)
			{
				file_ext = result->file_name.substr(idx);
				file_ext = CUtil::ToLower(file_ext);
				if (find(skip_cmplx_cyclomatic_file_extension_list.begin(), skip_cmplx_cyclomatic_file_extension_list.end(), file_ext) != skip_cmplx_cyclomatic_file_extension_list.end())
					process_cyclomatic_complexity = false;
			}
		}
	}

	// process each line
	for (fit = fmap->begin(); fit != fmap->end(); fit++)
	{
		line = fit->line;

		if (CUtil::CheckBlank(line))
			continue;

		line = " " + line;

		// mathematical functions
		cnt = 0;
		CUtil::CountTally(line, math_func_list, cnt, 1, exclude, "", "", &result->math_func_count, casesensitive);
		result->cmplx_math_lines += cnt;

		// trigonometric functions
		cnt = 0;
		CUtil::CountTally(line, trig_func_list, cnt, 1, exclude, "", "", &result->trig_func_count, casesensitive);
		result->cmplx_trig_lines += cnt;

		// logarithmic functions
		cnt = 0;
		CUtil::CountTally(line, log_func_list, cnt, 1, exclude, "", "", &result->log_func_count, casesensitive);
		result->cmplx_logarithm_lines += cnt;

		// calculations
		cnt = 0;
		CUtil::CountTally(line, cmplx_calc_list, cnt, 1, exclude, "", "", &result->cmplx_calc_count, casesensitive);
		result->cmplx_calc_lines += cnt;

		// conditionals
		cnt = 0;
		CUtil::CountTally(line, cmplx_cond_list, cnt, 1, exclude, "", "", &result->cmplx_cond_count, casesensitive);
		result->cmplx_cond_lines += cnt;

		// logical operators
		cnt = 0;
		CUtil::CountTally(line, cmplx_logic_list, cnt, 1, exclude, "", "", &result->cmplx_logic_count, casesensitive);
		result->cmplx_logic_lines += cnt;

		// preprocessor directives
		cnt = 0;
		CUtil::CountTally(line, cmplx_preproc_list, cnt, 1, exclude, "", "", &result->cmplx_preproc_count, casesensitive);
		result->cmplx_preproc_lines += cnt;

		// assignments
		cnt = 0;
		CUtil::CountTally(line, cmplx_assign_list, cnt, 1, exclude, "", "", &result->cmplx_assign_count, casesensitive);
		result->cmplx_assign_lines += cnt;

	/* No pointers for DOS batch commands
		// pointers
		cnt = 0;
		// Pointers are embedded syntax so there is NO exclude string or include strings
		CUtil::CountTally(line, cmplx_pointer_list, cnt, 1, "", "", "", &result->cmplx_pointer_count, casesensitive);
		result->cmplx_pointer_lines += cnt;
	*/

		// cyclomatic complexity
		if (process_cyclomatic_complexity)
		{
			// search for cyclomatic complexity keywords
			CUtil::CountTally(line, cmplx_cyclomatic_list, cyclomatic_cnt, 1, exclude, "", "", 0, casesensitive);

			// search for keywords to exclude
			if (ignore_cmplx_cyclomatic_list.size() > 0)
				CUtil::CountTally(line, ignore_cmplx_cyclomatic_list, ignore_cyclomatic_cnt, 1, exclude, "", "", 0, casesensitive);

			// search for cyclomatic complexity logical keywords
			if (cmplx_cyclomatic_logic_list.size() > 0)
				CUtil::CountTally(line, cmplx_cyclomatic_logic_list, cyclomatic_logic_cnt, 1, exclude, "", "", 0, casesensitive);

			// parse function name if found
			ret = (unsigned int)ParseFunctionName( line, lastline, function_stack, function_name, function_count );
			if (ret != 1 && !cyclomatic_stack.empty() && cyclomatic_stack.size() == function_stack.size())
			{
				// remove count stack entry for non-function names
				cyclomatic_cnt += cyclomatic_stack.top();
				ignore_cyclomatic_cnt = 0;
				cyclomatic_stack.pop();
			}
			if (ret != 1 && !cyclomatic_logic_stack.empty() && cyclomatic_logic_stack.size() == function_stack.size())
			{
				// remove count stack entry for non-function names
				cyclomatic_logic_cnt += cyclomatic_logic_stack.top();
				cyclomatic_logic_stack.pop();
			}
			if (ret == 1)
			{
				// capture count at end of function
				lineElement element(cyclomatic_cnt - ignore_cyclomatic_cnt + 1, function_name);
				function_map[function_count] = element;
				
				lineElement n_element(cyclomatic_cnt - ignore_cyclomatic_cnt + cyclomatic_logic_cnt + 1, function_name);
				logical_map[function_count] = n_element;

				//there is no embedded functions in DOS_Batch
				cyclomatic_cnt = 0;
				cyclomatic_logic_cnt = 0;
				function_name = "";
				ignore_cyclomatic_cnt = 0;
			}
			else if (ret == 2)
			{
				if (main_cyclomatic_cnt < 1)
					main_cyclomatic_cnt = 1;	// add 1 for main function here in case no other decision points are found in main
				// some code doesn't belong to any function
				main_cyclomatic_cnt += cyclomatic_cnt - ignore_cyclomatic_cnt;
				main_cyclomatic_logic_cnt += cyclomatic_cnt - ignore_cyclomatic_cnt + cyclomatic_logic_cnt;
				cyclomatic_cnt = ignore_cyclomatic_cnt = cyclomatic_logic_cnt = 0;
			}
			else { 
				if (!function_stack.empty() && (function_stack.size() > cyclomatic_stack.size() + 1 || (cyclomatic_stack.empty() && function_stack.size() > 1)))
				{
					// capture previous complexity count from open function
					cyclomatic_stack.push(cyclomatic_cnt - ignore_cyclomatic_cnt);
					cyclomatic_cnt = ignore_cyclomatic_cnt = 0;
				}
				if (!function_stack.empty() && (function_stack.size() > cyclomatic_logic_stack.size() + 1 || (cyclomatic_logic_stack.empty() && function_stack.size() > 1)))
				{
					// capture previous complexity count from open function
					cyclomatic_logic_stack.push(cyclomatic_logic_cnt);
					cyclomatic_logic_cnt = 0;
				}
			}
		}
	}

	//process the last function if it exists
	if (!function_stack.empty())
	{
		function_name = function_stack.back().line;
		function_count = function_stack.back().lineNumber + 1;
		function_stack.pop_back();

		lineElement element(cyclomatic_cnt + 1, function_name);
		lineElement n_element(cyclomatic_cnt + cyclomatic_logic_cnt + 1, function_name);
		function_map[function_count] = element;
		logical_map[function_count] = n_element;	
	}

	// done with a file
	if (main_cyclomatic_cnt > 0)
	{
		// add "main" code
		lineElement element(main_cyclomatic_cnt, "main");
		lineElement n_element(main_cyclomatic_logic_cnt, "main");
		function_map[0] = element;
		logical_map[0] = n_element;
	}

	// process ordered functions
	for (map<unsigned int, lineElement>::iterator it = function_map.begin(); it != function_map.end(); ++it)
		result->cmplx_cycfunct_count.push_back(it->second);
	if(cmplx_cyclomatic_logic_list.size() > 0)
	{
		for (map<unsigned int, lineElement>::iterator it = logical_map.begin(); it != logical_map.end(); ++it)
			result->cmplx_cycfunct_CC2_count.push_back(it->second);
	}

	return 1;
}

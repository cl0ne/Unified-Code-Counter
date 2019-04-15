//! Code counter class definition for the ColdFusion language.
/*!
* \file CColdFusionCounter.h
*
* This file contains the code counter class definition for the ColdFusion language.
*/

#ifndef CColdFusionCounter_h
#define CColdFusionCounter_h

#include "CTagCounter.h"

//! ColdFusion code counter class.
/*!
* \class CColdFusionCounter
*
* Defines the ColdFusion code counter class.
*/
class CColdFusionCounter : public CTagCounter
{
public:
	CColdFusionCounter();
protected:
	virtual int PreCountProcess(filemap* fmap);
	int ParseFunctionName(const string &line, string &lastline, filemap &functionStack, string &functionName, 
		unsigned int &functionCount);

private:
// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'CColdFusionCounter' : copy constructor could not be generated because a base class copy constructor is inaccessible
	CColdFusionCounter(const CColdFusionCounter& rhs);          // Declare without implementation

	// Take care of warning C4626: 'CColdFusionCounter' : assignment operator could not be generated because a base class assignment operator is inaccessible
	CColdFusionCounter operator=(const CColdFusionCounter);    // Declare without implementation
};

#endif

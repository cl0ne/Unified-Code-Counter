#ifndef USERIF_H
#define USERIF_H

//! User interface class definition.
/*!
* \file UserIF.h
*
* This file contains the user interface class definition.
*/

#include "UCCBeforeLibraryIncludes.h"    // Modification: 2015.12
#include <iostream>
#include <fstream>
#include <string>
#include "UCCAfterLibraryIncludes.h"     // Modification: 2015.12

using namespace std;

// Declare in Global scope so utility code can access without circular #includes
extern	bool execCanceled;			//!< Flag indicating canceled execution

#ifndef QTGUI

//! Console user interface class.
/*!
* \class UserIF
*
* Defines a console user interface class.
*/
class UserIF
{
public:
	UserIF();
	~UserIF(){}

	void SetErrorFile(const string &outDir);
	void AddError(const string &err, bool logOnly = false, int preNL = 0, int postNL = 1);
	
	// Update console or GUI with ongoing completion status or show a message.
	// Messages with "Warning:" may not be shown if User wants.
	// When pct >= 0 this will always overwrite last 4 characters.
	bool updateProgress( const string &msg, const bool postNL = true,
						const int pct = -1, const unsigned int progressTotal = 0 );
	bool isCanceled();

private:
	ofstream errFile;			//!< Stream for logging errors
	string errPath;				//!< Error log file path
	bool execCanceled;			//!< Flag indicating canceled execution

// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'UserIF' : copy constructor could not be generated because a base class copy constructor is inaccessible
	UserIF(const UserIF& rhs);          // Declare without implementation

	// Take care of warning C4626: 'UserIF' : assignment operator could not be generated because a base class assignment operator is inaccessible
	UserIF operator=(const UserIF);    // Declare without implementation
};

#else

#include <QWidget>

//! Graphical user interface class.
/*!
* \class UserIF
*
* Defines a graphical user interface class.
*/
class UserIF : public QObject
{
	Q_OBJECT

public:
	UserIF(QWidget *parent = 0);
	~UserIF(){}

	void SetErrorFile(const string &outDir);
	void AddError(const string &err, bool logOnly = false, int preNL = 0, int postNL = 1);

	// Update console or GUI with ongoing completion status or show a message.
	// Messages with "Warning:" may not be shown if User wants.
	// When pct >= 0 this will always overwrite last 4 characters.
	bool updateProgress( const string &msg, const bool postNL = true,
						const int pct = -1, const unsigned int progressTotal = 0 );

	bool isCanceled();

signals:
	void updatedLog(const QString &err);
	void updatedProgress(const QString &msg, int pct);

	public slots:
		void cancelExecution();

private:
	ofstream errFile;			//!< Stream for logging errors
	string errPath;				//!< Error log file path

// This class is NOT copied or assigned to.
// Avoid copying of this class.  Avoid assignment of this class.
// Compiler will give an Error if a copy or assignment is done and those Errors do NOT happen.
// This avoids a VC++ -W4 or -Wall warning C4625, C4626

	// Take care of warning C4625: 'UserIF' : copy constructor could not be generated because a base class copy constructor is inaccessible
	UserIF(const UserIF& rhs);          // Declare without implementation

	// Take care of warning C4626: 'UserIF' : assignment operator could not be generated because a base class assignment operator is inaccessible
	UserIF operator=(const UserIF);    // Declare without implementation
};

#endif

#endif

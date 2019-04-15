//! User interface class methods.
/*!
* \file UserIF.cpp
*
* This file contains the user interface class methods.
*/

#include "UCCBeforeLibraryIncludes.h"    // Modification: 2015.12
#include <time.h>
#include "UCCAfterLibraryIncludes.h"     // Modification: 2015.12

#include "UCCGlobals.h"                  // Modification: 2015.12
#include "UserIF.h"

// Declare in Global scope so utility code can access without circular #includes
bool execCanceled = false;			//!< Flag indicating canceled execution

//
//		LOCAL	PROTOTYPES
//
void  UpdateErrorCount( const string & err );

/*!
*   Keep counts of Errors/Warnings/Information messages updated
*/
void  UpdateErrorCount( const string & err )
{
// See if an Error or Warning was logged to be User friendly later
	bool	didWarning = false;
	bool	didInfo    = false;
	if ( err.find( "Information:" ) != string::npos )
	{
		// Found a Warning.
		didInfo = true;
	}

	if ( err.find( "Warning:" ) != string::npos )
	{
		// Found a Warning.
		didWarning = true;
		didInfo = false;
	}

	if ( ( err.find( "Error:" ) != string::npos )
		|| ( err.find( "ERROR:" ) != string::npos ) )
	{
		// At least 1 Error message so do not count as Warning or Info
		didWarning = false;
		didInfo = false;

		extern	unsigned int	errors_count;
		errors_count++;
	}
	else
	if ( didWarning )
	{
		extern	unsigned int	warnings_count;
		warnings_count++;
	}
	else
	if ( didInfo )
	{
		extern	unsigned int	information_count;
		information_count++;
	}
}


#ifndef QTGUI

/*!
* Constructs a UserIF object.
*/
UserIF::UserIF()
{
	errPath = "";
	execCanceled = false;
}

/*!
* Sets the execution error file.
*
* \param outDir output directory
*/
void UserIF::SetErrorFile(const string &outDir)
{
	time_t myTime;
	struct tm *myLocalTime;
	time(&myTime);
#if defined UNIX || defined MINGW
	myLocalTime = localtime(&myTime);
#else
	struct tm myLT;
	localtime_s(&myLT, &myTime);
	myLocalTime = &myLT;
#endif
	char s[50];
	strftime(s, 50, "error_log_%m%d%Y_%I%M%S.txt", myLocalTime);
	if (outDir.empty())
		errPath = s;
	else
		errPath = outDir + s;
}

/*!
* Adds a message entry to the error log.
*
* \param err error string
* \param logOnly flag to only write to log (no user output)
* \param preNL number of preceding new lines in user output (default=0)
* \param postNL number of proceeding new lines in user output (default=1)
*/
void UserIF::AddError(const string &err, bool logOnly, int preNL, int postNL)
{
	if (err.empty() || errPath.empty())
		return;

	// open the error file if not already opened
	if (!errFile.is_open())
	{
		errFile.open(errPath.c_str(), ofstream::out);
		if (!errFile.is_open())
		{
			printf( "Error: Failed to open error log file; %s\n", errPath.c_str() );
			return;
		}
	}
	errFile << err << endl;
	UpdateErrorCount( err );

	if (!logOnly)
	{
		for (int i = 0; i < preNL; i++)
			cout << endl;
		cout << err;
		for (int i = 0; i < postNL; i++)
			cout << endl;
		cout << flush;
	}
}

/*!
1. Function Description: 
* Updates progress by displaying a message or showing percent completion.
* Warning messages are only shown if wanted by User.
* return true if a message was actually shown.
*
* 2. Parameters:
* \Global	no_warnings_to_stdout	IN	true will not show a message containing "Warning:"
* \param    msg            IN  message string
* \param    postNL         IN  include a new line (default = true)
* \param    pct            IN  percent completion -1 if none, else shows 4 chars with possible leading dots
* \param    progressTotal  IN  progress total  IF 1 will output 100%
* \return   true           if message was shown
*
* 3. Creation Time and Owner:
*
* Rewritten to fix a defect, make calling code simpler and support Threads // Modification: 2015.12
*/
// Declare the function we use here without having to include the other Thread interfaces
extern void	IntToStr( const int val, string & result );

bool UserIF::updateProgress( const string &msg, const bool postNL,
	const int pct, 
	const unsigned int progressTotal )	// should be changed to const unsigned long to allow more items
{
	bool	msg_shown = false;

	if ( pct > 100 )
	{
		msg_shown = true;
	}
	msg_shown = false;

	if ( pct >= 0 )
	{
		// Assumption is that showing Percent done will not be a warning
		if ( !msg.empty() )
		{
			cout << msg << flush;
			msg_shown = true;
		}

		// Changed below to only change the LAST 4 positions
		// And this will always change those 4
		// Always overwrite last 4 positions 
		// so Caller must set up with a little extra for the first time
		string	tmp = "\b\b\b\b";	// Back up to overwrite previous % shown

		if (progressTotal == 1)	// check for special case
			tmp += "100%";
		else
		{
			if (pct < 10)
				tmp += "..";
			else if (pct < 100)
				tmp += ".";
			string tmp2;
			IntToStr( pct, tmp2 );
			tmp += tmp2 + "%";
		}
		cout << tmp;
		cout.flush();
		msg_shown = true;
	}
	else
	{
		bool	show_msg = true;
		if ( !msg.empty() )
		{
			if ( g_no_warnings_to_UI )
			{
				if ( msg.find( "Warning:" ) != string::npos )
					show_msg = false;
			}
		}
		else
			show_msg = false;

		if ( show_msg )
		{
			if ( !postNL )
				cout << msg << flush;
			else
				cout << msg << endl;
			msg_shown = true;
		}
	}
	return	msg_shown;
}

/*!
* Checks whether execution has been canceled by the user.
*
* \return execution canceled?
*/
bool UserIF::isCanceled()
{
	return(execCanceled);
}

#else

#include <QApplication>

/*!
* Constructs a UserIF object.
*
* \parent parent widget
*/
UserIF::UserIF(QWidget *parent)
	: QObject(parent)
{
	errPath = "";
	execCanceled = false;

	if (parent)
	{
		connect(this, SIGNAL(updatedLog(const QString &)), parent, SLOT(updateLog(const QString &)));
		connect(this, SIGNAL(updatedProgress(const QString &, int)), parent, SLOT(updateProgress(const QString &, int)));
		connect(parent, SIGNAL(canceledExecution()), this, SLOT(cancelExecution()));
	}
}

/*!
* Sets the execution error file.
*
* \param outDir output directory
*/
void UserIF::SetErrorFile(const string &outDir)
{
	time_t myTime;
	struct tm *myLocalTime;
	time(&myTime);
#if defined UNIX || defined MINGW
	myLocalTime = localtime(&myTime);
#else
	struct tm myLT;
	localtime_s(&myLT, &myTime);
	myLocalTime = &myLT;
#endif
	char s[50];
	strftime(s, 50, "error_log_%m%d%Y_%I%M%S.txt", myLocalTime);
	if (outDir.empty())
		errPath = s;
	else
		errPath = outDir + s;
}

/*!
* Adds a message entry to the error log.
*
* \param err error string
* \param logOnly flag to only write to log (no user output)
* \param preNL number of preceding new lines in user output (default=0)
* \param postNL number of proceeding new lines in user output (default=1)
*/
void UserIF::AddError(const string &err, bool /*logOnly*/, int preNL, int postNL)
{
	if (err.empty() || errPath.empty())
		return;

	// open the error file if not already opened
	if (!errFile.is_open())
	{
		errFile.open(errPath.c_str(), ofstream::out);
		if (!errFile.is_open())
		{
			emit updatedLog("Error: Failed to open error log file\n");
			return;
		}
	}
	errFile << err << endl;
	UpdateErrorCount( err );

	QString errStr = err.c_str();
	for (int i = 0; i < preNL; i++)
		errStr = "\n" + errStr;
	for (int i = 0; i < postNL; i++)
		errStr += "\n";
	emit updatedLog(errStr);
	QApplication::processEvents();
}


/*!
1. Function Description:
* Updates progress by displaying a message or showing percent completion.
* Warning messages are only shown if wanted by User.
* return true if a message was actually shown.
*
* 2. Parameters:
* \Global	g_no_warnings_to_UI	IN	true will not show a message containing "Warning:"
* \param    msg            IN  message string
* \param    postNL         IN  include a new line (default = true)
* \param    pct            IN  percent completion -1 if none
* \param    progressTotal  IN  progress total
* \return   true           if message was shown
*
* 3. Creation Time and Owner:
* Rewritten to make calling code simpler and support Threads // Modification: 2015.12
*/
bool UserIF::updateProgress(const string &msg, const bool /*postNL*/,
    const int pct,
    const unsigned int /*progressTotal*/)  // should be changed to const unsigned long to allow more items
{
    bool	msg_shown = false;

	if (!msg.empty() || pct >= 0)
	{
        bool	show_msg = true;
        if ( pct < 0 )
        {
            if ( !msg.empty() )
            {
                if ( g_no_warnings_to_UI )
                {
                    if ( msg.find( "Warning:" ) != string::npos )
                        show_msg = false;
                }
            }
            else
                show_msg = false;
        }

        if ( show_msg )
        {
            emit updatedProgress(QString(msg.c_str()), pct);
            QApplication::processEvents();
            msg_shown = true;
        }
	}
    return msg_shown;
}

/*!
* Cancels the running execution.
*/
void UserIF::cancelExecution()
{
	execCanceled = true;
}

/*!
* Checks whether execution has been canceled by the user.
*
* \return execution canceled?
*/
bool UserIF::isCanceled()
{
	return(execCanceled);
}

#endif

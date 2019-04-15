//! GUtil class static methods.
/*!
* \file GUtil.h
*
* This file contains the GUtil class static methods.
*/

#include <QProcess>
#include "GUtil.h"

/*!
* Opens the specified file using the default application.
*
* \param filePath file path to open
*
* \return status
*/
bool GUtil::openFile(const QString &filePath)
{
	QString appl;
	QStringList argList;

#if defined(Q_OS_MACX)
	appl = "open";
#elif defined(Q_OS_WIN)
	appl = "cmd";
	argList << "/C";
#else
	return(false);
#endif
	argList << filePath;

	// create external application process
	QProcess *proc = new QProcess(0);

	// launch process
	proc->start(appl, argList);
	if (! proc->waitForStarted())
		return(false);
	else
		return(true);
}

//! GUtil class definitions.
/*!
* \file GUtil.h
*
* This file contains the GUtil class definition.
*/

#ifndef GUTIL_H
#define GUTIL_H

#include <QString>

//! Utility class.
/*!
* \class GUtil
*
* Defines a utility class.
*/
class GUtil
{
public:
	static bool openFile(const QString &filePath);
};

#endif // GUTIL_H

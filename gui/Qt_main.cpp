//! The main GUCC function.
/*!
* \file Qt_main.cpp
*
* This file contains the main GUCC function.
*/

#include <QApplication>
#include "GMainWindow.h"

/*!
* Main function.
*
* \param argc number of arguments
* \param argv argument list
*
* \return function status
*/
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
    GMainWindow w;
    w.show();
	return a.exec();
}

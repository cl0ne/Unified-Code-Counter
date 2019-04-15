//! GFileOpenMenu class methods.
/*!
* \file GFileOpenMenu.cpp
*
* This file contains the GFileOpenMenu class methods.
*/

#include <QFileDialog>
#include "GFileOpenMenu.h"

/*!
* Constructs a GFileOpenMenu object.
*
* \param parent parent widget
* \param f window flags
*/
GFileOpenMenu::GFileOpenMenu(QWidget *parent, Qt::WindowFlags f)
	: QDialog(parent, f)
{
	ui.setupUi(this);

    ui.txtFileLines->clear();
	
	QString qs = QFileDialog::getOpenFileName(this, tr("Add a file"), "", tr("All Files (*.*)"));
	QFile file(qs);
	if (file.open(QIODevice::ReadOnly))
	{
		QString text = file.readAll();
		file.close();
		ui.txtFileLines->setText(text);
	}
}

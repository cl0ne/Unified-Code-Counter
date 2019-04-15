//! GAsciiDialog class methods.
/*!
* \file GAsciiDialog.cpp
*
* This file contains the GAsciiDialog class methods.
*/

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include "GAsciiDialog.h"

/*!
* Constructs a GAsciiDialog object.
*
* \param filePath results file or directory
* \param parent parent widget
* \param f window flags
*/
GAsciiDialog::GAsciiDialog(const QString &filePath, QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f)
{
    ui.setupUi(this);

	outputDir = "";
	ui.cboResultsSelect->clear();
	ui.txtResults->clear();

	QFont font = ui.txtResults->font();
	font.setStyleHint(QFont::Monospace);
	font.setFamily("Courier");
	ui.txtResults->setFont(font);

	if (!filePath.isEmpty())
	{
		QFileInfo fi(filePath);
		if (fi.isDir())
		{
			outputDir = filePath;
			QDir dir(filePath);
			dir.setNameFilters(QStringList() << tr("*outfile*.txt") << tr("MatchedPairs.txt") << tr("*DuplicatePairs.txt") << tr("error_log_*.txt"));
			QStringList fileList = dir.entryList();
			for (int i = 0; i < fileList.count(); i++)
				ui.cboResultsSelect->addItem(fileList.at(i));
		}
		else if (fi.isFile())
		{
			outputDir = fi.absolutePath();
			ui.cboResultsSelect->addItem(fi.fileName());
		}
	}
}

/*!
* Selects a file to display.
*
* \param fileName file name
*/
void GAsciiDialog::on_cboResultsSelect_currentIndexChanged(const QString &fileName)
{
    ui.txtResults->clear();
    QFile file(outputDir + "/" + fileName);
    if (file.open(QIODevice::ReadOnly))
	{
		QString text = file.readAll();
		file.close();
		ui.txtResults->setText(text);
	}
}

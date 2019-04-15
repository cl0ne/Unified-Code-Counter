//! GAsciiDialog class definitions.
/*!
* \file GAsciiDialog.h
*
* This file contains the GAsciiDialog class definition.
*/

#ifndef GASCIIDIALOG_H
#define GASCIIDIALOG_H

#include <QDialog>
#include "ui_GAsciiDialog.h"

//! ASCII dialog.
/*!
* \class GAsciiDialog
*
* Defines an ASCII dialog.
*/
class GAsciiDialog : public QDialog
{
    Q_OBJECT

public:
    GAsciiDialog(const QString &filePath, QWidget *parent = 0, Qt::WindowFlags f = 0);
	~GAsciiDialog(){};

private slots:
	void on_cboResultsSelect_currentIndexChanged(const QString &fileName);

private:
	Ui::GAsciiDialogClass ui;

	QString outputDir;				//!< Output directory defined by filePath
};

#endif // GASCIIDIALOG_H

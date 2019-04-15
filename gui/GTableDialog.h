//! GTableDialog class definitions.
/*!
* \file GTableDialog.h
*
* This file contains the GTableDialog class definition.
*/

#ifndef GTABLEDIALOG_H
#define GTABLEDIALOG_H

#include <QDialog>
#include "ui_GTableDialog.h"

//! Table dialog.
/*!
* \class GTableDialog
*
* Defines a table dialog.
*/
class GTableDialog : public QDialog
{
    Q_OBJECT

public:
	GTableDialog(const QString &filePath, QWidget *parent = 0, Qt::WindowFlags f = 0);
	~GTableDialog(){};

private slots:
    void on_cboResultsSelect_currentIndexChanged(const QString &fileName);

private:
	Ui::GTableDialogClass ui;

	QString outputDir;				//!< Output directory defined by filePath
};

#endif // GTABLEDIALOG_H

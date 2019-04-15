//! GExtensionDialog class definitions.
/*!
* \file GExtensionDialog.h
*
* This file contains the GExtensionDialog class definition.
*/

#ifndef GEXTENSIONDIALOG_H
#define GEXTENSIONDIALOG_H

#include <QCloseEvent>
#include <QDialog>
#include "ui_GExtensionDialog.h"
#include "ui_GMainWindow.h" //Modification: 2018.04 

//! Extension dialog.
/*!
* \class GExtensionDialog
*
* Defines an extension dialog.
*/
class GExtensionDialog : public QDialog
{
    Q_OBJECT

public:
    GExtensionDialog(QMap<QString, QStringList *> *extensionMapDefault, QMap<QString, QStringList *> *extensionMapCustom,
		QWidget *parent = 0, Qt::WindowFlags f = 0);
    ~GExtensionDialog();
	
signals:
	void updateExtensions();

private slots:
	void languageSelected(QListWidgetItem *lwItem);
	void itemCustomContextMenuRequested(const QPoint &pos);
	void revertSelLangExt();
	void revertAllLangExt();
    void on_btnAddExt_clicked();
    void on_btnRemoveExt_clicked();
	void on_btnClose_clicked();
	void closeEvent(QCloseEvent *);
        void on_pushButton_clicked(); //Modification: 2018.04
        void on_pushButton_2_clicked(); //Modification: 2018.04 


private:
        Ui::GExtensionDialogClass ui; //Modification: 2018.04 
        Ui::GMainWindowClass ui_main; //Modification: 2018.04
        bool checkButtonClick; //Modification: 2018.04 

	QMap<QString, QStringList *> *extMapDefault;		//!< Default extension map pointer
	QMap<QString, QStringList *> *extMapCustom;			//!< Custom extension map pointer
	bool customChanged;									//!< Flag indicating that custom extensions have changed
	bool extChanged;									//!< Flag indicating that unsaved extensions have changed for the current language

	void updateCustomExtensions();
};

#endif // GEXTENSIONDIALOG_H

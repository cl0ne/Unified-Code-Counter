//! GFileOpenMenu class definitions.
/*!
* \file GFileOpenMenu.h
*
* This file contains the GFileOpenMenu class definition.
*/

#ifndef GFILEOPENMENU_H
#define GFILEOPENMENU_H

#include <QDialog>
#include "ui_GFileOpenMenu.h"

//! File open menu dialog.
/*!
* \class GFileOpenMenu
*
* Defines a file open menu dialog.
*/
class GFileOpenMenu : public QDialog
{
    Q_OBJECT

public:
    GFileOpenMenu(QWidget *parent = 0, Qt::WindowFlags f = 0);
	~GFileOpenMenu(){};

private:
    Ui::GFileOpenMenuClass ui;
};

#endif // GFILEOPENMENU_H

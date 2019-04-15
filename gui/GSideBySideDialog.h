//! GSideBySideDialog class definitions.
/*!
* \file GSideBySideDialog.h
*
* This file contains the GSideBySideDialog class definition.
*/
#ifndef GSIDEBYSIDEDIALOG_H
#define GSIDEBYSIDEDIALOG_H

#ifndef NO_WEB_SUPPORT

#include <QDialog>
/* Modification: 2016.01; USC
   Updated to no longer require UI file, needed for dynamic
   webkit vs webengine code */

#define HTML_FILE "highlighted_diff.html"


//! Sidebyside dialog.
/*!
* \class GSideBySideDialog
*
* Defines an Sidebyside dialog.
*/
namespace Ui {
class GSideBySideDialog;
}

class GSideBySideDialog : public QDialog
{
    Q_OBJECT

public:
/* Modification: 2016.01; USC
   Minor cosmetic updates */
    GSideBySideDialog(const QString &filePathA, const QString &filePathB,
                      const QString &htmlPath, QWidget *parent = 0);
};

#endif
#endif // GSIDEBYSIDEDIALOG_H

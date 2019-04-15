//! GSideBySideDialog class methods.
/*!
* \file GSideBySideDialog.cpp
*
* This file contains the GSideBySideDialog class methods.
*/

/* Modification: 2016.01; USC
   Updated to no longer require UI file, needed for dynamic
   webkit vs webengine code */
#ifndef NO_WEB_SUPPORT
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFile>
#include "GSideBySideDialog.h"

//Different version of Qt need different web classes
#ifdef NEED_WEBKIT_LEGACY
#if (QT_VERSION < 0x050000)
#include <QWebView>
#else
#include <QtWebKitWidgets/QWebView>
#endif
#else
#include <QtWebEngineWidgets/QWebEngineView>
#endif

/*!
* Constructs a GSideBySideDialog object.
*
* \param filePathA souce A file or directory
* \param filePathB souce B file or directory
* \param parent parent widget
*/
GSideBySideDialog::GSideBySideDialog(const QString &filePathA, const QString &filePathB,
                                     const QString &htmlPath, QWidget *parent)
    :QDialog(parent)
{
    //Create the child widgets
    QLineEdit *lineEdit = new QLineEdit(this);
    QLineEdit *lineEdit_2 = new QLineEdit(this);
    QPushButton *btnClose = new QPushButton(this);
    btnClose->setText("Close");
    connect(btnClose, SIGNAL(clicked()), this, SLOT(accept()));
#ifdef NEED_WEBKIT_LEGACY
    QWebView *webView = new QWebView(this);
#else
    QWebEngineView *webView = new QWebEngineView(this);
#endif

    //layout the child widgets in this window
    QVBoxLayout *masterVLayout = new QVBoxLayout();

    QHBoxLayout *editLayout = new QHBoxLayout();
    editLayout->addWidget(lineEdit);
    editLayout->addWidget(lineEdit_2);
    editLayout->setContentsMargins(0,0,0,0);

    QHBoxLayout *closeLayout = new QHBoxLayout();
    closeLayout->addStretch();
    closeLayout->addWidget(btnClose);
    closeLayout->addStretch();
    closeLayout->setContentsMargins(0,0,0,0);

    masterVLayout->addLayout(editLayout);
    masterVLayout->addWidget(webView);
    masterVLayout->addLayout(closeLayout);
    masterVLayout->setContentsMargins(0,0,0,0);

    this->setLayout(masterVLayout);
    this->setWindowTitle("UCC Differencing Side-by-side Results");
    this->resize(811, 497);

    //Initialize data from the passed in arguments
    if (!filePathA.isEmpty())
    {
        lineEdit->setText(filePathA);
        lineEdit->setEnabled(true);
    }
    
    if (!filePathB.isEmpty())
    {
        lineEdit_2->setText(filePathB);
        lineEdit_2->setEnabled(true);
    }

    QString filename = htmlPath + "/" + HTML_FILE;
    if (QFile(filename).exists())
    {
       webView->load(QUrl::fromLocalFile(filename));
    }
    else
    {
        qDebug() << "WARNING: " << filename << " does not exist!";
    }
}
#endif

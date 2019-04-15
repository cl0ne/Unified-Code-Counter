//! GExtensionDialog class methods.
/*!
* \file GExtensionDialog.cpp
*
* This file contains the GExtensionDialog class methods.
*/

#include <QFile>
#include <QInputDialog>
#include <QMenu>
#include <QMessageBox>
#include "GExtensionDialog.h"
#include <QFileDialog> //Modification: 2018.04
#include "QFileInfo" //Modification: 2018.04
#include "GMainWindow.h" //Modification: 2018.04 

/*!
* Constructs a GExtensionDialog object.
*
* \param extensionMapDefault map of default language extensions
* \param extensionMapCustom map of custom language extensions
* \param parent parent widget
* \param f window flags
*/
GExtensionDialog::GExtensionDialog(QMap<QString, QStringList *> *extensionMapDefault, QMap<QString, QStringList *> *extensionMapCustom,
	QWidget *parent, Qt::WindowFlags f)
	: QDialog(parent, f)
{
	ui.setupUi(this);

	extMapDefault = extensionMapDefault;
	extMapCustom = extensionMapCustom;
	customChanged = false;
	extChanged = false;

	QListWidgetItem *lwItem;
	QFont font;
	foreach (QString name, extMapDefault->keys())
	{
		ui.lwLanguages->addItem(name);
		if (extMapCustom->contains(name))
		{
			lwItem = ui.lwLanguages->item(ui.lwLanguages->count() - 1);
			font = lwItem->font();
			font.setBold(true);
			lwItem->setFont(font);
		}
	}

        //Modification: 2018.04 Integration changes 
        bool flag = ((GMainWindow*)parent)->checkButtonClicked;
        if(flag){
           ui.pushButton_2->hide();
        } 

	ui.tblExtensions->horizontalHeader()->setStretchLastSection(true);
	connect(ui.lwLanguages, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(languageSelected(QListWidgetItem *)));
	connect(ui.tblExtensions, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(itemCustomContextMenuRequested(const QPoint &)));
}

/*!
* Destroys a GExtensionDialog object.
*/
GExtensionDialog::~GExtensionDialog()
{
}

/*!
* Updates list of extensions when a language is selected.
*
* \param lwItem selected list item
*/
void GExtensionDialog::languageSelected(QListWidgetItem *lwItem)
{
	if (lwItem)
	{
		if (extChanged)
		{
			this->updateCustomExtensions();
			extChanged = false;
		}

		QTableWidgetItem *tblItem;
		//QStringList *extList; warning
		QStringList *extList=NULL;
		QString selLang = lwItem->text();
		int i, cnt = 0;

		for (i = ui.tblExtensions->rowCount() - 1; i >= 0; i--)
			delete(ui.tblExtensions->takeItem(i, 0));

		if (extMapCustom->contains(selLang))
			extList = extMapCustom->value(selLang, 0);
		else if (extMapDefault->contains(selLang))
			extList = extMapDefault->value(selLang, 0);

		ui.tblExtensions->setRowCount(extList->count());
		if (extList->count() > 0)
		{
			for (i = 0; i < extList->count(); i++)
			{
				if (!extList->at(i).isEmpty())
				{
					tblItem = new QTableWidgetItem(extList->at(cnt));
					tblItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
					ui.tblExtensions->setItem(cnt, 0, tblItem);
					cnt++;
				}
			}
		}
	}
}

/*!
* Updates list of custom extensions.
*/
void GExtensionDialog::updateCustomExtensions()
{
	if (extChanged)
	{
		QList<QListWidgetItem *> lwItems = ui.lwLanguages->selectedItems();
		QListWidgetItem *lwItem;
		QStringList *extList;
		QString selLang;
		int i;

		if (lwItems.count() > 0)
		{
			lwItem = lwItems.first();
			selLang = lwItem->text();
			if (extMapCustom->contains(selLang))
				extList = extMapCustom->value(selLang);
			else
			{
				extList = new QStringList();
				extMapCustom->insert(selLang, extList);
			}
			extList->clear();

			for (i = 0; i < ui.tblExtensions->rowCount(); i++)
				extList->append(ui.tblExtensions->item(i, 0)->text());
		}
		extChanged = false;
	}
}

/*!
* Loads a custom context menu for the extension table.
*
* \param pos location of mouse selection
*/
void GExtensionDialog::itemCustomContextMenuRequested(const QPoint &pos)
{
	QMenu menu;
	QAction *aItem = 0;
	aItem = menu.addAction("Revert Selected Language Extensions", this, SLOT(revertSelLangExt()));
	aItem = menu.addAction("Revert All Language Extensions", this, SLOT(revertAllLangExt()));
	menu.exec(this->mapToGlobal(pos));
    //Warning fix 11.25.16
    (void)aItem;
}

/*!
* Reverts list of custom extensions to defaults for selected language.
*/
void GExtensionDialog::revertSelLangExt()
{
	QList<QListWidgetItem *> lwItems = ui.lwLanguages->selectedItems();
	QListWidgetItem *lwItem;
	QFont font;
	QStringList *extList;
	QString langName;

	if (lwItems.count() < 1)
		return;

	lwItem = lwItems.first();
	langName = lwItem->text();
	if (extMapCustom->contains(langName))
	{
		font = lwItem->font();
		font.setBold(false);
		lwItem->setFont(font);

		extList = extMapCustom->value(langName);
		delete(extList);
		extMapCustom->remove(langName);
		customChanged = true;
		extChanged = false;
		
		this->languageSelected(lwItem);
	}
}

/*!
* Reverts all custom extensions to defaults.
*/
void GExtensionDialog::revertAllLangExt()
{
	QList<QListWidgetItem *> lwItems = ui.lwLanguages->selectedItems();
	QListWidgetItem *lwItem;
	QFont font;
	QList<QStringList *> extLists;
	int i;

	if (extMapCustom->count() < 1)
		return;

	for (i = 0; i < ui.lwLanguages->count(); i++)
	{
		lwItem = ui.lwLanguages->item(i);
		font = lwItem->font();
		font.setBold(false);
		lwItem->setFont(font);
	}

	extLists = extMapCustom->values();
	for (i = extLists.count() - 1; i >= 0; i--)
		delete(extLists.takeAt(i));
	extMapCustom->clear();

	if (lwItems.count() > 0)
	{
		lwItem = lwItems.first();
		this->languageSelected(lwItem);
	}
	customChanged = true;
	extChanged = false;
}

/*!
* Adds a new extension to the list.
*/
void GExtensionDialog::on_btnAddExt_clicked()
{
	QList<QTableWidgetItem *> tblItems = ui.tblExtensions->selectedItems();
	QList<QListWidgetItem *> lwItems = ui.lwLanguages->selectedItems();
	QTableWidgetItem *tblItem;
	QListWidgetItem *lwItem;
	QFont font;
	QString ext;
	int i, row;
	bool ok;

	if (lwItems.count() < 1)
	{
		QMessageBox::warning(this, tr("Add Error"), tr("Unable to add an extension without a selected language."));
		return;
	}
	lwItem = lwItems.first();
	ext = QInputDialog::getText(this, tr("Add Extension"), tr("Please enter an extension for %1").arg(lwItem->text()),
		QLineEdit::Normal, "", &ok);
	if (ok && !ext.isEmpty())
	{
		if (!ext.startsWith("."))
		{
			QMessageBox::warning(this, tr("Add Error"), tr("Unable to add extension for %1. Extension must begin with a '.' character."));
			return;
		}
		for (i = 0; i < ui.tblExtensions->rowCount(); i++)
		{
			if (ui.tblExtensions->item(i, 0)->text() == ext)
				break;
		}
		if (i < ui.tblExtensions->rowCount())
		{
			QMessageBox::warning(this, tr("Add Error"), tr("Unable to add extension for %1. Extension already exists."));
			return;
		}

		if (tblItems.count() > 0)
		{
			tblItem = tblItems.first();
			row = tblItem->row() + 1;
		}
		else
			row = ui.tblExtensions->rowCount();
		ui.tblExtensions->insertRow(row);

		tblItem = new QTableWidgetItem(ext);
		tblItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		ui.tblExtensions->setItem(row, 0, tblItem);

		font = lwItem->font();
		font.setBold(true);
		lwItem->setFont(font);

		customChanged = true;
		extChanged = true;

                this->updateCustomExtensions();
                extChanged = false;
	}
}

/*!
* Removes the selected extension(s).
*/
void GExtensionDialog::on_btnRemoveExt_clicked()
{
	QList<QTableWidgetItem *> tblItems = ui.tblExtensions->selectedItems();
	QList<QListWidgetItem *> lwItems = ui.lwLanguages->selectedItems();
	QListWidgetItem *lwItem;
	QFont font;
	int i, row;

	if (lwItems.count() < 1 || tblItems.count() < 1)
		return;

	lwItem = lwItems.first();
	font = lwItem->font();
	font.setBold(true);
	lwItem->setFont(font);

	for (i = 0; i < tblItems.count(); i++)
	{
		row = tblItems.at(i)->row();
		delete(ui.tblExtensions->takeItem(row, 0));
		ui.tblExtensions->removeRow(row);
		customChanged = true;
		extChanged = true;
                this->updateCustomExtensions();
                extChanged = false;
	}
}

/*!
* Closes the dialog
*/
void GExtensionDialog::on_btnClose_clicked()
{
	this->close();
}

/*!
* Processes a close event sent to the form.
*/
void GExtensionDialog::closeEvent(QCloseEvent *)
{
	if (customChanged)
	{
                ((GMainWindow*)(parent()))->writeExtensionsFile();
		customChanged = false;
		this->accept();
	}
	else
		this->reject();
}

//Modification: 2018.04 USC starts
void GExtensionDialog::on_pushButton_clicked()
{
    if (customChanged)
    {
        ((GMainWindow*)(parent()))->writeExtensionsFile();
        customChanged = false;
        this->accept();
    }
    else
        this->reject();
}

void GExtensionDialog::on_pushButton_2_clicked()
{
    QString extensionFile="extensions.txt";
    extensionFile = QFileDialog::getSaveFileName(this,
            tr("Extension File"), extensionFile, tr("Extension File (*.txt)"), 0, QFileDialog::DontConfirmOverwrite);
    if(extensionFile == "")
        return;
    QFileInfo fi(extensionFile);

    if (!fi.exists())
    {QFile file(extensionFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
    QMessageBox::warning(this, tr("File Error"), tr("Unable to open extension file: ") + extensionFile + "\n" + file.errorString() );
    return;
    }
    file.close();
    ((GMainWindow*)(parent()))->ui.txtExtensionFile->setText(extensionFile);
    }
    this->close();
}
//Modification: 2018.04 USC ends

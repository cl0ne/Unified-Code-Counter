//! GAsciiDialog class methods.
/*!
* \file GAsciiDialog.cpp
*
* This file contains the GAsciiDialog class methods.
*/

#include <QDir>
#include <QTextStream>
#include "GTableDialog.h"

/*!
* Constructs a GTableDialog object.
*
* \param filePath results file or directory
* \param parent parent widget
* \param f window flags
*/
GTableDialog::GTableDialog(const QString &filePath, QWidget *parent, Qt::WindowFlags f)
	: QDialog(parent, f)
{
	ui.setupUi(this);

	outputDir = "";
	ui.cboResultsSelect->clear();
	ui.tblResults->clear();

	if (!filePath.isEmpty())
	{
		QFileInfo fi(filePath);
		if (fi.isDir())
		{
			outputDir = filePath;
			QDir dir(filePath);
			dir.setNameFilters(QStringList() << tr("*outfile*.csv") << tr("MatchedPairs.csv") << tr("*DuplicatePairs.csv"));
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
void GTableDialog::on_cboResultsSelect_currentIndexChanged(const QString &fileName)
{
    ui.tblResults->clear();
    ui.tblResults->setColumnCount(0);
    ui.tblResults->setRowCount(0);
    QFile file(outputDir + "/" + fileName);
    if (file.open(QIODevice::ReadOnly))
	{
		QTextStream ts(&file);
		QTableWidgetItem *twItem;
		QString line;
		int i = -1, j, k, n;
		while (ts.status() == QTextStream::Ok && !ts.atEnd())
		{
			line = ts.readLine();
			if (!line.isEmpty())
			{
				i++;
				ui.tblResults->insertRow(i);
                QStringList s;

                /*
                 * In some cases, we want to display quotes or commas in CSV file.
                 * To do that, we need to quote the whole line.
                 * In this if condition, we parse this kind of text and display it in one column
                */
                if (line.at(0) == QChar('"') && line.endsWith(QChar('"')))
                {
                    line = line.mid(1, line.size() - 2);
                    QString temp = "";
                    for (int it1 = 0; it1 < line.size(); it1++)
                    {
                        temp.append(line.at(it1));
                        /*
                         * The quote character we want to display in CSV has been replace with two quote characters ("")
                         * So here we need to skip one quote character
                        */
                        if (line.at(it1) == QChar('"') && it1 + 1 < line.size() && line.at(it1 + 1) == QChar('"'))
                            it1++;
                    }
                    s.append(temp);
                }
                else s = line.split(',');
				if (s.count() > ui.tblResults->columnCount())
					ui.tblResults->setColumnCount(s.count());
				for (j = 0; j < s.count(); j++)
				{
					twItem = new QTableWidgetItem(s.value(j));
					ui.tblResults->setItem(i, j, twItem);
					if (s.value(j).length() > 15 && j == s.count() - 1)
						ui.tblResults->setSpan(i, j, 1, 2);
					else if (j < s.count() - 1 && s.value(j + 1).isEmpty())
					{
						n = 2;
						for (k = j + 1; k < s.count() - 1; k++)
						{
							if (!s.value(k + 1).isEmpty())
								break;
							n++;
						}
						ui.tblResults->setSpan(i, j, 1, n);
					}
				}
			}
		}

		// update span for title rows
		for (i = 0; i < ui.tblResults->rowCount() - 1; i++)
		{
			QString text = ui.tblResults->item(i, 0)->text();
			if (ui.tblResults->item(i, 0)->text().startsWith("          "))
			{
				for (j = 0; j < ui.tblResults->columnCount(); j++)
				{
					if (!ui.tblResults->item(i + 1, j) || ui.tblResults->item(i + 1, j)->text().isEmpty())
						break;
					n = ui.tblResults->columnSpan(i + 1, j);
					if (n > 1)
						j += n - 1;
				}
				if (j < ui.tblResults->columnCount())
					ui.tblResults->setSpan(i, 0, 1, j);
			}
		}
	}
}

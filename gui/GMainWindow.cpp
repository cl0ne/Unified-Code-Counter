//! GMainWindow class methods.
/*!
* \file GMainWindow.cpp
*
* This file contains the GMainWindow class methods.
*/

#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QTextStream>
#include "../src/UCCGlobals.h"
#include "../src/CUtil.h"
#include "../src/DiffTool.h"
#include "../src/MainObject.h"
#include "GMainWindow.h"
#include "GUtil.h"
#include "GSideBySideDialog.h"
#include "QFileInfo"
#include "GMainWindow.h"


#define MAX_ARGUMENTS 20
#define ARGUMENT_LENGTH 1024

#ifdef Q_OS_WIN
#define strcpy(dest, src) strcpy_s(dest, ARGUMENT_LENGTH, src)
#endif

/*!
* Constructs a GMainWindow object.
*
* \param parent parent widget
* \param f window flags
*/
GMainWindow::GMainWindow(QWidget *parent, Qt::WindowFlags f)
	: QMainWindow(parent, f)
{
	ui.setupUi(this);
	customChanged = false;
	defaultDirSet = false;
	execCanceled = false;
        checkButtonClicked = true;

#ifdef NO_WEB_SUPPORT
    /* Modification: 2016.01; USC
       If there is no Qt Web support (such as mingw in 5.7) we must disable
       the visual differencing feature
    */
    ui.chkVisualDiffResult->setDisabled(true);
#endif

	ui.splitter->setSizes(QList<int>() << 400 << 0 << 100);

	ui.lblFilesB->hide();
	ui.lwFileListB->hide();
    ui.lblAddFileB->hide();
    ui.lblRemoveFileB->hide();
    ui.lblAddFolderB->hide();
	ui.btnAddFileB->hide();
	ui.btnRemoveFileB->hide();
	ui.btnAddFolderB->hide();
    //Grey out functional differencing on initial screen
    ui.chkFuncDifferencing->setEnabled(false);	this->getDefaultExtensions();
	this->extensionsUpdated();

	ui.txtModThreshold->setValidator(new QIntValidator(0, 100, ui.txtModThreshold));
	ui.txtDupThreshold->setValidator(new QIntValidator(0, 100, ui.txtModThreshold));
	ui.txtTruncThreshold->setValidator(new QIntValidator(1, INT_MAX, ui.txtModThreshold));
        ui.txtRamLimit->setValidator(new QIntValidator(1,5120, ui.txtRamLimit));
        ui.txtThreads->setValidator(new QIntValidator( MIN_UCC_THREAD_COUNT, MAX_UCC_THREAD_COUNT, ui.txtThreads));

	bgrpOutputFormat.addButton(ui.rdoCSVOutput, 0);
	bgrpOutputFormat.addButton(ui.rdoAsciiOutput, 1);
	bgrpOutputFormat.addButton(ui.rdoLegacyOutput, 2);
	bgrpOutputFormat.setExclusive(true);
	ui.rdoCSVOutput->setChecked(true);

#if defined(Q_OS_WIN)
	ui.chkProcLinks->setChecked(false);
	ui.chkProcLinks->setEnabled(false);
#endif

	ui.txtOutputDir->setText(QDir::toNativeSeparators(QDir::homePath()));

	this->parsePreferencesFile();
        ui.chkExtensionFile->setChecked(false);
        ui.txtExtensionFile->setText("");

	progressBar = new QProgressBar();
	progressBar->setRange(0, 100);
	progressBar->setTextVisible(false);
	progressBar->setVisible(false);		// initially hidden
	ui.statusBar->addPermanentWidget(progressBar);
    setAcceptDrops(true);

	connect(ui.lwFileListA, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(listACustomContextMenuRequested(const QPoint &)));
	connect(ui.lwFileListB, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(listBCustomContextMenuRequested(const QPoint &)));
}

/*!
* Destroys a GMainWindow object.
*/
GMainWindow::~GMainWindow()
{
	QMapIterator<QString, QStringList *> itd(extensionMapDefault);
	while (itd.hasNext())
	{
		itd.next();
		delete(itd.value());
	}
	extensionMapDefault.clear();
	QMapIterator<QString, QStringList *> itc(extensionMapCustom);
	while (itc.hasNext())
	{
		itc.next();
		delete(itc.value());
	}
	extensionMapCustom.clear();
}

/*!
* Start the count and/or comparison operation.
*
* This is roughly equal to the UCC main.cpp procedure
* so do similar calls to be sure UCC is setup OK.
*/
void GMainWindow::on_btnStart_clicked()
{
    ClearSummaryMsgCounts();    // Modification: 2015.12
    Init_StackDump();           // Modification: 2015.12
    CUtil::InitToLower();       // Initialize to lower helper array

    // Erase any messages from earlier runs
    ui.txtMessages->clear();

	QFile file;
	QStringList argList;
	QString text, fileName;
    int i;
    
    //Qwebviewer
    QString filePathA, filePathB;

    if ((ui.lwFileListA->count() < 1) && (!ui.chkDifferencing->isChecked() || ui.lwFileListB->count() < 1)) //|| (ui.lwFileListA->selectedItems()).length() < 1)
	{
        QMessageBox::warning(this, tr("At Least 1 Selection Needed"), tr("No files/folders are selected for processing."));
		return;
	}

    if ( ( ui.chkDifferencing->isChecked() )
      && ( ( 0 == ui.lwFileListA->count() ) || ( 0 == ui.lwFileListB->count() ) ) )
    {
        QMessageBox::warning(this, tr("2 Baselines for Differencing Needed"), tr("There are not enough files/folders selected to find Differences between.\nPlease select some more."));
        return;
    }

	QApplication::setOverrideCursor(QCursor(Qt::BusyCursor));
    ui.statusBar->showMessage(tr("Creating file list(s)..."));
	execCanceled = false;
	this->executionRunning(true);

    // Now that UCC is running without any more User input for now,
    // capture the start of the total running time
    InitTimes();

    // Change these as you wish
    bool	show_any_times = true;
    bool	show_total_only = false;

    // Check for this Option first as it uses Help feature from UCC
    if ( ui.chkExtensionFile->isChecked() )
    {
        if ( !ui.txtExtensionFile->text().isEmpty() )
        {
            // write out extension file if changed
            if (customChanged)
            {
                if (!this->writeExtensionsFile())
                {
                    ui.statusBar->clearMessage();
                    return;
                }
            }
            argList.append("-extfile");
            argList.append(ui.txtExtensionFile->text());
        }
        /*
        else
        {
            // No Extension file specs given so run UCC to get Help in a pop up
            QString qMsg;
            if ( 0 == qMsg.size() )
            {
                MainObject mainObject;
                mainObject.ConnectUserIF(this);
                string msg;
                mainObject.ShowUsage( "-extfile", false, &msg );
                qMsg = msg.c_str();
            }

            // Pop up and give the User a look at the Help text
            QMessageBox * pMsgBox = new QMessageBox( QMessageBox::Information, "About  -extfile  Option", "Working ... hit OK!", QMessageBox::Ok,
                                                  NULL,
                                                  Qt::Dialog );
            pMsgBox->setText( qMsg );
            pMsgBox->show();

            delete pMsgBox;

            return;
        }
        */
    }

    // process file list
    if (ui.chkDifferencing->isChecked())
    {
        fileName = FILE_LIST_A;
		if (!ui.txtOutputDir->text().isEmpty())
			fileName = ui.txtOutputDir->text() + "/" + fileName;
        file.setFileName(fileName);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QFileDevice::FileError fileError = file.error();
            if ( QFileDevice::OpenError == file.error() )
            {
                // We get this when the Path does not exist, so make it.
                QString path = file.fileName();
                QString dir = path.section( "/", 0, -2 );
                QDir qDir;
                bool dirMade = qDir.mkdir( dir );
                if ( !dirMade )
                {
                    ui.statusBar->clearMessage();
                    QApplication::restoreOverrideCursor();
                    QMessageBox::warning( this, tr("Unable to Create Directory"), dir );
                    this->executionRunning( false );
                    return;
                }

                // Now we should Write the file OK to the new directory
                if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
                    fileError = file.error();
                else
                    fileError = QFileDevice::NoError;
            }

            if ( QFileDevice::NoError != fileError )
            {
                ui.statusBar->clearMessage();
                QApplication::restoreOverrideCursor();
                QString text_msg = tr("Unable to open ") + QDir::toNativeSeparators(fileName) + tr(" for writing.\n");
                string numStr;
                IntToStr( (int)file.error(), numStr );
                text_msg += numStr.c_str();
                text_msg += " " + file.errorString();
                QMessageBox::warning( this, tr("File Error"), text_msg );
                this->executionRunning(false);
                return;
            }
		}
		QTextStream tsa(&file);
        for (i = 0; i < ui.lwFileListA->count(); ++i)
		{
            text = ui.lwFileListA->item(i)->text();
            //Qwebviewer
            filePathA = text;
			tsa << text << endl;
        }
        file.close();

        fileName = FILE_LIST_B;
		if (!ui.txtOutputDir->text().isEmpty())
			fileName = ui.txtOutputDir->text() + "/" + fileName;
		file.setFileName(fileName);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			ui.statusBar->clearMessage();
			QApplication::restoreOverrideCursor();
            QString text_msg = tr("Unable to open ") + QDir::toNativeSeparators(fileName) + tr(" for writing.\n");
            string numStr;
            IntToStr( (int)file.error(), numStr );
            text_msg += numStr.c_str();
            text_msg += " " + file.errorString();
            QMessageBox::warning( this, tr("File Error"), text_msg );
			this->executionRunning(false);
			return;
		}
		QTextStream tsb(&file);
		for (i = 0; i < ui.lwFileListB->count(); ++i)
		{
			text = ui.lwFileListB->item(i)->text();
            //Qwebviewer
            filePathB = text;			
			tsb << text << endl;
		}
		file.close();
    }
	else
	{
		fileName = "fileList.txt";

		/* save fileList to the current path instead of the output path */

		file.setFileName(fileName);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			ui.statusBar->clearMessage();
			QApplication::restoreOverrideCursor();
            QString text_msg = tr("Unable to open ") + QDir::toNativeSeparators(fileName) + tr(" for writing.\n");
            text_msg += file.errorString();
            QMessageBox::warning( this, tr("File Error"), text_msg );
			this->executionRunning(false);
			return;
		}
		QTextStream ts(&file);
		for (i = 0; i < ui.lwFileListA->count(); i++)
		{
			text = ui.lwFileListA->item(i)->text();
			ts << text << endl;
		}
		file.close();
	}
	
	ui.statusBar->showMessage(tr("Processing program options..."));
	QApplication::processEvents();
	if (execCanceled)
	{
		ui.statusBar->clearMessage();
		QApplication::restoreOverrideCursor();
		this->executionRunning(false);
		return;
	}

	// add application name and output directory to arguments
    argList.insert(begin(argList),QApplication::applicationName());
	if (ui.txtOutputDir->text().isEmpty())
	{
		ui.statusBar->clearMessage();
		QApplication::restoreOverrideCursor();
		QMessageBox::warning(this, tr("Selection Error"), tr("A valid Output Directory must be specified."));
		this->executionRunning(false);
		return;
	}
	QFileInfo fio(ui.txtOutputDir->text());
	if (!fio.isDir())
	{
        // Create the Directory the User gave
        QString dir = ui.txtOutputDir->text();
        QDir qDir;
        bool dirMade = qDir.mkdir( dir );
        if ( !dirMade )
        {
            ui.statusBar->clearMessage();
            QApplication::restoreOverrideCursor();
            QMessageBox::warning( this, tr("Unable to Create Directory"), dir );
            this->executionRunning(false);
            return;
        }
	}
	argList.append("-outdir");
	argList.append(ui.txtOutputDir->text());

    if (ui.cbxNoHeader->isChecked())
        argList.append("-noheader");
	// process program options
    if (ui.chkDifferencing->isChecked())
    {
		argList.append("-d");
        if (ui.chkModThreshold->isChecked())
		{
            argList.append("-t");
			argList.append(ui.txtModThreshold->text());
		}
        if(ui.chkVisualDiffResult->isChecked()){
            argList.append("-visualdiff");
        }
        // Modification 2017.02
        if(ui.chkFuncDifferencing->isChecked()) 
        {
            argList.append("-funcDiff");
        }
    }

	if (ui.chkDupThreshold->isChecked())
	{
		argList.append("-tdup");
		argList.append(ui.txtDupThreshold->text());
	}

	if (ui.chkTruncThreshold->isChecked())
	{
		argList.append("-trunc");
		argList.append(ui.txtTruncThreshold->text());
	}

    if (ui.rdoAsciiOutput->isChecked())
		argList.append("-ascii");
	else if (ui.rdoLegacyOutput->isChecked())
		argList.append("-legacy");

	if (ui.chkUnifiedOut->isChecked())
		argList.append("-unified");

	if (ui.chkClearCaseFiles->isChecked())
		argList.append("-cf");

	if (!ui.chkProcDuplicates->isChecked())
		argList.append("-nodup");
	
	if (!ui.chkProcComplexity->isChecked())
        argList.append("-nocomplex");

        if(ui.chkWarnings->isChecked()){
            argList.append("-nowarnings");
        }

        if(ui.chkUncounted->isChecked()){
            argList.append("-nouncounted");
        }
        if(ui.chkCc4Enable->isChecked()){
            argList.append("-cc4enable");
        }
        if(ui.chkRamLimit->isChecked()){
            argList.append("-ramlimit");
            argList.append(ui.txtRamLimit->text());
        }
#ifdef   ENABLE_THREADS
        if( ui.chkThreads->isChecked() )
        {
            argList.append("-threads");
            argList.append( ui.txtThreads->text().simplified() );
        }
#endif

#if !defined(Q_OS_WIN)
	if (!ui.chkProcLinks->isChecked())
		argList.append("-nolinks");
#endif

	// convert arguments from QStringList to char **
	char **argv = new char *[MAX_ARGUMENTS];
	for (int i = 0; i < MAX_ARGUMENTS; ++i)
	{
		argv[i] = new char[ARGUMENT_LENGTH];
		strcpy(argv[i], "");
	}
	for (i = 0; i < argList.count(); i++)
	{
		QByteArray ba = argList.at(i).toLocal8Bit();
		strcpy(argv[i], ba.data());
	}
	
	ui.statusBar->showMessage(tr("Clearing old results..."));
	
	// delete all previous output files
	QDir outDir(ui.txtOutputDir->text());
	outDir.setNameFilters(QStringList() << "*outfile*.csv" << "MatchedPairs.csv" << "DuplicatePairs.csv" << "*outfile*.txt" << "MatchedPairs.txt" << "DuplicatePairs.txt" << "error_log_*.txt");
	QStringList outList = outDir.entryList();
	QFile outFile;
	for (i = 0; i < outList.count(); i++)
	{
		outFile.setFileName(outDir.absolutePath() + "/" + outList.value(i));
		outFile.remove();
	}

	this->writePreferencesFile();
	QApplication::processEvents();
	if (execCanceled)
	{
		ui.statusBar->clearMessage();
		QApplication::restoreOverrideCursor();
		this->executionRunning(false);
		for (i = 0; i < MAX_ARGUMENTS; ++i)
			delete[] argv[i];
		delete[] argv;
		return;
	}
	
	ui.statusBar->showMessage(tr("Starting count process..."));
	progressBar->setValue(0);
	progressBar->show();

    bool			doDiff = false;
    bool			doDups = false;
    double			duplicate_threshold_used = 0.0;
    unsigned long	files_A_count = 0;
    unsigned long	files_B_count = 0;
	if (ui.chkDifferencing->isChecked())
	{
        doDiff = true;
           // Modification: 2017.02
           if (ui.chkFuncDifferencing->isChecked())
           {
               isFuncDiff = true;
           }
           else
           {
               isFuncDiff = false;
           }

		DiffTool diffTool;
		diffTool.ConnectUserIF(this);
        diffTool.diffToolProcess(argList.count(), argv);
        duplicate_threshold_used = diffTool.GetDuplicateThreshold();    // Modification: 2015.12

        // Make sure worker Threads are done.  Could be half done due to LOW Memory.
        FinishWorkerThreads();                                          // Modification: 2015.12

        files_A_count = SourceFileA.size();                             // Modification: 2015.12
        CountPhysicalFiles( SourceFileA, files_A_count );               // Modification: 2015.12

        files_B_count = SourceFileB.size();                             // Modification: 2015.12
        CountPhysicalFiles( SourceFileB, files_B_count );               // Modification: 2015.12
        // Modification: 2017.02
        if(isFuncDiff)
        {
            SourceFileA.resize(0);
            SourceFileB.resize(0);
            DiffTool funcDiffTool;
            funcDiffTool.funcDiffProcess(argList.count(), argv);
        }
    }
	else
	{
		MainObject mainObject;
		mainObject.ConnectUserIF(this);
        mainObject.MainProcess(argList.count(), argv);
        duplicate_threshold_used = mainObject.GetDuplicateThreshold();  // Modification: 2015.12

        files_A_count = SourceFileA.size();                             // Modification: 2015.12
        CountPhysicalFiles( SourceFileA, files_A_count );               // Modification: 2015.12
	}

    for (i = 0; i < MAX_ARGUMENTS; ++i)
        delete[] argv[i];
    delete[] argv;

	QApplication::restoreOverrideCursor();
	ui.statusBar->clearMessage();
	progressBar->setValue(0);
	progressBar->hide();
	this->executionRunning(false);

	if (!execCanceled)
	{
		// add all the output files in the combo box
		if (ui.rdoCSVOutput->isChecked())
		{

			mTableDialog = new GTableDialog(ui.txtOutputDir->text(), this);
			mTableDialog->show();
		}
		else
		{
			
			mAsciiDialog = new GAsciiDialog(ui.txtOutputDir->text(), this);
			mAsciiDialog->show();
		}

#ifndef NO_WEB_SUPPORT
       /* Modification: 2016.01; USC
	      Can only show if web support is available (this is currently true on
          all platforms that support Qt except for Qt 5.6 and above with mingw
       */
        // Qwebviewer: Show side-by-side dailog
        if(ui.chkDifferencing->isChecked() && ui.chkVisualDiffResult->isChecked())
        {
            mSideBySideDialog = new GSideBySideDialog(
                        ui.txtOutputDir->text() + "\\" + FILE_LIST_A,
                        ui.txtOutputDir->text() + "\\" + FILE_LIST_B,
                        ui.txtOutputDir->text(), this);
            mSideBySideDialog->show();
        }
        else{}
#endif

        // Get the Time to show
        time( &timeEnd );
        double	total_seconds = difftime( timeEnd, timeStart ) + 0.5;

        string	time_str;
        char buf[MSG_STR_BUF_SIZE];

        // Use more Secure C library APIs if available
    #ifdef	_MSC_VER
        strcpy_s( buf, MSG_STR_BUF_SIZE * sizeof(char), "" );
    #else
        // Use less Secure original C library APIs
        strcpy( buf, "" );
    #endif

        if ( doDiff )
        {
            TimeMsg( buf, time_str, " A Files: ", (long)files_A_count, "  ", false );
            TimeMsg( buf, time_str, " B Files: ", (long)files_B_count, "", false );
        }
        else
            TimeMsg( buf, time_str, " ", (long)files_A_count, " files", false );

        if ( 1.0 > total_seconds )
            total_seconds = 1.0;		// AVOID divide by Zero

    // Use more Secure C library APIs if available
    #ifdef	_MSC_VER
        strcpy_s( buf, MSG_STR_BUF_SIZE * sizeof(char), "" );
        sprintf_s( buf, MSG_STR_BUF_SIZE * sizeof(char), " for %.1f files processed per second\n",
                    ( (double)( files_A_count + files_B_count )/total_seconds ) + .05 );
    #else
        // Use less Secure original C library APIs
        strcpy( buf, "" );
        sprintf( buf, " for %.1f files processed per second\n",
                    ( (double)( files_A_count + files_B_count )/total_seconds ) + .05 );
    #endif
        time_str += buf;

        if ( duplicate_threshold_used >= 0.0 )
            doDups = true;

        // show the Times of various steps and total elapsed Time if wanted
        if ( true == show_any_times )
            ShowTiming( time_str, timeStart, timeEnd, show_total_only, doDiff, doDups );

        bool	extra_counts = false;
        if ( stack_dump_count )
        {
            string count;
            IntToStr( (int)stack_dump_count, count );

            if ( false == extra_counts )
            {
                time_str += "\n";
                extra_counts = true;
            }

            time_str += "    Stack Dumps:  ";
            time_str += count;
            time_str += "\n";
        }

        if ( errors_count )
        {
            string count;
            IntToStr( (int)errors_count, count );

            if ( false == extra_counts )
            {
                time_str += "\n";
                extra_counts = true;
            }

            time_str += "         Errors:  ";
            time_str += count;
            time_str += "\n";
        }

        if ( warnings_count )
        {
            string count;
            IntToStr( (int)warnings_count, count );

            if ( false == extra_counts )
            {
                time_str += "\n";
                extra_counts = true;
            }

            time_str += "       Warnings:  ";
            time_str += count;
            time_str += "\n";
        }

        if ( information_count )
        {
            string count;
            IntToStr( (int)information_count, count );

            if ( false == extra_counts )
            {
                time_str += "\n";
                extra_counts = true;
            }

            time_str += "    Information:  ";
            time_str += count;
            time_str += "\n";
        }

        if ( uncounted_files_count )
        {
            string count;
            LongToStr( (long)uncounted_files_count, count );

            if ( false == extra_counts )
            {
                time_str += "\n";
                extra_counts = true;
            }

            time_str += "Uncounted Files:  ";
            time_str += count;
            time_str += "\n";
        }

        if ( time_str.size() )
        {
            cout << time_str;

            // Write the Times to a file
            SavePerformanceStats( time_str );

            // and show in text message area
            QString message = time_str.c_str();
            updateLog( message );
        }

    }

}

/*!
* Stop the running the count/comparison operation.
*/
void GMainWindow::on_btnStop_clicked()
{
	execCanceled = true;
	emit this->canceledExecution();
}

/*!
* Updates the interface for running execution.
*
* \param enabled execution enabled
*/
void GMainWindow::executionRunning(bool enabled)
{
	ui.splitter->setEnabled(!enabled);
	ui.bgrpOptions->setEnabled(!enabled);
	ui.btnStart->setEnabled(!enabled);
	ui.btnStop->setEnabled(enabled);
}

/*!
* Update the displayed log messages.
*
* \param err error string
*/
void GMainWindow::updateLog(const QString &err)
{
	QString msgLog = ui.txtMessages->toPlainText();
    msgLog += err;
	ui.txtMessages->setPlainText(msgLog);
	ui.txtMessages->ensureCursorVisible();

    // Changed font in UI editor to Consolas (Monospaced font)
    ui.txtMessages->setEnabled( true );
}

/*!
* Update the progress bar and/or status message.
*
* \param msg message string
* \param pct percent completion (-1 if none)
*/
void GMainWindow::updateProgress(const QString &msg, int pct)
{
	if (!msg.isEmpty())
		ui.statusBar->showMessage(msg);
	if (pct >= 0)
		progressBar->setValue(pct);
}

/*!
* Open a file.
*/
void GMainWindow::on_actionOpen_File_triggered()
{
	QString filePath, defaultDir = "";
	if (!ui.txtOutputDir->text().isEmpty())
		defaultDir = ui.txtOutputDir->text();
	if (ui.rdoCSVOutput->isChecked())
		filePath = QFileDialog::getOpenFileName(this, tr("Open a File"), defaultDir, tr("CSV Files (*.csv);;Text Files (*.txt)"));
	else
		filePath = QFileDialog::getOpenFileName(this, tr("Open a File"), defaultDir, tr("Text Files (*.txt);;CSV Files (*.csv)"));
	if (!filePath.isEmpty())
	{
		if (filePath.toLower().endsWith(".csv"))
		{
			mTableDialog = new GTableDialog(filePath, this);
			mTableDialog->show();
		}
		else
		{
			mAsciiDialog = new GAsciiDialog(filePath, this);
			mAsciiDialog->show();
		}
	}
	defaultDirSet = true;
}

/*!
* Edit extensions.
*/
void GMainWindow::on_actionEdit_Extensions_triggered()
{
	GExtensionDialog eExtension(&extensionMapDefault, &extensionMapCustom, this);
	if (eExtension.exec() == QDialog::Accepted)
		customChanged = true;
}

/*!
* Exit the program.
*/
void GMainWindow::on_actionExit_triggered()
{
	this->close();
}

/*!
* Display the user manual.
*/
void GMainWindow::on_actionUser_Manual_triggered()
{
	QString fileName = "UCC_user_manual_v." + QString(PRODUCT_REVISION) + ".pdf";
	QString filePath = "../" + fileName;
	QFileInfo fi(filePath);
	if (!fi.exists())
	{
		filePath = fileName;
		fi.setFile(filePath);
		if (!fi.exists())
		{
            QMessageBox::warning(this, tr("File Error"), tr("Unable to open file: ") + fileName);
			return;
		}
	}
	GUtil::openFile(fi.absoluteFilePath());
}

/*!
* Display the release notes.
*/
void GMainWindow::on_actionRelease_Notes_triggered()
{
	QString fileName = "UCC_release_notes_v." + QString(PRODUCT_REVISION) + ".pdf";
	QString filePath = "../" + fileName;
	QFileInfo fi(filePath);
	if (!fi.exists())
	{
		filePath = fileName;
		fi.setFile(filePath);
		if (!fi.exists())
		{
			QMessageBox::warning(this, tr("File Error"), tr("Unable to open file: ") + fileName);
			return;
		}
	}
	GUtil::openFile(fi.absoluteFilePath());
}

/*!
* Display the release notes.
*/
void GMainWindow::on_actionLicense_triggered()
{
	QString fileName = "license.txt";
	QString filePath = "../" + fileName;
	QFileInfo fi(filePath);
	if (!fi.exists())
	{
		filePath = fileName;
		fi.setFile(filePath);
		if (!fi.exists())
		{
            QMessageBox::warning(this, tr("File Error"), tr("Unable to open file: ") + fileName);
			return;
		}
	}
	GUtil::openFile(fi.absoluteFilePath());
}

/*!
* Display the about information.
*/
void GMainWindow::on_actionAbout_triggered()
{
	QMessageBox aboutBox(QMessageBox::NoIcon, "About UCC",
		"Unified Code Counter\n\n"
		"University of Southern California\n"
		"Center for Systems and Software Engineering\n\n"
		"Version: " + QString(PRODUCT_REVISION),
		QMessageBox::Ok, this);
	aboutBox.setIconPixmap(QPixmap::fromImage(QImage(":/images/gucc.png").scaled(100, 100, Qt::KeepAspectRatio)));
	QPushButton *aboutQtButton = aboutBox.addButton("About Qt", QMessageBox::RejectRole);
	aboutBox.exec();
	if (aboutBox.clickedButton() == (QAbstractButton *)aboutQtButton)
		QMessageBox::aboutQt(this, "About Qt");
	else
		aboutBox.accept();
}

/*!
* Add a file to list A.
*/
void GMainWindow::on_btnAddFileA_clicked()
{
	ui.statusBar->showMessage(tr("Loading file browser..."));

	QStringList filters, fileList;
	QString filePath, filterStr, defaultDir = "";
	int i;

	if (!defaultDirSet && !ui.txtOutputDir->text().isEmpty())
		defaultDir = ui.txtOutputDir->text();
	filters = this->getFilters();
	if (filters.count() > 0)
	{
		filterStr = "Selected Files (" + filters.first();
		for (i = 1; i < filters.count(); i++)
			filterStr += " " + filters.at(i);
		filterStr += ")";
	}
	else
		filterStr = "All Files (*.*)";
	fileList = QFileDialog::getOpenFileNames(this, tr("Add a File"), defaultDir, tr("%1").arg(filterStr));

	foreach(filePath, fileList)
	{
		filePath = QDir::toNativeSeparators(filePath);
		if (ui.lwFileListA->findItems(filePath, Qt::MatchFlags()).isEmpty())
			ui.lwFileListA->addItem(filePath);
	

	}
	defaultDirSet = true;
	ui.statusBar->clearMessage();
}

/*!
* Add a file to list B.
*/
void GMainWindow::on_btnAddFileB_clicked()
{
	QStringList filters, fileList;
	QString filePath, filterStr, defaultDir = "";
	int i;

	if (!defaultDirSet && !ui.txtOutputDir->text().isEmpty())
		defaultDir = ui.txtOutputDir->text();
	filters = this->getFilters();
	if (filters.count() > 0)
	{
		filterStr = "Selected Files (" + filters.first();
		for (i = 1; i < filters.count(); i++)
			filterStr += " " + filters.at(i);
		filterStr += ")";
	}
	else
		filterStr = "All Files (*.*)";
	fileList = QFileDialog::getOpenFileNames(this, tr("Add a File"), defaultDir, tr("%1").arg(filterStr));

	foreach(filePath, fileList)
	{
		filePath = QDir::toNativeSeparators(filePath);
		if (ui.lwFileListB->findItems(filePath, Qt::MatchFlags()).isEmpty())
			ui.lwFileListB->addItem(filePath);
	}
	defaultDirSet = true;
	ui.statusBar->clearMessage();
}

/*!
* Remove selected file(s) from list A.
*/
void GMainWindow::on_btnRemoveFileA_clicked()
{
    QList<QListWidgetItem *> lwItems = ui.lwFileListA->selectedItems();
	for (int i = lwItems.count() - 1; i >= 0; i--)
		delete(lwItems.takeAt(i));
}

/*!
* Remove selected file(s) from list B.
*/
void GMainWindow::on_btnRemoveFileB_clicked()
{
	QList<QListWidgetItem *> lwItems = ui.lwFileListB->selectedItems();
	for (int i = lwItems.count() - 1; i >= 0; i--)
		delete(lwItems.takeAt(i));
}

/*!
* Add a folder to list A.
*/
void GMainWindow::on_btnAddFolderA_clicked()
{
	ui.statusBar->showMessage(tr("Loading file browser..."));
	
	QString dirPath, defaultDir = "";
	if (!defaultDirSet && !ui.txtOutputDir->text().isEmpty())
		defaultDir = ui.txtOutputDir->text();
	if (!ui.chkProcLinks->isChecked())
		dirPath = QFileDialog::getExistingDirectory(this, tr("Add a Folder"), defaultDir, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	else
		dirPath = QFileDialog::getExistingDirectory(this, tr("Add a Folder"), defaultDir, QFileDialog::ShowDirsOnly);
	if (!dirPath.isEmpty())
	{
		dirPath = QDir::toNativeSeparators(dirPath);
		if (ui.lwFileListA->findItems(dirPath, Qt::MatchFlags()).isEmpty())
			ui.lwFileListA->addItem(dirPath);
	}
	defaultDirSet = true;
	ui.statusBar->clearMessage();
}

/*!
* Add a folder to list B.
*/
void GMainWindow::on_btnAddFolderB_clicked()
{ 
    ui.statusBar->showMessage(tr("Loading file browser..."));

	QString dirPath, defaultDir = "";
	if (!defaultDirSet && !ui.txtOutputDir->text().isEmpty())
		defaultDir = ui.txtOutputDir->text();
	dirPath = QFileDialog::getExistingDirectory(this,
		tr("Add a Folder"), defaultDir, QFileDialog::ShowDirsOnly);
	if (!dirPath.isEmpty())
	{
		if (!dirPath.isEmpty())
		{
			dirPath = QDir::toNativeSeparators(dirPath);
			if (ui.lwFileListB->findItems(dirPath, Qt::MatchFlags()).isEmpty())
				ui.lwFileListB->addItem(dirPath);
		}
	}
	defaultDirSet = true;
	ui.statusBar->clearMessage();
}

/*!
* Loads a custom context menu for list A.
*
* \param pos location of mouse selection
*/
void GMainWindow::listACustomContextMenuRequested(const QPoint &pos)
{
	QMenu menu;
    QAction *aItem = 0;
    aItem = menu.addAction("Remove All Files/Folders", this, SLOT(removeAllFilesA()));
	menu.exec(this->mapToGlobal(pos));
    //Warning fix 11.25.16
    (void)aItem;
}

/*!
* Loads a custom context menu for list B.
*
* \param pos location of mouse selection
*/
void GMainWindow::listBCustomContextMenuRequested(const QPoint &pos)
{
	QMenu menu;
    QAction *aItem = 0;
    aItem = menu.addAction("Remove All Files/Folders", this, SLOT(removeAllFilesB()));
	menu.exec(this->mapToGlobal(pos));

    //Warning fix
    (void)aItem;
}

/*!
* Remove all file/folders from list A.
*/
void GMainWindow::removeAllFilesA()
{
	for (int i = ui.lwFileListA->count() - 1; i >= 0; i--)
		delete(ui.lwFileListA->takeItem(i));
}

/*!
* Remove all file/folders from list B.
*/
void GMainWindow::removeAllFilesB()
{
	for (int i = ui.lwFileListB->count() - 1; i >= 0; i--)
		delete(ui.lwFileListB->takeItem(i));
}

/*!
* Clear selected extensions.
*/
void GMainWindow::on_btnClearSelExt_clicked()
{
	ui.lwExtensionList->clearSelection();
}

/*!
* Process check boxes.
*/
void GMainWindow::on_chkDifferencing_clicked()
{
    if (ui.chkDifferencing->isChecked())
	{
		ui.lblFilesA->setText("File Set A to Differentiate:");
		ui.lblFilesB->show();
		ui.lwFileListB->show();
        ui.splitter->setSizes(QList<int>() << 200 << 200 << 100);

		ui.lblAddFileB->show();
		ui.lblAddFolderB->show();
		ui.lblRemoveFileB->show();
		ui.btnAddFileB->show();
		ui.btnRemoveFileB->show();
		ui.btnAddFolderB->show();

		// enable the modified threshold check box
		ui.chkModThreshold->setEnabled(true);
                // Modification: 2017.02
                //enable the function differencing checkbox
                ui.chkFuncDifferencing->setEnabled(true);
	}
	else
	{
		ui.lblFilesA->setText("Files/Folders To Count:");
		ui.lblFilesB->hide();
		ui.lwFileListB->hide();
		ui.splitter->setSizes(QList<int>() << 400 << 0 << 100);

		ui.lblAddFileB->hide();
		ui.lblRemoveFileB->hide();
		ui.lblAddFolderB->hide();
		ui.btnAddFileB->hide();
		ui.btnRemoveFileB->hide();
		ui.btnAddFolderB->hide();

		// disable the modified threshold check box
		ui.chkModThreshold->setEnabled(false);
                // Modification: 2017.02
                //disable the function differencing checkbox
                ui.chkFuncDifferencing->setEnabled(false);
	}
}

/*!
* Process modified threshold check box.
*/
void GMainWindow::on_chkModThreshold_clicked()
{
	ui.txtModThreshold->setEnabled(ui.chkModThreshold->isChecked());
}

/*!
* Process duplicate threshold check box.
*/
void GMainWindow::on_chkDupThreshold_clicked()
{
	ui.txtDupThreshold->setEnabled(ui.chkDupThreshold->isChecked());
}

/*!
* Process line truncation check box.
*/
void GMainWindow::on_chkTruncThreshold_clicked()
{
	ui.txtTruncThreshold->setEnabled(ui.chkTruncThreshold->isChecked());
}

void GMainWindow::on_chkRamLimit_clicked()
{
        ui.txtRamLimit->setEnabled(ui.chkRamLimit->isChecked());
}

void GMainWindow::on_chkThreads_clicked()
{
    ui.txtThreads->setEnabled(ui.chkThreads->isChecked());
}

/*!
* Prompt user for an output directory.
*/
void GMainWindow::on_btnBrowseOutputDir_clicked()
{
	ui.statusBar->showMessage(tr("Loading file browser..."));

	QString outputDir = ui.txtOutputDir->text();
	outputDir = QFileDialog::getExistingDirectory(this, tr("Select Output Directory"), outputDir, QFileDialog::ShowDirsOnly);
	if (!outputDir.isEmpty())
		ui.txtOutputDir->setText(QDir::toNativeSeparators(outputDir));
	defaultDirSet = true;
	ui.statusBar->clearMessage();
}

/*!
* Process extension file check box.
*/
void GMainWindow::on_chkExtensionFile_clicked()
{
    if (ui.chkExtensionFile->isChecked())
	{
		ui.txtExtensionFile->setEnabled(true);
		ui.btnBrowseExtensionFile->setEnabled(true);

		if (ui.txtExtensionFile->text().isEmpty())
			this->on_btnBrowseExtensionFile_clicked();
    }
	else
	{
                ui.txtExtensionFile->setText("");
		ui.txtExtensionFile->setEnabled(false);
		ui.btnBrowseExtensionFile->setEnabled(false);
	}
}

//Modification: 2018.04 USC starts 
void GMainWindow::on_createNewFile_clicked()
{  
    QString extensionFile="extensions.txt";
    ui.statusBar->showMessage("Creating a new extensions File..");
    extensionFile = QFileDialog::getSaveFileName(this,
        tr("Extension File"), extensionFile, tr("Extension File (*.txt)"), 0, QFileDialog::DontConfirmOverwrite);
    if (extensionFile=="")
    {
        ui.chkExtensionFile->setChecked(false);
        return;
    }
    QFileInfo fi(extensionFile);

    if(fi.exists())
    {
         QMessageBox::warning(this, tr("File Error"), tr("Extension File Already exists") + extensionFile);
    }
    else
    {QFile file(extensionFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
    QMessageBox::warning(this, tr("File Error"), tr("Unable to open extension file: ") + extensionFile + "\n" + file.errorString() );
    ui.statusBar->clearMessage();
    return;
    }
    file.close();
//    QFileInfo fi(extensionFile);
    if (!fi.isWritable())
        QMessageBox::warning(this, tr("File Error"), tr("Unable to write to extension file: ") + extensionFile);
    else
    {
        ui.txtExtensionFile->setText(QDir::toNativeSeparators(extensionFile));
        this->checkButtonClicked = true;
        this->parseExtensionsFile();
        this->on_actionEdit_Extensions_triggered();
    }
    }

}

void GMainWindow::on_chooseExisting()
{   QString extensionFile="extensions.txt";
    ui.statusBar->showMessage("Opening an existing extension file...");
    extensionFile = QFileDialog::getOpenFileName(this,
                                                 tr("Extension File"), extensionFile, tr("Extension File (*.txt)"), 0, QFileDialog::DontConfirmOverwrite);
    if (extensionFile==""){
        ui.chkExtensionFile->setChecked(false);
        return;
    }
    QFileInfo fi(extensionFile);
    if (fi.exists())
    {QFileInfo fi(extensionFile);
    if (!fi.isWritable())
        QMessageBox::warning(this, tr("File Error"), tr("Unable to write to extension file: ") + extensionFile);
    else
    {
        ui.txtExtensionFile->setText(QDir::toNativeSeparators(extensionFile));
        this->parseExtensionsFile();
        this->checkButtonClicked = false;
        this->on_actionEdit_Extensions_triggered();
    }}

    //if(!fi.exists()){
    //    QMessageBox::warning(this, tr("File Error"), tr("Extension File Not Found") + extensionFile);
    //} 

}

//Modification 2018.04 ends 

/*!
* Prompt user for an extension file.
*/
void GMainWindow::on_btnBrowseExtensionFile_clicked()
{
	ui.statusBar->showMessage("Loading file browser...");

//2018.04 Modification starts
        QMessageBox msgBox;
    msgBox.setText(tr("Choose New to create a new extension File and Existing to choose an Extension file already present."));
   msgBox.setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
    msgBox.setWindowFlags(windowFlags() | Qt::WindowCloseButtonHint);
    QPushButton* pButtonNew = msgBox.addButton(tr("New"), QMessageBox::ActionRole);
    QPushButton* pButtonExist = msgBox.addButton(tr("Existing"), QMessageBox::ActionRole);

    QPushButton* pButtonAbort = msgBox.addButton( QMessageBox::Abort);
    //QAbstractButton* pButtonNew = msgBox.addButton(tr("New"), QMessageBox::AcceptRole);
    //QAbstractButton* pButtonExist = msgBox.addButton(tr("Existing"), QMessageBox::AcceptRole);
    //QAbstractButton* pButtonAbort = msgBox.addButton(tr("Cancel"), QMessageBox::RejectRole);


    msgBox.exec();
    //int i = QMessageBox::question(this, tr("Create File?"), tr("If you want to create a new file click YES else click NO for existing file"));
    if (msgBox.clickedButton()==pButtonNew)
    {
        this->on_createNewFile_clicked();
    }
    if(msgBox.clickedButton()==pButtonExist){
        this->on_chooseExisting();
    }
    if(msgBox.clickedButton()==pButtonAbort){
        ui.chkExtensionFile->setChecked(false);
    }

    //     QString extensionFile = ui.txtExtensionFile->text();
//     if (extensionFile.isEmpty())
//             extensionFile = "extensions.txt";
//     extensionFile = QFileDialog::getSaveFileName(this,
//             tr("Extension File"), extensionFile, tr("Extension File (*.txt)"), 0, QFileDialog::DontConfirmOverwrite);
//     defaultDirSet = true;
//     if (!extensionFile.isEmpty())
//     {
//             QFileInfo fi(extensionFile);
//             if (!fi.exists())
//             {
//                     int i = QMessageBox::question(this, tr("Create File?"), tr("Specified extension file does not exist.\n Do you want to create it?"));
//                     if (i != QMessageBox::Yes)
//                     {
//                             ui.statusBar->clearMessage();
//                             return;
//                     }
//                     QFile file(extensionFile);
//                     if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
//                     {
//                QMessageBox::warning(this, tr("File Error"), tr("Unable to open extension file: ") + extensionFile + "\n" + file.errorString() );
//                             ui.statusBar->clearMessage();
//                             return;
//                     }
//                     file.close();
//             }
//             if (!fi.isWritable())
//                     QMessageBox::warning(this, tr("File Error"), tr("Unable to write to extension file: ") + extensionFile);
//             else
//             {
//                     ui.txtExtensionFile->setText(QDir::toNativeSeparators(extensionFile));
//                     this->parseExtensionsFile();
//                     this->on_actionEdit_Extensions_triggered();
//             }
//     }
//     ui.statusBar->clearMessage();
 
//2018.04 Modification ends
}

/*!
* Get the file extension filters.
*/
QStringList GMainWindow::getFilters()
{
	QListWidgetItem *lwItem;
	QStringList filters, *extensions;
	int i;

	QList<QListWidgetItem *> lwItems = ui.lwExtensionList->selectedItems();
	foreach (lwItem, lwItems)
	{
		if (extensionMapCustom.contains(lwItem->text()))
		{
			extensions = extensionMapCustom.value(lwItem->text());
			for (i = 0; i < extensions->count(); i++)
				filters << "*" + extensions->at(i);
		}
		else if (extensionMapDefault.contains(lwItem->text()))
		{
			extensions = extensionMapDefault.value(lwItem->text());
			for (i = 0; i < extensions->count(); i++)
				filters << "*" + extensions->at(i);
		}
	}
	return(filters);
}

/*!
* Retrieve the default language counter extensions from UCC.
*/
void GMainWindow::getDefaultExtensions()
{
	QStringList *extList;
	QString langName, ext;
    multimap<const string, string> lang_ext_map;

	MainObject *mainObject = new MainObject();
	mainObject->GetLanguageExtensionMap(&lang_ext_map);

    for (multimap<const string, string>::const_iterator iter = lang_ext_map.begin(); iter != lang_ext_map.end(); ++iter)
	{
		langName = QString::fromStdString(iter->first);
		ext = QString::fromStdString(iter->second);
		if (extensionMapDefault.contains(langName))
		{
			extList = extensionMapDefault.value(langName);
			if (extList)
				extList->append(ext);
		}
		else if (!ext.startsWith(".*"))
		{
			extList = new QStringList();
			if (!ext.isEmpty())
				extList->append(ext);
			extensionMapDefault.insert(langName, extList);
		}
	}
	delete(mainObject);
}

/*!
* Refresh the extension list based on the default list.
*/
void GMainWindow::extensionsUpdated()
{
	QString langName;

	ui.lwExtensionList->clear();
	foreach (langName, extensionMapDefault.keys())
		ui.lwExtensionList->addItem(langName);
}

/*!
* Refresh the custom extensions list from the file.
*
* \return status
*/
bool GMainWindow::parseExtensionsFile()
{
	if (ui.txtExtensionFile->text().isEmpty())
		return(false);

	QFile file(ui.txtExtensionFile->text());
	if (!file.open(QIODevice::ReadOnly))
	{
        QMessageBox::warning(this, tr("File Error"), tr("Unable to open the extension file: ") + ui.txtExtensionFile->text() + "\n" + file.errorString() );
		return(false);
	}

	QTextStream ts(&file);
	QStringList langAndExtensions, extensions, *extList;
	QString line, langName, ext;
	int i, n, n1, n2;
	bool inComment = false;

	QList<QStringList *> extLists = extensionMapCustom.values();
	for (i = extLists.count() - 1; i >= 0; i--)
		delete(extLists.takeAt(i));
	extensionMapCustom.clear();

	while (ts.status() == QTextStream::Ok && !ts.atEnd())
	{
		line = ts.readLine().simplified();
		if (!line.isEmpty())
		{
			// strip comments
			if (inComment)
			{
				n = line.indexOf(']');
				if (n >= 0)
				{
					line = line.right(line.length() - (n + 1)).simplified();
					inComment = false;
				}
			}
			n1 = line.indexOf('[');
			while (n1 >= 0)
			{
				n2 = line.indexOf(']');
				if (n2 >= 0)
				{
					line = line.right(line.length() - (n2 + 1)).simplified();
					inComment = false;
				}
				else
				{
					line = line.left(n1).simplified();
					inComment = true;
				}
				n1 = line.indexOf('[');
			}
			if (line.count('=') == 1)
			{
				// get language and extensions
				langAndExtensions = line.split('=');
				langName = langAndExtensions.first();
				extensions = langAndExtensions.at(1).split(',', QString::SkipEmptyParts);
				if (extensions.count() > 0)
				{
					if (!extensionMapCustom.contains(langName))
					{
						extList = new QStringList();
						for (i = 0; i < extensions.count(); i++)
						{
							ext = extensions.at(i).simplified();
							if (ext.startsWith('.'))
								extList->append(ext);
						}
						extensionMapCustom.insert(langName, extList);
					}
				}
				langAndExtensions.clear();
				extensions.clear();
			}
		}
	}
	file.close();

	return(true);
}

/*!
* Writes custom extensions list to the file.
*
* \return status
*/
bool GMainWindow::writeExtensionsFile()
{
	if (ui.txtExtensionFile->text().isEmpty())
		return(false);

	QFile file(ui.txtExtensionFile->text());
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
        QMessageBox::warning(this, tr("File Error"), tr("Unable to open the extension file: ") + ui.txtExtensionFile->text() + "\n" + file.errorString() );
		return(false);
	}

	QTextStream ts(&file);
	QStringList *extList;
	int i;
	QMapIterator<QString, QStringList *> it(extensionMapCustom);
	while (it.hasNext())
	{
		it.next();
		extList = it.value();
		if (extList && extList->count() > 0)
		{
			ts << it.key() << "=" << extList->first();
			for (i = 1; i < extList->count(); i++)
				ts << "," << extList->at(i);
			ts << endl;
		}
	}
	file.close();

	return(true);
}

/*!
* Parses the user preferences file.
*
* \return status
*/
bool GMainWindow::parsePreferencesFile()
{
	QFile file(QDir::homePath() + "/.gucc_prefs.txt");
	if (!file.open(QIODevice::ReadOnly))
		return(false);
	else
	{
		QTextStream ts(&file);
		QString line, key, val;
		QFileInfo fi;
		bool b, ok;
		int i; 

		while (!ts.atEnd())
		{
			line = ts.readLine();
			if (!line.isEmpty())
			{
				key = line.section("=", 0, 0).simplified();
				val = line.section("=", 1).simplified();
				if (!key.isEmpty() && !val.isEmpty())
				{
                    if (key == "Threads")
                    {
                        i = val.toInt(&ok);
                        if (ok)
                        {
                            ui.chkThreads->setChecked(true);
                            ui.txtThreads->setText(QString::number(i));
                        }
                    }
                    else if (key == "RAM Limit")
                    {
                        i = val.toInt(&ok);
                        if (ok)
                        {
                            ui.chkRamLimit->setChecked(true);
                            ui.txtRamLimit->setText(QString::number(i));
                        }
                    }
                    else if (key == "Output Directory")
					{
						fi.setFile(val);
						if (fi.isDir())
							ui.txtOutputDir->setText(val);
					}
					else if (key == "Extension File")
					{
						fi.setFile(val);
						if (fi.exists())
						{
							ui.chkExtensionFile->setChecked(true);
							ui.txtExtensionFile->setText(val);
							ui.txtExtensionFile->setEnabled(true);
							ui.btnBrowseExtensionFile->setEnabled(true);
							this->parseExtensionsFile();
						}
					}
					else if (key == "Differencing")
					{
						b = val.toInt(&ok);
						if (ok)
						{
							ui.chkDifferencing->setChecked(b);
							ui.chkModThreshold->setEnabled(b);
							if (b)
								this->on_chkDifferencing_clicked();
						}
					}
					else if (key == "Modified Threshold")
					{
						i = val.toInt(&ok);
						if (ok)
						{
							ui.chkModThreshold->setChecked(true);
							ui.txtModThreshold->setText(QString::number(i));
						}
					}
					else if (key == "Duplicate Threshold")
					{
						i = val.toInt(&ok);
						if (ok)
						{
							ui.chkDupThreshold->setChecked(true);
							ui.txtDupThreshold->setEnabled(true);
							ui.txtDupThreshold->setText(QString::number(i));
						}
					}
					else if (key == "Line Truncate")
					{
						i = val.toInt(&ok);
						if (ok)
						{
							ui.chkTruncThreshold->setChecked(true);
							ui.txtTruncThreshold->setEnabled(true);
							ui.txtTruncThreshold->setText(QString::number(i));
						}
					}
					else if (key == "Output Format")
					{
						if (val == "ASCII")
							ui.rdoAsciiOutput->setChecked(true);
						else if (val == "Legacy")
							ui.rdoLegacyOutput->setChecked(true);
						else
							ui.rdoCSVOutput->setChecked(true);
					}
					else if (key == "Unified Output")
					{
						b = val.toInt(&ok);
						if (ok)
							ui.chkUnifiedOut->setChecked(b);
					}
					else if (key == "Clear Case Files")
					{
						b = val.toInt(&ok);
						if (ok)
							ui.chkClearCaseFiles->setChecked(b);
					}
					else if (key == "Process Duplicates")
					{
						b = val.toInt(&ok);
						if (ok)
							ui.chkProcDuplicates->setChecked(b);
					}
					else if (key == "Process Complexity")
					{
						b = val.toInt(&ok);
						if (ok)
							ui.chkProcComplexity->setChecked(b);
					}
                    else if (key == "CC4 Enable")
                    {
                        b = val.toInt(&ok);
                        if (ok)
                            ui.chkCc4Enable->setChecked(b);
                    }
                #if !defined(Q_OS_WIN)
					else if (key == "Follow Links")
					{
						b = val.toInt(&ok);
						if (ok)
							ui.chkProcLinks->setChecked(b);
					}
                #endif
				}
			}
		}
		file.close();
	}
	return(true);
}

/*!
* Writes the user preferences file.
*
* \return status
*/
bool GMainWindow::writePreferencesFile()
{
	QFile file(QDir::homePath() + "/.gucc_prefs.txt");
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return(false);
	else
	{
		QTextStream ts(&file);
        if (!ui.chkThreads->isChecked())
            ts << "Threads = " << ui.txtThreads->text().simplified() << endl;
        if (!ui.chkRamLimit->isChecked())
            ts << "RAM Limit = " << ui.txtRamLimit->text().simplified() << endl;
		if (!ui.txtOutputDir->text().isEmpty())
		{
			QFileInfo fi(ui.txtOutputDir->text());
			if (fi.isDir())
				ts << "Output Directory = " << ui.txtOutputDir->text().trimmed() << endl;
		}
		if (ui.chkExtensionFile->isChecked() && !ui.txtExtensionFile->text().trimmed().isEmpty())
		{
			QFileInfo fi(ui.txtExtensionFile->text());
			if (fi.isReadable())
				ts << "Extension File = " << ui.txtExtensionFile->text().trimmed() << endl;
		}
		if (ui.chkDifferencing->isChecked())
		{
			ts << "Differencing = 1" << endl;
			if (ui.chkModThreshold->isChecked())
				ts << "Modified Threshold = " << ui.txtModThreshold->text().simplified() << endl;
		}
		if (ui.chkDupThreshold->isChecked())
			ts << "Duplicate Threshold = " << ui.txtDupThreshold->text().simplified() << endl;
		if (ui.chkTruncThreshold->isChecked())
			ts << "Line Truncate = " << ui.txtTruncThreshold->text().simplified() << endl;
		if (bgrpOutputFormat.checkedId() == 1)
			ts << "Output Format = ASCII" << endl;
		else if (bgrpOutputFormat.checkedId() == 2)
			ts << "Output Format = Legacy" << endl;
		else
			ts << "Output Format = CSV" << endl;
		if (ui.chkUnifiedOut->isChecked())
			ts << "Unified Output = 1" << endl;
		if (ui.chkClearCaseFiles->isChecked())
			ts << "Clear Case Files = 1" << endl;
		if (!ui.chkProcDuplicates->isChecked())
			ts << "Process Duplicates = 0" << endl;
		if (!ui.chkProcComplexity->isChecked())
        {
			ts << "Process Complexity = 0" << endl;
            ts << "CC4 Enable = 0" << endl;
        }
        else
        {
            // Only if Complexity checking is done
            ts << "Process Complexity = 1" << endl;
            if (ui.chkCc4Enable->isChecked())
                ts << "CC4 Enable = 1" << endl;
        }
#if !defined(Q_OS_WIN)
		if (!ui.chkProcLinks->isChecked())
			ts << "Follow Links = 1" << endl;
#endif

		file.close();
	}
	return(true);
}

void GMainWindow::dropEvent(QDropEvent *ev)
{
    QList<QUrl> urls = ev->mimeData()->urls();

    // Reading the first widget's span. Currently, we have only
    // three widgets: 1. for list A 2. for list B 3. for filter extensions.
    // So, if the drop location is not in widget related to A, then it belongs
    // to list B.
    QWidget* currDropWidget = ui.splitter->widget(0);
    QRect currRect;
    int topLeftX = 0;
    int topLeftY = 0;
    int width = 0;
    int height = 0;
    QPoint point = ev->pos();

    // Get rectangle properties for first widget.
    currRect = currDropWidget->geometry();
    currRect.getRect(&topLeftX, &topLeftY, &width, &height);

    foreach(QUrl url, urls)
    {
        // Logic is to get the widget information using
        //  QWidget * QSplitter::widget(int index) const.
        // Each widget has a geometry() API which returns QRect using which
        // we can get starting and ending positions of that widget.
        // Using this information we compare it with ev->pos->x to check whether
        // a drop location is inside widget or not.
        // Note: all this should be done if differencing is checked. Else, we can just add
        // the files/directories to list A.
        if (ui.chkDifferencing->isChecked())
        {
            if (point.x() > topLeftX && point.x() < topLeftX + width)
            {
                ui.lwFileListA->addItem(url.toLocalFile());
            }
            else
            {
                ui.lwFileListB->addItem(url.toLocalFile());
            }
        }
        else // case for counting
        {
            ui.lwFileListA->addItem(url.toLocalFile());
        }
    }
}

void GMainWindow::dragEnterEvent(QDragEnterEvent *ev)
{
    if (ev->mimeData()->hasUrls())
    {
        ev->acceptProposedAction();
    }
}

/*
    Oct 22 2016
    button used for choosing custom header text file
*/
void GMainWindow::on_btnCustomHeader_clicked()
{
    ui.statusBar->showMessage(tr("Loading file browser..."));
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open File"), "", tr("Text File (*.txt)"));
    userHeaderFile = fileName.toUtf8().constData();
    ui.txtHeader->setText(fileName);
}

/*
    Oct 22 2016
    check box for no header
*/

void GMainWindow::on_cbxNoHeader_clicked()
{
    if (ui.cbxNoHeader->checkState() == Qt::Checked){
        ui.btnCustomHeader->setEnabled(false);
        ui.txtHeader->setEnabled(false);
    } else {
        ui.btnCustomHeader->setEnabled(true);
        ui.txtHeader->setEnabled(true);
    }
    ui.txtHeader->setText(QString(""));
}

void GMainWindow::on_txtHeader_textChanged(const QString &arg1)
{
    QString fileName = ui.txtHeader->text();
    userHeaderFile = fileName.toUtf8().constData();
	(void)arg1; //Modification: 2018.04: Unused Variable

}

void GMainWindow::on_txtHeader_editingFinished()
{
    QString fileName = ui.txtHeader->text();
    QFileInfo check_file(fileName);
    if (!check_file.exists() || !check_file.isFile() || !fileName.endsWith(".txt")){
        QMessageBox msgBox;
        msgBox.setText("Invalid header text file path! No header is shown by default.");
        msgBox.exec();
    }
}


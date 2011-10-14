/* FreeEMS-Loader- the open source s19 loader with special features for FreeEMS
 *
 * Copyright (C) 2008-2011 by Sean Keys <skeys@powerefi.com>
 *
 * This file is part of the FreeEMS-Loader project.
 *
 * FreeEMS-Loader software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FreeEMS-Loader software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with any FreeEMS-Loader software.  If not, see http://www.gnu.org/licenses/
 *
 * We ask that if you make any changes to this file you email them upstream to
 * us at info(at)powerefi(dot)com or, even better, fork the code on github.com!
 *
 * Thank you for choosing FreeEMS-Loader to load your firmware! is not open
 *
 */

#include "inc/freeems_loader.h"
#include "string.h"
#include <new>
#include "inc/loaderTypes.h"
#include <string>
#include <algorithm>
#include <iostream>
#include "inc/about.h"

FreeEMS_Loader::FreeEMS_Loader(QWidget *parent) :
QWidget(parent), showHelp(false), fileArg(false), unattended(false) {
	ui.setupUi(this);
	qRegisterMetaType<string>("string");
	loaderComms = new FreeEMS_LoaderComms;
	fillBaud();
	fillDataBits();
	fillStopBits();
	fillParity();
	initGUI();
	//redirectCLI();
	loadFileName.clear();

	QObject::connect(loaderComms, SIGNAL( displayMessage(int, QString) ), this, SLOT( displayMessage(int, QString) ));
	QObject::connect(loaderComms, SIGNAL( setGUI(int) ), this, SLOT( changeGUIState(int) ));
	QObject::connect(loaderComms, SIGNAL( udProgress(int) ), this, SLOT( updateProgress(int) ));
	QObject::connect(loaderComms, SIGNAL( configureProgress(int, int) ), this, SLOT( configureProgress(int, int) ));

	/* restore settings */
	QSettings settings("FreeEMS", "Loader");
	resize(settings.value("size", QSize(400, 320)).toSize());
	move(settings.value("pos", QPoint(50, 50)).toPoint());
	ui.comboDevice->addItem(settings.value("serialDevice").toString());
	ui.comboDataBits->setCurrentIndex(settings.value("dataBits").toUInt());
	ui.comboStopBits->setCurrentIndex((settings.value("stopBits").toUInt()));
	ui.comboParity->setCurrentIndex((settings.value("parity").toUInt()));
	ui.chkRip->setChecked(settings.value("chkRip").toBool());
	ui.chkVerify->setChecked(settings.value("chkVerify").toBool());
	//loadFileName = settings.value("lastFileName").toString();
	loadDirectory = settings.value("lastDirectory").toString();

	/* process arguments */
	QString arg;
	cmdline_args = QCoreApplication::arguments();
	fillDevice();

	bool isFileName = false;
	bool isDevice = false;
	foreach(arg, cmdline_args)
	{
		if (isFileName) {
			loadFileName = arg;
			isFileName = false;
			displayMessage(USER_INFO, "using filename: " + loadFileName);
		} else if (isDevice) {
			isDevice = false;
			ui.comboDevice->clear();
			ui.comboDevice->addItem(arg);
			displayMessage(USER_INFO, "using device: " + arg);
		}
		if (arg.contains("--")) {
			if (arg.contains("--unattended")) //TODO use nested if and detect invalid args
			{
				cout << endl << "Argument: unattended connect and load ";
				unattended = true;
			} else if (arg.contains("--file")) {
				isFileName = true;
				fileArg = true;
			} else if (arg.contains("--verify")) {
				cout << endl << "Argument: Verify True ";
				ui.chkVerify->setChecked(true);
			} else if (arg.contains("--device")) {
				isDevice = true;
				cout << endl << "Argument: Serial Device Specified";
			} else if (arg.contains("--rip")) {
				cout << endl << "Argument: Rip Set True ";
				ui.chkRip->setChecked(true);
			} else if (arg.contains("--norip")) {
				cout << endl << "Argument: Rip Before Load Set False ";
				ui.chkRip->setChecked(false);
			} else if (arg.contains("--noverify")) {
				cout << endl << "Argument: Verify Set False ";
				ui.chkVerify->setChecked(false);
			} else if (arg.contains("--help")) {
				cout << endl
						<< "Help: Valid arguments are: --rip, --norip, --verify, --noverify, --file <path to file>, --device, --unattended"
						" --help";
				showHelp = true;
			} else {
				cout << endl << "Help: Invalid argument: " << arg.toStdString();
			}
		}
	}

	if (unattended) {
		connect();
		load();
	}
}

FreeEMS_Loader::~FreeEMS_Loader() {
	loaderComms->close();
	delete loaderComms;
	QSettings settings("FreeEMS", "Loader");
	settings.setValue("pos", pos());
	settings.setValue("size", size());
	settings.setValue("serialDevice", ui.comboDevice->currentText());
	settings.setValue("parity", ui.comboParity->currentIndex());
	settings.setValue("stopBits", ui.comboStopBits->currentIndex());
	settings.setValue("dataBits", ui.comboDataBits->currentIndex());
	settings.setValue("chkRip", ui.chkRip->isChecked());
	settings.setValue("chkVerify", ui.chkVerify->isChecked());
	settings.setValue("lastFileName", loadFileName);
}

int FreeEMS_Loader::fillDevice() {
#ifdef __linux__
	ui.comboDevice->addItem("/dev/ttyUSB0");
	QDir dir("/dev");
	QStringList filters;
	filters << "serial*"<<"tty*";
	dir.setNameFilters(filters);
	foreach ( QString file, dir.entryList(QDir::AllEntries) )
	{
		ui.comboDevice->addItem(file);
	}
#endif
#ifdef __WIN32__
	ui.comboDevice->addItem("COM1");
	//TODO scan the windows registry for valid ports
#endif

	return 0;
}

void FreeEMS_Loader::redirectCLI() {
	/*
	 StdRedirector<char>* coutRedirector = new StdRedirector<char> (std::cout,
	 outCallBack, ui.textOutput);
	 StdRedirector<char>* perrorRedirector = new StdRedirector <char>(&perror, outCallBack, ui.textOutput);
	 coutRedirector->compileWarning();
	 int *compileWarning = &coutRedirector;
	 compileWarning++;
	 std::cout << "CLI output redirected";
	 */
}

void FreeEMS_Loader::outCallBack(const char* ptr, std::streamsize count, void* pTextBox) {
	(void) count;
	QTextBrowser* p = static_cast<QTextBrowser*>(pTextBox);
	p->append(ptr);
}

void FreeEMS_Loader::fillBaud() {
	//#ifdef B0
	ui.comboBaud->addItem("0");
	//#endif
	//#ifdef B50
	ui.comboBaud->addItem("50");
	//#endif
	//#ifdef B75
	ui.comboBaud->addItem("75");
	//#endif
	//#ifdef B110
	ui.comboBaud->addItem("110");
	//#endif
	//#ifdef B134
	ui.comboBaud->addItem("134");
	//#endif
	//#ifdef B150
	ui.comboBaud->addItem("150");
	//#endif
	//#ifdef B200
	ui.comboBaud->addItem("200");
	//#endif
	//#ifdef B300
	ui.comboBaud->addItem("300");
	//#endif
	//#ifdef B600
	ui.comboBaud->addItem("600");
	//#endif
	//#ifdef B1200
	ui.comboBaud->addItem("1200");
	//#endif
	//#ifdef B1800
	ui.comboBaud->addItem("1800");
	//#endif
	//#ifdef B2400
	ui.comboBaud->addItem("2400");
	//#endif
	//#ifdef B4800
	ui.comboBaud->addItem("4800");
	//#endif
	//#ifdef B7200
	ui.comboBaud->addItem("7200");
	//#endif
	//#ifdef B9600
	ui.comboBaud->addItem("9600");
	//#endif
	//#ifdef B14400
	ui.comboBaud->addItem("14400");
	//#endif
	//#ifdef B19200
	ui.comboBaud->addItem("19200");
	//#endif
	//#ifdef B28800
	ui.comboBaud->addItem("28800");
	//#endif
	//#ifdef B38400
	ui.comboBaud->addItem("38400");
	//#endif
	//#ifdef B57600
	ui.comboBaud->addItem("57600");
	//#endif
	//#ifdef B76800
	ui.comboBaud->addItem("76800");
	//#endif
	// this one is the default
	ui.comboBaud->addItem("115200");

	ui.comboBaud->setCurrentIndex(ui.comboBaud->count() - 1);
}

void FreeEMS_Loader::fillStopBits() {
	ui.comboStopBits->addItem("2");
	ui.comboStopBits->addItem("1.5");
	ui.comboStopBits->addItem("1");

	ui.comboStopBits->setCurrentIndex(ui.comboStopBits->count() - 1);
}

void FreeEMS_Loader::fillDataBits() {
	ui.comboDataBits->addItem("5");
	ui.comboDataBits->addItem("6");
	ui.comboDataBits->addItem("7");
	ui.comboDataBits->addItem("8");

	ui.comboDataBits->setCurrentIndex(ui.comboDataBits->count() - 1);
}

void FreeEMS_Loader::fillParity() {

	ui.comboParity->addItem("ODD");
	ui.comboParity->addItem("EVEN");
	ui.comboParity->addItem("MARK");
	ui.comboParity->addItem("SPACE");
	ui.comboParity->addItem("NONE");

	ui.comboDataBits->setCurrentIndex(ui.comboParity->count() - 1);
}

//TODO combine setGUIState code into this function
void FreeEMS_Loader::connect() {
	QString portName = ui.comboDevice->currentText();
	QFile file;
	file.setFileName(ui.comboDevice->currentText());
	bool fileIsOpen = 0;
	if (_loaderState == WORKING) {
		loaderComms->terminate(); //TODO terminate should be used with caution review the pitfalls
		changeGUIState(CONNECTED);
		return;
	}
	if (!loaderComms->isReady()) {
		//setFlashType();
		//serialConnection->close();
		fileIsOpen = file.open(QIODevice::ReadWrite); //TODO further try to detect the problem
		file.close();
		if (fileIsOpen) {
			loaderComms->open(portName, ui.comboBaud->currentText().toUInt());
			//sleep(1); // POSIX ONLY TODO port takes a second to init after the open function
			loaderComms->setSM();
			loaderComms->setFlashType(defFlashType);
			if (loaderComms->isReady() == true) {
				changeGUIState(CONNECTED);
			} else {
				loaderComms->close();
			}
		} else {
			displayMessage(USER_INFO, "Unable to open port " + ui.comboDevice->currentText());
		}
	} else {
		loaderComms->resetSM();
		//sleep(1); // POSIX only TODO
		loaderComms->close();
		loaderComms->isReady() ? changeGUIState(CONNECTED) : changeGUIState(NOTCONNECTED);
	}
}

void FreeEMS_Loader::rip() {
	ui.chkRip->setEnabled(0);
	ripFileName = QFileDialog::getSaveFileName(this, tr("Save s19 as"), QDir::currentPath(), tr("s19 (*.s19)"));
	if (ripFileName.isNull()) {
		cout << endl << "error opening file";
	}
	loaderComms->setRipFilename(ripFileName);
	loaderComms->setAction(EXECUTE_RIP);
	loaderComms->start();
}

void FreeEMS_Loader::getFileName(QString name) {
	name = QFileDialog::getSaveFileName(this, tr("Save s19 as"), QDir::currentPath(), tr("s19 (*.s19)"));
	if (!name.isNull()) {
		displayMessage(ERROR, "error opening file");
	}
}

void FreeEMS_Loader::initGUI() {
	ui.chkVerify->setChecked(true);
	ui.chkRip->setChecked(true);
	changeGUIState(NOTCONNECTED);
	ui.progressBar->setValue(0);
	ui.radioRX->setEnabled(false);
	ui.radioTX->setEnabled(false);
	//	ui.radFlashType->setChecked(1);
	//	ui.radFlashType->setDisabled(1);
}

void FreeEMS_Loader::changeGUIState(int state) {
	_loaderState = state;
	updateGUIState();
}

void FreeEMS_Loader::updateGUIState() {
	switch (_loaderState) {
	case NOTCONNECTED:
		ui.pushLoad->setEnabled(0);
		ui.pushRip->setEnabled(0);
		ui.pushConnect->setText("Connect");
		ui.pushErase->setEnabled(0);
		break;
	case CONNECTED:
		ui.pushLoad->setEnabled(1);
		ui.pushRip->setEnabled(1);
		ui.pushConnect->setText("Close/Rst");
		ui.pushErase->setEnabled(1);
		break;
	case WORKING:
		ui.pushConnect->setText("Abort");
		ui.pushLoad->setEnabled(0);
		ui.pushRip->setEnabled(0);
		ui.pushErase->setEnabled(0);
		break;
	case ERROR:
		//TODO stop com thread and close port
		//loaderComms->st
		//loaderComms->close();
		ui.pushLoad->setEnabled(0);
		ui.pushRip->setEnabled(0);
		ui.pushConnect->setText("Connect");
		ui.pushErase->setEnabled(0);
		break;
	default:
		break;
	}
}

void FreeEMS_Loader::test() {
	loaderComms->setAction(TEST);
	loaderComms->start();
	/*
	 unsigned long i;
	 if(loaderComms->isReady()){
	 for(i = 0;  ; i++){
	 loaderComms->write(&SMReturn, 1);
	 if(!loaderComms->verifyReturn() > 0){
	 break;
	 }
	 writeText("Wrote one byte and read three");
	 }
	 writeText("Serial stress test failed see cli for iteration number");
	 cout<<i<<endl;
	 }else {
	 writeText("Error: SM NOT READY");
	 }
	 */

	//serialConnection->setSM();
	//FreeEMS_LoaderThreads test(serialConnection, EXECUTE_ERASE);
	//test.run(serialConnection);
	//test.start();
	//heapThreads->start();
	//heapThreads->wait();
	//test.wait();
	//serialConnection->setLoadFilename();
	//  string test = "Yo MOFO you ready to get loaded or what?";
	//  writeText(test);
}

void FreeEMS_Loader::eraseFlash() {
	loaderComms->setAction(EXECUTE_ERASE);
	loaderComms->start();
}

void FreeEMS_Loader::load() {
	QDate date = QDate::currentDate();
	QTime time = QTime::currentTime();

	if (!fileArg) //if no file was specified from the cmdline open browser
	{
		loadFileName = QFileDialog::getOpenFileName(this, tr("Load s19 file"), loadDirectory, tr("s19 (*.s19)"));
	}
	if (loadFileName.isNull()) {
		writeText("no file selected");
		return;
	}

	if (ui.chkVerify->isChecked()) {
		loaderComms->verifyLastWrite = true; //todo make set function
		loaderComms->verifyACKs = true;
	} else {
		loaderComms->verifyLastWrite = false; //todo make set function
		loaderComms->verifyACKs = true; //todo make settable
	}
	loadDirectory = loadFileName;
	QString name = loadFileName.section('/', -1);
	ripFileName = QDir::currentPath();
	ripFileName += "/saved/";
	if (!QDir(ripFileName).exists(ripFileName)) {
		QDir(ripFileName).mkpath(ripFileName);
	}
	ripFileName += date.toString("MM-dd-yyyy-");
	ripFileName += time.toString("H-m-s-");
	ripFileName += name;
	writeText("Ripping as ");
	writeText(ripFileName.toStdString());
	loaderComms->setLoadFilename(loadFileName);
	loaderComms->setRipFilename(ripFileName);
	if (ui.chkRip->isChecked()) {
		loaderComms->setAction(EXECUTE_RIP_ERASE_LOAD);
		loaderComms->start();
	} else {
		loaderComms->setAction(EXECUTE_LOAD);
		loaderComms->start();
	}

	QSettings settings("FreeEMS", "Loader"); //TODO make saveSettings fuction
	settings.setValue("lastDirectory", loadDirectory);
	//serialConnection->loadDevice(); // calls load without a seperate thread
}

void FreeEMS_Loader::writeText(string message) {
	QString out = message.c_str();
	ui.textOutput->append(out);
}

void FreeEMS_Loader::updateProgress(int percent) {
	ui.progressBar->setValue(percent);
}

bool FreeEMS_Loader::isUnattended() {
	return unattended;
}

void FreeEMS_Loader::configureProgress(int min, int max) {
	ui.progressBar->setMinimum(min);
	ui.progressBar->setMaximum(max);
}

void FreeEMS_Loader::showAbout() {
	About about(this);
	about.exec();
}

void FreeEMS_Loader::closeReset() {

}

void FreeEMS_Loader::displayMessage(int MESSAGE_TYPE, QString message) //TODO add eumu
{
	switch (MESSAGE_TYPE) {
	case USER_INFO:
		cout << endl << "Info: " << message.toStdString();
		writeText(message.toStdString());
		break;
	case ARGUMENT:
		cout << endl << "Argument: " << message.toStdString();
	case GENERIC:
		cout << message.toStdString();
	default:
		cout << endl << "Error Unknown Message Type";
		break;

	}
}

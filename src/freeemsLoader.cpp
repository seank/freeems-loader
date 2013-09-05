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

#include <freeems_loader.h>
#include <globals.h>
#include <string.h>
#include <new>
#include <string>
#include <algorithm>
#include <iostream>
#include <about.h>
#include <externalData.h>

FreeEMS_Loader::FreeEMS_Loader(QWidget *parent) :
QWidget(parent), showHelp(false), fileArg(false), unattended(false), _numBurnsPerformed(0), m_fileLoaded(false) {
	ui.setupUi(this);
	qRegisterMetaType<string>("string");
	qRegisterMetaType<unsigned int>("MESSAGE_TYPE");
	this->setWindowTitle(QString("FreeEMS-Loader ") + externalData::gitHASHShort);
	ui.label_build_info->setText(externalData::gitHASHLong);
	ui.comboDevice->installEventFilter(this);

	loaderComms = new FreeEMS_LoaderComms;
	fillBaud();
	fillDataBits();
	fillStopBits();
	fillParity();
	initGUI();
	//redirectCLI();
	loadFileName.clear();

	QObject::connect(loaderComms, SIGNAL( displayMessage(MESSAGE_TYPE, QString) ), this, SLOT( displayMessage(MESSAGE_TYPE, QString) ));
	QObject::connect(loaderComms, SIGNAL( setGUI(int) ), this, SLOT( changeGUIState(int) ));
	QObject::connect(loaderComms, SIGNAL( udProgress(int) ), this, SLOT( updateProgress(int) ));
	QObject::connect(loaderComms, SIGNAL( configureProgress(int, int) ), this, SLOT( configureProgress(int, int) ));

	fillDevice();
	restoreSettings();
	QString loadsNum;
	loadsNum.setNum(_numBurnsPerformed, 10);
	displayMessage(MESSAGE_INFO, "number of burns executed " + loadsNum);

	/* process arguments */
	QString arg;
	cmdline_args = QCoreApplication::arguments();
		bool isFileName = false;
	bool isDevice = false;
	foreach(arg, cmdline_args)
	{
		if (isFileName) {
			loadFileName = arg;
			isFileName = false;
			displayMessage(MESSAGE_INFO, "using filename: " + loadFileName);
		} else if (isDevice) {
			isDevice = false;
			ui.comboDevice->clear();
			ui.comboDevice->addItem(arg);
			displayMessage(MESSAGE_INFO, "using device: " + arg);
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
	cout << "TODO unattended mode";
		//	connect();
	//	load();
	}
}

FreeEMS_Loader::~FreeEMS_Loader() {
	qDebug() << "called FreeEMS_Loader::~FreeEMS_Loader()";
	saveSettings();
//	loaderComms->close();
	if(loaderComms != NULL)
		delete loaderComms;
}

void FreeEMS_Loader::fillDevice() {
    //TODO move fillDevice function to comms class
    ui.comboDevice->clear();
	QStringList prefixes;
	QDir path("/dev");
#ifdef __linux__
	//TOOD consider full file info for permissions troubleshooting.
	prefixes += "ttyS*";
	prefixes += "ttyUSB*";
	QStringList list = path.entryList(prefixes, QDir::System);
	QStringList filteredList = list.filter(QRegExp("^ttyS[0-9]{1,1}$|^ttyUSB[0-9]$"));
#elif __APPLE__
	prefixes += "cu.*";
	prefixes += "tty.*";
	QStringList filteredList = path.entryList(prefixes, QDir::System);
#elif __WIN32__
	//TODO scan the windows registry for valid ports
	QStringList filteredList;
	filteredList += "COM1";
	filteredList += "COM2";
	filteredList += "COM3";
	filteredList += "COM4";
	filteredList += "COM5";
	filteredList += "COM6";
	filteredList += "COM7";
	filteredList += "COM8";
	filteredList += "COM9";
#endif
	//Ensure there are no duplicates by using a hashMap
	int i;
	for(i = 0; i < filteredList.size(); ++i) {
	#ifdef __WIN32__
		deviceMap.insert(filteredList[i], i);
	#else
		deviceMap.insert('/' + path.dirName() + '/' + filteredList[i], i);
	#endif
	}
	QMap<QString, int>::const_iterator iter;
	for(iter = deviceMap.constBegin(); iter != deviceMap.constEnd(); ++iter) {
		ui.comboDevice->addItem(iter.key());
	}

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

void FreeEMS_Loader::connect() {
	if (m_loaderState == STATE_WORKING) {
		abort();
	} else {
		if (ui.comboBaud->currentText().compare("115200") || ui.comboDataBits->currentText().compare("8")
				|| ui.comboParity->currentText().compare("NONE") || ui.comboStopBits->currentText().compare("1")) {
			displayMessage(MESSAGE_INFO, "Warning you are not using default COM port settings");
			displayMessage(MESSAGE_INFO, "Defaults are 115200, 8, NONE, 1");
		}
		loaderComms->setupPort(ui.comboDevice->currentText(), ui.comboBaud->currentText().toUInt(),
				ui.comboStopBits->currentText().toUInt(), ui.comboDataBits->currentText().toUInt(),
				ui.comboParity->currentText());
	}
}

void FreeEMS_Loader::rip() {
	connect();
	QSettings loaderSettings(settingsFile, QSettings::IniFormat);
	QString
	loadRipDirectory = loaderSettings.value("lastRipDirectory").toString();
	ui.chkRip->setEnabled(0);
	QFileDialog fileDialog(this, "Save s19 as");
	fileDialog.setNameFilter("s19 Files (*.s19)");
	fileDialog.setDirectory(loadRipDirectory);
	fileDialog.setDefaultSuffix("s19");
	fileDialog.setViewMode(QFileDialog::Detail);
	fileDialog.setFileMode(QFileDialog::AnyFile);
	fileDialog.setAcceptMode(QFileDialog::AcceptSave);
	fileDialog.exec();
	ripFileName = fileDialog.selectedFiles().first();
	if(!ripFileName.endsWith(".s19")) {
		displayMessage(MESSAGE_INFO, "no rip file name specified");
		return;
	}
	qDebug() << "Ripping flash as " << ripFileName;
	ripFileName = fileDialog.selectedFiles().first();
	loaderSettings.setValue("lastRipDirectory", ripFileName);
	loaderComms->setRipFilename(ripFileName);
	loaderComms->setAction("RIP");
	if (ui.chkRipSMCode->isChecked()) {
		loaderComms->ripSMCode(true);
	} else {
		loaderComms->ripSMCode(false);
	}
	loaderComms->start();
}

void FreeEMS_Loader::getFileName(QString name) {
	name = QFileDialog::getSaveFileName(this, tr("Save s19 as"), QDir::currentPath(), tr("s19 (*.s19)"));
	if (!name.isNull()) {
		displayMessage(MESSAGE_ERROR, "error opening file");
	}
}

void FreeEMS_Loader::initGUI() {
	changeGUIState(STATE_STANDING_BY);
	ui.progressBar->setValue(0);
	ui.radioRX->setEnabled(false);
	ui.radioTX->setEnabled(false);
	m_fileLoaded = false;
}

void FreeEMS_Loader::changeGUIState(int state) {
	m_loaderState = state;
	updateGUIState();
}

void FreeEMS_Loader::updateGUIState() {
	switch (m_loaderState) {
	case STATE_WORKING:
		ui.pushAbort->setEnabled(true);
		ui.pushLoad->setEnabled(false);
		ui.pushRip->setEnabled(false);
		ui.pushErase->setEnabled(false);
		ui.chkRip->setEnabled(false);
		ui.chkVerify->setEnabled(false);
		ui.pushOpenFile->setEnabled(false);
		break;
	case STATE_LOAD_COMPLETE:
		m_fileLoaded = false; // force user to reopoen or select new file
	case STATE_STANDING_BY: //same as connected except actions are cleared
		loaderComms->setAction("NONE"); //This may need a mutex
		ui.progressBar->setValue(0);
		ui.pushRip->setEnabled(true);
		ui.pushErase->setEnabled(true);
		ui.chkRip->setEnabled(true);
		ui.chkVerify->setEnabled(true);
		ui.pushOpenFile->setEnabled(true);
		ui.pushLoad->setEnabled(false);
		ui.pushAbort->setEnabled(false);
		break;
	default:
		displayMessage(MESSAGE_ERROR, "INVALID STATE REQUESTED, THIS IS A BUG");
		break;
	}
}

void FreeEMS_Loader::eraseFlash() {
	connect();
	QSettings loaderSettings(settingsFile, QSettings::IniFormat); //TODO this should be done will a call back to be proper
	_numBurnsPerformed++;
	loaderSettings.setValue("numBurnsPerformed", _numBurnsPerformed);
	loaderComms->setAction("ERASE");
	loaderComms->start();
}

void FreeEMS_Loader::load() {
	connect();
	QSettings loaderSettings(settingsFile, QSettings::IniFormat);
	_numBurnsPerformed++;
	loaderSettings.setValue("numBurnsPerformed", _numBurnsPerformed);
	QDate date = QDate::currentDate();
	QTime time = QTime::currentTime();
//	QFileDialog fileDialog;
//	fileDialog.setViewMode(QFileDialog::Detail);
//
//	if (!fileArg) //if no file was specified from the cmdline open browser
//	{
//		//loadFileName = QFileDialog::getOpenFileName(this, tr("Load s19 file"), loadDirectory, tr("s19 (*.s19)"));
//		loadFileName = fileDialog.getOpenFileName(this, tr("Load s19 file"), loadDirectory, tr("s19 (*.s19)"));
//	}
	if (!m_fileLoaded) {
		displayMessage(MESSAGE_INFO, "s19 records have not been loaded or there was a problem parsing the input file");
		return;
	}
	if (ui.chkVerify->isChecked()) {
		loaderComms->verifyLastWrite = true; //todo make set function
	} else {
		loaderComms->verifyLastWrite = false; //todo make set function
	}

	QString name = loadFileName.section('/', -1);
//	ripFileName = QDir::currentPath();
//	ripFileName += "/saved/";
	if (!QDir(m_autoRipDirectory).exists(m_autoRipDirectory))
		QDir(m_autoRipDirectory).mkpath(m_autoRipDirectory);

	m_autoRipDirectory += date.toString("MM-dd-yyyy-");
	m_autoRipDirectory += time.toString("H-m-s-");
	m_autoRipDirectory += "replaced-with-";
	m_autoRipDirectory += name;
	if(ui.chkRip->isChecked()){
		displayMessage(MESSAGE_INFO, "Ripping as '" + m_autoRipDirectory + "'");
	}
	loaderComms->setLoadFilename(loadFileName);
	loaderComms->setRipFilename(m_autoRipDirectory);
	if (ui.chkRip->isChecked()) {
		loaderComms->setAction("RIP");
		loaderComms->setAction("LOAD");
		loaderComms->start();
	} else {
		loaderComms->setAction("LOAD");
		loaderComms->start();
	}
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

void FreeEMS_Loader::displayMessage(MESSAGE_TYPE type, QString message) {
	switch (type) {
		case MESSAGE_INFO:
			cout << endl << "Info: " << message.toStdString();
			writeText(message.toStdString());
		break;
		case MESSAGE_ARGUMENT:
			cout << endl << "Argument: " << message.toStdString();
		break;
		case MESSAGE_GENERIC:
			cout << endl << message.toStdString();
		break;
		case MESSAGE_ERROR:
			writeText(message.toStdString());
			cout << endl << "Error: " << message.toStdString();
		break;
	default:
		cout << endl << "Error Unknown Message Type";
		break;
	}
}

void FreeEMS_Loader::openFile() {

	QSettings loaderSettings(settingsFile, QSettings::IniFormat);
	loadDirectory = loaderSettings.value("lastDirectory").toString();
	QFileDialog fileDialog;
	fileDialog.setViewMode(QFileDialog::Detail);
	fileDialog.restoreGeometry(loaderSettings.value("lastOpenDialogGeo").toByteArray());
	QString qSNum;
	loadFileName = fileDialog.getOpenFileName(this, tr("Load s19 file"), loadDirectory, tr("s19 (*.s19)"));
	loaderSettings.setValue("lastOpenDialogGeo", fileDialog.saveGeometry());
	if (loadFileName.isNull()) {
		displayMessage(MESSAGE_ERROR, "no file selected");
		return;
	} else{
		loaderSettings.setValue("lastDirectory", loadFileName);
		loaderComms->setLoadFilename(loadFileName);
		displayMessage(MESSAGE_INFO,"Attempting to parse " + loadFileName);
		loaderComms->parseFile();
		if(loaderComms->numLoadableRecords() == 0){
			displayMessage(MESSAGE_ERROR, "no load-able records parsed");
		}else if(loaderComms->numBadSums()){
			displayMessage(MESSAGE_ERROR, "there are " + qSNum.setNum(loaderComms->numBadSums(), 10) + " records with bad checksums or lengths , loading will be disabled");
		} else {
			displayMessage(MESSAGE_INFO,"found " + 	qSNum.setNum(loaderComms->numLoadableRecords(), 10) +" load-able records in file");
			ui.pushLoad->setEnabled(true);
			m_fileLoaded = true;
		}
	}
}

void FreeEMS_Loader::setAutoRipDir() {
	QSettings loaderSettings(settingsFile, QSettings::IniFormat);
	QFileDialog fileDialog;
	fileDialog.setViewMode(QFileDialog::Detail);
	QString qSNum;
	m_autoRipDirectory = QFileDialog::getExistingDirectory(this, "Set Auto Rip Directory", m_autoRipDirectory, QFileDialog::ShowDirsOnly);
#ifdef __WIN32__
	m_autoRipDirectory += "\\";
#else
	m_autoRipDirectory += "/";
#endif
	loaderSettings.setValue("autoRipDirectory", m_autoRipDirectory);
}

//<malcom2073> seank-work: https://github.com/seank/freeems-loader/blob/master/src/freeemsLoader.cpp#L514
//<diyefi-bot> freeems-loader/src/freeemsLoader.cpp at master · seank/freeems-loader · GitHub
//<malcom2073> unneccesary. Qt can handle a path: c:/Users/michael/test.txt

void FreeEMS_Loader::saveSettings() {
	QSettings loaderSettings(settingsFile, QSettings::IniFormat);

	loaderSettings.setValue("pos", pos());
	loaderSettings.setValue("size", size());
	loaderSettings.setValue("serialDevice", ui.comboDevice->currentText());
	loaderSettings.setValue("parity", ui.comboParity->currentIndex());
	loaderSettings.setValue("stopBits", ui.comboStopBits->currentIndex());
	loaderSettings.setValue("dataBits", ui.comboDataBits->currentIndex());
	loaderSettings.setValue("chkRip", ui.chkRip->isChecked());
	loaderSettings.setValue("chkVerify", ui.chkVerify->isChecked());
	loaderSettings.setValue("lastRipFileName", ripFileName);
	loaderSettings.setValue("numBurnsPerformed", _numBurnsPerformed);
	loaderSettings.setValue("ripSMCode", ui.chkRipSMCode->isChecked());
}

void FreeEMS_Loader::abort() {
	loaderBusy.lock();
	loaderAbort = true;
	loaderBusy.unlock();
}

bool FreeEMS_Loader::eventFilter(QObject *target, QEvent *event) {
	if ( event->type() == QEvent::MouseButtonPress && target->objectName() == "comboDevice" ) {
		fillDevice();
		//qDebug() << "caught event" << target->objectName();
		//return TRUE;
	}
	//qDebug() << "sending event to qwidget" << event->type();
	return QWidget::eventFilter(target, event);
}

int FreeEMS_Loader::restoreSettings() {
    //TODO move to seperate settings file and reduce the amount of code that is platform specific
    // in other words use platform speficic code so that other code can be reused

    /* restore settings */
#ifdef __WIN32__
		QString appDataDir = getenv("AppData");
		m_autoRipDirectory = appDataDir;
		m_autoRipDirectory += "\\FreeEMS-Loader\\";
		appDataDir = m_autoRipDirectory;
		if (!QDir(appDataDir).exists(appDataDir))
			QDir(appDataDir).mkpath(appDataDir);
		appDataDir += "settings\\";
		if (!QDir(appDataDir).exists(appDataDir))
			QDir(appDataDir).mkpath(appDataDir);
		m_autoRipDirectory += "rips\\";
		if (!QDir(m_autoRipDirectory).exists(m_autoRipDirectory))
			QDir(m_autoRipDirectory).mkpath(m_autoRipDirectory);
#else
		appDataDir = getenv("HOME");
		m_autoRipDirectory = appDataDir;
		appDataDir += "/.FreeEMS-Loader/";
		m_autoRipDirectory += "/FreeEMS-Loader/";
		if (!QDir(appDataDir).exists(appDataDir))
			QDir(appDataDir).mkpath(appDataDir);
		appDataDir += "settings/";
		if (!QDir(appDataDir).exists(appDataDir))
			QDir(appDataDir).mkpath(appDataDir);
		if (!QDir(m_autoRipDirectory).exists(m_autoRipDirectory))
			QDir(m_autoRipDirectory).mkpath(m_autoRipDirectory);
		m_autoRipDirectory += "rips/";
		if (!QDir(m_autoRipDirectory).exists(m_autoRipDirectory))
			QDir(m_autoRipDirectory).mkpath(m_autoRipDirectory);
#endif
	settingsFile = appDataDir + "settings.ini";
	qDebug() << "settings file is -> " << settingsFile;
	QSettings loaderSettings(settingsFile, QSettings::IniFormat);

	/* initialize file and dir locations */
	if (loaderSettings.value("autoRipDirectory").isNull()) {
		displayMessage(MESSAGE_INFO, "This appears to be the first time FreeEMS Loader has been "
				"run on this host, initializing settings..");
#ifdef __WIN32__
		QString appDataDir = getenv("AppData");
		m_autoRipDirectory = appDataDir;
		m_autoRipDirectory += "\\FreeEMS-Loader\\";
		appDataDir = m_autoRipDirectory;
		if (!QDir(appDataDir).exists(appDataDir))
			QDir(appDataDir).mkpath(appDataDir);
		appDataDir += "settings\\";
		if (!QDir(appDataDir).exists(appDataDir))
			QDir(appDataDir).mkpath(appDataDir);
		m_autoRipDirectory += "rips\\";
		if (!QDir(m_autoRipDirectory).exists(m_autoRipDirectory))
			QDir(m_autoRipDirectory).mkpath(m_autoRipDirectory);
#else
		appDataDir = getenv("HOME");
		m_autoRipDirectory = appDataDir;
		appDataDir += "/.FreeEMS-Loader/";
		m_autoRipDirectory += "/FreeEMS-Loader/";
		if (!QDir(appDataDir).exists(appDataDir))
			QDir(appDataDir).mkpath(appDataDir);
		appDataDir += "settings/";
		if (!QDir(appDataDir).exists(appDataDir))
			QDir(appDataDir).mkpath(appDataDir);

		if (!QDir(m_autoRipDirectory).exists(m_autoRipDirectory))
			QDir(m_autoRipDirectory).mkpath(m_autoRipDirectory);
		m_autoRipDirectory += "rips/";
		if (!QDir(m_autoRipDirectory).exists(m_autoRipDirectory))
			QDir(m_autoRipDirectory).mkpath(m_autoRipDirectory);
#endif
		ui.comboBaud->setCurrentIndex(21);
		ui.comboDataBits->setCurrentIndex(3);
		ui.comboParity->setCurrentIndex(4);
		ui.comboStopBits->setCurrentIndex(2);
		ui.chkRip->setChecked(true);
		ui.chkVerify->setChecked(true);
		ui.chkRipSMCode->setChecked(false);
		_numBurnsPerformed = 0;
	} else {
		int comboIndex = deviceMap[loaderSettings.value("serialDevice").toString()];
		ui.comboDevice->setCurrentIndex(comboIndex);
		ui.comboDataBits->setCurrentIndex(loaderSettings.value("dataBits").toUInt());
		ui.comboStopBits->setCurrentIndex((loaderSettings.value("stopBits").toUInt()));
		ui.comboParity->setCurrentIndex((loaderSettings.value("parity").toUInt()));
		ui.chkRip->setChecked(loaderSettings.value("chkRip").toBool());
		ui.chkVerify->setChecked(loaderSettings.value("chkVerify").toBool());
		_numBurnsPerformed = loaderSettings.value("numBurnsPerformed").toInt();
		m_autoRipDirectory = loaderSettings.value("autoRipDirectory").toString();
		loaderSettings.setValue("appDataDir", appDataDir);
		resize(loaderSettings.value("size", QSize(400, 320)).toSize());
		move(loaderSettings.value("pos", QPoint(50, 50)).toPoint());
		ui.chkRipSMCode->setChecked(loaderSettings.value("ripSMCode").toBool());
	}

	qDebug() << "Settings location is -> " << appDataDir;
	qDebug() << "Auto RIP directory is -> " << m_autoRipDirectory;
	return 1;
}

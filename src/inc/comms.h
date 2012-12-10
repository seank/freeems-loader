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
 * Thank you for choosing FreeEMS-Loader to load your firmware!
 *
 */

#ifndef COMMS_H
#define COMMS_H

#include <stdexcept>
#include <string>
#include <sstream>
#include <iostream>
#include <QString>
#include <QThread>
#include <QObject>
#include <QDebug>
#include <QFile>
#include <SerialIO.h>
#include <sRecord.h>

#include <QObject>

#define SM_READY_CHAR_SIZE      0x03
#define ONE_TWENTY_EIGHT_K_RECORDS     15000  //enough records to hold 4MB at 16bytes each
using namespace std;

enum SM_COMMAND_TYPE{
	GENERIC = 1, SETSM
};
/**
 * Thrown if timeout occurs
 */
class timeout_exception: public std::runtime_error

{
public:
	timeout_exception(const std::string& arg) :
		runtime_error(arg) {
	}
	// timeout_exception(const std::string& arg)  std::cout<<arg {}
};

class FreeEMS_LoaderComms: public QThread //public QObject
{
	Q_OBJECT
public:
	FreeEMS_LoaderComms();
	void abortOperation();
	int ripDevice();
	void openTest(QString serPortName);
	int  loadDevice();
	void init();
	void clearSets();
	bool generateRecords(vector<string> lineArray);
	bool lineIsLoadable(string* line);
	void initRecordSet(unsigned int numRecords);
//	void setThreadAction(QString action);
	void setRipFilename(QString name);
	void setLoadFilename(QString name);
	int numBadSums();
	void resetSM();
	int getDeviceByteCount();
	int loadRecordSet();
	int SMWriteByteBlock(unsigned int address, char* bytes, unsigned int numBytes);
	void returnFlashType(unsigned char *responce);
	void setAbort(bool abortOperation);
	void setFlashType(const char *commonName);
	void SMSetPPage(unsigned char PPage);
	int SMReadByteBlock(unsigned int address, unsigned int plusBytes, std::vector<unsigned char> &vec);
	void SMReadChars(const char *data, unsigned int size);
	void flushRXStream();
	void parseFile();
	int verifyReturn(SM_COMMAND_TYPE type);
	void open();
	bool isReady() const;
	bool isOpen();
	void close();
	void write(const unsigned char *data, unsigned int size);
	void write(const std::vector<unsigned char>& data);
	void write(const unsigned char *data);
	void writeString(const std::string& s);
	void test();
	void read(unsigned char *data, unsigned int size);
	std::vector<unsigned char> read(unsigned int size);
	std::string	readString(unsigned int size);
	std::string readStringUntil(const std::string& delim = "\n");
	int erasePage(char PPage);
	int eraseDevice();
	void setSM();
	void setDataMode(QString& mode);
	void setAction(QString action);
	bool isRecordSetLoaded();
	int numLoadableRecords();
	void connect();
	void disConnect();
	void setupPort(QString portName, unsigned int baud, unsigned int stopBits, unsigned int dataBits,
					QString parity);
	void run();
	bool verifyLastWrite;
	~FreeEMS_LoaderComms();

	signals:
	void displayMessage(MESSAGE_TYPE type, QString text); //see about moving this to a global header
	void udProgress(int percent);
	void configureProgress(int min, int max);
	void setGUI(int);


private:

	bool m_actionConnect;
	bool m_actionDisConnect;
	bool m_actionErase;
	bool m_actionLoad;
	bool m_actionRip;
	bool m_fDeviceIsSet;
	bool m_recordSetLoaded;
	bool m_smIsReady;

	unsigned int m_lastLoadAddress;
	unsigned int m_portBaud;
	unsigned int m_portDataBits;
	unsigned int m_portStopBits;

	int m_badCheckSums;
	int m_flashTypeIndex;
	int m_loadableRecords;
	int m_s19SetOneCount;
	int m_s19SetTwoCount;

	QString m_loadFilename;
	QString m_portName;
	QString m_portParity;
	QString m_ripFilename;

	FreeEMS_LoaderSREC *m_s19SetOne;
	FreeEMS_LoaderSREC *m_s19SetTwo;
	IPDS::SerialIO	*m_serPort;

	int writeBlocks();

};

#else
/* let us know if we are being untidy with headers */
#warning "Header file COMMS_H seen before, sort it out!"
/* end of the wrapper ifdef from the very top */
#endif

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

/**
 * Serial port class, with timeout on read operations.
 */
class FreeEMS_LoaderComms: public QThread //public QObject
{
	Q_OBJECT
public:
	FreeEMS_LoaderComms();
	/**
	 * Opens a serial device. By default timeout is disabled.
	 * serial device
	 */

	/*
	 *  B7/DC/IDID — Returns the constant $DC (Device C=12) and the 2-byte
	 *  HCS12 device ID register. Please refer to selected device guides for device ID
	 *  register contents.
	 */

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

	//void SMSetLoadAddress(unsigned int address, unsigned int typeID, int numBytes);

	/*
	 *  B7/DC/IDID — Returns the constant $DC (Device C=12) and the 2-byte
	 *  HCS12 device ID register. Please refer to selected device guides for device ID
	 *  register contents.
	 */
	void returnFlashType(unsigned char *responce);

	void setAbort(bool abortOperation);

	void setFlashType(const char *commonName);

	void SMSetPPage(unsigned char PPage);

	int SMReadByteBlock(unsigned int address, unsigned int plusBytes, std::vector<unsigned char> &vec);

	void SMReadChars(const char *data, unsigned int size);

	void flushRXStream();

	void parseFile();

	/*
	 * Read a block of memory starting at address specified.
	 * Block is read twice to check integrity.
	 * 256 bytes max
	 */

	int verifyReturn(SM_COMMAND_TYPE type);

	//bool
	//verifyReturn(std::vector<char> &vec);

	/**
	 * Opens a serial device.
	 */
	void open();

	/**
	 * \return true if serial device is open
	 */
	bool isReady() const;
	/**
	 * \return true if serial device is open
	 */
	bool isOpen();

	/**
	 * Close the serial device
	 * \throws boost::system::system_error if any error
	 */
	void close();

	/**
	 * Set the timeout on read/write operations.
	 * To disable the timeout, call setTimeout(boost::posix_time::seconds(0));
	 */
	//  void
	//  setTimeout(const boost::posix_time::time_duration& t); //TODO reimpliment
	/**
	 * Write data
	 * \param data array of char to be sent through the serial device
	 * \param size array size
	 * \throws boost::system::system_error if any error
	 */
	void write(const unsigned char *data, unsigned int size);

	/**
	 * Write data
	 * \param data to be sent through the serial device
	 * \throws boost::system::system_error if any error
	 */
	void write(const std::vector<unsigned char>& data);

	void write(const unsigned char *data);

	/**
	 * Write a string. Can be used to send ASCII data to the serial device.
	 * To send binary data, use write()
	 * \param s string to send
	 * \throws boost::system::system_error if any error
	 */
	void writeString(const std::string& s);

	void test();
	/**
	 * Read some data, blocking
	 * \param data array of char to be read through the serial device
	 * \param size array size
	 * \return numbr of character actually read 0<=return<=size
	 * \throws boost::system::system_error if any error
	 * \throws timeout_exception in case of timeout
	 */
//	void read(char *data, size_t size);

	void read(unsigned char *data, unsigned int size);
	/**
	 * Read some data, blocking
	 * \param size how much data to read
	 * \return the receive buffer. It iempty if no data is available
	 * \throws boost::system::system_error if any error
	 * \throws timeout_exception in case of timeout
	 */
	std::vector<unsigned char> read(unsigned int size);

	/**
	 * Read a string, blocking
	 * Can only be used if the user is sure that the serial device will not
	 * send binary data. For binary data read, use read()
	 * The returned string is empty if no data has arrived
	 * \param size hw much data to read
	 * \return a string with the received data.
	 * \throws boost::system::system_error if any error
	 * \throws timeout_exception in case of timeout
	 */
	std::string	readString(unsigned int size);

	/**
	 * Read a line, blocking
	 * Can only be used if the user is sure that the serial device will not
	 * send binary data. For binary data read, use read()
	 * The returned string is empty if the line delimiter has not yet arrived.
	 * \param delimiter line delimiter, default="\n"
	 * \return a string with the received data. The delimiter is removed from
	 * the string.
	 * \throws boost::system::system_error if any error
	 * \throws timeout_exception in case of timeout
	 */
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
	void displayMessage(MESSAGE_TYPE type, QString text);
	void udProgress(int percent);
	void configureProgress(int min, int max);
	void setGUI(int);


private:
	//FreeEMS_SerialPort *serPort;
	IPDS::SerialIO	*serPort;
	/**
	 * Possible outcome of a read. Set by callbacks, read from main code
	 */
	enum ReadResult {
		resultInProgress, resultSuccess, resultError, resultTimeoutExpired
	};
	enum ReadResult result; ///< Used by read with timeout
	unsigned int bytesTransferred; ///< Used by async read callback

	FreeEMS_LoaderSREC *s19SetOne;
	FreeEMS_LoaderSREC *s19SetTwo;

	int s19SetOneCount;
	int s19SetTwoCount;
	unsigned int lastLoadAddress;

	int writeBlocks();

	QString ripFilename;
	QString loadFilename;
	QString m_portName;
	QString m_portParity;
	unsigned int  m_portBaud;
	unsigned int  m_portDataBits;
	unsigned int  m_portStopBits;

	int flashTypeIndex;
	bool fDeviceIsSet;
	bool smIsReady;
	bool _recordSetLoaded;
	int _badCheckSums;
	int _loadableRecords;

	bool m_actionRip;
	bool m_actionErase;
	bool m_actionLoad;
	bool m_actionConnect;
	bool m_actionDisConnect;
};

#else
/* let us know if we are being untidy with headers */
#warning "Header file COMMS_H seen before, sort it out!"
/* end of the wrapper ifdef from the very top */
#endif

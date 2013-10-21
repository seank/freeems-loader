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

#include <comms.h>
#include <fstream>
#include <globals.h>

using namespace std;

FreeEMS_LoaderComms::FreeEMS_LoaderComms() :
				verifyLastWrite(false), m_fDeviceIsSet(false), m_smIsReady(false), m_flashTypeIndex(0) {
	init();
}

void FreeEMS_LoaderComms::openTest(QString serPortName) {
	m_portName = serPortName;
}

void FreeEMS_LoaderComms::open() {
	m_serPort->openPort(m_portName);
	m_serPort->setupPort(m_portBaud, m_portDataBits, m_portParity, m_portStopBits);
	m_serPort->communicate();
	m_serPort->flushRX();
	this->sleep(1); //temp hack to be replace by a callback, we need to wait for the flush to complete via another thread
}

bool FreeEMS_LoaderComms::isReady() const {
	return m_smIsReady ? true : false;
}

bool FreeEMS_LoaderComms::isOpen() {
	return m_serPort->isOpen();
}

void FreeEMS_LoaderComms::init() {
	m_recordSetReady = false;
	m_lastLoadAddress = 0;
	//clearSets();
	m_s19SetOneCount = 0;
	m_serPort = new IPDS::SerialIO;
	m_s19SetOne = NULL;
	m_actionErase = false;
	m_actionRip = false;
	m_actionLoad = false;
	m_actionConnect = false;
	m_actionDisConnect = false;
	m_RipSMCode = false;
}

void FreeEMS_LoaderComms::close() {
	if (isOpen() == false)
		return;
	emit displayMessage(MESSAGE_INFO, "closing serial port");
	m_serPort->closePort();
	m_actionConnect = false;
	m_actionErase = false;
	m_actionDisConnect = false;
	m_actionLoad = false;
	m_actionRip = false;
	m_smIsReady = false;
}

int FreeEMS_LoaderComms::loadDevice() {
	if(m_recordSetReady) {
		return writeBlocks();
	} else {
		emit displayMessage(MESSAGE_ERROR, "can't perform load, record set not yet loaded");
		return -1;
	}
}

int FreeEMS_LoaderComms::ripDevice() {
	int i;
	int totalBytes = 0;
	int totalBytesTX = 0;
	unsigned int numSectors;
	unsigned int address;
	unsigned int PPageIndex;
	unsigned int nPages;
	unsigned int firstAddress;
	unsigned int lastAddress;
	unsigned int bytesPerRecord = 16; //read 16 bytes TODO make configurable, same as default hcs12mem tool
	//  unsigned int bytesInRange;
	unsigned int pagedAddress = 0;
	int result = 1;

	std::vector<unsigned char> rxBuffer(bytesPerRecord);
	ofstream outFile(m_ripFilename.toAscii(), ios::out | ios::binary);
	FreeEMS_LoaderSREC s19Record(S2);
	totalBytes = getDeviceByteCount();
	emit configureProgress(0, totalBytes - 1);

	for (i = 0; i < numDataVectorTableEntries; i++) {
		if (!strcmp(flashModuleTable[m_flashTypeIndex].name, dataVectorTable[i].association)) {
			nPages = dataVectorTable[i].stopPage - dataVectorTable[i].startPage;
			nPages++; //there is always 1 page to rip
			for (PPageIndex = dataVectorTable[i].startPage; nPages && (result == 1); PPageIndex++, nPages--) {
                if(SMSetPPage(PPageIndex)) { //set Ppage register
                    return -1;
                }
                firstAddress = dataVectorTable[i].startAddress;
				lastAddress = dataVectorTable[i].stopAddress;
				s19Record.setRecordAddress(firstAddress);
				numSectors = (lastAddress - firstAddress) / bytesPerRecord + 1;
				//TODO add error for invalid range or improper modulus
				rxBuffer.clear();
				for (address = firstAddress; numSectors; address += bytesPerRecord, numSectors--, totalBytesTX +=
						bytesPerRecord) {
					loaderBusy.lock();
					if (loaderAbort) { // see if the user has requested we abort
						loaderBusy.unlock();
						result = -2;
						break;
					}
					loaderBusy.unlock();
					// Check to see if this range needs to be skipped
					if (m_RipSMCode == false && address >= (0xFFFF & SM_CODE_START_ADDRESS)
							&& PPageIndex == (SM_CODE_START_ADDRESS >> 16)) {
						qDebug() << "Skipping serial monitor address " << address;
						continue; //TODO replace this with defined vectors rather than a static number
					}
					//Read Block
					pagedAddress = PPageIndex;
					pagedAddress <<= 16;
					pagedAddress += address;
					if (SMReadByteBlock((unsigned short) address, bytesPerRecord, rxBuffer) < 0) {
						emit udProgress(0);
						result = -1;
						break;
					}
					s19Record.initVariables(); // clear record
					s19Record.setTypeIndex(S2);
					s19Record.setRecordAddress(pagedAddress);
					s19Record.fillRecord(rxBuffer);
					s19Record.buildRecord();
					emit udProgress(totalBytesTX); //Update Progress Bar
					if (s19Record.isRecordNull() == false) {
						outFile << s19Record.retRecordString();
					}
				}
			}
		}
	}
	outFile.close();
//	delete s19Record;
	return result;
}

void FreeEMS_LoaderComms::setFlashType(const char *commonName) {
	int i;
	for (i = 0; flashModuleTable[i].name; i++) {
		if (!strcmp(flashModuleTable[i].name, commonName)) {
			m_flashTypeIndex = i;
			//cout << " set flash type to " << commonName; //TODO MAKE THREAD SAFE
			m_fDeviceIsSet = true;
			return;
		}
	}
}

int FreeEMS_LoaderComms::SMSetPPage(unsigned char PPage) {
	unsigned char page = PPage;
	write(&SMWriteByte, 1);
	write(&Zero, 1);
	write(&PPageRegister, 1);
	write(&page, 1);
	if (verifyReturn(GENERIC) < 0) {
		emit displayMessage(MESSAGE_ERROR, "cannot verify ACK after setting ppage");
        return -1;
	}
    return 0;
}

void FreeEMS_LoaderComms::SMReadChars(const char *data, unsigned int size) {
	int i;
	printf("\n about to write char(s) ");
	for (i = (int) size; i > 0; i--) {
		printf("-> %x", *(data + (i - 1)));
	}
}

void FreeEMS_LoaderComms::resetSM() {
	write(&SMReset, 1);
	//if (verifyReturn(GENERIC)) {
	//	emit displayMessage(MESSAGE_INFO, "SM Reset ACK received");
	//} else {
	//	emit displayMessage(MESSAGE_ERROR, "SM Reset ACK not received, TODO check for packets");
	//}
	return;
}

void FreeEMS_LoaderComms::setSM() {
	//TODO make configurable
	int i;
	for(i = 3; i; --i) {
        if(i < 3) {
            emit displayMessage(MESSAGE_INFO, "Serial Monitor verification failed retrying...");
        }
        m_serPort->flushRX();
        sleep(1);
        write(&SMReturn, 1);
		if (verifyReturn(SETSM) > 0) {
			m_smIsReady = true;
		} else {
			m_smIsReady = false;
		}
		if(m_smIsReady == true)
			break;
		sleep(1);
	}
	return;
}

void FreeEMS_LoaderComms::write(const unsigned char *data, unsigned int size) {
	//sleep(1);
	m_serPort->writeData(data, size);
}

void FreeEMS_LoaderComms::write(const std::vector<unsigned char>& data) {
	unsigned int i;
	unsigned char d;
	for (i = 0; i < data.size(); i++) {
		d = data[i];
		write(&d, 1);
	}
}

void FreeEMS_LoaderComms::writeString(const std::string& s) {
	unsigned int i;
	unsigned char d;
	for (i = 0; i < s.size(); i++) {
		d = s[i];
		write(&d, 1);
	}
}

//TODO add parity "double read" option
int FreeEMS_LoaderComms::read(unsigned char* data, unsigned int size) {
	int readResult = m_serPort->readData(data, size);

	if (readResult < 0) {
		close();
		emit displayMessage(MESSAGE_ERROR,"Problem reading from the device, is seems to have gone away, aborting");
		emit setGUI(STATE_STANDING_BY);
	}
    return readResult;
}

std::vector<unsigned char> FreeEMS_LoaderComms::read(unsigned int size) {
	vector<unsigned char> result(size, '\0'); //Allocate a vector with the desired size
	read(&result[0], size); //Fill it with values
	return result;
}

FreeEMS_LoaderComms::~FreeEMS_LoaderComms() {
	//TODO populate with proper code
	close();
	if(m_s19SetOne != NULL) {
		qDebug() << "calling delete [] for s19Records";
		delete [] m_s19SetOne;
	}
	delete m_serPort;
}

void FreeEMS_LoaderComms::returnFlashType(unsigned char *responce) {
	unsigned char getIDCommand = 0xB7;
	write(&getIDCommand, 1);
	read(responce, 4);
	return;
}

int FreeEMS_LoaderComms::verifyReturn(SM_COMMAND_TYPE type) { //TODO this should iterate though a list of possible returns, citing specific errors
	//QByteArray resp;
	//resp = serPort->read(3); //todo use wrapper
	//QByteArray tester(SMRDY);// = {0xe1, 0x00, 0xe3};
	//usleep(5000);
	unsigned char response[3] = { 0 };
    if(read(response, 3) < 3 ){
         emit displayMessage(MESSAGE_ERROR, "problem getting SM return message");
    }
	switch (type){
		case GENERIC:
            if (response[0] ==  0xE0 && response[2] == 0x3E) {
				qDebug("smVerify good");
				return 1;
			}
            emit displayMessage(MESSAGE_ERROR, "unable to verify serial monitor return code");
            qDebug("SM return reads %x %x %x", response[0], response[1], response[2]);
            return -1;
		break;
		case SETSM:
            if(response[0] == 0xE0){
				qDebug("need to read two bytes");
                if(response[2] != 0x3e){
					emit displayMessage(MESSAGE_ERROR, "unable to verify serial monitor presence");
					return -1;
				} else {
					return 1;
				}
        }else if(response[0] == 0xE1){
                if(response[2] == (char)0x3e){
					emit displayMessage(MESSAGE_INFO, "serial monitor already running");
					return 1;
				}
			}
			emit displayMessage(MESSAGE_ERROR, "Data read, but it was not a serial monitor ACK");
            qDebug("read %x %x %x", response[0], response[1], response[2]);
			return -1;
		break;
	}
	qDebug("error verifying return");
	return -1;
}

int FreeEMS_LoaderComms::SMReadByteBlock(unsigned int address, unsigned int plusBytes, std::vector<unsigned char> &vec) {
	std::vector<unsigned char> buffer(plusBytes);
	unsigned char highByte = (address & 0xFF00) >> 8;
	unsigned char lowByte = address & 0x00FF;
	unsigned char bytesRequested = plusBytes - 1;
	write(&SMReadBlock, 1);
	write(&highByte, 1);
	write(&lowByte, 1);
	write(&bytesRequested, 1);
	buffer = read(plusBytes);
	if (verifyReturn(GENERIC) < 0){ // you must always verify a return to "clear" the buffer
		emit displayMessage(MESSAGE_ERROR, "Error validating ACK from SMReadByteBlock");
		return -1;
	}
	vec = buffer;
	return 1;
}

int FreeEMS_LoaderComms::erasePage(char PPage) {
    if(SMSetPPage(PPage)) {
        return -1;
    }
	write(&SMErasePage, 1);
	if (verifyReturn(GENERIC) < 0) {
		emit displayMessage(MESSAGE_ERROR, "Error validating SMErasePage confirmation");
		emit udProgress(0);
		return -1;
	}
	return 1;
}

int FreeEMS_LoaderComms::eraseDevice() {
	//serPort->flushInBuffer();
	int i;
	int nPages;
	char PPageIndex;
	//setSM(); //change to checkSM Incase the SM has been reset by the user
	// TODO change all errors to a SM check call back or something
	if (m_smIsReady) {
		//calculate total bytes in device
		for (i = 0; i < numDataVectorTableEntries; i++) {
			if (!strcmp(flashModuleTable[m_flashTypeIndex].name, dataVectorTable[i].association)) {
				nPages = dataVectorTable[i].stopPage - dataVectorTable[i].startPage + 1;
				emit
				configureProgress((unsigned char) dataVectorTable[i].startPage,
						(unsigned char) dataVectorTable[i].stopPage);
				for (PPageIndex = dataVectorTable[i].startPage; nPages; PPageIndex++, nPages--) {
					loaderBusy.lock();
					if (loaderAbort) {
						loaderBusy.unlock();
						return -2;
					}
					loaderBusy.unlock();
					if ( erasePage(PPageIndex) < 0 ) {
						emit displayMessage(MESSAGE_ERROR, "Problem erasing device");
						emit udProgress(0);
						return -1;
					}
					emit udProgress((unsigned char) PPageIndex);
				}
			}
		}
	} else {
		emit displayMessage(MESSAGE_ERROR, "Serial monitor not ready");
	}
	return 1;
}

void FreeEMS_LoaderComms::setRipFilename(QString name) {
	m_ripFilename = name;
}

void FreeEMS_LoaderComms::setLoadFilename(QString name) {
	m_loadFilename = name;
}

int FreeEMS_LoaderComms::getDeviceByteCount() {
	int totalBytes = 0;
	int i;
	if (m_fDeviceIsSet == true) {
		for (i = 0; i < numDataVectorTableEntries; i++) {
			if (!strcmp(flashModuleTable[m_flashTypeIndex].name, dataVectorTable[i].association)) {
				totalBytes += (dataVectorTable[i].stopPage - dataVectorTable[i].startPage)
								* (dataVectorTable[i].stopAddress - dataVectorTable[i].startAddress);
			}
		}
		return totalBytes;
	} else {
		emit displayMessage(MESSAGE_INFO, "Cannot get byte count, no device set");
		return 0;
	}
}

bool FreeEMS_LoaderComms::generateRecords(vector<string>* lineArray) {
    unsigned int i;
    int status;
    bool result = true; //innocient until proven guilty
    string line;
    memset(&m_parsingResults, 0, sizeof(m_parsingResults));

    for (i = 0; i < lineArray->size(); i++) {
        line = lineArray->at(i);
        cout << endl << "Analysing: " << line;
        status = m_s19SetOne[i].createFromString(&line);
        switch (status) {
        case LOADABLE:
            m_parsingResults.numLoadable++;
            cout << " OK";
            break;
        case RECORD_NULL:
            //TODO review, since the readline code clips blank lines before we get there this should never be encountered
        	cout << " REMOVE THIS CASE";
            break;
        case RECORD_UNHANDLED:
            cout << " UNHANDLED SKIPPING BUG AUTHOR FOR IMPLIMENTATION";
            break;
        case SKIP_SM_RANGE:
            m_parsingResults.numSMRange++;
            cout << " SKIPPING SM RANGE PAYLOAD";
            break;
        case UNLOADABLE_BAD_CSUM:
            m_parsingResults.numBadCSum++;
            cout << " CHECKSUM MISMATCH";
            result = false;
            break;
        case UNLOADABLE_BAD_LENGTH_TOO_LONG:
        	cout << " LINE TOO LONG";
            result = false;
            break;
        case UNLOADABLE_BAD_LENGTH_TOO_SHORT:
        	cout << " LINE TOO SHORT";
            result = false;
            break;
        case UNLOADABLE_INVALID_START_CHAR:
            cout << " CHECKSUM MISMATCH";
            result = false;
            break;
        case VENDOR_RECORD:
            m_parsingResults.numVendorData++;
            cout << " VENDOR DATA";
            break;
        default:
            cout << "TODO figure out odd-ball problem" << endl;
            result = false;
            break;
        }
    }
    return result;
}

int FreeEMS_LoaderComms::loadRecordSet() {
	int i;
	emit configureProgress(0, m_s19SetOneCount - 1);
	for (i = 0; i < m_s19SetOneCount; i++) {
		loaderBusy.lock();
		if (loaderAbort) {
			loaderBusy.unlock();
			return -2;
		}
		loaderBusy.unlock();
		if (m_s19SetOne[i].getPayloadStatus() == LOADABLE) {
			if (SMWriteByteBlock(m_s19SetOne[i].payloadAddress,	m_s19SetOne[i].recordBytes, m_s19SetOne[i].recordPayloadBytes) != 1) {
				emit displayMessage(MESSAGE_ERROR, "Unable to load record set");
				emit udProgress(0);
				return -1;
			} else {
				emit udProgress(i);
			}
		} else {
			qDebug() << "Skipping unloadable record";
		}
	}
	return 1;
}

int FreeEMS_LoaderComms::SMWriteByteBlock(unsigned int address, char* bufPtr, unsigned int numBytes) {
	int typeID = S2; //TODO get from s19
	unsigned int Ppage;
	unsigned int i;
	std::vector<unsigned char> verifyString;
	std::vector<unsigned char> readString;
	unsigned char c;
	unsigned char highByte;
	unsigned char lowByte;
	unsigned char bytesToWrite;

	for (i = 0; i < numBytes; i++) {
		verifyString.push_back(*(bufPtr + i));
	}

	switch (typeID) {
	case S2:
		if ((address & 0x0FF0000) != (m_lastLoadAddress & 0x0FF0000)) {
			Ppage = address >> 16;
			erasePage((char)Ppage); //sets the page and erases it
		}
		m_lastLoadAddress = address; // save address
		highByte = (address & 0xFF00) >> 8;
		lowByte = address & 0x00FF;
		bytesToWrite = (char) numBytes - 1;
		c = (char) SMWriteBlock;
		write(&c, ONE_BYTE);
		write(&highByte, ONE_BYTE);
		write(&lowByte, ONE_BYTE);
		write(&bytesToWrite, ONE_BYTE);
		write((unsigned char*)bufPtr, numBytes);
    	if (verifyReturn(GENERIC) < 0) {
				emit displayMessage(MESSAGE_INFO, "Error did not receive ACK after writing a block");
				return -1;
		}
		if (verifyLastWrite == true) {
			SMReadByteBlock(address, numBytes, readString);
			if (verifyString != readString) {
				emit displayMessage(MESSAGE_INFO, "Error validating sector at TODO implement retry");
				return -1;
			}
		}
		break;
	default:
		emit displayMessage(MESSAGE_INFO, "Error device type not set");
		break;
	}
	return 1;
}

void FreeEMS_LoaderComms::flushRXStream() {
	qDebug("flushRXStream called todo repimpiment");
}

void FreeEMS_LoaderComms::setAction(QString action) {
	if (action == "NONE") {
		m_actionConnect = false;
		m_actionRip = false;
		m_actionErase = false;
		m_actionDisConnect = false;
		m_actionLoad = false;
	}
	if (action == "CONNECT")
		m_actionConnect = true;
	if (action == "RIP")
		m_actionRip = true;
	if (action == "ERASE")
		m_actionErase = true;
	if (action == "LOAD")
		m_actionLoad = true;
	if (action == "DISCONNECT")
		m_actionDisConnect = true;
}

void FreeEMS_LoaderComms::run() {
	loaderAbort = false;
	int result;
	emit setGUI(STATE_WORKING);
	m_actionConnect = false;
	connect();
	if (!isReady()) {
		//emit displayMessage(MESSAGE_ERROR, "Serial Monitor Not Found, Maybe Check Your Load Jumper");
		/* Error Message already displayed if SM was not set at this point */
		m_actionRip = 0;
		m_actionErase = 0;
		m_actionLoad = 0;
		//return;
	}
	if (m_actionRip) {
		emit setGUI(STATE_WORKING);
		m_actionRip = false;
		emit displayMessage(MESSAGE_INFO, "Ripping...");
		result = ripDevice();
		if (result == -1) {
			emit displayMessage(MESSAGE_ERROR, "Failed To Rip");
            m_actionLoad = 0;
		} else if (result == -2) {
			emit displayMessage(MESSAGE_INFO, "User Aborted!");
		} else {
			emit displayMessage(MESSAGE_INFO, "Rip Successful!");
		}
	}
	if (m_actionErase) {
		emit setGUI(STATE_WORKING);
        m_actionErase = false;
		emit displayMessage(MESSAGE_INFO, "Erasing...");
		result = eraseDevice();
		if (result == -1) {
			emit displayMessage(MESSAGE_ERROR, "Failed To Erase");
		} else if (result == -2) {
			emit displayMessage(MESSAGE_INFO, "User Aborted!");
		} else {
			emit displayMessage(MESSAGE_INFO, "Erase Successful!");
		}
	}
	if (m_actionLoad) {
		emit setGUI(STATE_WORKING);
		m_actionLoad = false;
		emit displayMessage(MESSAGE_INFO, "Loading...");
		result = loadDevice();
		if (result == -1) {
			emit displayMessage(MESSAGE_INFO, "Failed To Load");
		} else if (result == -2) {
			emit displayMessage(MESSAGE_INFO, "User Aborted!");
		} else {
            emit displayMessage(MESSAGE_INFO, "Load Successful, resetting device!");
			resetSM();
            emit setGUI(STATE_LOAD_COMPLETE);
		}
	}
	this->sleep(1); //temp hack, this should be done via a callback after the outgoing serial buffer is empty
	close();
	emit setGUI(STATE_STANDING_BY);
}

void FreeEMS_LoaderComms::abortOperation() {

}

void FreeEMS_LoaderComms::parseFile() {

    m_s19SetOneCount = 0;
    m_recordSetReady = false;
    int linesRead = 0;
    vector< string >* lineArray = new vector< string >;
    QFile ifs(m_loadFilename);
    if(!ifs.open(QIODevice::ReadOnly)) {
        //QMessageBox::information(0, "error", file.errorString());
        emit displayMessage(MESSAGE_ERROR, "Error opening file");
        return;
    }
    QTextStream in(&ifs);
    while(!in.atEnd()) {
        QString line = in.readLine();
        if(line.length() > 0) {
            lineArray->push_back(line.toStdString());
            linesRead++;
        } else {
            qDebug() << "Skipping blank line at ";
        }

    }
    ifs.close();
    qDebug() << "Lines read " << linesRead;
    initRecordSet(linesRead);
    m_recordSetReady = generateRecords(lineArray);
    if(m_recordSetReady == true) {
        cout << endl << "Record Set OK";
    }else {
        cout << endl << "Record Set FAIL";
    }
    delete lineArray;
}

int FreeEMS_LoaderComms::writeBlocks() {
	return loadRecordSet();
}

bool FreeEMS_LoaderComms::isRecordSetLoaded() {
	if(m_recordSetReady){
		return true;
	}
	return false;
}

int FreeEMS_LoaderComms::numLoadableRecords() {
    return m_parsingResults.numLoadable;
}

int FreeEMS_LoaderComms::numBadSums() {
    return m_parsingResults.numBadCSum;
}

void FreeEMS_LoaderComms::setDataMode(QString& mode) {
	m_serPort->setMode(mode);
}

void FreeEMS_LoaderComms::connect() {
	QFile file;
	file.setFileName(m_portName);
	bool fileIsOpen = 0;
	fileIsOpen = file.open(QIODevice::ReadWrite); //TODO further try to detect the problem
	file.close();
#ifdef __WIN32__
	fileIsOpen = true; /* TODO fix always set OK for windows, for now */
#endif
	if (fileIsOpen) {
		//TODO add code to check for packets here, before going into RAM mode for SM
		QString mode = "RAW";
		setDataMode(mode);
		open();
		setSM();
		setFlashType(defFlashType);
		if (!isReady()) {
			emit displayMessage(MESSAGE_INFO, "Unable to summon SerialMonitor");
		}
	} else {
		emit displayMessage(MESSAGE_INFO, "Unable to open port " + m_portName);
	}
}


void FreeEMS_LoaderComms::setupPort(QString portName, unsigned int baud, unsigned int stopBits, unsigned int dataBits,
										QString parity) {
	m_portName = portName;
	m_portBaud = baud;
	m_portDataBits = dataBits;
	m_portStopBits = stopBits;
	m_portParity = parity;
	//TODO finish other stuff
}

void FreeEMS_LoaderComms::initRecordSet(unsigned int numRecords) {
	m_s19SetOne = new FreeEMS_LoaderSREC[numRecords];
	m_s19SetOneCount = numRecords;
}

void FreeEMS_LoaderComms::ripSMCode(bool includeSM) {
	m_RipSMCode = includeSM;
}

bool FreeEMS_LoaderComms::payloadOverlapCheck() {
    //TODO make sure none of the payloads overlap each other in anyway
    //TODO we *may also want to support this at some point
    return true;
}

bool FreeEMS_LoaderComms::payloadSort() {
    //TODO sort by destination address
    return true;
}

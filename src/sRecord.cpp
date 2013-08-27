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

#include <sRecord.h>

/*
 +-------------------//------------------//-----------------------------+
 | type | count | address  |            data                 | checksum |
 +-------------------//------------------//-----------------------------+
 S2     14      F880B0    B746B7C687B7467D3044FD30421AEEFD      60
 */

FreeEMS_LoaderSREC::FreeEMS_LoaderSREC() {
	initVariables();
}

FreeEMS_LoaderSREC::FreeEMS_LoaderSREC(int type) {
	initVariables();
	setTypeIndex(type);
}

FreeEMS_LoaderSREC::~FreeEMS_LoaderSREC() {
	qDebug() << "called FreeEMS_LoaderSREC::~FreeEMS_LoaderSREC()";
	// TODO Auto-generated destructor stub
}

void FreeEMS_LoaderSREC::initVariables() {
	memset(recordPayload, 0, sizeof(recordPayload)); //clear buffers
	memset(recordTypeIdChars, 0, sizeof(recordTypeIdChars));
	memset(recordAddressChars, 0, sizeof(recordAddressChars));
	memset(recordPayloadPairCountChars, 0, sizeof(recordPayloadPairCountChars));
	memset(recordCheckSumChars, 0, sizeof(recordCheckSumChars));

	record.clear();
	recordIndex = 0;
	charsInAddress = 0;
	recordPayloadBytes = 0;
	payloadAddress = 0;
	typeIndex = 0;
	numHexValues = 0;
	writeAccess = true;
	addressIsSet = false;
	typeIsSet = false;
	recordIsNull = true;
	numPairsSet = false;
	mismatchedCheckSum = false;
	correctLineLength = true;
	charactersValid = true;
	m_payloadStatus = RECORD_NULL;
}

FreeEMS_LoaderSREC::FreeEMS_LoaderSREC(char *input, int numBytes, int type, unsigned int recordAddress) {
	input++;
	numBytes++;
	initVariables();
	setTypeIndex(type);
	setRecordAddress(recordAddress);
	//fillRecord(input, recordPayloadLength);
}

int FreeEMS_LoaderSREC::fillRecord(std::vector<unsigned char> binaryChars) {
	int i, j;
	for (j = binaryChars.size(), i = 0; i < j; i++) {
		putNextByte((char) binaryChars.at(i));
	}
	return 0;
}

int FreeEMS_LoaderSREC::putNextByte(char byte) {
	if (byte != (char) 0xFF)
		recordIsNull = false;
	FreeEMS_LoaderParsing::intToHexAscii((int) byte, (recordPayload + recordIndex), ONE_BYTE * BITS_PER_BYTE);
	recordIndex += ASCII_PAIR;
	recordPayloadBytes++;
	return 1;
}

int FreeEMS_LoaderSREC::setTypeIndex(int type) {
	int i;
	for (i = 0; s19Table[i].description; i++) {
		if (type == s19Table[i].type) {
			typeIndex = type;
			memcpy(recordTypeIdChars, s19Table[typeIndex].s19ID, TWO_BYTES);
			typeIsSet = true;
			return 1;
		}
	}
	return -1;
}

int FreeEMS_LoaderSREC::setRecordAddress(unsigned int address) {
	if (typeIsSet) {
		payloadAddress = address;
		FreeEMS_LoaderParsing::intToHexAscii(address, recordAddressChars,
				(s19Table[typeIndex].addressBytes) * BITS_PER_BYTE);
		charsInAddress = (s19Table[typeIndex].addressBytes) * ASCII_PAIR;
		addressIsSet = true;
	} else {
		cout << "error type not set";
	}

	return 0;
}

int FreeEMS_LoaderSREC::setRecordAddress(char* address) {
	if (typeIsSet) {
		sscanf(address, "%x", &payloadAddress);
		charsInAddress = (s19Table[typeIndex].addressBytes) * ASCII_PAIR;
		addressIsSet = true;
	} else {
		cout << "error type not set";
	}

	return 0;
}

/*
 These characters when paired and interpreted as a hexadecimal value display
 the least significant byte of the ones complement of the sum of the byte values
 represented by the pairs of characters making up the count, the address, and
 the data fields.
 */
void FreeEMS_LoaderSREC::calculateCheckSum() {
	int index;
	if (addressIsSet && typeIsSet && numPairsSet) {
		recordChkSum = 0;
		for (index = 0; index < (recordPayloadBytes * ASCII_PAIR); index += 2) {
			recordChkSum += FreeEMS_LoaderParsing::asciiPairToChar(&recordPayload[index]); // add all payload bytes
		}
		for (index = 0; index < (charsInAddress); index += 2) {
			recordChkSum += FreeEMS_LoaderParsing::asciiPairToChar(&recordAddressChars[index]); // add all address bytes
		}
		recordChkSum += numHexValues; // add char count to checksum
		recordChkSum = ~(recordChkSum & 0x00ff);
	} else {
		cout << "Checksum cannot be calculated because, address or type has not been set yet";
	}
	return;
}

void FreeEMS_LoaderSREC::buildRecord() {
	setNumPairsInRecord();
	calculateCheckSum();
	FreeEMS_LoaderParsing::intToHexAscii((int) recordChkSum, &recordCheckSumChars[0], 1 * BITS_PER_BYTE);

	int i;
	// write record ID
	for (i = 0; i < TWO_BYTES; i++) {
		record += recordTypeIdChars[i];
	}
	// write record pair count
	for (i = 0; i < TWO_BYTES; i++) {
		record += recordPayloadPairCountChars[i];
	}
	// write record address
	for (i = 0; i < charsInAddress; i++) {
		record += recordAddressChars[i];
	}
	// write record payload
	for (i = 0; i < recordIndex; i++) {
		record += recordPayload[i];
	}
	// write record checksum
	for (i = 0; i < TWO_BYTES; i++) {
		record += recordCheckSumChars[i];
	}
	// write line ending
	record.append("\r\n");
}

void FreeEMS_LoaderSREC::setNumPairsInRecord() { //TODO add isSet
	numHexValues = recordPayloadBytes + (charsInAddress / 2) + CH_PAIR_COUNT_BYTE;
	FreeEMS_LoaderParsing::intToHexAscii(numHexValues, &recordPayloadPairCountChars[0], ONE_BYTE * BITS_PER_BYTE);
	numPairsSet = true;
}

void FreeEMS_LoaderSREC::printRecord() {
	cout << "printing string ascii";
	cout << record;
}

std::string FreeEMS_LoaderSREC::retRecordString() {
	std::string str(record);
	return str;
}

int FreeEMS_LoaderSREC::retRecordSize() {
	return record.length();
}

int FreeEMS_LoaderSREC::createFromString(string* lineIn) {

	char type = *(lineIn->c_str() + 1);
	//int lineEndSequenceLenth; Unable to load record set

	if (*lineIn->c_str() == 'S') //start of record
	{
		switch (type) {
		case '0':
			setTypeIndex(S0);
			memcpy(recordTypeIdChars, lineIn->c_str(), TWO_BYTES);
			memcpy(recordPayloadPairCountChars, lineIn->c_str() + S2_PAIR_COUNT_OFFSET, TWO_BYTES);
            m_payloadStatus = VENDOR_RECORD;
			break;
		case '1':
			m_payloadStatus = RECORD_UNHANDLED;
			break;
		case '2': //Record is S2
			setTypeIndex(S2);
			memcpy(recordTypeIdChars, lineIn->c_str(), TWO_BYTES);
			memcpy(recordPayloadPairCountChars, lineIn->c_str() + S2_PAIR_COUNT_OFFSET, TWO_BYTES);
			bytePairCount = (int) FreeEMS_LoaderParsing::asciiPairToChar(recordPayloadPairCountChars);
			/* Check to make sure the pair count matches */
			//TODO once used in other cases move this to its own function
			/* readline() precedes this code so we should have clean ending strings already.  Maybe revise it all and determine our own
			 * line endings */
			int lengthDifference;
			lengthDifference = (lineIn->length() - 2 - 2) - (unsigned int) (bytePairCount * 2);
			if (lengthDifference == 1) {
				if ((lineIn->at(lineIn->length() - 1)) != '\r') {
					m_payloadStatus = UNLOADABLE_BAD_LENGTH;
				}
			} else if (lengthDifference > 1) {
				m_payloadStatus = UNLOADABLE_BAD_LENGTH;
			}else if (lengthDifference < 0) {
				m_payloadStatus = UNLOADABLE_BAD_LENGTH_TOO_SHORT;
			}
			recordPayloadBytes = bytePairCount - 4; //TODO make proper maybe make a function to call setNumPairsInRecord too
			memcpy(recordAddressChars, lineIn->c_str() + S2_ADDRESS_OFFSET, 6); //S2 has 6 chars in address
			setRecordAddress(recordAddressChars);
			memcpy(recordPayload, lineIn->c_str() + S2_PAYLOAD_OFFSET, recordPayloadBytes * 2);
			memcpy(recordCheckSumChars,
					lineIn->c_str() + S2_PAIR_COUNT_OFFSET + (recordPayloadBytes * 2) + charsInAddress + 2, TWO_BYTES);
			recordLoadedChkSum = FreeEMS_LoaderParsing::asciiPairToChar(recordCheckSumChars);
			FreeEMS_LoaderParsing::asciiPairToArray(recordPayload, recordBytes, recordPayloadBytes);
			setNumPairsInRecord();
			calculateCheckSum(); // this asserts that the data is good
			if (recordChkSum != recordLoadedChkSum) {
				m_payloadStatus = UNLOADABLE_BAD_CSUM;
			}else{
				m_payloadStatus = LOADABLE;
			}
			break;
		case '3':
			m_payloadStatus = RECORD_UNHANDLED;
			break;
		default:
			//m_payloadStatus = UNLOADABLE_BAD_START_CHAR;
			break;
		}
	} else {
		m_payloadStatus = UNLOADABLE_INVALID_START_CHAR;
	}
	return m_payloadStatus;
}

unsigned int FreeEMS_LoaderSREC::getRecordAddress() {
	return payloadAddress;
}

int FreeEMS_LoaderSREC::getRecordTypeIndex() {
	return typeIndex;
}

bool FreeEMS_LoaderSREC::isRecordNull() {
	return recordIsNull;
}

unsigned char FreeEMS_LoaderSREC::getCalculatedSum() {
	return recordChkSum;
}

bool FreeEMS_LoaderSREC::isCheckSumMismatched() {
	return mismatchedCheckSum;
}

bool FreeEMS_LoaderSREC::isLineLengthCorrect() {
	return correctLineLength;
}

bool FreeEMS_LoaderSREC::areCharactersValid() {
	return charactersValid;
}

void FreeEMS_LoaderSREC::setPayloadStatus(int status) {
	m_payloadStatus = status;
}

int FreeEMS_LoaderSREC::getPayloadStatus() {
	return m_payloadStatus;
}

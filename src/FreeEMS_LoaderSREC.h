/*
 * FreeEMS_LoaderSREC.h
 *
 *  Created on: Nov 29, 2010
 *      Author: seank
 */

#ifndef FREEEMS_LOADERSREC_H_
#define FREEEMS_LOADERSREC_H_

//enum BOOL {READONLY, READWRITE}

class FreeEMS_LoaderSREC {
public:
	FreeEMS_LoaderSREC();
	virtual ~FreeEMS_LoaderSREC();
//	bool verifyFile(int *file);
	int  fillRecord(char *input);
	/*
	 * verify a records crc data
	 */
	bool verifyRecord();
	int  getRecordAddress();
	char getNextByte();
	char getRecordType();
	char calculateCheckSum();
	int  putNextByte(char byte);
	int  setRecordAddress(int address);
	int  setRecordType(char *type);

private:
	char record[1024]; // should never exceede 515
	char recordPayload[1024];
	int  recordIndex;
	char recordChkSum;
	bool writeAccess;
	bool recordStatus;
};

#endif /* FREEEMS_LOADERSREC_H_ */

/*
 * FreeEMS_LoaderSREC.h
 *
 *  Created on: Nov 29, 2010
 *      Author: seank
 */

#ifndef FREEEMS_LOADERSREC_H_
#define FREEEMS_LOADERSREC_H_

#include <string>

using namespace std;

struct s19Info{
  int type;
  char *description;
  int addressBytes;
  bool dataSequence;
};

enum s19ID{
  S0,
  S1, // The type of record field is 'S1' (0x5331)
  S2, // The type of record field is 'S2' (0x5332)
  S3, // The type of record field is 'S3' (0x5333)
  S4,
  S5, // The type of record field is 'S5' (0x5335)
  S6,
  S7, // The type of record field is 'S7' (0x5337)
  S8, // The type of record field is 'S8' (0x5338)
  S9  // The type of record field is 'S9' (0x5339)
};

const struct s19Info s19Table[]
{
  {S0, "Block header", 2, true},
  {S1, "Data sequence", 2, true},
  {S2, "Data sequence", 3, true},
  {S3, "Data sequence", 4, true},
  {S5, "Record count", 2, false},
  {S7, "End of block", 4, false},
  {S8, "End of block", 3, false},
  {S9, "End of block", 2, false},
  {0,0,0,0}
};

//enum BOOL {READONLY, READWRITE}

class FreeEMS_LoaderSREC {
public:
	FreeEMS_LoaderSREC(unsigned char type);
	FreeEMS_LoaderSREC(char *input, int numBytes, unsigned char type);
	virtual ~FreeEMS_LoaderSREC();
//	bool verifyFile(int *file);
	int fillRecord(char *input, int numBytes, unsigned char type);
	/*
	 * calculate a records checksum and compre it to the stored value.
	 */
	bool verifyRecord();
	int  getRecordAddress();
	char getNextByte();
	char getRecordType();

	/*
	  These characters when paired and interpreted as a hexadecimal value display
	  the least significant byte of the ones complement of the sum of the byte values
	  represented by the pairs of characters making up the count, the address, and
	  the data fields.
	*/
	unsigned char calculateCheckSum(char *payload);

	int  putNextByte(char byte);
	int  setRecordAddress(unsigned int address);
	int  setRecordType(char *type);

private:
	string record;
	//char record[1024]; // should never exceede 515
	char recordPayload[1024];
	int recordPayloadLength;
 	int recordIndex;
 	unsigned int payloadAddress;
 	char recordChkSum;
	bool writeAccess;
	bool recordStatus;
};

#endif /* FREEEMS_LOADERSREC_H_ */

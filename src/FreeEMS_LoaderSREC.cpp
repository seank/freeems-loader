/*
 * FreeEMS_LoaderSREC.cpp
 *
 *  Created on: Nov 29, 2010
 *      Author: seank
 */

#include "FreeEMS_LoaderSREC.h"

/*
	+-------------------//------------------//-----------------------------+
	| type | count | address  |            data                 | checksum |
	+-------------------//------------------//-----------------------------+
	   S2     14      F880B0    B746B7C687B7467D3044FD30421AEEFD      60
*/

FreeEMS_LoaderSREC::FreeEMS_LoaderSREC(unsigned char type) {
   recordIndex = 0;
   recordPayloadLength = 0;

}

FreeEMS_LoaderSREC::~FreeEMS_LoaderSREC() {
	// TODO Auto-generated destructor stub
}

FreeEMS_LoaderSREC::FreeEMS_LoaderSREC(char *input, int numBytes, unsigned char type) {
  recordIndex = 0;
  recordPayloadLength = numBytes;
  fillRecord(input, recordPayloadLength, type);

}

int
FreeEMS_LoaderSREC::fillRecord(char *input, int numBytes, unsigned char type)
{

}

int
FreeEMS_LoaderSREC::putNextByte(char byte) {

}

int
FreeEMS_LoaderSREC::setRecordAddress(unsigned int address)
{
  //TODO add type vs size constraint checking
  payloadAddress = address;
  return 0;
}

unsigned char
FreeEMS_LoaderSREC::calculateCheckSum(char *payload){
  int index;
  unsigned char checksum = 0;
  for(index = 0; index < recordPayloadLength; index++)
    {
      checksum += (unsigned char) *(payload + index);
    }
  return ~checksum;
}

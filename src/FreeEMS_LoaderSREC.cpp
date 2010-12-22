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

FreeEMS_LoaderSREC::FreeEMS_LoaderSREC(int type) {
   recordIndex = 0;
   recordPayloadLength = 0;
   setTypeIndex(type);

}

FreeEMS_LoaderSREC::~FreeEMS_LoaderSREC() {
	// TODO Auto-generated destructor stub
}

FreeEMS_LoaderSREC::FreeEMS_LoaderSREC(char *input, int numBytes, int type, unsigned int recordAddress) {
  recordIndex = 0;
  recordPayloadLength = numBytes + CHECKSUM_BYTE;
  setTypeIndex(type);
  setRecordAddress(recordAddress);
  fillRecord(input, recordPayloadLength);

}

int
FreeEMS_LoaderSREC::fillRecord(char *input, int numBytes)
{
 int i;
 for(i = recordPayloadLength; i > 0; i--)
   {

   }
}

int
FreeEMS_LoaderSREC::putNextByte(char byte) {

}

int
FreeEMS_LoaderSREC::setTypeIndex(int type)
{
  int i;
  for(i = 0; s19Table[i].description; i++)
    {
      if(type == s19Table[i].type)
        {
         typeIndex = type;
         memcpy(recordPayload, s19Table[typeIndex].s19ID, 2);
         return 1;
        }
    }
  return -1;
}

int
FreeEMS_LoaderSREC::setRecordAddress(unsigned int address)
{
  char *startOfAddress = recordPayload + START_OF_ADDRESS_OFFSET;
  //TODO add type vs size constraint checking
  payloadAddress = address;
  addressIsSet = true;
  FreeEMS_LoaderParsing::intToHexAscii(address, startOfAddress, (s19Table[typeIndex].addressBytes) * BITS_PER_BYTE);
  //to convert an integer to an ASCII character

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

void
FreeEMS_LoaderSREC::buildRecord()
{
 //memcpy(record, recordPayload, recordPayloadLength );
}

void
FreeEMS_LoaderSREC::printRecord()
{
  cout<<"printing string ascii";
  cout<<record;
}

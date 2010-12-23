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
  memset(recordPayload, 0, ONE_KB); //clear buffer
  setTypeIndex(type);

}

FreeEMS_LoaderSREC::~FreeEMS_LoaderSREC() {
	// TODO Auto-generated destructor stub
}

FreeEMS_LoaderSREC::FreeEMS_LoaderSREC(char *input, int numBytes, int type, unsigned int recordAddress) {
  memset(recordPayload, 0, ONE_KB); //clear buffer
  setTypeIndex(type);
  setRecordAddress(recordAddress);
  //fillRecord(input, recordPayloadLength);
}

int
FreeEMS_LoaderSREC::fillRecord(char *input, int numBytes)
{
 int i;
 for(i = recordIndex; i > 0; i--)
   {

   }
}

int
FreeEMS_LoaderSREC::putNextByte(char byte) {

  if(typeIsSet == false)
    return -2;
  FreeEMS_LoaderParsing::intToHexAscii((int)byte, (recordPayload + recordIndex), ONE_BYTE * BITS_PER_BYTE);
  recordIndex++;
  recordPayloadBytes++;
  return 1;
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
         memcpy(recordTypeId, s19Table[typeIndex].s19ID, TWO_BYTES);
         typeIsSet = true;
         return 1;
        }
    }
  return -1;
}

int
FreeEMS_LoaderSREC::setRecordAddress(unsigned int address)
{
  payloadAddress = address;
  //recordPayloadLength += s19Table[typeIndex].addressBytes * TWO_BYTES;
  FreeEMS_LoaderParsing::intToHexAscii(address, recordAddress, (s19Table[typeIndex].addressBytes) * BITS_PER_BYTE);
  bytesInAddress = (s19Table[typeIndex].addressBytes) * ASCII_PAIR;
  addressIsSet = true;
  return 0;
}

unsigned char
FreeEMS_LoaderSREC::calculateCheckSum(){
  int index;
  unsigned char checksum = 0;
  for(index = 0; index < recordPayloadBytes; index++)
    {
      checksum += (unsigned char) *(recordPayload + index);
    }
  for(index = 0; index < bytesInAddress; index++)
    {
      checksum += (unsigned char) *(recordAddress + index);
    }
  return ~checksum;  // compliment and return
}

void
FreeEMS_LoaderSREC::buildRecord()
{

  int i;
  for(i =0; i > recordIndex; i++)
    {
      record += recordPayload[i];
    }
  cout<<record;
}

void
FreeEMS_LoaderSREC::setNumPairsInRecord()
{
  int numCharPairs = recordIndex / 2;
  FreeEMS_LoaderParsing::intToHexAscii(numCharPairs, recordPayload + TWO_BYTES, ONE_BYTE * BITS_PER_BYTE);
}


void
FreeEMS_LoaderSREC::printRecord()
{
  cout<<"printing string ascii";
  cout<<record;
}

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
  initVariables();
  setTypeIndex(type);

}

FreeEMS_LoaderSREC::~FreeEMS_LoaderSREC() {
	// TODO Auto-generated destructor stub
}

void
FreeEMS_LoaderSREC::initVariables()
{
  memset(recordPayload, 0, sizeof(recordPayload)); //clear buffer
  memset(recordTypeIdChars, 0, sizeof(recordTypeIdChars));
  memset(recordAddressChars, 0, sizeof(recordAddressChars));
  memset(recordPayloadPairCountChars, 0, sizeof(recordPayloadPairCountChars));
  memset(recordCheckSumChars, 0, sizeof(recordCheckSumChars));

  recordIndex = 0;
  charsInAddress = 0;
  checksum = 0;
  recordPayloadBytes = 0;
  payloadAddress = 0;
  typeIndex = 0;

  writeAccess = true;
  recordStatus = false;
  addressIsSet = false;
  typeIsSet = false;
}

FreeEMS_LoaderSREC::FreeEMS_LoaderSREC(char *input, int numBytes, int type, unsigned int recordAddress) {
  input++;
  numBytes++;
  initVariables();
  setTypeIndex(type);
  setRecordAddress(recordAddress);
  //fillRecord(input, recordPayloadLength);
}

int
FreeEMS_LoaderSREC::fillRecord(char *input, int numBytes)
{
  input++;
  numBytes++;
 int i;
 for(i = recordIndex; i > 0; i--)
   {

   }
 return 0;
}

int
FreeEMS_LoaderSREC::putNextByte(char byte) {

  if(typeIsSet == false)
    return -2;
  FreeEMS_LoaderParsing::intToHexAscii((int)byte, (recordPayload + recordIndex), ONE_BYTE * BITS_PER_BYTE);
  recordIndex += ASCII_PAIR;
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
         memcpy(recordTypeIdChars, s19Table[typeIndex].s19ID, TWO_BYTES);
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
  FreeEMS_LoaderParsing::intToHexAscii(address, recordAddressChars, (s19Table[typeIndex].addressBytes) * BITS_PER_BYTE);
  charsInAddress = (s19Table[typeIndex].addressBytes) * ASCII_PAIR;
  addressIsSet = true;
  return 0;
}

unsigned char
FreeEMS_LoaderSREC::calculateCheckSum(){
  int index;
  char checksum = 0;
  for(index = 0; index < (recordPayloadBytes * ASCII_PAIR); index += 2)
    {
      // TODO covert CH_PAIR to char value
      checksum += (char) FreeEMS_LoaderParsing::asciiPairToChar(&recordPayload[index]);
    }
  for(index = 0; index < (charsInAddress * ASCII_PAIR); index += 2)
    {
      // TODO covert CH_PAIR to char value
      checksum += (char) FreeEMS_LoaderParsing::asciiPairToChar(&recordAddressChars[index]);
    }
  return ~checksum;  // compliment and return
}

void
FreeEMS_LoaderSREC::buildRecord()
{
  calculateCheckSum();
  FreeEMS_LoaderParsing::intToHexAscii((int)calculateCheckSum(), recordCheckSumChars, TWO_BYTES * BITS_PER_BYTE);
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
  int numCharPairs = (recordPayloadBytes * 2) + charsInAddress;
  FreeEMS_LoaderParsing::intToHexAscii(numCharPairs, recordPayloadPairCountChars, ONE_BYTE * BITS_PER_BYTE);
}


void
FreeEMS_LoaderSREC::printRecord()
{
  cout<<"printing string ascii";
  cout<<record;
}

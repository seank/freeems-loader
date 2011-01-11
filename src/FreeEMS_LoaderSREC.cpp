/*
 * FreeEMS_LoaderSREC.cpp
 *
 *  Created on: Nov 29, 2010
 *      Author: seank
 */

#include "FreeEMS_LoaderSREC.h"
#include "freeems_loader_types.h"

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
  recordStatus = false;
  addressIsSet = false;
  typeIsSet = false;
  recordIsNull = true;
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
FreeEMS_LoaderSREC::fillRecord(std::vector<char> binaryChars)
{
 int i, j;
 for(j = binaryChars.size(), i = 0; i < j; i++)
   {
     putNextByte((char)binaryChars.at(i));
   }
 return 0;
}

int
FreeEMS_LoaderSREC::putNextByte(char byte) {
  if(byte != (char)0xFF)
    recordIsNull = false;
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

void
FreeEMS_LoaderSREC::calculateCheckSum(){
  int index;
  //unsigned  int checksum = payloadAddress;
  recordChkSum = 0;
  for(index = 0; index < (recordPayloadBytes * ASCII_PAIR); index += 2)
    {
      recordChkSum += FreeEMS_LoaderParsing::asciiPairToChar(&recordPayload[index]);// add all payload bytes
    }
  for(index = 0; index < (charsInAddress); index += 2)
    {
      recordChkSum += FreeEMS_LoaderParsing::asciiPairToChar(&recordAddressChars[index]);// add all address bytes
    }
  recordChkSum += numHexValues; // add char count to checksum
  recordChkSum = ~(recordChkSum & 0x00ff);
  return ;
}

void
FreeEMS_LoaderSREC::buildRecord()
{
  setNumPairsInRecord();
  calculateCheckSum();
  FreeEMS_LoaderParsing::intToHexAscii((int)recordChkSum, &recordCheckSumChars[0], 1 * BITS_PER_BYTE);

  int i;
  // write record ID
  for(i =0; i < TWO_BYTES; i++)
      {
        record += recordTypeIdChars[i];
      }
   // write record pair count
  for(i =0; i < TWO_BYTES; i++)
      {
       record += recordPayloadPairCountChars[i];
      }
   // write record address
  for(i =0; i < charsInAddress; i++)
    {
      record += recordAddressChars[i];
    }
  // write record payload
  for(i =0; i < recordIndex; i++)
    {
      record += recordPayload[i];
    }
  // write record checksum
  for(i =0; i < TWO_BYTES; i++)
    {
     record += recordCheckSumChars[i];
    }

  cout<<"record built";
  cout<<record;
}

void
FreeEMS_LoaderSREC::setNumPairsInRecord()
{
  numHexValues = recordPayloadBytes + (charsInAddress / 2 ) + CH_PAIR_COUNT_BYTE;
  FreeEMS_LoaderParsing::intToHexAscii(numHexValues, &recordPayloadPairCountChars[0], ONE_BYTE * BITS_PER_BYTE);
}


void
FreeEMS_LoaderSREC::printRecord()
{
  cout<<"printing string ascii";
  cout<<record;
}

std::string
FreeEMS_LoaderSREC::retRecordString()
{
  std::string str(record);
  return str;
}

int
FreeEMS_LoaderSREC::retRecordSize()
{
  return record.length();
}

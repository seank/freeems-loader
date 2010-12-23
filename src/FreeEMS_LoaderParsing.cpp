/*
 * FreeEMS_LoaderParsing.cpp
 *
 *  Created on: Dec 10, 2010
 *      Author: seank
 */

#include "FreeEMS_LoaderParsing.h"
#include "freeems_loader_types.h"
#include <string>
#include <algorithm>
#include <iostream>
#include "stdio.h"

using namespace std;

FreeEMS_LoaderParsing::FreeEMS_LoaderParsing()
{
  // TODO Auto-generated constructor stub
}

FreeEMS_LoaderParsing::~FreeEMS_LoaderParsing()
{
  // TODO Auto-generated destructor stub
}

void
FreeEMS_LoaderParsing::asciiPairToChars(char *ascii, char *eightBit)
{

}

int
FreeEMS_LoaderParsing::calcuateNumRecordsNeeded(int flashBytes, int bytesInRecord)
{
  return (int) flashBytes/bytesInRecord;
}

void
FreeEMS_LoaderParsing::intToHexAscii(int number, char* buffer, unsigned char numBitsToConvert)
{
  char charNumber = 0;
  int i;
    for( i = numBitsToConvert / 4, buffer += (i - 1); i > 0; i--, number >>= 4, buffer--)
      {
          charNumber = (number & 0x0F);
          if(charNumber > 0 && charNumber < 10)
            *buffer = charNumber  + '0';
          if(charNumber > 10 && charNumber < 16)
            *buffer = charNumber + 'A' - 10;
          if(charNumber == 0)
            *buffer = '0';
       } //debug
  return;
}

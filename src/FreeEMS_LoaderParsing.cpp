/*
 * FreeEMS_LoaderParsing.cpp
 *
 *  Created on: Dec 10, 2010
 *      Author: seank
 */

#include "FreeEMS_LoaderParsing.h"
#include "freeems_loader_types.h"

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

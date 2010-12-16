/*
 * FreeEMS_LoaderParsing.h
 *
 *  Created on: Dec 10, 2010
 *      Author: seank
 */

#ifndef FREEEMS_LOADERPARSING_H_
#define FREEEMS_LOADERPARSING_H_

class FreeEMS_LoaderParsing
{
public:
  FreeEMS_LoaderParsing();
  virtual
  ~FreeEMS_LoaderParsing();
  void asciiPairToChars(char *ascii, char *eightBit);
  static int calcuateNumRecordsNeeded(int flashBytes, int bytesInRecord);
};

#endif /* FREEEMS_LOADERPARSING_H_ */

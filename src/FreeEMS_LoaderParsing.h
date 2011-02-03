/*
 * FreeEMS_LoaderParsing.h
 *
 *  Created on: Dec 10, 2010
 *      Author: seank
 */

#ifndef FREEEMS_LOADERPARSING_H_
#define FREEEMS_LOADERPARSING_H_

#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;
//using namespace boost;

class FreeEMS_LoaderParsing
{
public:
  FreeEMS_LoaderParsing();
  virtual
  ~FreeEMS_LoaderParsing();
  static unsigned char
  asciiPairToChar(char *ascii_pair);
  //static unsigned char asciiPairsToChars(char *ascii_pair, char *buffer);
  static unsigned char
  asciiPairToChar(const char *ascii_pair);
  static void
  asciiPairToArray(string* inString, unsigned char* destBuffer, int numChars);
  static void
  asciiPairToArray(char* inString, char* destBuffer, int numChars);
  static void
  intToHexAscii(int number, char* buffer, unsigned char numBitsToConvert);
  static int
  calcuateNumRecordsNeeded(int flashBytes, int bytesInRecord);
};

#endif /* FREEEMS_LOADERPARSING_H_ */

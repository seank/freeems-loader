/*
 * FreeEMS_LoaderParsing.cpp
 *
 *  Created on: Dec 10, 2010
 *      Author: seank
 */

#include "FreeEMS_LoaderParsing.h"
#include <string>
#include <algorithm>
#include <iostream>
#include "stdio.h"

//#include "freeems_loader_types.cpp"

using namespace std;

FreeEMS_LoaderParsing::FreeEMS_LoaderParsing()
{
  // TODO Auto-generated constructor stub
}

FreeEMS_LoaderParsing::~FreeEMS_LoaderParsing()
{
  // TODO Auto-generated destructor stub
}

unsigned char
FreeEMS_LoaderParsing::asciiPairToChar(char *ascii_pair)
{
  int i;
  unsigned char byte = 0;
  char pair_char = 0;
  for(i = 0; i < 2; i++) // loop twice
    {
      pair_char = ascii_pair[i];
      if(pair_char >= '0' && pair_char <= '9')
          {
          byte |= pair_char - '0';
          }
        else if(pair_char >= 'A' && pair_char <= 'F' )
          {
            byte |= (pair_char - 'A') + 10;
          }
        else
          {
            cout<<"error converting ascii hiNibble: out of range";
          }
      if(i == 0) // right shift the first pair
        byte <<= 4;
    }
  return byte;
}

unsigned char //TODO CLEANUP AND REMOVE THIS DUPLICATE
FreeEMS_LoaderParsing::asciiPairToChar(const char *ascii_pair)
{
  int i;
  unsigned char byte = 0;
  char pair_char = 0;
  for(i = 0; i < 2; i++) // loop twice
    {
      pair_char = ascii_pair[i];
      if(pair_char >= '0' && pair_char <= '9')
          {
          byte |= pair_char - '0';
          }
        else if(pair_char >= 'A' && pair_char <= 'F' )
          {
            byte |= (pair_char - 'A') + 10;
          }
        else
          {
            cout<<"error converting ascii hiNibble: out of range";
          }
      if(i == 0) // right shift the first pair
        byte <<= 4;
    }
  return byte;
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
          if(charNumber >= 0 && charNumber < 10)
            *buffer = charNumber + '0';
          if(charNumber >= 10 && charNumber < 16)
            *buffer = charNumber + 'A' - 10;
       } //debug
  return;
}

void
FreeEMS_LoaderParsing::asciiPairToArray(string* inString, unsigned char* destBuffer, int numChars)
{
  int i, j;
  for(i = 0, j = 0; j < numChars; i += 2, j++)
    {
      *(destBuffer + j) =  asciiPairToChar((inString->c_str() + i));
    }
}

void
FreeEMS_LoaderParsing::asciiPairToArray(char* inString, char* destBuffer, int numChars)
{
  int i, j;
  char value;
  for(i = 0, j = 0; j < numChars; i += 2, j++)
    {
       value =  asciiPairToChar((inString + i));
       *(destBuffer + j) = value;
    }
}

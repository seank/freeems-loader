/* FreeEMS-Loader- the open source s19 loader with special features for FreeEMS
 *
 * Copyright (C) 2008-2011 by Sean Keys <skeys@powerefi.com>
 *
 * This file is part of the FreeEMS-Loader project.
 *
 * FreeEMS-Loader software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FreeEMS-Loader software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with any FreeEMS-Loader software.  If not, see http://www.gnu.org/licenses/
 *
 * We ask that if you make any changes to this file you email them upstream to
 * us at info(at)powerefi(dot)com or, even better, fork the code on github.com!
 *
 * Thank you for choosing FreeEMS-Loader to load your firmware!
 *
 */

#include "inc/parsing.h"
#include <string>
#include <algorithm>
#include <iostream>
#include "stdio.h"

//#include "freeems_loader_types.cpp"

using namespace std;

FreeEMS_LoaderParsing::FreeEMS_LoaderParsing() {
	// TODO Auto-generated constructor stub
}

FreeEMS_LoaderParsing::~FreeEMS_LoaderParsing() {
	// TODO Auto-generated destructor stub
}

unsigned char FreeEMS_LoaderParsing::asciiPairToChar(char *ascii_pair) {
	int i;
	unsigned char byte = 0;
	char pair_char = 0;
	for (i = 0; i < 2; i++) // loop twice
	{
		pair_char = ascii_pair[i];
		if (pair_char >= '0' && pair_char <= '9') {
			byte |= pair_char - '0';
		} else if (pair_char >= 'A' && pair_char <= 'F') {
			byte |= (pair_char - 'A') + 10;
		} else {
			cout << "error converting ascii hiNibble: out of range";
		}
		if (i == 0) // right shift the first pair
			byte <<= 4;
	}
	return byte;
}
//TODO CLEANUP AND REMOVE THIS DUPLICATE
unsigned char FreeEMS_LoaderParsing::asciiPairToChar(const char *ascii_pair) {
	int i;
	unsigned char byte = 0;
	char pair_char = 0;
	for (i = 0; i < 2; i++) // loop twice
	{
		pair_char = ascii_pair[i];
		if (pair_char >= '0' && pair_char <= '9') {
			byte |= pair_char - '0';
		} else if (pair_char >= 'A' && pair_char <= 'F') {
			byte |= (pair_char - 'A') + 10;
		} else {
			cout << "error converting ascii hiNibble: out of range";
		}
		if (i == 0) // right shift the first pair
			byte <<= 4;
	}
	return byte;
}

int FreeEMS_LoaderParsing::calcuateNumRecordsNeeded(int flashBytes, int bytesInRecord) {
	return (int) flashBytes / bytesInRecord;
}

void FreeEMS_LoaderParsing::intToHexAscii(int number, char* buffer, unsigned char numBitsToConvert) {
	char charNumber = 0;
	int i;
	for (i = numBitsToConvert / 4, buffer += (i - 1); i > 0; i--, number >>= 4, buffer--) {
		charNumber = (number & 0x0F);
		if (charNumber >= 0 && charNumber < 10)
			*buffer = charNumber + '0';
		if (charNumber >= 10 && charNumber < 16)
			*buffer = charNumber + 'A' - 10;
	} //debug
	return;
}

void FreeEMS_LoaderParsing::asciiPairToArray(string* inString, unsigned char* destBuffer, int numChars) {
	int i, j;
	for (i = 0, j = 0; j < numChars; i += 2, j++) {
		*(destBuffer + j) = asciiPairToChar((inString->c_str() + i));
	}
}

void FreeEMS_LoaderParsing::asciiPairToArray(char* inString, char* destBuffer, int numChars) {
	int i, j;
	char value;
	for (i = 0, j = 0; j < numChars; i += 2, j++) {
		value = asciiPairToChar((inString + i));
		*(destBuffer + j) = value;
	}
}

//TODO the recognizable record types should not be static
bool FreeEMS_LoaderParsing::lineIsLoadable(string* line) {
	if ((line->find("S3")) == 0 || (line->find("S2")) == 0 || ((line->find("S1")) == 0))
		return true;
	return false;
}

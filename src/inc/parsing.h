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

#ifndef PARSING_H
#define PARSING_H

#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;

class FreeEMS_LoaderParsing {
public:
	FreeEMS_LoaderParsing();
	virtual ~FreeEMS_LoaderParsing();
	static unsigned char asciiPairToChar(char *ascii_pair);
	//static unsigned char asciiPairsToChars(char *ascii_pair, char *buffer);
	bool lineIsLoadable(string* line);
	static unsigned char asciiPairToChar(const char *ascii_pair);
	static void asciiPairToArray(string* inString, unsigned char* destBuffer, int numChars);
	static void asciiPairToArray(char* inString, char* destBuffer, int numChars);
	static void intToHexAscii(int number, char* buffer, unsigned char numBitsToConvert);
	static int calcuateNumRecordsNeeded(int flashBytes, int bytesInRecord);
};

#else
/* let us know if we are being untidy with headers */
#warning "Header file PARSING_H seen before, sort it out!"
/* end of the wrapper ifdef from the very top */
#endif


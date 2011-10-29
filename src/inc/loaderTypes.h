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

#ifndef LOADERTYPES_H
#define LOADERTYPES_H

#define ODD	1
#define EVEN	2
#define MARK	3
#define SPACE	4

#define MAXSNINTEENPAYLOAD      0x14

#define FTX512K4        0x0001
#define EETX4K          0x0002

#define FIXED_FLASH     0x0001
#define EEPROM          0x0002
#define PAGED_FLASH     0x0004
#define PPAGE_REGISTER_ADDRESS  0x30

#define S2_PAIR_COUNT_OFFSET    0x02
#define S2_ADDRESS_OFFSET       0x04
#define S2_PAYLOAD_OFFSET       0x0A

enum ACTION {
	NO_ACTION, EXECUTE_ERASE, EXECUTE_RIP, EXECUTE_RIP_ERASE_LOAD, EXECUTE_LOAD, TEST
};

enum MESSAGE_TYPE {
	MESSAGE_INFO = 1, MESSAGE_DEBUG, MESSAGE_ARGUMENT, MESSAGE_GENERIC, MESSAGE_ERROR
};

enum LOADER_STATE {
	STATE_NOT_CONNECTED = 1, STATE_CONNECTED, STATE_WORKING, STATE_ERROR, STATE_FILELOADED
};

enum s19ID {
	S0, S1, // The type of record field is 'S1' (0x5331)
	S2, // The type of record field is 'S2' (0x5332)
	S3, // The type of record field is 'S3' (0x5333)
	S4,
	S5, // The type of record field is 'S5' (0x5335)
	S6,
	S7, // The type of record field is 'S7' (0x5337)
	S8, // The type of record field is 'S8' (0x5338)
	S9
	// The type of record field is 'S9' (0x5339)
};

struct dataVector_tag {
	const char *association;
	unsigned int startAddress;
	unsigned int stopAddress;
	unsigned int startPage;
	unsigned int stopPage;
	unsigned int memType;
	unsigned int pageRegister;
};

struct flashModuleInfo_tag {
	const char *name;
	unsigned int flashModule;
	unsigned int EEPROMModule;
	int numFlashBytes;
};

struct s19Info {
	int type;
	const char *description;
	const char *s19ID; // type in ascii
	int addressBytes;
	bool dataSequence;
};

extern const struct dataVector_tag dataVectorTable[];
extern const struct flashModuleInfo_tag flashModuleTable[];
extern const struct s19Info s19Table[];
extern const int numDataVectorTableEntries;
extern const char PPageRegister;
extern const char Zero;
extern const char SMReadByte;
extern const char SMWriteByte;
extern const char SMReadBlock;
extern const char SMErasePage;
extern const char SMWriteBlock;
extern const char* defFlashType;
extern const char SMReturn;
extern const char SMReset;
extern const char SMRDY[3];

#else
/* let us know if we are being untidy with headers */
#warning "Header file LOADERTYPES_H seen before, sort it out!"
/* end of the wrapper ifdef from the very top */
#endif


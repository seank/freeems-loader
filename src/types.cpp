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

#include "inc/loaderTypes.h"
#include <string>

const struct dataVector_tag dataVectorTable[] = { { "S12XDP512", 0x8000, 0xBFFF, 0xE0, 0xFF, PAGED_FLASH,
		PPAGE_REGISTER_ADDRESS },
		//{"S12XDP512", 0xC000, 0xFFFF, 0xFC, 0xDF, PAGED_FLASH, PPAGE_REGISTER_ADDRESS},//TODO impliment EEPROM
		{ "S12C64", 0xC000, 0xFFFF, 0xFC, 0xDF, PAGED_FLASH, PPAGE_REGISTER_ADDRESS }, { "0", 0, 0, 0, 0, 0, 0 } };

const struct flashModuleInfo_tag flashModuleTable[] = { { "S12XDP512", FTX512K4, EETX4K, 0x80000 }, { "S12C64", 0, 0,
		0x10000 }, // TODO get correct values
		{ "0", 0, 0, 0 } };

const struct s19Info s19Table[] = { { S0, "Block header", "S0", 2, true }, { S1, "Data sequence", "S1", 2, true }, { S2,
		"Data sequence", "S2", 3, true }, { S3, "Data sequence", "S3", 4, true },
		{ S5, "Record count", "S5", 2, false }, { S7, "End of block", "S7", 0, false }, { S8, "End of block", "S8", 3,
				false }, { S9, "End of block", "S9", 2, false }, { 0, 0, 0, 0, false } };

const int numDataVectorTableEntries = sizeof(dataVectorTable) / sizeof(struct dataVector_tag);
const char *defFlashType = "S12XDP512";

const char PPageRegister = 0x30;
const char Zero = 0x00;
const char SMReturn = 0x0D;
const char SMRDY[3] = { 0xe0, 0x00, 0x3e };

const char SMReadByte = 0xA1; /*  — Read_Byte  A1/AAAA/RD — Reads a byte of data from the specified 16-bit address and
   sends the 8-bit data back to the host PC. This routine assumes that accesses
   to the paged memory area have been preceded by a PPAGE register access
   (Write_Byte 0030 = page) to select the appropriate page. */

const char SMWriteByte = 0xA2; /*  — Write_Byte A2/AAAA/WD — Writes the supplied byte of data to the specified 16-bit
   address. This routine assumes that accesses to the paged memory area have
   been preceded by a PPAGE register access (Write_Byte 0030 = page) to
   select the appropriate page. All writes are processed through an intelligent
   routine that programs FLASH/EEPROM or writes to RAM or registers based on
   the address being written. If any error occurs during an attempt to program a
   nonvolatile memory location, an error code is transmitted before a new prompt
   is issued. See Intelligent Writes for details. */

const char SMReadWord = 0xA3; /*  — Read_Word A3/AAAA/RDW — Reads a word of data from the specified 16-bit address and
   sends the 16-bit data back to the host PC. This routine assumes that accesses
   to the paged memory area have been preceded by a PPAGE register access
   (Write_Byte 0030 = page) to select the appropriate page. */

const char SMWriteWord = 0xA4; /* — Write_Word A4/AAAA/WW — Writes the supplied word of data to the specified 16-bit
   address. This routine assumes that accesses to the paged memory area have
   been preceded by a PPAGE register access (Write_Byte 0030 = page) to
   select the appropriate page. All writes are processed through an intelligent
   routine that programs FLASH/EEPROM or writes to RAM or registers based on
   the address being written. If any error occurs during an attempt to program a
   nonvolatile memory location, an error code is transmitted before a new prompt
   is issued. See Intelligent Writes for details. */

const char SMReadNext = 0xA5; /*  — Read_Next A5/RDW — Pre-increments the user IX register (by 2), reads the word of data
   from the address pointed to by IX, and sends the 16-bit data back to the host
   PC. This routine assumes that accesses to the paged memory area have been
   preceded by a PPAGE register access (Write_Byte 0030 = page) to select the
   appropriate page. This command is not allowed when the user program is
   running. If executed during run mode, this command will return an $E2 —
   Command Not Allowed in Run Mode error and $0000 data. */

const char SMWriteNext = 0xA6; /* — Write_Next A6/WW — Pre-increments the user IX register (by 2) and writes the supplied
   word to the address pointed to by IX. This routine assumes that accesses to
   the paged memory area have been preceded by a PPAGE register access
   (Write_Byte 0030 = page) to select the appropriate page. All writes are
   processed through an intelligent routine that programs FLASH/EEPROM or
   writes to RAM or registers based on the address being written. If any error
   occurs during an attempt to program a nonvolatile memory location, an error
   code is transmitted before a new prompt is issued. See Intelligent Writes for
   details. This command is not allowed when the user program is running. If
   executed during run mode, this command will return an $E2 — Command Not
   Allowed in Run Mode error and the write data will be ignored. */

const char SMReadBlock = 0xA7; /* — Read_Block A7/AAAA/NN/RDB(AAAA) /RDB(AAAA+1) /.../RDB(AAAA+NN) — Reads a series of NN+1 (1
   to 256) bytes of data starting at address AAAA and returns the data one byte at a time
   to the host starting with the data read from address AAAA and ending with the data
   from address AAAA+NN. This routine assumes that accesses to the paged memory
   area have been preceded by a PPAGE register access (Write_Byte 0030 = page) to
   select the appropriate page. Although this command can be executed while a user
   program is running, it is not recommended because it could slow down operation of the
   user program. */

const char SMWriteBlock = 0xA8; /* — Write_Block A8/AAAA/NN/WB(AAAA) /WB(AAAA+1) /.../WB(AAAA+NN) — Writes a series of NN+1 (1 to
   256) bytes of data into the target MCU memory starting at address AAAA and ending
   with address AAAA+NN. This routine assumes that accesses to the paged memory
   area have been preceded by a PPAGE register access (Write_Byte 0030 = page) to
   select the appropriate page. All writes are processed through an intelligent routine that
   programs FLASH/EEPROM or writes to RAM or registers based on the address being
   written. If any error occurs during an attempt to program a nonvolatile memory location,
   an error code is transmitted before a new prompt is issued. See Intelligent Writes for
   details. Although this command can be executed while a user program is running, it is
   not recommended because it could slow down operation of the user program. */

const char SMErasePage = 0xB8; /* — Erase one page of FLASH memory selected by the current PPAGE
   register. $E6 error code will be returned if the command does not complete
   successfully. PPAGE must be preloaded with desired page to erase. */

const char SMReset = 0xB4; /* When a user reset vector is programmed, the levels on the run/load
   switch and the RxD0 line could cause a reset to either the user code or the
   monitor. The sequence of checks to determine the type of reset is listed and
   illustrated in Figure 1.
      1. If the first byte of the user reset pseudo-vector = $FF
          (unprogrammed), force monitor reset.
      2. If the run/load switch = 0 (logic low), force monitor reset.
      3. If RxD0 = 0 (logic low), force monitor reset.
      4. If none of the above, use the reset pseudo-vector to jump to the
         user reset start-up routine. */

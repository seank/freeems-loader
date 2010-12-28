/*
 * freeems_loader_types.c
 *
 *  Created on: Dec 28, 2010
 *      Author: seank
 */

#include "freeems_loader_types.h"

const struct dataVector_tag dataVectorTable[] = {
    {"S12XDP512", 0xC000, 0xFFFF, 0xFF, 0xFF, PAGED_FLASH, PPAGE_REGISTER_ADDRESS},
    {"S12XDP512", 0x8000, 0xBFFF, 0xFE, 0xFE, PAGED_FLASH, PPAGE_REGISTER_ADDRESS},
    {"S12XDP512", 0x4000, 0x7FFF, 0xFD, 0xFD, PAGED_FLASH, PPAGE_REGISTER_ADDRESS},
    {"S12XDP512", 0xC000, 0xFFFF, 0xFC, 0xDF, PAGED_FLASH, PPAGE_REGISTER_ADDRESS},
    {"S12C64"   , 0xC000, 0xFFFF, 0xFC, 0xDF, PAGED_FLASH, PPAGE_REGISTER_ADDRESS},
    {          0,      0,      0,    0,    0,           0,                      0}
};

const struct flashModuleInfo_tag flashModuleTable[] = {
    {"S12XDP512", FTX512K4, EETX4K, 0x80000},
    {"S12C64", 0, 0, 0x10000}, // TODO get correct values
    {       0, 0, 0,       0}
};

const struct s19Info s19Table[] = {
            {S0, "Block header", "S0", 2, true},
            {S1, "Data sequence", "S1", 2, true},
            {S2, "Data sequence", "S2", 3, true},
            {S3, "Data sequence", "S3", 4, true},
            {S5, "Record count", "S5", 2, false},
            {S7, "End of block", "S7", 0, false},
            {S8, "End of block", "S8", 3, false},
            {S9, "End of block", "S9", 2, false},
            { 0,              0,    0, 0, false}
};


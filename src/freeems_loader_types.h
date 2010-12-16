#ifndef FREEEMS_LOADER_TYPES_H
#define FREEEMS_LOADER_TYPES_H

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

struct dataVector{
  char *association;
  unsigned int startAddress;
  unsigned int stopAddress;
  unsigned int startPage;
  unsigned int stopPage;
  unsigned int memType;
  unsigned int pageRegister;
};

struct flashModuleInfo{
  char *name;
  unsigned int flashModule;
  unsigned int EEPROMModule;
  int numFlashBytes;
};

const struct dataVector dataVectorTable[]
{
    {"S12XDP512", 0xC000, 0xFFFF, 0xFF, 0xFF, PAGED_FLASH, PPAGE_REGISTER_ADDRESS},
    {"S12XDP512", 0x8000, 0xBFFF, 0xFE, 0xFE, PAGED_FLASH, PPAGE_REGISTER_ADDRESS},
    {"S12XDP512", 0x4000, 0x7FFF, 0xFD, 0xFD, PAGED_FLASH, PPAGE_REGISTER_ADDRESS},
    {"S12XDP512", 0xC000, 0xFFFF, 0xFC, 0xDF, PAGED_FLASH, PPAGE_REGISTER_ADDRESS},
    {"S12C64", 0xC000, 0xFFFF, 0xFC, 0xDF, PAGED_FLASH, PPAGE_REGISTER_ADDRESS},
    {0,0,0,0,0}
};

const struct flashModuleInfo flashModuleTable[]
{
    {"S12XDP512", FTX512K4, EETX4K, 0x80000},
    {"S12C64", 0, 0, 0x10000}, // TODO get correct values
    {0,0,0,0}
};

#endif

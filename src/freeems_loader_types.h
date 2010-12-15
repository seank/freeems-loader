#ifndef FREEEMS_LOADER_TYPES_H
#define FREEEMS_LOADER_TYPES_H

#define ODD	1
#define EVEN	2
#define MARK	3
#define SPACE	4
#define MAXSNINTEENPAYLOAD      0x14

//typedef struct  {
//	char *port;
//	unsigned int  baudrate;
//	unsigned int  databits;
//	unsigned int  parity;
//	unsigned int  stop;
//	unsigned int  softwareHandshake;
//	unsigned int  hardwareHandshake;
//}serialComSettings;

struct flashModuleInfo{
  char *name;
  int startFlashAddress;
  int endFlashAddress;
  int startPagedFlashAddress;
  int endPagedFlashAddress;
  int numFlashBytes;
};

const struct flashModuleInfo flashModuleTable[]
{
    {"S12XDP512", 0x4000, 0x7FFF, 0x8000, 0xBFFF, 0x80000},
    {"S12C64", 0x4000, 0x7FFF, 0x8000, 0xBFFF, 0x10000}, // TODO get correct values
    {0,0,0,0,0}
};

#endif

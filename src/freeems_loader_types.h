#ifndef FREEEMS_LOADER_TYPES_H
#define FREEEMS_LOADER_TYPES_H

#define ODD		1
#define EVEN	2
#define MARK	3
#define SPACE	4

typedef struct  {
	char *port;
	unsigned int  baudrate;
	unsigned int  databits;
	unsigned int  parity;
	unsigned int  stop;
	unsigned int  softwareHandshake;
	unsigned int  hardwareHandshake;
}serialComSettings;

#endif

#ifndef FREEEMS_LOADER_TYPES_H
#define FREEEMS_LOADER_TYPES_H

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

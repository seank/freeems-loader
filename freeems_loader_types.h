#ifndef FREEEMS_LOADER_TYPES_H
#define FREEEMS_LOADER_TYPES_H

typedef struct  {
	int baudrate;
	int databits;
	int parity;
	int stop;
	int softwareHandshake;
	int hardwareHandshake;
}serialComSettings;

#endif

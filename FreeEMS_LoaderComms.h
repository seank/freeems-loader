/*
 * FreeEMS_LoaderComms.h
 *
 *
 *  Created on: Oct 29, 2010
 *      Author: seank
 */

#ifndef FREEEMS_LOADERCOMMS_H_
#define FREEEMS_LOADERCOMMS_H_

#include "freeems_loader_types.h"

#define BUFF_SIZE	4096
#define CARRIAGE_RETURN	0x0D
#define SM_PROMPT	"\0xE0\0x08"

class FreeEMS_LoaderComms {
public:
	FreeEMS_LoaderComms();
	int smReady;
//	int serial_fd; /* serial file descriptor*/
	int fdConfigured;
//	int connected;

	int serialConnect(serialComSettings *settings);
	int initPort(int fd, serialComSettings *settings);
	int readBytes(char *buffer, int numBytes);
	int checkSM();
	int writeBytes(char *buffer, int numBytes);

	virtual ~FreeEMS_LoaderComms();
};

#endif /* FREEEMS_LOADERCOMMS_H_ */

/*
 * FreeEMS_LoaderComms.h
 *
 *  Created on: Oct 29, 2010
 *      Author: seank
 */

#ifndef FREEEMS_LOADERCOMMS_H_
#define FREEEMS_LOADERCOMMS_H_

#include "freeems_loader_types.h"

class FreeEMS_LoaderComms {
public:
	FreeEMS_LoaderComms();

	int serialConnect(serialComSettings *settings);
	int initPort(int fd, serialComSettings *settings);

	virtual ~FreeEMS_LoaderComms();
};

#endif /* FREEEMS_LOADERCOMMS_H_ */

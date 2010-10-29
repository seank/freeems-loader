/*
 * FreeEMS_LoaderComms.h
 *
 *  Created on: Oct 29, 2010
 *      Author: seank
 */

#ifndef FREEEMS_LOADERCOMMS_H_
#define FREEEMS_LOADERCOMMS_H_

class FreeEMS_LoaderComms {
public:
	FreeEMS_LoaderComms();
	char* SerialConnect(char*);
	virtual ~FreeEMS_LoaderComms();
};

#endif /* FREEEMS_LOADERCOMMS_H_ */

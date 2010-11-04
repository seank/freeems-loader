/*
 * FreeEMS_LoaderComms.cpp
 *
 *  Created on: Oct 29, 2010
 *      Author: seank
 */

#include "FreeEMS_LoaderComms.h"


#include <termios.h>


FreeEMS_LoaderComms::FreeEMS_LoaderComms() {
	// TODO Auto-generated constructor stub

}

FreeEMS_LoaderComms::~FreeEMS_LoaderComms() {
	// TODO Auto-generated destructor stub
}


int FreeEMS_LoaderComms::serialConnect(serialComSettings *settings) {

	int status = 0;
	settings->baudrate = 0;


	return status;
}

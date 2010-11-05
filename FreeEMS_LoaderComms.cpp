/*
 * FreeEMS_LoaderComms.cpp
 *
 *  Created on: Oct 29, 2010
 *      Author: seank
 */

#include "FreeEMS_LoaderComms.h"

#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

static int fd; /* access to serial file data */
static int fdConfigured;

FreeEMS_LoaderComms::FreeEMS_LoaderComms() {
	// TODO Auto-generated constructor stub

}

FreeEMS_LoaderComms::~FreeEMS_LoaderComms() {
	// TODO Auto-generated destructor stub
}


int FreeEMS_LoaderComms::serialConnect(serialComSettings *settings) {

	if(!fdConfigured)
		initPort(fd, settings);

	fd = open(settings->port, O_RDWR | O_NOCTTY | O_NDELAY);
		if (fd == -1) {
			perror("open_port: Unable to open /dev/ttyS0 - ");
			return 1;
		} else {
			fcntl(fd, F_SETFL, 0);
		}

	return 0;
}

int FreeEMS_LoaderComms::initPort(int fd, serialComSettings *settings)
{
	 	struct termios options;
		// Get the current options for the port...
		tcgetattr(fd, &options);

		printf("\n Current port settings are as follows speed=%d",(int)options.c_ispeed);

		// Set the baud rates to ...
		cfsetispeed(&options, settings->baudrate);
		cfsetospeed(&options, settings->baudrate);
		// Enable the receiver and set local mode...
		options.c_cflag |= (CLOCAL | CREAD);

		options.c_cflag &= ~PARENB;
		options.c_cflag &= ~(settings->stop);
		options.c_cflag &= ~CSIZE;
		options.c_cflag |= CS8;

		// Set the new options for the port...
		tcsetattr(fd, TCSANOW, &options);
		return 1;
}

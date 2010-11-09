/*
 * FreeEMS_LoaderComms.cpp
 *
 *  Created on: Oct 29, 2010
 *      Author: seank
 */

#include "FreeEMS_LoaderComms.h"

#include <iostream>
#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

#include <errno.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/ioctl.h>
#include <sys/termios.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <fcntl.h>

using namespace std;

/* static vars  */
static int fd; /* access to serial file data */
static int fdConfigured;
static char comInBuffer[BUFF_SIZE];
static char comOutBuffer[BUFF_SIZE];

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
			perror("open_port: Unable to open port - ");
			printf("%s",settings->port);
			return 1;
		} else {
			fcntl(fd, F_SETFL, 0);
		}

	return 0;
}

int FreeEMS_LoaderComms::initPort(int fd, serialComSettings *settings)
{
	 	struct termios options;

	 	speed_t _baud = 0;

	 	switch(settings->baudrate){
	#ifdef B0
	 	case	0: _baud=B0;     break;
	#endif
	#ifdef B50
	 	case     50: _baud=B50;    break;
	#endif
	#ifdef B75
	 	case     75: _baud=B75;    break;
	#endif
	#ifdef B110
	 	case    110: _baud=B110;   break;
	#endif
	#ifdef B134
	 	case    134: _baud=B134;   break;
	#endif
	#ifdef B150
	 	case    150: _baud=B150;   break;
	#endif
	#ifdef B200
	 	case    200: _baud=B200;   break;
	#endif
	#ifdef B300
	 	case    300: _baud=B300;   break;
	#endif
	#ifdef B600
	 	case    600: _baud=B600;   break;
	#endif
	#ifdef B1200
	 	case   1200: _baud=B1200;  break;
	#endif
	#ifdef B1800
	 	case   1800: _baud=B1800;  break;
	#endif
	#ifdef B2400
	 	case   2400: _baud=B2400;  break;
	#endif
	#ifdef B4800
	 	case   4800: _baud=B4800;  break;
	#endif
	#ifdef B7200
	 	case   7200: _baud=B7200;  break;
	#endif
	#ifdef B9600
	 	case   9600: _baud=B9600;  break;
	#endif
	#ifdef B14400
	 	case  14400: _baud=B14400; break;
	#endif
	#ifdef B19200
	 	case  19200: _baud=B19200; break;
	#endif
	#ifdef B28800
	 	case  28800: _baud=B28800; break;
	#endif
	#ifdef B38400
	 	case  38400: _baud=B38400; break;
	#endif
	#ifdef B57600
	 	case  57600: _baud=B57600; break;
	#endif
	#ifdef B76800
	 	case  76800: _baud=B76800; break;
	#endif
	#ifdef B115200
	 	case 115200: _baud=B115200; break;
	#endif
	#ifdef B128000
	 	case 128000: _baud=B128000; break;
	#endif
	#ifdef B230400
	 	case 230400: _baud=B230400; break;
	#endif
	#ifdef B460800
	 	case 460800: _baud=B460800; break;
	#endif
	#ifdef B576000
	 	case 576000: _baud=B576000; break;
	#endif
	#ifdef B921600
	 	case 921600: _baud=B921600; break;
	#endif
	 	default:
	 		printf("\n Invalid BAUD rate setting %d", settings->baudrate);
		break;
	 	}

	 	switch(settings->databits){
	 	case 5:
	 		options.c_cflag = (options.c_cflag & ~CSIZE) | CS5;
	 		break;
	 	case 6:
	 		options.c_cflag = (options.c_cflag & ~CSIZE) | CS6;
	 		break;
	 	case 7:
	 		options.c_cflag = (options.c_cflag & ~CSIZE) | CS7;
	 		break;
	 	case 8:
	 		options.c_cflag = (options.c_cflag & ~CSIZE) | CS8;
	 		break;
	 	default:
	 		printf("\n invalid databits setting %d", settings->databits);
	 		break;
	 	}

	 	// Set the baud rates to ...
	 	cfsetispeed(&options, (speed_t)_baud);
	 	cfsetospeed(&options, (speed_t)_baud);

	 	options.c_cflag |= CLOCAL | CREAD;

	 	//parity
	 	options.c_cflag &= ~(PARENB | PARODD);
	 	if (settings->parity == EVEN) /* even */
	 	 {
	 	   options.c_cflag |= PARENB;
	 	 }
	 	else if (settings->parity == ODD) /* odd */
	 	   {
	 	     options.c_cflag |= (PARENB | PARODD);
	 	   }

	 	options.c_cflag &= ~CRTSCTS;

	 	//stopbits
	 	if (settings->stop == 2)
	 	{
	 		options.c_cflag |= CSTOPB;
	 	}
	 	else
	 	{
	 	    options.c_cflag &= ~CSTOPB;
	 	}

	 	//   options.c_iflag=IGNPAR | IGNBRK;
	 	options.c_iflag=IGNBRK;
	 	//   options.c_iflag=IGNPAR;

	 	//software handshake
	 	if (settings->softwareHandshake)
	 	{
	 		options.c_iflag |= IXON | IXOFF;
	 	}
	 	else
	 	{
	 		options.c_iflag &= ~(IXON|IXOFF|IXANY);
	 	}

	 	options.c_lflag=0;
	 	options.c_oflag=0;

	 	options.c_cc[VTIME]=1;
	 	options.c_cc[VMIN]=60;

	    //   tcflush(m_fd, TCIFLUSH);
	    if (tcsetattr(fd, TCSANOW, &options)!=0)
	    {
	    	std::cerr<<"tcsetattr() 1 failed"<<std::endl;
	 	}

	 	int mcs=0;
	 	ioctl(fd, TIOCMGET, &mcs);
	 	mcs |= TIOCM_RTS;
	 	ioctl(fd, TIOCMSET, &mcs);

	 	if (tcgetattr(fd, &options)!=0)
	 	{
	 		std::cerr<<"tcgetattr() 4 failed"<<std::endl;
	 	}

	 	//hardware handshake
	 	if (settings->hardwareHandshake)
	 	{
	 		options.c_cflag |= CRTSCTS;
	 	}
	 	else
	 	{
	 		options.c_cflag &= ~CRTSCTS;
	 	}
	 	/*  if (on)
	 	options.c_cflag |= CRTSCTS;
	 	else
	 	options.c_cflag &= ~CRTSCTS;*/
	    if (tcsetattr(fd, TCSANOW, &options)!=0)
	    {
	 	std::cerr<<"tcsetattr() 2 failed"<<std::endl;
	 	}

		return 1;
}

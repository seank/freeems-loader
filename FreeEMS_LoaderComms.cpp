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
static int serial_fd; /* serial file descriptor*/
//static int fdConfigured;
static int connected;
//static int smReady;
static char comInBuffer[BUFF_SIZE];
//static char comOutBuffer[BUFF_SIZE];

void millisleep(int ms)
{
   if (ms>0)
   {
      struct timeval tv;
      tv.tv_sec=0;
      tv.tv_usec=ms*1000;
      select(0, 0, 0, 0, &tv);
   }
}

FreeEMS_LoaderComms::FreeEMS_LoaderComms() {
	// TODO Auto-generated constructor stub
}

FreeEMS_LoaderComms::~FreeEMS_LoaderComms() {
	// TODO Auto-generated destructor stub
}


int FreeEMS_LoaderComms::serialConnect(serialComSettings *settings) {

	if(!connected)
		{
			serial_fd = open(settings->port, O_RDWR | O_NOCTTY | O_NDELAY);
			if (serial_fd == -1) {
				cout << "unable to open port" << (char*)settings->port;
				//perror("open_port: Unable to open port - ");
				return -1;
			} else {
				connected = 1;
				std::cout<<"port opened";
			}
		}
		else
		{
			return -1;
		}

	if(!fdConfigured)
	{
		if(initPort(serial_fd, settings) < 0)
		{
			std::cout<<"error configuring port";
			return -1;
		}
		else
		{
			fdConfigured = 1;
		}
	}

	return 0;
}

int FreeEMS_LoaderComms::initPort(int fd, serialComSettings *settings)
{
		/* TODO SAVE OLD OPTIONS */
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
	 		return -5;
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
	 		return -6;
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

	 	/* set input mode (non-canonical, no echo,...) */
	 	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	 	options.c_oflag=0;
	 	options.c_cc[VTIME]=10; /* inter-character timer unused (1 second)*/
	 	options.c_cc[VMIN]=0; /* blocking read until 0 chars received */

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

	 	if (tcsetattr(fd, TCSAFLUSH, &options)!=0)
	    {
	 	std::cerr<<"tcsetattr() 2 failed"<<std::endl;
	 	}

		return 0;
}

int FreeEMS_LoaderComms::readBytes(char *buffer, int numBytes)
{
	/* http://www.easysw.com/~mike/serial/serial.html#2_1
	  Reading data from a port is a little trickier. When you operate the port in raw data
	  mode, each read(2) system call will return the number of characters that are actually
	  available in the serial input buffers. If no characters are available, the call will
	  block (wait) until characters come in, an interval timer expires, or an error occurs.

	   The read function can be made to return immediately by doing the following:

		fcntl(fd, F_SETFL, FNDELAY);

	   The FNDELAY option causes the read function to return 0 if no characters are available
	   on the port. To restore normal (blocking) behavior, call fcntl() without the FNDELAY option:

		fcntl(fd, F_SETFL, 0);

	  This is also used after opening a serial port with the O_NDELAY option.
	 */
	//fcntl(serial_fd, F_SETFL, O_NDELAY);
	return read(serial_fd, buffer, numBytes);
}

int FreeEMS_LoaderComms::writeBytes(char *buffer, int numBytes)
{
	int n;
	n = write(serial_fd, buffer, numBytes);
	if (n < 0)
		std::cerr<<"writeBytes failed"<<std::endl;
	return n;
}

int FreeEMS_LoaderComms::checkSM()
{
	int nBytesRead = 0;
	int nBytesWrote = 0;
	while(readBytes(comInBuffer,1) > 0);
	memset(comInBuffer, 0, sizeof(comInBuffer)); /* clear buffer contents */
	char cr = CARRIAGE_RETURN;
	char ready[4] = {0xE1,0xE00,0x3E};

	nBytesWrote = writeBytes(&cr,1);
	if(!nBytesWrote)
		std::cout<<"Error, wrote zero bytes to serial device";

	millisleep(50);

	nBytesRead = readBytes(comInBuffer, sizeof(comInBuffer));
	if(nBytesRead < 0)
		std::cout<<"Error, read zero bytes from serial device";
	if(!strcmp(comInBuffer, ready))
	{
		smReady = 1;
		std::cout<<"detected SM";
		return 0;
	}else
	{
		smReady = 0;
		std::cout<<"Error detecting SM";
		return -4;
	}
}

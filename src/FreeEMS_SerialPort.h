/*
 * FreeEMS_SerialPort.h
 *
 *  Created on: May 11, 2011
 *      Author: seank
 */

#ifndef FREEEMS_SERIALPORT_H_
#define FREEEMS_SERIALPORT_H_

#include <stdio.h>
#include <iostream>
//#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#ifdef __WIN32__
//#include <winserialio.h>
#include <serialio.h>
#include <winserialio.h>
#include <io.h>
#include <windows.h>
#else
#include <termios.h>
#endif

#include <fcntl.h> // File control definitions
#include <errno.h> // Error number definitions
#include <termios.h> // POSIX terminal control definitionss
#include <time.h>   // time calls

#ifndef CRTSCTS
#define CRTSCTS 0
#endif

#ifndef B115200
#define B115200 115200
#endif

typedef enum
{
	NONE,
	ODD,
	EVEN
}Parity;

typedef enum
{
	INBOUND=0x2E0,
	OUTBOUND,
	BOTH
}FlushDirection;

class FreeEMS_SerialPort {
public:
	FreeEMS_SerialPort();
	virtual ~FreeEMS_SerialPort();
	bool isOpen();

void openPort(char *port_name);
int setupPort(int baud);
void closePort();
void flushInBuffer();
void flushOutBuffer();
void writeData(const char *data, size_t size);
void readData(char *data, size_t size);
int readWrapper(int, char *, size_t size);
void flushSerial(FlushDirection direction);

#ifdef _WIN32_
void win32_setup_serial_params(int, int, int, Parity, int);
void win32_toggle_serial_control_lines(void);
void win32_flush_serial(int, FlushDirection);
#endif

private:
int _fd;
bool _isOpenFlag;
static const int _poll_attempts = 50;

/* Globals */

#ifndef __WIN32__
	struct termios oldtio;
	struct termios newtio;
#endif
	//static char * serial_lockfile = NULL;

#ifdef __WIN32__
struct _Serial_Params
{
	int _fd;		/*! File descriptor */
	char *port_name;	/*! textual name of comm port */
	bool open;		/*! flag, TRUE for open FALSE for closed */
	int read_wait;		/*! time delay between each read */
	int errcount;		/*! Serial I/O errors read error count */
	bool net_mode;	/*! When using TCP/IP socket mode */
};
#else
struct _Serial_Params
{
	int fd;		/*! File descriptor */
	char *port_name;	/*! textual name of comm port */
	bool open;		/*! flag, TRUE for open FALSE for closed */
	int read_wait;		/*! time delay between each read */
	int errcount;		/*! Serial I/O errors read error count */
	bool net_mode;	/*! When using TCP/IP socket mode */
	struct termios oldtio;	/*! serial port settings before we touch it */
	struct termios newtio;	/*! serial port settings we use when running */
#ifdef __PIS_SUPPORT__
	struct serial_struct oldctl;
	struct serial_struct newctl;
#endif
};
#endif

};

#endif /* FREEEMS_SERIALPORT_H_ */

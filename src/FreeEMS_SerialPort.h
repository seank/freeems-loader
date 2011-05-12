/*
 * FreeEMS_SerialPort.h
 *
 *  Created on: May 11, 2011
 *      Author: seank
 */

#ifndef FREEEMS_SERIALPORT_H_
#define FREEEMS_SERIALPORT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#ifdef __WIN32__
#include <winserialio.h>
#else
#include <termios.h>
#endif

#ifndef CRTSCTS
#define CRTSCTS 0
#endif


/* Globals */
#ifndef __WIN32__
	struct termios oldtio;
	struct termios newtio;
#endif
	//static char * serial_lockfile = NULL;

#ifdef __WIN32__
struct _Serial_Params
{
	int fd;		/*! File descriptor */
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

int open_port(char *port_name);
int setup_port(int fd, int baud);
void close_port(int fd);
//void flush_serial(int fd, FlushDirection direction);

	/* Prototypes */
	//bool open_serial(char *, bool);
	//void close_serial(void);
//	void setup_serial_params(void);
//	void toggle_serial_control_lines(void );
//	void flush_serial(int, FlushDirection);
//	//void *serial_repair_thread(gpointer );
//	bool parse_baud_str(char *, int *, int *, Parity *, int *);
//	/* Prototypes */

private:
int fd;

};

#endif /* FREEEMS_SERIALPORT_H_ */

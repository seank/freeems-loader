/* FreeEMS-Loader- the open source s19 loader with special features for FreeEMS
 *
 * Copyright (C) 2008-2011 by Sean Keys <skeys@powerefi.com>
 *
 * This file is part of the FreeEMS-Loader project.
 *
 * FreeEMS-Loader software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FreeEMS-Loader software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with any FreeEMS-Loader software.  If not, see http://www.gnu.org/licenses/
 *
 * We ask that if you make any changes to this file you email them upstream to
 * us at info(at)powerefi(dot)com or, even better, fork the code on github.com!
 *
 * Thank you for choosing FreeEMS-Loader to load your firmware!
 *
 * A special thanks goes out to djandruczyk <dandruczyk@gmail.com> for helping develop native serial IO calls!
 *
 */

#include "inc/serialPort.h"

FreeEMS_SerialPort::FreeEMS_SerialPort() :
_fd(0), _isOpenFlag(false) {
	// TODO Auto-generated constructor stub

}

FreeEMS_SerialPort::~FreeEMS_SerialPort() {
	// TODO Auto-generated destructor stub
}

#ifdef __WIN32__
void win32_setup_serial_params(unsigned int fd, int baud, int bits, Parity parity, int stop)
{
	DCB dcb;
	WSADATA wsaData;
	int res = 0;
	COMMTIMEOUTS timeouts;

	ZeroMemory(&dcb, sizeof(dcb));
	dcb.DCBlength = sizeof(dcb);

	/* Populate struct with defaults from windows */
	GetCommState((HANDLE) _get_osfhandle(fd), &dcb);

	if (baud == 9600)
		dcb.BaudRate = CBR_9600;
	else if (baud == 115200)
		dcb.BaudRate = CBR_115200;
	dcb.ByteSize = bits;
	switch (parity)
	{
	case NONE:
		dcb.Parity = NOPARITY;
		dcb.fParity = FALSE; /* Disabled */
		break;
	case ODD:
		dcb.Parity = ODDPARITY;
		dcb.fParity = TRUE; /* Enabled */
		break;
	case EVEN:
		dcb.Parity = EVENPARITY;
		dcb.fParity = TRUE; /* Enabled */
		break;
	}
	if (stop == 2)
		dcb.StopBits = TWOSTOPBITS; /* #defined in windows.h */
	else
		dcb.StopBits = ONESTOPBIT; /* #defined in windows.h */

	dcb.fBinary = TRUE; /* Enable binary mode */
	dcb.fOutxCtsFlow = FALSE; /* don't monitor CTS line */
	dcb.fOutxDsrFlow = FALSE; /* don't monitor DSR line */
	dcb.fDsrSensitivity = FALSE; /* ignore Dsr line */
	dcb.fDtrControl = DTR_CONTROL_DISABLE; /* Disable DTR line */
	dcb.fRtsControl = RTS_CONTROL_DISABLE; /* Disable RTS line */
	dcb.fOutX = FALSE; /* Disable Xoff */
	dcb.fInX = FALSE; /* Disable Xin */
	dcb.fErrorChar = FALSE; /* Don't replace bad chars */
	dcb.fNull = FALSE; /* don't drop NULL bytes */
	dcb.fAbortOnError = FALSE; /* Don't abort */
	dcb.wReserved = FALSE; /* as per msdn */

	/* Set the port properties and write the string out the port. */
	if(SetCommState((HANDLE) _get_osfhandle (fd) ,&dcb) == 0)
		printf(__FILE__": win32_setup_serial_params()\n\tERROR setting serial attributes\n");

	/* Set timeout params in a fashion that mimics linux behavior */

	GetCommTimeouts((HANDLE) _get_osfhandle (fd), &timeouts);
	/*
	 if (baud == 112500)
	 timeouts.ReadTotalTimeoutConstant    = 250;
	 else
	 timeouts.ReadTotalTimeoutConstant    = 100;
	 timeouts.ReadTotalTimeoutMultiplier  = 1;
	 timeouts.WriteTotalTimeoutMultiplier = 1;
	 timeouts.WriteTotalTimeoutConstant   = 25;
	 */

	timeouts.ReadIntervalTimeout = 0;
	timeouts.ReadTotalTimeoutConstant = 100;
	timeouts.ReadTotalTimeoutMultiplier = 1;
	timeouts.WriteTotalTimeoutConstant = 100;
	timeouts.WriteTotalTimeoutMultiplier = 1;

	SetCommTimeouts((HANDLE) _get_osfhandle (fd) ,&timeouts);

	res = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (res != 0) {
		//printf(_("WSAStartup failed: %d\n"),res);
		printf("WSAStartup failed: %d\n", res);
	}
	return;

}

void
win32_flush_serial(unsigned int fd, FlushDirection mode)
{
	switch (mode)
	{
	case INBOUND:
		PurgeComm((HANDLE) _get_osfhandle (fd),PURGE_RXCLEAR);
		break;
	case OUTBOUND:
		FlushFileBuffers((HANDLE) _get_osfhandle (fd));
		PurgeComm((HANDLE) _get_osfhandle (fd),PURGE_TXCLEAR);
		break;
	case BOTH:
		FlushFileBuffers((HANDLE) _get_osfhandle (fd));
		PurgeComm((HANDLE) _get_osfhandle (fd),PURGE_TXCLEAR|PURGE_RXCLEAR);
		break;
	}
	return;
}
#endif

void FreeEMS_SerialPort::openPort(char * port_name) {
#ifdef __WIN32__
	_fd = open(port_name, O_RDWR | O_BINARY );
#else
	/* NON-block open, then turn to blocking via fcntl so it works
	 nicely on linux and OS-X.  this MUST be done this way because
	 in OS-X some serial devices will BLOCK on open waiting for the
	 control lines to get into the state it wants, thus blocking
	 open() indefinitely.  Thus we open nonblocking then flip the bit
	 afterwards, as termios will set things up the way we want..
	 */
	_fd = open(port_name, O_RDWR | O_NOCTTY | O_NONBLOCK);
	fcntl(_fd, ~O_NONBLOCK);
#endif
	_isOpenFlag = _fd > 0 ? true : false;
}

int FreeEMS_SerialPort::setupPort(int baud) {
#ifdef __WIN32__

	win32_setup_serial_params(_fd, baud, 8, ODD, 1);
	return 0;
#else
	int _baud = 0;

	/* Save serial port status */
	tcgetattr(_fd, &oldtio);
	//flush_serial(fd, TCIOFLUSH);

	memset(&newtio, 0, sizeof(newtio));
	/*
	 * BAUDRATE: Set bps rate. You could also use cfsetispeed and
	 * cfsetospeed
	 * CRTSCTS : output hardware flow control (only used if the cable has
	 * all necessary lines. See sect. 7 of Serial-HOWTO)
	 * CS8     : 8n1 (8bit,no parity,1 stopbit)
	 * CLOCAL  : local connection, no modem contol
	 * CREAD   : enable receiving characters
	 */

	/* Set baud (posix way) */

	if (baud == 9600)
		_baud = B9600;
	else if (baud == 115200)
		_baud = B115200;
	else
		printf("INVALID BAUD RATE %i\n", baud);

	cfsetispeed(&newtio, _baud);
	cfsetospeed(&newtio, _baud);

	/* Mask and set to 8N1 mode... */
	newtio.c_cflag &= ~(PARENB | CSTOPB | CSIZE);
	/* Set additional flags, note |= syntax.. */
	/* Enable receiver, ignore modem ctrls lines, use 8 bits */
	newtio.c_cflag |= CLOCAL | CREAD | CS8;

	/* RAW Input */
	/* Ignore signals, enable canonical, etc */
	newtio.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

	/* Disable software flow control */
	newtio.c_iflag &= ~(IXON | IXOFF);

	/* Set raw output */
	newtio.c_oflag &= ~OPOST;
	/*
	 initialize all control characters
	 default values can be found in /usr/include/termios.h, and are given
	 in the comments, but we don't need them here
	 *                                           */
	newtio.c_cc[VINTR] = 0; /* Ctrl-c */
	newtio.c_cc[VQUIT] = 0; /* Ctrl-\ */
	newtio.c_cc[VERASE] = 0; /* del */
	newtio.c_cc[VKILL] = 0; /* @ */
	newtio.c_cc[VEOF] = 0; /* Ctrl-d */
	newtio.c_cc[VEOL] = 0; /* '\0' */
	newtio.c_cc[VMIN] = 1;
	newtio.c_cc[VTIME] = 1; /* 100ms timeout */

	tcsetattr(_fd, TCSANOW, &newtio);

#endif
	return 0;

}

void FreeEMS_SerialPort::closePort() {

	//	tcflush(_fd, TCIOFLUSH);
	//	fcntl(_fd, O_NONBLOCK);

	//	close(_fd);

	if (_fd != -1) {
		//		if (restoreSettings)
		//		{
#ifndef __WIN32__
		//tcsetattr(_fd,TCSAFLUSH,&oldtio);
		int temp = tcsetattr(_fd, TCSANOW, &oldtio);
		temp += temp; //TODO rectify
#endif
		//		}
		::close(_fd);
	}
	_fd = -1;
	return;
}

//
void FreeEMS_SerialPort::flushSerial(FlushDirection direction) {
#ifdef __WIN32__
	if (_fd) {
		win32_flush_serial(_fd, direction);
	}
#else
	if (_fd) {
		switch (direction) {
		case INBOUND:
			tcflush(_fd, TCIFLUSH);
			break;
		case OUTBOUND:
			tcflush(_fd, TCOFLUSH);
			break;
		case BOTH:
			tcflush(_fd, TCIOFLUSH);
			break;
		}
	} else {
		std::cout << "Error: Serial port not open";
	}
#endif
}

//
//void unlock_port()
//{
//#ifndef __WIN32__
///*	printf("told to unlock serial,  path \"%s\"\n",fname); */
//	if (serial_lockfile)
//	{
//		if (g_file_test(serial_lockfile,G_FILE_TEST_IS_REGULAR))
//		{
//			g_remove(serial_lockfile);
//			g_free(serial_lockfile);
//			serial_lockfile = NULL;
//		}
//	}
//#endif
//}

//bool lock_port(char * name)
//{
//#ifndef __WIN32__
//	gchar *tmpbuf = NULL;
//	gchar *lock = NULL;
//	gchar **vector = NULL;
//	gchar *contents = NULL;
//	GError *err = NULL;
//	guint i = 0;
//	gint pid = 0;
//
//	/* If no /proc (i.e. os-X), just fake it and return */
//	if (!g_file_test("/proc",G_FILE_TEST_IS_DIR))
//		return TRUE;
//
//	lock = g_strdup_printf("/var/lock/LCK..");
//	vector = g_strsplit(name,PSEP,-1);
//	for (i=0;i<g_strv_length(vector);i++)
//	{
//		if ((g_strcasecmp(vector[i],"") == 0) || (g_strcasecmp(vector[i],"dev") == 0))
//			continue;
//		lock = g_strconcat(lock,vector[i],NULL);
//	}
//	g_strfreev(vector);
//	if (g_file_test(lock,G_FILE_TEST_IS_REGULAR))
//	{
///*		printf("found existing lock!\n");*/
//		if(g_file_get_contents(lock,&contents,NULL,&err))
//		{
///*			printf("read existing lock\n");*/
//			vector = g_strsplit(g_strchug(contents)," ", -1);
///*			printf("lock had %i fields\n",g_strv_length(vector));*/
//			pid = (gint)g_ascii_strtoull(vector[0],NULL,10);
///*			printf("pid in lock \"%i\"\n",pid);*/
//			g_free(contents);
//			g_strfreev(vector);
//			tmpbuf = g_strdup_printf("/proc/%i",pid);
//			if (g_file_test(tmpbuf,G_FILE_TEST_IS_DIR))
//			{
///*				printf("process active\n");*/
//				g_free(tmpbuf);
//				g_free(lock);
//				return FALSE;
//			}
//			else
//				g_remove(lock);
//			g_free(tmpbuf);
//		}
//
//	}
//	contents = g_strdup_printf("     %i",getpid());
//	if(g_file_set_contents(lock,contents,-1,&err))
//	{
//		serial_lockfile = g_strdup(lock);
//		g_free(contents);
//		g_free(lock);
//		return TRUE;
//	}
//	else
//		printf("Error setting serial lock %s\n",(gchar *)strerror(errno));
//	g_free(contents);
//#endif
//	return TRUE;
//}

bool FreeEMS_SerialPort::isOpen() {
	return _isOpenFlag;
}

void FreeEMS_SerialPort::flushInBuffer() {
	flushSerial(INBOUND);
}

void FreeEMS_SerialPort::flushOutBuffer() {
	flushSerial(OUTBOUND);
}

int FreeEMS_SerialPort::readData(char *data, size_t size) {
	if (isOpen()) {
		return FreeEMS_SerialPort::readWrapper(_fd, data, size);
	} else {
		std::cout << "Error: Port is not open ";
		return -2;
	}
}

void FreeEMS_SerialPort::writeData(const char *data, size_t size) {
	if (isOpen()) {
		int temp = write(_fd, data, size);
		temp = temp; //TOOD rectify
	} else
		std::cout << "Error: Port is not open ";
}

int FreeEMS_SerialPort::readWrapper(int fd, char *buf, size_t requested) {
#ifdef __WIN32__
	/* Windows can't do select() on anything but network sockets which
	 is completely braindead, and thus yoou must do overlapped IO
	 or uglier busylooped IO.  I'm lazy and hate windows and the
	 overlapped IO stuff is ugly as hell, so I used the busylooped method
	 instead...
	 */
	int received = 0;
	int read_pos = 0;
	int timeout = FALSE;
	unsigned int tries = 0;
	int wanted = requested;
	unsigned int total = 0;

	while (!timeout)
	{
		tries++;
		read_pos = requested - wanted;
		received = read(fd, &buf[read_pos], wanted);
		printf("Try %i, requested %i, got %i\n",tries,wanted,received);
		//TODO fix g_usleep(10000); /* 10ms rest */
		if (received == -1)
		{
			//TODO FIX output("Serial I/O Error, read failure\n",FALSE);
			printf("Serial I/O Error, read failure\n");
			return -1;
		}
		total += received;
		wanted -= received;
		if (tries > requested)
			timeout = TRUE;
		if (total == requested)
		{
			printf("got what was requested, returning\n");
			return total;
		}
	}
	printf("timeout, returning only %i of %i bytes\n",total,requested);
	return total;

#else	/* Linux/OS-X where sane I/O lives */
	fd_set readfds;
	fd_set errfds;

	struct timeval t;
	int attempts = 0;
	int read_pos = 0;
	int wanted = requested;
	int received = 0;
	int res = 0;
	int total = 0;

	while (wanted > 0 ) {
		FD_ZERO(&readfds);
		FD_ZERO(&errfds);
		t.tv_sec = 0;
		t.tv_usec = 200000;
		FD_SET(fd, &readfds);

		FD_SET(fd,&errfds);
		res = select (fd+1, &readfds,NULL,&errfds, &t);
		if (res == -1) {
			printf("ERROR, select() failure! Closing Port\n");
		}
		 /* OK we have an error condition waiting for us, read it */
		if (FD_ISSET(fd,&errfds)) {
			printf("select() (read) ERROR !\n");
			attempts++;
		}
		if (attempts > POLL_ATTEMPTS) {
			//return total;
			return -1;
		}

		if (res == 0) /* Timeout */
		{
			//printf("ERROR, select() timeout! Retrying...\n");
			attempts++;
			if (attempts > POLL_ATTEMPTS) {
				printf("ERROR, select() timeout! Giving up\n");
				//return total;
				return -1;
			}
		}
	      //OK we have something waiting for us, read it
	     if (FD_ISSET(fd,&readfds)) {
	    	 printf("data avail!\n");
	    	 read_pos = requested - wanted;
	    	 received = read(fd, &buf[read_pos], wanted);
    		 if (received <= 0) {
    			 printf("Serial I/O Error, read failure\n");
    			 return -1;
    		 }
    		 total += received;
    		 wanted -= received;
	     }
	}
	return total;
#endif
}

/*
 * FreeEMS_SerialPort.cpp
 *
 *  Created on: May 11, 2011
 *      Author: seank
 */

#include "FreeEMS_SerialPort.h"

//using namespace std;

FreeEMS_SerialPort::FreeEMS_SerialPort(): _fd(0), _isOpenFlag(false) {
	// TODO Auto-generated constructor stub

}

FreeEMS_SerialPort::~FreeEMS_SerialPort() {
	// TODO Auto-generated destructor stub
}

void
FreeEMS_SerialPort::openPort(char * port_name)
{
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
	_fd = open(port_name, O_RDWR | O_NOCTTY | O_NONBLOCK );
	fcntl(_fd, ~O_NONBLOCK);
#endif
	_isOpenFlag = _fd > 0 ? true:false;
}

int FreeEMS_SerialPort::setupPort(int baud)
{
#ifdef __WIN32__

	win32_setup_serial_params(_fd, baud, 8, NONE, 1);
#else
	int _baud = 0;

	/* Save serial port status */
	tcgetattr(_fd,&oldtio);
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
		printf("INVALID BAUD RATE %i\n",baud);

	cfsetispeed(&newtio, _baud);
	cfsetospeed(&newtio, _baud);

	/* Mask and set to 8N1 mode... */
	newtio.c_cflag &= ~( PARENB | CSTOPB | CSIZE);
	/* Set additional flags, note |= syntax.. */
	/* Enable receiver, ignore modem ctrls lines, use 8 bits */
	newtio.c_cflag |= CLOCAL | CREAD | CS8;

	/* RAW Input */
	/* Ignore signals, enable canonical, etc */
	newtio.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

	/* Disable software flow control */
	newtio.c_iflag &= ~(IXON | IXOFF );

	/* Set raw output */
	newtio.c_oflag &= ~OPOST;
	/*
	 initialize all control characters
	 default values can be found in /usr/include/termios.h, and are given
	 in the comments, but we don't need them here
	 *                                           */
	newtio.c_cc[VINTR]    = 0;     /* Ctrl-c */
	newtio.c_cc[VQUIT]    = 0;     /* Ctrl-\ */
	newtio.c_cc[VERASE]   = 0;     /* del */
	newtio.c_cc[VKILL]    = 0;     /* @ */
	newtio.c_cc[VEOF]     = 0;     /* Ctrl-d */
	newtio.c_cc[VEOL]     = 0;     /* '\0' */
	newtio.c_cc[VMIN]     = 1;
	newtio.c_cc[VTIME]    = 1;     /* 100ms timeout */

	tcsetattr(_fd,TCSANOW,&newtio);

#endif
	return 0;

}


void FreeEMS_SerialPort::closePort()
{
#ifndef __WIN32__
	//tcsetattr(_fd,TCSAFLUSH,&oldtio);
	tcsetattr(_fd, TCSANOW, &oldtio);
#endif
//	tcflush(_fd, TCIOFLUSH);
//	fcntl(_fd, O_NONBLOCK);

	//	close(_fd);

	if (_fd!=-1)
	{
//		if (restoreSettings)
//		{
//			tcsetattr(_fd, TCSANOW, &m_oldtio);
			tcsetattr(_fd, TCSANOW, &oldtio);
//		}
		::close(_fd);
	}
	_fd=-1;
	return;
}

//
void FreeEMS_SerialPort::flushSerial(FlushDirection direction)
{
#ifdef __WIN32__
	if (_fd)
		win32_flush_serial(_fd, type);
#else
	if (_fd)
	{
		switch (direction)
		{
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
	}
	else{
		std::cout<<"Error: Serial port not open";
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
bool FreeEMS_SerialPort::isOpen(){
	return _isOpenFlag;
}

void FreeEMS_SerialPort::flushInBuffer(){
	flushSerial(INBOUND);
}

void FreeEMS_SerialPort::flushOutBuffer(){
	flushSerial(OUTBOUND);
}

void FreeEMS_SerialPort::readData(char *data, size_t size){
	if(isOpen()){
		FreeEMS_SerialPort::readWrapper(_fd, data, size);
	}else
		std::cout<<"Error: Port is not open";
}

void FreeEMS_SerialPort::writeData(const char *data, size_t size){
	if(isOpen()){
		write(_fd, data, size);

	}else
		std::cout<<"Error: Port is not open";
}

int FreeEMS_SerialPort::readWrapper(int fd, char *buf, size_t requested)
{
	fd_set readfds;
	struct timeval t;
	int attempts = 0;
	int read_pos = 0;
	int wanted = requested;
	int received = 0;
	int res = 0;
	int total = 0;

	while (wanted > 0)
	{
		FD_ZERO(&readfds);
		t.tv_sec = 0;
		t.tv_usec = 200000;
		FD_SET(fd,&readfds);
		res = select (fd+1, &readfds,NULL,NULL, &t);
		if (res == -1)
			return -1;
		if (res == 0) /* Timeout */
		{
			/*printf("timeout!\n");*/
			attempts++;
			if (attempts > _poll_attempts)
				return total;
		}
		/* OK we have something waiting for us, read it */
		if (FD_ISSET(fd,&readfds))
		{
			/*printf("data avail!\n");*/
			read_pos = requested - wanted;
			received = read(fd, &buf[read_pos], wanted);
			if (received == -1)
				return -1;
			total += received;
			/*printf("got %i bytes\n",received);*/
			wanted -= received;
		}
	}
	return total;
}


TODO
add stuff

Welcome to FreeEMS-Loader!

##########################  LINUX USERS #################################
To build and run this application please run the following commands:

1. "apt-get update && apt-get install  qt4-qmake libqt4-dev"

2.  Download, build and install QSerialPort.  It can be found here http://gitorious.org/inbiza-labs/qserialport

3. Simply type "qmake && make all" and hit enter!
3a. For Fedora users type "qmake-qt4 && make all"

3. To execute your freshly built loader type "./FreeEMS_Loader"

It's as easy as that!

#########################  WINDOWS USERS ################################
Install MinGW

	1. Download MinGW for use with qt4. QT is very specific there for I recommend you use the link below.

		 http://computer-ss.com/files/MinGW-gcc440_1.zip

	2. Extract to c:\mingw

	3. Add c:\mingw\bin to your PATH enviornment variable.





Install QTLibs

	1. Download QT(MinGW-4.x)

		http://get.qt.nokia.com/qt/source/qt-win-opensource-4.7.1-mingw.exe

	2. Run installer and install to



Install/Build QSerialPort

	1. Clone the repo available here

		http://gitorious.org/inbiza-labs/qserialport

	2. Follow the build/installation instruction

 	3. Simply type "qmake && make all" and hit enter!

######################### MAC USERS #####################################









######################### DEBUGGING TOOLS ##############################

GNU PROOF
add -pg -g  to CFLAGS to use http://sourceware.org/binutils/docs/gprof/
run app
when done run gprof app |less,  it'll read the profile app the program created (gmon.out) and the app and resolve the sombols and show you hte slow places

VALGRIND(MEMLEAKS)

http://valgrind.org/

GDB



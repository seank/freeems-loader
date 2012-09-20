Welcome to FreeEMS-Loader!

#########################  PREREQUISITES FOR BUILDING ON ALL PLATFORMS ####################
You must first have installed the SerialIO Library availiable at
	https://github.com/seank/FreeEMS-SerialIO

If you do not have this installed, you will receive a build error like this:
./inc/comms.h:39:22: error: SerialIO.h: No such file or directory

##########################  LINUX USERS #################################
To build and run this application please run the following commands:

1. <apt-get update && apt-get install  qt4-qmake libqt4-dev>

2. Simply type <qmake && make> from the src/ directory and hit enter!
2a. For Fedora users type <qmake-qt4 && make>

3. execute your freshly built loader by typing <./FreeEMS_Loader>

It's as easy as that!

#########################  WINDOWS USERS ################################
Install MinGW

	1. Download MinGW for use with qt4. QT is very specific there for I recommend you use the link below.

		 http://computer-ss.com/files/MinGW-gcc440_1.zip

	2. Extract to c:\mingw

	3. Add c:\mingw\bin to your PATH enviornment variable.
		PATH can be extended through the Control Panel|System|Advanced|Environment variables menu.

Install QTLibs

	1. Download QT(MinGW-4.x)

		http://releases.qt-project.org/qt4/source/qt-win-opensource-4.8.2-mingw.exe

	2. Run installer and install to c:\Qt\$ver\

	3. Add c:\Qt\<version>\bin to your PATH var.
		PATH can be extended through the Control Panel|System|Advanced|Environment variables menu.

Build Loader Source
	
	1. Open a new terminal window(cmd.exe)

	2. From the FreeEMS-Loader src/ directory simply type "qmake; make release"

######################### MAC USERS #####################################









######################### DEBUGGING TOOLS ##############################

GNU PROOF
add -pg -g  to CFLAGS to use http://sourceware.org/binutils/docs/gprof/
run app
when done run gprof app |less,  it'll read the profile app the program created (gmon.out) and the app and resolve the sombols and show you hte slow places

VALGRIND(MEMLEAKS)

http://valgrind.org/

GDB


 G_SLICE=always-malloc G_DEBUG=gc-friendly,resident-modules valgrind --show-reachable=yes --tool=memcheck --num-callers=45 -v --leak-check=full --log-file=/tmp/log  --track-origins=yes ./FreeEMS_Loader
 it'll make it run about 100x slower,  so be extra patient
 it'll seem like its locked up,  but its really not
 the main log will be /tmp/log
 view it AFTER 
 the  --track-origins=yes   makes it go really really slow,  so you may wish to omit that option


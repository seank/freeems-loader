Welcome to FreeEMS-Loader!

##########################  LINUX USERS #################################
To build and run this application please run the following commands:

1. "apt-get update && apt-get install  qt4-qmake libqt4-dev"

2. Simply type "qmake && make -f Release" and hit enter!
2a. For Fedora users type "qmake-qt4 && make -f Release"

3. execute your freshly built loader by typing "./FreeEMS_Loader"

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

	2. Run installer and install to c:\Qt\$ver\

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


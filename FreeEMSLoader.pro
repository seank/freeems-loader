# /* FreeEMS-Loader- the open source s19 loader with special features for FreeEMS
# *
# * Copyright (C) 2008-2012 by Sean Keys <skeys@powerefi.com>
# *
# * This file is part of the FreeEMS-Loader project.
# *
# * FreeEMS-Loader software is free software: you can redistribute it and/or modify
# * it under the terms of the GNU General Public License as published by
# * the Free Software Foundation, either version 3 of the License, or
# * (at your option) any later version.
# *
# * FreeEMS-Loader software is distributed in the hope that it will be useful,
# * but WITHOUT ANY WARRANTY; without even the implied warranty of
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# * GNU General Public License for more details.
# *
# * You should have received a copy of the GNU General Public License
# * along with any FreeEMS-Loader software.  If not, see http://www.gnu.org/licenses/
# *
# * We ask that if you make any changes to this file you email them upstream to
# * us at info(at)powerefi(dot)com or, even better, fork the code on github.com!
# *
# * Thank you for choosing FreeEMS-Loader to load your firmware!
# *
# */
TEMPLATE = app
TARGET = FreeEMS-Loader
VERSION = 0.1.0
LIB_VERSION = "0.1.0"
QMAKE_CXXFLAGS *= -Wall
QMAKE_CXXFLAGS *= -Werror
QMAKE_CXXFLAGS_DEBUG += -pg
QMAKE_LFLAGS_DEBUG += -pg
#Build directories
Release:DESTDIR = build/release
Release:OBJECTS_DIR = build/release
Release:MOC_DIR = build/release
Release:RCC_DIR = build/release
Release:UI_DIR = build/release
Debug:DESTDIR = build/debug
Debug:OBJECTS_DIR = build/debug
Debug:MOC_DIR = build/debug
Debug:RCC_DIR = build/debug
Debug:UI_DIR = build/debug
CONFIG *= qt \
    warn_on \
    thread \
    release \
    console \
    exceptions \
    debug
QT *= core \
    gui
HEADERS += src/inc/externalData.h \
    src/inc/globals.h \
    src/inc/about.h \
    src/inc/freeems_loader.h \
    src/inc/sRecord.h \
    src/inc/parsing.h \
    src/inc/comms.h \
    src/inc/loaderTypes.h
SOURCES += src/globals.cpp \
    src/freeemsLoader.cpp \
    src/main.cpp \
    src/sRecord.cpp \
    src/parsing.cpp \
    src/comms.cpp \
    src/types.cpp \
    src/about.cpp \
    src/externalData.cpp
FORMS *= src/freeemsLoader.ui \
    src/about.ui
RESOURCES += src/resource-root.qrc
RC_FILE += src/loader.rc

# We are making use of QMAKE_POST_LINK so we always get fresh GIT hashes in our builds
QMAKE_POST_LINK += touch \
    src/externalData.cpp
CONFIG(debug, debug|release):message("Building Debug Version, expect spew!")
else { 
    DEFINES += QT_NO_WARNING_OUTPUT \
        QT_NO_DEBUG_OUTPUT
    message("Building Release Version")
}

# Default install paths
unix {
  isEmpty(PREFIX) {
    PREFIX = /usr/local
  }
 message("ATTENTION, running 'make install' will install files under '" $$PREFIX "'. If \
    	 you wish to change this, rerun qmake as follows 'qmake PREFIX=/desiredPath' instead.")  
unix:target.path = $$PREFIX/bin
unix:INSTALLS += target
}

# Default make specs
 INCLUDEPATH += src/inc
 unix:INCLUDEPATH += $$quote($$PREFIX/include/)
 unix:LIBS += -lSerialIO
 #unix:PRE_TARGETDEPS += $$quote($$PREFIX/lib/libSerialIO.so.$$LIB_VERSION)
 unix:DEFINES += GIT_HASH=$$system(git describe --dirty=-DEV --always)
 unix:DEFINES += GIT_HASH_FULL=$$system(git rev-parse HEAD)
 

# Cross compilation
win32-x-g++ { 
    message("Crosscompiling on Unix to Windows")
    # for mikes CI 
    LIBS += -L/home/michael/QtWin32/lib
    QMAKE_CXXFLAGS -= -Werror
}

# Straight Mac-OS (OS-X)
mac { 
    message("Mac OS-X Build")
    unix:INCLUDEPATH *= /opt/local/include
}

# Straight Linux
linux-g++ { 
    message("Straight Linux Build")
    unix:INCLUDEPATH += $$quote($$PREFIX/include/)
    unix:LIBS += -lSerialIO
    #PRE_TARGETDEPS += $$quote($$PREFIX/lib/libSerialIO.so.$$LIB_VERSION)
    DEFINES += GIT_HASH=$$system(git describe --dirty=-DEV --always)
    DEFINES += GIT_HASH_FULL=$$system(git rev-parse HEAD)
}

# Native Windows Build
win32 { 
    #this is currently a bit dirty as linux and windows paths are in this block
    message("Straight compile on windows")
    INCLUDEPATH *= /usr/local/include/
    Debug:LIBS += -L../FreeEMS-SerialIO/build/debug
    Release:LIBS += -L../FreeEMS-SerialIO/build/release
    LIBS += -lSerialIO0
    INCLUDEPATH *= ../FreeEMS-SerialIO/src/inc/public
    LIBS += -L/usr/local/win32/lib \
        -lSerialIO0
    win32:LIBS *= -Lc:/mingw/lib \
        -lwsock32
    DEFINES += GIT_HASH=$$system(git describe --dirty=-DEV --always)
    DEFINES += GIT_HASH_FULL=$$system(git rev-parse HEAD)
}

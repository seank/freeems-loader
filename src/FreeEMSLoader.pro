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
CONFIG *= qt \
    warn_on \
    thread \
    release \
    debug
QT *= core \
    gui
HEADERS += inc/globals.h \
    inc/about.h \
    inc/freeems_loader.h \
    inc/sRecord.h \
    inc/redirector.h \
    inc/parsing.h \
    inc/comms.h \
    inc/loaderTypes.h
SOURCES += globals.cpp \
    freeemsLoader.cpp \
    main.cpp \
    sRecord.cpp \
    parsing.cpp \
    comms.cpp \
    types.cpp \
    about.cpp
FORMS *= freeemsLoader.ui \
    about.ui
RESOURCES += resource-root.qrc
RC_FILE += loader.rc
# We are making use of QMAKE_POST_LINK so we always get fresh GIT hashes in our builds
QMAKE_POST_LINK += touch \
    freeemsLoader.cpp \
    about.cpp

CONFIG(debug, debug|release) {
	message("Building Debug Version, expect spew!")
}
else { 
    DEFINES += QT_NO_WARNING_OUTPUT \
        QT_NO_DEBUG_OUTPUT
    message("Building Release Version")
}

# Cross compilation
win32-x-g++ { 
    message("Crosscompiling on Unix to Windows")
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
    unix:INCLUDEPATH += $$quote(/usr/local/include/)
    unix:LIBS += $$quote(/usr/local/lib/libSerialIO.so.$$LIB_VERSION)
    PRE_TARGETDEPS += $$quote(/usr/local/lib/libSerialIO.so.$$LIB_VERSION)
    DEFINES += GIT_HASH=$$system(git describe --dirty=-DEV --always)
    DEFINES += GIT_HASH_FULL=$$system(git rev-parse HEAD)
}

# Native Windows Build
win32 { 
    message("Straight compile on windows (seank only)")
    INCLUDEPATH *= /usr/local/include/
    LIBS += -L/usr/local/win32/lib \
        -lSerialIO0
    win32:LIBS *= -Lc:/mingw/lib \
        -lwsock32
    DEFINES += GIT_HASH=$$system(git describe --dirty=-DEV --always)
    DEFINES += GIT_HASH_FULL=$$system(git rev-parse HEAD)
}

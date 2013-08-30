CONFIG += qtestlib
HEADERS += ../src/inc/sRecord.h
SOURCES += unitTests.cpp \
		   ../src/sRecord.cpp \
		   ../src/parsing.cpp \
		   ../src/types.cpp

# default make spec stuff
INCLUDEPATH *= ../src/inc/public
INCLUDEPATH *= ../src/inc
INCLUDEPATH *= ../src

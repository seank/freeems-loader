TEMPLATE = app
TARGET = unitTests
QT += core
HEADERS += ../src/inc/sRecord.h
SOURCES += main.cpp \
    main.cpp \
    ../src/sRecord.cpp
FORMS += 
RESOURCES += 
QMAKE_CXXFLAGS_DEBUG += -pg
QMAKE_LFLAGS_DEBUG += -pg

# default make spec stuff
INCLUDEPATH *= ../src/inc/public
INCLUDEPATH *= ../src/inc
INCLUDEPATH *= ../src

TEMPLATE = app
TARGET = FreeEMS_Loader
QT += core \
    gui \
    xml \
    xmlpatterns \
    network \
    opengl
HEADERS += FreeEMS_LoaderComms.h \
    FreeEMS_LoaderSREC.h \
    freeems_LoaderRedirector.h \
    freeems_loader.h \
    freeems_loader_types.h \
    FreeEMS_LoaderSREC.h \
    freeems_LoaderRedirector.h \
    freeems_loader_types.h \
    FreeEMS_LoaderComms.h \
    freeems_loader.h \
    freeems_loader.h \
    freeems_loader.h
SOURCES += FreeEMS_LoaderSREC.cpp \
    FreeEMS_LoaderComms.cpp \
    freeems_loader.cpp \
    main.cpp
FORMS += freeems_loader.ui \
    freeems_loader.ui \
    freeems_loader.ui \
    freeems_loader.ui
RESOURCES += resource-root.qrc 
unix:LIBS += -lboost_system
win32:LIBS += -LC:/boost/lib \
    -lboost_system-mgw44-mt-1_45 \
    -Lc:/mingw/lib \
    -lwsock32
win32:INCLUDEPATH += $$quote(C:/boost/include/boost-1_45)
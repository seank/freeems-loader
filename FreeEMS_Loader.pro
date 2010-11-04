TEMPLATE = app
TARGET = FreeEMS_Loader
QT += core \
    gui \
    xml \
    xmlpatterns \
    network \
    opengl
HEADERS += freeems_loader_types.h \
    FreeEMS_LoaderComms.h \
    freeems_loader.h \
    freeems_loader.h \
    freeems_loader.h
SOURCES += FreeEMS_LoaderComms.cpp \
    freeems_loader.cpp \
    main.cpp
FORMS += freeems_loader.ui \
    freeems_loader.ui \
    freeems_loader.ui
RESOURCES += resource-root.qrc \
    resource-root.qrc

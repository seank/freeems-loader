#ifndef FREEEMS_LOADER_H
#define FREEEMS_LOADER_H

#include <QtGui/QWidget>
//#include <QtGui>
#include <QTextStream>
#include <QFile>
#include <QCoreApplication>
#include <QString>
#include <QStringList>
#include <QMessageBox>
#include <QFileDialog>

#include "ui_freeems_loader.h"
#include "FreeEMS_LoaderComms.h"
#include "qdebug.h"
#include "stdio.h"
#include <iostream>

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>

#define NOTCONNECTED	1
#define CONNECTED		2

class FreeEMS_Loader : public QWidget
{
    Q_OBJECT

public:
    FreeEMS_Loader(QWidget *parent = 0);
    ~FreeEMS_Loader();

protected:
    void fillBaud();
    void fillStopBits();
    void fillDataBits();
    void fillParity();
    void redirectCLI();
    static void outCallBack( const char* ptr, std::streamsize count, void* pTextBox );
    void setGUIState(int state);
    void initGUI();
    int fillDevice();
    void getFileName(QString name);
    void setFlashType();

private:
    Ui::FreeEMS_LoaderClass ui;

    FreeEMS_LoaderComms *serialConnection;

    QString ripFileName;
    QString inFileName;

    int flashTypeIndex;

 //   TimeoutSerial *serial("/dev/ttyUSB0",115200);
 //   int connected;

public slots:
	void connect();
	void rip();
};

#endif // FREEEMS_LOADER_H

#ifndef FREEEMS_LOADER_H
#define FREEEMS_LOADER_H

#include <QtGui/QWidget>
#include <QtGui>
#include <QTextStream>
#include <QFile>
#include <QCoreApplication>
#include <QString>
#include <QStringList>
#include <QMessageBox>
#include <QFileDialog>
#include <QObject>

#include "ui_freeems_loader.h"
#include "FreeEMS_LoaderComms.h"
#include "FreeEMS_LoaderThreads.h"
#include "qdebug.h"
#include "stdio.h"
#include <iostream>

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>

using namespace std;

#define NOTCONNECTED	1
#define CONNECTED	2

class FreeEMS_Loader : public QWidget
{
    Q_OBJECT

public:
    FreeEMS_Loader(QWidget *parent = 0);
    ~FreeEMS_Loader();

    virtual bool notify(QObject *rec, QEvent *ev)
       {
           qDebug() << "MyApplication::notify";
           try {
               return QApplication::instance()->notify(rec, ev);
           }
           catch( ... ) {
               qDebug() << "Unknown Exception: Terminating!";
               exit(0);
           }
           return false;
       }

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
    FreeEMS_LoaderThreads *heapThreads;

    QString ripFileName;
    QString loadFileName;

    int flashTypeIndex;

public slots:
	void connect();
	void rip();
	void eraseFlash();
	void test();
	void load();
	void writeText(string message);
	void updateProgress(int percent);
	void configureProgress(int min, int max);
};

#endif // FREEEMS_LOADER_H

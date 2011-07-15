/* FreeEMS-Loader- the open source s19 loader with special features for FreeEMS
 *
 * Copyright (C) 2008-2011 by Sean Keys <skeys@powerefi.com>
 *
 * This file is part of the FreeEMS-Loader project.
 *
 * FreeEMS-Loader software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FreeEMS-Loader software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with any FreeEMS-Loader software.  If not, see http://www.gnu.org/licenses/
 *
 * We ask that if you make any changes to this file you email them upstream to
 * us at info(at)powerefi(dot)com or, even better, fork the code on github.com!
 *
 * Thank you for choosing FreeEMS-Loader to load your firmware!
 *
 */

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
//#include "FreeEMS_LoaderThreads.h"
#include "qdebug.h"
#include "stdio.h"
#include <iostream>

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <about.h>

using namespace std;

#define NOTCONNECTED	1
#define CONNECTED	2

class FreeEMS_Loader : public QWidget
{
Q_OBJECT

public:
  FreeEMS_Loader(QWidget *parent = 0);
  ~FreeEMS_Loader();

  bool showHelp;
  bool fileArg;

  virtual bool
  notify(QObject *rec, QEvent *ev)
  {
    qDebug() << "MyApplication::notify";
    try
      {
        return QApplication::instance()->notify(rec, ev);
      }
    catch (...)
      {
        qDebug() << "Unknown Exception: Terminating!";
        exit(0);
      }
    return false;
  }

protected:
  void
  fillBaud();
  void
  fillStopBits();
  void
  fillDataBits();
  void
  fillParity();
  void
  redirectCLI();
  static void
  outCallBack(const char* ptr, std::streamsize count, void* pTextBox);
  void
  setGUIState(int state);
  void
  initGUI();
  int
  fillDevice();
  void
  getFileName(QString name);
  void
  setFlashType();
  bool
  isUnattended();

private:
  Ui::FreeEMS_LoaderClass ui;

  FreeEMS_LoaderComms *loaderComms;
  //FreeEMS_LoaderThreads *heapThreads;

  bool unattended;

  QString ripFileName;
  QString loadFileName;
  QString loadDirectory;
  QStringList cmdline_args;

  int flashTypeIndex;

public slots:
  void
  connect();
  void
  rip();
  void
  eraseFlash();
  void
  test();
  void
  load();
  void
  writeText(string message);
  void
  updateProgress(int percent);
  void
  configureProgress(int min, int max);
  void
  showAbout();
  void
  closeReset();
};

#endif // FREEEMS_LOADER_H

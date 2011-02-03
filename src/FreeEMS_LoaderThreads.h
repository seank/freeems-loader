/*
 * FreeEMS_LoaderThreads.h
 *
 *  Created on: Jan 18, 2011
 *      Author: seank
 */

#ifndef FREEEMS_LOADERTHREADS_H_
#define FREEEMS_LOADERTHREADS_H_

#include <QObject>
#include <qthread.h>
#include "FreeEMS_LoaderComms.h"
#include "freeems_loader_types.h"

using namespace std;

class FreeEMS_LoaderThreads : public QThread
{
Q_OBJECT
public:
  FreeEMS_LoaderThreads(FreeEMS_LoaderComms *connection);
  void
  run();
  virtual
  ~FreeEMS_LoaderThreads();

  FreeEMS_LoaderComms *threadedConnection;
  void
  setAction(int action);

private:
  int threadAction;

signals:
  void
  WOInfo(string text);
};

#endif /* FREEEMS_LOADERTHREADS_H_ */

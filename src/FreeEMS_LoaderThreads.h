/*
 * FreeEMS_LoaderThreads.h
 *
 *  Created on: Jan 18, 2011
 *      Author: seank
 */

#ifndef FREEEMS_LOADERTHREADS_H_
#define FREEEMS_LOADERTHREADS_H_

#include <qthread.h>
#include "FreeEMS_LoaderComms.h"
#include "freeems_loader_types.h"

class FreeEMS_LoaderThreads : public QThread
{
public:
  FreeEMS_LoaderThreads(FreeEMS_LoaderComms *connection);
  //virtual void run(FreeEMS_LoaderComms *connection);
  virtual void run();
  virtual
  ~FreeEMS_LoaderThreads();
  FreeEMS_LoaderComms *threadedConnection;

private:
  int action;
};

#endif /* FREEEMS_LOADERTHREADS_H_ */

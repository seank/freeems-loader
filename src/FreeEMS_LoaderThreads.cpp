/*
 * FreeEMS_LoaderThreads.cpp
 *
 *  Created on: Jan 18, 2011
 *      Author: seank
 */

#include "FreeEMS_LoaderThreads.h"

FreeEMS_LoaderThreads::FreeEMS_LoaderThreads(FreeEMS_LoaderComms *connection)
{
  // TODO Auto-generated constructor stub
  threadedConnection = connection;
  action = EXECUTE_ERASE;
}

FreeEMS_LoaderThreads::~FreeEMS_LoaderThreads()
{
  // TODO Auto-generated destructor stub
}

void
FreeEMS_LoaderThreads::run()
{

  emit WOInfo("CONSTANT STRING");
  switch (action)
  {
  case EXECUTE_ERASE: threadedConnection->eraseDevice();
    break;
  //case RIP_THREAD: threadedConnection->ripDevice();
  //  break;
  default:
    break;
  }
  //exec();
}

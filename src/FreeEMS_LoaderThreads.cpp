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
  threadAction = NONE;
}

FreeEMS_LoaderThreads::~FreeEMS_LoaderThreads()
{
  // TODO Auto-generated destructor stub
}

void
FreeEMS_LoaderThreads::run()
{
  switch (threadAction)
    {
  case EXECUTE_RIP_ERASE_LOAD:
    emit WOInfo("Executing rip, erase and load");
    emit WOInfo("Ripping...");
    threadedConnection->ripDevice();
    emit WOInfo("Erasing...");
    threadedConnection->eraseDevice();
    emit WOInfo("Loading...");
    threadedConnection->loadDevice();
    emit WOInfo("DONE!");
    break;
  case EXECUTE_LOAD:
    emit WOInfo("Erasing...");
    threadedConnection->eraseDevice();
    emit WOInfo("Executing load");
    threadedConnection->loadDevice();
    emit WOInfo("DONE!");
    break;
  case EXECUTE_ERASE:
    emit WOInfo("Executing erase");
    threadedConnection->eraseDevice();
    break;
  case EXECUTE_RIP:
    emit WOInfo("Executing rip");
    threadedConnection->ripDevice();
    break;
  case NONE:
    emit WOInfo("Action for thread not set!");
    break;
  default:
    break;
    }
  //if(FreeEMS_Loader::isUnattended())
  //  {
  //    emit closeReset();
  //  }
}

void
FreeEMS_LoaderThreads::setAction(int action)
{
  threadAction = action;
}

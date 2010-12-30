/*
 * FreeEMS_LoaderComms.cpp
 *
 *  Created on: Oct 29, 2010
 *      Author: seank
 *  Heavily copied from Terraneo Federico
 */

#include "FreeEMS_LoaderComms.h"
#include "FreeEMS_LoaderParsing.h"
#include <string>
#include <algorithm>
#include <iostream>
#include <boost/bind.hpp>
#include <freeems_loader_types.h>
#include "FreeEMS_LoaderSREC.h"
#include <fstream>

using namespace std;
using namespace boost;

FreeEMS_LoaderComms::FreeEMS_LoaderComms(): io(), port(io), timer(io),
        timeout(posix_time::seconds(1)), flashTypeIndex(0), fDeviceIsSet(false), smIsReady(false)
{

}

FreeEMS_LoaderComms::FreeEMS_LoaderComms(const std::string& devname, unsigned int baud_rate,
        asio::serial_port_base::parity opt_parity,
        asio::serial_port_base::character_size opt_csize,
        asio::serial_port_base::flow_control opt_flow,
        asio::serial_port_base::stop_bits opt_stop)
        : io(), port(io), timer(io), timeout(posix_time::seconds(1))
{
    open(devname,baud_rate,opt_parity,opt_csize,opt_flow,opt_stop);
}

void FreeEMS_LoaderComms::open(const std::string& devname, unsigned int baud_rate,
        asio::serial_port_base::parity opt_parity,
        asio::serial_port_base::character_size opt_csize,
        asio::serial_port_base::flow_control opt_flow,
        asio::serial_port_base::stop_bits opt_stop)
{
    if(isOpen()) close();
    port.open(devname);
    port.set_option(asio::serial_port_base::baud_rate(baud_rate));
    port.set_option(opt_parity);
    port.set_option(opt_csize);
    port.set_option(opt_flow);
    port.set_option(opt_stop);
}

bool FreeEMS_LoaderComms::isReady() const
{
  return smIsReady ? true:false ;
}

bool FreeEMS_LoaderComms::isOpen() const
{
  return port.is_open();
}

void FreeEMS_LoaderComms::close()
{
    if(isOpen()==false) return;
    port.close();
    smIsReady = false;
}

void FreeEMS_LoaderComms::setTimeout(const posix_time::time_duration& t)
{
    timeout=t;
}

void FreeEMS_LoaderComms::ripDevice(char *outFileName)
{
  int i;
  unsigned int firstAddress;
  unsigned int lastAddress;
  //unsigned int bytesPerRecord = 16; //TODO make configurable, same as default hcs12mem tool
  unsigned int bytesInRange;

  outFileName++; //TODO delete
  //boost::filesystem::path filePath(outFileName);

  //boost::fstream outFile(outFileName);
  FreeEMS_LoaderSREC *s19Record = new FreeEMS_LoaderSREC(S2);

  if(smIsReady)
    {
      //calculate total bytes in device
        for(i = 0; i < numDataVectorTableEntries; i++)
          {
            if(!strcmp(flashModuleTable[flashTypeIndex].name, dataVectorTable[i].association))
                {
                  //TODO loop though pages
                  firstAddress = dataVectorTable[i].startAddress;
                  lastAddress = dataVectorTable[i].stopAddress;
                  bytesInRange = lastAddress - firstAddress;

                  s19Record->setRecordAddress(firstAddress);
                  while(bytesInRange)
                    {
                      //char byte = this->re
                      //s19Record->putNextByte();
                    }
                  cout<<"match found";
                }
          }
    }
  else
    {
      cout<<"error SM not ready";
    }
  //calculate number of bytes in device
  //allocate number of s19 records needed based on bytes per record
  //
  delete s19Record;
  //delete outFile;
}

void FreeEMS_LoaderComms::setFlashType(char *commonName)
{
   int i;
   for(i = 0; flashModuleTable[i].name; i++)
     {
         if(!strcmp(flashModuleTable[i].name, commonName))
           {
             flashTypeIndex = i;
             cout<<"set flash type to"<<commonName;
             return;
           }
     }

}

void FreeEMS_LoaderComms::setSM()
{
  char charReturn = (unsigned char)0x0D;
  char values[4] = {0x00,0x00,0x00,0x00};
  char ready[4] = {(char)0xE1,(char)0x00,(char)0x3E,(char) 0x00};
  try{
      write(&charReturn, 1);
      sleep(1);
      read(values,3);
      sleep(1);
      if(!strcmp(values, ready))
        {
          smIsReady = true;
	 }else
	   {
	     smIsReady = false;
	    }
      }
  catch(boost::system::system_error& e)
   {
    cout<<"Error: "<<e.what()<<endl;
   }
}

void FreeEMS_LoaderComms::write(const char *data, size_t size)
{
    int i;
    printf("\n about to write char(s) ");
    for(i = (int)size; i > 0; i--)
      {
        printf("-> %x", *(data + (i - 1)));
      }
    printf("\n to com port \n");

    asio::write(port,asio::buffer(data,size));
}

void FreeEMS_LoaderComms::write(const std::vector<char>& data)
{
    asio::write(port,asio::buffer(&data[0],data.size()));
}

void FreeEMS_LoaderComms::writeString(const std::string& s)
{
    asio::write(port,asio::buffer(s.c_str(),s.size()));
}

//TODO add parity "double read" option
void FreeEMS_LoaderComms::read(char *data, size_t size)
{
    if(readData.size()>0)//If there is some data from a previous read
    {
        istream is(&readData);
        size_t toRead=min(readData.size(),size);//How many bytes to read?
        is.read(data,toRead);
        data+=toRead;
        size-=toRead;
        if(size==0) return;//If read data was enough, just return
    }

    setupParameters=ReadSetupParameters(data,size);
    performReadSetup(setupParameters);

    if(timeout!=posix_time::seconds(0)) //If timeout is set, start timer
    {
        timer.expires_from_now(timeout);
        timer.async_wait(boost::bind(&FreeEMS_LoaderComms::timeoutExpired,this,
                asio::placeholders::error));
    }

    result=resultInProgress;
    bytesTransferred=0;
    for(;;)
    {
        io.run_one();
        switch(result)
        {
            case resultSuccess:
                timer.cancel();
                return;
            case resultTimeoutExpired:
                port.cancel();
                throw(timeout_exception("Timeout expired"));
            case resultError:
                timer.cancel();
                port.cancel();
                throw(boost::system::system_error(boost::system::error_code(),
                        "Error while reading"));
            default:
                timer.cancel();
                port.cancel();
                throw(boost::system::system_error(boost::system::error_code(),
                        "Error while reading"));
            //if resultInProgress remain in the loop
        }
    }
}

std::vector<char> FreeEMS_LoaderComms::read(size_t size)
{
    vector<char> result(size,'\0');//Allocate a vector with the desired size
    read(&result[0],size);//Fill it with values
    return result;
}

std::string FreeEMS_LoaderComms::readString(size_t size)
{
    string result(size,'\0');//Allocate a string with the desired size
    read(&result[0],size);//Fill it with values
    return result;
}

std::string FreeEMS_LoaderComms::readStringUntil(const std::string& delim)
{
    // Note: if readData contains some previously read data, the call to
    // async_read_until (which is done in performReadSetup) correctly handles
    // it. If the data is enough it will also immediately call readCompleted()
    setupParameters=ReadSetupParameters(delim);
    performReadSetup(setupParameters);

    if(timeout!=posix_time::seconds(0)) //If timeout is set, start timer
    {
        timer.expires_from_now(timeout);
        timer.async_wait(boost::bind(&FreeEMS_LoaderComms::timeoutExpired,this,
                asio::placeholders::error));
    }
    result=resultInProgress;
    bytesTransferred=0;
    for(;;)
    {
        io.run_one();
        switch(result)
        {
            case resultSuccess:
                {
                    timer.cancel();
                    bytesTransferred-=delim.size();//Don't count delim
                    istream is(&readData);
                    string result(bytesTransferred,'\0');//Alloc string
                    is.read(&result[0],bytesTransferred);//Fill values
                    is.ignore(delim.size());//Remove delimiter from stream
                    return result;
                }
            case resultTimeoutExpired:
                port.cancel();
                throw(timeout_exception("Timeout expired"));
            case resultError:
                timer.cancel();
                port.cancel();
                throw(boost::system::system_error(boost::system::error_code(),
                        "Error while reading"));
            default:
                timer.cancel();
                port.cancel();
                throw(boost::system::system_error(boost::system::error_code(),
                        "Error while reading"));
            //if resultInProgress remain in the loop
        }
    }
}

FreeEMS_LoaderComms::~FreeEMS_LoaderComms()
{

}

void FreeEMS_LoaderComms::performReadSetup(const ReadSetupParameters& param)
{
    if(param.fixedSize)
    {
        asio::async_read(port,asio::buffer(param.data,param.size),boost::bind(
                &FreeEMS_LoaderComms::readCompleted,this,asio::placeholders::error,
                asio::placeholders::bytes_transferred));
    } else {
        asio::async_read_until(port,readData,param.delim,boost::bind(
                &FreeEMS_LoaderComms::readCompleted,this,asio::placeholders::error,
                asio::placeholders::bytes_transferred));
    }
}

void FreeEMS_LoaderComms::timeoutExpired(const boost::system::error_code& error)
{
    if(result!=resultInProgress) return;
    if(error!=asio::error::operation_aborted)
    {
        result=resultTimeoutExpired;
    }
}

void FreeEMS_LoaderComms::readCompleted(const boost::system::error_code& error,
        const size_t bytesTransferred)
{
    if(error)
    {
        if(error!=asio::error::operation_aborted)
        {
            #ifdef __APPLE__
            if(error.value()==45)
            {
                //Bug on OS X, it might be necessary to repeat the setup
                //http://osdir.com/ml/lib.boost.asio.user/2008-08/msg00004.html
                performReadSetup(setupParameters);
                return;
            }
            #endif //__APPLE__
            result=resultError;
        }
    } else {
        if(result!=resultInProgress) return;
        result=resultSuccess;
        this->bytesTransferred=bytesTransferred;
    }
}

void
FreeEMS_LoaderComms::returnFlashType(char *responce)
{
  char getIDCommand = 0xB7;
  write(&getIDCommand,1);
  read(responce, 4);
  return;
}

int
FreeEMS_LoaderComms::readBlock(unsigned short startAddress, char *buffer, int readNumBytes)
{
  unsigned int numRetries = 3; //TODO global
  bool memError = true;
  char requestBytes = (char) readNumBytes - 1; // it reads one by default
  readNumBytes += SM_READY_CHAR_SIZE; // the sm added the ready bytes after every command
  char *parityBuffer = (char*)malloc(readNumBytes);
  //char bytesToRead = (char)readNumBytes + SM_READY_CHAR_SIZE;
  char readBlockCommand = 0xa7;
  char lowByte;
  char highByte;
  lowByte = startAddress & 0x00FF;
  highByte = startAddress >> 8;

 // printf("\n debug %x, %x, %x, %x",readBlockCommand, highByte, lowByte, startAddress);

  while(numRetries && (memError == true))
    {
      /* read into first buffer */
      write(&readBlockCommand,1);
      write(&highByte,1);
      write(&lowByte,1);
      write(&requestBytes,1);
      read(buffer, readNumBytes);
      /* read into second buffer */
      write(&readBlockCommand,1);
      write(&highByte,1);
      write(&lowByte,1);
      write(&requestBytes,1);
      read(parityBuffer, readNumBytes);
      memError = (bool) memcmp(buffer, parityBuffer, readNumBytes);
      numRetries--;
    }
  free(parityBuffer);
  if(memError == true)
    cout<<"data error detected while reading block, tried three times";
  return verifyReturn(buffer, readNumBytes) ? 1:-1;
}

int
FreeEMS_LoaderComms::verifyReturn(char *buffer, int size)
{
  buffer += (size - 1);
  printf("\n buffer is %x", *buffer);
  if((unsigned char)*buffer == 0x3E)
    {
     buffer--;
     if((unsigned char)*buffer == 0x0)
       {
         buffer--;
         if((unsigned char)*buffer == 0xe0)
           {
           return 1;
           }
       }
    }
  cout<<"waring:return chars not found";
  return -1;
}



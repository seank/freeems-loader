/*
 * FreeEMS_LoaderComms.cpp
 *
 *  Created on: Oct 29, 2010
 *      Author: seank
 *  Heavily copied from Terraneo Federico
 */

#include "FreeEMS_LoaderComms.h"
#include <string>
#include <algorithm>
#include <iostream>
#include <boost/bind.hpp>

using namespace std;
using namespace boost;

FreeEMS_LoaderComms::FreeEMS_LoaderComms(): io(), port(io), timer(io),
        timeout(posix_time::seconds(1))
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
    return smReady;
}

bool FreeEMS_LoaderComms::isOpen() const
{
    return port.is_open();
}

void FreeEMS_LoaderComms::close()
{
    if(isOpen()==false) return;
    port.close();
    smReady = 0;
}

void FreeEMS_LoaderComms::setTimeout(const posix_time::time_duration& t)
{
    timeout=t;
}

void FreeEMS_LoaderComms::setSM()
{
	try{
	  char charReturn = 0x0D;
	  char values[4] = {0x00,0x00,0x00,0x00};
	  char ready[4] = {0xE1,0x00,0x3E, 0x00};

	  write(&charReturn, 1);
	  sleep(1);
	  read(values,3);
	  if(!strcmp(values, ready))
	     {
	     	smReady = 1;
	     }else
	     {
	     	smReady = 0;
	     }
    } catch(boost::system::system_error& e)
	    {
	      cout<<"Error: "<<e.what()<<endl;
	    }
}

void FreeEMS_LoaderComms::write(const char *data, size_t size)
{
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
FreeEMS_LoaderComms::readBlock(unsigned short startAddress, char *buffer, char readNumBytes)
{
  char readBlockCommand = 0xa7;
  char lowByte;
  char highByte;

  lowByte = startAddress & 0x00FF;
  highByte = startAddress;
  highByte >>= 8;

  write(&readBlockCommand,1);
  write(&highByte,1);
  write(&lowByte,1);
  write(&readNumBytes,1);

  read(buffer, readNumBytes);

  return (strlen(buffer) == (unsigned char)readNumBytes) ? -1:1;
}


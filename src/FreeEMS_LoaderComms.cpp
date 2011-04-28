/*
 * FreeEMS_LoaderComms.cpp
 *
 *  Created on: Oct 29, 2010
 *      Author: seank
 *
 */

#include "FreeEMS_LoaderComms.h"
#include "FreeEMS_LoaderParsing.h"
#include <algorithm>
#include <boost/bind.hpp>
#include <freeems_loader_types.h>
#include <fstream>
#include <ostream>

using namespace std;

FreeEMS_LoaderComms::FreeEMS_LoaderComms() : verifyLastWrite(false), verifyACKs(false),
      flashTypeIndex(0), fDeviceIsSet(false), smIsReady(false)
{
  init();
  flushMode = false;
}

void
FreeEMS_LoaderComms::open(QString serPortName)
{
  serPortName = serPortName;
  //const char SMRDY = 0x0D;
  TNX::CommTimeouts commTimeouts;
  commTimeouts.PosixVMIN = 0;
  commTimeouts.PosixVTIME = 2;
  //serPorttest = new TNX::QSerialPort(serPortName, "115200,8,n,1");
  //serPorttest.setPortName(serPortName);
  //serPorttest.open();
  //serPorttest.flushInBuffer();
  //serPorttest.setCommTimeouts(commTimeouts);
  write(SMRDY, sizeof(SMRDY));
  //TODO user return verify instead
  QByteArray resp;
  //resp = serPorttest.read(3);
  QByteArray tester(SMRDY);// = {0xe1, 0x00, 0xe3};
  if(resp.contains(tester)){
      WOInfo("Found String");
  }else{
  WOInfo("NONBOOST Serial Port Read/Write Complete: SM NOT FOUND");
  }
  //serPorttest.close();
  //delete(serPort);
}

void
FreeEMS_LoaderComms::open(QString serPortName, unsigned int baud_rate)
{
  if (isOpen())
    close();
  //TODO move else where
  baud_rate = baud_rate;//delete
  //serPortSettings.setBaudRate(TNX::QPortSettings::BAUDR_115200);
  serPortSettings.set("115200,8,n,1");
  serPort->setPortName(serPortName);
  //serPort.setBaudRate(baud_rate);
  //serPort.setBaudRate(BAUDR_115200);
  //serPort.portSettings_ = "115200,8,n,1"; //TODO dynamic config
  serPort->open();
}

bool
FreeEMS_LoaderComms::isReady() const
{
  return smIsReady ? true : false;
}

bool
FreeEMS_LoaderComms::isOpen() const
{
  return serPort->isOpen();
}

void
FreeEMS_LoaderComms::init()
{
  serPort = new TNX::QSerialPort("115200,8,n,1");
  s19SetOne = new FreeEMS_LoaderSREC[ONE_TWENTY_EIGHT_K_RECORDS];
  //s19SetTwo = new FreeEMS_LoaderSREC[ONE_TWENTY_EIGHT_K_RECORDS];
  clearSets();
  s19SetOneCount = 0;
}

void
FreeEMS_LoaderComms::clearSets()
{
  unsigned int i;

  for (i = 0; i < ONE_TWENTY_EIGHT_K_RECORDS; i++)
    {
      s19SetOne[i].initVariables();
     // s19SetTwo[i].initVariables();
    }
}

void
FreeEMS_LoaderComms::close()
{
  if (isOpen() == false)
    return;
  serPort->close();
  smIsReady = false;
}

//void //TODO reimpliment
//FreeEMS_LoaderComms::setTimeout(const posix_time::time_duration& t)
//{
//  timeout = t;
//}

void
FreeEMS_LoaderComms::loadDevice()
{
  clearSets();
  s19SetOneCount = 0;

  vector<string> lineArray;
  string line;
  int i;
  ifstream ifs(loadFilename.toAscii());
  if (ifs.fail())
    {
      cout << "error opening load file";
      return;
    }
  while (getline(ifs, line))
    {
      i = 0;
      lineArray.push_back(line);
    }
  generateRecords(lineArray);
  loadRecordSet();
  ifs.close();
  return;
}

void
FreeEMS_LoaderComms::ripDevice()
{
  int i;
  int totalBytes = 0;
  int totalBytesTX = 0;
  unsigned int numSectors;
  unsigned int address;
  unsigned int PPageIndex;
  unsigned int nPages;
  unsigned int firstAddress;
  unsigned int lastAddress;
  unsigned int bytesPerRecord = 16; //read 16 bytes TODO make configurable, same as default hcs12mem tool
  unsigned int bytesInRange;
  unsigned int pagedAddress = 0;

  //setSM(); //Incase the SM has been reset by the user
  std::vector<char> rxBuffer(bytesPerRecord);
  ofstream outFile(ripFilename.toAscii(), ios::out | ios::binary);
  FreeEMS_LoaderSREC *s19Record = new FreeEMS_LoaderSREC(S2);
  totalBytes = getDeviceByteCount();
  emit
  configureProgress(0, totalBytes);

  if (smIsReady)
    {
      for (i = 0; i < numDataVectorTableEntries; i++)
        {
          if (!strcmp(flashModuleTable[flashTypeIndex].name,
              dataVectorTable[i].association))
            {
              nPages = dataVectorTable[i].stopPage
                  - dataVectorTable[i].startPage;
              nPages++; //there is always 1 page to rip

              for (PPageIndex = dataVectorTable[i].startPage; nPages; PPageIndex++, nPages--)
                {
                  SMSetPPage(PPageIndex); //set Ppage register

                  firstAddress = dataVectorTable[i].startAddress;
                  lastAddress = dataVectorTable[i].stopAddress;
                  bytesInRange = lastAddress - firstAddress;
                  s19Record->setRecordAddress(firstAddress);
                  numSectors = (lastAddress - firstAddress) / bytesPerRecord
                      + 1;
                  //TODO add error for invalid range or improper modulus
                  rxBuffer.clear();
                  for (address = firstAddress; numSectors; address
                      += bytesPerRecord, numSectors--, totalBytesTX
                      += bytesPerRecord)
                    {
                      //Read Block
                      pagedAddress = PPageIndex;
                      pagedAddress <<= 16;
                      pagedAddress += address;
                      SMReadByteBlock((unsigned short) address, bytesPerRecord,
                          rxBuffer);
                      s19Record->initVariables(); // clear record
                      s19Record->setTypeIndex(S2);
                      s19Record->setRecordAddress(pagedAddress);
                      s19Record->fillRecord(rxBuffer);
                      s19Record->buildRecord();
                      //Update Progress Bar
                      emit
                      udProgress(totalBytesTX);
                      if (s19Record->recordIsNull == false)
                        {
                          outFile.write(s19Record->retRecordString().c_str(),
                              s19Record->retRecordSize());
                          outFile.write("\n", 1);
                        }
                    }
                }
            }
        }
      emit udProgress(totalBytes);
    }
  else
    {
      cout << "error SM not ready";
    }
  delete s19Record;
  outFile.close();
}

void
FreeEMS_LoaderComms::setFlashType(const char *commonName)
{
  int i;
  for (i = 0; flashModuleTable[i].name; i++)
    {
      if (!strcmp(flashModuleTable[i].name, commonName))
        {
          flashTypeIndex = i;
          cout << "set flash type to " << commonName; //TODO MAKE THREAD SAFE
          fDeviceIsSet = true;
          return;
        }
    }
}

void
FreeEMS_LoaderComms::SMSetPPage(char PPage)
{
  char page = PPage;
  //asio::write(port, asio::buffer(&SMWriteByte, ONE_BYTE));
  //asio::write(port, asio::buffer(&Zero, ONE_BYTE));
  //asio::write(port, asio::buffer(&PPageRegister, ONE_BYTE));
  //asio::write(port, asio::buffer(&page, ONE_BYTE));
  write(&SMWriteByte);
  write(&Zero);
  write(&PPageRegister);
  write(&page);

  //if(verifyACKs == true)
  //  {
      if(verifyReturn() < 0)
         {
           cout << "cannot verify return string after setting ppage";
         }
  //  }
}

void
FreeEMS_LoaderComms::SMReadChars(const char *data, size_t size)
{
  int i;
  printf("\n about to write char(s) ");
  for (i = (int) size; i > 0; i--)
    {
      printf("-> %x", *(data + (i - 1)));
    }
  //asio::write(port, asio::buffer(data, size));
}

void
FreeEMS_LoaderComms::resetSM()
{
 // try
 //    {
       write(&SMReset);
       //asio::write(port, asio::buffer(&SMReset, ONE_BYTE));
//     }
//   catch (/*boost::system::system_error& e*/)
//     {
//       cout << "Error trying to write SM Reset char to serial port: ";
       //    << e.what() << endl;
//       return;
//     }
   return;
}

void
FreeEMS_LoaderComms::setSM()
{
  //  unsigned int i;
  //  unsigned int length = 0;
  //  std::vector<char> data;
  //  flushRXStream();
  //  try
  //    {
  //      port.write_some(asio::buffer(&SMReturn, ONE_BYTE));
  //    }
  //  catch (boost::system::system_error& e)
  //    {
  //      cout << "Error trying to write SM return char to serial port: "
  //          << e.what() << endl;
  //      return;
  //    }
  //  data = read(3);
  //  char a, b ,c = 0;
  //      length = data.size() - 1;
  //   for (i = 0; i < data.size(); i++)
  //     {
  //       printf("char is %c, %i", data[i], data.size());
  //     }
  //      a = data[length - 2];
  //      b = data[length - 1];
  //      c = data[length];
  QByteArray response;
  write(&SMReturn);
  response = serPort->read(3); //todo write a new read()
  QByteArray tester(SMRDY);// = {0xe1, 0x00, 0xe3};
  if (response.contains(tester))
    {
      cout << "SM READY";
      smIsReady = true;
    }
  else
    {
      smIsReady = false;
    }
  return;
}

void
FreeEMS_LoaderComms::write(const char *data, size_t size)
{
  //asio::write(port, asio::buffer(data, size));
  serPort->write(data, size);
}

void
FreeEMS_LoaderComms::write(const char *data)
{
  //asio::write(port, asio::buffer(data, size));
  serPort->write(data);
}

void
FreeEMS_LoaderComms::write(const std::vector<char>& data)
{
  unsigned int i;
  char d;
  for(i = 0; i < data.size(); i++){
      d = data[i];
      write(&d);
  }
  //asio::write(port, asio::buffer(&data[0], data.size()));
}

void
FreeEMS_LoaderComms::writeString(const std::string& s)
{
  unsigned int i;
  char d;
  for(i = 0; i < s.size(); i++){
      d = s[i];
      write(&d);
  }
  //asio::write(port, asio::buffer(s.c_str(), s.size()));
}

//TODO add parity "double read" option
void
FreeEMS_LoaderComms::read(char *data, size_t size)
{
 serPort->read(data, size);
}

std::vector<char>
FreeEMS_LoaderComms::read(size_t size)
{
  vector<char> result(size, '\0');//Allocate a vector with the desired size
  read(&result[0], size);//Fill it with values
  return result;
}

std::string
FreeEMS_LoaderComms::readString(size_t size)
{
  string result(size, '\0');//Allocate a string with the desired size
  read(&result[0], size);//Fill it with values
  return result;
}

FreeEMS_LoaderComms::~FreeEMS_LoaderComms()
{

}
/*
void //TODO reimpliment
FreeEMS_LoaderComms::timeoutExpired(const boost::system::error_code& error)
{
  if (result != resultInProgress)
    return;
  if (error != asio::error::operation_aborted)
    {
      result = resultTimeoutExpired;
    }
}
*/


void
FreeEMS_LoaderComms::returnFlashType(char *responce)
{
  char getIDCommand = 0xB7;
  write(&getIDCommand, 1);
  read(responce, 4);
  return;
}

int
FreeEMS_LoaderComms::verifyReturn()
{
  QByteArray resp;
  resp = serPort->read(3); //todo use wrapper
  QByteArray tester(SMRDY);// = {0xe1, 0x00, 0xe3};
  if(resp.contains(tester)){
      return 1;
  }else{
      return -1;
  }

//  if(verifyACKs == true)
//    {
//      std::cout<<"Error verifying return buffer reads "<<data;
//    }
//  return -1;
}

void
FreeEMS_LoaderComms::SMReadByteBlock(unsigned int address, char plusBytes,
    std::vector<char> &vec)
{
  std::vector<char> buffer(plusBytes);
  char highByte = (address & 0xFF00) >> 8;
  char lowByte = address & 0x00FF;
  char bytesRequested = plusBytes - 1;
  //asio::write(port, asio::buffer(&SMReadBlock, ONE_BYTE));
  //asio::write(port, asio::buffer(&highByte, ONE_BYTE));
  //asio::write(port, asio::buffer(&lowByte, ONE_BYTE));
  //asio::write(port, asio::buffer(&bytesRequested, ONE_BYTE));
  write(&SMReadBlock);
  write(&highByte);
  write(&lowByte);
  write(&bytesRequested);

  buffer = read(plusBytes);
  if (verifyReturn() < 0) // you must always verify a return to "clear" the buffer
         cout << "error validating return from SMRequestByteBlock";
  vec = buffer;
  return;
}

void
FreeEMS_LoaderComms::erasePage(char PPage)
{
  SMSetPPage(PPage);
  write(&SMErasePage);
  //if(verifyACKs == true)
  //  {
      if(verifyReturn() < 0)
         cout<< "Error validating SMErasePage confirmation, page may already be erased";
  //  }
}

void
FreeEMS_LoaderComms::eraseDevice()
{
  int i;
  int nPages;
  char PPageIndex;
  setSM(); //Incase the SM has been reset by the user
  if (smIsReady)
    {
      //calculate total bytes in device
      for (i = 0; i < numDataVectorTableEntries; i++)
        {
          if (!strcmp(flashModuleTable[flashTypeIndex].name,
              dataVectorTable[i].association))
            {
              nPages = dataVectorTable[i].stopPage
                  - dataVectorTable[i].startPage + 1;
              emit
              configureProgress((unsigned char) dataVectorTable[i].startPage,
                  (unsigned char) dataVectorTable[i].stopPage);
              for (PPageIndex = dataVectorTable[i].startPage; nPages; PPageIndex++, nPages--)
                {
                  SMSetPPage(PPageIndex); //set Ppage register
                  erasePage(PPageIndex); // TODO put signal here
                  emit udProgress((unsigned char) PPageIndex);
                }
            }
        }
    }
  else
    {
      cout << "error SM not ready";
    }
}

void
FreeEMS_LoaderComms::setThreadAction(int action)
{
  threadAction = action;
}

void
FreeEMS_LoaderComms::setRipFilename(QString name)
{
  ripFilename = name;
}

void
FreeEMS_LoaderComms::setLoadFilename(QString name)
{
  loadFilename = name;
}

int
FreeEMS_LoaderComms::getDeviceByteCount()
{
  int totalBytes = 0;
  int i;
  if (fDeviceIsSet == true)
    {
      for (i = 0; i < numDataVectorTableEntries; i++)
        {
          if (!strcmp(flashModuleTable[flashTypeIndex].name,
              dataVectorTable[i].association))
            {
              totalBytes += (dataVectorTable[i].stopPage
                  - dataVectorTable[i].startPage)
                  * (dataVectorTable[i].stopAddress
                      - dataVectorTable[i].startAddress);
            }
        }
      return totalBytes;
    }
  else
    {
      emit WOInfo("Cannot get byte count, no device set");
      return 0;
    }
}

void
FreeEMS_LoaderComms::generateRecords(vector<string> lineArray)
{
  unsigned int i, j;
  string line;
  for (i = 0, j = 0; i < lineArray.size(); i++)
    {
      line = lineArray.at(i);
      if (lineIsLoadable(&line))
        {
          s19SetOne[j].createFromString(&line);
          s19SetOneCount++;
          j++;
        }
    }
}

bool //TODO move to parser class
FreeEMS_LoaderComms::lineIsLoadable(string* line)
{
  if ((line->find("S3")) == 0 || (line->find("S2")) == 0 || ((line->find("S1"))
      == 0))
    return true;
  return false;
}

void
FreeEMS_LoaderComms::loadRecordSet()
{
  int i;

  emit
  configureProgress(0, s19SetOneCount - 1);
  for (i = 0; i < s19SetOneCount; i++)
    {
      SMWriteByteBlock(s19SetOne[i].payloadAddress, s19SetOne[i].recordBytes,
          s19SetOne[i].recordPayloadBytes);
      emit udProgress(i);

    }
  return;
}

void
FreeEMS_LoaderComms::SMWriteByteBlock(unsigned int address, char* bytes,
    int numBytes)
{
  int typeID = S2; //TODO get from s19
  unsigned int Ppage;
  int i;

  std::vector<char> verifyString;
  std::vector<char> readString;
  char c;
  char highByte;
  char lowByte;
  char bytesToWrite;

  for(i = 0; i < numBytes; i++)
    {
      verifyString.push_back(*(bytes +i));
    }

  switch (typeID)
    {
  case S2:
    if ((address & 0x0FF0000) != (lastLoadAddress & 0x0FF0000))
      {
        Ppage = address >> 16;
        SMSetPPage((char) Ppage); //change page if necessary
      }
    highByte = (address & 0xFF00) >> 8;
    lowByte = address & 0x00FF;
    bytesToWrite = (char) numBytes - 1;
    c = (char) SMWriteBlock;
    write(&c, ONE_BYTE);
    write(&highByte, ONE_BYTE);
    write(&lowByte, ONE_BYTE);
    write(&bytesToWrite, ONE_BYTE);
    write(bytes, numBytes);
    if(verifyACKs == true)
      {
        if(verifyReturn() < 0)
          {
            emit WOInfo("Error: did not receive ACK after writing a block");
            return;
          }
      }
    if(verifyLastWrite == true)
      {
        SMReadByteBlock(address, numBytes, readString);
         if(verifyString != readString)
          {
           emit WOInfo("Error: validating sector at TODO implement retry");
          }
      }
    break;
  default:
    cout << "Cannot get byte count, no device set";
    break;
    }
}

void
FreeEMS_LoaderComms::flushRXStream()
{
 //check for open port
  flushMode = true;
  char c;
  unsigned int bytes;
  for(bytes = 0; flushMode == true; bytes++)
    {
      read(&c, 1);
      if(bytes > 4096)
        {
          emit WOInfo("Error: it seems there is a stream of data coming in the serial port, is the firmware running ?");
          return;
        }
    }
}

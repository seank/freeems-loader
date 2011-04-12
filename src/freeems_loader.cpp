#include "freeems_loader.h"
#include "freeems_LoaderRedirector.h"
#include "FreeEMS_LoaderComms.h"
#include "FreeEMS_LoaderParsing.h"
#include "FreeEMS_LoaderSREC.h"
#include "string.h"
#include <new>
#include "freeems_loader_types.h"

#include <string>
#include <algorithm>
#include <iostream>

//using namespace std;
//using namespace boost;

FreeEMS_Loader::FreeEMS_Loader(QWidget *parent) :
QWidget(parent), showHelp(false), fileArg(false), unattended(false)
{
  ui.setupUi(this);
  qRegisterMetaType<string> ("string");
  serialConnection = new FreeEMS_LoaderComms;
  heapThreads = new FreeEMS_LoaderThreads(serialConnection);
  fillBaud();
  fillDataBits();
  fillStopBits();
  fillParity();
  initGUI();
  //redirectCLI();
  //TODO move to a seperate function
  loadFileName.clear();
  QObject::connect(heapThreads, SIGNAL( WOInfo(string) ), this,
      SLOT( writeText(string) ));
  //QObject::connect(heapThreads, SIGNAL( closeReset() ), this,
  //      SLOT( closeReset() ));
  QObject::connect(serialConnection, SIGNAL( WOInfo(string) ), this,
      SLOT( writeText(string) ));
  QObject::connect(serialConnection, SIGNAL( udProgress(int) ), this,
      SLOT( updateProgress(int) ));
  QObject::connect(serialConnection, SIGNAL( configureProgress(int, int) ),
      this, SLOT( configureProgress(int, int) ));

  /* restore settings */
  QSettings settings("FreeEMS", "Loader");
  resize(settings.value("size", QSize(400, 320)).toSize());
  move(settings.value("pos", QPoint(50, 50)).toPoint());
  ui.comboDevice->addItem(settings.value("serialDevice").toString());
  ui.chkRip->setChecked(settings.value("chkRip").toBool());
  ui.chkVerify->setChecked(settings.value("chkVerify").toBool());
  //loadFileName = settings.value("lastFileName").toString();
  loadDirectory = settings.value("lastDirectory").toString();
  cout<<"load name is: "<<loadFileName.toStdString();

  QString arg;
  cmdline_args = QCoreApplication::arguments();
  fillDevice();

  bool isFileName = false;
  bool isDevice = false;
  foreach(arg, cmdline_args)
  {
    if(isFileName)
      {
        loadFileName = arg;
        isFileName = false;
        writeText("using filename: ");
        writeText(loadFileName.toStdString());
      }
    else if(isDevice)
      {
        isDevice = false;
        ui.comboDevice->clear();
        ui.comboDevice->addItem(arg);
        writeText("using device: ");
        writeText(arg.toStdString());
      }
        if (arg.contains("--"))
          {
            if (arg.contains("--unattended")) //TODO use nested if and detect invalid args
              {
                cout << " connect and load ";
                unattended = true;
              }
            else if (arg.contains("--file"))
              {
                isFileName = true;
                fileArg = true;
              }
            else if (arg.contains("--verify"))
              {
                cout << " Verify Set True ";
                ui.chkVerify->setChecked(true);
              }
            else if (arg.contains("--device"))
            {
              isDevice = true;
            }
            else if (arg.contains("--rip"))
              {
                cout << " Rip Set True ";
                ui.chkRip->setChecked(true);
              }
            else if (arg.contains("--norip"))
              {
                cout << " Rip Before Load Set False ";
                ui.chkRip->setChecked(false);
              }
            else if (arg.contains("--noverify"))
              {
                cout << " Verify Set False ";
                ui.chkVerify->setChecked(false);
              }
            else if (arg.contains("--help"))
            {
              cout << " valid arguments are: --rip, --norip, --verify, --noverify, --file <path to file>, --device, --unattended"
              " --help";
              showHelp = true;
            }
            else
              {
                cout << " invalid argument: " << arg.toStdString();
              }
          }
      }

  if(unattended)
    {
      connect();
      load();
    }
}

FreeEMS_Loader::~FreeEMS_Loader()
{
  serialConnection->close();
  delete serialConnection;
  QSettings settings("FreeEMS", "Loader");
  settings.setValue("pos", pos());
  settings.setValue("size", size());
  settings.setValue("serialDevice", ui.comboDevice->currentText());
  settings.setValue("chkRip", ui.chkRip->isChecked());
  settings.setValue("chkVerify", ui.chkVerify->isChecked());
  settings.setValue("lastFileName", loadFileName);
   //delete coutRedirector;
}

int
FreeEMS_Loader::fillDevice()
{
#ifdef __linux__
  ui.comboDevice->addItem("/dev/ttyUSB0");
  QDir dir("/dev");
   QStringList filters;
   filters << "serial*"<<"tty*";
   dir.setNameFilters(filters);
   foreach ( QString file, dir.entryList(QDir::AllEntries) )
     {
      ui.comboDevice->addItem(file);
     }
#endif
#ifdef __WIN32__
  ui.comboDevice->addItem("COM1");
#endif

  return 0;
}

void
FreeEMS_Loader::redirectCLI()
{
  StdRedirector<char>* coutRedirector = new StdRedirector<char> (std::cout,
      outCallBack, ui.textOutput);
  //StdRedirector<char>* perrorRedirector = new StdRedirector <char>(&perror, outCallBack, ui.textOutput);
  coutRedirector->compileWarning();
  //int *compileWarning = &coutRedirector;
  //compileWarning++;
  std::cout << "CLI output redirected";
}

void
FreeEMS_Loader::outCallBack(const char* ptr, std::streamsize count,
    void* pTextBox)
{
  (void) count;
  QTextBrowser* p = static_cast<QTextBrowser*> (pTextBox);
  p->append(ptr);
}

void
FreeEMS_Loader::fillBaud()
{
  //#ifdef B0
  ui.comboBaud->addItem("0");
  //#endif

  //#ifdef B50
  ui.comboBaud->addItem("50");
  //#endif
  //#ifdef B75
  ui.comboBaud->addItem("75");
  //#endif
  //#ifdef B110
  ui.comboBaud->addItem("110");
  //#endif
  //#ifdef B134
  ui.comboBaud->addItem("134");
  //#endif
  //#ifdef B150
  ui.comboBaud->addItem("150");
  //#endif
  //#ifdef B200
  ui.comboBaud->addItem("200");
  //#endif
  //#ifdef B300
  ui.comboBaud->addItem("300");
  //#endif
  //#ifdef B600
  ui.comboBaud->addItem("600");
  //#endif
  //#ifdef B1200
  ui.comboBaud->addItem("1200");
  //#endif
  //#ifdef B1800
  ui.comboBaud->addItem("1800");
  //#endif
  //#ifdef B2400
  ui.comboBaud->addItem("2400");
  //#endif
  //#ifdef B4800
  ui.comboBaud->addItem("4800");
  //#endif
  //#ifdef B7200
  ui.comboBaud->addItem("7200");
  //#endif
  //#ifdef B9600
  ui.comboBaud->addItem("9600");
  //#endif
  //#ifdef B14400
  ui.comboBaud->addItem("14400");
  //#endif
  //#ifdef B19200
  ui.comboBaud->addItem("19200");
  //#endif
  //#ifdef B28800
  ui.comboBaud->addItem("28800");
  //#endif
  //#ifdef B38400
  ui.comboBaud->addItem("38400");
  //#endif
  //#ifdef B57600
  ui.comboBaud->addItem("57600");
  //#endif
  //#ifdef B76800
  ui.comboBaud->addItem("76800");
  //#endif
  // this one is the default
  ui.comboBaud->addItem("115200");
  ui.comboBaud->setCurrentIndex(ui.comboBaud->count() - 1);
}

void
FreeEMS_Loader::fillStopBits()
{
  ui.comboStopBits->addItem("1");
  ui.comboStopBits->addItem("1.5");
  ui.comboStopBits->addItem("2");
}

void
FreeEMS_Loader::fillDataBits()
{
  ui.comboDataBits->addItem("5");
  ui.comboDataBits->addItem("6");
  ui.comboDataBits->addItem("7");
  ui.comboDataBits->addItem("8");
  ui.comboDataBits->setCurrentIndex(ui.comboDataBits->count() - 1);
}

void
FreeEMS_Loader::fillParity()
{
  ui.comboParity->addItem("NONE");
  ui.comboParity->addItem("ODD");
  ui.comboParity->addItem("EVEN");
  ui.comboParity->addItem("MARK");
  ui.comboParity->addItem("SPACE");
}

void
FreeEMS_Loader::connect()
{

#ifdef __APPLE__
         cout<<"Fred is gay, didnt you know";
#endif //__APPLE__

//  if(!QDir(ui.comboDevice->currentText()).exists(ui.comboDevice->currentText()))
//     {
//       writeText("ERROR: Serial device file does not exist!");
//       return;
//     }

QString portName = ui.comboDevice->currentText();
#ifdef __WIN32__
  portName.toUpper();
#endif

#ifndef __WIN32__
QFile file(portName);
  if (!file.open(QIODevice::ReadWrite))
    {
      writeText("ERROR: Cannot open serial deivce "+portName.toStdString());
      return;
    }
  file.close();
#endif
  if (!serialConnection->isReady())
    {
      //setFlashType();
      serialConnection->open(portName.toAscii().data(),
          ui.comboBaud->currentText().toUInt());
      serialConnection->setTimeout(boost::posix_time::seconds(5)); //TODO make configable
      serialConnection->setSM();
      serialConnection->setFlashType(defFlashType);
      serialConnection->isReady() ? setGUIState(CONNECTED) : setGUIState(
          NOTCONNECTED);
    }
  else
    {
      serialConnection->resetSM();
      serialConnection->close();
      serialConnection->isReady() ? setGUIState(CONNECTED) : setGUIState(
          NOTCONNECTED);
    }
}

void
FreeEMS_Loader::rip()
{
  ui.chkRip->setEnabled(0);
  ripFileName = QFileDialog::getSaveFileName(this, tr("Save s19 as"),
      QDir::currentPath(), tr("s19 (*.s19)"));
  if (ripFileName.isNull())
    {
      cout << "error opening file";
    }
  serialConnection->setRipFilename(ripFileName);
  heapThreads->setAction(EXECUTE_RIP);
  heapThreads->start();
}

void
FreeEMS_Loader::getFileName(QString name)
{
  name = QFileDialog::getSaveFileName(this, tr("Save s19 as"),
      QDir::currentPath(), tr("s19 (*.s19)"));
  if (!name.isNull())
    {
      writeText("error opening file");
    }
}

void
FreeEMS_Loader::initGUI()
{
  ui.chkVerify->setChecked(true);
  ui.chkRip->setChecked(true);
  setGUIState(NOTCONNECTED);
  ui.progressBar->setValue(0);
  ui.radioRX->setEnabled(false);
  ui.radioTX->setEnabled(false);
  //	ui.radFlashType->setChecked(1);
  //	ui.radFlashType->setDisabled(1);
}

void
FreeEMS_Loader::setGUIState(int state)
{
  switch (state)
    {
  case NOTCONNECTED:
    ui.pushLoad->setEnabled(0);
    ui.pushRip->setEnabled(0);
    ui.pushConnect->setText("Connect");
    ui.pushErase->setEnabled(0);
    break;
  case CONNECTED:
    ui.pushLoad->setEnabled(1);
    ui.pushRip->setEnabled(1);
    ui.pushConnect->setText("Close/Rst");
    ui.pushErase->setEnabled(1);
  default:
    break;
    }
}

void
FreeEMS_Loader::test()
{
  //serialConnection->setSM();
  //FreeEMS_LoaderThreads test(serialConnection, EXECUTE_ERASE);
  //test.run(serialConnection);
  //test.start();
  //heapThreads->start();
  //heapThreads->wait();
  //test.wait();
  //serialConnection->setLoadFilename();
  string test = "Yo MOFO you ready to get loaded or what?";
  writeText(test);
}

void
FreeEMS_Loader::eraseFlash()
{
  heapThreads->setAction(EXECUTE_ERASE);
  heapThreads->start();
}

void
FreeEMS_Loader::load()
{
  QDate date = QDate::currentDate();
  QTime time = QTime::currentTime();

  if(!fileArg) //if no file was specified from the cmdline open browser
    {
      loadFileName = QFileDialog::getOpenFileName(this, tr("Load s19 file"),
          loadDirectory, tr("s19 (*.s19)"));
    }
  if (loadFileName.isNull())
    {
      writeText("no file selected");
      return;
    }

  if(ui.chkVerify->isChecked())
    {
      serialConnection->verifyLastWrite = true; //todo make set function
      serialConnection->verifyACKs = true;
    }
  else
    {
      serialConnection->verifyLastWrite = false; //todo make set function
      serialConnection->verifyACKs = true; //todo make settable
    }
  loadDirectory = loadFileName;
  QString name = loadFileName.section( '/', -1 );
  ripFileName = QDir::currentPath();
  ripFileName += "/saved/";
  if(!QDir(ripFileName).exists(ripFileName))
    {
      QDir(ripFileName).mkpath(ripFileName);
    }
  ripFileName += date.toString("MM-dd-yyyy-");
  ripFileName += time.toString("H-m-s-");
  ripFileName += name;
  writeText("Ripping as "); writeText(ripFileName.toStdString());
  serialConnection->setLoadFilename(loadFileName);
  serialConnection->setRipFilename(ripFileName);
  if(ui.chkRip->isChecked())
    {
      heapThreads->setAction(EXECUTE_RIP_ERASE_LOAD);
    }
  else
    {
      heapThreads->setAction(EXECUTE_LOAD);
    }
  heapThreads->start();

  QSettings settings("FreeEMS", "Loader"); //TODO make saveSettings fuction
  settings.setValue("lastDirectory", loadDirectory);
  //serialConnection->loadDevice(); // calls load without a seperate thread
}

void
FreeEMS_Loader::writeText(string message)
{
  QString out = message.c_str();
  ui.textOutput->append(out);
}

void
FreeEMS_Loader::updateProgress(int percent)
{
  ui.progressBar->setValue(percent);
}

bool
FreeEMS_Loader::isUnattended()
{
  return unattended;
}

void
FreeEMS_Loader::configureProgress(int min, int max)
{
  ui.progressBar->setMinimum(min);
  ui.progressBar->setMaximum(max);
}

void
FreeEMS_Loader::showAbout()
{
  About *message = new About;
  message->show();
}

void
FreeEMS_Loader::closeReset()
{

}

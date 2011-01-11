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

FreeEMS_Loader::FreeEMS_Loader(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
	fillDevice();
	serialConnection = new FreeEMS_LoaderComms;
	fillBaud();
	fillDataBits();
	fillStopBits();
	fillParity();
	redirectCLI();
	initGUI();
}

FreeEMS_Loader::~FreeEMS_Loader()
{
  serialConnection->close();
  //delete serialConnection;
  //delete coutRedirector;
}

int FreeEMS_Loader::fillDevice()
{
	ui.comboDevice->addItem("/dev/ttyUSB0");
/*	DIR *dp;
	struct dirent *dirp;
	if((dp = opendir(dir.c_str("/dev/"))) == NULL) {
	cout << "Error(" << errno << ") opening " << dir << endl;
	return errno;
	}

	while ((dirp = readdir(dp)) != NULL) {
	//files.push_back(string(dirp->d_name));
	ui.comboDevice->addItem(string(dirp->d_name));
	}
	closedir(dp);
*/
	return 0;
}

void FreeEMS_Loader::redirectCLI()
{
  StdRedirector<char>* coutRedirector = new StdRedirector <char>(std::cout, outCallBack, ui.textOutput);
  //StdRedirector<char>* perrorRedirector = new StdRedirector <char>(&perror, outCallBack, ui.textOutput);
  coutRedirector->compileWarning();
  //int *compileWarning = &coutRedirector;
  //compileWarning++;
  std::cout <<"CLI output redirected";
}

void FreeEMS_Loader::outCallBack( const char* ptr, std::streamsize count, void* pTextBox )
{
  (void) count;
  QTextBrowser* p = static_cast< QTextBrowser* >( pTextBox );
  p->append( ptr );
}

void FreeEMS_Loader::fillBaud()

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
   ui.comboBaud->setCurrentIndex(ui.comboBaud->count()-1);

}

void FreeEMS_Loader::fillStopBits()
{
    ui.comboStopBits->addItem("1");
    ui.comboStopBits->addItem("1.5");
    ui.comboStopBits->addItem("2");

}

void FreeEMS_Loader::fillDataBits()
{
  ui.comboDataBits->addItem("5");
  ui.comboDataBits->addItem("6");
  ui.comboDataBits->addItem("7");
  ui.comboDataBits->addItem("8");
  ui.comboDataBits->setCurrentIndex(ui.comboDataBits->count()-1);
}

void FreeEMS_Loader::fillParity()
{
  ui.comboParity->addItem("NONE");
  ui.comboParity->addItem("ODD");
  ui.comboParity->addItem("EVEN");
  ui.comboParity->addItem("MARK");
  ui.comboParity->addItem("SPACE");
}

void FreeEMS_Loader::connect()
{
  if(!serialConnection->isReady())
    {
      //setFlashType();
      serialConnection->open(ui.comboDevice->currentText().toAscii().data(),ui.comboBaud->currentText().toUInt());
      //sleep(1); //some systems need a delay after a port config
      serialConnection->setTimeout(boost::posix_time::seconds(5)); //TODO make configable
      serialConnection->setSM();
      serialConnection->setFlashType(ui.radFlashType->text().toAscii().data());
      serialConnection->isReady() ? setGUIState(CONNECTED):setGUIState(NOTCONNECTED);
    }else
      {
        serialConnection->close();
        serialConnection->isReady() ? setGUIState(CONNECTED):setGUIState(NOTCONNECTED);
	}
 }

void FreeEMS_Loader::rip()
{
  ui.chkRip->setEnabled(0);
    ripFileName = QFileDialog::getSaveFileName(
          this,
          tr("Save s19 as"),
          QDir::currentPath(),
          tr("s19 (*.s19)") );
      if( ripFileName.isNull() )
      {
        cout<<"error opening file";
      }
//  while(this->ge)
//  cout<<"using file "<<ripFileName.toAscii().data();
//  char test[20] = "512.s19";
//  serialConnection->ripDevice(test);
  serialConnection->ripDevice(ripFileName.toAscii().data());
  ui.chkRip->setEnabled(1);
}

void FreeEMS_Loader::getFileName(QString name)
{
  cout<<" in getfile name";
  name = QFileDialog::getSaveFileName(
         this,
         tr("Save s19 as"),
         QDir::currentPath(),
         tr("s19 (*.s19)") );
     if( !name.isNull() )
     {
       //qDebug( filename.toAscii() );
       cout<<"error opening file ";
           //<<name.toAscii();
     }
   }

void FreeEMS_Loader::initGUI()
{
	ui.chkVerify->setChecked(true);
	ui.chkRip->setChecked(true);
	setGUIState(NOTCONNECTED);
	ui.progressBar->setValue(0);
	ui.radFlashType->setChecked(1);
	ui.radFlashType->setDisabled(1);
}

void FreeEMS_Loader::setGUIState(int state)
{
	switch(state)
	{
	case NOTCONNECTED:
		ui.pushLoad->setEnabled(0);
		ui.pushRip->setEnabled(0);
		ui.pushGo->setEnabled(0);
		ui.pushConnect->setText("Connect");
	break;
	case CONNECTED:
		ui.pushLoad->setEnabled(1);
		ui.pushRip->setEnabled(1);
		ui.pushConnect->setText("Disconnect");
	default:
	break;
	}
}

void FreeEMS_Loader::test()
{
  serialConnection->setSM();
}

void FreeEMS_Loader::eraseFlash()
{
  serialConnection->eraseDevice();
}

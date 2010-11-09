#include "freeems_loader.h"
#include "freeems_LoaderRedirector.h"

FreeEMS_Loader::FreeEMS_Loader(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);

	//ui.textWindow.parent->
	//ui.
	fillBaud();
	fillDataBits();
	fillStopBits();
	fillParity();
	redirectCLI();
}

FreeEMS_Loader::~FreeEMS_Loader()
{


}

void FreeEMS_Loader::redirectCLI()
{

	//QDebug cout(std::cout, ui.textOutput&);
	//myRedirector = new StdRedirector<>(std::cout, outcallback, ui.textOutput&);

	//QDebugStream cerr(std::cerr,logTextEdit);

	std::cout <<"Hello! We are ready for working" << endl;
	//QDebugStream test;

	//QDebugStream cout(std::cout,logTextEdit);
	//Redirector test = new Redirector(std::cout, ui.textOutput);
  // ui.QDebugStream qout(std::cout, ui.textOutput);

//	StdRedirector localRedirector = new StdRedirector( std::cout, outcallback, textOutput );

	return;
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
	serialComSettings settings;

	settings.baudrate = ui.comboBaud->currentText().toUInt();
	settings.databits = ui.comboDataBits->currentText().toUInt();
	settings.hardwareHandshake = (uint)ui.chkHard->isChecked();
	settings.parity = ui.comboParity->currentText().toUInt();
	settings.port = ui.comboDevice->currentText().toAscii().data();
	settings.softwareHandshake = (uint)ui.chkSoft->isChecked();
	settings.stop = ui.comboStopBits->currentText().toUInt();

	FreeEMS_LoaderComms *connection;
	connection = new FreeEMS_LoaderComms;

	//QString  *call = ;
	connection->serialConnect(&settings);
}

void FreeEMS_Loader::outcallback( const char* ptr, std::streamsize count, void* pTextBox )
{
  (void) count;
  QTextBrowser* p = static_cast< QTextBrowser* >( pTextBox );
  p->append( ptr );
}


#include "freeems_loader.h"

FreeEMS_Loader::FreeEMS_Loader(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
	fillBaud();
	fillDataBits();
	fillStopBits();
	//fillBaud();
}

FreeEMS_Loader::~FreeEMS_Loader()
{


}



void FreeEMS_Loader::fillBaud()

{

//#ifdef B0
   comboBaud->addItem("0");
//#endif

//#ifdef B50
   comboBaud->addItem("50");
//#endif
//#ifdef B75
   comboBaud->addItem("75");
//#endif
//#ifdef B110
   comboBaud->addItem("110");
//#endif
//#ifdef B134
   comboBaud->addItem("134");
//#endif
//#ifdef B150
   comboBaud->addItem("150");
//#endif
//#ifdef B200
   comboBaud->addItem("200");
//#endif
//#ifdef B300
   comboBaud->addItem("300");
//#endif
//#ifdef B600
   comboBaud->addItem("600");
//#endif
//#ifdef B1200
   comboBaud->addItem("1200");
//#endif
//#ifdef B1800
   comboBaud->addItem("1800");
//#endif
//#ifdef B2400
   comboBaud->addItem("2400");
//#endif
//#ifdef B4800
   comboBaud->addItem("4800");
//#endif
//#ifdef B7200
   comboBaud->addItem("7200");
//#endif
//#ifdef B9600
   comboBaud->addItem("9600");
//#endif
//#ifdef B14400
   comboBaud->addItem("14400");
//#endif
//#ifdef B19200
   comboBaud->addItem("19200");
//#endif
//#ifdef B28800
   comboBaud->addItem("28800");
//#endif
//#ifdef B38400
   comboBaud->addItem("38400");
//#endif
//#ifdef B57600
   comboBaud->addItem("57600");
//#endif
//#ifdef B76800
   comboBaud->addItem("76800");
//#endif

   // this one is the default
   comboBaud->addItem("115200");
   comboBaud->setCurrentIndex(comboBaud->count()-1);

}

void FreeEMS_Loader::fillStopBits()
{
    comboStopBits->addItem("1");
    comboStopBits->addItem("2");

}

void FreeEMS_Loader::fillDataBits()
{
    comboDataBits->addItem("5");
    comboDataBits->addItem("6");
    comboDataBits->addItem("7");
    comboDataBits->addItem("8");
    comboDataBits->setCurrentIndex(comboDataBits->count()-1);
}

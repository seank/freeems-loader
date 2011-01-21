#include "freeems_loader.h"

#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FreeEMS_Loader w;

//    QObject::connect( & oThread1, SIGNAL( MySignal() ),
//            & oThread2, SLOT( MySlot() ) );

    w.show();
    return a.exec();
}

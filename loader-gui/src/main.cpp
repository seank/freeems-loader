#include "freeems_loadergui.h"

#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FreeEMS_LoaderGUI w;
    w.show();
    return a.exec();
}

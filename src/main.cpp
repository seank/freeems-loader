#include "freeems_loader.h"

#include <QtGui>
#include <QApplication>

int
main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  FreeEMS_Loader w;
  if(w.showHelp)
    {
      cout<<" help requested ";
      return 2;
    }
  w.show();
  return a.exec();
}

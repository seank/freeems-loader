#ifndef FREEEMS_LOADER_H
#define FREEEMS_LOADER_H

#include <QtGui/QWidget>
#include "ui_freeems_loader.h"

class FreeEMS_Loader : public QWidget
{
    Q_OBJECT

public:
    FreeEMS_Loader(QWidget *parent = 0);
    ~FreeEMS_Loader();

private:
    Ui::FreeEMS_LoaderClass ui;
};

#endif // FREEEMS_LOADER_H

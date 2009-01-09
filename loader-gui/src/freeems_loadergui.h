#ifndef FREEEMS_LOADERGUI_H
#define FREEEMS_LOADERGUI_H

#include <QtGui/QWidget>
#include "ui_freeems_loadergui.h"

class FreeEMS_LoaderGUI : public QWidget
{
    Q_OBJECT

public:
    FreeEMS_LoaderGUI(QWidget *parent = 0);
    ~FreeEMS_LoaderGUI();

private:
    Ui::FreeEMS_LoaderGUIClass ui;
};

#endif // FREEEMS_LOADERGUI_H

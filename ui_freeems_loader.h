/********************************************************************************
** Form generated from reading UI file 'freeems_loader.ui'
**
** Created: Fri Oct 29 15:28:14 2010
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FREEEMS_LOADER_H
#define UI_FREEEMS_LOADER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QTabWidget>
#include <QtGui/QTextBrowser>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FreeEMS_LoaderClass
{
public:
    QLabel *label;
    QPushButton *pushConnect;
    QPushButton *pushLoad;
    QTextBrowser *textWindow;
    QPushButton *pushRip;
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;
    QProgressBar *progressBar;
    QPushButton *pushLoad_2;
    QTabWidget *tabWidget;
    QWidget *tab;
    QLabel *label_2;
    QComboBox *comboDevice;
    QWidget *tab_2;
    QLabel *label_5;
    QComboBox *comboBaud;
    QLabel *label_3;
    QLabel *label_4;
    QComboBox *comboDataBits;
    QComboBox *comboStopBits;

    void setupUi(QWidget *FreeEMS_LoaderClass)
    {
        if (FreeEMS_LoaderClass->objectName().isEmpty())
            FreeEMS_LoaderClass->setObjectName(QString::fromUtf8("FreeEMS_LoaderClass"));
        FreeEMS_LoaderClass->resize(800, 547);
        label = new QLabel(FreeEMS_LoaderClass);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(370, 20, 411, 151));
        label->setPixmap(QPixmap(QString::fromUtf8("FreeEMS_Picture_5.png")));
        pushConnect = new QPushButton(FreeEMS_LoaderClass);
        pushConnect->setObjectName(QString::fromUtf8("pushConnect"));
        pushConnect->setGeometry(QRect(160, 210, 111, 27));
        pushLoad = new QPushButton(FreeEMS_LoaderClass);
        pushLoad->setObjectName(QString::fromUtf8("pushLoad"));
        pushLoad->setGeometry(QRect(330, 210, 111, 27));
        textWindow = new QTextBrowser(FreeEMS_LoaderClass);
        textWindow->setObjectName(QString::fromUtf8("textWindow"));
        textWindow->setGeometry(QRect(20, 250, 731, 211));
        pushRip = new QPushButton(FreeEMS_LoaderClass);
        pushRip->setObjectName(QString::fromUtf8("pushRip"));
        pushRip->setGeometry(QRect(500, 210, 101, 27));
        radioButton = new QRadioButton(FreeEMS_LoaderClass);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));
        radioButton->setGeometry(QRect(320, 470, 51, 22));
        radioButton_2 = new QRadioButton(FreeEMS_LoaderClass);
        radioButton_2->setObjectName(QString::fromUtf8("radioButton_2"));
        radioButton_2->setGeometry(QRect(370, 470, 41, 22));
        progressBar = new QProgressBar(FreeEMS_LoaderClass);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(170, 490, 471, 23));
        progressBar->setValue(55);
        pushLoad_2 = new QPushButton(FreeEMS_LoaderClass);
        pushLoad_2->setObjectName(QString::fromUtf8("pushLoad_2"));
        pushLoad_2->setGeometry(QRect(20, 490, 71, 31));
        tabWidget = new QTabWidget(FreeEMS_LoaderClass);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(10, 10, 351, 181));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        label_2 = new QLabel(tab);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(0, 10, 51, 17));
        comboDevice = new QComboBox(tab);
        comboDevice->setObjectName(QString::fromUtf8("comboDevice"));
        comboDevice->setGeometry(QRect(100, 10, 221, 27));
        comboDevice->setEditable(true);
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        label_5 = new QLabel(tab_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(10, 90, 61, 17));
        comboBaud = new QComboBox(tab_2);
        comboBaud->setObjectName(QString::fromUtf8("comboBaud"));
        comboBaud->setGeometry(QRect(110, 10, 141, 27));
        label_3 = new QLabel(tab_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 10, 71, 17));
        label_4 = new QLabel(tab_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 50, 61, 17));
        comboDataBits = new QComboBox(tab_2);
        comboDataBits->setObjectName(QString::fromUtf8("comboDataBits"));
        comboDataBits->setGeometry(QRect(110, 50, 78, 27));
        comboStopBits = new QComboBox(tab_2);
        comboStopBits->setObjectName(QString::fromUtf8("comboStopBits"));
        comboStopBits->setGeometry(QRect(110, 90, 61, 27));
        tabWidget->addTab(tab_2, QString());

        retranslateUi(FreeEMS_LoaderClass);
        QObject::connect(pushConnect, SIGNAL(clicked()), FreeEMS_LoaderClass, SLOT(close()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(FreeEMS_LoaderClass);
    } // setupUi

    void retranslateUi(QWidget *FreeEMS_LoaderClass)
    {
        FreeEMS_LoaderClass->setWindowTitle(QApplication::translate("FreeEMS_LoaderClass", "FreeEMS_Loader", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
        pushConnect->setText(QApplication::translate("FreeEMS_LoaderClass", "Connect", 0, QApplication::UnicodeUTF8));
        pushLoad->setText(QApplication::translate("FreeEMS_LoaderClass", "Load Firmware", 0, QApplication::UnicodeUTF8));
        pushRip->setText(QApplication::translate("FreeEMS_LoaderClass", "Rip Firmware", 0, QApplication::UnicodeUTF8));
        radioButton->setText(QApplication::translate("FreeEMS_LoaderClass", "RX", 0, QApplication::UnicodeUTF8));
        radioButton_2->setText(QApplication::translate("FreeEMS_LoaderClass", "TX", 0, QApplication::UnicodeUTF8));
        pushLoad_2->setText(QApplication::translate("FreeEMS_LoaderClass", "Go", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("FreeEMS_LoaderClass", "Device:", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("FreeEMS_LoaderClass", "Settings", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("FreeEMS_LoaderClass", "Stop bits:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("FreeEMS_LoaderClass", "Baud rate:", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("FreeEMS_LoaderClass", "Data bits:", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("FreeEMS_LoaderClass", "Advanced Settings", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class FreeEMS_LoaderClass: public Ui_FreeEMS_LoaderClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FREEEMS_LOADER_H

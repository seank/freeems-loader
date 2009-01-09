/********************************************************************************
** Form generated from reading ui file 'freeems_loadergui.ui'
**
** Created: Thu Jan 8 13:29:05 2009
**      by: Qt User Interface Compiler version 4.4.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_FREEEMS_LOADERGUI_H
#define UI_FREEEMS_LOADERGUI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QTabWidget>
#include <QtGui/QTextBrowser>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FreeEMS_LoaderGUIClass
{
public:
    QGroupBox *groupBox;
    QLabel *labelMCUStatus;
    QLabel *labelMCUConnectionStatus;
    QLabel *labelCheckSumStatus;
    QLabel *labelCheckSumStatus_2;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGroupBox *groupBoxComms;
    QPushButton *pushButton;
    QLineEdit *lineEditPort;
    QLabel *labelPort;
    QPushButton *pushButton_2;
    QGroupBox *groupBoxFirmware;
    QLineEdit *lineEditFirmware;
    QLabel *labelFirmware;
    QLabel *labelVersion;
    QLabel *labelMaintainer;
    QPushButton *pushButtonFirmware;
    QLabel *labelApplication;
    QLabel *labelChecksum;
    QWidget *tab_2;
    QTextBrowser *textBrowser;
    QPushButton *pushButtonBurn;
    QProgressBar *progressBarBurn;

    void setupUi(QWidget *FreeEMS_LoaderGUIClass)
    {
    if (FreeEMS_LoaderGUIClass->objectName().isEmpty())
        FreeEMS_LoaderGUIClass->setObjectName(QString::fromUtf8("FreeEMS_LoaderGUIClass"));
    FreeEMS_LoaderGUIClass->resize(771, 300);
    groupBox = new QGroupBox(FreeEMS_LoaderGUIClass);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    groupBox->setGeometry(QRect(609, 0, 151, 281));
    labelMCUStatus = new QLabel(groupBox);
    labelMCUStatus->setObjectName(QString::fromUtf8("labelMCUStatus"));
    labelMCUStatus->setGeometry(QRect(10, 20, 81, 16));
    labelMCUConnectionStatus = new QLabel(groupBox);
    labelMCUConnectionStatus->setObjectName(QString::fromUtf8("labelMCUConnectionStatus"));
    labelMCUConnectionStatus->setGeometry(QRect(100, 20, 46, 15));
    labelCheckSumStatus = new QLabel(groupBox);
    labelCheckSumStatus->setObjectName(QString::fromUtf8("labelCheckSumStatus"));
    labelCheckSumStatus->setGeometry(QRect(40, 50, 51, 16));
    labelCheckSumStatus_2 = new QLabel(groupBox);
    labelCheckSumStatus_2->setObjectName(QString::fromUtf8("labelCheckSumStatus_2"));
    labelCheckSumStatus_2->setGeometry(QRect(100, 50, 46, 15));
    tabWidget = new QTabWidget(FreeEMS_LoaderGUIClass);
    tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
    tabWidget->setGeometry(QRect(10, 20, 451, 261));
    tab = new QWidget();
    tab->setObjectName(QString::fromUtf8("tab"));
    groupBoxComms = new QGroupBox(tab);
    groupBoxComms->setObjectName(QString::fromUtf8("groupBoxComms"));
    groupBoxComms->setGeometry(QRect(0, 140, 361, 91));
    pushButton = new QPushButton(groupBoxComms);
    pushButton->setObjectName(QString::fromUtf8("pushButton"));
    pushButton->setGeometry(QRect(60, 60, 91, 25));
    lineEditPort = new QLineEdit(groupBoxComms);
    lineEditPort->setObjectName(QString::fromUtf8("lineEditPort"));
    lineEditPort->setGeometry(QRect(40, 20, 113, 21));
    labelPort = new QLabel(groupBoxComms);
    labelPort->setObjectName(QString::fromUtf8("labelPort"));
    labelPort->setGeometry(QRect(10, 20, 31, 16));
    pushButton_2 = new QPushButton(groupBoxComms);
    pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
    pushButton_2->setGeometry(QRect(160, 20, 51, 21));
    groupBoxFirmware = new QGroupBox(tab);
    groupBoxFirmware->setObjectName(QString::fromUtf8("groupBoxFirmware"));
    groupBoxFirmware->setGeometry(QRect(0, 10, 361, 131));
    lineEditFirmware = new QLineEdit(groupBoxFirmware);
    lineEditFirmware->setObjectName(QString::fromUtf8("lineEditFirmware"));
    lineEditFirmware->setGeometry(QRect(50, 20, 113, 21));
    labelFirmware = new QLabel(groupBoxFirmware);
    labelFirmware->setObjectName(QString::fromUtf8("labelFirmware"));
    labelFirmware->setGeometry(QRect(0, 20, 46, 15));
    labelVersion = new QLabel(groupBoxFirmware);
    labelVersion->setObjectName(QString::fromUtf8("labelVersion"));
    labelVersion->setGeometry(QRect(10, 60, 46, 15));
    labelMaintainer = new QLabel(groupBoxFirmware);
    labelMaintainer->setObjectName(QString::fromUtf8("labelMaintainer"));
    labelMaintainer->setGeometry(QRect(10, 80, 61, 16));
    pushButtonFirmware = new QPushButton(groupBoxFirmware);
    pushButtonFirmware->setObjectName(QString::fromUtf8("pushButtonFirmware"));
    pushButtonFirmware->setGeometry(QRect(170, 20, 51, 21));
    labelApplication = new QLabel(groupBoxFirmware);
    labelApplication->setObjectName(QString::fromUtf8("labelApplication"));
    labelApplication->setGeometry(QRect(10, 100, 91, 16));
    labelChecksum = new QLabel(groupBoxFirmware);
    labelChecksum->setObjectName(QString::fromUtf8("labelChecksum"));
    labelChecksum->setGeometry(QRect(190, 60, 61, 16));
    tabWidget->addTab(tab, QString());
    tab_2 = new QWidget();
    tab_2->setObjectName(QString::fromUtf8("tab_2"));
    textBrowser = new QTextBrowser(tab_2);
    textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
    textBrowser->setGeometry(QRect(0, 0, 321, 201));
    tabWidget->addTab(tab_2, QString());
    pushButtonBurn = new QPushButton(FreeEMS_LoaderGUIClass);
    pushButtonBurn->setObjectName(QString::fromUtf8("pushButtonBurn"));
    pushButtonBurn->setGeometry(QRect(480, 260, 91, 25));
    progressBarBurn = new QProgressBar(FreeEMS_LoaderGUIClass);
    progressBarBurn->setObjectName(QString::fromUtf8("progressBarBurn"));
    progressBarBurn->setGeometry(QRect(470, 62, 118, 191));
    progressBarBurn->setValue(24);
    progressBarBurn->setOrientation(Qt::Vertical);

    retranslateUi(FreeEMS_LoaderGUIClass);

    tabWidget->setCurrentIndex(0);


    QMetaObject::connectSlotsByName(FreeEMS_LoaderGUIClass);
    } // setupUi

    void retranslateUi(QWidget *FreeEMS_LoaderGUIClass)
    {
    FreeEMS_LoaderGUIClass->setWindowTitle(QApplication::translate("FreeEMS_LoaderGUIClass", "FreeEMS_LoaderGUI", 0, QApplication::UnicodeUTF8));
    groupBox->setTitle(QApplication::translate("FreeEMS_LoaderGUIClass", "Status", 0, QApplication::UnicodeUTF8));
    labelMCUStatus->setText(QApplication::translate("FreeEMS_LoaderGUIClass", "MCU Connection:", 0, QApplication::UnicodeUTF8));
    labelMCUConnectionStatus->setText(QApplication::translate("FreeEMS_LoaderGUIClass", "unknown", 0, QApplication::UnicodeUTF8));
    labelCheckSumStatus->setText(QApplication::translate("FreeEMS_LoaderGUIClass", "Checksum:", 0, QApplication::UnicodeUTF8));
    labelCheckSumStatus_2->setText(QApplication::translate("FreeEMS_LoaderGUIClass", "unknown", 0, QApplication::UnicodeUTF8));
    groupBoxComms->setTitle(QApplication::translate("FreeEMS_LoaderGUIClass", "Comms", 0, QApplication::UnicodeUTF8));
    pushButton->setText(QApplication::translate("FreeEMS_LoaderGUIClass", "Connect", 0, QApplication::UnicodeUTF8));
    labelPort->setText(QApplication::translate("FreeEMS_LoaderGUIClass", "Port:", 0, QApplication::UnicodeUTF8));
    pushButton_2->setText(QApplication::translate("FreeEMS_LoaderGUIClass", "List", 0, QApplication::UnicodeUTF8));
    groupBoxFirmware->setTitle(QApplication::translate("FreeEMS_LoaderGUIClass", "Firmware", 0, QApplication::UnicodeUTF8));
    labelFirmware->setText(QApplication::translate("FreeEMS_LoaderGUIClass", "Firmware", 0, QApplication::UnicodeUTF8));
    labelVersion->setText(QApplication::translate("FreeEMS_LoaderGUIClass", "Version:", 0, QApplication::UnicodeUTF8));
    labelMaintainer->setText(QApplication::translate("FreeEMS_LoaderGUIClass", "Maintainer:", 0, QApplication::UnicodeUTF8));
    pushButtonFirmware->setText(QApplication::translate("FreeEMS_LoaderGUIClass", "List", 0, QApplication::UnicodeUTF8));
    labelApplication->setText(QApplication::translate("FreeEMS_LoaderGUIClass", "Application Notes:", 0, QApplication::UnicodeUTF8));
    labelChecksum->setText(QApplication::translate("FreeEMS_LoaderGUIClass", "Checksum:", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("FreeEMS_LoaderGUIClass", "Main", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("FreeEMS_LoaderGUIClass", "Output", 0, QApplication::UnicodeUTF8));
    pushButtonBurn->setText(QApplication::translate("FreeEMS_LoaderGUIClass", "Burn", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(FreeEMS_LoaderGUIClass);
    } // retranslateUi

};

namespace Ui {
    class FreeEMS_LoaderGUIClass: public Ui_FreeEMS_LoaderGUIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FREEEMS_LOADERGUI_H

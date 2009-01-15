/********************************************************************************
** Form generated from reading ui file 'freeems_loader.ui'
**
** Created: Thu Jan 15 10:46:08 2009
**      by: Qt User Interface Compiler version 4.4.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_FREEEMS_LOADER_H
#define UI_FREEEMS_LOADER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FreeEMS_LoaderClass
{
public:
    QGroupBox *groupBox;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGroupBox *groupBox_2;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    QCheckBox *checkBox_3;
    QCheckBox *checkBox_4;
    QLineEdit *lineEdit;
    QPushButton *pushButton_4;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QGroupBox *groupBox_3;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;
    QWidget *tab_2;
    QGroupBox *groupBox_4;
    QWidget *tab_4;
    QWidget *tab_3;
    QProgressBar *progressBar;
    QPushButton *pushButton_7;
    QPushButton *pushButton_9;
    QLabel *label_5;
    QLabel *label_6;

    void setupUi(QWidget *FreeEMS_LoaderClass)
    {
    if (FreeEMS_LoaderClass->objectName().isEmpty())
        FreeEMS_LoaderClass->setObjectName(QString::fromUtf8("FreeEMS_LoaderClass"));
    FreeEMS_LoaderClass->resize(1102, 518);
    groupBox = new QGroupBox(FreeEMS_LoaderClass);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    groupBox->setGeometry(QRect(750, 10, 331, 361));
    label = new QLabel(groupBox);
    label->setObjectName(QString::fromUtf8("label"));
    label->setGeometry(QRect(20, 30, 101, 16));
    label_2 = new QLabel(groupBox);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    label_2->setGeometry(QRect(20, 60, 131, 16));
    label_3 = new QLabel(groupBox);
    label_3->setObjectName(QString::fromUtf8("label_3"));
    label_3->setGeometry(QRect(20, 90, 161, 16));
    label_4 = new QLabel(groupBox);
    label_4->setObjectName(QString::fromUtf8("label_4"));
    label_4->setGeometry(QRect(130, 30, 191, 16));
    tabWidget = new QTabWidget(FreeEMS_LoaderClass);
    tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
    tabWidget->setGeometry(QRect(10, 30, 721, 421));
    tab = new QWidget();
    tab->setObjectName(QString::fromUtf8("tab"));
    groupBox_2 = new QGroupBox(tab);
    groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
    groupBox_2->setGeometry(QRect(10, 10, 651, 141));
    checkBox = new QCheckBox(groupBox_2);
    checkBox->setObjectName(QString::fromUtf8("checkBox"));
    checkBox->setGeometry(QRect(450, 10, 81, 21));
    checkBox_2 = new QCheckBox(groupBox_2);
    checkBox_2->setObjectName(QString::fromUtf8("checkBox_2"));
    checkBox_2->setGeometry(QRect(550, 10, 81, 21));
    checkBox_3 = new QCheckBox(groupBox_2);
    checkBox_3->setObjectName(QString::fromUtf8("checkBox_3"));
    checkBox_3->setGeometry(QRect(450, 40, 91, 21));
    checkBox_4 = new QCheckBox(groupBox_2);
    checkBox_4->setObjectName(QString::fromUtf8("checkBox_4"));
    checkBox_4->setGeometry(QRect(550, 40, 81, 21));
    lineEdit = new QLineEdit(groupBox_2);
    lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
    lineEdit->setGeometry(QRect(130, 20, 231, 22));
    pushButton_4 = new QPushButton(groupBox_2);
    pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
    pushButton_4->setGeometry(QRect(370, 20, 51, 28));
    label_7 = new QLabel(groupBox_2);
    label_7->setObjectName(QString::fromUtf8("label_7"));
    label_7->setGeometry(QRect(40, 20, 71, 16));
    label_8 = new QLabel(groupBox_2);
    label_8->setObjectName(QString::fromUtf8("label_8"));
    label_8->setGeometry(QRect(10, 60, 131, 16));
    label_9 = new QLabel(groupBox_2);
    label_9->setObjectName(QString::fromUtf8("label_9"));
    label_9->setGeometry(QRect(40, 90, 61, 16));
    groupBox_3 = new QGroupBox(tab);
    groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
    groupBox_3->setGeometry(QRect(10, 150, 651, 151));
    pushButton_5 = new QPushButton(groupBox_3);
    pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
    pushButton_5->setGeometry(QRect(210, 110, 101, 28));
    pushButton_6 = new QPushButton(groupBox_3);
    pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));
    pushButton_6->setGeometry(QRect(330, 110, 101, 28));
    tabWidget->addTab(tab, QString());
    tab_2 = new QWidget();
    tab_2->setObjectName(QString::fromUtf8("tab_2"));
    groupBox_4 = new QGroupBox(tab_2);
    groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
    groupBox_4->setGeometry(QRect(0, 10, 501, 131));
    tabWidget->addTab(tab_2, QString());
    tab_4 = new QWidget();
    tab_4->setObjectName(QString::fromUtf8("tab_4"));
    tabWidget->addTab(tab_4, QString());
    tab_3 = new QWidget();
    tab_3->setObjectName(QString::fromUtf8("tab_3"));
    tabWidget->addTab(tab_3, QString());
    progressBar = new QProgressBar(FreeEMS_LoaderClass);
    progressBar->setObjectName(QString::fromUtf8("progressBar"));
    progressBar->setGeometry(QRect(910, 440, 118, 23));
    progressBar->setValue(24);
    pushButton_7 = new QPushButton(FreeEMS_LoaderClass);
    pushButton_7->setObjectName(QString::fromUtf8("pushButton_7"));
    pushButton_7->setGeometry(QRect(760, 390, 121, 26));
    pushButton_9 = new QPushButton(FreeEMS_LoaderClass);
    pushButton_9->setObjectName(QString::fromUtf8("pushButton_9"));
    pushButton_9->setGeometry(QRect(930, 470, 141, 28));
    label_5 = new QLabel(FreeEMS_LoaderClass);
    label_5->setObjectName(QString::fromUtf8("label_5"));
    label_5->setGeometry(QRect(760, 450, 111, 20));
    label_6 = new QLabel(FreeEMS_LoaderClass);
    label_6->setObjectName(QString::fromUtf8("label_6"));
    label_6->setGeometry(QRect(780, 470, 81, 20));

    retranslateUi(FreeEMS_LoaderClass);

    tabWidget->setCurrentIndex(0);


    QMetaObject::connectSlotsByName(FreeEMS_LoaderClass);
    } // setupUi

    void retranslateUi(QWidget *FreeEMS_LoaderClass)
    {
    FreeEMS_LoaderClass->setWindowTitle(QApplication::translate("FreeEMS_LoaderClass", "FreeEMS_Loader", 0, QApplication::UnicodeUTF8));
    groupBox->setTitle(QApplication::translate("FreeEMS_LoaderClass", "Status", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("FreeEMS_LoaderClass", "MCU Connection:", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("FreeEMS_LoaderClass", "Source s19 Checksum:", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("FreeEMS_LoaderClass", "Last Average Transfer Rate", 0, QApplication::UnicodeUTF8));
    label_4->setText(QApplication::translate("FreeEMS_LoaderClass", "Detected $freeems-version", 0, QApplication::UnicodeUTF8));
    groupBox_2->setTitle(QApplication::translate("FreeEMS_LoaderClass", "s19 To ECU Actions", 0, QApplication::UnicodeUTF8));
    checkBox->setText(QApplication::translate("FreeEMS_LoaderClass", "Burn All", 0, QApplication::UnicodeUTF8));
    checkBox_2->setText(QApplication::translate("FreeEMS_LoaderClass", "Burn Code", 0, QApplication::UnicodeUTF8));
    checkBox_3->setText(QApplication::translate("FreeEMS_LoaderClass", "Burn Config", 0, QApplication::UnicodeUTF8));
    checkBox_4->setText(QApplication::translate("FreeEMS_LoaderClass", "Burn Tables", 0, QApplication::UnicodeUTF8));
    pushButton_4->setText(QApplication::translate("FreeEMS_LoaderClass", "Open", 0, QApplication::UnicodeUTF8));
    label_7->setText(QApplication::translate("FreeEMS_LoaderClass", "Source File:", 0, QApplication::UnicodeUTF8));
    label_8->setText(QApplication::translate("FreeEMS_LoaderClass", "Application Notes:", 0, QApplication::UnicodeUTF8));
    label_9->setText(QApplication::translate("FreeEMS_LoaderClass", "Maintainer:", 0, QApplication::UnicodeUTF8));
    groupBox_3->setTitle(QApplication::translate("FreeEMS_LoaderClass", "ECU to s19 Actions", 0, QApplication::UnicodeUTF8));
    pushButton_5->setText(QApplication::translate("FreeEMS_LoaderClass", "Download Config", 0, QApplication::UnicodeUTF8));
    pushButton_6->setText(QApplication::translate("FreeEMS_LoaderClass", "Download Tables", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("FreeEMS_LoaderClass", "Main", 0, QApplication::UnicodeUTF8));
    groupBox_4->setTitle(QApplication::translate("FreeEMS_LoaderClass", "Com Settings", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("FreeEMS_LoaderClass", "Settings", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("FreeEMS_LoaderClass", "Diagnostics", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("FreeEMS_LoaderClass", "Output", 0, QApplication::UnicodeUTF8));
    pushButton_7->setText(QApplication::translate("FreeEMS_LoaderClass", "Connect/Disconnect", 0, QApplication::UnicodeUTF8));
    pushButton_9->setText(QApplication::translate("FreeEMS_LoaderClass", "Execute Action", 0, QApplication::UnicodeUTF8));
    label_5->setText(QApplication::translate("FreeEMS_LoaderClass", "Requested Actions", 0, QApplication::UnicodeUTF8));
    label_6->setText(QApplication::translate("FreeEMS_LoaderClass", "Burn All From s19", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(FreeEMS_LoaderClass);
    } // retranslateUi

};

namespace Ui {
    class FreeEMS_LoaderClass: public Ui_FreeEMS_LoaderClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FREEEMS_LOADER_H

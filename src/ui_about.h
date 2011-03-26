/********************************************************************************
** Form generated from reading UI file 'about.ui'
**
** Created: Sat Mar 26 08:32:07 2011
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUT_H
#define UI_ABOUT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QTextBrowser>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AboutClass
{
public:
    QTextBrowser *textBrowser;
    QLabel *label;

    void setupUi(QWidget *AboutClass)
    {
        if (AboutClass->objectName().isEmpty())
            AboutClass->setObjectName(QString::fromUtf8("AboutClass"));
        AboutClass->resize(444, 422);
        textBrowser = new QTextBrowser(AboutClass);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(10, 160, 421, 231));
        label = new QLabel(AboutClass);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 30, 401, 121));
        label->setPixmap(QPixmap(QString::fromUtf8("images/FreeEMS_Picture_5.png")));

        retranslateUi(AboutClass);

        QMetaObject::connectSlotsByName(AboutClass);
    } // setupUi

    void retranslateUi(QWidget *AboutClass)
    {
        AboutClass->setWindowTitle(QApplication::translate("AboutClass", "About", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class AboutClass: public Ui_AboutClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUT_H

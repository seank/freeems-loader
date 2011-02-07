/********************************************************************************
** Form generated from reading UI file 'about.ui'
**
** Created: Mon Feb 7 09:36:30 2011
**      by: Qt User Interface Compiler version 4.6.3
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
#include <QtGui/QTextBrowser>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AboutClass
{
public:
    QTextBrowser *textBrowser;

    void setupUi(QWidget *AboutClass)
    {
        if (AboutClass->objectName().isEmpty())
            AboutClass->setObjectName(QString::fromUtf8("AboutClass"));
        AboutClass->resize(455, 300);
        textBrowser = new QTextBrowser(AboutClass);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(20, 20, 421, 231));

        retranslateUi(AboutClass);

        QMetaObject::connectSlotsByName(AboutClass);
    } // setupUi

    void retranslateUi(QWidget *AboutClass)
    {
        AboutClass->setWindowTitle(QApplication::translate("AboutClass", "About", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class AboutClass: public Ui_AboutClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUT_H

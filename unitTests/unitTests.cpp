#include <sRecord.h>
#include <QtTest/QtTest>

 class unitTests: public QObject
 {
     Q_OBJECT
 private slots:
     void toUpper();
 };

 /* we will keep this test here since we know it *should always pass */
 void unitTests::toUpper()
 {
     QString str = "Hello";
     QCOMPARE(str.toUpper(), QString("HELLO"));
 }

QTEST_MAIN(unitTests)
#include "unitTests.moc"

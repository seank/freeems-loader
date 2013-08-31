#include <sRecord.h>
#include <QtTest/QtTest>

 class unitTests: public QObject
 {
     Q_OBJECT
 private slots:
     void createValidS2RecordFromString();
 };

 void unitTests::createValidS2RecordFromString() {
	 string  validRecord = "S214FD82D001AE3048180B08004E180400443046FE28";
	 FreeEMS_LoaderSREC record;
	 record.createFromString(&validRecord);
	 QCOMPARE(record.getPayloadStatus(), (int)LOADABLE);
 }

QTEST_MAIN(unitTests)
#include "unitTests.moc"

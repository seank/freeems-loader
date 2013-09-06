#include <sRecord.h>
#include <QtTest/QtTest>

 class unitTests: public QObject
 {
     Q_OBJECT
 private slots:
     void createValidS2RecordFromString();
     void checkS2RecordForBadCSUM();
     void checkS2RecordForCorruptionCSUM();
     void checkS2TooShort();
     void checkS2TooLong();
     void checkS2RecordAddress();
 };

 void unitTests::createValidS2RecordFromString() {
	 string  validRecord = "S214FD82D001AE3048180B08004E180400443046FE28";
	 FreeEMS_LoaderSREC record;
	 record.createFromString(&validRecord);
	 QCOMPARE(record.getPayloadStatus(), (int)LOADABLE);
 }

 void unitTests::checkS2RecordForBadCSUM() {
     //the last octet should be 28
     string  validRecord = "S214FD82D001AE3048180B08004E180400443046FE27";
     FreeEMS_LoaderSREC record;
     record.createFromString(&validRecord);
     QCOMPARE(record.getPayloadStatus(), (int)UNLOADABLE_BAD_CSUM);
 }

 void unitTests::checkS2RecordForCorruptionCSUM() {
     //the AD should be AE
     string  validRecord = "S214FD82D001AD3048180B08004E180400443046FE27";
     FreeEMS_LoaderSREC record;
     record.createFromString(&validRecord);
     QCOMPARE(record.getPayloadStatus(), (int)UNLOADABLE_BAD_CSUM);
 }

 void unitTests::checkS2TooShort() {
     //Three chars have been clipped off of a valid record
     string  validRecord = "S214FD82D001AD3048180B08004E180400443046F";
     FreeEMS_LoaderSREC record;
     record.createFromString(&validRecord);
     QCOMPARE(record.getPayloadStatus(), (int)UNLOADABLE_BAD_LENGTH_TOO_SHORT);
 }

 void unitTests::checkS2TooLong() {
     //one space has been added to a valid record
     string  validRecord = "S214FD82D001AD3048180B08004E180400443046FE27 ";
     FreeEMS_LoaderSREC record;
     record.createFromString(&validRecord);
     QCOMPARE(record.getPayloadStatus(), (int)UNLOADABLE_BAD_LENGTH_TOO_LONG);
 }

 void unitTests::checkS2RecordAddress() {
     string  validRecord = "S214FD82D001AE3048180B08004E180400443046FE28";
     FreeEMS_LoaderSREC record;
     record.createFromString(&validRecord);
     QCOMPARE(record.getRecordAddress(), (unsigned int)0xFD82D0);
 }

 void unitTests::checkS2RecordLength() {
     string  validRecord = "S214FD82D001AE3048180B08004E180400443046FE28";
     FreeEMS_LoaderSREC record;
     record.createFromString(&validRecord);
     QCOMPARE(record.getRecordAddress(), (unsigned int)0xFD82D0);
 }

QTEST_MAIN(unitTests)
#include "unitTests.moc"

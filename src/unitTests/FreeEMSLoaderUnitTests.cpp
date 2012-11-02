/*
 * S19UT.cpp
 *
 *  Created on: Oct 23, 2012
 *      Author: seank
 */


#include <QtTest/QtTest>
#include <sRecord.h>

 class FreeEMSLoaderUnitTests: public QObject
 {
     Q_OBJECT
 private slots:
     void sRecordInit();
 	 void sRecordCheckSum();
 	 void sRecordCheckSumFail();
 	 void sRecordLengthFail();
 };

 /* check to see that our variables are initialized properly */
 void FreeEMSLoaderUnitTests::sRecordInit() {
     FreeEMS_LoaderSREC sRecord;
     QCOMPARE(sRecord.payloadAddress, (unsigned int) 0);
     QCOMPARE(sRecord.isRecordNull(), true);
     QCOMPARE(sRecord.isCheckSumMismatched(), false);
     QCOMPARE(sRecord.areCharactersValid(), true);
     QCOMPARE(sRecord.isLineLengthCorrect(), true);
     QCOMPARE(sRecord.getRecordAddress(), (unsigned int) 0);
     QCOMPARE(sRecord.getRecordTypeIndex(), (int) 0);
 }

 void FreeEMSLoaderUnitTests::sRecordCheckSum() {
     std::string goodRecord = "S214E08810000000040000000500000006000000075D";
	 goodRecord += '\n';
     FreeEMS_LoaderSREC sRecord;
     sRecord.createFromString(&goodRecord);
     QCOMPARE(sRecord.getCalculatedSum(), (unsigned char) 0x5D);
 }

 void FreeEMSLoaderUnitTests::sRecordCheckSumFail() {
     std::string goodRecord = "S214E08810000000040010000500000006000000075D";
 	 goodRecord += '\n';
     FreeEMS_LoaderSREC sRecord;
     sRecord.createFromString(&goodRecord);
     QCOMPARE(sRecord.isCheckSumMismatched(), true);
  }

 void FreeEMSLoaderUnitTests::sRecordLengthFail() {
     std::string goodRecord = "S214E0881000000040000000500000006000000075D";
 	 goodRecord += '\n';
     FreeEMS_LoaderSREC sRecord;
     sRecord.createFromString(&goodRecord);
     QCOMPARE(sRecord.isLineLengthCorrect(), false);
  }

 void FreeEMSLoaderUnitTests::sRecordLengthFail() {
     std::string goodRecord = "S214E0881000g00040000000500000006000000075D";
  	 goodRecord += '\n';
     FreeEMS_LoaderSREC sRecord;
     sRecord.createFromString(&goodRecord);
     QCOMPARE(sRecord.areCharactersValid(), false);
   }


 QTEST_MAIN(FreeEMSLoaderUnitTests)
 #include "FreeEMSLoaderUnitTests.moc"

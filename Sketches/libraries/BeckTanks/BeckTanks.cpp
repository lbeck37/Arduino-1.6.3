//BeckTanks.cpp
#include <BeckTanks.h>

BeckTanks::BeckTanks(void) {
  String szLogString="BeckTanks Constructor: Begin";
  LogToSerial(szLogString);
  return;
} //Constructor


void BeckTanks::Read(void) {
  double     adTankVoltReading[sNumTanks_];
  String     szLogString;

   if (millis() > ulNextTanksTime_) {
    ulNextTanksTime_= millis() + ulTanksReadTime_;
    for (int sTank= eGrey1; sTank <= sNumTanks_; sTank++) {
       adTankVoltReading[sTank]= 3.737;
    }  //for

   }  //if (millis()>ulNextTanksTime)
   return;
} //Read
//Last line.

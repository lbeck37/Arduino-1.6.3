//BeckTanks.cpp
#include <BeckTanks.h>

BeckTanks::BeckTanks(BeckDisplay*	pBeckDisplay) {
  String szLogString="BeckTanks Constructor: Begin";
  LogToSerial(szLogString);
  pBeckDisplay_= pBeckDisplay;
  return;
} //Constructor


void BeckTanks::Read(void) {
  double     adTankVoltReading[sNumTanks_];
  String     szLogString;

   if (millis() > ulNextTanksTime_) {
    ulNextTanksTime_= millis() + ulTanksReadTime_;
    for (int sTank= eGrey1; sTank < sNumTanks_; sTank++) {
    		adVolts[sTank]= pBeckAtoD->dRead(sTank + 1, GAIN_ONE);
    }  //for
   }  //if (millis()>ulNextTanksTime)
   return;
} //Read


void BeckTanks::UpdateDisplay(void) {
   return;
} //UpdateDisplay
//Last line.

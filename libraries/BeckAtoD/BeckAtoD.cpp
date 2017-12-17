//BeckAtoD.cpp, Dec 17, 2017, Lenny-c
#include <BeckAtoD.h>

BeckAtoD::BeckAtoD(BeckI2C* pBeckI2C, AtoD_t eType){
  String szLogString="BeckAtoD Constructor: Begin";
  LogToSerial(szLogString);
  pBeckI2C_= pBeckI2C;
  eType_= eType;
  return;
} //Constructor


double BeckAtoD::dRead(INT16 sChan, adsGain_t eGain) {
/*
  if (bDevicePresent_) {
    return(dRead_ADS1115(sChan, eGain));
  }
  else {
    double dReturn= 3.737;
    String szLogString="BeckAtoD::dRead(): I2C AtoD not present, returning";
    //LogToSerial(szLogString, dReturn);
    return (dReturn);
  }
*/
  return(dRead_ADS1115(sChan, eGain));
} //dRead


double BeckAtoD::dRead_ADS1115(INT16 sChannel, adsGain_t eGain) {
  UINT16  usConfig= usDefaultSingleChanReadConfig_;
  Serial << "dRead_ADS1115(): Begin " << endl;

  usConfig |= eGain;
  switch (sChannel) {
    case (0):
      usConfig |= ADS1015_REG_CONFIG_MUX_SINGLE_0;
      break;
    case (1):
      usConfig |= ADS1015_REG_CONFIG_MUX_SINGLE_1;
      break;
    case (2):
      usConfig |= ADS1015_REG_CONFIG_MUX_SINGLE_2;
      break;
    case (3):
      usConfig |= ADS1015_REG_CONFIG_MUX_SINGLE_3;
      break;
    default:
      Serial << "dRead_ADS1115(): Bad sChannel switch= " << sChannel << endl;
      break;
  } //switch

  pBeckI2C_->WriteI2cRegister(ucADS1115_Address_, ADS1015_REG_POINTER_CONFIG, usConfig);
  delay(50);      //Adafruit code only delays for 8

  INT16 sVoltCount= pBeckI2C_->ReadI2cRegister(ucADS1115_Address_, ADS1015_REG_POINTER_CONVERT);
  Serial << "dRead_ADS1115(): sChannel, eGain, sVoltCount " << sChannel << ", " << eGain << ", " << sVoltCount << endl;

  double dVoltsFullScale= 2.048;
  double dCountFullScale= 32767.0;
  double dVoltsRead= ((double)sVoltCount * dVoltsFullScale) / dCountFullScale;
  Serial << "dRead_ADS1115(): dVoltsRead= " << dVoltsRead << endl;
  return(dVoltsRead);
} //dRead_ADS1115
//Last line.

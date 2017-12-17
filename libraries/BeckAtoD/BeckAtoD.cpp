//BeckAtoD.cpp, Dec 17, 2017, Lenny-d
#include <BeckAtoD.h>

BeckAtoD::BeckAtoD(BeckI2C* pBeckI2C, AtoD_t eType){
  String szLogString="BeckAtoD Constructor: Begin";
  LogToSerial(szLogString);
  pBeckI2C_= pBeckI2C;
  eType_= eType;
  return;
} //Constructor


double BeckAtoD::dReadRawVolts(INT16 sChan, adsGain_t eGain) {
  double	dRawVolts= dRead_ADS1115(sChan, eGain);
  return(dRawVolts);
} //dReadRawVolts


double BeckAtoD::dReadRealVolts(INT16 sChan, adsGain_t eGain, const double dDivider[]) {
  double	dRealVolts= dRead_ADS1115(sChan, eGain) * dDivider[sChan];
  return(dRealVolts);
} //dReadRealVolts


double BeckAtoD::dReadAmps(INT16 sChan, adsGain_t eGain, const double dDivider[]) {
	//Read current from ACS712 +/-20A full scale current sense
	//Vcc supply is nominal 5V but is read on channel 2 to use in current calculation
	//Zero amps is at 1/2 Vcc (2.5V nominal) and swings +/-2.0V (0.5 - 4.5)
  double	dRawVolts;
/*
  double	dRawVolts;
  double	dRawVolts;
  double	dRawVolts;
  double	dRawVolts;
*/
	dRawVolts= dRead_ADS1115(sChan, eGain) * dDivider[sChan];
  return(dRawVolts);
} //dReadAmps


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

//BeckI2C.cpp
#include <BeckI2C.h>
#include <Wire.h>

BeckI2C::BeckI2C(INT16 sDummy) {
  String szLogString="BeckI2C Constructor: Begin";
  LogToSerial(szLogString);
  szLogString = "BeckI2C Constructor: Call Wire.begin()";
  LogToSerial(szLogString);
  Wire.begin();
  TestI2C();
  return;
} //Constructor


void BeckI2C::TestI2C(void) {
	ucTestI2C(0x48);
	ucTestI2C(0x68);
  return;
} //TestI2C:void


UINT8 BeckI2C::ucTestI2C(UINT8 ucAddress) {
	UINT8 		ucError;
  String 		szLogString;
  if (!bSkipTestI2c_) {
		Wire.beginTransmission(ucAddress);
		ucError= Wire.endTransmission();
		if (ucError == 0) {
			szLogString="TestI2C(): Good response from";
		}
		else {
			szLogString="TestI2C(): ERROR: No response from";
		}
		LogToSerial(szLogString, ucAddress);
  }
  return ucError;
} //ucTestI2C


//Writes 16-bits to the specified destination register
//void WriteI2cRegister(uint8_t i2cAddress, uint8_t reg, uint16_t value) {
void BeckI2C::WriteI2cRegister(UINT8 ucI2cAddress, UINT8 ucRegister, UINT16 usValue) {
  TestI2C();
  Wire.beginTransmission(ucI2cAddress);
  Wire.write(ucRegister);
  Wire.write((UINT8)(usValue >> 8));      //High byte
  Wire.write((UINT8)(usValue & 0xFF));    //Low byte
  Wire.endTransmission();
} //WriteI2cRegister


//Reads 16-bits from the specified source register
INT16 BeckI2C::ReadI2cRegister(UINT8 ucI2cAddress, UINT8 ucRegister) {
  Wire.beginTransmission(ucI2cAddress);
  Wire.write(ucRegister);                     //Was ADS1015_REG_POINTER_CONVERT
  Wire.endTransmission();

  //Read the high byte and then the low byte
  Wire.requestFrom(ucI2cAddress, (UINT8)2);
  INT16 sReturn= ((Wire.read() << 8) | Wire.read());
  return sReturn;
} //ReadI2cRegister


INT16 BeckI2C::sReadTwoBytes(void) {
  return(Wire.read() << 8 | Wire.read());
} //sReadTwoBytes
//Last line.

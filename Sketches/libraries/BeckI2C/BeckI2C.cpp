//BeckI2C.cpp
#include <BeckI2C.h>
#include <Wire.h>

BeckI2C::BeckI2C(INT16 sDummy) {
  String szLogString="BeckI2C Constructor: Begin";
  LogToSerial(szLogString);
  szLogString = "BeckI2C Constructor: &Wire=";
  LogToSerial(szLogString, (UINT32)&Wire);
  //this->sSetup_I2C();
  Wire.begin();
  TestI2C(0x48);
  TestI2C(0x68);
  return;
} //Constructor

void BeckI2C::TestI2C(UINT8 ucAddress) {
	String szLogString;
	Wire.beginTransmission(ucAddress);
	UINT8 ucError= Wire.endTransmission();
	if (ucError == 0) {
		szLogString="TestI2C(): Good responce from";
	}
	else {
		szLogString="TestI2C(): ERROR: No responce from";
	}
	LogToSerial(szLogString, ucAddress);
	return;
}	//TestI2C


//Writes 16-bits to the specified destination register
//void WriteI2cRegister(uint8_t i2cAddress, uint8_t reg, uint16_t value) {
void BeckI2C::WriteI2cRegister(UINT8 ucI2cAddress, UINT8 ucRegister, UINT16 usValue) {
  String szLogString = "BeckI2C::::WriteI2cRegister(): &Wire=";
  LogToSerial(szLogString, (UINT32)&Wire);

  szLogString = "BeckI2C::::WriteI2cRegister(): &ucI2cAddress=";
  LogToSerial(szLogString, ucI2cAddress);
  TestI2C(ucI2cAddress);

  Wire.beginTransmission(ucI2cAddress);
  Wire.write(ucRegister);
  Wire.write((UINT8)(usValue >> 8));      //High byte
  Wire.write((UINT8)(usValue & 0xFF));    //Low byte
  Wire.endTransmission();
} //WriteI2cRegister


//Reads 16-bits from the specified source register
//static uint16_t ReadI2cRegister(uint8_t i2cAddress, uint8_t reg) {
INT16 BeckI2C::ReadI2cRegister(UINT8 ucI2cAddress, UINT8 ucRegister) {
  Wire.beginTransmission(ucI2cAddress);
  Wire.write(ucRegister);                     //Was ADS1015_REG_POINTER_CONVERT
  Wire.endTransmission();

  //Read the high byte and then the low byte
  Wire.requestFrom(ucI2cAddress, (UINT8)2);
  INT16 sReturn= ((Wire.read() << 8) | Wire.read());
  return sReturn;
} //ReadI2cRegister


/*
INT16 BeckI2C::sSetup_I2C() {
  String szLogString="BeckI2C:sSetup_I2C(): call Wire.begin()";
  LogToSerial(szLogString);
  Wire.begin();
  //Wire.begin(4, 5);
  return 1;
} //sSetup_I2C
*/


INT16 BeckI2C::sReadTwoBytes(void) {
  return(Wire.read() << 8 | Wire.read());
} //sReadTwoBytes
//Last line.

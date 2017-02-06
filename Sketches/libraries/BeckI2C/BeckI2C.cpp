//BeckI2C.cpp
#include <BeckI2C.h>
#include <Wire.h>

BeckI2C::BeckI2C(INT16 sDummy) {
	return;
}	//Constructor


//Writes 16-bits to the specified destination register
//void WriteI2cRegister(uint8_t i2cAddress, uint8_t reg, uint16_t value) {
void BeckI2C::WriteI2cRegister(UINT8 ucI2cAddress, UINT8 ucRegister, UINT16 usValue) {
  Wire.beginTransmission(ucI2cAddress);
  Wire.write(ucRegister);
  Wire.write((UINT8)(usValue >> 8));			//High byte
  Wire.write((UINT8)(usValue & 0xFF));		//Low byte
  Wire.endTransmission();
}	//WriteI2cRegister


//Reads 16-bits from the specified source register
//static uint16_t ReadI2cRegister(uint8_t i2cAddress, uint8_t reg) {
INT16 BeckI2C::ReadI2cRegister(UINT8 ucI2cAddress, UINT8 ucRegister) {
  Wire.beginTransmission(ucI2cAddress);
  Wire.write(ucRegister);											//Was ADS1015_REG_POINTER_CONVERT
  Wire.endTransmission();

  //Read the high byte and then the low byte
  Wire.requestFrom(ucI2cAddress, (UINT8)2);
  INT16 sReturn= ((Wire.read() << 8) | Wire.read());
  return sReturn;
}	//ReadI2cRegister


INT16 BeckI2C::sSetup_I2C() {
  Wire.begin();
  return 1;
}	//sSetup_I2C


INT16 BeckI2C::sReadTwoBytes(void) {
	return(Wire.read() << 8 | Wire.read());
}	//sReadTwoBytes
//Last line.

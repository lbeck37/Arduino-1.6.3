//BeckI2C.cpp, Dec 16, 2017, Lenny-a
#include <BeckI2C.h>

BeckI2C::BeckI2C(INT16 sI2C_SDA, INT16 sI2C_SCL) {
  String szLogString="BeckI2C Constructor: Begin";
  LogToSerial(szLogString);
  sI2C_SDA_= sI2C_SDA;
  sI2C_SCL_= sI2C_SCL;
  //ScanForDevices();
  TestAllI2C();
  return;
} //Constructor


void BeckI2C::Begin() {
  Serial << "BeckI2C::Begin(): Call Wire.begin(sI2C_SDA, sI2C_SCL) " << sI2C_SDA_ << ", " << sI2C_SCL_ << endl;
  Wire.begin(sI2C_SDA_, sI2C_SCL_);
  return;
} //Begin


bool BeckI2C::bDevicePresent(I2cDevice_t eDevice) {
  return abDevicePresent_[eDevice];
} //bDevicePresent


void BeckI2C::SetDevicePresent(UINT8 ucAddress) {
  String szLogString="BeckI2C::SetDevicePresent(): Address";
  LogToSerial(szLogString, ucAddress);

/*
	I2cDevice_t	eI2cDevice= mI2cAddresses_[ucAddress];
	abDevicePresent_[eI2cDevice]= true;
  szLogString="BeckI2C::SetDevicePresent(): Device";
  LogToSerial(szLogString, eI2cDevice);
*/

  return;
} //SetDevicePresent


void BeckI2C::ScanForDevices(void){
	UINT8 ucError, ucAddress;
  int nDevices;
  String szLogString="BeckI2C::ScanForDevices(): Scanning";
  LogToSerial(szLogString);

  //Serial.println("Scanning...");
  nDevices = 0;
  for(ucAddress = 1; ucAddress < 127; ucAddress++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(ucAddress);
    ucError = Wire.endTransmission();

    if (ucError == 0)
    {
    	SetDevicePresent(ucAddress);
      Serial.print("I2C device found at address 0x");
      if (ucAddress<16) {
        Serial.print("0");
      }
      Serial.print(ucAddress,HEX);
      Serial.println("  !");
      nDevices++;
    }
    else if (ucError==4)
    {
      Serial.print("Unknown error at address 0x");
      if (ucAddress<16) {
        Serial.print("0");
      }
      Serial.println(ucAddress,HEX);
    }
  }
 /*if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done");
*/
  return;
}	//ScanForDevices


void BeckI2C::TestAllI2C(void) {
	TestI2C(0x48);
	TestI2C(0x68);
  return;
} //TestI2C:void


UINT8 BeckI2C::TestI2C(UINT8 ucAddress) {
	UINT8 		ucError			= 0;
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
  TestAllI2C();
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

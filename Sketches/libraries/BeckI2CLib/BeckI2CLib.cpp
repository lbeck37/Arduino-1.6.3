//BeckI2cLib.cpp
#include <BeckI2cLib.h>
#include <Wire.h>

//******* MPU-6050 6-axis accelerometer and gyro
const int MPU= 0x68;  // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

//Gyro defines
const int       sXAxis             = 0;
const int       sYAxis             = 1;
const int       sZAxis             = 2;
const int       sNumAxis           = 3;

const int       sAccel             = 0;
const int       sRotation          = 1;
const int       sTemperature       = 2;
const int       sNumGyroTypes      = 3;
const uint32_t  ulGyroReadTime     = 500;   //Gyro reads spaced by this.

uint32_t        ulNextGyroTime     = 0;  //msec when the gyro will be read
boolean         bGyroChanged       = false;

INT16           asGyro[sNumGyroTypes][sNumAxis];  //Was int

const UINT8			ucADS1115_Address	 = 0x48;

const UINT16 usDefaultSingleChanReadConfig=
																	ADS1015_REG_CONFIG_CQUE_NONE    | // Disable the comparator (default val)
																	ADS1015_REG_CONFIG_CLAT_NONLAT  | // Non-latching (default val)
																	ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
																	ADS1015_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
																	ADS1015_REG_CONFIG_DR_1600SPS   | // 1600 samples per second (default)
																	ADS1015_REG_CONFIG_MODE_SINGLE  | // Single-shot mode (default)
																	ADS1015_REG_CONFIG_OS_SINGLE;     // Single-shot start conversion

//*********************************************************************************
//Local function protos
void 			WriteI2cRegister		(UINT8 ucI2cAddress, UINT8 ucRegister, UINT16 usValue);
UINT16 		ReadI2cRegister			(UINT8 ucI2cAddress, UINT8 ucRegister);

//Writes 16-bits to the specified destination register
//void WriteI2cRegister(uint8_t i2cAddress, uint8_t reg, uint16_t value) {
void WriteI2cRegister(UINT8 ucI2cAddress, UINT8 ucRegister, UINT16 usValue) {
  Wire.beginTransmission(ucI2cAddress);
  Wire.write(ucRegister);
  Wire.write((UINT8)(usValue >> 8));			//High byte
  Wire.write((UINT8)(usValue & 0xFF));		//Low byte
  Wire.endTransmission();
}	//WriteI2cRegister


//Reads 16-bits from the specified source register
//static uint16_t ReadI2cRegister(uint8_t i2cAddress, uint8_t reg) {
UINT16 ReadI2cRegister(UINT8 ucI2cAddress, UINT8 ucRegister) {
  Wire.beginTransmission(ucI2cAddress);
  Wire.write(ucRegister);											//Was ADS1015_REG_POINTER_CONVERT
  Wire.endTransmission();

  //Read the high byte and then the low byte
  Wire.requestFrom(ucI2cAddress, (UINT8)2);
  UINT16 usReturn= ((Wire.read() << 8) | Wire.read());
  return usReturn;
}	//ReadI2cRegister


//Exported functions
INT16 sSetup_I2C() {
  Wire.begin();
  return 1;
}	//sSetup_I2C


INT16 sSetup_ADS1115() {
  return 1;
}	//sSetup_ADS1115


double dRead_ADS1115(INT16 sChannel, adsGain_t eGain) {
  UINT16	usConfig= usDefaultSingleChanReadConfig;

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
      String szLogString="dRead_ADS1115(): Bad switch";
      LogToBoth(szLogString, sChannel);
    	break;
  }	//switch

  WriteI2cRegister(ucADS1115_Address, ADS1015_REG_POINTER_CONFIG, usConfig);
  delay(50);			//Adafruit code only delays for 8.
  UINT16 usVoltCount= ReadI2cRegister(ucADS1115_Address, ADS1015_REG_POINTER_CONVERT);

  double dVoltsRead= (double)usVoltCount / 60000.0;
  return(dVoltsRead);
}	//dRead_ADS1115


INT16 sSetup_Gyro() {
   //Serial << sLC++ <<"sSetupGyro(): Begin"<< endl;
   BLog("sSetup_Gyro(): Begin");
   Wire.beginTransmission(MPU);
   Wire.write(0x6B);  // PWR_MGMT_1 register
   Wire.write(0);     // set to zero (wakes up the MPU-6050)
   Wire.endTransmission(true);
   //Initialize the data array.
   for (int sDataType= sAccel; sDataType < sNumGyroTypes; sDataType++) {
      for (int sAxis= sXAxis; sAxis < sNumAxis; sAxis++) {
         asGyro[sDataType][sAxis]= 0;
      }  //for sDataType
   }  //for sAxis
   return 1;
}  //sSetup_Gyro


void Read_Gyro() {
  INT16      asGyroReading[sNumGyroTypes][sNumAxis];
   //boolean  bApplySmoothing= APPLY_SMOOTHING;

   if (millis() > ulNextGyroTime) {
      Wire.beginTransmission(MPU);
      Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
      Wire.endTransmission(false);
      //Wire.requestFrom(MPU,14,true);  // request a total of 14 registers
      //bool  bTrue= true;
      Wire.requestFrom((uint8_t)MPU, (size_t)14, (bool)true);  // request a total of 14 registers

      // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
      // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
      // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
      // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
      // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
      // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
      // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
      asGyroReading[sAccel][sXAxis]= Wire.read()<<8|Wire.read();
      asGyroReading[sAccel][sYAxis]= Wire.read()<<8|Wire.read();
      asGyroReading[sAccel][sZAxis]= Wire.read()<<8|Wire.read();

      asGyroReading[sTemperature][sXAxis]= Wire.read()<<8|Wire.read();

      asGyroReading[sRotation][sXAxis]=Wire.read()<<8|Wire.read();
      asGyroReading[sRotation][sYAxis]=Wire.read()<<8|Wire.read();
      asGyroReading[sRotation][sZAxis]=Wire.read()<<8|Wire.read();

      //Initialize missing temperature fields.
      for (int sAxis= sYAxis; sAxis < sNumAxis; sAxis++) {
         asGyroReading[sTemperature][sAxis]= 0;
      }  //for

      //Apply low-pass filter to data
      for (int sDataType= sAccel; sDataType < sNumGyroTypes; sDataType++) {
         for (int sAxis= sXAxis; sAxis < sNumAxis; sAxis++) {
#if APPLY_SMOOTHING
            asGyro[sDataType][sAxis]= FILTER_FUNC(asGyroReading[sDataType][sAxis],
                                                  pusSmoothingMemory[sDataType][sAxis]);
#else
            asGyro[sDataType][sAxis]= asGyroReading[sDataType][sAxis];
#endif
         }  //for sDataType
      }  //for sAxis
      //The following is for bringing up gyro
      String szLogString="Read_Gyro(): AccelZ";
      INT16 sAccelZaxis= asGyro[sAccel][sZAxis];
      LogToBoth(szLogString, sAccelZaxis);

      bGyroChanged= true;
      ulNextGyroTime= millis() + ulGyroReadTime;
   }  //if (millis()>ulNextGyroTime)
   return;
}  //Read_Gyro
//Last line.

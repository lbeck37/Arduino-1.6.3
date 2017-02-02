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

//*********************************************************************************
void SetupGyro() {
   //Serial << sLC++ <<"sSetupGyro(): Begin"<< endl;
   BLog("sSetupGyro(): Begin");
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
   return;
}  //SetupGyro


void ReadGyro() {
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
      String szLogString="ReadGyro(): AccelZ";
      INT16 sAccelZaxis= asGyro[sAccel][sZAxis];
      LogToBoth(szLogString, sAccelZaxis);

      bGyroChanged= true;
      ulNextGyroTime= millis() + ulGyroReadTime;
   }  //if (millis()>ulNextGyroTime)
   return;
}  //ReadGyro
//Last line.

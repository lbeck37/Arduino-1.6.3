//BeckGyro.cpp
#include <BeckGyro.h>
//#include <BeckI2cLib.h>
#include <Wire.h>

//BeckAtoD class methods
BeckGyro::BeckGyro(INT16 sDummy) {
  SetupData();
  SetupI2C();
  return;
} //Constructor


INT16 BeckGyro::sReadTwoBytes(void) {
	return(Wire.read() << 8 | Wire.read());
}	//sReadTwoBytes


void BeckGyro::Read(void) {
  INT16      asGyroReading[sNumGyroSensors_][sNumAxis_];
   //boolean  bApplySmoothing= APPLY_SMOOTHING;

   if (millis() > ulNextGyroTime_) {
      Wire.beginTransmission(ucGyroAddress_);
      Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
      Wire.endTransmission(false);
      //Wire.requestFrom(ucGyroAddress_,14,true);  // request a total of 14 registers
      //bool  bTrue= true;
      String szLogString="BeckGyro::Read(): Call requestFrom()";
      LogToSerial(szLogString);
      Wire.requestFrom((uint8_t)ucGyroAddress_, (size_t)14, (bool)true);  // request a total of 14 registers

      // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
      // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
      // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
      // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
      // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
      // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
      // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

      asGyroReading[eAccel][eXAxis]= Wire.read() << 8 | Wire.read();
      asGyroReading[eAccel][eYAxis]= Wire.read() << 8 | Wire.read();
      asGyroReading[eAccel][eZAxis]= Wire.read() << 8 | Wire.read();

      szLogString="BeckGyro::Read(): asGyroReading[eAccel][eZAxis]=";
      LogToSerial(szLogString, asGyroReading[eAccel][eZAxis]);

      //asGyroReading[eTemperature][eXAxis]= Wire.read() << 8 | Wire.read();
      asGyroReading[eTemperature][eXAxis]= sReadTwoBytes();

      szLogString="BeckGyro::Read(): asGyroReading[eTemperature][eXAxis]=";
      LogToSerial(szLogString, asGyroReading[eTemperature][eXAxis]);


      asGyroReading[eRotation][eXAxis]=Wire.read() << 8 | Wire.read();
      asGyroReading[eRotation][eYAxis]=Wire.read() << 8 | Wire.read();
      asGyroReading[eRotation][eZAxis]=Wire.read() << 8 | Wire.read();

      //Initialize missing temperature fields.
      for (int sAxis= eYAxis; sAxis < sNumAxis_; sAxis++) {
         asGyroReading[eTemperature][sAxis]= 0;
      }  //for

      //Apply low-pass filter to data
      for (int sSensorType= eAccel; sSensorType <= sNumGyroSensors_; sSensorType++) {
          szLogString="BeckGyro::Read():sSensorType=";
          LogToSerial(szLogString, sSensorType);
         for (int sAxis= eXAxis; sAxis <= sNumAxis_; sAxis++) {
#if APPLY_SMOOTHING
            asGyro_[sSensorType][sAxis]= FILTER_FUNC(asGyroReading[sSensorType][sAxis],
                                                  pusSmoothingMemory[sSensorType][sAxis]);
#else
            szLogString="BeckGyro::Read():sAxis=";
            LogToSerial(szLogString, sAxis);

            asGyro_[sSensorType][sAxis]= asGyroReading[sSensorType][sAxis];
            szLogString="BeckGyro::Read(): asGyro_[][]=";
            LogToSerial(szLogString, asGyro_[sSensorType][sAxis]);

#endif
         }  //for sAxis
      }  //for sSensorType

      szLogString="BeckGyro::Read(): asGyroReading[eAccel][eZAxis]=";
      LogToSerial(szLogString, asGyroReading[eAccel][eZAxis]);

     szLogString="BeckGyro::Read(): asGyro_[eAccel][eZAxis]=";
      LogToSerial(szLogString, asGyro_[eAccel][eZAxis]);

      szLogString="BeckGyro::Read(): asGyro_[eTemperature][eXAxis]=";
      LogToSerial(szLogString, asGyro_[eTemperature][eXAxis]);


      //The following is for bringing up gyro
      szLogString="Read_Gyro(): AccelZ";
      INT16 sAccelZaxis= asGyro_[eAccel][eZAxis];
      LogToBoth(szLogString, sAccelZaxis);

      bGyroChanged_= true;
      ulNextGyroTime_= millis() + ulGyroReadTime_;
   }  //if (millis()>ulNextGyroTime)
   return;
} //dRead


void BeckGyro::SetupI2C(void) {
  BLog("BeckGyro::SetupI2C: Begin");
  Wire.beginTransmission(ucGyroAddress_);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  return;
} //SetupI2C


void BeckGyro::SetupData(void) {
  //Initialize the data array.
  for (int sDataType= eAccel; sDataType < sNumGyroSensors_; sDataType++) {
     for (int sAxis= eXAxis; sAxis < sNumAxis_; sAxis++) {
        asGyro_[sDataType][sAxis]= 37;
     }  //for sDataType
  }  //for sAxis
  return;
} //SetupData
//Last line.

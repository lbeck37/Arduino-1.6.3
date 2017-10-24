#include <BeckMPU6050_Gyro.h>

void MPU6050_PrintName(){
  int wError;
  uint8_t c;

  wError = MPU6050_read (MPU6050_WHO_AM_I, &c, 1);
  Serial.print(F("WHO_AM_I : "));
  Serial.print(c,HEX);
  Serial.print(F(", wError = "));
  Serial.println(wError,DEC);

  // According to the datasheet, the 'sleep' bit
  // should read a '1'.
  // That bit has to be cleared, since the sensor
  // is in sleep mode at power-up.
  wError = MPU6050_read (MPU6050_PWR_MGMT_1, &c, 1);
  Serial.print(F("PWR_MGMT_1 : "));
  Serial.print(c,HEX);
  Serial.print(F(", wError = "));
  Serial.println(wError,DEC);

  // Clear the 'sleep' bit to start the sensor.
  //MPU6050_write_reg (MPU6050_PWR_MGMT_1, 0);
	return;
}	//MPU6050_PrintName


// --------------------------------------------------------
// MPU6050_read
//
// This is a common function to read multiple bytes
// from an I2C device.
//
// It uses the boolean parameter for Wire.endTransMission()
// to be able to hold or release the I2C-bus.
// This is implemented in Arduino 1.0.1.
//
// Only this function is used to read.
// There is no function for a single byte.
//
int MPU6050_read(int start, uint8_t *buffer, int size)
{
  int i, n;

  //Wire.beginTransmission(MPU6050_I2C_ADDRESS);
  Wire.beginTransmission(wMPU6050);
  n = Wire.write(start);
  if (n != 1)
    return (-10);

  n = Wire.endTransmission(false);    // hold the I2C-bus
  if (n != 0)
    return (n);

  // Third parameter is true: relase I2C-bus after data is read.
  //Wire.requestFrom(MPU6050_I2C_ADDRESS, size, true);
  Wire.requestFrom(wMPU6050, size, true);
  i = 0;
  while(Wire.available() && i<size)
  {
    buffer[i++]=Wire.read();
  }
  if ( i != size)
    return (-11);

  return (0);  // return : no error
}	//MPU6050_read


// --------------------------------------------------------
// MPU6050_write
//
// This is a common function to write multiple bytes to an I2C device.
//
// If only a single register is written,
// use the function MPU_6050_write_reg().
//
// Parameters:
//   start : Start address, use a define for the register
//   pData : A pointer to the data to write.
//   size  : The number of bytes to write.
//
// If only a single register is written, a pointer
// to the data has to be used, and the size is
// a single byte:
//   int data = 0;        // the data to write
//   MPU6050_write (MPU6050_PWR_MGMT_1, &c, 1);
//
int MPU6050_write(int start, const uint8_t *pData, int size)
{
  int n, wError;

  //Wire.beginTransmission(MPU6050_I2C_ADDRESS);
  Wire.beginTransmission(wMPU6050);
  n = Wire.write(start);        // write the start address
  if (n != 1)
    return (-20);

  n = Wire.write(pData, size);  // write data bytes
  if (n != size)
    return (-21);

  wError = Wire.endTransmission(true); // release the I2C-bus
  if (wError != 0)
    return (wError);

  return (0);         // return : no error
}	//MPU6050_write


// --------------------------------------------------------
// MPU6050_write_reg
//
// An extra function to write a single register.
// It is just a wrapper around the MPU_6050_write()
// function, and it is only a convenient function
// to make it easier to write a single register.
//
int MPU6050_write_reg(int reg, uint8_t data)
{
  int wError;

  wError = MPU6050_write(reg, &data, 1);
  return (wError);
}	//MPU6050_write_reg
//Last line.

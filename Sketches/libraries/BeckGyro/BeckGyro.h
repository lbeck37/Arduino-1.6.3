//BeckGyro.h
#ifndef BECKGYRO_H
#define BECKGYRO_H
#include <BeckLib.h>

typedef enum {
  eXAxis= 1,
  eYAxis,
  eZAxis,
  eLastAxis
} Axis_t;

typedef enum {
  eAccel= 1,
  eRotation,
  eTemperature,
  eLastSensor
} GyroSensor;

class BeckGyro {
protected:
  INT16     		sNumAxis_           = (eLastAxis   - 1);
  INT16    		 	sNumGyroSensors_    = (eLastSensor - 1);
  UINT8					ucGyroAddress_			= 0x68;
  const UINT32  ulGyroReadTime_    	= 500;   //Gyro reads spaced by this.

  uint32_t        ulNextGyroTime_   = 0;  //msec when the gyro will be read
  boolean         bGyroChanged_     = false;
public:
  INT16     asGyro_[eLastSensor - 1][eLastAxis - 1];

            BeckGyro    (INT16 sDummy);
  void      Read        (void);

private:
  void      SetupI2C   	(void);
  void      SetupData   (void);
};  //BeckGyro
#endif  //BECKGYRO_H

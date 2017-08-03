String	acSketchName	= "Beck_MPU6050_DMP_Pitch_NoINT.ino, ";
String	acFileDate		= "Aug 3, 2017, Ace-M";

#include <BeckLogLib.h>
#include <Streaming.h>
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"

MPU6050 		mpu;
uint16_t 		packetSize;
uint16_t 		fifoCount;
uint8_t 		fifoBuffer[64];
Quaternion 	q;
VectorFloat	gravity;
float 			ypr[3];

const unsigned long	ulGyroSpacingMilli	= 500;
unsigned long				ulNextGyroMilli			= 0;

void setup() {
  Serial.begin(115200);
	Serial << endl << "setup(): Begin " << acSketchName << acFileDate << endl;
	Wire.begin();
	TWBR = 24;
	mpu.initialize();
	mpu.dmpInitialize();
	mpu.setXAccelOffset(-1343);
	mpu.setYAccelOffset(-1155);
	mpu.setZAccelOffset(1033);
	mpu.setXGyroOffset(19);
	mpu.setYGyroOffset(-27);
	mpu.setZGyroOffset(16);
	mpu.setDMPEnabled(true);
	packetSize= mpu.dmpGetFIFOPacketSize();
	fifoCount = mpu.getFIFOCount();
}	//setup


void loop() {
	//Serial << "loop(): Call GyroLoop()" << endl;
	BLog("loop(): Call GyroLoop()");
	GyroLoop();
	BLog("loop(): Back from GyroLoop()");
	return;
}	//loop


void GyroLoop(){
	if (true || (millis() > ulNextGyroMilli)) {
		ulNextGyroMilli= millis() + ulNextGyroMilli;
		while (fifoCount < packetSize) {
			//insert here your code
			fifoCount= mpu.getFIFOCount();
		}	//while

		if (fifoCount == 1024) {
			mpu.resetFIFO();
			Serial.println(F("FIFO overflow!"));
		}	//if(fifoCount==1024)
		else {
			BLog("GyroLoop(): fifoCount= ", fifoCount);
			if (fifoCount % packetSize != 0) {
				mpu.resetFIFO();
			}	//if(fifoCount%packetSize!=0)
			else {
				while (fifoCount >= packetSize) {
					mpu.getFIFOBytes(fifoBuffer, packetSize);
					fifoCount -= packetSize;
				}	//while
				BLog("GyroLoop(): Call dmpGetQuaternion()");
				mpu.dmpGetQuaternion(&q, fifoBuffer);
				mpu.dmpGetGravity(&gravity, &q);
				mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

				BLog("GyroLoop(): Pitch= ", (ypr[1] * 180 / PI));
/*
				Serial.print("ypr\t");
				Serial.print(ypr[0] * 180 / PI);
				Serial.print("\t");
				Serial.print(ypr[1] * 180 / PI);
				Serial.print("\t");
				Serial.print(ypr[2] * 180 / PI);
				Serial.println();
*/
			}	//if(fifoCount%packetSize!=0)else
		}	//if(fifoCount==1024)else

	}	//if (milli() > ulNextGyroMilli)
	return;
}	//GyroLoop
//Last line.

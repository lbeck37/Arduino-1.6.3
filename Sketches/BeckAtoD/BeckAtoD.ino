static const String SketchName  = "BeckAtoD.ino";
static const String FileDate    = "Dec 17, 2017, Lenny-f";

#include <Wire.h>
//#include <Adafruit_ADS1015.h>
#include <BeckMiniLib.h>
#include <BeckAtoD.h>

static const INT16       sI2C_SDA				= 26;
static const INT16       sI2C_SCL				= 27;

BeckI2C     I2C(sI2C_SDA, sI2C_SCL);
BeckAtoD    AtoD(&I2C, eADS1115);

void setup(void){
  Serial.begin(115200);
  Serial << endl << "setup(): Begin " << SketchName << ", " << FileDate << endl;
  I2C.Begin();
} //setup


void loop(void){
	static double	dRealVolts;

  for (int sChan= 0; sChan <= 2; sChan++) {
  	if(sChan == sAmpChannel){
			dRealVolts= AtoD.dReadRealVolts(sChan);
			Serial << "loop(): Channel= " << sChan << ", dRealVolts=  " << dRealVolts << endl << endl;
  	}	//
  	else {
			dRealVolts= AtoD.dReadRealVolts(sChan);
			Serial << "loop(): Channel= " << sChan << ", dRealVolts=  " << dRealVolts << endl << endl;
  	}	//
  } //for(int sChannel=0;
  delay(5000);
} //loop
//Last line.

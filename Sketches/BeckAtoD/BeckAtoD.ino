static const String SketchName  = "BeckAtoD.ino";
static const String FileDate    = "Dec 17, 2017, Lenny-b";

#include <Wire.h>
//#include <Adafruit_ADS1015.h>
#include <BeckMiniLib.h>
#include <BeckAtoD.h>

static const INT16       sI2C_SDA              = 26;
static const INT16       sI2C_SCL              = 27;

/*
Adafruit_ADS1115  ads(0x48);
float Voltage = 0.0;
*/
BeckI2C     I2C(sI2C_SDA, sI2C_SCL);
BeckAtoD    AtoD(&I2C, eADS1115);

void setup(void){
/*
  //Serial.begin(9600);
  //Serial.println("\nBeckAtoD.ino 12/15/17A Lenny");
  //Serial << "setup(): Call Wire.begin(sI2C_SDA, sI2C_SCL) " << sI2C_SDA << ", " << sI2C_SCL << endl;
  //Wire.begin(sI2C_SDA, sI2C_SCL);
  //ads.begin();
*/
  Serial.begin(115200);
  Serial << endl << "setup(): Begin " << SketchName << ", " << FileDate << endl;
  I2C.Begin();
} //setup


void loop(void){
  //int16_t adc0= 0xFF;  // we read from the ADC, we have a sixteen bit integer as a result

  for (int sChannel= 0; sChannel <= 0; sChannel++) {
/*
     adc0 = ads.readADC_SingleEnded(sChannel);
     Voltage = (adc0 * 0.1875)/1000;

     Serial.print("sChannel= ");
     Serial.print(sChannel);
     Serial.print(",\tAin= ");
     Serial.print(adc0);
     Serial.print(",\tVoltage: ");
     Serial.println(Voltage, 7);
     Serial.println();
*/
  AtoD.dRead(sChannel, GAIN_TWO);
  } //for(int sChannel=0;
  delay(5000);
} //loop
//Last line.

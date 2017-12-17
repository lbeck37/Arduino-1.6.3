static const String SketchName  = "BeckAtoD.ino";
static const String FileDate    = "Dec 16, 2017, Lenny-b";

//Beck 1/27/17
#include <Wire.h>
//#include <Adafruit_ADS1015.h>
#include <BeckAtoD.h>
#include <Streaming.h>

static const INT16       sI2C_SDA              = 26;
static const INT16       sI2C_SCL              = 27;

//Adafruit_ADS1115 	ads(0x48);
BeckI2C			I2C(sI2C_SDA, sI2C_SCL);
BeckAtoD		AtoD(&I2C, eADS1115);

float Voltage = 0.0;

void setup(void)
{
  //Serial.begin(9600);
  Serial.begin(115200);
  //Serial.println("\nBeckAtoD.ino 12/15/17A Lenny");
  Serial << endl << "setup(): Begin " << SketchName << ", " << FileDate << endl;
  I2C.Begin();
  //Serial << "setup(): Call Wire.begin(sI2C_SDA, sI2C_SCL) " << sI2C_SDA << ", " << sI2C_SCL << endl;
  //Wire.begin(sI2C_SDA, sI2C_SCL);
  //ads.begin();
} //setup


void loop(void)
{
  int16_t adc0= 0xFF;  // we read from the ADC, we have a sixteen bit integer as a result

  for (int sChannel= 0; sChannel < 4; sChannel++) {
     //adc0 = ads.readADC_SingleEnded(sChannel);
     Voltage = (adc0 * 0.1875)/1000;

     Serial.print("sChannel= ");
     Serial.print(sChannel);
     Serial.print(",\tAin= ");
     Serial.print(adc0);
     Serial.print(",\tVoltage: ");
     Serial.println(Voltage, 7);
     Serial.println();
  } //for(int sChannel=0;

  delay(5000);
} //loop
//Last line.

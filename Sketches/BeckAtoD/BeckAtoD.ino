//Beck 11/22/16
//#include <Wire.h>
#include <Adafruit_ADS1015.h>

Adafruit_ADS1115 ads(0x48);
float Voltage = 0.0;


void setup(void)
{
  //Serial.begin(9600);
  Serial.begin(115200);
  Serial.println("\nBeckAtoD.ino 11/22/16 Lenny");
  ads.begin();
} //setup


void loop(void)
{
  int16_t adc0;  // we read from the ADC, we have a sixteen bit integer as a result

  for (int sChannel= 0; sChannel < 4; sChannel++) {
     adc0 = ads.readADC_SingleEnded(sChannel);
     Voltage = (adc0 * 0.1875)/1000;

     Serial.print("sChannel= ");
     Serial.print(sChannel);
     Serial.print(",\tAin= ");
     Serial.print(adc0);
     Serial.print(",\tVoltage: ");
     Serial.println(Voltage, 7);
     Serial.println();
  } //for(int sChannel=0;

  delay(1000);
} //loop
//Last line.

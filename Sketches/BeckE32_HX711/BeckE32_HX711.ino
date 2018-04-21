#include "HX711.h"
#include <soc/rtc.h>
#include <Streaming.h>

// HX711.DOUT	- pin #A1
// HX711.PD_SCK	- pin #A0

const byte 		cHX711_DOUT		= 25;				//IO pin number was 34 and 35
const byte		cHX711_SCK		= 26;
const byte		cHX711_Gain		= 128;			//Default gain is 128

//HX711 scale(A1, A0);		// parameter "gain" is ommited; the default value 128 is used by the library

//HX711			oPedalForce(cHX711_DOUT, cHX711_SCK, cHX711_Gain);
HX711			oPedalForce;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("BeckE32_HX711.ino, Apr 21, 2018-r");

  Serial.println("setup(): Call rtc_clk_cpu_freq_set(RTC_CPU_FREQ_80M)");
  rtc_clk_cpu_freq_set(RTC_CPU_FREQ_80M);

  oPedalForce.begin(cHX711_DOUT, cHX711_SCK);		//Use default gain
  TakeReadings();

/*
  //Set the scale.
  Serial.print("oPedalForce.set_scale(2280.f): \t\t");
  oPedalForce.set_scale(2280.f);                      // this value is obtained by calibrating the scale with known weights; see the README for details

  Serial.println("Call oPedalForce.tar()");
  oPedalForce.tare();				        // reset the scale to 0

  Serial.println("After setting up the scale:");
  TakeReadings();
*/

  //Serial.println("Readings:");
  return;
}	//setup


void loop() {
	unsigned long		ulValue;

/*
	ulValue= oPedalForce.get_units();
  Serial << "loop(): oPedalForce.get_units() returned " << ulValue << endl;
*/
	Serial << endl;
  ulValue= oPedalForce.read();
  Serial << "loop(): oPedalForce.read() returned " << ulValue << endl;

/*
  Serial.print("\t| average:\t");
  Serial.println(oPedalForce.get_units(10), 1);
*/

  oPedalForce.power_down();			        // put the ADC in sleep mode
  delay(5000);
  oPedalForce.power_up();
  return;
}	//loop


void TakeReadings(){
	unsigned long		ulValue;
  Serial.println("TakeReadings(): Call oPedalForce.read()");
  //Serial.print("read: \t\t");
  //Serial.println(oPedalForce.read());			// print a raw reading from the ADC
  ulValue= oPedalForce.read();
  Serial << "TakeReadings(): oPedalForce.read() returned " << ulValue << endl;

/*
  ulValue= oPedalForce.read_average(20);
  Serial << "TakeReadings(): oPedalForce.read_average(20) returned " << ulValue << endl;
*/

  ulValue= oPedalForce.get_value(1);
  Serial << "TakeReadings(): oPedalForce.get_value(1) returned " << ulValue << endl;

  // Print the average of n readings from the ADC minus tare weight (not set) divided
	// by the SCALE parameter (not set yet)
  ulValue= oPedalForce.get_units(1);
  Serial << "TakeReadings(): oPedalForce.get_units(1) returned " << ulValue << endl;

	return;
}	//TakeReadings
//Last line

//BeckDisplay.cpp
#include <BeckDisplay.h>
#include <Adafruit_GFX.h>
#include <BeckTanks.h>

// Adafruit_SSD1306 		oDisplay(-1);		//Looks like -1 is default


BeckDisplay::BeckDisplay(void) {
  String szLogString="BeckDisplay Constructor: Begin";
  LogToSerial(szLogString);

  szLogString="BeckDisplay Constructor: Create Adafruit_SSD1306 object";
  LogToSerial(szLogString);
  pDisplay_= new Adafruit_SSD1306(-1);

  pDisplay_->begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // Clear the buffer.
  pDisplay_->clearDisplay();
  return;
} //Constructor


void BeckDisplay::UpdateTanks(double adVolts) {
	pDisplay_->setTextSize(2);
	pDisplay_->setTextColor(WHITE);
	pDisplay_->setCursor(0,0);

  for (int sTank= eGrey1; sTank < sNumTanks_; sTank++) {
  	pDisplay_->println(adVolts[sTank]);
  }  //for

  return;
}
//Last line.

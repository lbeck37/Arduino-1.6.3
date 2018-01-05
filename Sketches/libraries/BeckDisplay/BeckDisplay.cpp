//BeckDisplay.cpp
#include <BeckDisplay.h>
#include <Adafruit_GFX.h>
#include <BeckTanks.h>

// Adafruit_SSD1306 		oDisplay(-1);		//Looks like -1 is default


BeckDisplay::BeckDisplay(void) {
  String szLogString="BeckDisplay Constructor: Begin";
  LogToSerial(szLogString);

/*
  szLogString="BeckDisplay Constructor: Create Adafruit_SSD1306 object";
  LogToSerial(szLogString);
  pDisplay_= new Adafruit_SSD1306(-1);

  szLogString="BeckDisplay Constructor: Call begin(SSD1306_SWITCHCAPVCC, 0x3C)";
  LogToSerial(szLogString);
  pDisplay_->begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)

  // Clear the buffer.
  szLogString="BeckDisplay Constructor: Call clearDisplay()";
  LogToSerial(szLogString);
  pDisplay_->clearDisplay();
*/
  return;
} //Constructor


void BeckDisplay::UpdateTanks(double adVolts[]) {
  //String szLogString="BeckDisplay::UpdateTanks(): Write values to screen";
  //LogToSerial(szLogString);
	pDisplay_->setTextSize(2);
	pDisplay_->setTextColor(WHITE);
	pDisplay_->setCursor(0,0);

  for (int sTank= eGrey1; sTank < eLastTank; sTank++) {
    	pDisplay_->println(adVolts[sTank]);
//    	pDisplay_->println("3.737");
  }  //for
  pDisplay_->display();
  return;
}



void BeckDisplay::Setup(void) {
	String szLogString="BeckDisplay::Setup(): Create Adafruit_SSD1306 object";
	LogToSerial(szLogString);
	pDisplay_= new Adafruit_SSD1306(-1);

	szLogString="BeckDisplay::Setup(): Call begin(SSD1306_SWITCHCAPVCC, 0x3C)";
	LogToSerial(szLogString);
	pDisplay_->begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)

	// Clear the buffer.
	szLogString="BeckDisplay::Setup(): Call clearDisplay()";
	LogToSerial(szLogString);
	pDisplay_->clearDisplay();
	return;
}	//Setup


//Last line.

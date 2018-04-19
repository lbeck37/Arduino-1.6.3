const String SketchName  = "BeckE32_PedalForce.ino";
const String FileDate    = "Apr 18, 2018, Lenny-w";
//Beck 2/13/17, from Adafruit example ssd1306_128x64_i2c.ino
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Streaming.h>
#include <iostream>   // std::cout
#include <string>     // std::string, std::to_string

//#define OLED_RESET 4
//Adafruit_SSD1306 oDisplay(OLED_RESET);

Adafruit_SSD1306 		oDisplay(-1);		//Looks like -1 is default

#if (SSD1306_LCDHEIGHT != 64)
	#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void setup()   {
  Serial.begin(115200);
  Serial << endl << "setup(): Begin " << SketchName << ", " << FileDate << endl;
  //Serial << "setup(): Call Wire.begin(21, 22)" << endl;
  Wire.begin(21, 22);		//Beck 1-3-18
  ScanForDevices();

  //By default, we'll generate the high voltage from the 3.3v line internally! (neat!) Beck????
  //oDisplay.begin(SSD1306_SWITCHCAPVCC, 0x3D);  // initialize with the I2C addr 0x3D (for the 128x64)
  oDisplay.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)

  // Clear the buffer.
  oDisplay.clearDisplay();
  return;
}	//setup


void loop() {
	TestDashboard();
 	return;
}	//loop


void TestDashboard(void) {
  //float val;
  char szNumber[10];
	static double dDigit= 1.0;
	double dNumber;
	oDisplay.setTextSize(2);
	oDisplay.setTextColor(WHITE);
	oDisplay.setCursor(0,0);
	for (int sLine= 1; sLine <= 4; sLine++){
		dNumber= dDigit + (2. * dDigit / 10.0) + (3. * dDigit / 100.0) + (4. * dDigit / 1000.0);
		//dNumber= 1.234;
		dtostrf(dNumber, 8, 4, szNumber);
		Serial << "TestDashboard(): dNumber= " << dNumber << endl;
		oDisplay.println(szNumber);
		dDigit += 1.0;
	}
	oDisplay.display();
	Serial << endl;
/*
	oDisplay.println("+3.101");
	oDisplay.println("+3.202");
	oDisplay.println("+3.303");
	oDisplay.println("+3.404");
*/
	delay(5000);
	//Serial.println("Dashboard(): Call clearDisplay()");
	oDisplay.clearDisplay();
	oDisplay.display();
	//delay(1000);
	return;
}	//TestDashboard


void ScanForDevices(void){
	byte ucError, ucAddress;
  int nDevices;
  Serial.println("ScanForDevices(): Begin");

  Serial.println("Scanning...");
  nDevices = 0;
  for(ucAddress = 1; ucAddress < 127; ucAddress++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(ucAddress);
    ucError = Wire.endTransmission();

    if (ucError == 0)
    {
    	//SetDevicePresent(ucAddress);
      Serial.print("I2C device found at address 0x");
      if (ucAddress<16) {
        Serial.print("0");
      }
      Serial.print(ucAddress,HEX);
      Serial.println("  !");
      nDevices++;
    }
    else if (ucError==4)
    {
      Serial.print("Unknown error at address 0x");
      if (ucAddress<16) {
        Serial.print("0");
      }
      Serial.println(ucAddress,HEX);
    }
  }
 if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done");
  return;
}	//ScanForDevices
//Lastline

static const String SketchName  = "BeckE32Rover_Powershift.ino";
static const String FileDate    = "Oct 18, 2017, Lenny-c";
/***************************************************
  This is our GFX example for the Adafruit ILI9341 Breakout and Shield
  ----> http://www.adafruit.com/products/1651

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
 ****************************************************/

#include <Arduino.h>
#include <BeckLogLib.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <WROVER_KIT_LCD.h>
#include <esp_wp1.h>
#include <esp_wp2.h>
#include <Wire.h>
#include <EasyButton.h>
#include <Servo.h>

#define min(X, Y)     (((X) < (Y)) ? (X) : (Y))
#define PAUSE_DELAY   delay(2000)

WROVER_KIT_LCD    RoverLCD;

void setup() {
  Serial.begin(115200);
  Serial.println("setup(): Begin");

  RoverLCD.begin();
  RoverLCD.setRotation(1);

  uint32_t uwRoverID = RoverLCD.readId();
  if(uwRoverID){
      Serial.println("======= WROVER ST7789V Display Test ========");
  } else {
      Serial.println("======= WROVER ILI9341 Display Test ========");
  }
  Serial.println("============================================");
  Serial.printf("Display ID:      0x%06X\n", uwRoverID);

  Serial.println("setup(): End");
  return;
} //setup


void loop(void) {
  return;
} //loop


//Last line

static const String SketchName  = "Powershift_E32Rover.ino";
static const String FileDate    = "Nov 29, 2017, Lenny-c";

#include <Arduino.h>
#include <BeckLogLib.h>
#include <BeckMPU6050_Gyro.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <WROVER_KIT_LCD.h>
#include <esp_wp1.h>
#include <esp_wp2.h>
#include <Wire.h>
#include <EasyButton.h>
#include <ESP32_Servo.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSansOblique18pt7b.h>
//#include <Fonts/FreeMonoBoldOblique24pt7b.h>
//#include <Fonts/FreeSansBoldOblique12pt7b.h>
//#include <avr/wdt.h>

#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>

//using namespace std;

#define min(X, Y)     (((X) < (Y)) ? (X) : (Y))
#define PAUSE_DELAY   delay(2000)

#define RADIX_10			10

#define DO_BUTTONS		true
#define DO_SERVO			true
#define DO_GYRO				true

//using namespace std;

//const int MPU6050= 0x68;  // I2C address of the MPU-6050

int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

//Here come the const's
//static const int  asDefaultGearLocation[]= {0, 150, 119, 92, 74, 64, 48, 17};
//static const int asDefaultGearLocation[]= {0, 122, 101, 74, 68, 56, 35, 20};   //9-spd cogs 3-9
//static const int asDefaultGearLocation[]= {0, 152, 137, 122, 101, 74, 68, 56, 35, 27, 20};  //10-speed estimates
//static const int asDefaultGearLocation[]= {0, 168, 135, 113, 101, 79, 66, 58, 41, 29, 14};  //10-speed
static const int asDefaultGearLocation[]= {0, 135, 113, 101, 81, 68, 58, 41, 29, 14}; //10-speed. cogs 2-10
static const int       sServoMin             = 0;
static const int       sServoMax             = 180;
static const int       sServoMsecWait        = 15;
static const int       sNumGears             = 9;
static const boolean   bServoOn              = true;
static const int       sHoldDeltaPos         = 2; //Servo move size when button held.
static const int       sTrimDeltaPos         = 1; //Servo move size when button clicked.

static const int       sNormalMode           = 0;
static const int       sCalibMode            = 1;

static const int       sUp                   = 0;
static const int       sDown                 = 1;
//static const int       sSelect               = 2;
static const int       sFirstButton          = sUp;
static const int       sLastButton           = sDown;

static const boolean   bButtonPullUp         = true;

//Digital Pins
#if 1   //ESP32
static const int       sServoPin             = 12;
static const int       sUpButton             =  2;
static const int       sDownButton           =  4;
static const int       sI2C_SDA              = 26;
static const int       sI2C_SCL              = 27;
#endif

#if 0   //Pro Mini
//static const int       sSelectButton         = A3;
static const int       sDownButton           = A2;
static const int       sUpButton             = A1;
//static const int       sBacklightPin         =  6;
static const int       sServoPin             =  7;
static const byte      cSPICmdDataPin        =  9;
static const byte      cSPIChipSelectPin     = 10;
#endif

#if 0   //NodeMCU
//static const int       sServoPin             =  9;    //Pin 9, SD2
//static const int       sServoPin             =  4;    //Pin 4, D2, I2C SDA
//static const int       sServoPin             =  1;    //Pin 1, D10, TXD0
static const int       sServoPin             =  3;    //Pin 3, D9, RXD0
static const int       sDownButton           =  2;    //Pin 2, D4
static const int       sUpButton             =  0;    //Pin 0, D3
static const byte      cSPICmdDataPin        = 16;    //Pin 16, D0
//static const byte      cSPICmdDataPin        =  5;    //Pin 5, D1, ILI9341 display article
static const byte      cSPIChipSelectPin     = 15;    //Pin 15, D8
//static const byte      cSPIChipSelectPin     =  4;    //Pin 4, D2, ILI9341 display article
static const byte      cSPI_MOSI_Pin         = 13;    //Pin 13, D7
static const byte      cSPI_CLK_Pin          = 14;    //Pin 14, D5
#endif

//Gyro defines
static const int       sXAxis             = 0;
static const int       sYAxis             = 1;
static const int       sZAxis             = 2;
static const int       sNumAxis           = 3;

static const int       sAccel             = 0;
static const int       sRotation          = 1;
static const int       sTemperature       = 2;
static const int       sNumGyroTypes      = 3;

//Constants used locally for state in sCheckButtons
static const int       sButtonOpen        = 0;
static const int       sButtonPressed     = 1;
static const int       sButtonClosed      = 2;
static const int       sButtonReleased    = 3;
static const int       sButtonHeld        = 4;

static const int       sDefaultFont       =  0;
static const int       sMaxButtonPresses  = 10;

static const unsigned long    ulModeSwitchTime  = 1000;  //Minimum msecs between mode changes
static const unsigned long    ulModeWaitTime    = 2000;  //Minimum msecs before mode is on
static const unsigned long    ulGyroReadTime    = 5000;   //Gyro reads spaced by this.

//Constants for DOGS102 display.
static const int       sDisplayWidth        = 102;
static const int       sDisplayHeight       = 64;

static const int       sDisplayNormal       = 0xC0;
static const int       sDisplayFlipped      = 0xC8;

static const int       sBrightness100       = 255;
static const int       sBrightness75        = 192;
static const int       sBrightness50        = 128;
static const int       sBrightness25        =  64;
static const int       sBrightness0         =   0;
static const int       sDefaultBrightness   = sBrightness100;

static const int       sFontNormal         	=   1;     //6x8
static const int       sFontBig            	=   2;     //8x16
static const int       sFontBigNum         	=   3;     //16x32nums
static const int       sFontSquare         	=   4;     //8x8

static const int       sFontSize1          	=   1;
static const int       sFontSize2          	=   2;
static const int       sFontSize3          	=   3;
static const int       sFontSize4          	=   4;
static const int       sFontSize5          	=   5;

static const byte       cBogusResetPin      = 4;
static const byte       cHW_SPI             = 0;      //This is what their demo used.
//End of the const's

static int asGearLocation[sNumGears + 1];
static int sCurrentGear                   = 2;

static int asGyro[sNumGyroTypes][sNumAxis];

static int sCurrentMode                   = sNormalMode;
static int sServoPosLast                  = 0;

WROVER_KIT_LCD    RoverLCD;

#if DO_SERVO
//Create servo object to control the servo
Servo 						myservo;
#endif	//DO_SERVO

#if DO_BUTTONS
//Create EasyButton objects to handle button presses.
EasyButton UpButton     (sUpButton,     NULL, CALL_NONE, bButtonPullUp);
EasyButton DownButton   (sDownButton,   NULL, CALL_NONE, bButtonPullUp);
//EasyButton SelectButton (sSelectButton, NULL, CALL_NONE, bButtonPullUp);
#endif	//DO_BUTTONS

//Number of unhandled presses, up to sMaxButtonPresses
static int              sButtonCount[]       = { 0, 0, 0};
//static int              sButtonCountLast[]   = { 0, 0, 0};
static boolean          abButtonBeingHeld[]  = { false, false, false};
static unsigned long    ulNextModeTime       = 0;  //msec when a mode switch can take place
static unsigned long    ulModeReadyTime      = 0;  //msec when button presses can be handled
static unsigned long    ulNextGyroTime       = 0;  //msec when the gyro will be read

//State of display items being changed and needing refresh.
static boolean     bButtonsChanged          = false;
static boolean     bGearChanged             = false;
static boolean     bServoChanged            = false;
static boolean     bModeChanged             = false;
static boolean     bGyroChanged             = false;

static int         sLineCount= 0;     //Used in outputs to Serial Monitor for clarity.

static char       szTempBuffer[100];   //DOGS102 line is 17 chars with 6x8 normal font.
static char       sz100CharString[101];
//static char       szFloatBuffer[15];

//wMPU6050 acceleration and pitch things
const double      dGConvert         = 16384.0;  //wMPU6050 16-bit +/- 2G Full scale
const double      dRadsToDeg        = 180.0/PI;
double            adGvalueXYZ[3];
double            dRollDeg_;
double            dPitchDeg_;
double            dPitchPercent_		= 16.0;

double            dVolts_		= 37.7;
double            dAmps_		= 10.0;
double            dWatts_		= dVolts_ * dAmps_;

void(* ResetESP32)(void)= 0;				//Hopefully system crashes and reset when this is called.

// The Arduino setup() method runs once, when the sketch starts
void setup()   {
  Serial.begin(115200);
  Serial << endl << "setup(): Begin " << SketchName << ", " << FileDate << endl;

  Serial << "setup(): Call SetupPins()" << endl;
  //TestButtonPins();
  //Serial << "setup(): Call sSetupGyro()" << endl;
  Serial << "setup(): Call sFillGearLocations()" << endl;
  sFillGearLocations();
#if DO_GYRO
  sSetupGyro();
#endif
#if DO_SERVO
  Serial << "setup(): Call sServoInit()" << endl;
  sServoInit();
#endif
  Serial << "setup(): Call sShowStartScreen()" << endl;
  ShowStartScreen();

  //Dither the servo once so it's position shows on the LCD.
  //sServoDither(1, 1); // +/- 1 degree, once
  bButtonsChanged= true;	//Make the display show up during debugging.
  return;
}  //setup


// The Arduino loop() method gets called over and over.
void loop() {
  sCheckButtons();
#if DO_GYRO
  sLoopI2C();
#endif
  DisplayUpdate();
  sHandleButtons();
  CheckKeyboard();
  return;
}  //loop()


int sServoInit() {
  Serial << "sServoInit(): Check bServoOn" << endl;
  if (bServoOn) {
    Serial << "sServoInit(): Call myservo.attach() for pin " << sServoPin << endl;
    myservo.attach(sServoPin);
  }  //if(bServoOn)
  Serial << "sServoInit(): Call sServoMove()" << endl;
  sServoMove(asGearLocation[sCurrentGear]);
  return 1;
} //sServoInit


int sServoMove(int sServoPos) {
  if (sServoPos != sServoPosLast) {
      sServoPos= constrain(sServoPos, sServoMin, sServoMax);
      //Serial << "sServoMove(): Move to " << sServoPos << endl;
      sServoPosLast= sServoPos;
      sServoSetPosition(sServoPos);
      //Serial << "sServoMove(): sServoSetPosition() returned" << endl;
      bServoChanged= true;
  }  //if(sServoPos...
  return 1;
} //sServoMove


//sServoDither() moves the servo back and forth and back to where it started.
int sServoDither(int sDitherSize, int sNumTimes){
   int sStartPosition = sServoPosLast;
   int sFirstPosition = sStartPosition + sDitherSize;
   int sSecondPosition= sStartPosition - sDitherSize;
   int sCycle;

   for (sCycle= 0; sCycle < sNumTimes; sCycle++){
      sServoMove(sFirstPosition);
      sServoMove(sSecondPosition);
   }  //for(sCycle...
   sServoMove(sStartPosition);
   bServoChanged= true;
   return 1;
}  //sServoDither


int sServoSetPosition(int sServoPos) {
   //Note that values of sServoPos less than 200 are considered degrees and
   //values greater are uSec pulse width by myservo.write().
   if (bServoOn) {
  	  //Serial << "sServoSetPosition(): Call myservo.write() " << sServoPos << endl;
      myservo.write(sServoPos);
   }
   else {
     Serial << "sServoSetPosition():bServoOn is false, skip move to " << sServoPos << endl;
   }
   delay(sServoMsecWait);
   return 1;
}  //sServoSetPosition


void CheckKeyboard() {
  if (Serial.available()) {
    Serial <<" CheckKeyboard(): Character available, call Serial.read()" << endl;
    char cChar= Serial.read();
    int sChar= cChar;
    if (sChar != 13) {
      Serial <<" CheckKeyboard(): Character read= "<< cChar <<", "<< sChar << endl;
    }
    else {
      Serial <<" CheckKeyboard(): Character read= CR" << endl;
    }
    switch (cChar) {
      case 'r':
      case 'R':
      	ResetESP32();
        break;
      case 'u':
      case 'U':
        sButtonCount[sUp]++;
        break;
      case 'd':
      case 'D':
        sButtonCount[sDown]++;
        break;
      default:
        break;
    } //switch
  } //if(Serial.available()
  return;
}  //CheckKeyboard


int sDisplayBegin() {
  Serial << "sDisplayBegin(): Call RoverLCD.begin()" << endl;
  RoverLCD.begin();
  RoverLCD.setRotation(1);
  return 1;
}  //sDisplayBegin


int ShowStartScreen(void) {
  Serial << "ShowStartScreen(): Call sDisplayBegin()" << endl;
  sDisplayBegin();
  Serial << "ShowStartScreen(): Call sShowSplash()" << endl;
  sShowSplash();
  delay(3000);
  DisplayClear();
  return 1;
}  //ShowStartScreen


int sShowSplash(void) {
   DisplayClear();
/*
   //2 lines of big font takes lines 0-3
   RoverLCD.setTextColor(WROVER_YELLOW);
   sDisplayText(0, 0, sFontSize3, (char *)"PowerShift");
   RoverLCD.setTextColor(WROVER_GREEN);
   sDisplayText(2, 0, sFontSize3, (char *)"  by ShiftE");

   //2 lines in normal font
   RoverLCD.setTextColor(WROVER_RED);
   sDisplayText(5, 0, sFontSize2, (char *)"\nAlways ride safe!");
   //sDisplayText(7, 0, sFontNormal, (char *)"**Larry & Candy**");
   sDisplayText(7, 0, sFontSize2, (char *)"October 19, 2017J ");
   //sDisplayText(7, 0, sFontSize2, (char *)FileDate);
*/
   RoverLCD.setTextColor(WROVER_YELLOW);
   sDisplayTextOrig(0, 0, sFontSize1, (char *)"Size 1");
   RoverLCD.setTextColor(WROVER_GREEN);
   sDisplayTextOrig(0, 0, sFontSize2, (char *)"Size 2");
   RoverLCD.setTextColor(WROVER_BLUE);
   sDisplayTextOrig(0, 0, sFontSize3, (char *)"Size 3");
   RoverLCD.setTextColor(WROVER_GREEN);
   sDisplayTextOrig(0, 0, sFontSize4, (char *)"Size 4");
   RoverLCD.setTextColor(WROVER_WHITE);
   sDisplayTextOrig(0, 0, sFontSize5, (char *)"Size 5");

   RoverLCD.setTextColor(WROVER_WHITE);
   return 1;
}  //sShowSplash


void DisplayUpdate(void) {
   if (bScreenChanged()) {
      //Serial << "sDisplayUpdate(): Refreshing screen" << endl;
      //DisplayClear();
    	//FillScreen(WROVER_RED);
      DisplayCurrentGear();
      DisplayServoPos();
      DisplayButtons();
      DisplayWatts();
      DisplayPitchRoll();
      //sDisplayOdometer();
      DisplayLowerBanner();
   } //if(bScreenChanged())
   return;
}  //DisplayUpdate


void DisplayClear() {
	FillScreen(WROVER_BLACK);
	//FillScreen(WROVER_RED);
	//FillScreen(WROVER_GREEN);
  return;
}  //DisplayClear


void FillScreen(UINT16 usColor) {
  RoverLCD.fillScreen(usColor);
  return;
}  //FillScreen


void DisplayText(UINT16 usCursorX, UINT16 usCursorY, char *pcText,
                 const GFXfont *pFont, UINT8 ucSize, UINT16 usColor, bool bRightJustify) {
  //Pass pFont as NULL for default text font.
	//If bRightJustify is true then usCursorX is number of pixels text is in from right side
  //240x320 3.2", 10 lines => 24 pixels/line
  RoverLCD.setFont(pFont);
  RoverLCD.setTextColor(usColor);
  RoverLCD.setTextSize(ucSize);
  RoverLCD.setTextWrap(false);
  if (bRightJustify) {
  	// 10/21/17 Justification calculation does not work
    UINT16					usBoundsX			= 0;
    UINT16					usBoundsY			= usCursorY;
		INT16						sUpperLeftX;
		INT16						sUpperLeftY;
		UINT16					usWidth;
		UINT16					usHeight;
		UINT16					usExtra				= 0;		//Extra space to get right justify calculation work
		Serial << "DisplayText(): Call getTextBounds(" << pcText << ", " << usBoundsX << ", " << usBoundsY << ", ..." << endl;
		RoverLCD.getTextBounds(pcText, usBoundsX, usBoundsY, &sUpperLeftX, &sUpperLeftY, &usWidth, &usHeight);
		Serial << "DisplayText(): getTextBounds() returned: sUpperLeftX= " << sUpperLeftX << ", sUpperLeftY= " << sUpperLeftY << endl;
		Serial << "DisplayText(): getTextBounds() returned: usWidth= " << usWidth << ", usHeight= " << usHeight << endl;
		//Serial << "DisplayText(): usCursorX= " << usCursorX << endl;
		Serial << "DisplayText(): WROVER_HEIGHT= " << WROVER_HEIGHT << endl;
		usCursorX= WROVER_HEIGHT - usCursorX - (usWidth * ucSize) - usExtra;
		Serial << "DisplayText(): usCursorX set to " << usCursorX << endl;
  }	//if (bRightJustify)
  RoverLCD.setCursor(usCursorX, usCursorY);
	//Serial << "DisplayText(): Call RoverLCD.println() with usCursorX= " << usCursorX << ", usCursorY= " << usCursorY << endl;
  RoverLCD.println(pcText);
  return;
}  //DisplayText


boolean bScreenChanged() {
   //Determine if something being displayed has changed & clear the flags.
   boolean bChanged= bGearChanged || bButtonsChanged || bServoChanged || bModeChanged;
   bGearChanged= bButtonsChanged= bServoChanged= bModeChanged= false;
   return bChanged;
}  //bScreenChanged


void DisplayCurrentGear() {
	// Place gear number (1 to 10) at right side 2x sized
	//const GFXfont   *pFont    			= &FreeSansBoldOblique24pt7b;
	const GFXfont   *pFont    			= &FreeMonoBold24pt7b;
	//const GFXfont   *pFont    			= &FreeMonoBoldOblique24pt7b;
	UINT16					usLine1Baseline	= 36;	//Puts TopOfChar 2 pixels below screen top
	UINT16					usLineSpacing		= 40;	//4 pixel spacing between lines (240 pixels is 6 lines)
  UINT16          usCursorX 			= 2;
  UINT16          usCursorY 			= 62;		//Was 72
  UINT8           ucSize    			= 2;
  UINT16          usColor   			= WROVER_WHITE;
  UINT16					usRightInset		= 2;	//Number of pixels to right of justified text
  bool						bRightJustify		= false;

	if (sCurrentMode == sNormalMode) {
		//sCurrentGear= 10;	//For testing
		itoa(sCurrentGear, sz100CharString, RADIX_10);
		if (sCurrentGear < 10){
			usCursorX= 250;		//Gears 1-9
		}
		else{
			usCursorX= 200;		//Gear 10
		}
		DisplayText( usCursorX, usCursorY, sz100CharString, pFont, ucSize, usColor, bRightJustify);
	}  //if (sCurrentMode..
	else {
		usCursorX= 220;		//Same as gears 1-9
		//We're in Calib mode so let's put a zero for the gear.
		strcpy(sz100CharString, "0");
		DisplayText( usCursorX, usCursorY, sz100CharString, pFont, ucSize, usColor, bRightJustify);
	}  //if (sCurrentMode..else

	//Set to smallest normal Sans font to label Gear under the gear number, 45mm,20mm
	usCursorX= 255;
	usCursorY= usCursorY + 20;
	ucSize= 1;
	pFont= &FreeSans9pt7b;
	DisplayText( usCursorX, usCursorY, "Gear", pFont, ucSize, usColor, bRightJustify);
  return;
}  //DisplayCurrentGear


void DisplayServoPos() {
	//50, 25
	//const GFXfont   *pFont    = &FreeSansBoldOblique12pt7b;
	const GFXfont   *pFont    = &FreeMonoBold12pt7b;
  UINT16          usCursorX = 260;
  UINT16          usCursorY = 115;
  UINT8           ucSize    = 1;
  UINT16          usColor   = WROVER_ORANGE;

  itoa(sServoPosLast, sz100CharString, RADIX_10);
  strcpy(szTempBuffer, sz100CharString);
  DisplayText( usCursorX + 6, usCursorY, szTempBuffer, pFont, ucSize, usColor, false);

	//Set to smallest normal Sans font for label under servo position
	usCursorY= usCursorY + 15;
	ucSize= 1;
	pFont= &FreeSans9pt7b;
	DisplayText( usCursorX, usCursorY, "Servo", pFont, ucSize, usColor, false);
  return;
}  //DisplayServoPos


void DisplayWatts() {
	// Place at left side 2x sized
	//const GFXfont   *pFont    			= &FreeSansBoldOblique24pt7b;
	const GFXfont   *pFont    			= &FreeMonoBold24pt7b;
	//const GFXfont   *pFont    			= &FreeMonoBoldOblique24pt7b;
  UINT16          usCursorX 			= 10;
  UINT16          usCursorY 			= 62;
  UINT8           ucSize    			= 2;
  UINT16          usColor   			= WROVER_YELLOW;
  //UINT16					usRightInset		= 2;	//Number of pixels to right of justified text
  bool						bRightJustify		= false;

	itoa((INT16)dWatts_, sz100CharString, RADIX_10);
	//usCursorX= 10;		//Gears 1-9
	DisplayText( usCursorX, usCursorY, sz100CharString, pFont, ucSize, usColor, bRightJustify);

	//Set to smallest normal Sans font to label
	usCursorX= 50;
	usCursorY= usCursorY + 20;
	ucSize= 1;
	pFont= &FreeSans9pt7b;
	DisplayText( usCursorX, usCursorY, "Watts", pFont, ucSize, usColor, bRightJustify);

	//Show Volts and Amps in 12pt Mono
	usCursorX= 10;
	usCursorY= usCursorY + 20;
	pFont= &FreeMonoBold12pt7b;
  strcpy(szTempBuffer, "V: ");
  itoa((INT16)dVolts_, sz100CharString, RADIX_10);
  strcat(szTempBuffer, sz100CharString);
  strcat(szTempBuffer, "   A: ");
	itoa((INT16)dAmps_, sz100CharString, RADIX_10);
	strcat(szTempBuffer, sz100CharString);
  DisplayText( usCursorX + 6, usCursorY, szTempBuffer, pFont, ucSize, usColor, false);
  return;
}  //DisplayWatts


void DisplayPitchRoll() {
	// Place at left side 2x sized
	const GFXfont   *pFont    			= &FreeMonoBold24pt7b;
  UINT16          usCursorX 			= 10;
  UINT16          usCursorY 			= 180;	//Was 62
  UINT8           ucSize    			= 2;
  UINT16          usColor   			= WROVER_GREEN;
  bool						bRightJustify		= false;
  //sprintf(szTempBuffer, "P %f", 123.45);
  //strcpy(szTempBuffer, "P");
  //dtostrf( dPitchPercent_, 5, 1, sz100CharString);
	itoa((INT16)dPitchPercent_, sz100CharString, RADIX_10);
  strcpy(szTempBuffer, sz100CharString);
  strcat(szTempBuffer, "%");
	DisplayText( usCursorX, usCursorY, szTempBuffer, pFont, ucSize, usColor, bRightJustify);

	//Set to smallest normal Sans font to label Gear under the gear number, 45mm,20mm
	usCursorX= 50;
	usCursorY= usCursorY + 20;
	ucSize= 1;
	pFont= &FreeSans9pt7b;
	DisplayText( usCursorX, usCursorY, "Pitch", pFont, ucSize, usColor, bRightJustify);
/*
  strcpy(szTempBuffer, "R");
  dtostrf( dRollDeg_, 6, 1, sz100CharString);
  strcat(szTempBuffer, sz100CharString);
  strcat(szTempBuffer, "Deg");
  sDisplayTextOrig(7,28, sFontNormal, szTempBuffer);
*/
  return;
}  //DisplayPitchRoll


void DisplayButtons() {
	const GFXfont   *pFont    			= &FreeSans9pt7b;
  UINT16          usCursorX 			= 260;
  UINT16          usCursorY 			= 160;
  UINT8           ucSize    			= 1;
  UINT16          usColor   			= WROVER_MAGENTA;
  bool						bRightJustify		= false;

  //Show 2 lines at right bottom for U d, D d
  strcpy(szTempBuffer, "U ");
  itoa(sButtonCount[sUp]  ,sz100CharString  , 10);
  strcat(szTempBuffer, sz100CharString);
	DisplayText( usCursorX, usCursorY, szTempBuffer, pFont, ucSize, usColor, bRightJustify);

	strcpy(szTempBuffer, "D ");
	itoa(sButtonCount[sDown]  ,sz100CharString  , 10);
	strcat(szTempBuffer, sz100CharString);
	usCursorY= usCursorY + 20;
	DisplayText( usCursorX, usCursorY, szTempBuffer, pFont, ucSize, usColor, bRightJustify);
  return;
}  //DisplayButtons


void DisplayLowerBanner(){
	const GFXfont   *pFont    			= &FreeSansOblique18pt7b;
  UINT16          usCursorX 			= 20;
  UINT16          usCursorY 			= 235;		//Was 72
  UINT8           ucSize    			= 1;
  UINT16          usColor   			= WROVER_CYAN;
  bool						bRightJustify		= false;

	DisplayText( usCursorX, usCursorY, "PowerShift Coach", pFont, ucSize, usColor, bRightJustify);
	return;
}	//DisplayLowerBanner


int sDisplayOdometer() {
   strcpy(szTempBuffer, "21.50");
   sDisplayTextOrig(5, 2, sFontBig, szTempBuffer);  //Start 2 pixels in.

   strcpy(szTempBuffer, "1123.00");
   sDisplayTextOrig(7, 2, sFontNormal, szTempBuffer);  //Start 2 pixels in.
   return 1;
}  //sDisplayOdometer


int sFillGearLocations(void) {
   for (int sGear=0; sGear <= sNumGears; sGear++) {
      asGearLocation[sGear]= asDefaultGearLocation[sGear];
   }  //for
   return 1;
}  //sFillGearLocations


int sLoopI2C() {
   int      asGyroReading[sNumGyroTypes][sNumAxis];
   //boolean  bApplySmoothing= APPLY_SMOOTHING;

   if (millis() > ulNextGyroTime) {
     //Serial << "sLoopI2C(): Reading gyro, milllis()= " << millis() << endl;
      Wire.beginTransmission(wMPU6050);
      Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
      Wire.endTransmission(false);
      //Wire.requestFrom(MPU,14,true);  // request a total of 14 registers
      //bool  bTrue= true;
      //Wire.requestFrom((uint8_t)MPU, (size_t)14, (bool)true);  // request a total of 14 registers

      //Wire.requestFrom((uint8_t)wMPU6050, (uint8_t)14, (uint8_t)true);  // request a total of 14 registers, with a stop

      // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
      // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
      // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
      // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
      // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
      // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
      // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
      asGyroReading[sAccel][sXAxis]= Wire.read()<<8|Wire.read();
      asGyroReading[sAccel][sYAxis]= Wire.read()<<8|Wire.read();
      asGyroReading[sAccel][sZAxis]= Wire.read()<<8|Wire.read();

      asGyroReading[sTemperature][sXAxis]= Wire.read()<<8|Wire.read();

      asGyroReading[sRotation][sXAxis]=Wire.read()<<8|Wire.read();
      asGyroReading[sRotation][sYAxis]=Wire.read()<<8|Wire.read();
      asGyroReading[sRotation][sZAxis]=Wire.read()<<8|Wire.read();

      //Initialize missing temperature fields.
      for (int sAxis= sYAxis; sAxis < sNumAxis; sAxis++) {
         asGyroReading[sTemperature][sAxis]= 0;
      }  //for
#if 0
      for (int sType= sAccel; sType < sNumGyroTypes; sType++) {
      	Serial << "sLoopI2C(): sType= " << sType;
        for (int sAxis= sXAxis; sAxis < sNumAxis; sAxis++) {
          Serial << "  " << asGyro[sType][sAxis];
         }  //for sAxis
        Serial << endl;
      }  //for sType
#endif

      //Serial << "sLoopI2C(): X Accel= " << asGyroReading[sAccel][sXAxis] << endl;
      //BLog("sLoopI2C(): XAcc   YAcc   ZAcc");
      //BLog("          ", asGyroReading[sAccel][sXAxis], asGyroReading[sAccel][sYAxis], asGyroReading[sAccel][sZAxis]);

      //Apply low-pass filter to data
      for (int sDataType= sAccel; sDataType < sNumGyroTypes; sDataType++) {
         for (int sAxis= sXAxis; sAxis < sNumAxis; sAxis++) {
#if APPLY_SMOOTHING
            asGyro[sDataType][sAxis]= FILTER_FUNC(asGyroReading[sDataType][sAxis],
                                                  pusSmoothingMemory[sDataType][sAxis]);
#else
            asGyro[sDataType][sAxis]= asGyroReading[sDataType][sAxis];
#endif
         }  //for sAxis
      }  //for sDataType

      //Convert raw accel readings into G's and correct axis
      //Because accel on BB is pointing down I am converting the axis
      //Report  -Y for X
      //        +Z for Y
      //        -X for Z
/*
      for (int sAxis= sXAxis; sAxis < sNumAxis; sAxis++) {
          adGvalueXYZ[sAxis]= (double)asGyroReading[sAccel][sAxis] / dGConvert;
      }  //for sAxis
*/
      adGvalueXYZ[sXAxis]= -(double)asGyroReading[sAccel][sYAxis] / dGConvert;
      adGvalueXYZ[sYAxis]=  (double)asGyroReading[sAccel][sZAxis] / dGConvert;
      adGvalueXYZ[sZAxis]= -(double)asGyroReading[sAccel][sXAxis] / dGConvert;

/*
      Serial << "sLoopI2C(): G's X, Y, Z " << adGvalueXYZ[sXAxis] << ", "
             << adGvalueXYZ[sYAxis] << ", " << adGvalueXYZ[sZAxis] << endl;
*/
      ComputePitchAndRoll();
      bGyroChanged= true;
      ulNextGyroTime= millis() + ulGyroReadTime;
   }  //if (millis()>ulNextGyroTime)
   return 1;
}  //sLoopI2C


void ComputePitchAndRoll() {
  dRollDeg_ = atan2((-adGvalueXYZ[sYAxis]), adGvalueXYZ[sZAxis]) * dRadsToDeg;
  dPitchDeg_= atan2(adGvalueXYZ[sXAxis],
                sqrt(adGvalueXYZ[sYAxis] * adGvalueXYZ[sYAxis] +
                     adGvalueXYZ[sZAxis] * adGvalueXYZ[sZAxis])) * dRadsToDeg;
  dPitchPercent_= dGetPitchPercent(dPitchDeg_);

  //Correct for current readings being 180 degrees off
  if (dRollDeg_ < 0.0) {
    dRollDeg_= dRollDeg_= -180.0 - dRollDeg_;
  } //if(dRollDeg_<0.0)
  else {
    dRollDeg_= dRollDeg_= 180.0 - dRollDeg_;
  } //if(dRollDeg_<0.0)else

  Serial << "ComputePitchAndRoll(): Pitch Deg, Pitch%, Roll " << dPitchDeg_ << ", " << dPitchPercent_ << ", " << dRollDeg_<< endl;
  return;
} //ComputePitchAndRoll


double dGetPitchPercent(double dPitchDeg) {
  double dPitchPercent= -99.99;
  //Serial << "dGetPitchPercent(): dPitchDeg_= " << dPitchDeg_ << endl;
  if ((dPitchDeg_ < 44.0) && (dPitchDeg_ > -44.0)) {
    dPitchPercent= 100.0 * tan(dPitchDeg_ / dRadsToDeg);
    //Serial << "dGetPitchPercent(): dPitchPercent_= " << dPitchPercent_ << endl;
  } //if((dPitchDeg_<44.0)&&...
  return dPitchPercent;
} //dGetPitchPercent


int sSetupGyro() {
   Serial << "sSetupGyro(): Begin"<< endl;
   //BLog("sSetupGyro(): Begin");
   //Set up the I2C bus.
   //Wire.begin();
   Wire.begin(sI2C_SDA, sI2C_SCL);
   Wire.beginTransmission(wMPU6050);
   Serial << "sSetupGyro(): Call MPU6050_PrintName()"<< endl;
   MPU6050_PrintName();
/*
   Wire.write(0x6B);  // PWR_MGMT_1 register
   Wire.write(0);     // set to zero (wakes up the MPU-6050)
*/
   // Clear the 'sleep' bit to start the sensor.
   MPU6050_write_reg (MPU6050_PWR_MGMT_1, 0);
   Wire.endTransmission(true);
   //Initialize the data array.
   for (int sDataType= sAccel; sDataType < sNumGyroTypes; sDataType++) {
      for (int sAxis= sXAxis; sAxis < sNumAxis; sAxis++) {
         asGyro[sDataType][sAxis]= 0;
      }  //for sDataType
   }  //for sAxis
   return 1;
}  //sSetupGyro


int sHandleButtons(void) {
/*
   int          sButton;
   int          sGearChange;
   int          sNewGear;
   int          sTargetLocation;
   int          sTargetChange= 0;
*/
  if (!bHandleBothHeld()) {
     if (millis() > ulModeReadyTime) {
       switch(sCurrentMode) {
         case sNormalMode:
            sHandleNormalMode();
            break;   //sNormalMode
         case sCalibMode:
            sHandleCalibMode();
            break;   //sCalibMode
         default:
            Serial << "sHandleButtons(): Unexpected switch value." << endl;
            break;
        } //switch
     }   //if(millis()...
   }  //if(!bHandleBothHeld())
   return 1;
}  //sHandleButtons


boolean bHandleBothHeld(void) {
   if (abButtonBeingHeld[sUp] &&  abButtonBeingHeld[sDown]) {
      if (millis() > ulNextModeTime) {
         Serial << sLineCount++ << " bHandleBothHeld(): Both buttons are being held." << endl;
         //Clear the button counts;
         sButtonCount[sUp]  = 0;
         sButtonCount[sDown]= 0;
         switch (sCurrentMode) {
            case sNormalMode:
               sCurrentMode= sCalibMode;
               bModeChanged= true;
               Serial << sLineCount++
                      << " bHandleBothHeld(): Switch from Normal to Calib mode." << endl;
               break;
            case sCalibMode:
               sCurrentMode= sNormalMode;
               bModeChanged= true;
               Serial << sLineCount++
                      << " bHandleBothHeld(): Switch from Calib to Normal mode." << endl;
               break;
            default:
               Serial << sLineCount++
                      << " bHandleBothHeld(): Bad switch :" << sCurrentMode << endl;
               break;
         }  //switch
         //Create the guard times between mode switches or button handling
         ulNextModeTime= millis() + ulModeSwitchTime;
         ulModeReadyTime= millis() + ulModeWaitTime;
      }  //if(millis()...
      return true;
   }    //if((sButtonCount[sUp]==sHoldCode)...
   else {
      return false;
   }  //if((sButtonCount[sUp]==sHoldCode)...
}  //bHandleBothHeld


int sHandleNormalMode(void) {
   int          sButton;
   int          sGearChange;
   int          sNewGear;
   int          sTargetLocation;
   int          sTargetChange= 0;
   boolean      bReturn= false;

   if (!bReturn) {
     for (sButton= sUp; sButton <= sDown; sButton++) {
      if (sButton == sUp) {
         sGearChange= 1;
      }
      else {
         sGearChange= -1;
      }
      //Compute net gear change by handling at most one request from each button
      if (sButtonCount[sButton] > 0) {
         sTargetChange += sGearChange;
         //Serial << sLineCount++ << " sHandleNormalMode(): Button" << sButton << ", Count= " << sButtonCount[sButton] << ", sTargetChange= " << sTargetChange << endl;
         sButtonCount[sButton]--;
         bButtonsChanged= true;
      }  //if((sButtonCount[sButton]!=sHoldCode)...
    } //for

    sNewGear= constrain(sCurrentGear + sTargetChange, 1, sNumGears);
    sCurrentGear= sNewGear;
    sTargetLocation= asGearLocation[sCurrentGear];

    //Make the actual shift
#if DO_SERVO
    sServoMove(sTargetLocation);
#endif
   }  //if(!bReturn)
   return 1;
}  //sHandleNormalMode


int sHandleCalibMode(void) {
   int          sButton;
   int          sDirection;
   int          sTarget;

   for (sButton= sUp; sButton <= sDown; sButton++) {
      if (sButton == sUp) {
         sDirection= 1;
      }
      else {
         sDirection= -1;
      }
      //if (sButtonCount[sButton] == sHoldCode) {
      if (abButtonBeingHeld[sButton]) {
         sTarget= sServoPosLast + sDirection * sHoldDeltaPos;
         Serial << sLineCount++ << " sHandleCalibMode(): Button" << sButton
                << ", Count= " << sButtonCount[sButton] << ", Target= " << sTarget << endl;
         sServoMove(sTarget);
         bServoChanged= true;
      }  //if(sButtonCount[sButton]==sHoldCode)

      //if ((sButtonCount[sButton] != sHoldCode) && (sButtonCount[sButton] > 0)) {
      if (!abButtonBeingHeld[sButton] && (sButtonCount[sButton] > 0)) {
         sTarget= sServoPosLast + sDirection * sTrimDeltaPos;
         Serial << sLineCount++ << " sHandleCalibMode(): Button" << sButton
                << ", Count= " << sButtonCount[sButton] << ", Target= " << sTarget << endl;
         sServoMove(sTarget);
         bServoChanged= true;
         sButtonCount[sButton]--;
      }  //if((sButtonCount[sButton]!=sHoldCode)...
    } //for
 return 1;
}  //sHandleCalibMode


int sCheckButtons(void) {
/*Algorithm to determine when a button has been pressed or held.
 * Use IsRelease() to indicate the button has been pressed
 * Use IsHold() to indicate the button has been held down
 * States (Initial idea, not sure how close method is to this 4/25/15)
 *    0 - Button open
 *    1 - Button pressed, IsPushed() returned true once
 *          Moves State to 2 when InPush() first returns true
 *    2 - InPush() returning true, button is down
 *    3 - IsRelease() returned true, button has been released
 *          Moves State to 0
 *    4 - IsHold() returned true, button  held past threshhold
 *          Moves State to 0 when IsRelease returns true.
 * sUpButtonCount will be:
 *    0   Nothing to be done
 *   99   Button was held and not yet released
 *   1-10 Number of button presses not yet handled, max 10
 */
  //static int   sLocalButtonState[]  = {0, 0};
  boolean      bReturn              = false;
  int          sButton;

#if DO_BUTTONS
  UpButton.update();
  DownButton.update();
  //SelectButton.update();

   //Run through the buttons, use short-circuiting to select
   for (sButton= sUp; sButton <= sDown; sButton++) {
      //Check for IsRelease for all buttons.
      if ( ((sButton == sUp)   && UpButton.IsRelease  ()) ||
      ((sButton == sDown) && DownButton.IsRelease()) ) {
         Serial << sLineCount++ << " sCheckButtons(): Button " << sButton
                << " was released." << endl;
         //Check to see if button is being held
         //if (sLocalButtonState[sButton] != sButtonHeld) {
         if ( !abButtonBeingHeld[sButton]) {
            if (sButtonCount[sButton] < sMaxButtonPresses) {
               //Increment the count of button presses to be handled.
               //Serial << sLineCount++ << " sCheckButtons(): Button " << sButton << " count incremented." << endl;
               sButtonCount[sButton]++;
               bButtonsChanged= true;
            } //if(sLocalUpButtonState!=sButtonHeld)
         }    //if(sLocalButtonState!=...
         else {
            //The button was being held down, update the state variable..
            Serial << sLineCount++ << " sCheckButtons(): Button " << sButton
                   << " done being held." << endl;
            //sLocalButtonState[sButton]= sButtonOpen;
            abButtonBeingHeld[sButton]= false;
         } //if(sLocalUpButtonState!=sButtonHeld)else
         bReturn= true;
      }   //if(((sButton==sUp)&&...

      //Check IsHold for up and down buttons.
      if (!bReturn &&
      (((sButton == sUp)   && UpButton.IsHold  ()) ||
       ((sButton == sDown) && DownButton.IsHold()) )) {
         Serial << sLineCount++ << " sCheckButtons(): Button " << sButton
                << " being held." << endl;
         //Set state to indicate in hold.
         //sLocalButtonState[sButton]= sButtonHeld;
         //sButtonCount[sButton]= sHoldCode;
         abButtonBeingHeld[sButton] = true;
         sButtonCount[sButton]      = 0;  //Clear the count
         bButtonsChanged= true;
      }   //if(!bReturn&&UpButton.IsHold()
      bReturn= false;
   } //for (sButton= sUp...
#endif	//DO_BUTTONS
  return 1;
}  //sCheckButtons


void TestButtonPins() {
	int sUpButtonLocal		= 2;
	int sDownButtonLocal	= 4;
  Serial << "SetupPins(): Call pinMode(" << sUpButtonLocal << ", INPUT_PULLUP) for Up" << endl;
	pinMode(sUpButtonLocal, INPUT_PULLUP);

  Serial << "SetupPins(): Call pinMode(" << sDownButtonLocal << ", INPUT_PULLUP) for Down" << endl;
	pinMode(sDownButtonLocal, INPUT_PULLUP);

	int sValueUp;
	int sValueDown;
	for (int i= 0; i<10; i++){
		Serial << "SetupPins(): Read buttons, Try= " << i << endl;
		sValueUp= digitalRead(sUpButtonLocal);
		sValueDown= digitalRead(sDownButtonLocal);
	  Serial << "SetupPins(): Read buttons, Pin "
	  		<< sUpButtonLocal << "= " << sValueUp << ", Pin " << sDownButtonLocal <<  "= " << sValueDown << endl;
	  delay(1000);
	}	//for(inti= 0;...
  return;
}  //TestButtonPins


int sDisplayTextOrig(int sLineNumber, int sPixelStart, int sFont, char *pcText) {
   //RoverLCD.setTextColor(WROVER_WHITE);
   switch (sFont) {
     case sFontSize1:
         RoverLCD.setTextSize(1);
         break;
      case sFontSize2:
         RoverLCD.setTextSize(2);
         break;
      case sFontSize3:
        RoverLCD.setTextSize(3);
         //DOG.string(sPixelStart, sLineNumber, font_16x32nums, pcText);
         break;
      case sFontSize4:
         RoverLCD.setTextSize(4);
         break;
       case sFontSize5:
         RoverLCD.setTextSize(5);
         //DOG.string(sPixelStart, sLineNumber, font_8x8, pcText);
         break;
      default:
         Serial << "sDisplayText(): Bad case in switch()= " << sFont << endl;
         break;
   }  //switch
   RoverLCD.println(pcText);
   return 1;
}  //sDisplayTextOrig


/*
void DisplayTextTest() {
	//const GFXfont   *pFont    = &FreeSansBoldOblique24pt7b;
	const GFXfont   *pFont    = &FreeMonoBold24pt7b;
	UINT16					usLine1Baseline	= 36;	//Puts TOC 2 pixels below screen top
	//UINT16					usLineSpacing	= 36;	//No spacing between lines (240 pixels is 6.7 lines)
	//UINT16					usLineSpacing	= 39;	//3 pixel spacing between lines (234 pixels is 6 lines)
	UINT16					usLineSpacing	= 40;	//4 pixel spacing between lines
  UINT16          usCursorX = 0;
  UINT16          usCursorY;
  UINT8           ucSize    = 1;
  UINT16          usColor   = WROVER_YELLOW;

  strcpy(szTempBuffer, "G2Servo ");
  itoa(sServoPosLast  ,sz100CharString  , 10);
  strcat(szTempBuffer, sz100CharString);
  Serial << "DisplayServoPos(): Display text: " << szTempBuffer << endl;
  for (int wLine= 0; wLine < 6; wLine++) {
  	usCursorY = usLine1Baseline + (wLine * usLineSpacing);
  	DisplayText( usCursorX, usCursorY, szTempBuffer, pFont, ucSize, usColor, false);
  }
  DisplayText( usCursorX, usCursorY, szTempBuffer, pFont, ucSize, usColor, false);
  return;
}  //DisplayTextTest
*/
//Last line

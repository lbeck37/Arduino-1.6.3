/* ShiftE_Calib.ino Arduino Sketch to run ShiftE derailer
 05/09/15- Change Gear locations for 9-spd cassette using cogs 3 to 9
 04/25/15- Update to work with MicroView
 04/19/15- Implement Normal and Calibrate modes
 04/10/15 Beck- Port from Arduino 0022 to 1.6.3
*/
#include <Arduino.h>
#include <SPI.h>
#include <EasyButton.h>
#include <Streaming.h>
#include <Servo.h>

#include <dog_1701.h>
#include <font_16x32nums.h>
#include <font_6x8.h>
#include <font_8x16.h>
#include <font_8x8.h>
#include <logo_BLH.h>

#include <Wire.h>
const int MPU= 0x68;  // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

#include <microsmooth.h>

//Defines                12345678901234567
#define szSplashLine1   "Electic Shifting"
#define szSplashLine2   "-Max Performance"
#define szSplashLine3   "-Max Range"
#define UINT16          unsigned int
//#define NO_SMOOTHING
#define SKIP_SMOOTH

//Here come the const's sSplashDelay
static const int       sSplashDelay          = 3000;     //mSec that Splash screen is on
//static const int  asDefaultGearLocation[]= {0, 150, 119, 92, 74, 64, 48, 17};
static const int asDefaultGearLocation[]= {0, 122, 101, 74, 68, 56, 35, 20};   //9-spd cogs 3-9
static const int       sServoMin             = 0;
static const int       sServoMax             = 180;
static const int       sServoMsecWait        = 15;
static const int       sNumGears             = 7;
static const boolean   bServoOn              = true;
static const int       sHoldDeltaPos         = 2; //Servo move size when button held.
static const int       sTrimDeltaPos         = 1; //Servo move size when button clicked.

static const int       sNormalMode           = 0;
static const int       sCalibMode            = 1;

static const int       sUp                   = 0;
static const int       sDown                 = 1;
static const int       sSelect               = 2;
static const int       sFirstButton          = sUp;
static const int       sLastButton           = sSelect;

static const boolean   bButtonPullUp         = true;

//Digital Pins
static const int       sSelectButton         = A3;
static const int       sDownButton           = A2;
static const int       sUpButton             = A1;
static const int       sBacklightPin         =  6;
static const int       sServoPin             =  7;
static const byte      cSPICmdDataPin        =  9;
static const byte      cSPIChipSelectPin     = 10;

//Gyro defines
static const int       sXAxis             = 0;
static const int       sYAxis             = 1;
static const int       sZAxis             = 2;
static const int       sNumAxis           = 3;

static const int       sAccel             = 0;
static const int       sRotation          = 1;
static const int       sTemperature       = 2;
static const int       sNumDataTypes      = 3;

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
static const unsigned long    ulGyroReadTime    = 500;   //Gyro reads spaced by this.

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

static const int       sFontNormal         =   1;     //6x8
static const int       sFontBig            =   2;     //8x16
static const int       sFontBigNum         =   3;     //16x32nums
static const int       sFontSquare         =   4;     //8x8

static const byte       cBogusResetPin      = 4;
static const byte       cHW_SPI             = 0;      //This is what their demo used.
static const byte       cDisplayType        = DOGS102;

//End of the const's

static int asGearLocation[sNumGears + 1];
static int sCurrentGear                   = 2;

#ifdef NO_SMOOTHING
static int asAccelReading[]               = {0, 0, 0};
static int asLastAccelReading[]           = {0, 0, 0};
#else
static UINT16  *pusSmoothingMemory[sNumDataTypes][sNumAxis];
static int     asGyro             [sNumDataTypes][sNumAxis];
#endif   //NO_SMOOTHING

static int sCurrentMode                   = sNormalMode;
static int sServoPosLast                  = 0;

//Create servo object to control the servo
Servo myservo;

//Create display object fot the DOGS102-6 (102x64) display
dog_1701 DOG;

//Create EasyButton objects to handle button presses.
EasyButton UpButton     (sUpButton,     NULL, CALL_NONE, bButtonPullUp);
EasyButton DownButton   (sDownButton,   NULL, CALL_NONE, bButtonPullUp);
EasyButton SelectButton (sSelectButton, NULL, CALL_NONE, bButtonPullUp);


//Number of unhandled presses, up to sMaxButtonPresses
static int              sButtonCount[]       = { 0, 0, 0};
static int              sButtonCountLast[]   = { 0, 0, 0};
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

static char        szLineBuffer[25];   //DOGS102 line is 17 chars with 6x8 normal font.
static char        sz10CharString[10];

// The Arduino setup() method runs once, when the sketch starts
void setup() {
   Serial.begin(9600);
   Serial << "Begin setup()" << endl;
   Serial << "Free Ram= " << freeRam() << endl;

   Serial << sLineCount++ << " setup(): Call sSetupI2C()" << endl;
   sSetupI2C();
   sFillGearLocations();
   sServoInit();
   Serial << sLineCount++ << " setup(): Call sSetupSmoothing()" << endl;
   sSetupSmoothing();
   sShowStartScreen();

   //Dither the servo once so it's position shows on the LCD.
   sServoDither(1, 1); // +/- 1 degree, once
   return;
}  //setup

int sSetupSmoothing() {
   Serial << sLineCount++ << " sSetupSmoothing(): Begin" << endl;
//#ifndef NO_SMOOTHING
   //Initialize memory for data smoothing and set data fields to zero.
   for (int sDataType= sAccel; sDataType < sNumDataTypes; sDataType++) {
      for (int sAxis= sXAxis; sAxis < sNumAxis; sAxis++) {
         pusSmoothingMemory[sNumDataTypes][sNumAxis]  = ms_init(SMA);
         asGyro            [sNumDataTypes][sNumAxis]  = 0;
         //asGyroLast        [sNumDataTypes][sNumAxis]  = 0;
      }  //for
   }  //for
//#endif
   Serial << sLineCount++ << " sSetupSmoothing(): End" << endl;
   return 1;
}  //sSetupSmoothing


int sSetupI2C() {
   Wire.begin();
   Wire.beginTransmission(MPU);
   Wire.write(0x6B);  // PWR_MGMT_1 register
   Wire.write(0);     // set to zero (wakes up the MPU-6050)
   Wire.endTransmission(true);
   return 1;
}  //sSetupI2C


// The Arduino loop() method gets called over and over.
void loop() {
  sCheckButtons();
  sLoopI2C();
  sDisplayUpdate();
  sHandleButtons();
  return;
}  //loop()


int sLoopI2C() {
   int asGyroReading[sNumDataTypes][sNumAxis];

   //Serial << sLineCount++ << " sLoopI2C(): Begin" << endl;
   if (millis() > ulNextGyroTime) {
      Serial << sLineCount++ << " sLoopI2C(): Take reading" << endl;
      Wire.beginTransmission(MPU);
      Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
      Wire.endTransmission(false);
      Wire.requestFrom(MPU,14,true);  // request a total of 14 registers

#ifdef NO_SMOOTHING
      AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
      AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
      AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
      Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
      GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
      GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
      GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

      asAccelReading[sXAxis]= AcX;
      asAccelReading[sYAxis]= AcY;
      asAccelReading[sZAxis]= AcZ;
      //See if any of the displayed readings have changed.
      for (int sAxis= sXAxis; sAxis <= sZAxis; sAxis++) {
         if (asAccelReading[sAxis] != asLastAccelReading[sAxis]) {
            asLastAccelReading[sAxis]= asAccelReading[sAxis];
            bGyroChanged= true;
         }  //if(asAccelReading[sAxis]!=...
      }  //for
#else
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

      //Apply low-pass filter to data
      for (int sDataType= sAccel; sDataType < sNumDataTypes; sDataType++) {
         for (int sAxis= sXAxis; sAxis < sNumAxis; sAxis++) {
#ifdef SKIP_SMOOTH
            asGyro[sDataType][sAxis]= asGyroReading[sDataType][sAxis];
#else
            asGyro[sDataType][sAxis]=
                  sma_filter(asGyroReading[sDataType][sAxis], pusSmoothingMemory[sDataType][sAxis]);
#endif   //SKIP_SMOOTH
         }  //for
      }  //for
#endif   //ifdef NO_SMOOTHING

      ulNextGyroTime= millis() + ulGyroReadTime;
   }  //if (millis()>ulNextGyroTime)
   return 1;
}  //sLoopI2C


int sDisplayUpdate(void) {
   if (bScreenChanged()) {
      Serial << sLineCount++ << " sDisplayUpdate(): Begin" << endl;
      sDisplayClear();
      //sDisplayButtons();
      sDisplayCurrentGear();
      sDisplayServoPos();
      sDisplayGyro();
      sDisplayOdometer();
   }  //if(bScreenChanged())
   return 1;
}  //sDisplayUpdate


int sDisplayBegin() {
   DOG.initialize(cSPIChipSelectPin, cHW_SPI       , cHW_SPI,
                  cSPICmdDataPin   , cBogusResetPin, DOGS102);
   DOG.view(sDisplayNormal);  //View screen Normal or Flipped
   //Set backlight pin to be a PWM "analog" out pin.
   //Drive LED backlight through 15 ohm resistor.
   pinMode(sBacklightPin, OUTPUT);
   sDisplaySetBrightness(sDefaultBrightness);
   return 1;
}  //sDisplayBegin


int sShowStartScreen(void) {
   sDisplayBegin();
   sShowSplash();
   delay(sSplashDelay);
   return 1;
}  //sShowStartScreen


int sShowSplash(void) {
   sDisplayClear();

   //2 lines of big font takes lines 0-3
   sDisplayText(0, 0, sFontBig, "PowerShift");
   sDisplayText(2, 0, sFontBig, "  by ShiftE");

   //Lines in normal font
   sDisplayText(5, 0, sFontNormal, szSplashLine1);
   sDisplayText(6, 0, sFontNormal, szSplashLine2);
   sDisplayText(7, 0, sFontNormal, szSplashLine3);

   return 1;
}  //sShowSplash


int sDisplaySetBrightness(int sBrightness){
   analogWrite(sBacklightPin, sBrightness);
   return 1;
}  //sDisplaySetBrightness


int sDisplayClear() {
   DOG.clear();  //clear whole display
   return 1;
}  //sDisplayClear


int sDisplayText(int sLineNumber, int sPixelStart, int sFont, char *pcText) {
   switch (sFont) {
      case sFontNormal:
         DOG.string(sPixelStart, sLineNumber, font_6x8, pcText);
         break;
      case sFontBig:
         DOG.string(sPixelStart, sLineNumber, font_8x16, pcText);
         break;
      case sFontBigNum:
         DOG.string(sPixelStart, sLineNumber, font_16x32nums, pcText);
         break;
      case sFontSquare:
         DOG.string(sPixelStart, sLineNumber, font_8x8, pcText);
         break;
      default:
         Serial << "sDisplayText(): Bad case in switch()= " << sFont << endl;
         break;
   }  //switch
   return 1;
}  //sDisplayText


boolean bScreenChanged() {
   //Determine if something being displayed has changed & clear the flags.
   boolean bChanged= bGearChanged || bButtonsChanged || bServoChanged ||
                     bModeChanged || bGyroChanged;
   bGearChanged= bButtonsChanged= bServoChanged= bModeChanged= bGyroChanged= false;
   return bChanged;
}  //bScreenChanged


int sDisplayButtons() {
   //Show 3 lines at right bottom for U d, D d, S d
   //String will be 3 char long => 18 pixels, start so 2 pixels remain on right
   strcpy(szLineBuffer, "U ");
   itoa(sButtonCount[sUp]  ,sz10CharString  , 10);
   strcat(szLineBuffer, sz10CharString);
   sDisplayText(5, 82, sFontNormal, szLineBuffer);

   strcpy(szLineBuffer, "D ");
   itoa(sButtonCount[sDown]  ,sz10CharString  , 10);
   strcat(szLineBuffer, sz10CharString);
   sDisplayText(6, 82, sFontNormal, szLineBuffer);

   strcpy(szLineBuffer, "S ");
   itoa(sButtonCount[sSelect]  ,sz10CharString  , 10);
   strcat(szLineBuffer, sz10CharString);
   sDisplayText(7, 82, sFontNormal, szLineBuffer);
   return 1;
}  //sDisplayButtons


int sDisplayGyro() {
   //Show 3 lines at right side for X, Y and Z acceleration
   //String will be 3 char long => 18 pixels, start so 2 pixels remain on right
   int sStartPixel   =  60;
   int sStartLine    = 2;

   sDisplayText(sStartLine++, sStartPixel, sFontNormal, "Accel");

   strcpy(szLineBuffer, "X= ");
#ifdef NO_SMOOTHING
   itoa(asAccelReading[sXAxis]  ,sz10CharString  , 10);
#else
   itoa(asGyro[sAccel][sXAxis]  ,sz10CharString  , 10);
#endif
   strcat(szLineBuffer, sz10CharString);
   sDisplayText(sStartLine++, sStartPixel, sFontNormal, szLineBuffer);

   strcpy(szLineBuffer, "Y= ");
#ifdef NO_SMOOTHING
   itoa(asAccelReading[sYAxis]  ,sz10CharString  , 10);
#else
   itoa(asGyro[sAccel][sYAxis]  ,sz10CharString  , 10);
#endif
   strcat(szLineBuffer, sz10CharString);
   sDisplayText(sStartLine++, sStartPixel, sFontNormal, szLineBuffer);

   strcpy(szLineBuffer, "Z= ");
#ifdef NO_SMOOTHING
   itoa(asAccelReading[sZAxis]  ,sz10CharString  , 10);
#else
   itoa(asGyro[sAccel][sZAxis]  ,sz10CharString  , 10);
#endif
   strcat(szLineBuffer, sz10CharString);
   sDisplayText(sStartLine++, sStartPixel, sFontNormal, szLineBuffer);

   return 1;
}  //sDisplayButtons


int sDisplayCurrentGear() {
   Serial << sLineCount++ << " sDisplayCurrentGear(): Gear 3 location= " << asDefaultGearLocation[3] << endl;
   if (sCurrentMode == sNormalMode) {
      itoa(sCurrentGear, sz10CharString, 10);
      sDisplayText(0, 4, sFontBigNum, sz10CharString);    //1st char in 4 pixels.
   }  //if (sCurrentMode..
   else {
      //We're in Calib mode so let's put a zero for the gear.
      sDisplayText(0, 4, sFontBigNum, "0");               //1st char in 4 pixels.
   }  //if (sCurrentMode..else

   return 1;
}  //sDisplayCurrentGear


int sDisplayServoPos() {
   itoa(sServoPosLast, sz10CharString, 10);
   strcpy(szLineBuffer, "Servo ");
   strcat(szLineBuffer, sz10CharString);
   //Text will end 2 pixels in from right edge when servo value is 3 digits.
   sDisplayText(0, 28, sFontBig, szLineBuffer);
   return 1;
}  //sDisplayServoPos


int sDisplayOdometer() {
   strcpy(szLineBuffer, "21.50");
   sDisplayText(6, 2, sFontBig, szLineBuffer);  //Start 2 pixels in.

   //strcpy(szLineBuffer, "1123.00");
   //sDisplayText(7, 2, sFontNormal, szLineBuffer);  //Start 2 pixels in.
   return 1;
}  //sDisplayOdometer


int sFillGearLocations(void) {
   for (int sGear=0; sGear <= sNumGears; sGear++) {
      asGearLocation[sGear]= asDefaultGearLocation[sGear];
   }  //for
   return 1;
}  //sFillGearLocations


int sHandleButtons(void) {
   int          sButton;
   int          sGearChange;
   int          sNewGear;
   int          sTargetLocation;
   int          sTargetChange= 0;

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
         Serial << sLineCount++ << " sHandleNormalMode(): Button" << sButton << ", Count= "
                << sButtonCount[sButton] << ", sTargetChange= " << sTargetChange << endl;
         sButtonCount[sButton]--;
         bButtonsChanged= true;
      }  //if((sButtonCount[sButton]!=sHoldCode)...
    } //for

    sNewGear= constrain(sCurrentGear + sTargetChange, 1, sNumGears);
    Serial << sLineCount++ << " sHandleNormalMode(): Current gear= " << sCurrentGear << ", New= " << sNewGear << endl;
    sCurrentGear= sNewGear;
    Serial << sLineCount++ << " sHandleNormalMode(): asGearLocation[2]= " << asGearLocation[2] << endl;
    Serial << sLineCount++ << " sHandleNormalMode(): asGearLocation[3]= " << asGearLocation[3] << endl;
    sTargetLocation= asGearLocation[sCurrentGear];

    Serial << sLineCount++ << " sHandleNormalMode(): Call sServoMove() Target= " << sTargetLocation << endl;

    //Make the actual shift
    sServoMove(sTargetLocation);
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
/* Algorithm to determine when a button has been pressed or held.
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

  UpButton.update();
  DownButton.update();
  SelectButton.update();

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
               Serial << sLineCount++ << " sCheckButtons(): Button " << sButton
                      << " count incremented." << endl;
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
      ((sButton == sUp)   && UpButton.IsHold  ()) ||
      ((sButton == sDown) && DownButton.IsHold()) ) {
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
  return 1;
}  //sCheckButtons


int sServoInit() {
   if (bServoOn) {
      myservo.attach(sServoPin);
      sServoMove(asGearLocation[sCurrentGear]);
   }  //if(bServoOn)
   return 1;
} //sServoInit

int sServoMove(int sServoPos) {
  if (sServoPos != sServoPosLast) {
      sServoPos= constrain(sServoPos, sServoMin, sServoMax);
      Serial << "sServoMove(): Move to " << sServoPos << endl;
      sServoPosLast= sServoPos;
      sServoSetPosition(sServoPos);
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
      myservo.write(sServoPos);
   }
   delay(sServoMsecWait);
   return 1;
}  //sServoSetPosition


//freeRam() returns the number of bytes currently free in RAM.
int freeRam(void)
{
  extern int  __bss_end;
  extern int  *__brkval;
  int free_memory;
  if((int)__brkval == 0) {
    free_memory = ((int)&free_memory) - ((int)&__bss_end);
  }
  else {
    free_memory = ((int)&free_memory) - ((int)__brkval);
  }
  return free_memory;
}  //freeRam
//Last line.

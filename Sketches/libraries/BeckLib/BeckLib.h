//BeckLib.h, Apr 14, 2017
#ifndef BECKLIB_H
#define BECKLIB_H
#include <Streaming.h>
#include <Time.h>
//#include <Firebase.h>
//#include <BeckFirebase.h>
#include <Adafruit_ADS1015.h>   //Just for the defines

#ifndef OTA_SERVER
  #define OTA_SERVER
#endif  //OTA_SERVER

#ifdef ESP8266
  #include <ESP8266WiFi.h>
  #include <WiFiClient.h>
  //#include <BlynkSimpleEsp8266.h>
  #ifdef OTA_SERVER
    #include <ESP8266WebServer.h>
    #include <ESP8266mDNS.h>
    #include <ESP8266HTTPUpdateServer.h>
    //extern ESP8266WebServer           oHttpServer;
    //extern ESP8266HTTPUpdateServer    oHttpUpdateServer;
  #endif  //OTA_SERVER
#endif  //ESP8266

#ifdef ESP32                    //FromBeckESP32WiFiExample.ino from Blynk Examples
  #define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
  #include <WiFi.h>
  #include <WiFiClient.h>
  //#include <BlynkSimpleEsp32.h>
#endif  //ESP32

//#define NO_DISPLAY
#ifndef NO_DISPLAY
	#ifdef USE_U8GLIB
		#include <U8glib.h>
	#else
		#include <dog_1701.h>
		#include <font_16x32nums.h>
		#include <font_6x8.h>
		#include <font_8x16.h>
		#include <font_8x8.h>
		#include <logo_BLH.h>
	#endif	//USE_U8GLIB
#endif	//NO_DISPLAY

#define INT8        char
#define UINT8       unsigned char
#define INT16       int16_t
#define UINT16      uint16_t
#define INT32       int32_t
#define UINT32      uint32_t
#define INT64       int64_t
#define UINT64      uint64_t

#define LOG0        szLogLineHeader(++lLineCount)
#define BLog        LogJustToSerial
#define BLogS       LogJustToSerial

static const long   lSerialMonitorBaud    = 115200;
static const long   lMsecPerDay           = 86400000;
static const long   lMsecPerHour          =  3600000;
static const long   lMsecPerMin           =    60000;
static const long   lMsecPerSec           =     1000;

//Define Virtual Pin names
#define ReadF_V0          V0
#define ReadF_V1          V1
#define SetSetpointF_V2   V2
#define GetSetpointF_V3   V3
#define ThermoSwitch_V4   V4
#define ThermoLED_V5      V5

#define AtoD_1V6          V6

#define Terminal_V7       V7
#define LCD_Line0_V8      V8
#define LCD_Line1_V9      V9

//Relay #1
#define Switch_1V10       V10
#define TimerA_1V11       V11
#define TimerB_1V12       V12
#define LED_1V13          V13

#define AtoD_2V14         V14

//Relay #2
#define Switch_2V15       V15
#define TimerA_2V16       V16
#define TimerB_2V17       V17
#define LED_2V18          V18

#define AtoD_3V19         V19

//Relay #3
#define Switch_3V20       V20
#define TimerA_3V21       V21
#define TimerB_3V22       V22
#define LED_3V23          V23

#define AtoD_4V24         V24

//Relay #4
#define Switch_4V25       V25
#define TimerA_4V26       V26
#define TimerB_4V27       V27
#define LED_4V28          V28

#define Unassigned_V29    V29
#define Unassigned_V30    V30
#define Unassigned_V31    V31


//extern WidgetTerminal   oTerminal;    //Blynk Terminal
extern bool           bDebugLog;    //Used to limit number of printouts.

//Function protos
void      SetupWiFi(const char* pcRouterName, const char* pcRouterPW);
String    szWiFiStatus(wl_status_t status);
#ifdef OTA_SERVER
  void      SetupHttpServer(const char* acHostname,
                            ESP8266WebServer&        oHttpServer,
                            ESP8266HTTPUpdateServer& oHttpUpdateServer);
  void      HandleHttpServer(ESP8266WebServer& oHttpServer);
#endif  //OTA_SERVER
void      LogJustToSerial(String sLogline);
void      LogESPValues();
String    szLogLineHeader(long lLineCount);
String    szGetTime(long lMsec);
String    szAddZeros(int sValue, int sNumDigits);
String    szIPaddress(IPAddress oIP);


//Global variables
extern long                     lLineCount;       //Serial Monitor uses for clarity.
extern int32_t                  wHttpServerCount; //To allow logging every nth call at UPLOAD_FILE_WRITE
extern String                   szLogLine;

extern bool                     bWiFiOn;
extern bool                     bFirebaseOn;
extern bool                     bStartedOTA;

#ifdef OTA_SERVER
  extern ESP8266WebServer         oHttpServer;
  extern ESP8266HTTPUpdateServer  oHttpUpdateServer;
#endif  //OTA_SERVER

#ifndef NO_DISPLAY
	#ifdef USE_U8GLIB
		extern U8GLIB_DOGS102           u8g;
	#else
		//Reference display object fot the DOGS102-6 (102x64) display
		extern dog_1701 DOG;
	#endif	//USE_U8GLIB
#endif

  extern void LogToBoth(String szLogString);
  extern void LogToBoth(String szLogString, String szLogValue);
  extern void LogToBoth(String szLogString, int sLogValue);
  extern void LogToBoth(String szLogString, UINT32 uwLogValue);
  extern void LogToBoth(String szLogString, float fLogValue);
  extern void LogToBoth(String szLogString, double dLogValue);

  extern void LogToSerial(String szLogString);
  extern void LogToSerial(String szLogString, String szLogValue);
  extern void LogToSerial(String szLogString, int sLogValue);
  extern void LogToSerial(String szLogString, UINT32 uwLogValue);
  extern void LogToSerial(String szLogString, float fLogValue);
  extern void LogToSerial(String szLogString, double dLogValue);

extern const int       sUpButtonPin;
extern const int       sDownButtonPin;
extern const byte      cI2C_SDA_Pin;
extern const byte      cI2C_SCL_Pin;
extern const byte      cSPI_CmdData_Pin;
extern const byte      cSPI_MISO_Pin;
extern const byte      cSPI_MOSI_Pin;
extern const byte      cSPI_CLK_Pin;
extern const byte      cSPI_Select_Pin;
extern const int       sServoPin;
extern const byte      cBogusResetPin;
extern const byte      cHW_SPI;

#endif   //BECKLIB_H

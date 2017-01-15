//BeckLib.h, Nov 29, 2016
#ifndef BECKLIB_H
#define BECKLIB_H
#include <Streaming.h>
//#include <Firebase.h>
#ifdef ESP32                    //FromBeckESP32WiFiExample.ino from Blynk Examples
  #define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
  #include <WiFi.h>
  #include <WiFiClient.h>
  #include <BlynkSimpleEsp32.h>
#else //ESP32
  #include <ESP8266WiFi.h>
  #include <WiFiClient.h>
  #include <BlynkSimpleEsp8266.h>
  #if OTA_SERVER
    #include <ESP8266WebServer.h>
    #include <ESP8266mDNS.h>
    #include <ESP8266HTTPUpdateServer.h>
  #endif  //OTA_SERVER
  #ifndef ESP8266
    #define ESP8266
  #endif  //ESP8266
#endif  //ESP32 else

//#include <BeckFirebase.h>

#define NO_DISPLAY
#ifndef NO_DISPLAY
  #include <U8glib.h>
#endif

#define LOG0        szLogLineHeader(++lLineCount)
#define BLog        LogJustToSerial
#define BLogS       LogJustToSerial

static const long   lSerialMonitorBaud    = 115200;
static const long   lMsecPerDay           = 86400000;
static const long   lMsecPerHour          =  3600000;
static const long   lMsecPerMin           =    60000;
static const long   lMsecPerSec           =     1000;

void      SetupWiFi(const char* pcRouterName, const char* pcRouterPW);
String    szWiFiStatus(wl_status_t status);
#if OTA_SERVER
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
#if OTA_SERVER
  extern ESP8266WebServer         oHttpServer;
  extern ESP8266HTTPUpdateServer  oHttpUpdateServer;
#endif  //OTA_SERVER

#ifndef NO_DISPLAY
  extern U8GLIB_DOGS102           u8g;
#endif

extern const int       sUpButtonPin;
extern const int       sDownButtonPin;
extern const byte      cI2C_SDAPin;
extern const byte      cI2C_SCLPin;
extern const byte      cSPIMISOPin;
extern const byte      cSPIMOSIPin;
extern const byte      cSPICLKPin;
extern const byte      cSPISelectPin;
extern const int       sServoPin;

#endif   //BECKLIB_H

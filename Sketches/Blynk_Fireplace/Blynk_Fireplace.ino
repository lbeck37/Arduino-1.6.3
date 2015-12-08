/***** Blynk_Fireplace.ino *****
 * 12/07/15 Set up to work with inverting relay
 * 12/06/15 Copy from Blynk thermostat sketch.
 **************************************************************/

#include <Streaming.h>
#define BLYNK_PRINT Serial    // Comment out to disable prints and save space
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>

//Get Blynk Auth Token from the Blynk App, go to the Project Settings (nut icon).
//char acBlynkAuthToken[] = "55bce1afbf894b3bb67b7ea34f29d45a";   //LEDonD4 Blynk token
char acBlynkAuthToken[] = "0192b13c767c49e6a7fc8f3bbc5c8f7f";     //Fireplace Blynk token

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelgyro;

int16_t   ax, ay, az;
int16_t   gx, gy, gz;
int16_t   mx, my, mz;

float     fDegF;

#define LOG0      lLineCount++ << " " << millis()

//Define Virtual and Analog pin names
#define DegF_VPin1      V1
#define DegF_VPin2      V2
#define DegF_VPin3      V3
#define Flame_VPin5     V5
#define Relay_VPin10    V10

#define AnalogInputPin      A0

static const bool   bRelaysInverted   = true;
static const int    sOpen             = 0;
static const int    sClosed           = 1;
static int          sRelayClosed;
static int          sRelayOpen;
static int          sFlameState;
static int          sGyroTemperature;
static int          sFlameRelayPin    = 12;
static long         lLineCount        = 0;      //Serial Monitor uses for clarity. sRelayOpen
static long         lNumLoops         = 1;

void setup()
{
  Serial.begin(9600);
  Serial << LOG0 << " setup(): Initialized serial to 9600 baud" << endl;
  Wire.begin();
  accelgyro.initialize();

  // verify connection
  Serial << LOG0 << " setup(): Calling testConnection()" << endl;

  if (accelgyro.testConnection()) {
    Serial << LOG0 << " setup(): MPU6050 connection successful" << endl;
  }
  else {
    Serial << LOG0 << " setup(): MPU6050 connection failed" << endl;
  }

  Blynk.begin(acBlynkAuthToken, "dlinky", "Qazqaz11");
} //setup


void loop()
{
  Blynk.run();
} //loop


int sSetupRelay(){
  if (bRelaysInverted){
    sRelayOpen  = HIGH;
    sRelayClosed= LOW;
  } //if(bRelaysInverted)
  else{
    sRelayOpen  = LOW;
    sRelayClosed= HIGH;
  } //if(bRelaysInverted)else

  pinMode(sFlameRelayPin, OUTPUT);
  sSetRelay(sOpen);
  return 1;
} //sSetupRelay


int sSetRelay(int sRelaySetting){
  Serial << LOG0 << " sSetRelay: Received sRelaySetting= " << sRelaySetting << endl;
  if (sRelaySetting == sOpen){
    Serial << LOG0 << " sSetRelay: Call digitalWrite to OPEN relay on Pin " << sFlameRelayPin << endl;
    digitalWrite(sFlameRelayPin, sRelayOpen);
  } //if(sRelaySetting==sOpen)
  else{
    Serial << LOG0 << " sSetRelay: Call digitalWrite to CLOSE relay on Pin " << sFlameRelayPin << endl;
    digitalWrite(sFlameRelayPin, sRelayClosed);
  } //if(sRelaySetting==sOpen)else
  sFlameState= sRelaySetting;
  return 1;
} //sSetRelay


BLYNK_WRITE(Relay_VPin10){
  int sRelayValue= param.asInt();
  Serial << LOG0 << " BLYNK_WRITE(Relay_VPin10): Received Parameter= " << sRelayValue << endl;
  sSetRelay(sRelayValue);
  return;
} //BLYNK_WRITE(Relay_VPin10)


//BLYNK_READ(VPin) handles the call from a Widget requesting data for a Virtual Pin
BLYNK_READ(DegF_VPin1){
  Blynk.virtualWrite(DegF_VPin1, sGyroTemperature);
} //BLYNK_READ(DegF_VPin1)


BLYNK_READ(DegF_VPin2){
  float fDegF= fGetGyroDegF();
  Serial << LOG0 << " BLYNK_READ(DegF_VPin2): fGetGyroDegF() returned " << fDegF << endl;
  Blynk.virtualWrite(DegF_VPin2, fDegF);
} //BLYNK_READ(DegF_VPin2)


BLYNK_READ(DegF_VPin3){
  float fDegF= fGetGyroDegF();
  Serial << LOG0 << " BLYNK_READ(DegF_VPin3): fGetGyroDegF() returned " << fDegF << endl;
  Blynk.virtualWrite(DegF_VPin3, fDegF);
} //BLYNK_READ(DegF_VPin3)


BLYNK_READ(Flame_VPin5){
  Blynk.virtualWrite(Flame_VPin5, sFlameState);
} //BLYNK_READ(Flame_VPin5)


float fGetGyroDegF() {
  float fDegC;
  float fDegF;
  //accelgyro.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);
  sGyroTemperature= accelgyro.getTemperature();

  //fDegC= (sGyroTemperature / 340.0) + 36.53;    //Numbers from datasheet.
  fDegC= (sGyroTemperature / 340.0) + 33.0;
  fDegF= (fDegC * 1.8) + 32.0;

  return fDegF;
}  //fGetGyroDegF
//Last line.

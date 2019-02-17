//BeckMiniLib.cpp, Beck 2/16/19
#include <BeckMiniLib.h>
#include <BeckLogLib.h>
/*
#include <BeckNTPLib.h>
#include <NTPClient.h>
#include <Timezone.h>
*/

const long		lMsecPerSec					= 1000;
const long		lMsecPerMin					= 60 * lMsecPerSec;
const long		lMsecPerHour				= 60 * lMsecPerMin;
const long		lMsecPerDay					= 24 * lMsecPerHour;
const long   	lSerialMonitorBaud  = 115200;

//Digital Pins
#ifdef ESP32
  //BlynkBeck uses pins 4, 5, 15, 16
  //static const int       sSelectButton;
  //static const int       sBacklightPin;
  const int       sUpButtonPin       =  0;
  const int       sDownButtonPin     =  2;
  const byte      cI2C_SDA_Pin       =  26;
  const byte      cI2C_SCL_Pin       =  27;
  const byte      cSPI_A0CmdData_Pin =  4;
  const byte      cSPI_MISO_Pin      = 12;
  const byte      cSPI_MOSI_Pin      = 13;
  const byte      cSPI_CLK_Pin       = 14;
  const byte      cSPI_Select_Pin    = 15;
  const int       sServoPin          = 16;
#endif  //ESP32

//time_t          _lLocalTime;
unsigned long   ulLastTaskMsec        = 0;      //For checking time handling tasks

  //Local function prototypes
String  szPrintDigits         (int digits);


void CheckTaskTime(String szTask){
  unsigned long    ulMaxTaskMsec= lMsecPerSec / 2;  //Half second time limit before reporting task.
  unsigned long    ulNowMsec= millis();
  unsigned long    ulTaskMsec= ulNowMsec - ulLastTaskMsec;
  if (ulTaskMsec >  ulMaxTaskMsec){
    float fTaskSeconds= (float)ulTaskMsec / 1000.0;
    Serial << LOG0 << "CheckTaskTime(): " << szTask << " task took " << fTaskSeconds << " seconds"<< endl;
  } //
  ulLastTaskMsec= millis();
  return;
} //CheckTaskTime


void ClearTaskTime2(unsigned long* pulLastTaskMsec){
  if (pulLastTaskMsec){
    *pulLastTaskMsec= millis();
  }
  else{
    Serial << LOG0 << "ClearTaskTime2(): ERROR: Passed in NULL pointer" << endl;
  }
  return;
} //ClearTaskTime2


void CheckTaskTime2(String szTask, unsigned long* pulLastTaskMsec){
  unsigned long    ulMaxTaskMsec= lMsecPerSec / 2;  //Half second time limit before reporting task.
  unsigned long    ulNowMsec= millis();
  if (pulLastTaskMsec == NULL){
    pulLastTaskMsec= &ulLastTaskMsec;
  } //if (plLastTaskMsec==NULL)
  unsigned long    ulTaskMsec= ulNowMsec - *pulLastTaskMsec;

  if (ulTaskMsec >  ulMaxTaskMsec){
    float fTaskSeconds= (float)ulTaskMsec / 1000.0;
    Serial << LOG0 << "CheckTaskTime2(): " << szTask << " task took " << fTaskSeconds << " seconds"<< endl;
  } //
  *pulLastTaskMsec= millis();
  return;
} //CheckTaskTime2


/*
String szLogLineHeader(void){
	String 					szHeader					= "";
	String 					szThousanths			= "";
	unsigned long 	ulCurrentMillis		= millis();
	char						szDaysFloat[10];

	//Compute a float with N.NNN with a leading zero representing days uptime
	//Starts out "0.00", 0.01 of a day is 864 seconds or 14.4 minutes
	float		fDays	= ((float)ulCurrentMillis / (float)lMsecPerDay);

  szHeader += fDays;
  szHeader += " ";
#if 1
  _lLocalTime= lGetLocalTime();
  szHeader += szFormatTimeString(_lLocalTime);   //szFormatTimeString has a space at the end

  //Replace the space with a decimal point
  //Follow "." with the lowest 3 digits of the msec count
  int wNumChar= szHeader.length();
  szHeader.setCharAt((wNumChar - 1), '.');
  szThousanths= (ulCurrentMillis % lMsecPerSec);

  //Make sure it is 3 digits, pad wit zeros if not
  wNumChar= szThousanths.length();
	switch (wNumChar){
		case 0:
			szThousanths += "000";
			break;
		case 1:
			szThousanths += "00";
			break;
		case 2:
			szThousanths += "0";
			break;
		default:
			break;
	} //switch

  szHeader += szThousanths;
  szHeader += " ";				//Adds a trailing space
#endif
  return szHeader;
} //szLogLineHeader


time_t lGetLocalTime(void){
  //return (oMT_Timezone.toLocal (now(), &pTimeChangeRule));
  return oNTPClient.getEpochTime();
} //lGetLocalTime


String szFormatTimeString(time_t lLocalTime) {
  String szReturnString = "";
  szReturnString += szPrintDigits(hour(lLocalTime));
  szReturnString += ":";
  szReturnString += szPrintDigits(minute(lLocalTime));
  szReturnString += ":";
  szReturnString += szPrintDigits(second(lLocalTime));
  szReturnString += " ";
  return szReturnString;
} //szFormatTimeString


String szFormatDateString(time_t lLocalTime){
  String szReturnString = "";
  szReturnString += szPrintDigits(month(lLocalTime));
  szReturnString += "/";
  szReturnString += szPrintDigits(day(lLocalTime));
  szReturnString += "/";
  szReturnString += String(year(lLocalTime));
  return szReturnString;
} //szFormatDateString


String szPrintDigits(int digits) {
  // utility for digital clock display: prints preceding colon and leading 0
  String digStr = "";

  if (digits < 10)
    digStr += '0';
  digStr += String(digits);

  return digStr;
} //szPrintDigits


//szAddLeadingZeros() adds 1 or 2 zeros (depending on sNumDigits being 3 or not).
String szAddZeros(int sValue, int sNumDigits){
  String szReturn;
  if ((sNumDigits == 3) && (sValue < 100)){
    szReturn= "0";
  } //if((sNumDigits==3)&&(sValue<100)
  if (sValue < 10){
    szReturn += "0";
  } //if(lValue<10)
  szReturn += String(sValue);
  return szReturn;
} //szAddZeros
*/
//Last line.

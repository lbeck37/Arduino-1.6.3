//BeckMiniLib.cpp, Beck 1/30/19
#include <BeckMiniLib.h>
#include <BeckNTPLib.h>

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

time_t    _lLocalTime;

  //Local function prototypes
time_t  lGetLocalTime         (void);
String  szFormatTimeString    (time_t lBoiseTime);
String  szFormatDateString    (time_t lLocalTime);
String  szPrintDigits         (int digits);
//String  szFormatRawTime       (long lMsec){


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
  return (oMT_Timezone.toLocal (now(), &pTimeChangeRule));
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


/*
String szFormatRawTime(long lMsec){
  String  szString;
  int sDays    =    lMsec                                               / lMsecPerDay ;
  int sHours   =   (lMsec % lMsecPerDay)                                / lMsecPerHour;
  int sMinutes =  ((lMsec % lMsecPerDay) % lMsecPerHour)                / lMsecPerMin ;
  int sSeconds = (((lMsec % lMsecPerDay) % lMsecPerHour) % lMsecPerMin) / lMsecPerSec;
  int sMsec    =    lMsec % lMsecPerSec;
  szString = String(sDays) + ":";
  szString+= String(szAddZeros(sHours, 2)) + ":";
  szString+= String(szAddZeros(sMinutes, 2)) + ":";
  szString+= String(szAddZeros(sSeconds, 2)) + ".";
  szString+= String(szAddZeros(sMsec, 3)) + " ";  //Trailing blank.
  return szString;
} //szFormatRawTime
*/
//Last line.

//BeckMiniLib.cpp, Beck Dec 9, 2018
#include <BeckMiniLib.h>
#include <BeckNtpLib.h>

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

//long         lLineCount            = 0;      //Serial Monitor uses for clarity.

String szGetTime(long lMsec){
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
} //szGetTime


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


String szLogLineHeader(void){
	String 					szHeader					= "";
	unsigned long 	ulCurrentMillis		= millis();
	char						szThousanths[10];

	//Compute a float with N.NNN with a leading zero representing days uptime
	//Starts out "0.00" for quite a while for 0.001 of day to happen, 85 minutes?
	float		fDays	= ((float)ulCurrentMillis / (float)lMsecPerDay);

  //szHeader += ++lLineCount;
  szHeader += fDays;
  szHeader += " ";
  //szHeader += szGetTime(millis());
  szHeader += szFormatTimeString();		//szFormatTimeString has a space at the end

  //Replace the space with a decimal point
  //Follow "." with the lowest 3 digits of the msec count
  int wNumChar= szHeader.length();
  szHeader.setCharAt((wNumChar - 1), '.');
  //sprintf(szThousanths, "%03d", (ulCurrentMillis % lMsecPerSec));
  //szHeader += szThousanths;
  szHeader += (ulCurrentMillis % lMsecPerSec);
  szHeader += " ";
  return szHeader;
} //szLogLineHeader
//Last line.

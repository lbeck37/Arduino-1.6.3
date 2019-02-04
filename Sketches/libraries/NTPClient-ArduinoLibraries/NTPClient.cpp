/** NTPClient.cpp 2/3/19c Beck
 * The MIT License (MIT)
 * Copyright (c) 2015 by Fabrice Weinberg
 */

#include <BeckMiniLib.h>
#include "NTPClient.h"

NTPClient::NTPClient(UDP& udp) {
  this->_udp            = &udp;
}

NTPClient::NTPClient(UDP& udp, long timeOffset) {
  this->_udp            = &udp;
  this->_timeOffset     = timeOffset;
}

NTPClient::NTPClient(UDP& udp, const char* poolServerName) {
  this->_udp            = &udp;
  this->_poolServerName = poolServerName;
}

NTPClient::NTPClient(UDP& udp, const char* poolServerName, long timeOffset) {
  this->_udp            = &udp;
  this->_timeOffset     = timeOffset;
  this->_poolServerName = poolServerName;
}

NTPClient::NTPClient(UDP& udp, const char* poolServerName, long timeOffset, unsigned long updateInterval) {
  this->_udp            = &udp;
  this->_timeOffset     = timeOffset;
  this->_poolServerName = poolServerName;
  this->_updateInterval = updateInterval;
}


void NTPClient::begin() {
  this->begin(NTP_DEFAULT_LOCAL_PORT);
} //begin

void NTPClient::begin(int port) {
  this->_port = port;
  this->_udp->begin(this->_port);
  this->_udpSetup = true;
} //begin(int)


bool NTPClient::forceUpdate() {
  int   wTimeoutSec       = 1;
  int   wDelayMsec        = 10;
  int   wTimeoutCountMax  = (wTimeoutSec * 1000) / wDelayMsec;
  int   wTimeoutCount     = 0;
  int   wBytesRead        = 0;
  bool  bSuccess          = false;
  bool  bDone             = false;

  unsigned long   ulLocalTime= this->getEpochTime();
  Serial << LOG0 << "NTPClient::forceUpdate() Request network time, Date= " << szFormatDateString(ulLocalTime) << endl;
  this->sendNTPPacket();

  // Wait till data is there or timeout...
  while (!bDone){
    delay(wDelayMsec);
    wBytesRead = this->_udp->parsePacket();
    if (wBytesRead > 0){
      bSuccess= true;
      bDone   = true;
    } //if (wBytesRead>0)
    else{
      if (wTimeoutCount > wTimeoutCountMax){
        bSuccess= false;
        bDone   = true;
      } //if(wTimeoutCount>wTimeoutCountMax)
      else{
        wTimeoutCount++;
      } //if(wTimeoutCount>wTimeoutCountMax)else
    } //if (wBytesRead>0)else
  } //while(!bDone)

  if (bSuccess){
    this->_lastUpdate= (millis() - (10 * (wTimeoutCount + 1))); // Account for delay in reading the time

    this->_udp->read(this->_packetBuffer, NTP_PACKET_SIZE);

    unsigned long highWord = word(this->_packetBuffer[40], this->_packetBuffer[41]);
    unsigned long lowWord  = word(this->_packetBuffer[42], this->_packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;

    this->_currentEpoc= (secsSince1900 - SEVENZYYEARS);
  } //if(bSuccess)

  Serial << LOG0 << "NTPClient::forceUpdate() Returning " << bSuccess << endl;
  return bSuccess;
} //forceUpdate


bool NTPClient::update() {
  if ((millis() - this->_lastUpdate >= this->_updateInterval)     // Update after _updateInterval
    || this->_lastUpdate == 0) {                                // Update if there was no update yet.
    if (!this->_udpSetup) this->begin();                         // setup the UDP client if needed
    return this->forceUpdate();
  }
  return true;
} //update


unsigned long NTPClient::getEpochTime() const {
  return this->_timeOffset + // User offset
         this->_currentEpoc + // Epoc returned by the NTP server
         ((millis() - this->_lastUpdate) / 1000); // Time since last update
} //getEpochTime


int NTPClient::getDay() const {
  return (((this->getEpochTime()  / 86400L) + 4 ) % 7); //0 is Sunday
}
int NTPClient::getHours() const {
  return ((this->getEpochTime()  % 86400L) / 3600);
}
int NTPClient::getMinutes() const {
  return ((this->getEpochTime() % 3600) / 60);
}
int NTPClient::getSeconds() const {
  return (this->getEpochTime() % 60);
}

String NTPClient::getFormattedTime() const {
  unsigned long rawTime = this->getEpochTime();
  unsigned long hours = (rawTime % 86400L) / 3600;
  String hoursStr = hours < 10 ? "0" + String(hours) : String(hours);

  unsigned long minutes = (rawTime % 3600) / 60;
  String minuteStr = minutes < 10 ? "0" + String(minutes) : String(minutes);

  unsigned long seconds = rawTime % 60;
  String secondStr = seconds < 10 ? "0" + String(seconds) : String(seconds);

  return hoursStr + ":" + minuteStr + ":" + secondStr;
}

void NTPClient::end() {
  this->_udp->stop();

  this->_udpSetup = false;
}

void NTPClient::setTimeOffset(int timeOffset) {
  this->_timeOffset     = timeOffset;
}

void NTPClient::setUpdateInterval(unsigned long updateInterval) {
  this->_updateInterval = updateInterval;
}

void NTPClient::setPoolServerName(const char* poolServerName) {
    this->_poolServerName = poolServerName;
}

void NTPClient::sendNTPPacket() {
  // set all bytes in the buffer to 0
  memset(this->_packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  this->_packetBuffer[0]   = 0b11100011;    // LI, Version, Mode
  this->_packetBuffer[1]   = 0;             // Stratum, or type of clock
  this->_packetBuffer[2]   = 6;             // Polling Interval
  this->_packetBuffer[3]   = 0xEC;          // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  this->_packetBuffer[12]  = 49;
  this->_packetBuffer[13]  = 0x4E;
  this->_packetBuffer[14]  = 49;
  this->_packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  this->_udp->beginPacket(this->_poolServerName, 123); //NTP requests are to port 123
  this->_udp->write(this->_packetBuffer, NTP_PACKET_SIZE);
  this->_udp->endPacket();
  return;
} //sendNTPPacket
//Last line.

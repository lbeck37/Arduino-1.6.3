/*05/20/15 Beck- Create file as Lbeck37.h from Powershift.ino
 */
#ifndef LBECK37_H
#define LBECK37_H

#include <Arduino.h>
#include <Streaming.h>  //Why can I include this here only if I already included it?

#define INT8           int8_t
#define UINT8         uint8_t
#define INT16         int16_t
#define UINT16       uint16_t
#define INT32         int32_t
#define UINT32       uint32_t

static INT16 sLC    = 0;  //Serial Monitor Line Count, for clarity.

//Protos
INT16    sSetupSerialStreaming   (void);
INT16    sFreeRam                (void);

INT16 sSetupSerialStreaming() {
   Serial.begin(9600);
   Serial << sLC++ << "sSetupSerialStreaming() Begin" << endl;
   Serial << sLC++ << "Free Ram= " << sFreeRam() << endl;
   return 1;
}  //sSetupSerialStreaming


//sFreeRam() returns the number of bytes currently free in RAM.
int sFreeRam(void)
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
}  //sFreeRam
#endif   //LBECK37_H
//Last line.
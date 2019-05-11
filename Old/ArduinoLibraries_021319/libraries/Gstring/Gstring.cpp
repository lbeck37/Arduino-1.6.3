#include "Gstring.h" //include the declaration for this class

const byte LED_PIN = 13; //use the LED @ Arduino pin 13

//<<constructor>> setup the LED, make pin 13 an OUTPUT
Gstring::Gstring(){
    pinMode(LED_PIN, OUTPUT); //make that pin an OUTPUT
}

//<<destructor>>
Gstring::~Gstring(){/*nothing to destruct*/}

//blink the LED in a period equal to paramterer -time.
void Gstring::blink(int time){
   on();          //turn LED on
   delay(time/2);  //wait half of the wanted period
   off();         //turn LED off
   delay(time/2);  //wait the last half of the wanted period
}

//turn the LED on
void Gstring::on(){
   digitalWrite(LED_PIN,HIGH); //set the pin HIGH and thus turn LED on
}

//turn the LED off
void Gstring::off(){
   digitalWrite(LED_PIN,LOW); //set the pin LOW and thus turn LED off
}

#if 1
//*This makes Streaming library work in this class
void Gstring::write(uint8_t usChar){
   //Doing nothing right now.
}
#endif


#if 0
#include "LED13.h" //include the declaration for this class

const byte LED_PIN = 13; //use the LED @ Arduino pin 13

//<<constructor>> setup the LED, make pin 13 an OUTPUT
LED13::LED13(){
    pinMode(LED_PIN, OUTPUT); //make that pin an OUTPUT
}

//<<destructor>>
LED13::~LED13(){/*nothing to destruct*/}

//turn the LED on
void LED13::on(){
   digitalWrite(LED_PIN,HIGH); //set the pin HIGH and thus turn LED on
}

//turn the LED off
void LED13::off(){
   digitalWrite(LED_PIN,LOW); //set the pin LOW and thus turn LED off
}

//blink the LED in a period equal to paramterer -time.
void LED13::blink(int time){
   on();          //turn LED on
   delay(time/2);  //wait half of the wanted period
   off();         //turn LED off
   delay(time/2);  //wait the last half of the wanted period
}
#endif
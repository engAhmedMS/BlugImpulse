#include "Arduino.h"
#include "esp8266_touch.h"

touchSensor::touchSensor(char scl, char sda, char rst)
{
   SCL_pin = scl;
   SDA_pin = sda;
   RST_pin = rst;
   keyboardSize = 8;
}


void touchSensor::attach()
{
    pinMode(SCL_pin, OUTPUT);
    pinMode(RST_pin, OUTPUT);
    pinMode(SDA_pin, INPUT);

}

/*
 * https://forum.mysensors.org/topic/5069/ttp226-module-touch-panel-serial-mode-library-for-mysensors/4
 */
void touchSensor::read(bool* keyState)
{
    // fetch serial data, store in keyState
    digitalWrite(SCL_pin, LOW);                             // get first bit
    delayMicroseconds(100);                                 // allow for stable input
    digitalWrite(SCL_pin, HIGH);
    for(int i = 0; i < keyboardSize; i++)}
    {                  // read all keys 
        keyState[i] = (digitalRead(SDA_pin) == LOW) ;       // set key if pressed
        digitalWrite(SCL_pin, LOW);                         // get first bit
        delayMicroseconds(100);                             // allow for stable input
        digitalWrite(SCL_pin, HIGH);
        delayMicroseconds(100);
    } 
}






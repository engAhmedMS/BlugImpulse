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

void touchSensor::reset(){
    digitalWrite(RST_pin,HIGH);
    delayMicroseconds(PROPER_DELAY);
    digitalWrite(RST_pin,LOW);
}

#define SAFE
#ifndef SAFE
void touchSensor::read(bool* keyState)
{
    // reset is important to always get the pins in order
    reset();
    for(int i=0;i<8;i++){
        // its more safe to read the data after the rising edge
        digitalWrite(SCL_pin,HIGH);
        keyState[i] = (digitalRead(SDA_pin) == LOW);
        delayMicroseconds(PROPER_DELAY);
        digitalWrite(SCL_pin,LOW);
        delayMicroseconds(PROPER_DELAY);
    }
    // if the reading is zeros then we should keep the last reading ...
}

// note this is another implemtation 
#else
void touchSensor::read(char KeyState){
    reset();
    char tmp = 0;
    for(int i=0;i<8;i++){
        // its more safe to read the data after the rising edge
        digitalWrite(SCL_pin,HIGH);
        tmp |= (digitalRead(SDA_pin) << i);
        delayMicroseconds(PROPER_DELAY);
        digitalWrite(SCL_pin,LOW);
        delayMicroseconds(PROPER_DELAY);
    }
    // to check if there any change in the level or just return the last state "level"
    if(tmp){
        // after declaring the lastkeystate in the class definition
        // to get the highst one bit from the left
        for(int i=0;i<8;i++) lastKeyState = ((tmp >> i) & 1) ? i : lastKeyState;
    }
    Keystate = lastKeystate;
    // this explicity will return the level you want 
}

#endif







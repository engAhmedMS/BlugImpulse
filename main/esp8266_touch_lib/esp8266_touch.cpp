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






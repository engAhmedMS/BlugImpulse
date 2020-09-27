#include "Arduino.h"
#include "esp32_touch.h"

touchSensor::touchSensor(char pinNum, int thr)
{
    threshold = thr;
    pinNumber = pinNum;
}


void touchSensor::attach()
{
    Number_sensors++;
}

int touchSensor::read()
{
    value = touchRead(pinNumber); 
    return value; 
}

bool touchSensor::pressed()
{
    value = touchRead(pinNumber);
    bool ret;
    if(value < threshold)
    {
        ret = 1;  
    }
    else
    {
        ret = 0;  
    }
    return ret;
}

void touchSensor::setThreshold(int thr)
{
    threshold = thr;
}

int touchSensor::getThreshold()
{

    return threshold;
}




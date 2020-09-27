#include "Arduino.h"
#include "esp32_touch.h"

#define SET_BIT(REG, PIN)   (REG |= (1<<PIN))
#define CLR_BIT(REG, PIN)   (REG &= (~(1<<PIN)))

touchSensor::touchSensor(char* arr_pins, int* arr_reads, int thr, char num_sens)
{
    threshold = thr;
    pins = arr_pins;
    reads = arr_reads;
    Number_sensors = num_sens; //sizeof(arr_pins);
}


void touchSensor::attach()
{
    Number_sensors=Number_sensors;
}

int touchSensor::read()
{
    for(int i=0; i<Number_sensors; i++)
    {
        reads[i] = touchRead(pins[i]);
    }
    return 1; 
}

int touchSensor::pressed()
{
    for(int i=0; i<Number_sensors; i++)
    {
        if( touchRead(pins[i]) < threshold )
        {
            SET_BIT(press, i);
        }
        else
        {
            CLR_BIT(press, i);
        }
    }
    
    return press;
}

void touchSensor::setThreshold(int thr)
{
    threshold = thr;
}

int touchSensor::getThreshold()
{

    return threshold;
}




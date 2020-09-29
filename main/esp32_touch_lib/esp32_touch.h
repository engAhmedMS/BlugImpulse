#ifndef ESP32_TOUCH_H__
#define ESP32_TOUCH_H__
#include "Arduino.h"

#define MAX_NUMBER_OF_SENSORS 10

class touchSensor
{
  private:
    int  threshold;
    char Number_sensors;
    int* reads;
    int  press;
    char* pins;

  
  public:
    touchSensor(char* arr_pins, int* arr_reads, int thr, char num_sens);
    
    void attach();
    
    int read();
    
    int pressed();

    void setThreshold(int thr);
    
    int getThreshold();
  
  
  
  };
#endif

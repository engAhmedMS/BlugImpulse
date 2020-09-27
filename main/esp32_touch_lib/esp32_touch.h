#ifndef ESP32_TOUCH_H__
#define ESP32_TOUCH_H__
#include "Arduino.h"
class touchSensor
{
  private:
    char pinNumber;
    int  threshold;
    char Number_sensors;
    char value;

  
  public:
    touchSensor(char pinNum, int thr);
    
    void attach();
    
    int read();
    
    bool pressed();

    void setThreshold(int thr);
    
    int getThreshold();
  
  
  
  };
#endif

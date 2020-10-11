#ifndef ESP8266_TOUCH_H__
#define ESP8266_TOUCH_H__
#include "Arduino.h"
#define PROPER_DELAY 62
typedef enum
{
  DIRECT_MODE,
  SERIAL_MODE,
  MATRIX_MODE,

}TOUCH_SENSOR_MODE;

class touchSensor
{
  private:
    char SCL_pin;
    char SDA_pin;
    char RST_pin;
    char keyboardSize;


  
  public:
    touchSensor(char SCL_pin, char SDA_pin, char RST_pin);
    
    void attach();

    void reset();
    
    void read(bool* keyState);
  
  
  
  };
#endif

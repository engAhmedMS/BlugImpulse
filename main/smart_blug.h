#ifndef SMART_BLUG_H__
#define SMART_BLUG_H__

#include "esp8266_touch.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <ESP8266Ping.h>
#include <ESP8266HTTPClient.h>
#include "HAL/SN74HC595/SN74HC595_FUNs.h"

//Macro for debuggong and tracing code.
#define PRINT_TOUCH() for(int i=0; i<NUMBER_TOUCH_SENSORS; i++)\
                        {\
                          Serial.print(touch_reads[i]);\
                          Serial.print("  -- ");\
                        }\
                        Serial.println();

typedef enum
{
  ACCESS_POINT,
  STATION,
}WIFI_SOURCE;
WIFI_SOURCE wifi_source = STATION;
 
//PWM parameters
#define PWM_MAX (255)
#define PWM_PIN (23)
//esp8266 touch parameters
//using pins between GPIO6 to GPIO11 introduced a bug
//the controller stucks becaus of that and resets by wtd timer
#define NUMBER_TOUCH_SENSORS 8
#define TOUCH_SCL_PIN 12
#define TOUCH_SDA_PIN 13
#define TOUCH_RST_PIN 14
bool touch_reads[NUMBER_TOUCH_SENSORS];
touchSensor touch(TOUCH_SCL_PIN, TOUCH_SDA_PIN, TOUCH_RST_PIN);

//led Shift register parameters
#define Ser_LED    0
#define Rclk_LED   1
#define Srclk_LED  3
#define NUMBER_LEDS 8


//Relay Shift register parameters
#define Ser    2
#define Rclk   4
#define Srclk  5
#define NUMBER_RELAYS 3

void led_display(SN74HC595* LEDS, char level);

#endif
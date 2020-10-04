#ifndef SMART_BLUG_H__
#define SMART_BLUG_H__
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "esp8266_touch.h"
#include <EEPROM.h>
#include "HAL/SN74HC595/SN74HC595_FUNs.h"

//Relay Shift register parameters
#define Ser    2
#define Rclk   4
#define Srclk  5
#define NUMBER_RELAYS 3
//touch sensor parameters
#define NUMBER_TOUCH_SENSORS 8
//PWM parameters
#define PWM_MAX (255)
#define PWM_PIN (23)
//Helper macro function
#define READ_BIT(REG, PIN) ((REG>>PIN)&1)
//helper define to set max time for trying to connect esp as station
#define MAX_CONNECTION_TIME 10

#define BUTTON 3
typedef enum
{
    NOT_CONNECTED,
    CONNECTED,

}CONNECTION_STATE;

typedef enum
{
  NO_SOURCE,
  STATION,
  ACCESS_POINT,
}WIFI_SOURCE;

CONNECTION_STATE station_init(const char* ssid, const char* password, int max_time_s, WIFI_SOURCE* wifi_source);
int get_port(const char* ssid, const char* password,HTTPClient* http);
int get_data(HTTPClient* http);
int end_connection(HTTPClient* http);
int get_data();
void splitData(String ps, uint8_t* RELAYS);
void Restore_Session();
void UPDATE_RELAYS(uint8_t* RELAYs);
int update_changes();
String form(int *data, int n);
char slider(bool* arr_read);
IPAddress accessPoint_init(const char* ssid, const char* password, WIFI_SOURCE* wifi_source);


#endif

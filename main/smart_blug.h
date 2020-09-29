#ifndef SMART_BLUG_H__
#define SMART_BLUG_H__
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include "esp32_touch.h"
#include <EEPROM.h>
#include "HAL/SN74HC595/SN74HC595_FUNs.h"
//Relay Shift register parameters
#define Ser    2
#define Rclk   4
#define Srclk  5
#define NUMBER_RELAYS 3
//touch sensor parameters
#define NUMBER_TOUCH_SENSORS 8
#define THRESHOLD  40
//PWM parameters
#define PWM_CHANNEL (0)
#define PWM_FREQ  (5000)
#define PWM_RESOLUTION  (8)
#define PWM_MAX (255)
#define LED_PIN (23)
//Helper macro fumction
#define READ_BIT(REG, PIN) ((REG>>PIN)&1)
//helper define to set max time for trying to connect esp as station
#define MAX_CONNECTION_TIME 10

int changes[NUMBER_RELAYS] = {0, 0, 0}; // you have to update this array once the state is changeged

//ESP32 as a station
const char *ERROR_MSG = "Error occured on connectiong the server";
const char *INIT_CONNECTION = "http://impulses-iot.herokuapp.com/?action=init-connection&username=tmp&password=1234";


String UPDATE_REQ = "http://impulses-iot.herokuapp.com/?action=get-state&port=";          // +PORT
String END_CONNECTION = "http://impulses-iot.herokuapp.com/?action=end-connection&port="; // +PORT
String UPDATE_CHANGES = "http://impulses-iot.herokuapp.com/?action=update-changes&port="; // +port + changes
String GET_CHANGES = "http://impulses-iot.herokuapp.com/?action=get-changes&port=";       // +port
String PORT = "";
String DATA_OUTPUT = "";

boolean PORT_ADDED = false;
boolean SERVER_CONN = false;

typedef enum
{
    NOT_CONNECTED,
    CONNECTED,

}CONNECTION_STATE;

CONNECTION_STATE station_init(const char* ssid, const char* password, int max_time_s);
int get_port(const char* ssid, const char* password,HTTPClient* http);
int get_data(HTTPClient* http);
int end_connection(HTTPClient* http);
int get_data();
void splitData(String ps, uint8_t* RELAYS);
void Restore_Session();
void UPDATE_RELAYS(uint8_t* RELAYs);
int update_changes();
String form(int *data, int n);
char slider(int p);
IPAddress accessPoint_init(const char* ssid, const char* password);


#endif
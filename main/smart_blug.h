#ifndef SMART_BLUG_H__
#define SMART_BLUG_H__

#include <ESP8266WiFi.h>      
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include "HAL/SN74HC595/SN74HC595_FUNs.h"
#include "esp8266_touch.h"
//OTA
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

typedef enum
{
  NOT_CONNECTED,
  CONNECTED,
}WIFI_STATUS;

typedef enum
{
  ACCESS_POINT,
  STATION,
}WIFI_SOURCE;

/**
 * @brief <initStation>
 *
 * <Disc> this function is responsible for connecting MCU with WIFI netwotk as station
 *  
 * @parameter: const char *  <ssid>       <SSID "name" of the network as string>
 * @parameter: const char *  <password>   <password of the network as string>
 * @parameter: int           <timeout>    <time in milli-seconds for trying to connect to network, if passed connection fails>
 *
 * @return <bool> 1: connected succesfuly
 *                0: failed to connect
 *                
 * @future: - adding number of trials 
 */ 
WIFI_STATUS initStation(const char* ssid, const char* password, int timeout);

/**
 * @brief <root>
 *
 * <Disc> this function is responsible for creating simple webpage for testing
 *  
 * @parameter: <void>
 *
 * @return <void> 
 * 
 * @Note: function is not studied yet
 * 
 * @dependace: HOME global variable, server object
 * 
 * @hardcoded: 200: HTTP status means (Ok)
 * 
 */
void root();

/**
 * @brief <initAccessPoint>
 *
 * <Disc> this function is responsible for connecting MCU with WIFI netwotk as AccessPoint
 *  
 * @parameter: const char *     <ssid_ac>       <SSID "name" of the network as string>
 * @parameter: const char *     <password_ac>   <password of the network as string>
 * @parameter: ESP8266WebServer <server>        <webserver object that listens for HTTP request on port>
 * 
 * @return <IPAddress> Address of IP to connect
 * 
 * @dependace: this function is dependant on root() which creats simple web page, and server global variable
 * 
 * @future: modfing it to be indebendant
 */
IPAddress initAccessPoint(String ssid_ac, String pw_ac, ESP8266WebServer &server);

/**
 * @brief <parse_data>
 *
 * <Disc> simple parseing function
 *  
 * @parameter: String   <data>         <the incomming data is in the form of CSV  ---> 0,0,1>
 * @parameter: char*    <pin_states>   <states of pins extracted from data>
 * 
 * @return <int> indication that process is complete
 * 
 * @future: enhancing parsing process due to chnges in data recived from srver
 */
int parse_data(String data, const char* pin_states);

/**
 * @brief <slider>
 *
 * <Disc> fundtion that finds the pressed level of touch slider, and generates PWM signal depends on the level
 *  
 * @parameter: bool*   <arr_read>               <array carries reads from touch slider componnents>
 * @parameter: char    <number_touch_sensors>   <number of touch slider components>
 * @parameter: char    <pwm_pin>                <pin attached to PWM to control dimmer circuit>
 * @parameter: int     <pwm_max>                <max value of PWM used or provided by controller>
 * 
 * @return <char> detected level
 * 
 */
char slider(bool* arr_read, char number_touch_sensors, char pwm_pin, int pwm_max);

/**
 * @brief <led_display>
 *
 * <Disc> display touch sensor state on LEDs via shift register
 *  
 * @parameter: SN74HC595*   <LEDS>          <shift register object connected to leds>
 * @parameter: char         <level>         <state of touch slider>
 * @parameter: char         <number_leds>   <number of leds used for display>
 * 
 * @return <void> 
 * 
 * @future: display special codes on those LEDs
 */
void led_display(SN74HC595* LEDS, char level, char number_leds);

/**
 * @brief <get_data>
 *
 * <Disc> display touch sensor state on LEDs via shift register
 *  
 * @parameter: HTTPClient&    <http>    <http object passed by refernce to listen to server>
 * @parameter: const String&  <url>     <url of server>
 * @parameter: const String&  <grt_req> <regust data from server>
 * 
 * @return <int> indication that process is complete or nit
 * 
 */
int get_data(HTTPClient& http, const String& url, const String& grt_req, String& data);
#endif

// this differs from the other example in interperting the changes of the pins
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
 
const char *ERROR_MSG = "Error occured on connected the server";
const char *ssid      = "ahmed mohammed";      // adding the username
const char *password  = "am0113099454";      // adding the password 
 
const char *ssid_ac = "test1";
const char *pw_ac   = "11111111";
const char *HOME    = "<h1> Hello , world!</h1>";
const char *IP      = "www.impulses-iot.herokuapp.com";
const int timeout   = 5000;  
 
 
String URL      = "http://i...content-available-to-author-only...p.com/" ;               //"http://i...content-available-to-author-only...p.com/"; 
String ROOM     = "1111";
String USERNAME = "test";
String PASSWORD = "1111";
String AUTH     = "&username=" + USERNAME + "&password=" + PASSWORD + "&room=" + ROOM;
String GET_REQ  = "?action=get"  + AUTH;
String POST_REQ = "?action=post" + AUTH + "&changes=";       // + changes made to be sent to the server 
String data     = "";
String changes  = "";

typedef enum
{
  ACCESS_POINT,
  STATION,
}WIFI_SOURCE;
WIFI_SOURCE wifi_source = STATION;
 
HTTPClient http;
ESP8266WebServer server(80);
//PWM parameters
#define PWM_MAX (255)
#define PWM_PIN (23)
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
//esp8266 touch parameters
//using pins between GPIO6 to GPIO11 introduced a bug
//the controller stucks becaus of that and resets by wtd timer
#define NUMBER_TOUCH_SENSORS 8
#define TOUCH_SCL_PIN 12
#define TOUCH_SDA_PIN 13
#define TOUCH_RST_PIN 14
bool touch_reads[NUMBER_TOUCH_SENSORS];
touchSensor touch(TOUCH_SCL_PIN, TOUCH_SDA_PIN, TOUCH_RST_PIN);
uint8_t L_RELAY_STATE[NUMBER_RELAYS];
uint8_t N_RELAY_STATE[NUMBER_RELAYS];
SN74HC595 RELAYS;
SN74HC595 LEDS;

char pin_states[3];


void setup(){
  Serial.begin(115200);
  WiFi.begin(ssid,password);
  Serial.print("\n\nConntecting");
  int cur = millis();
  while(WiFi.status() != WL_CONNECTED){
    delay(100);
    Serial.print(".");
    if(millis() - cur >= timeout){
      Serial.println("canno't connect to the network :(");
      Serial.println("init the access point");
      initAccessPoint(ssid_ac, pw_ac);   
      break;
    }
  }
  Serial.println("\nConnected to the network");

  touch.attach();
 
}
 
void loop(){
 
  if(WiFi.status() == WL_CONNECTED && (wifi_source==STATION)){
    //if(!Ping.ping(IP)){
    //  initAccessPoint();
    //  goto a;
    //}
    Serial.println("Getting data from the server __room__ " + ROOM);
    // getting the data to the global varible data
    int res = get_data();
    if(!res){ 
      Serial.println(ERROR_MSG);
      initAccessPoint(ssid_ac, pw_ac);
      wifi_source = ACCESS_POINT;
      goto a;
    }
    goto b;
  }
 
 
  // handle the requests of the access pointi
  a:server.handleClient();  
  b:;

  touch.read(touch_reads);
  PRINT_TOUCH();
  slider(touch_reads);
}
 
int get_data(){
  if(WiFi.status() == WL_CONNECTED){
    Serial.println("sending GET request to the server..");
    http.begin( URL + GET_REQ );
    int HTTP_CODE_STATUS = http.GET();
    if(HTTP_CODE_STATUS > 0){
      data = http.getString();
      Serial.println(" incominng data :: " + data );
      http.end();
      return 1;
    }
    http.end();
  }
  return 0;
}

// note : you can add a new route or just a function to handle a request to retry to connect to the internet
void root(){
  Serial.println("GET request to /");
  server.send(200,"text/html",HOME);
}
 
IPAddress initAccessPoint(String ssid_ac, String pw_ac){
  WiFi.softAP(ssid_ac,pw_ac);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("access point active on IP :: ");
  Serial.print(myIP);
  server.on("/",root);
  server.begin();
  return myIP;
}

 
//int post_data(){
//    if(WiFi.status() == WL_CONNECTED){
//        Serial.println("sending POST request to the server..");
//        if(changes == "") pack_data();
//        http.begin( URL + POST_REQ + changes);
//        int HTTP_CODE_STATUS = http.GET();
//        if(HTTP_CODE_STATUS > 0){
//            Serial.println(" changes posted successfully ");
//            http.end();
//            return 1;
//        }
//        http.end();
//    }
//    return 0;
//}
 
// simple parseing function ,
// the incomming data is in the form of CSV  ---> 0,0,1
int parse_data(){
    char tmp[3],ptr = 0;
    for(int i=0;i<data.length();i++)
    {
        if(data[i] == ',') ptr++;
        else tmp[ptr] = data[i];
    }
    for(int i=0;i<3;i++)
    {
        pin_states[i] = tmp[i] - '0';
    }
    Serial.println("data parsed successfully ..");
    return 1;
}
 
//int pack_data(){
//    changes = "";
//    for(int i=0;i<3;i++)
//    changes += String(pin_states[i]) + ( i==2 ? "" : ",");
//    Serial.println(changes);
//    Serial.println("data packed successfully ..");
//    return 1;
//}
//
//int update(){
//    int nz = 0;
//    for(int i=0;i<3;i++){
//        if(toggle[i]){
//            pin_states[i] = pin_states[i] == 1 ? 0 : 1;
//            toggle[i] = 0;
//        }else nz++;
//    }
//    // check if ther is any updates
//    if(nz == 3) return 0;
//    // order them according to the most priority
//    write();
//    pack_data();
//    post_data();
//    return 1;
//}
//


//int write(){
//    for(int i=0;i<3;i++) digitalWrite(pins[i],pin_states[i]);
//    return 1;
//}

char slider(bool* arr_read)
{
  int level=NUMBER_TOUCH_SENSORS-1;
  for(; level>=0; level--)
  {
    if(arr_read[level])
    {
      analogWrite(PWM_PIN,(level*PWM_MAX/(NUMBER_TOUCH_SENSORS-1) ) ); 
      break;
    }
  }
  return level;  
}

void led_display(SN74HC595* LEDS, char level)
{
  int i;
  for(i = 0; i < level; i++)
  {
      SN74HC595_Write(LEDS, i, HIGH);
  }
  for(; i< NUMBER_LEDS; i++)
  {
      SN74HC595_Write(LEDS, i, LOW);
  }
}

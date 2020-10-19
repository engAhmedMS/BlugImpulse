#include "smart_blug.h"
ESP8266WebServer server(80);                 //create a webserver object that listens for HTTP request on port
HTTPClient http;

#define SLEEP_TIME  1e6
const char *ssid      = "ahmed mohammed";    // adding the username
const char *password  = "am0113099454";      // adding the password 
const int   timeout   = 5000;  

const char *ssid_ac = "test1";
const char *pw_ac   = "11111111";
const char *HOME    = "<h1> Hello , world!</h1>";

//parsing_data() parameters
String data     = "";
char pin_states[3];

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
#define Ser_LED    2
#define Rclk_LED   3
#define Srclk_LED  4
#define NUMBER_LEDS 8
SN74HC595 LEDS;

//get_data() parameters
String URL      = "http://i...content-available-to-author-only...p.com/" ;               //"http://i...content-available-to-author-only...p.com/"; 
String ROOM     = "1111";
String USERNAME = "test";
String PASSWORD = "1111";
String AUTH     = "&username=" + USERNAME + "&password=" + PASSWORD + "&room=" + ROOM;
String GET_REQ  = "?action=get"  + AUTH;
const char *ERROR_MSG = "Error occured on connected the server";

//variable to indicate if wifi connected as station or access point
WIFI_SOURCE wifi_source = STATION;

void setup() 
{
  Serial.begin(115200);  
  SN74HC595_INIT_PIN(&LEDS, SER, Ser_LED);
  SN74HC595_INIT_PIN(&LEDS, RCLK, Rclk_LED);
  SN74HC595_INIT_PIN(&LEDS, SRCLK, Srclk_LED);
  SN74HC595_INIT(&LEDS);
  bool wifi_status = initStation(ssid, password, timeout);
  if(wifi_status!= CONNECTED)
  {
    initAccessPoint(ssid_ac, pw_ac, server);
    start_server();
  }
  //OTA
  else
  {
      ArduinoOTA.onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH) {
        type = "sketch";
      } else { // U_FS
        type = "filesystem";
      }
  
      // NOTE: if updating FS this would be the place to unmount FS using FS.end()
      Serial.println("Start updating " + type);
    });
    ArduinoOTA.onEnd([]() {
      Serial.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) {
        Serial.println("Auth Failed");
      } else if (error == OTA_BEGIN_ERROR) {
        Serial.println("Begin Failed");
      } else if (error == OTA_CONNECT_ERROR) {
        Serial.println("Connect Failed");
      } else if (error == OTA_RECEIVE_ERROR) {
        Serial.println("Receive Failed");
      } else if (error == OTA_END_ERROR) {
        Serial.println("End Failed");
      }
    });
    ArduinoOTA.begin();
    Serial.println("Ready");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());  
  }
  touch.attach();
}

void loop() 
{
  
    if(WiFi.status() == WL_CONNECTED && (wifi_source==STATION)){
      ArduinoOTA.handle();
    //if(!Ping.ping(IP)){
    //  initAccessPoint();
    //  goto a;
    //}
    Serial.println("Getting data from the server __room__ " + ROOM);
    // getting the data to the global varible data
    int res = get_data(http, URL, GET_REQ, data);
    if(!res){ 
      Serial.println(ERROR_MSG);
      initAccessPoint(ssid_ac, pw_ac, server);
      wifi_source = ACCESS_POINT;
      goto a;
    }
    goto b;
  }
 
 
  // handle the requests of the access pointi
  a:server.handleClient();  
  b:;

  touch.read(touch_reads);
  char level = slider(touch_reads, NUMBER_TOUCH_SENSORS, PWM_PIN, PWM_MAX);
  led_display(&LEDS, level, NUMBER_LEDS);

   ESP.deepSleep(SLEEP_TIME); 
}





void root()
{
  Serial.println("GET request to /");
  server.send(200,"text/html",HOME);
}

void start_server()
{
  server.on("/",root);
  server.begin();
}

// this differs from the other example in interperting the changes of the pins
#include "smart_blug.h"

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

HTTPClient http;
ESP8266WebServer server(80);
SN74HC595 LEDS;
SN74HC595 RELAYS;
uint8_t L_RELAY_STATE[NUMBER_RELAYS];
uint8_t N_RELAY_STATE[NUMBER_RELAYS];
char pin_states[3];


void setup(){
  SN74HC595_INIT_PIN(&RELAYS, SER, Ser);
  SN74HC595_INIT_PIN(&RELAYS, RCLK, Rclk);
  SN74HC595_INIT_PIN(&RELAYS, SRCLK, Srclk);
  SN74HC595_INIT(&RELAYS);
  
  SN74HC595_INIT_PIN(&LEDS, SER, Ser_LED);
  SN74HC595_INIT_PIN(&LEDS, RCLK, Rclk_LED);
  SN74HC595_INIT_PIN(&LEDS, SRCLK, Srclk_LED);
  SN74HC595_INIT(&LEDS);
  
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

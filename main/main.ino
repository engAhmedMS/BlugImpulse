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

int changes[NUMBER_RELAYS] = {0, 0, 0}; // you have to update this array once the state is changeged

const char *ERROR_MSG = "Error occured on connectiong the server";
const char *INIT_CONNECTION = "http://impulses-iot.herokuapp.com/?action=init-connection&username=tmp&password=12NUMBER_RELAYS4";
const char *ssid = "TE-Data-BFF2B2";    // network name
const char *password = "azharOTHMan7"; // network pass

String UPDATE_REQ = "http://impulses-iot.herokuapp.com/?action=get-state&port=";          // +PORT
String END_CONNECTION = "http://impulses-iot.herokuapp.com/?action=end-connection&port="; // +PORT
String UPDATE_CHANGES = "http://impulses-iot.herokuapp.com/?action=update-changes&port="; // +port + changes
String GET_CHANGES = "http://impulses-iot.herokuapp.com/?action=get-changes&port=";       // +port
String PORT = "";
String DATA_OUTPUT = "";

boolean PORT_ADDED = false;
boolean SERVER_CONN = false;

uint8_t L_RELAY_STATE[NUMBER_RELAYS];
uint8_t N_RELAY_STATE[NUMBER_RELAYS];

SN74HC595 RELAYS;

HTTPClient http;

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

void setup()
{
//    EEPROM.begin(512);
    Serial.begin(115200);

    
    SN74HC595_INIT_PIN(&RELAYS, SER, Ser);
    SN74HC595_INIT_PIN(&RELAYS, RCLK, Rclk);
    SN74HC595_INIT_PIN(&RELAYS, SRCLK, Srclk);
    SN74HC595_INIT(&RELAYS);
    
    Restore_Session();
    
    WiFi.begin(ssid, password);
    Serial.print("\n\nConnecting");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nConnected to the network ...");
}

void loop()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        if (!SERVER_CONN){
            Serial.println(get_port() ? "connected to the server.." : ERROR_MSG);
        }
        if (SERVER_CONN)
        {
            get_data();
//            Serial.println(get_data() ? DATA_OUTPUT : ERROR_MSG);
        }
        delay(200); // delay for updateing status ..
    }
    splitData(DATA_OUTPUT, N_RELAY_STATE);
    UPDATE_RELAYS(N_RELAY_STATE);
    //end_connection();
}

int get_port()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("Getting the `PORT`...");
        http.begin(INIT_CONNECTION);
        int httpCode = http.GET();
        if (httpCode > 0)
        {
            PORT = http.getString();
            if(PORT == ""){
              Serial.println(ERROR_MSG);
              SERVER_CONN = false;
              return 0;
            }
            Serial.print("YOU ARE CONNECTED ON PORT :: ");
            Serial.println(PORT);
            SERVER_CONN = true;
            if (!PORT_ADDED)
            {
                UPDATE_REQ += PORT;
                END_CONNECTION += PORT;
                PORT_ADDED = true;
            }
        }
        else
        {
            Serial.println(ERROR_MSG);
            SERVER_CONN = false;
            return 0;
        }
        http.end();
        return 1;
    }
    return 0;
}

int get_data()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        http.begin(UPDATE_REQ);
        int httpCode = http.GET();
        if (httpCode > 0)
        {
            DATA_OUTPUT = http.getString();
            SERVER_CONN = true;
        }
        else
        {
            Serial.println(ERROR_MSG);
            SERVER_CONN = false;
            return 0;
        }
        http.end();
        return 1;
    }
    return 0;
}

int end_connection()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("Ending the connnection..");
        http.begin(END_CONNECTION);
        int httpCode = http.GET();
        if (httpCode > 0)
            Serial.print("CONNECTION ENDED");
        SERVER_CONN = false;
        http.end();
    }
}
void splitData(String ps, uint8_t* RELAYS){
  int i = 0;
  if(ps[0] != 'R'){
    return;
  }
  else{
    for(int j = 0; j < NUMBER_RELAYS; j++){
      while((ps[i++]) != ':');
      RELAYS[j] = ps[i] - '0';
    }
  }
}

void Restore_Session(){
  EEPROM.begin(512);
  for(int i = 0; i < NUMBER_RELAYS; i++){
    L_RELAY_STATE[i] = EEPROM.read(i+1);
    N_RELAY_STATE[i] = L_RELAY_STATE[i];
    SN74HC595_Write(&RELAYS, i+1, N_RELAY_STATE[i]);
  }
  EEPROM.end();
}
void UPDATE_RELAYS(uint8_t* RELAYs){
  for(int i = 0; i < NUMBER_RELAYS; i++){
    if(RELAYs[i] != L_RELAY_STATE[i]){
      EEPROM.begin(512);
      SN74HC595_Write(&RELAYS, i+1, RELAYs[i]);
      L_RELAY_STATE[i] = RELAYs[i];
      EEPROM.write(i+1, L_RELAY_STATE[i]);
      EEPROM.end();
    }
  }
}


int update_changes()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("UPDATE THE CHANGES...");
        String data = "&changes=" + form(changes, NUMBER_RELAYS);
        http.begin(UPDATE_CHANGES + data);
        int httpCode = http.GET();
        if (httpCode > 0)
        {
            Serial.println("CHNGES UPDATED ...");
        }
        else
        {
            Serial.println(ERROR_MSG);
            SERVER_CONN = false;
        }
        http.end();
    }
}


String form(int *data, int n)
{
    String res = "";
    for (int i = 0; i < n; i++)
    {
        res += "RELAY" + String(i) + ":" + String(data[i]) + (i == n - 1 ? "" : ",");
    }
    return res;
}

char slider(int p)
{
  #define READ_BIT(REG, PIN) ((REG>>PIN)&1)
  int i=NUMBER_TOUCH_SENSORS;
  for(; i>=0; i--)
  {
    if(READ_BIT(p, i))
    {
      ledcWrite(PWM_CHANNEL, (i*PWM_MAX/(NUMBER_TOUCH_SENSORS-1)));
      break;
    }
  }
  return i;  
}

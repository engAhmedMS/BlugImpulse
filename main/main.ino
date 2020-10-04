#include "smart_blug.h"
#include "esp8266_touch.h"


extern int changes[NUMBER_RELAYS]; 
//ESP32 as a station
extern const char *ERROR_MSG;
extern const char *INIT_CONNECTION;
const char *ssid = "ahmed mohammed";//"TE-Data-BFF2B2";    // network name
const char *password = "am0113099454";//"azharOTHMan7"; // network pass

extern String UPDATE_REQ;
extern String END_CONNECTION;
extern String UPDATE_CHANGES;
extern String GET_CHANGES;
extern String PORT;
extern String DATA_OUTPUT;

extern boolean PORT_ADDED;
extern boolean SERVER_CONN;

//esp8266 touch parameters
#define SCL_PIN 1
#define SDA_PIN 2
#define RST_PIN 3
#define TOUCHPAD_SIZE 8
bool touch_read[TOUCHPAD_SIZE];
touchSensor touch(SCL_PIN, SDA_PIN, RST_PIN);

uint8_t L_RELAY_STATE[NUMBER_RELAYS];
uint8_t N_RELAY_STATE[NUMBER_RELAYS];
SN74HC595 RELAYS;

//ESP8266 as access poin
const char *access_ssid = "yourAP";
const char *access_password = "yourPassword";

ESP8266WebServer server(80);

HTTPClient http;

int counter = 0;
IPAddress ip;
CONNECTION_STATE connection_state;
WIFI_SOURCE wifi_source;

void setup()
{
//    EEPROM.begin(512);
    Serial.begin(115200);
    delay(1000);
    Serial.print("Configuring access point...");
    /* You can remove the password parameter if you want the AP to be open. */
    WiFi.softAP(access_ssid, access_password);
  
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
    server.begin();
    Serial.println("HTTP server started");

    SN74HC595_INIT_PIN(&RELAYS, SER, Ser);
    SN74HC595_INIT_PIN(&RELAYS, RCLK, Rclk);
    SN74HC595_INIT_PIN(&RELAYS, SRCLK, Srclk);
    SN74HC595_INIT(&RELAYS);
    
    //Restore_Session();
    
    connection_state = station_init(ssid, password, MAX_CONNECTION_TIME, &wifi_source);
    if(connection_state == CONNECTED)
    {
      wifi_source = STATION;
      Serial.println("\nConnected to the network ...");
    }
    else
    {
      ip = accessPoint_init(access_ssid, access_password, &wifi_source);
      wifi_source = ACCESS_POINT;
      Serial.print("ACCES POINT IP = ");
      Serial.println(ip);
      server.begin();
      Serial.println("Server started");
    }
    
    touch.attach();
}

void loop()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        if (!SERVER_CONN){
            Serial.println(get_port(ssid, password, &http) ? "connected to the server.." : ERROR_MSG);
        }
        if (SERVER_CONN)
        {
            get_data(&http);
//            Serial.println(get_data() ? DATA_OUTPUT : ERROR_MSG);
        }
        delay(200); // delay for updateing status ..
    }
    else if(wifi_source != ACCESS_POINT)
    {
      ip = accessPoint_init(access_ssid, access_password, &wifi_source);
      Serial.print("ACCESS PION IP = ");
      Serial.println(ip);
      server.begin();
      Serial.println("Server started");
    }
    splitData(DATA_OUTPUT, N_RELAY_STATE);
    
    touch.read(touch_read);
    for(int i=0; i<TOUCHPAD_SIZE; i++)
    {
      Serial.print(touch_read[i]);
      Serial.print(" -- ");
    }
      Serial.println();
      slider(touch_read);
    
    UPDATE_RELAYS(N_RELAY_STATE);
    //end_connection(http);
}



void Restore_Session()
{
  EEPROM.begin(512);
  for(int i = 0; i < NUMBER_RELAYS; i++){
    L_RELAY_STATE[i] = EEPROM.read(i+1);
    N_RELAY_STATE[i] = L_RELAY_STATE[i];
    SN74HC595_Write(&RELAYS, i+1, N_RELAY_STATE[i]);
  }
  EEPROM.end();
}

void UPDATE_RELAYS(uint8_t* RELAYs)
{
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

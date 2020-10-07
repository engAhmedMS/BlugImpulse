#include "smart_blug.h"
#include "esp8266_touch.h"

#define DEL 0
//debugging start
//puuting small delay solved some serial monitors issues, but the problem still not completely solved
#define PRINT(STR)    do{\
                            Serial.print(STR);\
                            delay(DEL);\
                        }while(0);
#define PRINTLN(STR)  do{\
                            Serial.println(STR);\
                            delay(DEL);\
                        }while(0);
//debugging end

extern int changes[NUMBER_RELAYS]; 
//ESP8266 as a station
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
bool touch_read[TOUCHPAD_SIZE];
touchSensor touch(SCL_PIN, SDA_PIN, RST_PIN);

uint8_t L_RELAY_STATE[NUMBER_RELAYS];
uint8_t N_RELAY_STATE[NUMBER_RELAYS];
SN74HC595 RELAYS;

SN74HC595 LEDS;

//ESP8266 as access poin
const char *access_ssid = "yourAP";
const char *access_password = "yourPassword";

WiFiServer server(80);

HTTPClient http;

int counter = 0;
IPAddress ip;
CONNECTION_STATE connection_state;
WIFI_SOURCE wifi_source;

void setup()
{
    
//    EEPROM.begin(512);
    Serial.begin(115200);
    Serial.println();
    Serial.println();
    SN74HC595_INIT_PIN(&RELAYS, SER, Ser);
    SN74HC595_INIT_PIN(&RELAYS, RCLK, Rclk);
    SN74HC595_INIT_PIN(&RELAYS, SRCLK, Srclk);
    SN74HC595_INIT(&RELAYS);

    connection_state = station_init(ssid, password, MAX_CONNECTION_TIME, &wifi_source);
    if(connection_state == CONNECTED)
    {
      wifi_source = STATION;
      PRINTLN("\nConnected to the network ...");
      server.begin();
      PRINTLN("Server started");
      PRINT("Use this URL to connect: ");
      PRINT("http://");
      PRINT(WiFi.localIP());
      
    }
    else
    {
      ip = accessPoint_init(access_ssid, access_password, &wifi_source);
      wifi_source = ACCESS_POINT;
      PRINT("ACCES POINT IP = ");
      PRINTLN(ip);
      server.begin();
      PRINTLN("Server started");
    }
    touch.attach();   
}

void loop()
{  
    if (WiFi.status() == WL_CONNECTED)
    {
        if (!SERVER_CONN){
            PRINTLN(get_port(ssid, password, &http) ? "connected to the server.." : ERROR_MSG);        
        }
        if (SERVER_CONN)
        {
            get_data(&http);
//            PRINTLN(get_data() ? DATA_OUTPUT : ERROR_MSG);
        }
        delay(200); // delay for updateing status ..

    }
    else if(wifi_source != ACCESS_POINT)
    {
      ip = accessPoint_init(access_ssid, access_password, &wifi_source);
      PRINT("ACCESS PION IP = ");
      PRINTLN(ip);
      server.begin();
      PRINTLN("Server started");      
    }
    splitData(DATA_OUTPUT, N_RELAY_STATE);
    
    touch.read(touch_read);
    for(int i=0; i<8; i++)
    {
      PRINT(touch_read[i]);
      PRINT(" -- ");
      
    }

    PRINTLN();
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
        PRINTLN("UPDATE THE CHANGES...");
        String data = "&changes=" + form(changes, NUMBER_RELAYS);
        http.begin(UPDATE_CHANGES + data);
        int httpCode = http.GET();
        if (httpCode > 0)
        {
            PRINTLN("CHNGES UPDATED ...");
        }
        else
        {
            PRINTLN(ERROR_MSG);
            SERVER_CONN = false;
        }
        http.end();
    }
}

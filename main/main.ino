#include "smart_blug.h"

extern int changes[NUMBER_RELAYS]; 
//ESP32 as a station
extern const char *ERROR_MSG;
extern const char *INIT_CONNECTION;
const char *ssid = "TE-Data-BFF2B2";    // network name
const char *password = "azharOTHMan7"; // network pass

extern String UPDATE_REQ;
extern String END_CONNECTION;
extern String UPDATE_CHANGES;
extern String GET_CHANGES;
extern String PORT;
extern String DATA_OUTPUT;

extern boolean PORT_ADDED;
extern boolean SERVER_CONN;

//ESP32 as access poin
const char *access_ssid = "yourAP";
const char *access_password = "yourPassword";

uint8_t L_RELAY_STATE[NUMBER_RELAYS];
uint8_t N_RELAY_STATE[NUMBER_RELAYS];

WiFiServer server(80);
SN74HC595 RELAYS;
HTTPClient http;

char touch_pins[NUMBER_TOUCH_SENSORS] = {T0, T3, T4, T5, T6, T7, T8, T9};
int led_pins[NUMBER_TOUCH_SENSORS]   = {23, 22, 18, 5, 17, 16, 0, 2};
int t_read[NUMBER_TOUCH_SENSORS];
touchSensor touch(touch_pins, t_read, THRESHOLD, NUMBER_TOUCH_SENSORS);

int counter = 0;

CONNECTION_STATE connection_state;
void setup()
{
//    EEPROM.begin(512);
    Serial.begin(115200);

    SN74HC595_INIT_PIN(&RELAYS, SER, Ser);
    SN74HC595_INIT_PIN(&RELAYS, RCLK, Rclk);
    SN74HC595_INIT_PIN(&RELAYS, SRCLK, Srclk);
    SN74HC595_INIT(&RELAYS);
    
    Restore_Session();
    
    connection_state = station_init(ssid, password, MAX_CONNECTION_TIME);
    if(connection_state == CONNECTED)
    {
      Serial.println("\nConnected to the network ...");
    }
    else
    {
      IPAddress IP = accessPoint_init(access_ssid, access_password);
      Serial.print("AP IP address: ");
      Serial.println(IP);
      server.begin();
      Serial.println("Server started");
    }
    
    touch.attach();
    ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_FREQ);
    ledcAttachPin(LED_PIN, PWM_CHANNEL);
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
    splitData(DATA_OUTPUT, N_RELAY_STATE);
    
    int p = touch.pressed();
    slider(p);
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

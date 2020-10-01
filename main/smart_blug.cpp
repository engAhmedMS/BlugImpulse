#include "smart_blug.h"

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

int changes[NUMBER_RELAYS] = {0, 0, 0}; // you have to update this array once the state is changeged

CONNECTION_STATE station_init(const char* ssid, const char* password, int max_time_s, WIFI_SOURCE* wifi_source)
{
  int counter = 0;
  CONNECTION_STATE ret = NOT_CONNECTED;
  *wifi_source = STATION;
  WiFi.begin(ssid, password);
  Serial.print("\n\nConnecting");
  while (WiFi.status() != WL_CONNECTED)
  {
      delay(1000);
      Serial.print(".");
      ret = CONNECTED;
      counter++;
      if(counter>max_time_s)
      {
         ret = NOT_CONNECTED;
         *wifi_source = NO_SOURCE;
         break;
      }
      
  }

  return ret;
}

int get_port(const char* ssid, const char* password,HTTPClient* http)
{
    WiFi.begin(ssid, password);
    Serial.print("\n\nConnecting");
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("Getting the `PORT`...");
        http->begin(INIT_CONNECTION);
        int httpCode = http->GET();
        if (httpCode > 0)
        {
            PORT = http->getString();
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
        http->end();
        return 1;
    }
    return 0;
}

int get_data(HTTPClient* http)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        http->begin(UPDATE_REQ);
        int httpCode = http->GET();
        if (httpCode > 0)
        {
            DATA_OUTPUT = http->getString();
            SERVER_CONN = true;
        }
        else
        {
            Serial.println(ERROR_MSG);
            SERVER_CONN = false;
            return 0;
        }
        http->end();
        return 1;
    }
    return 0;
}

int end_connection(HTTPClient* http)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("Ending the connnection..");
        http->begin(END_CONNECTION);
        int httpCode = http->GET();
        if (httpCode > 0)
            Serial.print("CONNECTION ENDED");
        SERVER_CONN = false;
        http->end();
    }
}

void splitData(String ps, uint8_t* RELAYS)
{
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
  int i=NUMBER_TOUCH_SENSORS-1;
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

char slider2(int* arr_read)
{
  
  int i=NUMBER_TOUCH_SENSORS-1;
  for(; i>=0; i--)
  {
    if(arr_read[i]<THRESHOLD)
    {
      ledcWrite(PWM_CHANNEL, (i*PWM_MAX/(NUMBER_TOUCH_SENSORS-1)));
      break;
    }
  }
  return i;  
}

IPAddress accessPoint_init(const char* ssid, const char* password, WIFI_SOURCE* wifi_source)
{
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  *wifi_source = ACCESS_POINT;

  return IP;
}

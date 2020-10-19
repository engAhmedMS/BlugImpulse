#include "smart_blug.h"

WIFI_STATUS initStation(const char* ssid, const char* password, int timeout)
{
  WIFI_STATUS ret = CONNECTED;
  WiFi.begin(ssid,password);
  Serial.print("\n\nConntecting");
  int cur = millis();
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");
    if(millis() - cur >= timeout){
      Serial.println("canno't connect to the network :(");   
      ret = NOT_CONNECTED;
      break;
    }
  } 
  Serial.print("Connected to: ");
  Serial.println(ssid);
  return ret; 
  
}

IPAddress initAccessPoint(String ssid_ac, String pw_ac, ESP8266WebServer &server)
{
  WiFi.softAP(ssid_ac,pw_ac);
  IPAddress myIP = WiFi.softAPIP();
  Serial.println("init the access point");
  Serial.print("access point active on IP :: ");
  Serial.print(myIP);
  return myIP;
}

int parse_data(String data, char* pin_states)
{
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

char slider(bool* arr_read, char number_touch_sensors, char pwm_pin, int pwm_max)
{
  int level=number_touch_sensors-1;
  for(; level>=0; level--)
  {
    if(arr_read[level])
    {
      analogWrite(pwm_pin,(level*pwm_max/(number_touch_sensors-1) ) ); 
      break;
    }
  }
  return level;  
}

void led_display(SN74HC595* LEDS, char level, char number_leds)
{
  int i;
  for(i = 0; i < level; i++)
  {
      SN74HC595_Write(LEDS, i, HIGH);
  }
  for(; i< number_leds; i++)
  {
      SN74HC595_Write(LEDS, i, LOW);
  }
}

int get_data(HTTPClient& http, const String& url, const String& grt_req, String& data)
{
  if(WiFi.status() == WL_CONNECTED)
  {
    Serial.println("sending GET request to the server..");
    http.begin( url + grt_req );
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

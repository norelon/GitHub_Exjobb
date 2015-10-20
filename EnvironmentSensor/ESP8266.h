//CCGuest
//+1cyber!
//57146
#ifndef esp8266_h
#define esp8266_h
#include "arduino.h"
#include <SoftwareSerial.h>

int ESP_RST = 8;

SoftwareSerial ser(1, 0); // RX, TX

void esp_reset()
{
  digitalWrite(ESP_RST, LOW);
  delay(100);
  digitalWrite(ESP_RST, HIGH);
}

void esp_8266(float temperature = 0, float co2 = 0, float ljud = 0, float fukt = 0, float ljus = 0, int pir = 0, int antal = 0,float odds = 0)
{
  float sample = 0;
  const String apiKey   = "GD1CU4S6T3BEERF8";//"FSF6ZFHK4EULF367";

  // TCP connection
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "184.106.153.149"; // api.thingspeak.com
  cmd += "\",80";
  
  ser.println(cmd);

  if (ser.find("ERROR")) {
    Serial.println("AT+CIPSTART error");
    return;
  }
  
  delay(1000);//för stabilitet
  
  
  // prepare GET string
  String getStr = "GET /update?api_key=";
  getStr += apiKey;
  getStr += "&field1=";
  getStr += String(temperature);
  getStr += "&field2=";
  getStr += String(co2);
  getStr += "&field3=";
  getStr += String(ljud);
  getStr += "&field4=";
  getStr += String(fukt);
  getStr += "&field5=";
  getStr += String(ljus);
  getStr += "&field6=";
  getStr += String(pir);
  getStr += "&field7=";
  getStr += String(antal);
  getStr += "&field8=";
  getStr += String(odds);
  getStr += "\r\n\r\n";

  // send data length
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  ser.println(cmd);

  if (ser.find(">")) {
    ser.print(getStr);
    
  }
  else {
    //don't close if you can't read and write.
      ser.println("AT+CIPCLOSE");
      //alert user
      Serial.println("AT+CIPCLOSE");
  }
}


#endif

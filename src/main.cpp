#include<ESP8266WiFi.h>
#include<ESP8266WebServer.h>
#include<ESP8266HTTPClient.h>
#include<WiFiClient.h>
#include<Arduino.h>
#include<Arduino_JSON.h>

const char* ssid = "MathrukripaG";
const char* pass = "MathrukripaG574!48";
const char* host = "http://192.168.86.46:8080";
String pa;
String getpa(const char* host){
  WiFiClient client;
  HTTPClient http;
  http.begin( client , host );

  int responsecode = http.GET();
  String payload = "{}";
  if (responsecode > 0){
    Serial.print("HTTP Response code: ");
    Serial.println(responsecode);
    payload = http.getString();
  }
  else{
    Serial.print("Error code: ");
    Serial.println(responsecode);
  }
  http.end();
  return payload;

}

void setup(){
  Serial.begin(115200);
  WiFi.begin(ssid,pass);
  Serial.print("Connecting");
  while( WiFi.status() != WL_CONNECTED){
    delay(350);
    Serial.print(".");
  }
  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  pa = getpa(host);
  Serial.println(pa);
  
}

void loop(){  
  
}

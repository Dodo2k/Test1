#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define LED 2

const char *ssid = "ESP8266_AP";
const char *pass = "password";

ESP8266WebServer server(80);

void handleroot(){
  server.send(200, "text/html", "<h1>Hello from ESP8266 AP!</h1>");
}
void setup() {

  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.softAP(ssid,pass);
  delay(10);


/*  WiFi.begin(ssid, pass);
  while( WiFi.status() != WL_CONNECTED ){
    delay(500);
    Serial.println(".");
  }
  Serial.println("\nConnected\n");
  delay(500);
  */

  Serial.println("AP activated.\n Server IP:\t");
  Serial.println(WiFi.softAPIP());
  Serial.println("\nServer MAC address:\t");
  Serial.println(WiFi.softAPmacAddress());

//  server.on("/",handleroot);
//  server.begin();

  Serial.println("Server listening");
  
}

void loop() {
//  server.handleClient();

  // put your main code here, to run repeatedly:

}
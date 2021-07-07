#include<ESP8266WiFi.h>
#include<ESP8266WebServer.h>
#include<Arduino.h>

const char* ssid = "ESP_AP";
const char* pass = "password";

String pa[10000];
ESP8266WebServer server(80);

void varhandler(){
  if ( server.hasArg("pa") ){
    String pa = {server.arg("pa")};
    server.send( 200, "text/html" , "Data received" );
  }

}

void setup(){
  Serial.begin(115200);
  delay(1000);
  WiFi.softAP(ssid,pass);

  server.begin();
  while( WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println("Waiting to connect.....");
  }
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/data/" , HTTP_GET , varhandler );
  server.begin();
  Serial.println("Server listening.....");
  //if server recieves a request with /data in the 
  //string then run 'varhandler()' routine 

}

void loop(){
  server.handleClient();

}


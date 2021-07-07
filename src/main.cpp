#include<ESP8266WiFi.h>
#include<ESP8266WebServer.h>
#include<Arduino.h>

const char* ssid = "MathrukripaG";
const char* pass = "MathrukripaG574!48";

String pa[3000];

ESP8266WebServer server(80);

void varhandler(){
  Serial.println("Client Handler invoked....");
  if ( server.hasArg("pa") ){
    String pa = {server.arg("pa")};
    server.send( 200, "text/html" , pa );
    Serial.print("String data recieved\t");
    Serial.println(pa);
  }

}

void setup(){

  Serial.begin(115200);
  WiFi.begin(ssid,pass);
  server.begin();

  while( WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println("Waiting to connect.....");
  }
  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/data/" , HTTP_GET , varhandler );
// server.on("/data/" , varhandler );
  server.begin();
  Serial.println("Server listening.....");
  //if server recieves a request with /data in the 
  //string then run 'varhandler()' routine 

}

void loop(){
  server.handleClient();
  

}

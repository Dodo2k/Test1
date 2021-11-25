#include<ESP8266WiFi.h>
#include<ESP8266HTTPClient.h>
#include<WiFiClient.h>
#include<Arduino.h>
#include<Regexp.h>
#include<SoftwareSerial.h>

#include<mavlink.h> 
#include<params.h>
#include<mav_extras.h>

// WiFi credentials
const char* ssid = "V";
const char* pass = "vvvvvvvv";

int check = 0;  // This is set to 1 if everything goes as expected.
int arv = 0;    // a random variable needed while looping in void loop()
// Change the IP address to that of your local machine
const char* host = "http://192.168.161.254:8080";
//const char* host = "http://10.145.1.169:8080";   

// reponse code issued by server for http GET request from ESP8266 client.
int responsecode;

// character string pointer to the retrieved Permission Artifact.
char* pa; 

// function to get the permission artifact
char* GetPA(const char* host){
  WiFiClient client;
  HTTPClient http;
  http.begin( client , host );

  responsecode = http.GET();
  String payload = "{}";
  if (responsecode > 0){
    Serial.print("HTTP Response code: ");
    Serial.println(responsecode);
    payload = http.getString();
  }
  else{
    Serial.print("Error code: ");
    Serial.println(responsecode);
    Serial.println("Please try again.");
      }
  http.end();
  return &payload[0];
}

// Function to match string patterns in the PA, parse them using parser functions and store them in respective variables.
int PatternMatcher(){
  MatchState ms (pa);  // Regex state matching objects

  // stores spatial coordinates into geofence_params
  int count = ms.GlobalMatch("latitude=%p(%-?%d+%.%d+)", coordinate_parser);
  tot = j;
  k++;
  j=0;
  count = ms.GlobalMatch("longitude=%p(%-?%d+%.%d+)", coordinate_parser);

  // stores validation parameters in validation_params
  j=0; k=0;
  count = ms.GlobalMatch("alue>(.*)</DigestValue>", value_parser);
  j++;
  count = ms.GlobalMatch("<SignatureValue>(.*)</SignatureValue>", value_parser);
  j++;
  count = ms.GlobalMatch("<X509Certificate>(.*)</X509Certificate>", value_parser);

  // stores identfication parameters into ID_params
  j=0;
  count = ms.GlobalMatch("operatorID=%p(.*)%p>%s+<Pilot ", ID_parser);
  j++;
  count = ms.GlobalMatch("Pilot.*=%p(.*)%p validTo", ID_parser);
  j++;
  count = ms.GlobalMatch("uinNo=%p(.*)%p/>%s+<FlightPurpose", ID_parser);

  // procures date and time and stores them in DateTime_params
  j=0;
  count = ms.GlobalMatch("StartTime=%p(%d+%-%d+%-%d+)T", DateTime_parser);
  count = ms.GlobalMatch("EndTime=%p(%d+%-%d+%-%d+)T", DateTime_parser);
  k++; j=0;
  count = ms.GlobalMatch("StartTime=%p%d+%-%d+%-%d+T(%d+:%d+:%d+)", DateTime_parser);
  count = ms.GlobalMatch("EndTime=%p%d+%-%d+%-%d+T(%d+:%d+:%d+)", DateTime_parser);

  // procures atitude
  count = ms.GlobalMatch("maxAltitude=%p(%d+)%p", Alt_parser);

  return 1; // return 1 after successfully parsing the document
}

// Setup function.
void setup(){

  Serial.begin(115200);
  MavSerial.begin(115200); 

  WiFi.begin(ssid,pass);
  Serial.println("Connecting");
  while( WiFi.status() != WL_CONNECTED){
    delay(350);
    Serial.print(".");  }
  Serial.println();
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  pa = GetPA(host); // 
  int i = PatternMatcher();

  if( (i==1) && (responsecode == 200) ){
    ParamPrint();

    // To check if ID paramaters in PA matches IDs of drone to be flown, drone pilot and Operator respectively.
    if (IdCheck() == 1){
      Serial.println("XML document verified sending ARM command.");
      Serial.println();
      i=0;
      //while(i<1000) { MavArm(1); delay(5); i++; }
      //MavReceive();
      Serial.println("Sent Arming command and received arming ACK");
      check = 1;
    }
    else
      Serial.println("ID credential mismatch.");
  }

}

void loop( ){ 

  if(check==1 && arv<500){
    MavArm(1);
    arv++;
    delay(10);
  }

 }

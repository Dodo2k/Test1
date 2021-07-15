#include<ESP8266WiFi.h>
#include<ESP8266HTTPClient.h>
#include<WiFiClient.h>
#include<Arduino.h>
#include<Regexp.h>

// WiFi credentials
const char* ssid = "MathrukripaG";
const char* pass = "MathrukripaG574!48";

// Change the IP address to that of your local machine
const char* host = "http://192.168.86.46:8080";   

// character string pointer to the retrieved Permission Artifact.
char* pa; 
// below array contains coordinates of rectangular geofence
// which is to be fed to the flight controller to detect breach. 
String geofence_params[4][2] = { {"", ""}, {"", ""}, {"", ""}, {"", ""} };
// 1st string in below array is digest value.
// 2nd string in below array is signature value.
// 3rd string in below array contains DGCA public key.
String validation_params[3] = { "", "", "" };
// below array contains identification of the RPAS and operator.
// 1st string in below array is operatorID.
// 2nd string in below array is Pilot ID.
// 3rd string in below array contains UIN number.
String ID_params[3] = { "", "", "" };
// below array contains date and time frame.
String DateTime_params[4][2] = { { "", "" }, { "", "" } };
String altitude = "";
int j=0,k=0;



// function tp get the permission artifact
char* getpa(const char* host){
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
  return &payload[0];
}

// function to parse geofence coordinates from the obtained PA string.
void coordinate_parser( const char* match, const unsigned int length, const MatchState & ms ){
  char cap[3000];
  for (byte i=0; i < ms.level; i++ ){
    ms.GetCapture ( cap , i );
    String cro = cap;
    geofence_params[j][k] = cro;
    j++;
  }
} 

// function to parse the time specifications.
void time_parser( const char* match, const unsigned int length, const MatchState & ms ){
char cap[3000];
  for (byte i=0; i < ms.level; i++ ){
    ms.GetCapture ( cap , i );
    String cro = cap;
    Serial.println(cap);
  }
}

// function to parse the digest value, signature value and public key.
void value_parser( const char* match, const unsigned int length, const MatchState & ms ){
  char cap[3000];
  for (byte i=0; i < ms.level; i++ ){
    ms.GetCapture ( cap , i );
    String cro = cap;
    validation_params[j] = cro;
  }
}

// function to parse the drone, pilot and operator credentials. 
void ID_parser( const char* match, const unsigned int length, const MatchState & ms ){
  char cap[3000];
  for (byte i=0; i < ms.level; i++ ){
    ms.GetCapture ( cap , i );
    String cro = cap;
    ID_params[j] = cro;
  }
}

// function to parse the date and time. 
void DateTime_parser( const char* match, const unsigned int length, const MatchState & ms ){
  char cap[3000];
  for (byte i=0; i < ms.level; i++ ){
    ms.GetCapture ( cap , i );
    String cro = cap;
    DateTime_params[j][k] = cro;
    j++;
  }
}

// function to parse the altiutude limit.
void Alt_parser( const char* match, const unsigned int length, const MatchState & ms ){
  char cap[3000];
  for (byte i=0; i < ms.level; i++ ){
    ms.GetCapture ( cap , i );
    String cro = cap;
    altitude = cro;
 }
}

void setup(){
  Serial.begin(115200);
  WiFi.begin(ssid,pass);
  Serial.println("Connecting");
  while( WiFi.status() != WL_CONNECTED){
    delay(350);
    Serial.print(".");  }
  Serial.println();
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  pa = getpa(host);

  MatchState ms (pa);  // Regex state matching object

  // stores spatial coordinates into geofence_params
  int count = ms.GlobalMatch("latitude=%p(%-?%d+%.%d+)", coordinate_parser);
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
}

void loop( ){  


}

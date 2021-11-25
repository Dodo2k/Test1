#include<Arduino.h>
#include<Regexp.h>

String UAOP = "4e23ed47448c4f009797143805170117";      // Here the UAOP of the operator/owner of the drone should be entered betwenn the "".
String PilotID = "5ce276f8a6be4d86b661de9fa70c62b0";   // Here the pilot ID of the pilot flying the drone is to be entered between the "".
String UIN = "5d765baca320590004e2d11f";       // Here the UIN no. of the specific drone is to be entered between the "".


// below array contains coordinates of rectangular geofence
// which is to be fed to the flight controller to detect breach. 
String geofence_params[10][2] ;
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
int tot;

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

// function to parse the operator(UAOP), Pilot ID and drone(UIN) credentials. 
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

// Print all parameters in Serial Monitor
void ParamPrint(){
  Serial.println();
  Serial.println("Geofence coordinates");
  for(j=0;j<tot;j++){
    Serial.print(geofence_params[j][0]);
    Serial.print("\t");
    Serial.println(geofence_params[j][1]);
 }
  Serial.println();
  Serial.println("\nValidation Parameters: Digest , Signature and Certificate respectively\n");
  for(j=0;j<3;j++){
    Serial.print(validation_params[j]);
    Serial.print("\n\n");
  }
  Serial.println("\nIdentication Parameters:  UAOP, Pilot ID, UIN respectively\n");
  for(j=0;j<3;j++){
    Serial.print(ID_params[j]);
    Serial.print("\t");
  }
  Serial.println();
  Serial.println();
  Serial.println("time coordinates");
  for(j=0;j<2;j++){
    Serial.print(DateTime_params[j][0]);
    Serial.print("\t");
    Serial.println(DateTime_params[j][1]);
}
  Serial.println();
  Serial.printf("Max altitude: %sm",altitude);
  Serial.println();
}

// To check if ID paramaters in PA matches IDs of drone to be flown, drone pilot and Operator respectively.
int IdCheck(){
  if (UAOP == ID_params[0]){
    if (PilotID == ID_params[1]){
      if (UIN == ID_params[2]) {return 1;}
      else {return 0;}
        }
    else {return 0;}
  }
  else return 0;
}
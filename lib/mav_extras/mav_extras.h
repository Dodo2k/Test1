#include<mavlink.h>
#include<Arduino.h>
#include<SoftwareSerial.h>

// For telem1/UART connection with pixhawk
#define RXpin 13  
#define TXpin 15

SoftwareSerial MavSerial(RXpin, TXpin); // sets up serial communication on pins 3 and 2


// Function to arm and disarm the drone
void MavArm(boolean state) {
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];

  //Arm the drone
  //400 stands for MAV_CMD_COMPONENT_ARM_DISARM
  // 1 an 8'th argument is for ARM (0 for DISARM)
  if(state) {
    //ARM
    mavlink_msg_command_long_pack(0xFF, 0xBE, &msg, 1, 1, 400, 1,1.0,0,0,0,0,0,0);
  }else {
    //DISARM
    mavlink_msg_command_long_pack(0xFF, 0xBE, &msg, 1, 1, 400, 1,0.0,0,0,0,0,0,0);
  }
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  MavSerial.write( buf, len);
}

int MavReceive(){ 
  mavlink_message_t msg;
  mavlink_status_t status;
 // mavlink_command_ack_t ack_com; // Command received ACK
  //mavlink_mission_ack_t ack_mis; // Mission completion or clearing acknowledgement
  //mavlink_mission_request_int_t MissionReq;
  while(1){ MavArm(1);
  while(MavSerial.available())
  { 
    uint32_t c= MavSerial.read();
    if(mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &status))
    {
      switch(msg.msgid)
      {
        case MAVLINK_MSG_ID_COMMAND_LONG:   // If ACK frame for COMMAND_LONG is received
          Serial.print("MSGID:  "); Serial.println(msg.msgid);
          return 76; break;
       default:
          continue;         
      }
    }
    Serial.println("Non decodable data");
  }
 }
 return 0;
}
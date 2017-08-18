#include <SPI.h>
#include "mcp_can.h"


enum K9_KEY {
  MEM_2               = 0,
  MEM_1,
  SET,
  RECLINER_DOWN,
  RECLINER_UP,
  LEGREST_DOWN,
  LEGREST_UP,
  FOOTREST_DOWN,
  FOOTREST_UP,
  SLIDE_DOWN,
  SLIDE_UP,
  PASSENGER_BACKWARD,
  PASSENGER_FORWARD,
  PASSENGER_HEAD_DOWN,
  PASSENGER_HEAD_UP,
  HEATER,
  FAN,
  MASSAGE,
};

uint32_t deviceType = LH;
char receiveCB_Str[25] = {0,};
unsigned char stmp[8] = {0,};

// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 9;
MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin

void InitCanbus()
{
  while (CAN_OK != CAN.begin(CAN_500KBPS))              // init can bus : baudrate = 500k
  {
      Serial.println("CAN BUS Shield init fail");
      Serial.println(" Init CAN BUS Shield again");
      delay(100);
  }
  Serial.println("CAN BUS Shield init ok!");
}

void LoopCanbus()
{
  while (Serial.available()) {
    char data = Serial.read();
    receiveCB_Str[strlen(receiveCB_Str)] = data;
    delay(5);
  }
  if (receiveCB_Str[0] != 0) {
    if (strncmp(receiveCB_Str, "DC+", strlen("DC+")) == 0) {
      char* data = &receiveCB_Str[strlen("DC+")];
//    Serial.print("Change Device : ");
//    Serial.println(data);
      if (strncmp(data, "L", strlen("L")) == 0) {
        ChangeDevice(LH);
      } else if (strncmp(data, "R", strlen("R")) == 0) {
        ChangeDevice(RH);
      } else if (strncmp(data, "H", strlen("H")) == 0) {
        ChangeDevice(HEADER);
      }
    } else if (strncmp(receiveCB_Str, "K9+", strlen("K9+")) == 0) {
      char* data = &receiveCB_Str[strlen("K9+")];
      SendCanbus(SEND_K9_SWITCH_ID, atoi(data));
    } else if (strncmp(receiveCB_Str, "AT", strlen("AT")) == 0) {
      mySerial.write(receiveCB_Str);
    }
  
    memset(receiveCB_Str, 0, 25);
  }
  RecvCanBus();
}

void RecvCanBus()
{
  unsigned char len = 0;
  unsigned char buf[8];

  if(CAN_MSGAVAIL == CAN.checkReceive())            // check if data coming
  {
      CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf

      unsigned int canId = CAN.getCanId();

      Serial.println("-----------------------------");
      Serial.print("Get data from ID: ");
      Serial.println(canId, HEX);
      Serial.print("data len : ");
      Serial.println(len);

      if (len >= 8) {
        int16_t rc_pos = buf[1]; 
        rc_pos = (rc_pos << 8) + buf[0];
        int16_t sl_pos = buf[3];
        sl_pos = (sl_pos << 8) + buf[2];
        int16_t lr_pos = buf[5];
        lr_pos = (lr_pos << 8) + buf[4];
        int16_t fr_pos = buf[7];
        fr_pos = (fr_pos << 8) + buf[6];
        
        Serial.print("Recliner pos recv data : ");
        Serial.println(rc_pos);//0~286
        Serial.print("Slide pos recv data : ");
        Serial.println(sl_pos);//0~128
        Serial.print("Legrest pos recv data : ");
        Serial.println(lr_pos);//0~760 0~732
        Serial.print("Footrest pos recv data : ");
        Serial.println(fr_pos);//0~376 0~340

        if (canId == RECV_POSITION_ID + LH && deviceType == LH) {
          Send(RECV_LH_SEAT_RECLINER_POS_ID, rc_pos);
//          delay(1);
          Send(RECV_LH_SEAT_SLIDE_POS_ID, sl_pos);
//          delay(1);
          Send(RECV_LH_SEAT_LEGREST_POS_ID, lr_pos);
//          delay(1);
          Send(RECV_LH_SEAT_FOOTREST_POS_ID, fr_pos);
        } else if (canId == RECV_POSITION_ID + RH && deviceType == RH) {
          Send(RECV_RH_SEAT_RECLINER_POS_ID, rc_pos);
//          delay(1);
          Send(RECV_RH_SEAT_SLIDE_POS_ID, sl_pos);
//          delay(1);
          Send(RECV_RH_SEAT_LEGREST_POS_ID, lr_pos);
//          delay(1);
          Send(RECV_RH_SEAT_FOOTREST_POS_ID, fr_pos);
        } else if (canId == RECV_POSITION_ID + HEADER) {
        }
      }
  }
}

void SendCanbus(uint32_t _id, uint8_t _v)
{
  stmp[0] = _v/10;
  stmp[1] = _v%10;
  
  CAN.sendMsgBuf(_id + deviceType, 0, 8, stmp);

  memset(stmp, 0, 8);
}

void ChangeDevice(uint32_t _id)
{
  deviceType = _id;
}

void ChangeDeviceByIndex(uint32_t _index)
{
  if (_index == 0)
    deviceType = LH;
  else if (_index == 1)
    deviceType = RH;
  else
    deviceType = HEADER;
}


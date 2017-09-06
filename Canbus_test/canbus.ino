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
uint8_t stmp[8] = {0,};
uint8_t stmpIndex = 0;

int16_t reclinerLPos = 0xFFFF;
int16_t slideLPos = 0xFFFF;
int16_t legrestLPos = 0xFFFF;
int16_t footrestLPos = 0xFFFF;
int16_t reclinerRPos = 0xFFFF;
int16_t slideRPos = 0xFFFF;
int16_t legrestRPos = 0xFFFF;
int16_t footrestRPos = 0xFFFF;

int16_t heaterCorrectionL = 0xFFFF;
int16_t heaterLevelL = 0xFFFF;
int16_t fanLevelL = 0xFFFF;
int16_t massageLevelL = 0xFFFF;
int8_t compingModeL = 0xFF;
int16_t heaterCorrectionR = 0xFFFF;
int16_t heaterLevelR = 0xFFFF;
int16_t fanLevelR = 0xFFFF;
int16_t massageLevelR = 0xFFFF;
int8_t compingModeR = 0xFF;

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
//  while (Serial.available()) {
//    char data = Serial.read();
//    receiveCB_Str[strlen(receiveCB_Str)] = data;
//    delay(5);
//  }
//  if (receiveCB_Str[0] != 0) {
//    if (strncmp(receiveCB_Str, "C+", strlen("C+")) == 0) {
//      char* data = &receiveCB_Str[strlen("C+")];
////    Serial.print("Change Device : ");
////    Serial.println(data);
//      if (strncmp(data, "L", strlen("L")) == 0) {
//        ChangeDevice(LH);
//      } else if (strncmp(data, "R", strlen("R")) == 0) {
//        ChangeDevice(RH);
//      } else if (strncmp(data, "H", strlen("H")) == 0) {
//        ChangeDevice(HEADER);
//      }
//    } else if (strncmp(receiveCB_Str, "K9+", strlen("K9+")) == 0) {
//      char* data = &receiveCB_Str[strlen("K9+")];
//      SendCanbus(SEND_K9_SWITCH_IDX, atoi(data));
//    } else if (strncmp(receiveCB_Str, "AT", strlen("AT")) == 0) {
//      mySerial.write(receiveCB_Str);
//    }
//  
//    memset(receiveCB_Str, 0, 25);
//  }
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

      if (len >= 8) {
        int16_t rc_pos = 0;
        int16_t sl_pos = 0;
        int16_t lr_pos = 0;
        int16_t fr_pos = 0;
        
        rc_pos = buf[1];
        rc_pos = (rc_pos << 8) + buf[0];
        sl_pos = buf[3];
        sl_pos = (sl_pos << 8) + buf[2];
        lr_pos = buf[5];
        lr_pos = (lr_pos << 8) + buf[4];
        fr_pos = buf[7];
        fr_pos = (fr_pos << 8) + buf[6];
        
        if (canId == RECV_POSITION_IDX + LH && deviceType == LH) {
//          Serial.println("-----------------------------");
//          Serial.print("Get data from ID: ");
//          Serial.println(canId, HEX);
//          Serial.print("data len : ");
//          Serial.println(len);
          // 왼쪽 시트 움직임 위치 받기
          if (reclinerLPos != rc_pos) {
            Send(RECV_LH_SEAT_RECLINER_POS_ID, rc_pos);
            reclinerLPos = rc_pos;
//            Serial.print("Recliner pos recv data : ");
//            Serial.println(rc_pos);//0~286
          }
          if (slideLPos != sl_pos) {
            Send(RECV_LH_SEAT_SLIDE_POS_ID, sl_pos);
            slideLPos = sl_pos;
//            Serial.print("Slide pos recv data : ");
//            Serial.println(sl_pos);//0~128
          }
          if (legrestLPos != lr_pos) {
            Send(RECV_LH_SEAT_LEGREST_POS_ID, lr_pos);
            legrestLPos = lr_pos;
//            Serial.print("Legrest pos recv data : ");
//            Serial.println(lr_pos);//0~760 0~732
          }
          if (footrestLPos != fr_pos) {
            Send(RECV_LH_SEAT_FOOTREST_POS_ID, fr_pos);
            footrestLPos = fr_pos;
//            Serial.print("Footrest pos recv data : ");
//            Serial.println(fr_pos);//0~376 0~340
          }
        } else if (canId == RECV_POSITION_IDX + RH && deviceType == RH) {
          // 오른쪽 시트 움직임 위치 받기
          if (reclinerRPos != rc_pos) {
            Send(RECV_RH_SEAT_RECLINER_POS_ID, rc_pos);
            reclinerRPos = rc_pos;
//            Serial.print("Recliner pos recv data : ");
//            Serial.println(rc_pos);//0~286
          }
          if (slideRPos != sl_pos) {
            Send(RECV_RH_SEAT_SLIDE_POS_ID, sl_pos);
            slideRPos = sl_pos;
//            Serial.print("Slide pos recv data : ");
//            Serial.println(sl_pos);//0~128
          }
          if (legrestRPos != lr_pos) {
            Send(RECV_RH_SEAT_LEGREST_POS_ID, lr_pos);
            legrestRPos = lr_pos;
//            Serial.print("Legrest pos recv data : ");
//            Serial.println(lr_pos);//0~760 0~732
          }
          if (footrestRPos != fr_pos) {
            Send(RECV_RH_SEAT_FOOTREST_POS_ID, fr_pos);
            footrestRPos = fr_pos;
//            Serial.print("Footrest pos recv data : ");
//            Serial.println(fr_pos);//0~376 0~340
          }
        } else if (canId == RECV_MEM_1_POSITION_IDX + LH && deviceType == LH) {
          // 왼쪽 시트 메모리 1 위치값 받기
//          Serial.print("Get data from ID: ");
//          Serial.println(canId, HEX);
          Send(RECV_LH_MEM_1_RECLINER_POS_ID, rc_pos);
//            Serial.print("Mem1 pos recv data : ");
//            Serial.println(rc_pos);
          Send(RECV_LH_MEM_1_SLIDE_POS_ID, sl_pos);
//            Serial.print("Mem1 pos recv data : ");
//            Serial.println(sl_pos);
          Send(RECV_LH_MEM_1_LEGREST_POS_ID, lr_pos);
//            Serial.print("Mem1 pos recv data : ");
//            Serial.println(lr_pos);
          Send(RECV_LH_MEM_1_FOOTREST_POS_ID, fr_pos);
//            Serial.print("Mem1 pos recv data : ");
//            Serial.println(fr_pos);
        } else if (canId == RECV_MEM_1_POSITION_IDX + RH && deviceType == RH) {
          // 오른쪽 시트 메모리 1 위치값 받기
//          Serial.print("Get data from ID: ");
//          Serial.println(canId, HEX);
          Send(RECV_RH_MEM_1_RECLINER_POS_ID, rc_pos);
//            Serial.print("Mem1 pos recv data : ");
//            Serial.println(rc_pos);
          Send(RECV_RH_MEM_1_SLIDE_POS_ID, sl_pos);
//            Serial.print("Mem1 pos recv data : ");
//            Serial.println(sl_pos);
          Send(RECV_RH_MEM_1_LEGREST_POS_ID, lr_pos);
//            Serial.print("Mem1 pos recv data : ");
//            Serial.println(lr_pos);
          Send(RECV_RH_MEM_1_FOOTREST_POS_ID, fr_pos);
//            Serial.print("Mem1 pos recv data : ");
//            Serial.println(fr_pos);
        } else if (canId == RECV_MEM_2_POSITION_IDX + LH && deviceType == LH) {
          // 왼쪽 시트 메모리 2 위치값 받기
          Send(RECV_LH_MEM_2_RECLINER_POS_ID, rc_pos);
          Send(RECV_LH_MEM_2_SLIDE_POS_ID, sl_pos);
          Send(RECV_LH_MEM_2_LEGREST_POS_ID, lr_pos);
          Send(RECV_LH_MEM_2_FOOTREST_POS_ID, fr_pos);
        } else if (canId == RECV_MEM_2_POSITION_IDX + RH && deviceType == RH) {
          // 오른쪽 시트 메모리 2 위치값 받기
          Send(RECV_RH_MEM_2_RECLINER_POS_ID, rc_pos);
          Send(RECV_RH_MEM_2_SLIDE_POS_ID, sl_pos);
          Send(RECV_RH_MEM_2_LEGREST_POS_ID, lr_pos);
          Send(RECV_RH_MEM_2_FOOTREST_POS_ID, fr_pos);
        }
     } else {
        if (canId == RECV_STATUS_IDX + LH || canId == RECV_STATUS_IDX + RH) {
          int16_t header = 0;
          int16_t heater_lv = 0;
          int16_t fan_lv = 0;
          int16_t massage_lv = 0;
          int16_t comping_mode = 0;
          header = buf[0];
          if (header == 0) {
            heater_lv = buf[2];
            heater_lv = (heater_lv << 8) + buf[1];
          } else if (header == 3) {
            heater_lv = buf[1];
            fan_lv = buf[2];
            massage_lv = buf[3];
            comping_mode = buf[4];
          }

          if (canId == RECV_STATUS_IDX + LH && deviceType == LH) {
            // 현재 왼쪽 시트 히터 보정값
            if (header == 0 && heaterCorrectionL != heater_lv) {
              Send(RECV_LH_HEATER_VALUE_ID, heater_lv);
              heaterCorrectionL = heater_lv;
            } else if (header == 3) {
              if (heaterLevelL != heater_lv) {
                Send(RECV_LH_HEATER_LEVEL_ID, heater_lv);
                heaterLevelL = heater_lv;
              }
              if (fanLevelL != fan_lv) {
                Send(RECV_LH_FAN_LEVEL_ID, fan_lv);
                fanLevelL = fan_lv;
              }
              if (massageLevelL != massage_lv) {
                Send(RECV_LH_MASSAGE_LEVEL_ID, massage_lv);
                massageLevelL = massage_lv;
              }
              if (compingModeL != massage_lv) {
                Send(RECV_LH_CAMPING_MODE_ID, comping_mode);
                compingModeL = massage_lv;
              }
            }
//          Serial.print("Heater correction recv data : ");
//          Serial.println(heater_lv);
          } else if (canId == RECV_STATUS_IDX + RH && deviceType == RH) {
            // 현재 오른쪽 시트 히터 보정값
            if (header == 0 && heaterCorrectionR != heater_lv) {
              Send(RECV_RH_HEATER_VALUE_ID, heater_lv);
              heaterCorrectionR = heater_lv;
            } else if (header == 3) {
              if (heaterLevelR != heater_lv) {
                Send(RECV_RH_HEATER_LEVEL_ID, heater_lv);
                heaterLevelR = heater_lv;
              }
              if (fanLevelR != fan_lv) {
                Send(RECV_RH_FAN_LEVEL_ID, fan_lv);
                fanLevelR = fan_lv;
              }
              if (massageLevelR != massage_lv) {
                Send(RECV_RH_MASSAGE_LEVEL_ID, massage_lv);
                massageLevelR = massage_lv;
              }
              if (compingModeR != massage_lv) {
                Send(RECV_RH_CAMPING_MODE_ID, comping_mode);
                compingModeR = massage_lv;
              }
            }
//          Serial.print("Heater correction recv data : ");
//          Serial.println(heater_lv);
          }
        }
     }
  }
}

void ResetCanbusData()
{
  stmpIndex = 0;
  memset(stmp, 0, 8);
}

void SetCanbus1Btye(int8_t _v)
{
  if (sizeof(stmp) > stmpIndex+1) {
    stmp[stmpIndex++] = _v;
  }
}

void SetCanbus2Btye(int16_t _v)
{
  if (sizeof(stmp) > stmpIndex+2) {
    stmp[stmpIndex++] = (_v & 0xFF);
    stmp[stmpIndex++] = (_v >> 8);
  }
}

void SendCanbus(uint32_t _id)
{
  CAN.sendMsgBuf(_id + deviceType, 0, 8, stmp);
  ResetCanbusData();
}

void SendCanbus(uint32_t _id, uint8_t _v)
{
  stmp[0] = _v/10;
  stmp[1] = _v%10;
  
  SendCanbus(_id);
}

void ChangeDevice(uint32_t _id)
{
  deviceType = _id;
}

void ResetValues()
{
  reclinerLPos = 0xFFFF;
  slideLPos = 0xFFFF;
  legrestLPos = 0xFFFF;
  footrestLPos = 0xFFFF;
  reclinerRPos = 0xFFFF;
  slideRPos = 0xFFFF;
  legrestRPos = 0xFFFF;
  footrestRPos = 0xFFFF;
  heaterCorrectionL = 0xFFFF;
  heaterLevelL = 0xFFFF;
  fanLevelL = 0xFFFF;
  massageLevelL = 0xFFFF;
  compingModeL = 0xFF;
  heaterCorrectionR = 0xFFFF;
  heaterLevelR = 0xFFFF;
  fanLevelR = 0xFFFF;
  massageLevelR = 0xFFFF;
  compingModeR = 0xFF;
}

void ChangeDeviceByIndex(uint32_t _index)
{
  if (_index == 0)
    ChangeDevice(LH);
  else if (_index == 1)
    ChangeDevice(RH);
  else
    ChangeDevice(HEADER);
}


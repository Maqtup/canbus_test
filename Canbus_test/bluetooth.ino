#include <SoftwareSerial.h> //시리얼 통신 라이브러리 호출

#define RESET_VALUES                        "RVS"
#define SEAT_SWITCH_L_R_ID                  "SSLR"
#define SEND_SEAT_K9_SWITCH_ID              "SSK9S"
#define SEND_SEAT_WORKIN_LUMBAR_SWITCH_ID   "SSWLS"

#define SEND_SEAT_MOVE_POS_ID               "SSMP"

#define SEND_MEM_1_SEAT_POS_ID              "SM1SP"
#define SEND_MEM_2_SEAT_POS_ID              "SM2SP"
#define SEND_COMPLEX_ID                     "SC"
#define SEND_REQUEST_ITEM_ID                "SRI"

#define RECV_LH_SEAT_SLIDE_POS_ID           "RLSSP"
#define RECV_LH_SEAT_RECLINER_POS_ID        "RLSRP"
#define RECV_LH_SEAT_LEGREST_POS_ID         "RLSLP"
#define RECV_LH_SEAT_FOOTREST_POS_ID        "RLSFP"
#define RECV_RH_SEAT_SLIDE_POS_ID           "RRSSP"
#define RECV_RH_SEAT_RECLINER_POS_ID        "RRSRP"
#define RECV_RH_SEAT_LEGREST_POS_ID         "RRSLP"
#define RECV_RH_SEAT_FOOTREST_POS_ID        "RRSFP"

#define RECV_LH_MEM_1_RECLINER_POS_ID       "RLM1RP"
#define RECV_LH_MEM_1_SLIDE_POS_ID          "RLM1SP"
#define RECV_LH_MEM_1_LEGREST_POS_ID        "RLM1LP"
#define RECV_LH_MEM_1_FOOTREST_POS_ID       "RLM1FP"
#define RECV_LH_MEM_2_RECLINER_POS_ID       "RLM2RP"
#define RECV_LH_MEM_2_SLIDE_POS_ID          "RLM2SP"
#define RECV_LH_MEM_2_LEGREST_POS_ID        "RLM2LP"
#define RECV_LH_MEM_2_FOOTREST_POS_ID       "RLM2FP"
#define RECV_RH_MEM_1_RECLINER_POS_ID       "RRM1RP"
#define RECV_RH_MEM_1_SLIDE_POS_ID          "RRM1SP"
#define RECV_RH_MEM_1_LEGREST_POS_ID        "RRM1LP"
#define RECV_RH_MEM_1_FOOTREST_POS_ID       "RRM1FP"
#define RECV_RH_MEM_2_RECLINER_POS_ID       "RRM2RP"
#define RECV_RH_MEM_2_SLIDE_POS_ID          "RRM2SP"
#define RECV_RH_MEM_2_LEGREST_POS_ID        "RRM2LP"
#define RECV_RH_MEM_2_FOOTREST_POS_ID       "RRM2FP"

#define RECV_LH_HEATER_VALUE_ID             "RLHV"
#define RECV_RH_HEATER_VALUE_ID             "RRHV"

#define RECV_LH_HEATER_LEVEL_ID             "RLHLV"
#define RECV_LH_FAN_LEVEL_ID                "RLFLV"
#define RECV_LH_MASSAGE_LEVEL_ID            "RLMLV"
#define RECV_LH_CAMPING_MODE_ID             "RLCMD"
#define RECV_LH_SEATDOWN_ID                 "RLSTD"
#define RECV_LH_PRESS_SET_ID                "RLPRS"
#define RECV_RH_HEATER_LEVEL_ID             "RRHLV"
#define RECV_RH_FAN_LEVEL_ID                "RRFLV"
#define RECV_RH_MASSAGE_LEVEL_ID            "RRMLV"
#define RECV_RH_CAMPING_MODE_ID             "RRCMD"
#define RECV_RH_SEATDOWN_ID                 "RRSTD"
#define RECV_RH_PRESS_SET_ID                "RRPRS"

int blueTx=3;   //Tx (보내는핀 설정)
int blueRx=4;   //Rx (받는핀 설정)
SoftwareSerial mySerial(blueTx, blueRx);  //시리얼 통신을 위한 객체선언

char receiveStr[25] = {0,};
long bluetoothPreviousMillis = 0;
long bluetoothInterval = 10000;

void InitBluetooth()
{
  mySerial.begin(9600); //블루투스 시리얼 개방
}

void LoopBluetooth()
{
//  unsigned long current_millis = millis();
//  if (current_millis - bluetoothPreviousMillis >= bluetoothInterval) {
//    bluetoothPreviousMillis = current_millis;
//  }

  Recv();
}

void Recv()
{
  while(mySerial.available())  //mySerial에 전송된 값이 있으면
  {
    char myChar = (char)mySerial.read();  //mySerial int 값을 char 형식으로 변환
    receiveStr[strlen(receiveStr)] = myChar;
    Serial.print(myChar);
    delay(5);           //수신 문자열 끊김 방지
  }
  if(receiveStr[0] != 0)  //receiveStr 값이 있다면
  {
    Serial.println("");
    Serial.print("Bluetooth Receive : ");
    Serial.println(receiveStr);
    
    if (strncmp(receiveStr, SEAT_SWITCH_L_R_ID, strlen(SEAT_SWITCH_L_R_ID)) == 0) {
      char* data = &receiveStr[strlen(SEAT_SWITCH_L_R_ID)];
      Serial.print("Seat switch L, R : ");
      Serial.println(data);
      ChangeDeviceByIndex(atoi(data));
    } else if (strncmp(receiveStr, SEND_SEAT_K9_SWITCH_ID, strlen(SEND_SEAT_K9_SWITCH_ID)) == 0) {
      char* data = &receiveStr[strlen(SEND_SEAT_K9_SWITCH_ID)];
      Serial.print("Seat K9 switch : ");
      Serial.println(data);
      SendCanbus(SEND_K9_SWITCH_IDX, atoi(data));
    } else if (strncmp(receiveStr, SEND_SEAT_WORKIN_LUMBAR_SWITCH_ID, strlen(SEND_SEAT_WORKIN_LUMBAR_SWITCH_ID)) == 0) {
      char* data = &receiveStr[strlen(SEND_SEAT_WORKIN_LUMBAR_SWITCH_ID)];
      Serial.print("Seat Workin & Lumbar switch : ");
      Serial.println(data);
      ResetCanbusData();
      SetCanbus1Btye(0);
      SetCanbus1Btye(0);
      SetCanbus1Btye(atoi(data));
      SendCanbus(SEND_K9_SWITCH_IDX);
    } else if (strncmp(receiveStr, SEND_MEM_1_SEAT_POS_ID, strlen(SEND_MEM_1_SEAT_POS_ID)) == 0) {
      char* data = &receiveStr[strlen(SEND_MEM_1_SEAT_POS_ID)];
      Serial.print("Memory 1 : ");
      Serial.println(data);
      ResetCanbusData();
      char *ptr;
      ptr = strtok(data, ",");
      while (ptr != NULL) {
        SetCanbus2Btye(atoi(ptr));
        ptr = strtok(NULL, ",");
      }
      SendCanbus(SEND_MEM_1_POSITION_IDX);
    } else if (strncmp(receiveStr, SEND_MEM_2_SEAT_POS_ID, strlen(SEND_MEM_2_SEAT_POS_ID)) == 0) {
      char* data = &receiveStr[strlen(SEND_MEM_2_SEAT_POS_ID)];
      Serial.print("Memory 2 : ");
      Serial.println(data);
      ResetCanbusData();
      char *ptr;
      ptr = strtok(data, ",");
      while (ptr != NULL) {
        SetCanbus2Btye(atoi(ptr));
        ptr = strtok(NULL, ",");
      }
      SendCanbus(SEND_MEM_2_POSITION_IDX);
    } else if (strncmp(receiveStr, SEND_COMPLEX_ID, strlen(SEND_COMPLEX_ID)) == 0) {
      char* data = &receiveStr[strlen(SEND_COMPLEX_ID)];
      Serial.print("Complex : ");
      Serial.println(data);
      ResetCanbusData();
      char *ptr;
      ptr = strtok(data, ",");
      SetCanbus1Btye(atoi(ptr));
      ptr = strtok(NULL, ",");
      if (ptr != NULL)
        SetCanbus2Btye(atoi(ptr));
      SendCanbus(SEND_COMPLEX_IDX);
    } else if (strncmp(receiveStr, SEND_REQUEST_ITEM_ID, strlen(SEND_REQUEST_ITEM_ID)) == 0) {
      char* data = &receiveStr[strlen(SEND_REQUEST_ITEM_ID)];
      Serial.print("Request Item : ");
      Serial.println(data);
      ResetCanbusData();
      SetCanbus1Btye(atoi(data));
      SendCanbus(SEND_REQUEST_ITEM_IDX);
    } else if (strncmp(receiveStr, RESET_VALUES, strlen(RESET_VALUES)) == 0) {
      Serial.println("Reset Values.");
      ResetValues();
    } else if (strncmp(receiveStr, SEND_SEAT_MOVE_POS_ID, strlen(SEND_SEAT_MOVE_POS_ID)) == 0) {
        char* data = &receiveStr[strlen(SEND_SEAT_MOVE_POS_ID)];
        Serial.print("Seat Move : ");
        Serial.println(data);
        ResetCanbusData();
        uint8_t count=0;
        char *ptr;
        ptr = strtok(data, ",");
        while (ptr != NULL) {
            ++count;
            if (count == 2) {
                SetCanbus2Btye(GetSlidePos());
            } else {
                SetCanbus2Btye(atoi(ptr));
            }
            ptr = strtok(NULL, ",");
        }
        SendCanbus(SEND_POSITION_IDX);
    }
    memset(receiveStr, 0, 20);
  }
}

void Send(char* _id, int _data)
{
  char send_data[20] = {0,};
  char data[10] = {0,};
  itoa(_data, data, 10);
  strcat(send_data, _id);
  int len = strlen(send_data);
  send_data[len] = ',';
  strcat(send_data, data);
  len = strlen(send_data);
  send_data[len] = (char)10;
  mySerial.write(send_data);
  Serial.print("Send Bluetooth data : ");
  Serial.print(send_data);
  Serial.println("");
}

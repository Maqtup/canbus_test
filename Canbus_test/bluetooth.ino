#include <SoftwareSerial.h> //시리얼 통신 라이브러리 호출

#define SEAT_SWITCH_L_R_ID                  "SSLR"
#define SEND_SEAT_K9_SWITCH_ID              "SSK9S"
#define SEND_LH_SEAT_SLIDE_POS_ID           "SLSSP"
#define SEND_LH_SEAT_RECLINER_POS_ID        "SLSRP"
#define SEND_LH_SEAT_LEGREST_POS_ID         "SLSLP"
#define SEND_LH_SEAT_FOOTREST_POS_ID        "SLSFP"

#define SEND_RH_SEAT_SLIDE_POS_ID           "SRSSP"
#define SEND_RH_SEAT_RECLINER_POS_ID        "SRSRP"
#define SEND_RH_SEAT_LEGREST_POS_ID         "SRSLP"
#define SEND_RH_SEAT_FOOTREST_POS_ID        "SRSFP"

#define RECV_LH_SEAT_SLIDE_POS_ID           "RLSSP"
#define RECV_LH_SEAT_RECLINER_POS_ID        "RLSRP"
#define RECV_LH_SEAT_LEGREST_POS_ID         "RLSLP"
#define RECV_LH_SEAT_FOOTREST_POS_ID        "RLSFP"

#define RECV_RH_SEAT_SLIDE_POS_ID           "RRSSP"
#define RECV_RH_SEAT_RECLINER_POS_ID        "RRSRP"
#define RECV_RH_SEAT_LEGREST_POS_ID         "RRSLP"
#define RECV_RH_SEAT_FOOTREST_POS_ID        "RRSFP"

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
  unsigned long current_millis = millis();
  if (current_millis - bluetoothPreviousMillis >= bluetoothInterval) {
    bluetoothPreviousMillis = current_millis;

  
  }

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
      SendCanbus(SEND_K9_SWITCH_ID, atoi(data));
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
}

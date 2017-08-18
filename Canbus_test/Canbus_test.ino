
#define LH                        0x00
#define RH                        0x40
#define HEADER                    0x80

#define SEND_K9_SWITCH_ID         0x130
#define SEND_POSITION_ID          0x131
#define SEND_HEATER_ID            0x132
#define RECV_POSITION_ID          0x310

void setup() {
  Serial.begin(9600);
  
  // put your setup code here, to run once:
  InitBluetooth();

  InitCanbus();
}

void loop() {
  // put your main code here, to run repeatedly:
  LoopBluetooth();

  LoopCanbus();
}

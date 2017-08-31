
#define LH                        0x00
#define RH                        0x40
#define HEADER                    0x80

#define SEND_K9_SWITCH_IDX        0x130
#define SEND_POSITION_IDX         0x131
#define SEND_COMPLEX_IDX          0x132
#define SEND_REQUEST_ITEM_IDX     0x133
#define SEND_MEM_1_POSITION_IDX   0x134
#define SEND_MEM_2_POSITION_IDX   0x135

#define RECV_POSITION_IDX         0x310
#define RECV_MEM_1_POSITION_IDX   0x311
#define RECV_MEM_2_POSITION_IDX   0x312
#define RECV_HEATER_IDX           0x313

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

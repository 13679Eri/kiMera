#include <Wire.h>
#include "Adafruit_MPR121.h"

Adafruit_MPR121 cap = Adafruit_MPR121();

#define CHANNEL_NUM 12

uint16_t touch_status_flag[CHANNEL_NUM] = { 0 };
int note = 0;
int pre_note = 0;

void setup() {
  Serial.begin(115200);
  // while (!Serial) delay(10);

  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  } else {
    Serial.println("MPR121 init OK!");
  }
}

void loop() {
  uint16_t result = cap.touched();

  for (int i = 0; i < CHANNEL_NUM; i++) {
    if (result & (1 << i)) { // touched
      if (touch_status_flag[i] == 0) {
        touch_status_flag[i] = 1;
        Serial.print("key ");
        Serial.print(i);
        Serial.println(" pressed");
      }
    } else { // not touched
      if (touch_status_flag[i] == 1) {
        touch_status_flag[i] = 0;
        Serial.print("key ");
        Serial.print(i);
        Serial.println(" release");
      }
    }
  }

  note = flute_note();
  if (note != pre_note) {
    Serial.println(note);
  }
  pre_note = note;
  delay(50);
}

// 11,10,9,8,0,1,2, 3,4,5, 6+,7-
int flute_note() {
  int value = 0;

  if (touch_status_flag[3]) value |= 1 << 0;
  if (touch_status_flag[4]) value |= 1 << 1;
  if (touch_status_flag[5]) value |= 1 << 2;
  if (touch_status_flag[6]) value |= 1 << 3;
  if (touch_status_flag[9]) value |= 1 << 4;
  if (touch_status_flag[10]) value |= 1 << 5;
  if (touch_status_flag[11]) value |= 1 << 6;
  if (touch_status_flag[2]) value |= 1 << 7;
  if (touch_status_flag[1]) value |= 1 << 8;
  if (touch_status_flag[0]) value |= 1 << 9;
  if (touch_status_flag[7]) value |= 1 << 10;
  if (touch_status_flag[8]) value |= 1 << 11;

  // value に応じた音程を返す
  switch (value) {
    case 0b010011111111: return 59;  //B
    case 0b000011111111: return 60;  //C
    case 0b100011111111: return 61;  //C#
    case 0b010011111110: return 61;
    case 0b000011111110: return 62;
    case 0b100011111110: return 63;
    case 0b010011111100: return 63;
    case 0b000011111100: return 64;
    case 0b010011111000: return 64;
    case 0b100011111100: return 65;
    case 0b000011111000: return 65;
    case 0b100011111000: return 66;
    case 0b010011110000: return 66;
    case 0b000011110000: return 67;
    case 0b100011110000: return 68;
    case 0b010011100000: return 68;
    case 0b000011100000: return 69;
    case 0b100011100000: return 70;
    case 0b010011000000: return 70;
    case 0b000011000000: return 71;
    case 0b010010100000: return 71;
    case 0b100011000000: return 72;
    case 0b000010100000: return 72;
    case 0b100010100000: return 73;
    case 0b010010000000: return 73;
    case 0b000010000000: return 74;
    case 0b100010000000: return 75;

    // 以下略：残りのswitchも必要なら追加できます（元のコードそのまま使えます）
    default: return -1;
  }
}

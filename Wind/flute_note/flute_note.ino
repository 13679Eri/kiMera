#include "Seeed_MPR121_driver.h"

Mpr121 mpr121;

u16 touch_status_flag[CHANNEL_NUM] = { 0 };
int note = 0;
int pre_note = 0;

void setup() {
  s32 ret = 0;
  Serial.begin(115200);
  if (mpr121.begin() < 0) {
    Serial.println("Can't detect device!!!!");
  } else {
    Serial.println("mpr121 init OK!");
  }

  /*  set 0x00~0xFF to sensitivity,the value
        The higher the value, the higher the sensitivity of 4/5/6/7 channel
        When the sensor is connected to items such as crocodile clip,
        the value needs to be adjusted
    */
  mpr121.set_sensitivity(0x60);
  delay(100);
}

void loop() {
  u16 result = 0;
  u16 filtered_data_buf[CHANNEL_NUM] = { 0 };
  u8 baseline_buf[CHANNEL_NUM] = { 0 };

  result = mpr121.check_status_register();

  mpr121.get_filtered_reg_data(&result, filtered_data_buf);


    for (int i = 0; i < CHANNEL_NUM; i++) {
        if (result & (1 << i)) {                      /*key i is pressed!!*/
            if (0 == touch_status_flag[i]) {
                touch_status_flag[i] = 1;
                Serial.print("key ");
                Serial.print(i);
                Serial.println("pressed");
            }
        } else {
            if (1 == touch_status_flag[i]) {
                touch_status_flag[i] = 0;
                Serial.print("key ");
                Serial.print(i);
                Serial.println("release");
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
// 11,10,9,8,0,1,2,
// 3,4,5,
// 6+,7-
int flute_note() {
  // ch = 4;
  int value = 0;

  if (touch_status_flag[2]) value |= 1 << 0;   // flag3 が true なら 0ビット目に1をセット
  if (touch_status_flag[1]) value |= 1 << 1;   // flag5 が true なら 2ビット目に1をセット
  if (touch_status_flag[0]) value |= 1 << 2;   // flag4 が true なら 1ビット目に1をセット
  if (touch_status_flag[8]) value |= 1 << 3;   // flag3 が true なら 3ビット目に1をセット
  if (touch_status_flag[9]) value |= 1 << 4;   // flag5 が true なら 4ビット目に1をセット
  if (touch_status_flag[10]) value |= 1 << 5;  // flag4 が true なら 5ビット目に1をセット
  if (touch_status_flag[11]) value |= 1 << 6;  // flag3 が true なら 0ビット目に1をセット

  if (touch_status_flag[5]) value |= 1 << 7;  // flag5 が true なら 2ビット目に1をセット
  if (touch_status_flag[4]) value |= 1 << 8;  // flag4 が true なら 1ビット目に1をセット
  if (touch_status_flag[3]) value |= 1 << 9;  // flag3 が true なら 0ビット目に1をセット

  if (touch_status_flag[6]) value |= 1 << 10;  // flag5 が true なら 2ビット目に1をセット
  if (touch_status_flag[7]) value |= 1 << 11;  // flag4 が true なら 1ビット目に1をセット

  // 7+,6-, 3,4,5, 11,10,9,8,0,1,2,

  switch (value) {
    case 0b010011111111: return 59;  //B

    case 0b000011111111: return 60;  //C
    case 0b100011111111: return 61;  //C#
    case 0b010011111110: return 61;  //C#
    case 0b000011111110: return 62;  //D
    case 0b100011111110: return 63;  //D#
    case 0b010011111100: return 63;  //D#
    case 0b000011111100: return 64;  //E
    case 0b010011111000: return 64;  //E
    case 0b100011111100: return 65;  //F
    case 0b000011111000: return 65;  //F
    case 0b100011111000: return 66;  //F#
    case 0b010011110000: return 66;  //F#
    case 0b000011110000: return 67;  //G
    case 0b100011110000: return 68;  //G#
    case 0b010011100000: return 68;  //G#
    case 0b000011100000: return 69;  //A
    case 0b100011100000: return 70;  //A#
    case 0b010011000000: return 70;  //A#
    case 0b000011000000: return 71;  //B
    case 0b010010100000: return 71;  //B
    case 0b100011000000: return 72;  //C
    case 0b000010100000: return 72;  //C
    case 0b100010100000: return 73;  //C#
    case 0b010010000000: return 73;  //C#
    case 0b000010000000: return 74;  //D
    case 0b100010000000: return 75;  //D#

    case 0b000101111111: return 72;  //C
    case 0b100101111111: return 73;  //C#
    case 0b010101111110: return 73;  //C#
    case 0b000101111110: return 74;  //D
    case 0b100101111110: return 75;  //D#
    case 0b010101111100: return 75;  //D#
    case 0b000101111100: return 76;  //E
    case 0b010101111000: return 76;  //E
    case 0b100101111100: return 77;  //F
    case 0b000101111000: return 77;  //F
    case 0b100101111000: return 78;  //F#
    case 0b010101110000: return 78;  //F#
    case 0b000101110000: return 79;  //G
    case 0b100101110000: return 80;  //G#
    case 0b010101100000: return 80;  //G#
    case 0b000101100000: return 81;  //A
    case 0b100101100000: return 82;  //A#
    case 0b010101000000: return 82;  //A#
    case 0b000101000000: return 83;  //B
    case 0b010100100000: return 83;  //B
    case 0b100101000000: return 84;  //C
    case 0b000100100000: return 84;  //C
    case 0b100100100000: return 85;  //C#
    case 0b010100000000: return 85;  //C#
    case 0b000100000000: return 86;  //D
    case 0b100100000000: return 87;  //D#

    case 0b001001111111: return 84;  //C
    case 0b101001111111: return 85;  //C#
    case 0b011001111110: return 85;  //C#
    case 0b001001111110: return 86;  //D
    case 0b101001111110: return 87;  //D#
    case 0b011001111100: return 87;  //D#
    case 0b001001111100: return 88;  //E
    case 0b011001111000: return 88;  //E
    case 0b101001111100: return 89;  //F
    case 0b001001111000: return 89;  //F
    case 0b101001111000: return 90;  //F#
    case 0b011001110000: return 90;  //F#
    case 0b001001110000: return 91;  //G
    case 0b101001110000: return 92;  //G#
    case 0b011001100000: return 92;  //G#
    case 0b001001100000: return 93;  //A
    case 0b101001100000: return 94;  //A#
    case 0b011001000000: return 94;  //A#
    case 0b001001000000: return 95;  //B
    case 0b011000100000: return 95;  //B
    case 0b101001000000: return 96;  //C
    case 0b001000100000: return 96;  //C
    case 0b101000100000: return 97;  //C#
    case 0b011000000000: return 97;  //C#
    case 0b001000000000: return 98;  //D
    case 0b101000000000: return 99;  //D#
    default: return -1;
  }
  Serial.println(value);
}
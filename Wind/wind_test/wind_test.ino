#include "Seeed_MPR121_driver.h"
#include <Adafruit_DPS310.h>

Mpr121 mpr121;
float degree_on = 0;
float degree_off = 0;
boolean reset = 1;
int ring = 0;
float roomPascal = 0;
int note = 0;

u16 touch_status_flag[CHANNEL_NUM] = { 0 };

// Dps310 Opject
Adafruit_DPS310 dps;
#define DPS310_CS 10

//buzzer
//int buzzer = 13;

void setup() {
  Serial.begin(115200);
  s32 ret = 0;

  if (mpr121.begin() < 0) {
    Serial.println("Can't detect device!!!!");
  } else {
    Serial.println("mpr121 init OK!");
  }

  dps.configurePressure(DPS310_64HZ, DPS310_64SAMPLES);
  dps.configureTemperature(DPS310_64HZ, DPS310_64SAMPLES);

  mpr121.set_sensitivity(0x60);
  delay(100);
}

void loop() {
  float pre_pressure;
  float pressure;
  uint8_t oversampling = 7;
  int16_t ret;
  Serial.println();
  u16 result = 0;
  u16 filtered_data_buf[CHANNEL_NUM] = {0};
  u8 baseline_buf[CHANNEL_NUM] = {0};
  result = mpr121.check_status_register();
  mpr121.get_filtered_reg_data(&result, filtered_data_buf);

  roomPascal = pre_pressure;
  pre_pressure = pressure;
  sensors_event_t temp_event, pressure_event;
  dps.getEvents(&temp_event, &pressure_event);
  pressure = pressure_event.pressure;
  
  degree_on = pressure - pre_pressure;
  degree_off = pre_pressure - pressure;

  for (int i = 0; i < CHANNEL_NUM; i++) {
    if (result & (1 << i)) {                      /*key i is pressed!!*/
      if (0 == touch_status_flag[i]) {
        touch_status_flag[i] = 1;
        //          Serial.print("key ");
        //          Serial.print(i);
        //          Serial.println("pressed");
      }
    } else {
      if (1 == touch_status_flag[i]) {
        touch_status_flag[i] = 0;
        //          Serial.print("key ");
        //          Serial.print(i);
        //          Serial.println("release");
      }
    }
  }

  if (ret != 0) {
    //Something went wrong.
    //Look at the library code for more information about return codes
    Serial.print("FAIL! ret = ");
    Serial.println(ret);

  } else {
    Serial.print("Pressure1: ");
    Serial.print(pre_pressure);
    Serial.println(" Pascal");

    Serial.print("Pressure2: ");
    Serial.print(pressure);
    Serial.println(" Pascal");

    Serial.print("degree_on: ");
    Serial.print(degree_on);
    Serial.println(" Pascal");

    Serial.print("degree_off: ");
    Serial.print(degree_off);
    Serial.println(" Pascal");

    if (degree_on >= 10 && ring == 0) { //前後の値1より大きかったら
      ring = 1;
      send_note();
      Serial.println(note);
    } else if (degree_off >= 10 && ring == 1) {
      ring = 0;
      Serial.print("ring = ");
      Serial.println(ring);
    }

    Serial.print("ring = ");
    Serial.println(ring);
  }
  delay(100);
}

void send_note() {
  /*
     5  7 9 3  2 1 10 0
  */
  int fing[8] = {5, 7, 9, 3, 2, 1, 10, 0}; // 指の配列
  //  int note = -1; // 初期値として無効なノート番号を設定

  // 指遣いをビットマスクで表現（1: 押されている, 0: 離されている）
  int fingerPattern = 0;
  for (int i = 0; i < 8; i++) {
    if (touch_status_flag[fing[i]] == 1) {
      fingerPattern |= (1 << i);
    }
  }

  // 指遣いに対応するノート番号を設定
  switch (fingerPattern) {
    case 0b11111111:
      note = 60; // C（ド）
      break;
    case 0b01111111:
      note = 62; // D（レ）
      break;
    case 0b00111111:
      note = 64; // E（ミ）
      break;
    case 0b00011111:
      note = 65; // F（ファ）
      break;
    case 0b00001111:
      note = 67; // G（ソ）
      break;
    case 0b00000111:
      note = 69; // A（ラ）
      break;
    case 0b00000011:
      note = 71; // B（シ）
      break;
    case 0b00000101:
      note = 72; // C（高いド）
      break;
    case 0b00000100:
      note = 74; // D（高いレ）
    // 他の指遣いパターンを追加
    default:
      // 必要に応じてデフォルトの処理を追加
      break;
  }
}

/*
  30 = 127
  127/30 * degree: velocity

*/

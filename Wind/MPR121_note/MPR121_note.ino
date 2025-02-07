/*
    MPR121_demo.ino
    Driver for DIGITAL I2C HUMIDITY AND TEMPERATURE SENSOR

    Copyright (c) 2018 Seeed Technology Co., Ltd.
    Website    : www.seeed.cc
    Author     : downey
    Create Time: May 2018
    Change Log :

    The MIT License (MIT)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/
#include "Seeed_MPR121_driver.h"

Mpr121 mpr121;
int note = 0;

u16 touch_status_flag[CHANNEL_NUM] = {0};
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
    u16 filtered_data_buf[CHANNEL_NUM] = {0};
    u8 baseline_buf[CHANNEL_NUM] = {0};

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
//    send_note();
//    Serial.println(note);
    delay(50);
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

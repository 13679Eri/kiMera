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

#include <Wire.h>
#include "Adafruit_MPR121.h"

#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif

Adafruit_MPR121 cap = Adafruit_MPR121();
int note = 0;

uint16_t lasttouched = 0;
uint16_t currtouched = 0;

void setup() {
  Serial.begin(115200);

  while (!Serial) {  // needed to keep leonardo/micro from starting too fast!
    delay(10);
  }

  Serial.println("Adafruit MPR121 Capacitive Touch sensor test");

  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1)
      ;
  }
  Serial.println("MPR121 found!");
}

void loop() {
  // Get the currently touched pads
  currtouched = cap.touched();

  for (uint8_t i = 0; i < 12; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i))) {
      Serial.print(i);
      Serial.println(" touched");
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i))) {
      Serial.print(i);
      Serial.println(" released");
    }
  }

  // reset our state
  lasttouched = currtouched;

  return;

  // debugging info, what
  Serial.print("\t\t\t\t\t\t\t\t\t\t\t\t\t 0x");
  Serial.println(cap.touched(), HEX);
  Serial.print("Filt: ");
  for (uint8_t i = 0; i < 12; i++) {
    Serial.print(cap.filteredData(i));
    Serial.print("\t");
  }
  Serial.println();
  Serial.print("Base: ");
  for (uint8_t i = 0; i < 12; i++) {
    Serial.print(cap.baselineData(i));
    Serial.print("\t");
  }
  Serial.println();

  send_note();
  Serial.println(note);
  delay(100);
}

void send_note() {
  /*
     5  7 9 3  2 1 10 0
  */
  int fing[8] = { 5, 7, 9, 3, 2, 1, 10, 0 };  // 指の配列
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
      note = 60;  // C（ド）
      break;
    case 0b01111111:
      note = 62;  // D（レ）
      break;
    case 0b00111111:
      note = 64;  // E（ミ）
      break;
    case 0b00011111:
      note = 65;  // F（ファ）
      break;
    case 0b00001111:
      note = 67;  // G（ソ）
      break;
    case 0b00000111:
      note = 69;  // A（ラ）
      break;
    case 0b00000011:
      note = 71;  // B（シ）
      break;
    case 0b00000101:
      note = 72;  // C（高いド）
      break;
    case 0b00000100:
      note = 74;  // D（高いレ）
    // 他の指遣いパターンを追加
    default:
      // 必要に応じてデフォルトの処理を追加
      break;
  }
}

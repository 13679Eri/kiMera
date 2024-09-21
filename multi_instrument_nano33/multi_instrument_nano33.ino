//複合楽器
//nano 33 ble
//ピアノ
//管楽器
//実装用

/*
  piano 1
  midi情報送る

  violinモード 2
  加速度送る
  midi情報は送らない

  trumpet 3
  midi情報送る

  flute 4
  midi情報送る
*/

#include <Arduino_LSM9DS1.h>
#include "Seeed_MPR121_driver.h"
#include <Adafruit_DPS310.h>

Mpr121 mpr121;
Adafruit_DPS310 dps;

u16 touch_status_flag[CHANNEL_NUM] = { 0 }; 　 //静電容量のやつ

int sw[30];       //鍵盤の数
int flag[30];     //フラグ

// 送信用変数
int ch = 0; //チャンネル
int note = 60; 　 //音程
bool ring = 0; //なっているかなっていないか
int velo = 127; //音量
int mode = 0; //楽器モード

void setup() {
  Serial.begin(115200);
  s32 ret = 0;
  if (mpr121.begin() < 0) {
    Serial.println("Can't detect device!!!!");
  } else {
    Serial.println("mpr121 init OK!");
  }

  f (!dps.begin_I2C()) {  // Can pass in I2C address here
    //if (! dps.begin_SPI(DPS310_CS)) {  // If you want to use SPI
    Serial.println("Failed to find DPS");
    while (1) yield();
  }
  dps.configurePressure(DPS310_64HZ, DPS310_64SAMPLES);
  dps.configureTemperature(DPS310_64HZ, DPS310_64SAMPLES);

  delay(100);

  pinMode( 2, INPUT);
  pinMode( 3, INPUT);
  pinMode( 4, INPUT);
  pinMode( 5, INPUT);
  pinMode( 6, INPUT);

  pinMode( 7, OUTPUT);
  pinMode( 8, OUTPUT);
  pinMode( 9, OUTPUT);
  pinMode( 10, OUTPUT);
  pinMode( 11, OUTPUT);
  pinMode( 12, OUTPUT);

  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
}

void loop() {
  while (Serial.available()) { // 受信データを確認する
    mode = Serial.read();
    if (mode == 1) {
      piano();
    } else if (mode == 2) {
      angle();
      data_send();
    } else if (mode == 3) {
      trumpet();
    } else if (mode == 4) {
      flute();
    }
  }
}

void piano() {
  ch = 1;
  digitalWrite( 12, LOW);
  delay(10);
  digitalWrite( 7, HIGH);
  sw[0] = digitalRead( 2);
  sw[1] = digitalRead( 3);
  sw[2] = digitalRead( 4);
  sw[3] = digitalRead( 5);
  sw[4] = digitalRead( 6);

  digitalWrite( 7, LOW);
  delay(10);
  digitalWrite( 8, HIGH);
  sw[5] = digitalRead( 2);
  sw[6] = digitalRead( 3);
  sw[7] = digitalRead( 4);
  sw[8] = digitalRead( 5);
  sw[9] = digitalRead( 6);

  digitalWrite( 8, LOW);
  delay(10);
  digitalWrite( 9, HIGH);
  sw[10] = digitalRead( 2);
  sw[11] = digitalRead( 3);
  sw[12] = digitalRead( 4);
  sw[13] = digitalRead( 5);
  sw[14] = digitalRead( 6);

  digitalWrite( 9, LOW);
  delay(10);
  digitalWrite( 10, HIGH);
  sw[15] = digitalRead( 2);
  sw[16] = digitalRead( 3);
  sw[17] = digitalRead( 4);
  sw[18] = digitalRead( 5);
  sw[19] = digitalRead( 6);

  digitalWrite( 10, LOW);
  delay(10);
  digitalWrite( 11, HIGH);
  sw[20] = digitalRead( 2);
  sw[21] = digitalRead( 3);
  sw[22] = digitalRead( 4);
  sw[23] = digitalRead( 5);
  sw[24] = digitalRead( 6);

  digitalWrite( 11, LOW);
  delay(10);
  digitalWrite( 12, HIGH);
  sw[25] = digitalRead( 2);
  sw[26] = digitalRead( 3);
  sw[27] = digitalRead( 4);
  sw[28] = digitalRead( 5);
  sw[29] = digitalRead( 6);

  note_piano();

  for (int i = 0; i < sizeof(sw) / sizeof(sw[0]); i++) {
    Serial.print(sw[i]);
  }

  Serial.println("");
  delay(100);
}

void trumpet() {
  ch = 3;
  u16 result = 0;
  u16 filtered_data_buf[CHANNEL_NUM] = { 0 };
  u8 baseline_buf[CHANNEL_NUM] = { 0 };

  result = mpr121.check_status_register();

  mpr121.get_filtered_reg_data(&result, filtered_data_buf);

  sensors_event_t temp_event, pressure_event;
  while (!dps.temperatureAvailable() || !dps.pressureAvailable()) {
    return;  // wait until there's something to read
  }
  for (int i = 0; i < CHANNEL_NUM; i++) {
    if (result & (1 << i)) /*key i is pressed!!*/
    {
      if (0 == touch_status_flag[i]) { //押したとき
        touch_status_flag[i] = 1;
        ring = 1;
        //音程の作成
      }
    } else {
      if (1 == touch_status_flag[i]) { //離したとき
        touch_status_flag[i] = 0;
        ring = 0;
        //音程の作成
      }
    }
  }
  delay(50);
}

void flute() {
  ch = 4;
  u16 result = 0;
  u16 filtered_data_buf[CHANNEL_NUM] = { 0 };
  u8 baseline_buf[CHANNEL_NUM] = { 0 };

  result = mpr121.check_status_register();

  mpr121.get_filtered_reg_data(&result, filtered_data_buf);

  sensors_event_t temp_event, pressure_event;
  while (!dps.temperatureAvailable() || !dps.pressureAvailable()) {
    return;  // wait until there's something to read
  }
  for (int i = 0; i < CHANNEL_NUM; i++) {
    if (result & (1 << i)) /*key i is pressed!!*/
    {
      if (0 == touch_status_flag[i]) { //押したとき
        touch_status_flag[i] = 1;
        ring = 1;
        //音程の作成
      }
    } else {
      if (1 == touch_status_flag[i]) { //離したとき
        touch_status_flag[i] = 0;
        ring = 0;
        //音程の作成
      }
    }
  }
  delay(50);
}

void note_piano() {
  int pitch[] = {48, 49, 50, 51, 52,
                 53, 54, 55, 56, 57,
                 58, 59, 60, 61, 62,
                 63, 64, 65, 66, 67,
                 68, 69, 70, 71, 72,
                 73, 74, 75, 76, 77
                };

  for (int i = 0; i < sizeof(pitch) / sizeof(pitch[0]); i++) {
    if (sw[i] == 1) {
      if (flag[i] == 0) {
        flag[i] = 1;
        ring = 1;
        note = pitch[i];
      }
    } else {
      if (flag[i] == 1) {
        flag[i] = 0;
        ring = 0;
        note = pitch[i];
      }
    }
    data_send();
  }
}

void angle() {
  float x, y, z;
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);
    angle = atan2(x, z) * 180.0 / PI; //向きは適宜調査
  }
}

void data_send() {
  if (mode == 1) {
    //violinモードは加速度だけ送る
    Serial.print(accel);
    Serial.println();
  } else { //2,3,4モード
    Serial.print(ch);
    Serial.print(",");
    Serial.print(note);
    Serial.print(",");
    Serial.print(ring);
    Serial.print(",");
    Serial.print(velo);
    Serial.print(",");
    Serial.print(mode);
    Serial.println();
  }
}

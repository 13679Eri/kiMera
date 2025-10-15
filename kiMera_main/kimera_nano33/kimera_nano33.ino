#include <Wire.h>
#include "Adafruit_MPR121.h"
#include <Adafruit_LPS35HW.h>
#include <Arduino_LSM6DS3.h>

#define CHANNEL_NUM 12

//タッチセンサ　気圧
Adafruit_LPS35HW lps35hw = Adafruit_LPS35HW();
Adafruit_MPR121 cap = Adafruit_MPR121();
uint16_t touch_status_flag[CHANNEL_NUM] = { 0 };


float baselinePressure;
unsigned long updatetime = 0;
const unsigned long Interval = 5000;        // 基準気圧を毎分更新
const float pressureChangeThreshold = 0.2;  // 圧力変化が0.2hPa未満の場合

//音とflag
int pitch[9] = { 60, 62, 64, 65, 67, 69, 71, 72, 74 };
int prev_note = -1;

int sw[30];    //鍵盤の数
int flag[30];  //フラグ

//送信変数
int ch = 0;
int note = 60;
bool ring = 0;
int velo = 0;
float angle = 0;
int pt = 0;

//受信変数
int mode = 0;
int previousMode = 0;

void setup() {
  //シリアル
  Serial.begin(115200);
  Serial.println("setup start");
  Serial1.begin(115200);  //mode受け取る midi送る
  mode = 1;
  
  set_sensor();
  set_violin();
  set_piano();
  Serial.print("all OK");
}

void loop() {
  set_mode();
}

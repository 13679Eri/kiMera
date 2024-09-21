#include "Seeed_MPR121_driver.h"
#include <Adafruit_LPS35HW.h>
#include <Arduino_LSM6DS3.h>

//タッチセンサ　気圧
Mpr121 mpr121;
u16 touch_status_flag[CHANNEL_NUM] = { 0 };
Adafruit_LPS35HW lps35hw = Adafruit_LPS35HW();

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

void setup() {
  //シリアル
  Serial.begin(115200);
  Serial1.begin(115200);  //mode受け取る midi送る
  set_sensor();
  set_violin();
  set_piano();
}

void loop() {
  rcv_serial();
  if (mode == 1) {
    piano();
  } else if (mode == 2) {
    violin();
  } else if (mode == 3) {
    trumpet();
  } else if (mode == 4) {
    flute();
  }
}

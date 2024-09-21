//フルート
//https://chatgpt.com/share/d8162383-aedc-4bf6-8e95-cf8e48e448fd
#include <Adafruit_LPS35HW.h>
#include "Seeed_MPR121_driver.h"

Adafruit_LPS35HW lps35hw = Adafruit_LPS35HW();

float baselinePressure;
unsigned long updatetime = 0;
const unsigned long Interval = 5000;        // 基準気圧を毎分更新
const float pressureChangeThreshold = 0.2;  // 圧力変化が0.2hPa未満の場合

Mpr121 mpr121;
u16 touch_status_flag[CHANNEL_NUM] = { 0 };

int ch = 4;
int note = 0;
bool ring = 0;
int velo = 0;

//音とflag
int pitch[9] = { 60, 62, 64, 65, 67, 69, 71, 72, 74 };
int prev_note = -1;

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);  //midi送る

  //タッチセンサ
  s32 ret = 0;
  if (mpr121.begin() < 0) {
    Serial.println("Can't detect device!!!!");
  } else {
    Serial.println("mpr121 init OK!");
  }
  mpr121.set_sensitivity(0x60);
  mpr121.set_sensitivity(0x60);
  delay(100);

  //気圧
  if (!lps35hw.begin_I2C()) {
    //if (!lps35hw.begin_SPI(LPS_CS)) {
    //if (!lps35hw.begin_SPI(LPS_CS, LPS_SCK, LPS_MISO, LPS_MOSI)) {
    Serial.println("Couldn't find LPS35HW chip");
    while (1)
      ;
  }
  Serial.println("Found LPS35HW chip");

  baselinePressure = lps35hw.readPressure();
  Serial.print("Baseline Pressure: ");
  Serial.println(baselinePressure);
}

void loop() {
  //気圧
  unsigned long currentMillis = millis();  //今の時間
  float pressure = lps35hw.readPressure();
  // Serial.print("Pressure: ");
  // Serial.print(pressure);
  // Serial.println(" hPa");

  //タッチ
  u16 result = 0;
  u16 filtered_data_buf[CHANNEL_NUM] = { 0 };
  u8 baseline_buf[CHANNEL_NUM] = { 0 };

  result = mpr121.check_status_register();

  mpr121.get_filtered_reg_data(&result, filtered_data_buf);

  for (int i = 0; i < CHANNEL_NUM; i++) {
    if (result & (1 << i)) { /*key i is pressed!!*/
      if (0 == touch_status_flag[i]) {
        touch_status_flag[i] = 1;
        //        Serial.print("key ");
        //        Serial.print(i);
        //        Serial.print("pressed   ");
        //        Serial.println(note);
      }
    } else {
      if (1 == touch_status_flag[i]) {
        touch_status_flag[i] = 0;
        //        Serial.print("key ");
        //        Serial.print(i);
        //        Serial.print("release   ");
        //        Serial.println(note);
      }
    }
    Serial.print(touch_status_flag[i]);
  }
  //  wind_note();
  //  Serial.print("  ");
  //  Serial.print(note);
  Serial.println();

  float preChange = readPreChange();
  //  Serial.println(preChange);
  if (abs(preChange) < 0.05) {
    if (currentMillis - updatetime >= Interval) {
      update_bspre();
      updatetime = currentMillis;
    }
  } else {
    updatetime = currentMillis;
  }


  //  for (int i = 0; i < 9; i++) {
  //    Serial.print(flag[i]);
  //  }
  //  Serial.println();
  if (preChange > 0.20) {
    note = wind_note();
    if (prev_note != note) {
      ring = 1;
      velo = PreChangeToVolume(preChange);
      if (velo > 127) {
        velo = 127;
      }
      data_send(ch, prev_note, 0, 0);
      data_send(ch, note, ring, velo);
      prev_note = note;
    }
  } else {
    if (prev_note != -1) {
      note = -1;
      ring = 0;
      velo = 0;
      data_send(ch, note, ring, velo);
      prev_note = note;
    }
  }
  Serial.println(note);
  //  if (preChange > 0.20) {
  //    ring = 1;
  //    velo = PreChangeToVolume(preChange);
  //    if (velo > 127) {
  //      velo = 127;
  //    }
  //    wind_note();
  //    //    Serial.println(ring);
  //    //    Serial.print(note);
  //    data_send(ch, note, ring, velo);
  //  } else {
  //    ring = 0;
  //    velo = 0;
  //    data_send(ch, note, ring, velo);
  //  }
  delay(100);
}

int PreChangeToVolume(float preChange) {
  // 圧力変化を音量にマップ（適宜調整）
  return map(preChange, -1, 1, 0, 127);
}

void update_bspre() {
  baselinePressure = lps35hw.readPressure();
  //  Serial.print("Updated Baseline Pressure: ");
  //  Serial.println(baselinePressure);
}

float readPreChange() {
  float currentPre = lps35hw.readPressure();
  return currentPre - baselinePressure;  // 圧力変化を計算
}

int wind_note() {
  //   指遣いをビットマスクで表現（1: 押されている, 0: 離されている）
  int fingerPattern = 0;

  for (int i = 0; i < CHANNEL_NUM; i++) {
    if (touch_status_flag[i] == 1) {
      fingerPattern |= (1 << i);
    }
  }

  // 指遣いに対応するノート番号を設定
  switch (fingerPattern) {
    case 0b111100101101:
      return 60;
      break;
    case 0b111100100101:
      return 62;
      break;
    case 0b111100100001:
      return 64;
      break;
    case 0b111100100000:
      return 65;
      break;
    case 0b111000100000:
      return 67;
      break;
    case 0b110000100000:
      return 69;
      break;
    case 0b100000100000:
      return 71;
      break;

      //次のオクターブ

    case 0b010000100000:
    case 0b111100011101:
      return 72;
      break;
    case 0b000000100000:
    case 0b111100010101:
      return 74;
      break;
    case 0b111100010001:
      return 76;
      break;
    case 0b111100010000:
      return 77;
      break;
    case 0b111000010000:
      return 79;
      break;
    case 0b110000010000:
      return 81;
      break;
    case 0b100000010000:
      return 83;
      break;

      //次のオクターブ

    case 0b010000010000:
    case 0b111100001111:
      return 84;
      break;
    case 0b000000010000:
    case 0b111100000111:
      return 86;
      break;
    case 0b111100000011:
      return 88;
      break;
    case 0b111100000010:
      return 89;
      break;
    case 0b111000000010:
      return 91;
      break;
    case 0b110000000010:
      return 93;
      break;
    case 0b100000000010:
      return 95;
      break;

      //次のオクターブ

    case 0b010000000010:
    case 0b111100001101:
      return 96;
      break;
    case 0b000000000010:
    case 0b111100000101:
      return 98;
      break;
    case 0b111100000001:
      return 100;
      break;
    case 0b111100000000:
      return 101;
      break;
    case 0b111000000000:
      return 103;
      break;
    case 0b110000000000:
      return 105;
      break;
    case 0b100000000000:
      return 107;
      break;

    case 0b010000000000:
      return 108;
      break;
    case 0b000000000000:
      return 110;
      break;
    // 他の指遣いパターンを追加
    default:
      // 必要に応じてデフォルトの処理を追加
      break;
  }
  //  Serial.println("///////////");
  //  Serial.println(fingerPattern);
}

void data_send(int ch, int note, bool ring, int velo) {
  Serial1.print(ch);
  Serial1.print(",");
  Serial1.print(note);
  Serial1.print(",");
  Serial1.print(ring);
  Serial1.print(",");
  Serial1.print(velo);
  Serial1.println();

  Serial.print(ch);
  Serial.print(",");
  Serial.print(note);
  Serial.print(",");
  Serial.print(ring);
  Serial.print(",");
  Serial.print(velo);
  Serial.println();
}
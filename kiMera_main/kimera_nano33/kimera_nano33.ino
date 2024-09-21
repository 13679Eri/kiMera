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
  pinInit();
}

void loop() {
  if (Serial1.available()) {      // 受信データを確認する
    mode = (byte)Serial1.read();  // 受信データを読み取る
    Serial.println(mode);
    if (mode == 4) {
      baselinePressure = lps35hw.readPressure();
      Serial.println(baselinePressure);
    }
  }

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

void pinInit() {
  //シリアル
  Serial.begin(115200);
  Serial1.begin(115200);  //mode受け取る midi送る

  //タッチセンサ
  s32 ret = 0;
  if (mpr121.begin() < 0) {
    Serial.println("Can't detect device!!!!");
  } else {
    Serial.println("mpr121 init OK!");
  }
  mpr121.set_sensitivity(0x60);

  //気圧
  if (!lps35hw.begin_I2C()) {
    //if (!lps35hw.begin_SPI(LPS_CS)) {
    //if (!lps35hw.begin_SPI(LPS_CS, LPS_SCK, LPS_MISO, LPS_MOSI)) {
    Serial.println("Couldn't find LPS35HW chip");
    while (1);
  }
  Serial.println("Found LPS35HW chip");

  //IMU
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  //ピアノ
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);

  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);

  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
}

void piano() {
  digitalWrite(12, LOW);
  delay(10);
  digitalWrite(7, HIGH);
  sw[0] = digitalRead(2);
  sw[1] = digitalRead(3);
  sw[2] = digitalRead(4);
  sw[3] = digitalRead(5);
  sw[4] = digitalRead(6);

  digitalWrite(7, LOW);
  delay(10);
  digitalWrite(8, HIGH);
  sw[5] = digitalRead(2);
  sw[6] = digitalRead(3);
  sw[7] = digitalRead(4);
  sw[8] = digitalRead(5);
  sw[9] = digitalRead(6);

  digitalWrite(8, LOW);
  delay(10);
  digitalWrite(9, HIGH);
  sw[10] = digitalRead(2);
  sw[11] = digitalRead(3);
  sw[12] = digitalRead(4);
  sw[13] = digitalRead(5);
  sw[14] = digitalRead(6);

  digitalWrite(9, LOW);
  delay(10);
  digitalWrite(10, HIGH);
  sw[15] = digitalRead(2);
  sw[16] = digitalRead(3);
  sw[17] = digitalRead(4);
  sw[18] = digitalRead(5);
  sw[19] = digitalRead(6);

  digitalWrite(10, LOW);
  delay(10);
  digitalWrite(11, HIGH);
  sw[20] = digitalRead(2);
  sw[21] = digitalRead(3);
  sw[22] = digitalRead(4);
  sw[23] = digitalRead(5);
  sw[24] = digitalRead(6);

  digitalWrite(11, LOW);
  delay(10);
  digitalWrite(12, HIGH);
  sw[25] = digitalRead(2);
  sw[26] = digitalRead(3);
  sw[27] = digitalRead(4);
  sw[28] = digitalRead(5);
  sw[29] = digitalRead(6);

  piano_note();
}

void violin() {
  //  ch = 2;
  float x, y, z;

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);

    //角度
    angle = atan2(x, -z) * 180.0 / PI;
    //    Serial.println(angle);
  }

  //m5からringが来たら加速度送る
  if (Serial1.available()) {  // 受信データを確認する
    if (mode == 2) {
      Serial1.print(angle);
      //      Serial.print("  ring  ");
      Serial.println(angle);
    } else {
      mode = ring;
    }
  }
}

void trumpet() {
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
      }
    } else {
      if (1 == touch_status_flag[i]) {
        touch_status_flag[i] = 0;
      }
    }
  }

  pt = trunpet_patern(touch_status_flag[1], touch_status_flag[4], touch_status_flag[5]);

  if (Serial1.available()) {      // 受信データを確認する
    if (mode == 3) {
      data_send(ch, pt, 1, 127);
    } else {
      mode = ring;
    }
  }
}

void flute() {
  //気圧
  unsigned long currentMillis = millis();  //今の時間
  float pressure = lps35hw.readPressure();

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
      }
    } else {
      if (1 == touch_status_flag[i]) {
        touch_status_flag[i] = 0;
      }
    }
  }

  //基準気圧の更新
  float preChange = readPreChange();
  Serial.println(preChange);
  if (abs(preChange) < 0.09) {
    if (currentMillis - updatetime >= Interval) {
      update_bspre();
      updatetime = currentMillis;
    }
  } else {
    updatetime = currentMillis;
  }

  //吹く
  if (preChange > 0.10) {
    note = flute_note();
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
  delay(100);
}

//piano
void piano_note() {
  ch = 1;
  int pitch[] = { 48, 49, 50, 51, 52,
                  53, 54, 55, 56, 57,
                  58, 59, 60, 61, 62,
                  63, 64, 65, 66, 67,
                  68, 69, 70, 71, 72,
                  73, 74, 75, 76, 77
                };
  for (int i = 0; i < 30; i++) {
    if (sw[i] == 1) {
      if (flag[i] == 0) {
        flag[i] = 1;
        data_send(1, pitch[i], flag[i], 127);
      }
    } else {
      if (flag[i] == 1) {
        flag[i] = 0;
        data_send(1, pitch[i], flag[i], 0);
      }
    }
  }
}

//trumpet
int trunpet_patern(bool flag1, bool flag4, bool flag5) {
  ch = 3;
  int value = 0;

  if (flag1) value |= 1 << 0;  // flag1 が true なら 0ビット目に1をセット
  if (flag5) value |= 1 << 1;  // flag5 が true なら 2ビット目に1をセット
  if (flag4) value |= 1 << 2;  // flag4 が true なら 1ビット目に1をセット

  // value の値に応じて数字を返す
  switch (value) {
    case 0b000: return 0;  // 押さえてない
    case 0b001: return 1;  // flag1 だけが true
    case 0b010: return 2;  // flag4 だけが true
    case 0b100: return 3;  // flag5 だけが true
    case 0b011: return 4;  // flag1 と flag4 が true
    case 0b110: return 5;  // flag4 と flag5 が true
    case 0b101: return 6;  // flag1 と flag5 が true
    case 0b111: return 7;  // flag1, flag4, flag5 が全て true
    default: return -1;    // 上記以外の場合
  }
}

//flute
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

int flute_note() {
  ch = 4;
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

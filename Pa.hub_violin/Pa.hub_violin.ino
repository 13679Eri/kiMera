#include <M5Core2.h>
#include "ClosedCube_TCA9548A.h"
#include <Unit_Sonic.h>　　//超音波
#include "IMU_6886.h"　　　//加速度

ClosedCube::Wired::TCA9548A tca;
const uint8_t JOY_CHANNEL = 0;       // ジョイスティックを接続するチャンネル
const uint8_t ULTRASONIC_CHANNEL = 1; // 超音波センサを接続するチャンネル
const uint8_t IMU_CHANNEL = 2;       // 加速度センサを接続するチャンネル

IMU_6886 imu6886; //加速度
SONIC_I2C sensor; //超音波
#define JOY_ADDR 0x52     //ジョイスティック

//MIDI
#include "M5UnitSynth.h"
M5UnitSynth synth;
int note_color[4] = {1 , 41, 57, 74};

//violinのグローバル変数
float accX = 0;
float accY = 0;
float accZ = 0;

float gyroX = 0;
float gyroY = 0;
float gyroZ = 0;

float distan = 0;
float angle = 0;
uint8_t x_data, y_data, button_data;
bool chord = false;
int note = 0;
int prev_note = -1;

void setup() {
  M5.begin();
  Wire.begin(21, 22);  // Pa.Hub (TCA9548A)が接続されているI2Cピン

  // Pa.Hubの初期化
  tca.address(0x70); // Pa.Hubのデフォルトアドレス

  // ジョイスティックの初期化
  tca.selectChannel(JOY_CHANNEL);
  Wire.beginTransmission(0x52);
  Wire.endTransmission();

  // 超音波センサの初期化
  tca.selectChannel(ULTRASONIC_CHANNEL);
  sensor.begin(&Wire, 0x57);

  // 加速度センサの初期化
  tca.selectChannel(IMU_CHANNEL);
  imu6886.Init(21, 22);

  //MIDI
  synth.begin(&Serial2, UNIT_SYNTH_BAUD, 33, 32);
  for (int i = 0; i < 4; i++) {
    if (ch == i + 1) {
      synth.setInstrument(0, i + 1, note_color[i]);
    }
  }

  ch = 2;
  synth.setInstrument(0, 2, note_color[1]);
}

void loop() {
  M5.update();
  // 各センサのデータ取得例

  // ジョイスティックデータ取得
  tca.selectChannel(JOY_CHANNEL);
  Wire.requestFrom(0x52, 3);
  if (Wire.available() == 3) {
    x_data = Wire.read();
    y_data = Wire.read();
    button_data = Wire.read();
  }

  // 超音波センサデータ取得
  tca.selectChannel(ULTRASONIC_CHANNEL);
  distan = sensor.getDistance();

  // 加速度データ取得
  tca.selectChannel(IMU_CHANNEL);
  imu6886.getAccelData(&accX, &accY, &accZ);

  // データ表示など
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(0, 20);
  M5.Lcd.printf("Joystick X: %d, Y: %d, Button: %d\n", x_data, y_data, button_data);
  M5.Lcd.printf("Distance: %.2f cm\n", distan);
  //  M5.Lcd.printf("Accel X: %.2f, Y: %.2f, Z: %.2f\n", accX, accY, accZ);

  violin();
  M5.Lcd.println(angle);
  M5.Lcd.println(note);

  //  delay(100);
}

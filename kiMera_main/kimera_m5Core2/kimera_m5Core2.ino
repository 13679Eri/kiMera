#include <M5Core2.h>

//センサ
//ヴァイオリン
//pa.hub
#include "ClosedCube_TCA9548A.h"
#include <Unit_Sonic.h>　　//超音波
#include "IMU_6886.h"　　　//加速度

ClosedCube::Wired::TCA9548A tca;
const uint8_t JOY_CHANNEL = 1;       // ジョイスティックを接続するチャンネル
const uint8_t ULTRASONIC_CHANNEL = 2; // 超音波センサを接続するチャンネル
const uint8_t IMU_CHANNEL = 3;       // 加速度センサを接続するチャンネル

IMU_6886 imu6886; //加速度
SONIC_I2C sensor; //超音波
#define JOY_ADDR 0x52     //ジョイスティック

//violinのグローバル変数
float accX = 0;
float accY = 0;
float accZ = 0;

float gyroX = 0;
float gyroY = 0;
float gyroZ = 0;

float previousDistan = 0;
float distan = 0;
float angle = 0;
uint8_t prev_x_data;
uint8_t x_data, y_data, button_data;
bool chord = false;
float mainbody_ag = 0;

//トランペット
#include <driver/i2s.h>
#include "fft.h"
#include <math.h>

#define PIN_CLK  19
#define PIN_DATA 27
#define MODE_MIC 0

int patern = 0;

//MIDI
#include "M5UnitSynth.h"
M5UnitSynth synth;
int note_color[4] = {1 , 41, 57, 74};

// 受信データ格納
int ch = 0;
int note = 0;
bool ring = 0;
int velo = 0;

int prev_note = -1;

//送信データ
int mode = 0;
int previousMode = 0;

//ボタン
ButtonColors cl_on  = {0x7BEF, 0x7f7f7f, 0x7f7f7f}; // タップした時の色 (背景, 文字列, ボーダー)
ButtonColors cl_p = {BLACK, WHITE, BLACK}; // 指を離した時の色 (背景, 文字列, ボーダー)
ButtonColors cl_v = {0xCB8D4A, WHITE, 0xCB8D4A}; // 指を離した時の色 (背景, 文字列, ボーダー)
ButtonColors cl_t = {0xFFDD31, WHITE, 0xFFDD31}; // 指を離した時の色 (背景, 文字列, ボーダー)
ButtonColors cl_f = {0xDEDCD7, WHITE, 0xDEDCD7}; // 指を離した時の色 (背景, 文字列, ボーダー)

Button btn_p(200, 20, 90, 35, false , "piano", cl_p, cl_on);
Button btn_v(200, 75, 90, 35, false , "violin", cl_v, cl_on);
Button btn_t(200, 130, 90, 35, false , "trumpet", cl_t, cl_on);
Button btn_f(200, 185, 90, 35, false , "flute", cl_f, cl_on);

void setup() {
  M5.begin();

  //シリアル
  //  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, 13, 14); //nanoから受け取る
  Serial2.begin(115200, SERIAL_8N1, 3, 1); //MIDI

  //ボタン
  set_button();

  //センサ 超音波 加速度 JOYSTICK PDM MIDI
  set_sensor();

  //最初のモード変更
  mode = 1;
}

void loop() {
  M5.update();

  // モードが変更された場合のみセット関数を呼び出す
  if (mode != previousMode) {
    Serial1.write(mode);
    for (int i = 1; i <= 4; i++) {
      synth.setAllNotesOff(i);
      //      M5.Lcd.setCursor(35, 100);
      //      M5.Lcd.print("AllNoteoff");
    }
    if (mode == 1) {
      set_piano();
      //      Serial.println("piano");
    } else if (mode == 2) {
      set_violin();
      //      Serial.println("violin");
    } else if (mode == 3) {
      set_trumpet();
      //      Serial.println("trumpet");
    } else if (mode == 4) {
      set_flute();
      //      Serial.println("flute");
    }
    previousMode = mode; // 前回のモードを更新
  }

  // 現在のモードに基づいて関数を呼び出す
  switch (mode) {
    case 1:
      piano();
      break;
    case 2:
      violin();
      break;
    case 3:
      trumpet();
      break;
    case 4:
      flute();
      break;
  }
  delay(10); // 少し待機してから再度ループ
}

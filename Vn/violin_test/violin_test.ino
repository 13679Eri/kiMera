//ヴァイオリン
#include <M5Core2.h>
#include <Unit_Sonic.h>　　//超音波
#include "IMU_6886.h"　　　//加速度
#include "M5UnitSynth.h"

IMU_6886 imu6886;
SONIC_I2C sensor;
#define JOY_ADDR 0x52     //ジョイスティック
M5UnitSynth synth;
bool chord = false;
float accX = 0;
float accY = 0;
float accZ = 0;

float gyroX = 0;
float gyroY = 0;
float gyroZ = 0;

float distan = 0;
float angle = 0;
uint8_t x_data, y_data, button_data;

int flag = 0;
int note = 0;
int prev_note = -1;

//0 stop
//1 down
//2 up

void setup() {
  M5.begin();
  Serial.begin(115200);

  Wire.begin(21, 22); //joy
  sensor.begin(&Wire, 0x57, 21, 22, 400000L); //超音波
  imu6886.Init(21, 22); //加速度

  //MIDI
  synth.begin(&Serial2, UNIT_SYNTH_BAUD, 33, 32);
}

void loop() {
  M5.update();

  //IMU
  imu6886.getGyroData(&gyroX, &gyroY, &gyroZ);
  imu6886.getAccelData(&accX, &accY, &accZ);

  //角度
  angle = atan2(accX, accZ) * 180.0 / PI;
  //ultora_sonic
  distan = sensor.getDistance() / 10;

  Wire.requestFrom(JOY_ADDR, 3);  //Request 3 bytes from the slave device.  向从设备请求3个字节
  if (Wire.available()) {  //If data is received.  如果接收到数据
    x_data = Wire.read();
    y_data = Wire.read();
    button_data = Wire.read();
  }

  if (120 <= x_data && x_data <= 130) {
    flag = 0;
    note = -1;
    //止める
    synth.setAllNotesOff(2);
    synth.setInstrument(0, 2, 41);
    prev_note = note;
    Serial.printf(" note= %d  ", note);
  } else if (130 < x_data) {
    note = violin_Pitch(angle, distan);
    if (prev_note != note) {
      flag = 1;
      synth.setNoteOff(2, prev_note, 0);
      synth.setNoteOn(2, note, 127);
      prev_note = note;
      Serial.printf(" angle= %f  ", angle);
      Serial.printf(" distance= %f", distan);
      Serial.print(" up");
      Serial.printf(" note= %d  ", note);
      Serial.println();
    }
  } else if (x_data < 120) {
    note = violin_Pitch(angle, distan);
    if (prev_note != note) {
      flag = 2;
      synth.setNoteOff(2, prev_note, 0);
      synth.setNoteOn(2, note, 127);
      prev_note = note;
      Serial.printf(" angle= %d", angle);
      Serial.printf(" distance= %d", distan);
      Serial.print(" up");
      Serial.printf(" note= %d", note);
      Serial.println();
    }
  }
  Serial.println(angle);
}

int violin_Pitch(float angle, float newvalue) {
  double dis[] = {28, 26.5, 24.5, 22.5, 20.5, 19.5, 18.5};
  int size = sizeof(dis) / sizeof(dis[0]);

  if (35 <= angle && angle <= 45) { // G弦
    chord = false;
    int notes[] = {55, 56, 57, 58, 59, 60, 61};
    return determineNote(dis, notes, size, newvalue);
  }

  if (15 <= angle && angle <= 25) { // D弦
    chord = false;
    int notes[] = {62, 63, 64, 65, 66, 67, 68};
    return determineNote(dis, notes, size, newvalue);
  }

  if (-25 <= angle && angle <= -15) { // A弦
    chord = false;
    int notes[] = {69, 70, 71, 72, 73, 74, 75};
    return determineNote(dis, notes, size, newvalue);
  }

  if (-45 <= angle && angle <= -35) { // E弦
    chord = false;
    int notes[] = {76, 77, 78, 79, 80, 81, 82};
    return determineNote(dis, notes, size, newvalue);
  }

  return -2; // エラーハンドリング
}


int determineNote(double dis[], int notes[], int size, float newvalue) {
  for (int i = 0; i < size; i++) {
    if (i == 0) {
      if (dis[i] <= newvalue) {
        return notes[i];
      }
    } else {
      if (dis[i] <= newvalue && newvalue <= dis[i - 1]) {
        return notes[i];
      }
    }
  }
  return notes[size - 1]; // デフォルトで最後の音符を返す
}

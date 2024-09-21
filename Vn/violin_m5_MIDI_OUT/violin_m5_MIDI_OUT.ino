//M5から
//violin できてるやつ

#include <M5Core2.h>

//センサ
#include <Unit_Sonic.h>　　//超音波
#define JOY_ADDR 0x52     //ジョイスティック

const bool LCD_Enable = true;
const bool SD_Enable = true;
const bool Serial_Enable = true;
const bool I2C_Enable = true;

#ifdef __cplusplus
extern "C"
{
#include "IMU_6886.h"
}
#endif
//加速度

IMU_6886 imu6886;
SONIC_I2C sensor;

//MIDI
#include <MIDI.h>
MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI);
#define DEVICE_NAME  "M5Core2 MIDI"

//これで音変わる？
#define MIDI_CH  1
int pn = 41;

void setup() {
  M5.begin();
  Wire.begin(32, 33);
  
  sensor.begin();
  imu6886.Init(32, 33);

//  dacWrite(26, 0);
  MIDI.begin();
  MIDI.turnThruOff();
  MIDI.sendProgramChange(pn, MIDI_CH);
}

//必要なグローバル変数
float accX = 0;
float accY = 0;
float accZ = 0;

float angle = 0;

float gyroX = 0;
float gyroY = 0;
float gyroZ = 0;

int note = 0;
int note2 = 0;
char data[100]; //文字列
int i = 0;
bool chord = false;
bool isConnected = false;
bool isSounding = false;
float newvalue = 0;
int kamae = 0;

uint8_t x_data, y_data, button_data;

void loop() {
  //MIDI progrum change
  MIDI.sendProgramChange(pn, MIDI_CH);
  
  //IMU
  imu6886.getGyroData(&gyroX, &gyroY, &gyroZ);
  imu6886.getAccelData(&accX, &accY, &accZ);

  //角度
  kamae = 30;
  angle = atan2(accX, accZ) * 180.0 / PI + kamae;

  Wire.requestFrom(JOY_ADDR, 3);  //Request 3 bytes from the slave device.  向从设备请求3个字节
  if (Wire.available()) {  //If data is received.  如果接收到数据
    x_data = Wire.read();
    y_data = Wire.read();
    button_data = Wire.read();

    sprintf(data, "x:%d y:%d button:%d\n", x_data, y_data, button_data);
    //    Serial.print(data);
  }

  //joystickで音が出る
  if (isSounding == true && 120 <= x_data && x_data <= 130) {
    isSounding = false;
    noteOff(MIDI_CH, note, 0);
  } else if (isSounding == false && 130 < x_data) {
    noteOff(MIDI_CH, note, 0);
    isSounding = true;
    //ultora_sonic
    newvalue = sensor.getDistance() / 10;
    soundPitch();
    noteOn(MIDI_CH, note, 127);
  } else if (isSounding == false && x_data < 120) {
    noteOff(MIDI_CH, note, 0);
    //    chordOff(note, note2);
    isSounding = true;
    //ultora_sonic
    newvalue = sensor.getDistance() / 10;
    soundPitch();
    noteOn(MIDI_CH, note, 127);
  }

  //画面
  M5.Lcd.fillRect(0, 0, 320, 240, BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.print(angle);
  M5.Lcd.setCursor(0, 125);
  if (( newvalue < 400 ) && ( newvalue > 2 )) {
    M5.Lcd.printf("%.2fmm \r\n", newvalue);
    M5.Lcd.println(note);
    M5.Lcd.println(note2);
  }
  M5.Lcd.setCursor(0, 175);
  M5.Lcd.print(x_data);
  delay(100);
}

void noteOn(uint8_t channel, uint8_t pitch, uint8_t velocity) {
  MIDI.sendNoteOn(pitch, velocity, channel);
}
void noteOff(uint8_t channel, uint8_t pitch, uint8_t velocity) {
  MIDI.sendNoteOff(pitch, velocity, channel);
}


void soundPitch() {
  //G
  if (35 <= angle && angle <= 45) {
    chord = false;
    double dis[] = {28, 26.5, 24.5, 22.5, 20.5, 19.5, 28};
    int notes[] = {55, 56, 57, 58, 59, 60, 61};

    for (int i = 0; i < sizeof(dis) / sizeof(dis[0]); i++) {
      if (i == 0) {
        if (dis[i] <= newvalue) {
          note = notes[i];
          break;
        }
      } else {
        if (dis[i] <= newvalue && newvalue <= dis[i - 1]) {
          note = notes[i];
          break;
        }
      }
    }
  }

  //D
  if (15 <= angle && angle <= 25) {
    chord = false;
    double dis[] = {28, 26.5, 24.5, 22.5, 20.5, 19.5, 28};
    int notes[] = {62, 63, 64, 65, 66, 67, 68};

    for (int i = 0; i < sizeof(dis) / sizeof(dis[0]); i++) {
      if (i == 0) {
        if (dis[i] <= newvalue) {
          note = notes[i];
          break;
        }
      } else {
        if (dis[i] <= newvalue && newvalue <= dis[i - 1]) {
          note = notes[i];
          break;
        }
      }
    }
  }

  //A
  if (-25 <= angle && angle <= -15) {
    chord = false;
    double dis[] = {28, 26.5, 24.5, 22.5, 20.5, 19.5, 28};
    int notes[] = {69, 70, 71, 72, 73, 74, 75};

    for (int i = 0; i < sizeof(dis) / sizeof(dis[0]); i++) {
      if (i == 0) {
        if (dis[i] <= newvalue) {
          note = notes[i];
          break;
        }
      } else {
        if (dis[i] <= newvalue && newvalue <= dis[i - 1]) {
          note = notes[i];
          break;
        }
      }
    }
  }

  //E
  if (-45 <= angle && angle <= -35) {
    chord = false;
    double dis[] = {28, 26.5, 24.5, 22.5, 20.5, 19.5, 28};
    int notes[] = {76, 77, 78, 79, 80, 81, 82};

    for (int i = 0; i < sizeof(dis) / sizeof(dis[0]); i++) {
      if (i == 0) {
        if (dis[i] <= newvalue) {
          note = notes[i];
          break;
        }
      } else {
        if (dis[i] <= newvalue && newvalue <= dis[i - 1]) {
          note = notes[i];
          break;
        }
      }
    }
  }
}

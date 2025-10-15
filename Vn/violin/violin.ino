//最終
#include <M5Core2.h>
#include <Unit_Sonic.h>
#define JOY_ADDR 0x52  //define Joystick I2C address.  定义摇杆的I2C地址


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

IMU_6886 imu6886;
SONIC_I2C sensor;

//MIDI
#include <BLEMIDI_Transport.h>
#include <hardware/BLEMIDI_ESP32_NimBLE.h>

#define DEVICE_NAME  "M5Core2 MIDI"
BLEMIDI_CREATE_INSTANCE(DEVICE_NAME, MIDI);

void setup() {
  M5.begin();
  Wire.begin(21, 22);
  sensor.begin(&Wire, 0x57, 21, 22, 400000L);
  imu6886.Init(21, 22);

  //BLE-MIDI
  MIDI.begin();
  //接続
  BLEMIDI.setHandleConnected(OnConnected);
  BLEMIDI.setHandleDisconnected(OnDisconnected);
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
float dis[10];
float x[10];
int i = 0;
bool chord = false;
bool isConnected = false;
bool isSounding = false;
float newvalue = 0;
int kamae = 0;

uint8_t x_data, y_data, button_data;

void loop() {
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
    noteOff(0, note, 0);
  } else if (isSounding == false && 130 < x_data) {
    noteOff(0, note, 0);
    isSounding = true;
    //ultora_sonic
    newvalue = sensor.getDistance() / 10;
    soundPitch();
    noteOn(0, note, 127);
  } else if (isSounding == false && x_data < 120) {
    noteOff(0, note, 0);
    //    chordOff(note, note2);
    isSounding = true;
    //ultora_sonic
    newvalue = sensor.getDistance() / 10;
    soundPitch();
    noteOn(0, note, 127);
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

//BLE-MIDI送信
void noteOn(uint8_t channel, uint8_t pitch, uint8_t velocity) {
  MIDI.sendNoteOn(pitch, velocity, 1);
}
void noteOff(uint8_t channel, uint8_t pitch, uint8_t velocity) {
  MIDI.sendNoteOff(pitch, velocity, 1);
}

// コード送信
void chordOn(uint8_t pitch1, uint8_t pitch2) {
  MIDI.sendNoteOn(pitch1, 127, 1);
  MIDI.sendNoteOn(pitch2, 127, 1);
}
void chordOff(uint8_t pitch1, uint8_t pitch2) {
  MIDI.sendNoteOff(pitch1, 0, 1);
  MIDI.sendNoteOff(pitch2, 0, 1);
}

//角度で弦、距離で音程
void soundPitch() {
  //G
  if (35 <= angle && angle <= 45) {
    chord = false;
    if (28 <= newvalue) {
      note = 55;
    } else if (26.5 <= newvalue && newvalue <= 28) {
      note = 56;
    } else if (24.5 <= newvalue && newvalue < 26.5) {
      note = 57;
    } else if (22.5 <= newvalue && newvalue < 24.5) {
      note = 58;
    } else if (20.5 <= newvalue && newvalue < 22.5) {
      note = 59;
    } else if (19.5 <= newvalue && newvalue < 20.5) {
      note = 60;
    } else if (18 <= newvalue && newvalue < 19.5) {
      note = 61;
    }
  }

  //  //G + D
  //  if (-7 <= angle && angle <= 0) {
  //    chord = true;
  //    if (28 <= newvalue) {
  //      note = 55;
  //      note2 = 62;
  //    } else if (26.5 <= newvalue && newvalue <= 28) {
  //      note = 56;
  //      note2 = 63;
  //    } else if (24.5 <= newvalue && newvalue < 26.5) {
  //      note = 57;
  //      note2 = 64;
  //    } else if (22.5 <= newvalue && newvalue < 24.5) {
  //      note = 58;
  //      note2 = 65;
  //    } else if (20.5 <= newvalue && newvalue < 22.5) {
  //      note = 59;
  //      note2 = 66;
  //    } else if (19.5 <= newvalue && newvalue < 20.5) {
  //      note = 60;
  //      note2 = 67;
  //    } else if (18 <= newvalue && newvalue < 19.5) {
  //      note = 61;
  //      note2 = 68;
  //    }
  //  }

  //D
  if ( 15 <= angle && angle <= 25) {
    chord = false;
    if (28 <= newvalue) {
      note = 62;
    } else if (26.5 <= newvalue && newvalue <= 28) {
      note = 63;
    } else if (24.5 <= newvalue && newvalue < 26.5) {
      note = 64;
    } else if (22.5 <= newvalue && newvalue < 24.5) {
      note = 65;
    } else if (20.5 <= newvalue && newvalue < 22.5) {
      note = 66;
    } else if (19.5 <= newvalue && newvalue < 20.5) {
      note = 67;
    } else if (18 <= newvalue && newvalue < 19.5) {
      note = 68;
    }
  }

  //  //D + A
  //  if (-35 <= angle && angle <= -15) {
  //    chord = true;
  //    if (28 <= newvalue) {
  //      note = 62;
  //      note2 = 69;
  //    } else if (26.5 <= newvalue && newvalue <= 28) {
  //      note = 63;
  //      note2 = 70;
  //    } else if (24.5 <= newvalue && newvalue < 26.5) {
  //      note = 64;
  //      note2 = 71;
  //    } else if (22.5 <= newvalue && newvalue < 24.5) {
  //      note = 65;
  //      note2 = 72;
  //    } else if (20.5 <= newvalue && newvalue < 22.5) {
  //      note = 66;
  //      note2 = 73;
  //    } else if (19.5 <= newvalue && newvalue < 20.5) {
  //      note = 67;
  //      note2 = 74;
  //    } else if (18 <= newvalue && newvalue < 19.5) {
  //      note = 68;
  //      note2 = 75;
  //    }
  //  }

  //A
  if (-25 <= angle && angle <= -15) {
    chord = false;
    if (28 <= newvalue) {
      note = 69;
    } else if (26.5 <= newvalue && newvalue <= 28) {
      note = 70;
    } else if (24.5 <= newvalue && newvalue < 26.5) {
      note = 71;
    } else if (22.5 <= newvalue && newvalue < 24.5) {
      note = 72;
    } else if (20.5 <= newvalue && newvalue < 22.5) {
      note = 73;
    } else if (19.5 <= newvalue && newvalue < 20.5) {
      note = 74;
    } else if (18 <= newvalue && newvalue < 19.5) {
      note = 75;
    }
  }

  //  //A + E
  //  if (-65 <= angle && angle <= -45) {
  //    chord = true;
  //    if (28 <= newvalue) {
  //      note = 69;
  //      note2 = 76;
  //    } else if (26.5 <= newvalue && newvalue <= 28) {
  //      note = 70;
  //      note2 = 77;
  //    } else if (24.5 <= newvalue && newvalue < 26.5) {
  //      note = 71;
  //      note2 = 78;
  //    } else if (22.5 <= newvalue && newvalue < 24.5) {
  //      note = 72;
  //      note2 = 79;
  //    } else if (20.5 <= newvalue && newvalue < 22.5) {
  //      note = 73;
  //      note2 = 80;
  //    } else if (19.5 <= newvalue && newvalue < 20.5) {
  //      note = 74;
  //      note2 = 81;
  //    } else if (18 <= newvalue && newvalue < 19.5) {
  //      note = 75;
  //      note2 = 82;
  //    }
  //  }

  //E
  if (-45 <= angle && angle <= -35) {
    chord = false;
    if (28 <= newvalue) {
      note = 76;
    } else if (26.5 <= newvalue && newvalue <= 28) {
      note = 77;
    } else if (24.5 <= newvalue && newvalue < 26.5) {
      note = 78;
    } else if (22.5 <= newvalue && newvalue < 24.5) {
      note = 79;
    } else if (20.5 <= newvalue && newvalue < 22.5) {
      note = 80;
    } else if (19.5 <= newvalue && newvalue < 20.5) {
      note = 81;
    } else if (18 <= newvalue && newvalue < 19.5) {
      note = 82;
    }
  }
}

//BLE
void OnConnected() {
  isConnected = true;
  //画面
  M5.Lcd.printf("Connected.   ");
}

void OnDisconnected() {
  isConnected = false;
  //画面
  M5.Lcd.printf("Disconnected.");
}

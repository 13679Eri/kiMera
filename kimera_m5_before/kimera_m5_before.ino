//M5から
//violin できてるやつ

#include <M5Core2.h>

//センサ
#include <Unit_Sonic.h>　　//超音波
#include "IMU_6886.h"　　　//加速度
#define JOY_ADDR 0x52     //ジョイスティック

const bool LCD_Enable = true;
const bool SD_Enable = true;
const bool Serial_Enable = true;
const bool I2C_Enable = true;

IMU_6886 imu6886;
SONIC_I2C sensor;

//MIDI
#include "M5UnitSynth.h"
M5UnitSynth synth;
int note_color[4] = {1 , 41, 57, 74};
int nc = 0;

#define DEVICE_NAME  "KIMERA"

char data[100]; //文字列

// 受信データ格納
int ch = 0;
int note = 0;
bool ring = 0;
int velo = 0;

//送信データ
int mode = 0;

void setup() {
  M5.begin();

  //センサ
  Wire.begin(21, 22);
  sensor.begin();
  imu6886.Init(21, 22);

  //シリアル
  Serial1.begin(115200, SERIAL_8N1, 3, 1); //nanoから受け取る
  Serial2.begin(115200, SERIAL_8N1, 13, 14); //RXTX

  //MIDI
  synth.begin(&Serial2, UNIT_SYNTH_BAUD, 33, 32);
  synth.setInstrument(0, 1, 1);  // ピアノ

  //画面
}

//必要なグローバル変数
float accX = 0;
float accY = 0;
float accZ = 0;

float angle = 0;

float gyroX = 0;
float gyroY = 0;
float gyroZ = 0;

//int note = 0;
//int note2 = 0;

bool chord = false;
bool isConnected = false;
bool isSounding = false;

float newvalue = 0;
uint8_t x_data, y_data, button_data;

void loop() {
  M5.update();
  serial_send();

  //画面
  M5.Lcd.fillRect(0, 0, 320, 240, BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.print("mode= ");
  M5.Lcd.print(mode);
  M5.Lcd.setCursor(0, 100);
  M5.Lcd.print("angle= ");
  M5.Lcd.print(angle);
  M5.Lcd.setCursor(0, 125);
  M5.Lcd.print(ch);
  M5.Lcd.print(note);
  M5.Lcd.print(ring);
  M5.Lcd.print(velo);
  //  if (( newvalue < 400 ) && ( newvalue > 2 )) {
  //    M5.Lcd.printf("%.2fmm \r\n", newvalue);
  //    M5.Lcd.println(note);
  ////    M5.Lcd.println(note2);
  //  }
  //  M5.Lcd.setCursor(0, 175);
  //  M5.Lcd.print(x_data);
  delay(100);
}

void serial_send() {
  //メソッドの中に配列初期化してると文字化け？？する
  //グローバル変数にしたら治った
  if (M5.BtnA.wasPressed()) {
    mode++;
    if (mode == 5) mode = 1;
    Serial1.write(mode);
    for (int i = 0; i < 4; i++) {
      synth.setAllNotesOff(i);
    }

    if (mode == 1) {
      nano_ins();
    } else if (mode == 2) {
      viollin();
    } else if (mode == 3) {
      nano_ins();
    } else if (mode == 4) {
      nano_ins();
    }
  }

  M5.Lcd.setCursor(10, 20);
  M5.Lcd.println(mode);
}

int split(String data, char delimiter, String *dst) {
  int index = 0;
  int datalength = data.length();
  for (int i = 0; i < datalength; i++) {
    char tmp = data.charAt(i);
    if (tmp == delimiter) {
      index++;
    } else {
      dst[index] += tmp;
    }
    return (index + 1);
  }
}

void nano_ins() {
  if (Serial1.available()) {          // 受信データを確認する
    String myString = Serial1.readStringUntil('\n');
    // 分割数 = 分割処理(文字列, 区切り文字, 配列)
    int index = split(myString, ',', cmds);

    ch = cmds[0].toInt();
    note = cmds[1].toInt();
    ring = cmds[2].toInt();
    velo = cmds[3].toInt();

    //いきなり全部チャンネル設定するとダメみたい
    //送られたchによって変えることにした
    for (int i = 0; i < 4; i++) {
      if (ch == i + 1) {
        synth.setInstrument(0, i + 1, note_color[i]);
        M5.Lcd.setCursor(10, 200);
        M5.Lcd.printf("nc = %d", note_color[i]);
      }
    }

    //鳴らす
    if (ring == 1) {
      synth.setNoteOn(ch, note, velo);
    } else {
      synth.setNoteOff(ch, note, velo);
    }

  }
}

void viollin() {
  //IMU
  imu6886.getGyroData(&gyroX, &gyroY, &gyroZ);
  imu6886.getAccelData(&accX, &accY, &accZ);

  //角度
  angle = atan2(accX, accZ) * 180.0 / PI;

  Wire.requestFrom(JOY_ADDR, 3);  //Request 3 bytes from the slave device.  向从设备请求3个字节
  if (Wire.available()) {  //If data is received.  如果接收到数据
    x_data = Wire.read();
    y_data = Wire.read();
    button_data = Wire.read();

    sprintf(data, "x:%d y:%d button:%d\n", x_data, y_data, button_data);
//    Serial.print(data);
  }


  synth.setInstrument(0, 2, note_color[1]);
  ch = 2;
  //joystickで音が出る
  if (isSounding == true && 120 <= x_data && x_data <= 130) {
    isSounding = false;
    synth.setNoteOff(ch, note, 0);
  } else if (isSounding == false && 130 < x_data) {
    synth.setNoteOff(ch, note, 0);
    isSounding = true;
    //ultora_sonic
    newvalue = sensor.getDistance() / 10;
    soundPitch();
    synth.setNoteOn(ch, note, 127);
    Serial.println(angle);
    Serial.println(newvalue);
  } else if (isSounding == false && x_data < 120) {
    synth.setNoteOff(ch, note, 0);
    //    chordOff(note, note2);
    isSounding = true;
    //ultora_sonic
    newvalue = sensor.getDistance() / 10;
    soundPitch();
    synth.setNoteOn(ch, note, 127);
    Serial.println(angle);
    Serial.println(newvalue);
  }
}

void soundPitch() {
  Serial1.write(isSounding);
  int mainbody_ag = 0;
  //本体の角度
  if (Serial1.available()) {
    byte Signal = (byte)Serial1.read();
    mainbody_ag = Signal;
    angle += mainbody_ag;
  }

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

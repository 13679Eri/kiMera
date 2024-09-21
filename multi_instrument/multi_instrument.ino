//複合楽器
//M5Core2
/*
   テストするべきこと
   ①RXTX送受信
   　M5⇨nano ピアノモードにします
   　nano⇨M5 音色(チャンネル)、音程、音量を送る
   　M5⇨nano ヴァイオリンモードにします
   　nano⇨M5 nanoの方の加速度を逐一送る、M5の方はjoystick動かした時に弓の角度と本体の角度で算出
   　M5⇨nano トランペット、フルートモードにします
   　nano⇨M5 音色(チャンネル)、音程、気圧センサの値を正した音量を送る
   ②↑の時にシリアルから変数を読む方法を調べる
   ③

*/


#include <M5Core2.h>
//MIDI
#include <MIDI.h>
MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI);
int MIDI_CH = 1;

// 分割された文字列を格納する配列
int data[5];

// #define データの種類 定数　という形で、データの種類を定義していきます。
#define CH 1
#define NOTE 2
#define RING 3
#define VELO 4
#define MODE 5

// 受信データ格納
int ch = 0;
int note = 0;
bool ring = 0;
int velo = 0;
int mode = 0;

//センサ
#include <Unit_Sonic.h>　　//超音波
#define JOY_ADDR 0x52     //ジョイスティック
IMU_6886 imu6886;
SONIC_I2C sonic;

//ピアノ
//バイオリン
float accX = 0;
float accY = 0;
float accZ = 0;
float bodyAngle = 0;
float bowAngle = 0;
float angle = 0;
uint8_t x_data, y_data, button_data;
bool isSounding = false;
float finger_position = 0;
//管楽器

//楽器のモードごとに音色を毎回変えるか、チャンネルで分けるか、多分後者がセオリー通り
int inst[4]; //楽器の種類　ピアノ、バイオリン、トランペット、フルート
int pn = 41; //音色

void setup() {
  //シリアル MIDIとシリアル変えればいい Serial1
  Serial1.begin(9600); //nanoちゃんとの通信
  M5.begin();
  MIDI.begin();
  MIDI_set();

  Wire.begin(32, 33); //i2c joystick
  sonic.begin(); //超音波
  imu6886.Init(32, 33); //加速度
}

void loop() {
  //画面

  //ボタン押したらモードが変わる
//  if (wasPressed()) {
//    send_mode();
//  }

  while (Serial.available()) // 受信データを確認する
  {
    String myString = Serial.readStringUntil('\n');
    // 分割数 = 分割処理(文字列, 区切り文字, 配列)
    Serial.println(myString);

    stringToIntValues(myString, data, ',');

    Serial.println(data[0]);
    Serial.println(data[1]);
    Serial.println(data[2]);
    Serial.println(data[3]);
    Serial.println(data[4]);

    ch = data[0];
    note = data[1];
    ring = data[2];
    velo = data[3];
    mode = data[4];

    if (ring == 1)
    {
      noteOn(ch, note, velo);
    }
    else
    {
      noteOff(ch, note, velo);
    }
  }
}

void noteOn(uint8_t channel, uint8_t pitch, uint8_t velocity, uint8_t note_color) {
  MIDI.sendProgramChange(note_color, MIDI_CH);
  MIDI.sendNoteOn(pitch, velocity, channel);
}

void noteOff(uint8_t channel, uint8_t pitch) {
  MIDI.sendNoteOff(pitch, 0, channel);
}

void violin() {
  MIDI_CH = 2;
  //IMU
  imu6886.getGyroData(&gyroX, &gyroY, &gyroZ);
  imu6886.getAccelData(&accX, &accY, &accZ);

  //角度
  Serial1.read(); //nanoで読んだ角度を読み取る ⇨ bodyAngleに格納
  bowAngle = atan2(accX, accZ) * 180.0 / PI;
  angle = bowAngle + bodyAngle;

  //joystick
  Wire.requestFrom(JOY_ADDR, 3);
  if (Wire.available()) {
    x_data = Wire.read();
    y_data = Wire.read();
    button_data = Wire.read();
  }

  //joystickで音が出る
  if (isSounding == true && 120 <= x_data && x_data <= 130) { //真ん中で音が出ないようにする
    isSounding = false;
    noteOff(MIDI_CH, note);
  } else if (isSounding == false && 130 < x_data) {
    noteOff(MIDI_CH, note); //鳴ってるやつを消してから
    isSounding = true;
    //ultora_sonic
    finger_position = sensor.getDistance() / 10;
    soundPitch();
    noteOn(MIDI_CH, note, 127);
  } else if (isSounding == false && x_data < 120) {
    noteOff(MIDI_CH, note); //鳴ってるやつを消してから
    isSounding = true;
    //ultora_sonic
    finger_position = sensor.getDistance() / 10;
    soundPitch();
    noteOn(MIDI_CH, note, 127);
  }
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

void piano() {
  int pn = ; //1か2かなんか　ピアノの音
  int sw[30];       //鍵盤の数
  int flag[30];     //フラグ
  int pitch[] = {48, 49, 50, 51, 52, 53, 54, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83};
  Serial1.read(); //sw[]の配列を受け取ればいい
  for (int i = 0; i < sizeof(pitch) / sizeof(pitch[0]); i++) {
    if (sw[i] == 1) {
      if (flag[i] == 0) {
        flag[i] = 1;
        noteOn(1, pitch[i], 127, pn);
      }
    } else {
      if (flag[i] == 1) {
        flag[i] = 0;
        noteOff(1, pitch[i]);
      }
    }
  }
}
void send_mode() {
  //モード選択したら↓
  //  if (button1 isPressed()) {
  //    mode = 1;
  //    violin();
  //  } else if (button2) {
  //    mode = 2
  //  } else if (button3) {
  //    mode = 3;
  //    trumpet();
  //  } else if (button4) {
  //    mode = 4;
  //    flute();
  //  }

}

void MIDI_set()
{
  // 2. MIDI.turnThruOff()の追加
  // これは、もともと必要だったのかも？
  // コントローラーだけ作ってる分には問題にはならないかもしれませんが、
  // MIDI OUTしたものがMIDI INにループバックするのを防ぐために、
  // setup()でMIDI.turnThruOff()を呼び出すよう追加しています。
  MIDI.turnThruOff();
  // MIDI.sendProgramChange(音色の番号, MIDIチャンネル);
  MIDI.sendProgramChange(1, 1);  // ピアノ
  MIDI.sendProgramChange(41, 2); // ヴァイオリン
  MIDI.sendProgramChange(57, 3); // トランペット
  MIDI.sendProgramChange(74, 4); // フルート
}

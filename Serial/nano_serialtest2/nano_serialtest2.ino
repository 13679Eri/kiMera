//Serialtest_2
// 複数Arduino間のシリアル通信(子機Arduino用スケッチ)
// https://burariweb.info/electronic-work/multiple-arduino-connections-serial-communication.html

#include <Arduino.h>
// 四つのボタンを作る
// それぞれボタンを押したらMIDIにつながってるM5に(音色(チャンネル),音程(ド),on/off,音量(127))を送る
// 送信用変数

int ch = 0;
int note = 60;
bool ring = 0;
int velo = 127;

// ボタン判定用
int number = 4;
int flag[4]; // フラグ

void setup() {
  // button
  pinMode(2, INPUT_PULLUP); // ピアノ
  pinMode(3, INPUT_PULLUP); // ヴァイオリン
  pinMode(4, INPUT_PULLUP); // トランペット
  pinMode(5, INPUT_PULLUP); // フルート
  // led
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  Serial1.begin(115200); // シリアル通信の開始(親機・子機Arduinoでボーレードを合わせる)
  Serial.begin(115200);
}
void loop() {
  //ボタンに対応したLEDが光り、M5にシリアルを送る
  for (int i = 0; i < number; i++) {
    if (digitalRead(i + 2) == LOW) {
      if (flag[i] == 0) {
        flag[i] = 1;
        digitalWrite(i + 6, HIGH);
        ch = i + 1;
        ring = flag[i];
        data_send();
      }
    } else {
      if (flag[i] == 1) {
        flag[i] = 0;
        digitalWrite(i + 6, LOW);
        ch = i + 1;
        ring = flag[i];
        data_send();
      }
    }
  }

  //もしモードが変わったらLEDチカチカさせる
  if (Serial1.available()) {
    byte Signal = (byte)Serial1.read();
    for (int i = 0; i < 4; i++) {
      if (Signal == i + 1) {
        digitalWrite(i + 6, HIGH);
        delay(100);
        digitalWrite(i + 6, LOW);
        delay(50);
        digitalWrite(i + 6, HIGH);
        delay(100);
        digitalWrite(i + 6, LOW);
      }
    }
  }
}

void data_send() {
  Serial1.print(ch);
  Serial1.print(",");
  Serial1.print(note);
  Serial1.print(",");
  Serial1.print(ring);
  Serial1.print(",");
  Serial1.print(velo);
  Serial1.println("");

  Serial.print(ch);
  Serial.print(",");
  Serial.print(note);
  Serial.print(",");
  Serial.print(ring);
  Serial.print(",");
  Serial.print(velo);
  Serial.println("");
}

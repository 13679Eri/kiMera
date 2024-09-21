#include <M5Core2.h>
#include "M5UnitSynth.h"

M5UnitSynth synth;
//音色
int note_color[4] = {1 , 41, 57, 74};
int nc = 0;

// 受信データ格納
int ch = 0;
int note = 0;
bool ring = 0;
int velo = 0;

//送信データ
int mode = 0;

void setup() {
  M5.begin();
  Serial1.begin(115200, SERIAL_8N1, 13, 14); //nanoとやりとりするシリアル
  Serial.begin(115200);  //m5内のシリアル

  //MIDI
  Serial2.begin(115200, SERIAL_8N1, 3, 1);
  synth.begin(&Serial2, UNIT_SYNTH_BAUD, 33, 32);
  synth.setInstrument(0, 0, 57);  // synth piano 1

  //画面
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(1);
  M5.Lcd.printf("ch = %d note = %d ring = %d", ch, note, ring);
}

void loop() {
  M5.update();
  serial_receve();
  serial_send();
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
  }
  return (index + 1);
}

void serial_receve() {
  if (Serial1.available()) {      // 受信データを確認する
    String cmds[4] = {"\n"};
    String myString = Serial1.readStringUntil('\n');  //改行されるところまで読む

    // 分割数 = 分割処理(文字列, 区切り文字, 配列)
    int index = split(myString, ',', cmds);

    ch = cmds[0].toInt();
    note = cmds[1].toInt();
    ring = cmds[2].toInt();
    velo = cmds[3].toInt();

    M5.Lcd.setCursor(40, 100);
    M5.Lcd.printf("ch = %d note = %d ring = %d　velo = %d", ch, note, ring, velo);

    //音色
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

    if(note == -1){
      for (int i = 0; i < 4; i++) {
        synth.setAllNotesOff(i);
      }
    }
  }
}

void serial_send() {
  if (M5.BtnA.wasPressed()) {
    mode++;
    if (mode == 5) mode = 1;
    Serial1.write(mode);
  }

  M5.Lcd.setCursor(10, 20);
  M5.Lcd.println(mode);
}

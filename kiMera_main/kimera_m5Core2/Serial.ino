int split(String data, char delimiter, String * dst) {
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
    String cmds[4] = {"\n"}; //グローバル変数にすると桁が増えていっちゃう
    String myString = Serial1.readStringUntil('\n');  //改行されるところまで読む

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
      }
    }

    if (ch == 3) {
      patern = note; //0~5
    } else {
      //鳴らす
      if (ring == 1) {
        synth.setNoteOn(ch, note, velo);
      } else {
        synth.setNoteOff(ch, note, velo);
      }
    }

    if (note == -1) {
      for (int i = 0; i < 4; i++) {
        synth.setAllNotesOff(i);
      }
    }

    //    M5.Lcd.setCursor(35, 20);
    //    M5.Lcd.printf("ch=%d,note=%d,ring=%d,velo=%d", ch, note, ring, velo);
  }
}

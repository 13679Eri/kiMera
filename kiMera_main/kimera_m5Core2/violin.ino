//シリアルで加速度受け取るのやめて遅延が改善されるか
//超音波読み取るタイミングを戻して遅延が改善されるか
//Pa.hubにして遅延が改善されるか
//手前の弦の方も情報を増やす

void set_violin() {
  set_sensor();
  ch = 2;
  synth.setInstrument(0, 2, note_color[1]);
  M5.Lcd.fillRect(30, 14, 150, 200, 0xff0d);
  M5.Lcd.fillCircle(110, 90, 30, 0xcb24);
  M5.Lcd.fillCircle(110, 135, 30, 0xcb24);
  M5.Lcd.fillRect(105, 40, 10, 70, BLACK);
  M5.Lcd.drawLine(95, 100, 85, 130, BLACK);
  M5.Lcd.drawLine(120, 100, 130, 130, BLACK);
  M5.Lcd.fillTriangle(105, 135, 115, 135, 110, 165, BLACK);
}

void violin() {
  M5.update();
  ch = 2;
  synth.setInstrument(0, 2, note_color[1]);

  //IMU
  tca.selectChannel(IMU_CHANNEL);
  imu6886.getGyroData(&gyroX, &gyroY, &gyroZ);
  imu6886.getAccelData(&accX, &accY, &accZ);

  //角度
  angle = atan2(accX, accZ) * 180.0 / PI;

  //距離
  tca.selectChannel(ULTRASONIC_CHANNEL);
  previousDistan = distan;
  distan = sensor.getDistance() / 10;

  //joystick
  tca.selectChannel(JOY_CHANNEL);
  Wire.requestFrom(JOY_ADDR, 3);  //Request 3 bytes from the slave device.  向从设备请求3个字节

  if (Wire.available()) {  //If data is received.  如果接收到数据
    x_data = Wire.read();
    y_data = Wire.read();
    button_data = Wire.read();
  }

  if (120 <= x_data && x_data <= 130) {
    note = -1;
    //止める
    synth.setAllNotesOff(2);
    synth.setInstrument(0, 2, note_color[1]);
    prev_note = note;
  } else if (130 < x_data) {
    Serial1.write(mode);
    //本体の角度
    if (Serial1.available()) {
      String receivedData = Serial1.readString();  // シリアルから文字列としてデータを受け取る
      mainbody_ag = receivedData.toFloat();
    }
    angle += mainbody_ag;
    
    note = violin_Pitch(angle, distan);
    if (prev_note != note) {
      synth.setNoteOff(ch, prev_note, 0);
      synth.setNoteOn(ch, note, 127);
      prev_note = note;
    }
  } else if (x_data < 120) {
    Serial1.write(mode);
    //本体の角度
    if (Serial1.available()) {
      String receivedData = Serial1.readString();  // シリアルから文字列としてデータを受け取る
      mainbody_ag = receivedData.toFloat();
    }
    angle += mainbody_ag;

    note = violin_Pitch(angle, distan);
    if (prev_note != note) {
      synth.setNoteOff(ch, prev_note, 0);
      synth.setNoteOn(ch, note, 127);
      prev_note = note;
    }
  }

  //  // 音の開始・停止の条件
  //  if (120 <= x_data && x_data <= 130) {
  //    note = -1;
  //    synth.setAllNotesOff(2); // 止める
  //    synth.setInstrument(0,ch, note_color[1]);
  //    prev_note = note;
  //  } else {
  //    // 本体の角度を更新
  //    Serial1.write(mode);
  //    if (Serial1.available()) {
  //      String receivedData = Serial1.readString();
  //      mainbody_ag = receivedData.toFloat();
  //    }
  //    angle += mainbody_ag;
  //
  //    // 音程の更新条件
  //    if (abs(distan - previousDistan) > 0.5 || x_data != prev_x_data) {  // 超音波の変化またはジョイスティックの変化をチェック
  //      note = violin_Pitch(angle, distan);
  //      if (prev_note != note) {
  //        synth.setNoteOff(ch, prev_note, 0);
  //        synth.setNoteOn(ch, note, 127);
  //        prev_note = note;
  //      }
  //    }
  //  }
  //
  //  prev_x_data = x_data;  // ジョイスティックの前回の値を保存
  //描画
  //  M5.Lcd.setCursor(35, 145);
  //  M5.Lcd.printf("ch=%d", ch);
  //  M5.Lcd.setCursor(35, 160);
  //  M5.Lcd.printf("dis=%f", distan);
  //  M5.Lcd.setCursor(35, 175);
  //  M5.Lcd.printf("body_ag=%f", mainbody_ag);
  //  M5.Lcd.setCursor(35, 190);
  //  M5.Lcd.printf("joy=%d", x_data);
  //  M5.Lcd.setCursor(35, 215);
  //  M5.Lcd.printf("note=%d", note);
}

//指の位置測る
int violin_Pitch(float angle, float newvalue) {
  if (30 <= angle && angle <= 50) { // G弦
    chord = false;
    int notes[] = {55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72};
    return determineNote(notes, newvalue);
  }

  if (10 <= angle && angle <= 30) { // D弦
    chord = false;
    int notes[] = {62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79};
    return determineNote(notes, newvalue);
  }

  if (-30 <= angle && angle <= -10) { // A弦
    chord = false;
    int notes[] = {69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86};
    return determineNote(notes, newvalue);
  }

  if (-50 <= angle && angle <= -30) { // E弦
    chord = false;
    int notes[] = {76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93};
    return determineNote(notes, newvalue);
  }

  return -2; // エラーハンドリング
}


int determineNote(int notes[], float newvalue) {
  for (int i = 0; i < 18; i++) { // 28から10までの範囲で値を取得するので18回ループ
    float upper_bound = 28 - i;
    float lower_bound = upper_bound - 1;

    if (newvalue >= lower_bound && newvalue < upper_bound) {
      return notes[i];
    }
  }
  return -1; // 該当する範囲がなかった場合、エラー値として-1を返す
}



//  for (int i = 0; i < size; i++) {
//    if (i == 0) {
//      if (dis[i] <= newvalue) {
//        return notes[i];
//      }
//    } else {
//      if (dis[i] <= newvalue && newvalue <= dis[i - 1]) {
//        return notes[i];
//      }
//    }
//  }
//  return notes[size - 1]; // デフォルトで最後の音符を返す

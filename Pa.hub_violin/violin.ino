//シリアルで加速度受け取るのやめて遅延が改善されるか
//超音波読み取るタイミングを戻して遅延が改善されるか
//Pa.hubにして遅延が改善されるか
//手前の弦の方も情報を増やす

void violin() {
  M5.update();
  M5.Lcd.fillRect(30, 14, 150, 200, 0xff0d);
  M5.Lcd.fillCircle(110, 90, 30, 0xcb24);

  //IMU
  tca.selectChannel(IMU_CHANNEL);
  imu6886.getGyroData(&gyroX, &gyroY, &gyroZ);
  imu6886.getAccelData(&accX, &accY, &accZ);

  //角度
  angle = atan2(accX, accZ) * 180.0 / PI;

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
    tca.selectChannel(ULTRASONIC_CHANNEL);
    distan = sensor.getDistance() / 10;

    note = violin_Pitch(angle, distan);
    if (prev_note != note) {
      //      flag = 1;
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
    tca.selectChannel(ULTRASONIC_CHANNEL);
    distan = sensor.getDistance() / 10;
    //    delay(10);:

    note = violin_Pitch(angle, distan);
    if (prev_note != note) {
      synth.setNoteOff(ch, prev_note, 0);
      synth.setNoteOn(ch, note, 127);
      prev_note = note;
    }
  }

  //描画
  M5.Lcd.setCursor(35, 145);
  M5.Lcd.printf("dis=%f", distan);
  M5.Lcd.setCursor(35, 175);
  M5.Lcd.printf("body_ag=%f", mainbody_ag);
  M5.Lcd.setCursor(35, 190);
  M5.Lcd.printf("joy=%d", x_data);
  M5.Lcd.setCursor(35, 215);
  M5.Lcd.printf("note=%d", note);
}

//指の位置測る
int violin_Pitch(float angle, float newvalue) {
  double kaihou = 28.5
  double dis[] = {28.5, 26.5, 25.5, 24.5, 23.5, 22.5, 20.5, 19.5, 18.5};
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

  return 60; // エラーハンドリング
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

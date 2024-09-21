void rcv_serial() {
  if (Serial1.available()) {      // 受信データを確認する
    mode = (byte)Serial1.read();  // 受信データを読み取る
    Serial.println(mode);
    if (mode == 4) {
      baselinePressure = lps35hw.readPressure();
      Serial.println(baselinePressure);
    }
  }
}

void data_send(int ch, int note, bool ring, int velo) {
  Serial1.print(ch);
  Serial1.print(",");
  Serial1.print(note);
  Serial1.print(",");
  Serial1.print(ring);
  Serial1.print(",");
  Serial1.print(velo);
  Serial1.println();

  Serial.print(ch);
  Serial.print(",");
  Serial.print(note);
  Serial.print(",");
  Serial.print(ring);
  Serial.print(",");
  Serial.print(velo);
  Serial.println();
}

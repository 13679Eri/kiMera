void set_violin() {
  //IMU
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
}

void violin() {
  //  ch = 2;
  float x, y, z;

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);

    //角度
    angle = atan2(x, -z) * 180.0 / PI;
    //    Serial.println(angle);
  }

  //m5からringが来たら加速度送る
  if (Serial1.available()) {  // 受信データを確認する
    if (ring == 1) {
      Serial1.print(angle);
      Serial.println(angle);
    } else {
      mode = ring;
    }
  }
}

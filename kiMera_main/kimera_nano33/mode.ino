//楽器の向き(加速度でとる)によってmodeを変更したい
//加速度の初期化はset_violin()にはいってる
//ヴァイオリンモードになった後1音ずつ(1シリアルずつ)送信が遅れてしまう
//ヴァイオリンモードになった後M5のモードがボタンじゃないと切り替わらない

void set_mode() {
  //  nano_mode();
  rcv_serial();
  // モードが変更された場合のみセット関数を呼び出す
  if (mode != previousMode) {
    Serial.println(mode);
//    data_send(5, mode, 0, 0);
    previousMode = mode; // 前回のモードを更新
  }

  switch (mode) {
    case 1:
      piano();
      break;
    case 2:
      violin();
      break;
    case 3:
      trumpet();
      break;
    case 4:
      flute();
      break;
  }
}

void nano_mode() {
  float x, y, z;
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);

    if (abs(x) > abs(y) && abs(x) > abs(z)) {
      if (x > 0) {
        mode = 4;
      } else {
        mode = 3;
      }
    } else {
      if (z > 0) {
        mode = 1;
      } else {
        mode = 2;
      }
    }
  }
}

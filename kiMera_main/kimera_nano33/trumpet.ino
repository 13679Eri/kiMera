void trumpet() {
  //タッチ
  u16 result = 0;
  u16 filtered_data_buf[CHANNEL_NUM] = { 0 };
  u8 baseline_buf[CHANNEL_NUM] = { 0 };

  result = mpr121.check_status_register();

  mpr121.get_filtered_reg_data(&result, filtered_data_buf);

  for (int i = 0; i < CHANNEL_NUM; i++) {
    if (result & (1 << i)) { /*key i is pressed!!*/
      if (0 == touch_status_flag[i]) {
        touch_status_flag[i] = 1;
      }
    } else {
      if (1 == touch_status_flag[i]) {
        touch_status_flag[i] = 0;
      }
    }
  }

  pt = trunpet_patern(touch_status_flag[1], touch_status_flag[4], touch_status_flag[5]);

  if (Serial1.available()) {      // 受信データを確認する
    mode = (byte)Serial1.read();
    Serial.println(mode);
    if (mode == 3) {
      data_send(ch, pt, 1, 127);
    }
  }
}

int trunpet_patern(bool flag3, bool flag4, bool flag5) {
  ch = 3;
  int value = 0;

  if (flag3) value |= 1 << 0;  // flag3 が true なら 0ビット目に1をセット
  if (flag5) value |= 1 << 1;  // flag5 が true なら 2ビット目に1をセット
  if (flag4) value |= 1 << 2;  // flag4 が true なら 1ビット目に1をセット

  // value の値に応じて数字を返す
  switch (value) {
    case 0b000: return 0;  // 押さえてない
    case 0b001: return 1;  // flag3 だけが true
    case 0b010: return 2;  // flag5 だけが true
    case 0b100: return 3;  // flag4 だけが true
    case 0b011: return 4;  // flag3 と flag5 が true
    case 0b110: return 5;  // flag5 と flag4 が true
    case 0b101: return 6;  // flag3 と flag4 が true
    case 0b111: return 7;  // flag3, flag5, flag4 が全て true
    default: return -1;    // 上記以外の場合
  }
}

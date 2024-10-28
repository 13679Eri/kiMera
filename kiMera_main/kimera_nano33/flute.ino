void set_sensor() {
  //タッチセンサ
  s32 ret = 0;
  if (mpr121.begin() < 0) {
    Serial.println("Can't detect device!!!!");
  } else {
    Serial.println("mpr121 init OK!");
  }
  mpr121.set_sensitivity(0x60);

  //気圧
  if (!lps35hw.begin_I2C()) {
    Serial.println("Couldn't find LPS35HW chip");
    while (1);
  }
  Serial.println("Found LPS35HW chip");
  lps35hw.zeroPressure();
}

void flute() {
  //気圧
  unsigned long currentMillis = millis();  //今の時間
  float pressure = lps35hw.readPressure();

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

  //吹く
  if (pressure > 0.10) {
    note = flute_note();
    if (prev_note != note) {
      ring = 1;
      velo = PreChangeToVolume(pressure);
      if (velo > 127) {
        velo = 127;
      }
      data_send(ch, prev_note, 0, 0);
      data_send(ch, note, ring, velo);
      prev_note = note;
    }
  } else {
    if (prev_note != -1) {
      note = -1;
      ring = 0;
      velo = 0;
      data_send(ch, note, ring, velo);
      prev_note = note;
    }
  }
  delay(100);
}

int PreChangeToVolume(float preChange) {
  // 圧力変化を音量にマップ（適宜調整）
  return map(preChange, -1, 1, 0, 127);
}

int flute_note() {
  ch = 4;
  //   指遣いをビットマスクで表現（1: 押されている, 0: 離されている）
  int fingerPattern = 0;

  for (int i = 0; i < CHANNEL_NUM; i++) {
    if (touch_status_flag[i] == 1) {
      fingerPattern |= (1 << i);
    }
  }

  // 指遣いに対応するノート番号を設定
  switch (fingerPattern) {
    case 0b111100101101:
      return 60;
      break;
    case 0b111100100101:
      return 62;
      break;
    case 0b111100100001:
      return 64;
      break;
    case 0b111100100000:
      return 65;
      break;
    case 0b111000100000:
      return 67;
      break;
    case 0b110000100000:
      return 69;
      break;
    case 0b100000100000:
      return 71;
      break;

    //次のオクターブ

    case 0b010000100000:
    case 0b111100011101:
      return 72;
      break;
    case 0b000000100000:
    case 0b111100010101:
      return 74;
      break;
    case 0b111100010001:
      return 76;
      break;
    case 0b111100010000:
      return 77;
      break;
    case 0b111000010000:
      return 79;
      break;
    case 0b110000010000:
      return 81;
      break;
    case 0b100000010000:
      return 83;
      break;

    //次のオクターブ

    case 0b010000010000:
    case 0b111100001111:
      return 84;
      break;
    case 0b000000010000:
    case 0b111100000111:
      return 86;
      break;
    case 0b111100000011:
      return 88;
      break;
    case 0b111100000010:
      return 89;
      break;
    case 0b111000000010:
      return 91;
      break;
    case 0b110000000010:
      return 93;
      break;
    case 0b100000000010:
      return 95;
      break;

    //次のオクターブ

    case 0b010000000010:
    case 0b111100001101:
      return 96;
      break;
    case 0b000000000010:
    case 0b111100000101:
      return 98;
      break;
    case 0b111100000001:
      return 100;
      break;
    case 0b111100000000:
      return 101;
      break;
    case 0b111000000000:
      return 103;
      break;
    case 0b110000000000:
      return 105;
      break;
    case 0b100000000000:
      return 107;
      break;

    case 0b010000000000:
      return 108;
      break;
    case 0b000000000000:
      return 110;
      break;
    // 他の指遣いパターンを追加
    default:
      // 必要に応じてデフォルトの処理を追加
      break;
  }
}

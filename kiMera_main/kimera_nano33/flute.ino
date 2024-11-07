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
  if (pressure > 0.50) {
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
  return map(preChange, 0.5, 1.5, 63, 127);
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
    case 0b111101100111:
      return 59;//B
      break;
    case 0b111100100111:
      return 60;//C
      break;
    case 0b111110100111:
      return 61;//C#
      break;
    case 0b111101100011:
      return 61;//C#
      break;
    case 0b111100100011:
      return 62;//D
      break;
    case 0b111110100011:
      return 63;//D#
      break;
    case 0b111101100001:
      return 63;//D#
      break;
    case 0b111100100001:
      return 64;//E
      break;
    case 0b111101100000:
      return 64;//E
      break;
    case 0b111110100001:
      return 65;//F
      break;
    case 0b111100100000:
      return 65;//F
      break;
    case 0b111110100000:
      return 66;//F#
      break;
    case 0b111001100000:
      return 66;//F#
      break;
    case 0b111000100000:
      return 67;//G
      break;
    case 0b111010100000:
      return 68;//G#
      break;
    case 0b110001100000:
      return 68;//G#
      break;
    case 0b110000100000:
      return 69;//A
      break;
    case 0b110010100000:
      return 70;//A#
      break;
    case 0b100001100000:
      return 70;//A#
      break;
    case 0b100000100000:
      return 71;//B
      break;
    case 0b010001100000:
      return 71;//B
      break;
    case 0b100010100000:
      return 72;//C
      break;
    case 0b010000100000:
      return 72;//C
      break;
    case 0b010010100000:
      return 73;//C#
      break;
    case 0b000001100000:
      return 73;//C#
      break;
    case 0b000000100000:
      return 74;//D
      break;
    case 0b000010100000:
      return 75;//D#
      break;

    //次のオクターブ
    case 0b111101010111:
      return 71;//B
      break;
    case 0b111100010111:
      return 72;//C
      break;
    case 0b111110010111:
      return 73;//C#
      break;
    case 0b111101010011:
      return 73;//C#
      break;
    case 0b111100010011:
      return 74;//D
      break;
    case 0b111110010011:
      return 75;//D#
      break;
    case 0b111101010001:
      return 75;//D#
      break;
    case 0b111100010001:
      return 76;//E
      break;
    case 0b111101010000:
      return 76;//E
      break;
    case 0b111110010001:
      return 77;//F
      break;
    case 0b111100010000:
      return 77;//F
      break;
    case 0b111110010000:
      return 78;//F#
      break;
    case 0b111001010000:
      return 78;//F#
      break;
    case 0b111000010000:
      return 79;//G
      break;
    case 0b111010010000:
      return 80;//G#
      break;
    case 0b110001010000:
      return 80;//G#
      break;
    case 0b110000010000:
      return 81;//A
      break;
    case 0b110010010000:
      return 82;//A#
      break;
    case 0b100001010000:
      return 82;//A#
      break;
    case 0b100000010000:
      return 83;//B
      break;
    case 0b010001010000:
      return 83;//B
      break;
    case 0b100010010000:
      return 84;//C
      break;
    case 0b010000010000:
      return 84;//C
      break;
    case 0b010010010000:
      return 85;//C#
      break;
    case 0b000001010000:
      return 85;//C#
      break;
    case 0b000000010000:
      return 86;//D
      break;
    case 0b000010010000:
      return 87;//D#
      break;

    //次のオクターブ
    case 0b111001001111:
      return 83;//B
      break;
    case 0b111100001111:
      return 84;//C
      break;
    case 0b111110001111:
      return 85;//C#
      break;
    case 0b111101001011:
      return 85;//C#
      break;
    case 0b111100001011:
      return 86;//D
      break;
    case 0b111110001011:
      return 87;//D#
      break;
    case 0b111101001001:
      return 87;//D#
      break;
    case 0b111100001001:
      return 88;//E
      break;
    case 0b111101001000:
      return 88;//E
      break;
    case 0b111110001001:
      return 89;//F
      break;
    case 0b111100001000:
      return 89;//F
      break;
    case 0b111110001000:
      return 90;//F#
      break;
    case 0b111001001000:
      return 90;//F#
      break;
    case 0b111000001000:
      return 91;//G
      break;
    case 0b111010001000:
      return 92;//G#
      break;
    case 0b110001001000:
      return 92;//G#
      break;
    case 0b110000001000:
      return 93;//A
      break;
    case 0b110010001000:
      return 94;//A#
      break;
    case 0b100001001000:
      return 94;//A#
      break;
    case 0b100000001000:
      return 95;//B
      break;
    case 0b010001001000:
      return 95;//B
      break;
    case 0b100010001000:
      return 96;//C
      break;
    case 0b010000001000:
      return 96;//C
      break;
    case 0b010010001000:
      return 97;//C#
      break;
    case 0b000001001000:
      return 97;//C#
      break;
    case 0b000000001000:
      return 98;//D
      break;
    case 0b000010001000:
      return 99;//D#
      break;

    case 0b000000000000:
      return -2;//エラーハンドリング
      break;
  }
}

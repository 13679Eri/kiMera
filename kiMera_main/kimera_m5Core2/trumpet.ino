//エッジインパルスを使えるかどうか
//防音シートを増やす

void set_trumpet() {
  set_sensor();
  ch = 3;
  synth.setInstrument(0, ch, note_color[2]);

  M5.Lcd.fillRect(30, 14, 150, 200, 0xff0d);
  M5.Lcd.fillTriangle(60, 75, 90, 90, 60, 105, YELLOW);
  M5.Lcd.fillRect(85, 88, 80, 4, YELLOW);
  M5.Lcd.fillRect(110, 88, 4, 30, YELLOW);
  M5.Lcd.fillRect(130, 88, 4, 30, YELLOW);
  M5.Lcd.fillRect(150, 88, 4, 30, YELLOW);
  M5.Lcd.drawLine(88, 90, 90, 118, YELLOW);
  M5.Lcd.drawLine(90, 118, 154, 118, YELLOW);
  M5.Lcd.fillTriangle(173, 83, 163, 88, 173, 93, YELLOW);
}

void trumpet() {
  M5.update();

  //  M5.Lcd.setCursor(35, 215);
  //  M5.Lcd.printf("mode=%d", mode);

  uint8_t* microRawData = (uint8_t*)calloc(2048, sizeof(uint8_t));
  size_t bytesread;
  i2s_read(I2S_NUM_0, (char*)microRawData, 2048, &bytesread, portMAX_DELAY);

  fft_config_t* real_fft_plan = fft_init(1024, FFT_REAL, FFT_FORWARD, NULL, NULL);
  int16_t* buffptr = (int16_t*)microRawData;

  for (int i = 0; i < real_fft_plan->size; i++) {
    float adc_data = (float)map(buffptr[i], INT16_MIN, INT16_MAX, -2000, 2000);
    real_fft_plan->input[i] = adc_data;
  }

  //周波数
  fft_execute(real_fft_plan);
  float pitch = calculatePitch(real_fft_plan->output, real_fft_plan->size, 44100);

  // 音量
  float volume = calculateVolume(buffptr, 1024);
  //  int trVelo = toVelocity(volume);

  fft_destroy(real_fft_plan);
  free(microRawData);

  if (volume >= 100 && 270 <= pitch && pitch <= 1100) {
    synth.setInstrument(0, 3, 57);
    ring = 1;
    Serial1.write(ring);
    serial_receve(); //paternが送られてくる
    note = frequencyToMidiNoteNumber(pitch); //基準音決める
    note = convertNote(patern); //基準音とpaternから鳴る音を決める
    if (prev_note != note) {
      synth.setNoteOff(3, prev_note, 0); //前の音消す
      synth.setNoteOn(3, note, 127); //鳴らす
      prev_note = note;
    }
  } else {
    if (prev_note != -1) {
      ring = 0;
      note = -1;
      synth.setNoteOn(3, note, 0);
      prev_note = note;
    }
  }

  M5.Lcd.setCursor(35, 120);
  M5.Lcd.printf("ch=%d", ch);
  //  M5.Lcd.setCursor(35, 135);
  //  M5.Lcd.printf("volume=%d", volume);
  M5.Lcd.setCursor(35, 150);
  M5.Lcd.printf("patern=%d", patern);
  M5.Lcd.setCursor(35, 175);
  M5.Lcd.printf("note=%d", note);
}


// PDM初期化
bool InitI2SMic() {
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM),
    .sample_rate = 44100,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 2,
    .dma_buf_len = 128,
  };

  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_PIN_NO_CHANGE,
    .ws_io_num = PIN_CLK,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = PIN_DATA
  };
  i2s_set_pin(I2S_NUM_0, &pin_config);
  i2s_set_clk(I2S_NUM_0, 44100, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);

  return true;
}

//fft
float calculatePitch(float * fftOutput, int fftSize, float sampleRate) {
  float maxVal = 0;
  int maxIndex = 0;

  for (int i = 1; i < fftSize / 2; i++) {
    double magnitude = sqrt(fftOutput[2 * i] * fftOutput[2 * i] + fftOutput[2 * i + 1] * fftOutput[2 * i + 1]);
    if (magnitude > maxVal) {
      maxVal = magnitude;
      maxIndex = i;
    }
  }

  float peakFrequency = (float)maxIndex * sampleRate / fftSize;
  return peakFrequency;
}


// 周波数をMIDIノート番号に変換
int frequencyToMidiNoteNumber(float frequency) {
  if (frequency <= 0) return 0;
  if (frequency <= 277.2) {
    return 58;
  } else if (277.2 < frequency && frequency <= 392) {
    return 65;
  } else if (392 < frequency && frequency <= 554.4) {
    return 70;
  } else if (554.4 < frequency && frequency <= 784) {
    return 77;
  } else if (784 < frequency && frequency <= 1108.7) {
    return 82;
  } else if (1108.7 < frequency) {
    return 89;
  }
}

// 音量を計算する関数
float calculateVolume(int16_t* buffer, int bufferSize) {
  int32_t sum = 0;
  for (int i = 0; i < bufferSize; i++) {
    sum += abs(buffer[i]);
  }
  return (float)sum / bufferSize;
}

int toVelocity(float vl) {
  // 圧力変化を音量にマップ（適宜調整）
  return map(vl, 100, 500, 0, 127);
}

int convertNote(int patern) {
  switch (patern) {
    case 0:  return note; // 押さえてない
    case 1:  return note - 2; // flag3 だけが true
    case 2:  return note - 1; // flag5 だけが true
    case 3:  return note - 3; // flag4 だけが true
    case 4:  return note - 3; // flag3 と flag5 が true
    case 5:  return note - 4; // flag5 と flag4 が true
    case 6:  return note - 5; // flag3 と flag4 が true
    case 7:  return note - 6; // flag3, flag5, flag4 が全て true
    default: return -1;    // 上記以外の場合
  }
}

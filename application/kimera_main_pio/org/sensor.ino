void set_sensor() {
  //MIDI
  synth.begin(&Serial2, UNIT_SYNTH_BAUD, 33, 32);
  //いきなり全部チャンネル設定するとダメみたい
  //送られたchによって変えることにした
  for (int i = 0; i < 4; i++) {
    if (ch == i + 1) {
      synth.setInstrument(0, i + 1, note_color[i]);
    }
  }

  //センサ Wire: SDA SCL
  Wire.begin(21, 22); //pa.hub

  // Pa.Hubの初期化
  tca.address(0x70); // Pa.Hubのデフォルトアドレス
  // ジョイスティックの初期化
  tca.selectChannel(JOY_CHANNEL);
  Wire.beginTransmission(0x52);
  Wire.endTransmission();

  // 超音波センサの初期化
  tca.selectChannel(ULTRASONIC_CHANNEL);
  sensor.begin(&Wire, 0x57);

  // 加速度センサの初期化
  tca.selectChannel(IMU_CHANNEL);
  imu6886.Init(21, 22);

  //PDM
  InitI2SMic();
}

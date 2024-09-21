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
  Wire.begin(21, 22); //joy
  sensor.begin(&Wire, 0x57, 21, 22, 400000L); //超音波
  imu6886.Init(21, 22); //加速度
  InitI2SMic(); //PDM
}

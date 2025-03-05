/*
   ble.cpp ble.send
   void Controller::send(const String& message) {
  uint8_t data[message.length() + 1];
  message.getBytes(data, message.length() + 1);
  characteristic_->setValue(data, message.length() + 1);
  characteristic_->notify();
  }
  結局引数Stringにすればいいってことだよね
*/

//M5.Log.~~はUniffiedのやつだよね？？
void set_ble() {
  ble.setup_after_begin();
}

void ble_send(int mode) {  //mode(ch) midinumber(note) velo notecolor 適宜増やしていく
  //受け取ったものがなんなの(modeの変更の変数なのかMIDIのnotenumberなのか)かはアプリで精査する？？んだよね？
  //(Dart内で)受け取った数字が1~4のどれかだったらmodeを変える(楽器の画面が変わる)みたいな感じだよね
  /*
    DynamicJsonDocument 変数名(確保する大きさ);
    StaticJsonDocumet<確保する大きさ>　変数名; 
  */
  if (!ble.isDeviceConnected()) {
    return;
  }

  StaticJsonDocument<64> doc;
  doc["mode"] = mode;

  String send_info_json;
  serializeJson(doc, send_info_json);  // JSON を String に変換

  ble.send(send_info_json);
}

void isBLEConnected(){
  if (!ble.isDeviceConnected()) {
    return;
  }
}
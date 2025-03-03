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
void ble_setup() {
  ble.setup_after_begin();
}

void ble_send(int n) {
  //受け取ったものがなんなの(modeの変更の変数なのかMIDIのnotenumberなのか)かはアプリで精査する？？んだよね？
  //(Dart内で)受け取った数字が1~4のどれかだったらmodeを変える(楽器の画面が変わる)みたいな感じだよね
  ble.send(toString(n));
}

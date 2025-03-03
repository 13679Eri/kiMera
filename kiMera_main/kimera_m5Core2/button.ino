void set_button() {
  //ボタンイベント
  btn_p.addHandler(event_btn_p, E_TOUCH + E_RELEASE); // ボタンのイベント関数を指定
  btn_v.addHandler(event_btn_v, E_TOUCH + E_RELEASE);
  btn_t.addHandler(event_btn_t, E_TOUCH + E_RELEASE);
  btn_f.addHandler(event_btn_f, E_TOUCH + E_RELEASE);

  //画面 ボタン
  M5.Lcd.setTextSize(1);  //Set the font size.
  M5.Lcd.setTextColor(BLACK);
  M5.Lcd.fillScreen(0xffb9dc);
  M5.Lcd.fillRect(30, 14, 150, 200, 0xff0d);
  M5.Buttons.draw(); // 全てのボタンを描画
}

void event_btn_p(Event & e) {
  if (e == E_TOUCH) {
    mode = 1;
    ble_send(mode);
  }
}

void event_btn_v(Event & e) {
  if (e == E_TOUCH) {
    mode = 2;
    ble_send(mode);
  }
}

void event_btn_t(Event & e) {
  if (e == E_TOUCH) {
    mode = 3;
    ble_send(mode);
  }
}

void event_btn_f(Event & e) {
  if (e == E_TOUCH) {
    mode = 4;
    ble_send(mode);
  }
}

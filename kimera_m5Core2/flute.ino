void set_flute() {
  M5.Lcd.fillRect(30, 14, 150, 200, 0xff0d);
  M5.Lcd.fillRect(43, 98, 100, 15,  0xd69a);
  M5.Lcd.fillCircle(63, 108, 5, 0x9492);
  M5.Lcd.fillCircle(83, 108, 5, 0x9492);
  M5.Lcd.fillCircle(102, 108, 5, 0x9492);
  M5.Lcd.fillCircle(132, 108, 5, 0x9492);
}

void flute() {
  M5.update();
  ch = 4;
  //  M5.Lcd.setCursor(35, 215);
  //  M5.Lcd.printf("mode=%d", mode);
  serial_receve();
}

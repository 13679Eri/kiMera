void set_piano() {
  //描画
  M5.Lcd.fillRect(30, 14, 150, 200, 0xff0d);
  M5.Lcd.fillRect(60, 120, 90, 30, BLACK);  //Draw a 50x50 blue rectangle wireframe at (x,y).
  M5.Lcd.fillRect(84, 90, 66, 30, BLACK);
  M5.Lcd.fillCircle(117, 90, 32, BLACK);
  M5.Lcd.fillRect(70, 135, 70, 15, WHITE);
  M5.Lcd.drawLine(80, 140, 80, 150, BLACK);
  M5.Lcd.drawLine(90, 140, 90, 150, BLACK);
  M5.Lcd.drawLine(110, 135, 110, 150, BLACK);
  M5.Lcd.drawLine(120, 140, 120, 150, BLACK);
  M5.Lcd.drawLine(130, 140, 130, 150, BLACK);
  M5.Lcd.fillRect(80, 135, 4, 8, BLACK);
  M5.Lcd.fillRect(90, 135, 4, 8, BLACK);
  M5.Lcd.fillRect(110, 135, 4, 8, BLACK);
  M5.Lcd.fillRect(120, 135, 4, 8, BLACK);
  M5.Lcd.fillRect(130, 135, 4, 8, BLACK);
}

void piano() {
  M5.update();
  ch = 1;
  serial_receve();
  //  M5.Lcd.setCursor(35, 215);
  //  M5.Lcd.printf("mode=%d", mode);
}

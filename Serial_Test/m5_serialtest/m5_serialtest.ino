//#include <M5Core2.h>
////pearent
//int note = 0;
//int Board_No = 1;  // 子機の番号の割り振り
//byte pre_signal = 0;
//int flag = 0;
//
//int mode = 0;
//
//void setup() {
//  M5.begin();
//  // 画面初期設定
//  M5.Lcd.setCursor(10, 10);
//  M5.Lcd.setTextSize(2);
//  M5.Lcd.println("START");
//  M5.Lcd.setCursor(10, 40);
//  M5.Lcd.println("off");
//  M5.Lcd.setCursor(10, 20);
//  M5.Lcd.println(note);
//  // シリアル通信機能の設定
//  // Serial1.begin(unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin, bool invert)
//  Serial.begin(115200, SERIAL_8N1, 3, 1);
//  // Serial1.begin(115200);
//}
//void loop() {
//  M5.update();
//  //  while (Serial.available()) {          // 受信データを確認する
//  byte Signal = (byte)Serial.read();  // 受信データを読み取る
//  if (Signal != pre_signal) {
//    flag = 1;
//  }
//  Serial.println(Signal);
//  Serial.println(pre_signal);
//
//  if (Signal == Board_No && flag == 1) {  // 受信データが自分の番号なのか確認
//    M5.Lcd.setCursor(10, 40);
//    M5.Lcd.println("on");
//    flag = 0;
//  } else if (Signal == 0 && flag == 1) {
//    M5.Lcd.setCursor(10, 40);
//    M5.Lcd.println("off");
//    flag = 0;
//  }
//  pre_signal = Signal;
//
//  // ボタンAが押された時の処理
//  //    if (M5.BtnA.wasReleased()) {
//  //      note = 1;
//  //      Serial.println(note);
//  //    } else {
//  //      note = 0;
//  //      Serial.println(note);
//  //    }
//
//  if (M5.BtnA.wasReleased()) {
//    mode++;
//    if (mode == 5) mode = 1;
//    Serial.println(mode);
//    M5.Lcd.setCursor(10, 200);
//    M5.Lcd.print(mode);
//  }
//  //    M5.Lcd.setCursor(10, 100);
//  //    M5.Lcd.print(note);
//  //     Serial.write(note);
//  //  }
//}


#include <M5Core2.h>
//pearent
int note = 10;
int pre_note = 0;
int flag = 0;

void setup() {

  M5.begin();

  // 画面初期設定
  M5.Lcd.setCursor(10, 10);
  M5.Lcd.setTextSize(2);
  M5.Lcd.println("START");
  M5.Lcd.setCursor(10, 40);
  M5.Lcd.println("off");
  M5.Lcd.setCursor(10, 20);
  M5.Lcd.println(note);

  // シリアル通信機能の設定
  // Serial1.begin(unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin, bool invert)
  Serial1.begin(115200, SERIAL_8N1, 3, 1); //nanoから受け取る
}

void loop() {
  M5.update();
  if (Serial1.available()) {          // 受信データを確認する
    byte Signal = (byte)Serial1.read();  // 受信データを読み取る
    if (Signal == 1 ) {  // 受信データが自分の番号なのか確認
      M5.Lcd.setCursor(10, 40);
      M5.Lcd.println("on   ");
    } else if (Signal == 0) {
      M5.Lcd.setCursor(10, 40);
      M5.Lcd.println("off");
    }
  }

  // ボタンAが押された時の処理
  if (M5.BtnA.wasPressed()){
    note = 60;
    Serial1.write(note);
  } else if(M5.BtnA.wasReleased()){
    note = 10;
    Serial1.write(note);
  }
  M5.Lcd.setCursor(10, 20);
  M5.Lcd.println(note);
}

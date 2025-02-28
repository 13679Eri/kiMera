#include <M5Unified.h>

#include "Ble.hpp"

const char* DEVICE_NAME = "kiMera";
const char* SERVICE_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";
const char* CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8";

void on_message_received(const String& message) { M5.Display.print(message); }

cps::ble::Controller ble{DEVICE_NAME, SERVICE_UUID, CHARACTERISTIC_UUID,
                         on_message_received};

void setup() {
  auto cfg = M5.config();  // M5Stack初期設定用の構造体を代入
  M5.begin(cfg);           // M5デバイスの初期化
  M5.setLogDisplayIndex(0);
  M5.Display.setTextWrap(true, true);
  M5.Log.setLogLevel(m5::log_target_display, ESP_LOG_DEBUG);
  M5.Log.setEnableColor(m5::log_target_serial, true);
  M5.Log.setEnableColor(m5::log_target_display, true);
  M5.Display.setTextSize(3);
  M5.Display.print("setup");

  M5.Power.setLed(0);

  ble.setup_after_begin();
}

void loop() {
  M5.update();
  M5.Display.print("loop");

  // notify changed value
  if (ble.isDeviceConnected()) {
    M5.Power.setLed(255);
    M5.Display.print("connected");
    ble.send("Hello, World!");
  }

  delay(5000);
}

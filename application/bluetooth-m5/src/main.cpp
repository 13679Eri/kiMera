#include "Ble.hpp"
#include "Led.hpp"

const char* DEVICE_NAME = "kiMera";
const char* SERVICE_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";
const char* CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8";

void setup_log() {
  M5.Log.setLogLevel(m5::log_target_serial, ESP_LOG_DEBUG);
  M5.Log.setEnableColor(m5::log_target_serial, true);
}

cps::ble::Controller ble{DEVICE_NAME, SERVICE_UUID, CHARACTERISTIC_UUID};

cps::led::Controller led{};

void setup() {
  auto cfg = M5.config();  // M5Stack初期設定用の構造体を代入
  M5.begin(cfg);           // M5デバイスの初期化
  setup_log();

  M5_LOGD("Waiting a client connection to notify...");

  led.red();
}

void loop() {
  M5.update();

  // notify changed value
  if (ble.isDeviceConnected()) {
    led.green();
  }
}

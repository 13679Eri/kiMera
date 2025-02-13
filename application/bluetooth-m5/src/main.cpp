#include "Ble.hpp"
#include "Led.hpp"

constexpr int32_t VOLUME_THRESHOLD = 3000;
cps::Ble_Controller ble_controller{};

void setup_log() {
  M5.Log.setLogLevel(m5::log_target_serial, ESP_LOG_DEBUG);
  M5.Log.setEnableColor(m5::log_target_serial, true);
}

void setup() {
  auto cfg = M5.config();  // M5Stack初期設定用の構造体を代入
  M5.begin(cfg);           // M5デバイスの初期化
  setup_log();

  M5_LOGD("Waiting a client connection to notify...");

  cps::setup_fast_led();

  cps::led_red();
}

void loop() {
  M5.update();

  // notify changed value
  if (cps::deviceConnected) {
    cps::led_green();
    ble_controller.send();
  }

  // disconnecting
  if (!cps::deviceConnected && cps::oldDeviceConnected) {
    cps::led_blue();
    delay(500);  // give the bluetooth stack the chance to get things ready
    cps::pServer->startAdvertising();  // restart advertising
    M5_LOGD("start advertising");
    cps::oldDeviceConnected = cps::deviceConnected;
  }

  // connecting
  if (cps::deviceConnected && !cps::oldDeviceConnected) {
    // do stuff here on connecting
    cps::oldDeviceConnected = cps::deviceConnected;
    cps::led_blue();
  }
}

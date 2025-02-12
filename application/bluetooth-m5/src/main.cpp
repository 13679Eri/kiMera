#include "Ble.hpp"
#include "Led.hpp"

constexpr int32_t VOLUME_THRESHOLD = 3000;
kimera::Ble_Controller ble_controller{};

void setup_log() {
  M5.Log.setLogLevel(m5::log_target_serial, ESP_LOG_DEBUG);
  M5.Log.setEnableColor(m5::log_target_serial, true);
}

void setup() {
  auto cfg = M5.config();  // M5Stack初期設定用の構造体を代入
  M5.begin(cfg);           // M5デバイスの初期化
  setup_log();

  M5_LOGD("Waiting a client connection to notify...");

  kimera::setup_fast_led();
  ble_controller.setup();

  kimera::led_red();
}

void loop() {
  M5.update();

  // notify changed value
  if (kimera::deviceConnected) {
    kimera::led_green();
    ble_controller.send();
  }

  // disconnecting
  if (!kimera::deviceConnected && kimera::oldDeviceConnected) {
    kimera::led_blue();
    delay(500);  // give the bluetooth stack the chance to get things ready
    kimera::pServer->startAdvertising();  // restart advertising
    M5_LOGD("start advertising");
    kimera::oldDeviceConnected = kimera::deviceConnected;
  }

  // connecting
  if (kimera::deviceConnected && !kimera::oldDeviceConnected) {
    // do stuff here on connecting
    kimera::oldDeviceConnected = kimera::deviceConnected;
    kimera::led_blue();
  }
}

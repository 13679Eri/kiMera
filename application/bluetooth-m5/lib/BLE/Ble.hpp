#pragma once
#include <Arduino.h>
#include <BLE2902.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

namespace cps {
namespace ble {

class Controller;

class MyServerCallbacks : public BLEServerCallbacks {
 public:
  explicit MyServerCallbacks(Controller* controller);
  void onConnect(BLEServer* server) override;
  void onDisconnect(BLEServer* server) override;

 private:
  Controller* controller_;
};

class MyWriteCallbacks : public BLECharacteristicCallbacks {
 public:
  explicit MyWriteCallbacks(Controller* controller);
  void onWrite(BLECharacteristic* characteristic) override;

 private:
  Controller* controller_;
};

class Controller {
 private:
  const char* device_name_;
  const char* desc_uuid_;
  const char* serv_uuid_;
  const char* chara_uuid_;
  BLEServer* server_;
  BLECharacteristic* characteristic_;

  // 接続状態を管理するメンバ変数
  bool deviceConnected_;
  bool oldDeviceConnected_;

  // コールバッククラスのインスタンスをメンバとして持つ
  MyServerCallbacks myServerCallbacks_;
  MyWriteCallbacks myWriteCallbacks_;

 public:
  Controller(const char* device_name, const char* serv_uuid,
             const char* chara_uuid, void (*on_message_received)(String));

  void setup_after_begin();

  void send(const String& message);

  void setDeviceConnected(bool connected);

  // 接続状態を取得するためのゲッター
  bool isDeviceConnected() const;

  void (*on_message_received_)(String);
};

}  // namespace ble
}  // namespace cps

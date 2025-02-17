#pragma once
#include <BLE2902.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <M5Unified.h>

#include <functional>
#include <memory>
#include <string>

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
  const std::string device_name_;
  const std::string desc_uuid_;
  const std::string serv_uuid_;
  const std::string chara_uuid_;
  BLEServer* server_;
  BLECharacteristic* characteristic_;

  // 接続状態を管理するメンバ変数
  bool deviceConnected_;
  bool oldDeviceConnected_;

  // コールバッククラスのインスタンスをメンバとして持つ
  MyServerCallbacks myCallbacks_;
  MyWriteCallbacks myWriteCallbacks_;

 public:
  Controller(std::string device_name, std::string serv_uuid,
             std::string chara_uuid);
  void send(const std::string& message);

  void setDeviceConnected(bool connected);

  // 接続状態を取得するためのゲッター
  bool isDeviceConnected() const;
};

}  // namespace ble
}  // namespace cps

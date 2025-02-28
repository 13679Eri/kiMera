#include "Ble.hpp"

namespace cps {
namespace ble {

// MyServerCallbacks の実装
MyServerCallbacks::MyServerCallbacks(Controller* controller)
    : controller_(controller) {}

void MyServerCallbacks::onConnect(BLEServer* server) {
  if (controller_) {
    controller_->setDeviceConnected(true);
  }
}

void MyServerCallbacks::onDisconnect(BLEServer* server) {
  if (controller_) {
    controller_->setDeviceConnected(false);
  }
}

MyWriteCallbacks::MyWriteCallbacks(Controller* controller)
    : controller_(controller) {}

void MyWriteCallbacks::onWrite(BLECharacteristic* characteristic) {
  String value = characteristic->getValue().c_str();
  if (value.length() != 0) {
    if (controller_) {
      controller_->on_message_received_(value);
    }
  }
}

// Controller の実装
Controller::Controller(const char* device_name, const char* serv_uuid,
                       const char* chara_uuid,
                       void (*on_message_received)(const String&))
    : device_name_(device_name),
      serv_uuid_(serv_uuid),
      chara_uuid_(chara_uuid),
      server_(nullptr),
      characteristic_(nullptr),
      deviceConnected_(false),
      myCallbacks_(this),  // コールバックに自身のポインタを渡す
      myWriteCallbacks_(this),
      on_message_received_(on_message_received){};

void Controller::setup_after_begin() {
  // BLEデバイスの初期化
  BLEDevice::init(device_name_);

  // BLEサーバの作成
  server_ = BLEDevice::createServer();
  server_->setCallbacks(&myCallbacks_);

  // BLEサービスの作成
  BLEService* pService = server_->createService(serv_uuid_);

  // BLEキャラクタリスティックの作成
  characteristic_ = pService->createCharacteristic(
      chara_uuid_, BLECharacteristic::PROPERTY_READ |
                       BLECharacteristic::PROPERTY_WRITE |
                       BLECharacteristic::PROPERTY_NOTIFY |
                       BLECharacteristic::PROPERTY_INDICATE);

  // BLEディスクリプタ（0x2902）の追加
  auto descriptor = new BLE2902();
  characteristic_->addDescriptor((BLEDescriptor*)descriptor);

  // サービス開始
  pService->start();

  // アドバタイズの設定と開始
  BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(serv_uuid_);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);
  BLEDevice::startAdvertising();
}

void Controller::send(const String& message) {
  uint8_t data[message.length() + 1];
  message.getBytes(data, message.length() + 1);
  characteristic_->setValue(data, message.length() + 1);
  characteristic_->notify();
}

void Controller::setDeviceConnected(bool connected) {
  deviceConnected_ = connected;
}

bool Controller::isDeviceConnected() const { return deviceConnected_; }

}  // namespace ble
}  // namespace cps

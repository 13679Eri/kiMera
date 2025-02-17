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
  std::string value = characteristic->getValue();
  if (!value.empty()) {
    // TODO: ここに関数オブジェクトを引っ張ってくるなどして、mainに値を戻したい
    M5_LOGD("*********");
    M5_LOGD("New value: ");
    for (size_t i = 0; i < value.length(); i++) {
      M5_LOGD("%c", value[i]);
    }
    M5_LOGD("\n");
    M5_LOGD("*********");
  }
}

// Controller の実装
Controller::Controller(std::string device_name, std::string serv_uuid,
                       std::string chara_uuid)
    : device_name_(device_name),
      serv_uuid_(serv_uuid),
      chara_uuid_(chara_uuid),
      server_(nullptr),
      characteristic_(nullptr),
      deviceConnected_(false),
      myCallbacks_(this),  // コールバックに自身のポインタを渡す
      myWriteCallbacks_(this) {
  // BLEデバイスの初期化
  BLEDevice::init(device_name);

  // BLEサーバの作成
  server_ = BLEDevice::createServer();
  server_->setCallbacks(&myCallbacks_);

  // BLEサービスの作成
  BLEService* pService = server_->createService(serv_uuid_);

  // BLEキャラクタリスティックの作成
  characteristic_ = pService->createCharacteristic(
      chara_uuid, BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_WRITE |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE);

  // BLEディスクリプタ（0x2902）の追加
  std::unique_ptr<BLE2902> descriptor(new BLE2902());
  characteristic_->addDescriptor(descriptor.get());
  descriptor.release();

  // サービス開始
  pService->start();

  // アドバタイズの設定と開始
  BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(serv_uuid_);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);
  BLEDevice::startAdvertising();
}

void Controller::send(const std::string& message) {
  characteristic_->setValue(message);
  characteristic_->notify();
}

void Controller::setDeviceConnected(bool connected) {
  deviceConnected_ = connected;
}

bool Controller::isDeviceConnected() const { return deviceConnected_; }

}  // namespace ble
}  // namespace cps

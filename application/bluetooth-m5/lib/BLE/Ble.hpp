#pragma once
#include <BLE2902.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#include <utility/Log_Class.hpp>

namespace cps {

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) override { deviceConnected = true; }

  void onDisconnect(BLEServer* pServer) override { deviceConnected = false; }

  // データが自分に**書き込まれた**ときに呼び出されるコールバック関数
  void onWrite(BLECharacteristic* pCharacteristic) {
    std::string value = pCharacteristic->getValue();

    if (value.length() > 0) {
      M5_LOGD("*********");  // NOLINT
      M5_LOGD("New value: ");
      for (int i = 0; i < value.length(); i++) {
        M5_LOGD("%c", value[i]);
      }

      M5_LOGD("\n");
      M5_LOGD("*********");
    }
  }
};

class Ble_Controller {
  const std::string DeviceName;
  // the following for generating UUIDs: https://www.uuidgenerator.net/
  const std::string Descriptor_Uuid;
  const std::string ServUuid;
  const std::string CHARACTERISTIC_UUID;
  BLEServer* pServer = nullptr;
  BLECharacteristic* pCharacteristic = nullptr;
  bool deviceConnected = false;
  bool oldDeviceConnected = false;

 public:
  Ble_Controller(std::string DeviceName, std::string DescUuid,
                 std::string ServUuid, std::string CharUuid)
      : DEVICE_NAME(DeviceName),
        DESCRIPTER_UUID(DescUuid),
        SERVICE_UUID(ServUuid),
        CHARACTERISTIC_UUID(CharUuid) {
    // Create the BLE Device
    BLEDevice::init(D);

    // Create the BLE Server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    // Create the BLE Service
    BLEService* pService = pServer->createService(SERVICE_UUID);

    // Create a BLE Characteristic
    pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ |
                                 BLECharacteristic::PROPERTY_WRITE |
                                 BLECharacteristic::PROPERTY_NOTIFY |
                                 BLECharacteristic::PROPERTY_INDICATE);

    // Creates BLE Descriptor 0x2902: Client Characteristic Configuration
    // Descriptor
    std::unique_ptr<BLE2902> descriptor{new BLE2902{}};
    pCharacteristic->addDescriptor(descriptor.get());
    // Start the service
    descriptor.release();
    pService->start();

    // Start advertising
    BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(false);
    pAdvertising->setMinPreferred(
        0x0);  // set value to 0x00 to not advertise this parameter
    BLEDevice::startAdvertising();
  }

  void send() {
    // 送信したい文字列（UTF-8）
    std::string message = u8"hello";
    pCharacteristic->setValue(message);
    pCharacteristic->notify();
  }
};

}  // namespace cps

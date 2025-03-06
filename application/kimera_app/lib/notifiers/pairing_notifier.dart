import 'dart:async';
import 'dart:convert';

import 'package:flutter/material.dart';
import 'package:flutter_reactive_ble/flutter_reactive_ble.dart';
import 'package:riverpod_annotation/riverpod_annotation.dart';
import 'package:kimera_app/models/pairing_state.dart';
import 'package:kimera_app/notifiers/kimera_notifier.dart';

part 'pairing_notifier.g.dart';

@Riverpod(keepAlive: true)
class PairingNotifier extends _$PairingNotifier {
  // 定数の設定
  String _deviceNamePattern = 'kiMera';
  static final Uuid _serviceUuid =
      Uuid.parse('4fafc201-1fb5-459e-8fcc-c5c9c331914b');
  static final Uuid _characteristicUuid =
      Uuid.parse('beb5483e-36e1-4688-b7f5-ea07361b26a8');

  static const Duration _connectionDelay = Duration(seconds: 1);
  static const Duration _retryDelay = Duration(seconds: 1);
  static const Duration _connectionTimeout = Duration(seconds: 5);

  // BLE 操作用のインスタンス
  final FlutterReactiveBle _ble = FlutterReactiveBle();

  // 各種サブスクリプション
  StreamSubscription<DiscoveredDevice>? _scanSubscription;
  StreamSubscription<ConnectionStateUpdate>? _connectionSubscription;
  StreamSubscription<List<int>>? _notifySubscription;

  // リトライ用
  int _retryCount = 0;
  final int _maxRetry = 3;

  // 接続済みのデバイス情報を保持
  DiscoveredDevice? _connectedDevice;

  QualifiedCharacteristic? _characteristic;

  @override
  PairingState build() {
    // Notifier が破棄されるときのクリーンアップ
    ref.onDispose(() {
      debugPrint("PairingNotifier disposed");
      _scanSubscription?.cancel();
      _connectionSubscription?.cancel();
      _notifySubscription?.cancel();
    });
    return const Idle();
  }

  void setDeviceNamePattern(String deviceNamePattern) {
    debugPrint("setDeviceNamePattern: $deviceNamePattern");
    _deviceNamePattern = deviceNamePattern;
  }

  /// スキャン開始
  void startScan() {
    debugPrint("pairingNotifier: startScan");
    _retryCount = 0;
    state = const Scanning();

    _scanSubscription = _ble
        .scanForDevices(withServices: [], scanMode: ScanMode.lowLatency).listen(
      (device) {
        if (device.name.contains(_deviceNamePattern)) {
          _stopScan();
          _connect(device);
        }
      },
      onError: (error) {
        state = PairingError(message: 'Scan error: $error');
      },
    );
  }

  /// スキャン停止
  void _stopScan() {
    debugPrint("pairingNotifier: stopScan");
    _scanSubscription?.cancel();
    _scanSubscription = null;
  }

  /// 指定されたデバイスに接続
  void _connect(DiscoveredDevice device) {
    debugPrint("pairingNotifier: connect: ${device.name}");
    state = Connecting(deviceName: device.name);
    _connectionSubscription = _ble
        .connectToDevice(
      id: device.id,
      connectionTimeout: _connectionTimeout,
    )
        .listen(
      (connectionState) async {
        final connState = connectionState.connectionState;
        if (connState == DeviceConnectionState.connected) {
          await Future.delayed(_connectionDelay);
          _connectedDevice = device;
          _characteristic = QualifiedCharacteristic(
            serviceId: _serviceUuid,
            characteristicId: _characteristicUuid,
            deviceId: _connectedDevice!.id,
          );
          state = Connected();
        } else if (connState == DeviceConnectionState.disconnected) {
          _attemptRetry(() => _connect(device));
        }
      },
      onError: (error) {
        state = PairingError(message: 'Connection error: $error');
        _attemptRetry(() => _connect(device));
      },
    );
  }

  /// リトライ処理（最大 _maxRetry 回まで）
  void _attemptRetry(VoidCallback action) {
    debugPrint("attemptRetry: $_retryCount");
    if (_retryCount < _maxRetry) {
      _retryCount++;
      Future.delayed(_retryDelay, action);
    } else {
      state = const PairingError(message: 'Max retry reached. Give up.');
    }
  }

  /// characteristic の値を読み出す
  Future<String?> readCharacteristic() async {
    debugPrint("pairingNotifier: readCharacteristic");
    assert(_connectedDevice != null);
    try {
      final data = await _ble.readCharacteristic(_characteristic!);
      debugPrint('Read data: $data');
      return utf8.decode(data);
    } catch (e) {
      return null;
    }
  }

  /// characteristic に値を書き込む（「Write」ボタン用）
  Future<void> writeCharacteristic(String value) async {
    debugPrint("pairingNotifier: writeCharacteristic");
    assert(_connectedDevice != null);
    try {
      final data = utf8.encode(value);
      await _ble.writeCharacteristicWithResponse(_characteristic!, value: data);
      debugPrint('Write data: $data');
    } catch (e) {
      debugPrint('Write error: $e');
    }
  }

  /// 通知購読
  Future<void> subscribe() async {
    assert(_connectedDevice != null);
    try {
      _notifySubscription =
          _ble.subscribeToCharacteristic(_characteristic!).listen(
        (data) {
          debugPrint('Received data: $data');
          String message = utf8.decode(data);
          ref.read(kimeraNotifierProvider.notifier).onReceivedMessage(message);
        },
        onError: (error) {
          state = PairingError(message: 'Notify error: $error');
        },
      );
    } catch (e) {
      state = PairingError(message: 'Subscribe error: $e');
    }
  }

  /// 切断
  void disconnect() {
    debugPrint("disconnect");
    // 繋がっているか分からないけど一応呼んでおく、という利用も考えて
    if (_connectedDevice == null) {
      debugPrint("No device connected");
      return;
    }

    _notifySubscription?.cancel();
    _connectionSubscription?.cancel();
    _connectedDevice = null;
    _characteristic = null;
    state = const Idle();
  }
}

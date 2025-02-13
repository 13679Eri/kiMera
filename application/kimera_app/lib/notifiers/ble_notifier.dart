// lib/notifiers/ble_notifier.dart
import 'dart:async';
import 'dart:convert';

import 'package:flutter/material.dart';
import 'package:flutter_reactive_ble/flutter_reactive_ble.dart';
import 'package:riverpod_annotation/riverpod_annotation.dart';
import '../models/ble_state.dart';

part 'ble_notifier.g.dart';

@riverpod
class BleNotifier extends _$BleNotifier {
  // 定数の設定
  static const String _deviceNamePattern = 'kiMera';
  static final Uuid _serviceUuid =
      Uuid.parse('c39e46c6-88d8-48d2-9310-278848445900');
  static final Uuid _characteristicUuid =
      Uuid.parse('68d37433-bd72-4ee1-95e0-b9f1d1e27dce');

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

  @override
  BleAppState build() {
    // Notifier が破棄されるときのクリーンアップ
    ref.onDispose(() {
      _scanSubscription?.cancel();
      _connectionSubscription?.cancel();
      _notifySubscription?.cancel();
    });
    return const Idle();
  }

  /// スキャン開始
  void startScan() {
    _stopScan();
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
        state = BleError(message: 'Scan error: $error');
      },
    );
  }

  /// スキャン停止
  void _stopScan() {
    _scanSubscription?.cancel();
    _scanSubscription = null;
  }

  /// 指定されたデバイスに接続
  void _connect(DiscoveredDevice device) {
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
          state = Connected(deviceName: device.name);
        } else if (connState == DeviceConnectionState.disconnected) {
          _attemptRetry(() => _connect(device));
        }
      },
      onError: (error) {
        state = BleError(message: 'Connection error: $error');
        _attemptRetry(() => _connect(device));
      },
    );
  }

  /// リトライ処理（最大 _maxRetry 回まで）
  void _attemptRetry(VoidCallback action) {
    if (_retryCount < _maxRetry) {
      _retryCount++;
      Future.delayed(_retryDelay, action);
    } else {
      state = const BleError(message: 'Max retry reached. Give up.');
    }
  }

  /// サービス発見と通知購読
  Future<void> subscribe() async {
    try {
      final characteristic = QualifiedCharacteristic(
        serviceId: _serviceUuid,
        characteristicId: _characteristicUuid,
        deviceId: _connectedDevice!.id,
      );

      _notifySubscription =
          _ble.subscribeToCharacteristic(characteristic).listen(
        (data) {
          final dataStr = utf8.decode(data);
        },
        onError: (error) {
          state = BleError(message: 'Notify error: $error');
        },
      );
    } catch (e) {
      state = BleError(message: 'Discover/Subscribe error: $e');
    }
  }

  /// 通知購読を解除する（「Unsubscribe」など任意のボタンで）
  Future<void> unsubscribe() async {
    await _notifySubscription?.cancel();
    _notifySubscription = null;
    if (_connectedDevice != null) {
      state = Connected(deviceName: _connectedDevice!.name);
    }
  }

  /// characteristic の値を読み出す（「Read」ボタン用）
  Future<void> readCharacteristic() async {
    if (_connectedDevice == null) {
      state = const BleError(message: 'No connected device.');
      return;
    }
    try {
      final characteristic = QualifiedCharacteristic(
        serviceId: _serviceUuid,
        characteristicId: _characteristicUuid,
        deviceId: _connectedDevice!.id,
      );
      final data = await _ble.readCharacteristic(characteristic);
      debugPrint('Read data: ${utf8.decode(data)}');
      // 必要に応じて state を更新したり、UI に反映する
    } catch (e) {
      state = BleError(message: 'Read error: $e');
    }
  }

  /// characteristic に値を書き込む（「Write」ボタン用）
  Future<void> writeCharacteristic(String value) async {
    if (_connectedDevice == null) {
      state = const BleError(message: 'No connected device.');
      return;
    }
    try {
      final characteristic = QualifiedCharacteristic(
        serviceId: _serviceUuid,
        characteristicId: _characteristicUuid,
        deviceId: _connectedDevice!.id,
      );
      final data = utf8.encode(value);
      await _ble.writeCharacteristicWithResponse(characteristic, value: data);
      debugPrint('Write successful: $value');
      // 書き込み成功時の処理を追加可能
    } catch (e) {
      state = BleError(message: 'Write error: $e');
    }
  }
}

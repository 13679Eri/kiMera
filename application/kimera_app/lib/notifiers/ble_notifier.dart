// lib/notifiers/ble_notifier.dart
import 'dart:async';
import 'dart:convert';

import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter_reactive_ble/flutter_reactive_ble.dart';
import 'package:riverpod_annotation/riverpod_annotation.dart';
import '../models/ble_state.dart';

part 'ble_notifier.g.dart';

@riverpod
class BleNotifier extends _$BleNotifier {
  // 定数の設定
  static const String _targetDeviceName = 'kitahatsu';
  static final Uuid _serviceUuid =
      Uuid.parse('c39e46c6-88d8-48d2-9310-278848445900');
  static final Uuid _characteristicUuid =
      Uuid.parse('68d37433-bd72-4ee1-95e0-b9f1d1e27dce');

  static const Duration _connectionDelay = Duration(seconds: 1);
  static const Duration _retryDelay = Duration(seconds: 1);
  static const Duration _receiveCooldown = Duration(seconds: 1);
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

  // 連続受信防止用フラグ
  bool _canReceive = true;

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
    _canReceive = true;
    state = const Scanning();

    _scanSubscription = _ble
        .scanForDevices(withServices: [], scanMode: ScanMode.lowLatency).listen(
      (device) {
        if (device.name == _targetDeviceName) {
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
          _discoverServicesAndSubscribe(device);
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

  /// サービス発見と通知購読
  Future<void> _discoverServicesAndSubscribe(DiscoveredDevice device) async {
    try {
      state = const DiscoveringServices();
      final services = await _ble.getDiscoveredServices(device.id);
      debugPrint('Discovered services: $services');

      // 初期の Subscribed 状態（受信データは空）
      state = const Subscribed(receivedData: [], receivedCount: 0);

      final characteristic = QualifiedCharacteristic(
        serviceId: _serviceUuid,
        characteristicId: _characteristicUuid,
        deviceId: device.id,
      );

      _notifySubscription =
          _ble.subscribeToCharacteristic(characteristic).listen(
        (data) {
          if (!_canReceive) return;
          if (state is Subscribed) {
            final current = state as Subscribed;
            final newCount = current.receivedCount + 1;
            final dataStr = utf8.decode(data);
            final newData = List<String>.from(current.receivedData)
              ..add('[$newCount] $dataStr');
            state = Subscribed(receivedData: newData, receivedCount: newCount);
          }
          HapticFeedback.vibrate();
          _canReceive = false;
          Future.delayed(_receiveCooldown, () {
            _canReceive = true;
          });
        },
        onError: (error) {
          state = BleError(message: 'Notify error: $error');
          _attemptRetry(() => _discoverServicesAndSubscribe(device));
        },
      );
    } catch (e) {
      state = BleError(message: 'Discover/Subscribe error: $e');
      _attemptRetry(() => _discoverServicesAndSubscribe(device));
    }
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
}

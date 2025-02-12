// lib/models/ble_state.dart
sealed class BleAppState {
  const BleAppState();
}

class Idle extends BleAppState {
  const Idle();
}

class Scanning extends BleAppState {
  const Scanning();
}

class Connecting extends BleAppState {
  final String deviceName;
  const Connecting({required this.deviceName});
}

class DiscoveringServices extends BleAppState {
  const DiscoveringServices();
}

class Subscribed extends BleAppState {
  final List<String> receivedData;
  final int receivedCount;
  const Subscribed({required this.receivedData, required this.receivedCount});
}

class BleError extends BleAppState {
  final String message;
  const BleError({required this.message});
}

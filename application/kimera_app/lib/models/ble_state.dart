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

class Connected extends BleAppState {
  final String deviceName;
  const Connected({required this.deviceName});
}

class BleError extends BleAppState {
  final String message;
  const BleError({required this.message});
}

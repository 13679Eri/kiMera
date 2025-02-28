// lib/models/ble_state.dart
sealed class PairingState {
  const PairingState();
}

class Idle extends PairingState {
  const Idle();
}

class Scanning extends PairingState {
  const Scanning();
}

class Connecting extends PairingState {
  final String deviceName;
  const Connecting({required this.deviceName});
}

class Connected extends PairingState {
  const Connected();
}

class PairingError extends PairingState {
  final String message;
  const PairingError({required this.message});
}

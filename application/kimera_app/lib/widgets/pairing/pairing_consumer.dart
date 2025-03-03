// lib/widgets/ble_widget.dart
import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:kimera_app/widgets/pairing/connected_view.dart';
import 'package:kimera_app/models/pairing_state.dart';
import 'package:kimera_app/notifiers/pairing_notifier.dart';
import 'idle_view.dart';
import 'scanning_view.dart';
import 'connecting_view.dart';
import 'error_view.dart';

class PairingConsumer extends ConsumerWidget {
  const PairingConsumer({super.key});

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    final pairingState = ref.watch(pairingNotifierProvider);

    return switch (pairingState) {
      Idle() => IdleView(),
      Scanning() => const ScanningView(),
      Connecting(deviceName: final deviceName) =>
        ConnectingView(deviceName: deviceName),
      Connected() => const ConnectedView(),
      PairingError(message: final message) => ErrorView(message: message),
    };
  }
}

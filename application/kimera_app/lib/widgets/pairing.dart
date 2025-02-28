// lib/widgets/ble_widget.dart
import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:kimera_app/widgets/pairing/connected_view.dart';
import '../../models/pairing_state.dart';
import '../../notifiers/pairing_notifier.dart';
import 'pairing/idle_view.dart';
import 'pairing/scanning_view.dart';
import 'pairing/connecting_view.dart';
import 'pairing/error_view.dart';

class PairingConsumer extends ConsumerWidget {
  const PairingConsumer({super.key});

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    final pairingState = ref.watch(pairingNotifierProvider);
    final pairingNotifier = ref.read(pairingNotifierProvider.notifier);

    return Center(
        child: switch (pairingState) {
      Idle() => IdleView(onStartScan: pairingNotifier.startScan),
      Scanning() => const ScanningView(),
      Connecting(deviceName: final deviceName) =>
        ConnectingView(deviceName: deviceName),
      Connected() => const ConnectedView(),
      PairingError(message: final message) =>
        ErrorView(message: message, onRetry: pairingNotifier.startScan),
    });
  }
}

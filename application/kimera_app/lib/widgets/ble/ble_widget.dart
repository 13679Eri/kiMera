// lib/widgets/ble_widget.dart
import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import '../../models/ble_state.dart';
import '../../notifiers/ble_notifier.dart';
import 'idle_view.dart';
import 'scanning_view.dart';
import 'connecting_view.dart';
import '../kimera_home.dart';
import 'error_view.dart';

class BleWidget extends ConsumerWidget {
  const BleWidget({super.key});

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    final bleState = ref.watch(bleNotifierProvider);
    final bleNotifier = ref.read(bleNotifierProvider.notifier);

    return Scaffold(
      appBar: AppBar(),
      body: switch (bleState) {
        Idle() => IdleView(onStartScan: bleNotifier.startScan),
        Scanning() => const ScanningView(),
        Connecting(deviceName: final deviceName) =>
          ConnectingView(deviceName: deviceName),
        Connected(deviceName: final deviceName) =>
          KimeraHome(deviceName: deviceName),
        BleError(message: final message) =>
          ErrorView(message: message, onRetry: bleNotifier.startScan),
      },
    );
  }
}

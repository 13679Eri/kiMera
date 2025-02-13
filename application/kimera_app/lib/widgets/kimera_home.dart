// 例：home_screen.dart
import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import '../notifiers/ble_notifier.dart';

class KimeraHome extends ConsumerWidget {
  final String deviceName;
  const KimeraHome({super.key, required this.deviceName});

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    final bleNotifier = ref.watch(bleNotifierProvider.notifier);
    return Scaffold(
      appBar: AppBar(title: const Text('kiMera Home')),
      body: Padding(
        padding: const EdgeInsets.all(16),
        child: Column(
          children: [
            ElevatedButton(
              onPressed: () => bleNotifier.subscribe(),
              child: const Text('Subscribe'),
            ),
            const SizedBox(height: 8),
            ElevatedButton(
              onPressed: () => bleNotifier.readCharacteristic(),
              child: const Text('Read'),
            ),
            const SizedBox(height: 8),
            ElevatedButton(
              onPressed: () => bleNotifier.writeCharacteristic('Hello BLE'),
              child: const Text('Write'),
            ),
            const SizedBox(height: 8),
            ElevatedButton(
              onPressed: () => bleNotifier.unsubscribe(),
              child: const Text('Unsubscribe'),
            ),
          ],
        ),
      ),
    );
  }
}

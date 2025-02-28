import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import '../../notifiers/kimera_notifier.dart';

class ModeSelectView extends ConsumerWidget {
  const ModeSelectView({super.key});

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    final kimeraNotifier = ref.watch(kimeraNotifierProvider.notifier);
    return Center(
      child: Padding(
        padding: const EdgeInsets.all(16),
        child: Column(
          children: [
            ElevatedButton(
              onPressed: () => kimeraNotifier.goToPiano(),
              child: const Text('ピアノモード'),
            ),
            const SizedBox(height: 8),
            ElevatedButton(
              onPressed: () => kimeraNotifier.goToFlute(),
              child: const Text('フルートモード'),
            ),
            const SizedBox(height: 8),
            ElevatedButton(
              onPressed: () => kimeraNotifier.goToTrumpet(),
              child: const Text('トランペットモード'),
            ),
            const SizedBox(height: 8),
            ElevatedButton(
              onPressed: () => kimeraNotifier.goToViolin(),
              child: const Text('バイオリンモード'),
            ),
          ],
        ),
      ),
    );
  }
}

import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:kimera_app/notifiers/pairing_notifier.dart';

class IdleView extends ConsumerWidget {
  const IdleView({super.key});

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('kiMera App  v1.0'),
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
      ),
      body: Center(
        child: Column(
          mainAxisSize: MainAxisSize.min,
          children: [
            const Text('kiMeraが接続されていません'),
            const SizedBox(height: 64),
            ElevatedButton(
              onPressed: () {
                ref.read(pairingNotifierProvider.notifier).disconnect();
                ref.read(pairingNotifierProvider.notifier).startScan();
              },
              child: const Text('指定したkiMeraとペアリングする'),
            ),
          ],
        ),
      ),
    );
  }
}

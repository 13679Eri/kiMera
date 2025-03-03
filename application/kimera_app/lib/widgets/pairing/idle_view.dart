import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:kimera_app/notifiers/pairing_notifier.dart';

class IdleView extends ConsumerWidget {
  const IdleView({super.key});

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    final onStartScan = ref.read(pairingNotifierProvider.notifier).startScan;
    return MaterialApp(
      theme: ThemeData(
        colorScheme: ColorScheme.fromSeed(seedColor: Colors.deepPurple),
        useMaterial3: true,
      ),
      home: Scaffold(
        appBar: AppBar(
          title: const Text('kiMera設定アプリ'),
          backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        ),
        body: Center(
          child: Column(
            mainAxisSize: MainAxisSize.min,
            children: [
              const Text('kiMera未接続'),
              const SizedBox(height: 64),
              ElevatedButton(
                onPressed: onStartScan,
                child: const Text('スキャン開始'),
              ),
            ],
          ),
        ),
      ),
    );
  }
}

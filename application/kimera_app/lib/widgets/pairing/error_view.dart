import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:kimera_app/notifiers/pairing_notifier.dart';

class ErrorView extends ConsumerWidget {
  final String message;

  const ErrorView({super.key, required this.message});

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    final onRetry = ref.read(pairingNotifierProvider.notifier).startScan;
    return Material(
      child:
      Center(
        child: Column(
          mainAxisSize: MainAxisSize.min,
          children: [
            Text('エラー: $message'),
            const SizedBox(height: 16),
            ElevatedButton(
              onPressed: onRetry,
              child: const Text('リトライ'),
            ),
          ],
        ),
      )
    );
  }
}

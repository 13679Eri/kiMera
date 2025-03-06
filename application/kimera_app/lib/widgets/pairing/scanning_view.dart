import 'package:flutter/material.dart';

class ScanningView extends StatelessWidget {
  const ScanningView({super.key});

  @override
  Widget build(BuildContext context) {
    return Material(
      child: Center(
        child: Column(
          mainAxisSize: MainAxisSize.min,
          children: const [
            Text('付近のデバイスをスキャン中...'),
            SizedBox(height: 16),
            CircularProgressIndicator(),
          ],
        ),
      ),
    );
  }
}

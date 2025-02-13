import 'package:flutter/material.dart';

class ScanningView extends StatelessWidget {
  const ScanningView({super.key});

  @override
  Widget build(BuildContext context) {
    return Center(
      child: Column(
        mainAxisSize: MainAxisSize.min,
        children: const [
          Text('スキャン中...'),
          SizedBox(height: 16),
          CircularProgressIndicator(),
        ],
      ),
    );
  }
}

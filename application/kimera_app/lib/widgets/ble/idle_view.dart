import 'package:flutter/material.dart';

class IdleView extends StatelessWidget {
  final VoidCallback onStartScan;

  const IdleView({super.key, required this.onStartScan});

  @override
  Widget build(BuildContext context) {
    return Center(
      child: Column(
        mainAxisSize: MainAxisSize.min,
        children: [
          const Text('kiMera未接続'),
          const SizedBox(height: 16),
          ElevatedButton(
            onPressed: onStartScan,
            child: const Text('スキャン開始'),
          ),
        ],
      ),
    );
  }
}

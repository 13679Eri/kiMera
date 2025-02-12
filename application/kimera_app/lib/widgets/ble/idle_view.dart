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
          const Text('Idle state.'),
          const SizedBox(height: 16),
          ElevatedButton(
            onPressed: onStartScan,
            child: const Text('Start Scan'),
          ),
        ],
      ),
    );
  }
}

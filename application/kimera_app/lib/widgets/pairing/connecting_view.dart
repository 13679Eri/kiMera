import 'package:flutter/material.dart';

class ConnectingView extends StatelessWidget {
  final String deviceName;

  const ConnectingView({super.key, required this.deviceName});

  @override
  Widget build(BuildContext context) {
    return Center(
      child: Column(
        mainAxisSize: MainAxisSize.min,
        children: [
          Text('$deviceName に接続中...'),
          const SizedBox(height: 16),
          const CircularProgressIndicator(),
        ],
      ),
    );
  }
}

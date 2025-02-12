import 'package:flutter/material.dart';

class DiscoveringServicesView extends StatelessWidget {
  const DiscoveringServicesView({super.key});

  @override
  Widget build(BuildContext context) {
    return Center(
      child: Column(
        mainAxisSize: MainAxisSize.min,
        children: const [
          Text('Discovering services...'),
          SizedBox(height: 16),
          CircularProgressIndicator(),
        ],
      ),
    );
  }
}

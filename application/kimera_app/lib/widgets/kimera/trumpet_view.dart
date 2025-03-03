import 'package:flutter/material.dart';

class TrumpetView extends StatelessWidget {
  const TrumpetView({super.key});

  @override
  Widget build(BuildContext context) {
    return Material(
      child:Center(
      child: Column(
        mainAxisSize: MainAxisSize.min,
        children: [
          Image.asset('assets/trumpet.png'),
          Text('Trumpet画面'),
          const SizedBox(height: 16),
          const CircularProgressIndicator(),
        ],
      ),
      ),
    );
  }
}

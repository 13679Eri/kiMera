import 'package:flutter/material.dart';

class FluteView extends StatelessWidget {
  const FluteView({super.key});

  @override
  Widget build(BuildContext context) {
    return Material(
      child:Center(
      child: Column(
        mainAxisSize: MainAxisSize.min,
        children: [
          Image.asset('assets/flute.png'),
          Text('Flute画面'),
          const SizedBox(height: 16),
          const CircularProgressIndicator(),
        ],
      ),
      ),
    );
  }
}

import 'package:flutter/material.dart';

class PianoView extends StatefulWidget {
  int transpose = 0;
  PianoView({super.key});

  @override
  Widget build(BuildContext context) {
    return Center(
      child: Column(
        mainAxisSize: MainAxisSize.min,
        children: [
          Text('Piano画面'),
          Text('transpose: $transpose'),
          //トランスポーズボタン
          Row(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              ElevatedButton(
                onPressed: () {
                  transpose = transpose + 1;
                },
                child: const Text('+1'),
              ),
              const SizedBox(width: 16),
              ElevatedButton(
                onPressed: () {
                  transpose = transpose - 1;
                },
                child: const Text('-1'),
              ),
            ],
          ),
        ],
      ),
    );
  }
}

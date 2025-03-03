import 'package:flutter/material.dart';

class PianoView extends StatefulWidget {
  const PianoView({super.key});

  @override
  State<PianoView> createState() => PianoViewState();
}

class PianoViewState extends State<PianoView> {
  int transpose = 0;
  @override
  Widget build(BuildContext context) {
    return Material(
      child: Center(
        child: Column(
          mainAxisSize: MainAxisSize.min,
          children: [
            Image.asset('assets/piano.png'),
            Text('Piano'),
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
                ElevatedButton(
                    onPressed: () {
                      Navigator.of(context).pop();
                    },
                    child: const Text('戻る')),
              ],
            ),
          ],
        ),
      ),
    );
  }
}

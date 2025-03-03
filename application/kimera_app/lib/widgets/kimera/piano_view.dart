import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:go_router/go_router.dart';
import 'package:kimera_app/notifiers/pairing_notifier.dart';

class PianoView extends ConsumerStatefulWidget {
  const PianoView({super.key});

  @override
  PianoViewState createState() => PianoViewState();
}

class PianoViewState extends ConsumerState<PianoView> {
  int transpose = 0;

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
                  ref
                      .read(pairingNotifierProvider.notifier)
                      .writeCharacteristic("hello");
                },
                child: const Text('+1'),
              ),
              const SizedBox(width: 16),
              ElevatedButton(
                onPressed: () async {
                  transpose = transpose - 1;
                  String? text = await ref
                      .read(pairingNotifierProvider.notifier)
                      .readCharacteristic();
                  if (text != null) {
                    transpose = 100;
                  }
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
    );
  }
}

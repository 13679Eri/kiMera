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
  void initState() {
    super.initState();
    //TODO: json_serializableを使って、jsonを作成する
    ref
        .read(pairingNotifierProvider.notifier)
        .writeCharacteristic("{\"mode\": \"piano\"}");
  }

  void _incrementTranspose() {
    setState(() {
      transpose++;
    });
  }

  void _decrementTranspose() {
    setState(() {
      transpose--;
    });
  }

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
                onPressed: _incrementTranspose,
                child: const Text('+1'),
              ),
              const SizedBox(width: 16),
              ElevatedButton(
                onPressed: _decrementTranspose,
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

import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:kimera_app/models/json_schema.dart';
import 'package:kimera_app/notifiers/kimera_notifier.dart';

class FluteView extends ConsumerStatefulWidget {
  const FluteView({super.key});

  @override
  FluteViewState createState() => FluteViewState();
}

class FluteViewState extends ConsumerState<FluteView> {
  int transpose = 0;

  @override
  void initState() {
    super.initState();
    JsonSchema filled = JsonSchema(
      mode: Instruments.flute,
    );
    ref.read(kimeraNotifierProvider.notifier).send(filled);
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
          Text('Flute画面'),
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
                    ref.read(kimeraNotifierProvider.notifier).modeSelect();
                  },
                  child: const Text('モードセレクト画面に戻る')),
            ],
          ),
        ],
      ),
    );
  }
}

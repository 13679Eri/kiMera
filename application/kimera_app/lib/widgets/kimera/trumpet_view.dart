import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:kimera_app/models/json_schema.dart';
import 'package:kimera_app/notifiers/kimera_notifier.dart';

class TrumpetView extends ConsumerStatefulWidget {
  const TrumpetView({super.key});

  @override
  TrumpetViewState createState() => TrumpetViewState();
}

class TrumpetViewState extends ConsumerState<TrumpetView> {
  int transpose = 0;

  @override
  void initState() {
    super.initState();
    JsonSchema filled = JsonSchema(
      mode: Instruments.trumpet,
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
    return Material(
      child:Center(
      child: Column(
        mainAxisSize: MainAxisSize.min,
        children: [
          Image.asset('assets/trumpet.png'),
Text('Trumpet画面'),
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
      ),
    );
  }
}

import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:kimera_app/notifiers/kimera_notifier.dart';
import 'package:kimera_app/notifiers/pairing_notifier.dart';
import 'package:kimera_app/widgets/kimera/row_text_image_widget.dart';

class ModeSelectView extends ConsumerWidget {
  const ModeSelectView({super.key});

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    return MaterialApp(
      theme: ThemeData(
        colorScheme: ColorScheme.fromSeed(seedColor: Colors.deepPurple),
        useMaterial3: true,
      ),
      home: Scaffold(
        appBar: AppBar(
          title: const Text('楽器を選択してください！'),
          backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        ),
        body: Center(
          child: Column(
            children: [
              const SizedBox(height: 30),
              ElevatedButton(
                onPressed: () =>
                    ref.read(kimeraNotifierProvider.notifier).pianoMode(),
                child: RowTextImage(
                  text: const Text('ピアノ',
                      style: TextStyle(fontWeight: FontWeight.bold)),
                  image: Image.asset(
                    'assets/piano.png',
                    width: 80,
                  ),
                ),
              ),
              const SizedBox(height: 30),
              ElevatedButton(
                onPressed: () =>
                    ref.read(kimeraNotifierProvider.notifier).violinMode(),
                child: RowTextImage(
                  text: const Text('ヴァイオリン',
                      style: TextStyle(fontWeight: FontWeight.bold)),
                  image: Image.asset(
                    'assets/violin.png',
                    width: 60,
                  ),
                ),
              ),
              const SizedBox(height: 30),
              ElevatedButton(
                onPressed: () =>
                    ref.read(kimeraNotifierProvider.notifier).trumpetMode(),
                child: RowTextImage(
                  text: const Text('トランペット',
                      style: TextStyle(fontWeight: FontWeight.bold)),
                  image: Image.asset(
                    'assets/trumpet.png',
                    width: 80,
                  ),
                ),
              ),
              const SizedBox(height: 30),
              ElevatedButton(
                onPressed: () =>
                    ref.read(kimeraNotifierProvider.notifier).fluteMode(),
                child: RowTextImage(
                  text: const Text('フルート',
                      style: TextStyle(fontWeight: FontWeight.bold)),
                  image: Image.asset(
                    'assets/flute.png',
                    width: 70,
                  ),
                ),
              ),
              const SizedBox(height: 30),
              ElevatedButton(
                child: const Text('楽器の選択にもどる'),
                onPressed: () {
                  ref.read(pairingNotifierProvider.notifier).disconnect();
                  ref.read(kimeraNotifierProvider.notifier).restart();
                },
              ),
            ],
          ),
        ),
      ),
    );
  }
}

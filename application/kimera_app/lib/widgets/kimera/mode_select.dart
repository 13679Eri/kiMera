import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:kimera_app/notifiers/kimera_notifier.dart';
import 'package:go_router/go_router.dart';
import 'package:kimera_app/notifiers/pairing_notifier.dart';
import 'package:kimera_app/widgets/kimera/piano_view.dart';
import 'package:kimera_app/widgets/kimera/flute_view.dart';
import 'package:kimera_app/widgets/kimera/trumpet_view.dart';
import 'package:kimera_app/widgets/kimera/violin_view.dart';

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
          title: const Text('kiMera設定アプリ'),
          backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        ),
        body: Center(
          child: Column(
            children: [
              ElevatedButton(
                onPressed: () => Navigator.of(context).push(
                    MaterialPageRoute(builder: (context) => const PianoView())),
                child: const Text('ピアノモード'),
              ),
              const SizedBox(height: 8),
              ElevatedButton(
                onPressed: () => Navigator.of(context).push(
                    MaterialPageRoute(builder: (context) => const FluteView())),
                child: const Text('フルートモード'),
              ),
              const SizedBox(height: 8),
              ElevatedButton(
                onPressed: () => Navigator.of(context).push(MaterialPageRoute(
                    builder: (context) => const TrumpetView())),
                child: const Text('トランペットモード'),
              ),
              const SizedBox(height: 8),
              ElevatedButton(
                onPressed: () => Navigator.of(context).push(MaterialPageRoute(
                    builder: (context) => const ViolinView())),
                child: const Text('バイオリンモード'),
              ),
              const SizedBox(height: 8),
              ElevatedButton(
                child: const Text('もどる'),
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

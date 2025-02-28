// 例：home_screen.dart
import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import '../notifiers/kimera_notifier.dart';
import '../models/kimera_state.dart';
import 'pairing.dart';
import 'kimera/modeselect_view.dart';
import 'kimera/piano_view.dart';
import 'kimera/flute_view.dart';
import 'kimera/trumpet_view.dart';
import 'kimera/violin_view.dart';

class Kimera extends ConsumerWidget {
  final String deviceName;
  const Kimera({super.key, required this.deviceName});

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    final kimeraState = ref.watch(kimeraNotifierProvider);
    return Scaffold(
      appBar: AppBar(),
      body: switch (kimeraState) {
        Pairing() => PairingConsumer(),
        ModeSelect() => ModeSelectView(),
        Piano() => PianoView(),
        Flute() => const FluteView(),
        Trumpet() => const TrumpetView(),
        Violin() => const ViolinView(),
      },
    );
  }
}

// 例：home_screen.dart
import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:kimera_app/notifiers/kimera_notifier.dart';
import 'package:kimera_app/models/kimera_state.dart';
import 'pairing/pairing_consumer.dart';
import 'kimera/mode_select_consumer.dart';
import 'kimera/piano_view.dart';
import 'kimera/flute_view.dart';
import 'kimera/trumpet_view.dart';
import 'kimera/violin_view.dart';

class Kimera extends ConsumerWidget {
  const Kimera({super.key});

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    final kimeraState = ref.watch(kimeraNotifierProvider);
    return switch (kimeraState) {
      Pairing() => PairingConsumer(),
      ModeSelect() => ModeSelectView(),
      Piano() => PianoView(),
      Flute() => const FluteView(),
      Trumpet() => const TrumpetView(),
      Violin() => const ViolinView(),
    };
  }
}

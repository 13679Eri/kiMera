// 例：home_screen.dart
import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:kimera_app/notifiers/kimera_notifier.dart';
import 'package:kimera_app/models/kimera_state.dart';
import 'package:kimera_app/widgets/pairing/pairing_consumer.dart';
import 'package:kimera_app/widgets/kimera/mode_select.dart';

class Kimera extends ConsumerWidget {
  const Kimera({super.key});

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    final kimeraState = ref.watch(kimeraNotifierProvider);
    return switch (kimeraState) {
      Pairing() => PairingConsumer(),
      ModeSelect() => ModeSelectView(),
    };
  }
}

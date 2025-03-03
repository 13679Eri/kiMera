import 'package:flutter/foundation.dart';
import 'package:riverpod_annotation/riverpod_annotation.dart';
import 'package:kimera_app/models/kimera_state.dart';

part 'kimera_notifier.g.dart';

@Riverpod(keepAlive: true)
class KimeraNotifier extends _$KimeraNotifier {
  @override
  KimeraState build() {
    ref.onDispose(() {
      debugPrint("KimeraNotifier disposed");
    });
    return const Pairing();
  }

  void connected() {
    state = const ModeSelect();
  }

  void restart() {
    state = const Pairing();
  }
}

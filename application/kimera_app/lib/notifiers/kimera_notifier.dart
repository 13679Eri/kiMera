import 'package:riverpod_annotation/riverpod_annotation.dart';
import '../models/kimera_state.dart';

part 'kimera_notifier.g.dart';

@riverpod
class KimeraNotifier extends _$KimeraNotifier {
  @override
  KimeraState build() {
    return const Pairing();
  }

  void connected() {
    state = const ModeSelect();
  }
}

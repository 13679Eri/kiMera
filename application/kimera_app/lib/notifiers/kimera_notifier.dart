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
    state = const Home();
  }

  void goToFlute() {
    state = const Flute();
  }

  void goToPiano() {
    state = Piano();
  }

  void goToTrumpet() {
    state = const Trumpet();
  }

  void goToViolin() {
    state = const Violin();
  }
}

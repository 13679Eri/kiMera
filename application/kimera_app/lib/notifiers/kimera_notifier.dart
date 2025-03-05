import 'package:flutter/foundation.dart';
import 'dart:convert';
import 'package:kimera_app/models/json_schema.dart';
import 'package:riverpod_annotation/riverpod_annotation.dart';
import 'package:kimera_app/notifiers/pairing_notifier.dart';
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
    debugPrint("kimeraNotifier: connected");
    state = const ModeSelect();
  }

  void send(JsonSchema filled) {
    debugPrint("kimeraNotifier: send");
    var jsonStr = json.encode(filled.toJson());
    debugPrint(jsonStr);
    ref.read(pairingNotifierProvider.notifier).writeCharacteristic(jsonStr);
  }

  void onReceivedMessage(String message_cstr) {
    //Null文字を削除
    String message = message_cstr.replaceAll('\u0000', '');
    debugPrint("kimeraNotifier: onReceivedMessage: $message");
    var jsonMap = json.decode(message);
    var received = JsonSchema.fromJson(jsonMap);
    state = switch (received.mode) {
      Instruments.piano => const Piano(),
      Instruments.trumpet => const Trumpet(),
      Instruments.flute => const Flute(),
      Instruments.violin => const Violin(),
    };
  }

  void restart() {
    debugPrint("kimeraNotifier: restart");
    state = const Pairing();
  }

  void modeSelect() {
    debugPrint("kimeraNotifier: modeSelect");
    state = const ModeSelect();
  }

  void pianoMode() {
    debugPrint("kimeraNotifier: pianoMode");
    state = const Piano();
  }

  void trumpetMode() {
    debugPrint("kimeraNotifier: trumpetMode");
    state = const Trumpet();
  }

  void fluteMode() {
    debugPrint("kimeraNotifier: fluteMode");
    state = const Flute();
  }

  void violinMode() {
    debugPrint("kimeraNotifier: violinMode");
    state = const Violin();
  }
}

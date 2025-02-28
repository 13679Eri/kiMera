// main.dart
import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:kimera_app/widgets/pairing/pairing.dart';

// Riverpod の ProviderScope でラップ
void main() => runApp(const ProviderScope(child: KimeraApp()));

class KimeraApp extends StatelessWidget {
  const KimeraApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'kiMera設定アプリ',
      theme: ThemeData(
        colorScheme: ColorScheme.fromSeed(seedColor: Colors.deepPurple),
        useMaterial3: true,
      ),
      home: const Kimera(),
    );
  }
}

// main.dart
import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:kimera_app/widgets/ble/ble_widget.dart'; // ble_widget.dart をインポート

// Riverpod の ProviderScope でラップ
void main() => runApp(const ProviderScope(child: KimeraApp()));

class KimeraApp extends StatelessWidget {
  const KimeraApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'kimera_app',
      theme: ThemeData(
        colorScheme: ColorScheme.fromSeed(seedColor: Colors.deepPurple),
        useMaterial3: true,
      ),
      home: const HomeScreen(),
    );
  }
}

class HomeScreen extends StatelessWidget {
  const HomeScreen({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        title: const Text('kimera_app'),
      ),
      body: const BleWidget(), // ble_app.dart 内のウィジェットを利用
    );
  }
}

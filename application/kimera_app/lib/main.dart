// main.dart
import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:kimera_app/widgets/kimera.dart';

void main() => runApp(ProviderScope(child: App()));

class App extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Kimera',
      theme: ThemeData(
        primarySwatch: Colors.deepPurple,
      ),
      home: const Kimera(),
    );
  }
}

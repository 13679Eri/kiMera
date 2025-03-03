// main.dart
import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';

import 'package:kimera_app/widgets/pairing/idle_view.dart';
import 'package:kimera_app/widgets/kimera.dart';
import 'package:go_router/go_router.dart';
import 'package:kimera_app/widgets/kimera/mode_select_consumer.dart';

// Riverpod の ProviderScope でラップ
//void main() => runApp(const ProviderScope(child: App()));
void main() => runApp(ProviderScope(
        child: MaterialApp.router(
      routerConfig: _router,
    )));

final _router = GoRouter(
  routes: [
    GoRoute(
      path: '/',
      builder: (context, state) => const Kimera(),
      routes: [
        GoRoute(
          path: '/idle',
          builder: (context, state) => const IdleView(),
          routes: [
            GoRoute(
              path: '/mode_select',
              builder: (context, state) => const ModeSelectView(),
            ),
          ],
        ),
      ],
    ),
  ],
);

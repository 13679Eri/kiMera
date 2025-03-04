import 'dart:async';

import 'package:flutter/material.dart';
import 'package:go_router/go_router.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:kimera_app/notifiers/kimera_notifier.dart';

class ConnectedView extends ConsumerStatefulWidget {
  const ConnectedView({super.key});

  @override
  ConnectedViewState createState() => ConnectedViewState();
}

class ConnectedViewState extends ConsumerState<ConnectedView> {
  Timer? timer;

  @override
  Widget build(BuildContext context) {
    return Material(
      child:Center(
        child: Column(
          mainAxisSize: MainAxisSize.min,
          children: [
            Text('接続完了'),
            const SizedBox(height: 16),
            const CircularProgressIndicator(),
          ],
        ),
      )
    );
  }

  @override
  void initState() {
    super.initState();
    timer = Timer(
      const Duration(seconds: 2),
      () {
        ref.read(kimeraNotifierProvider.notifier).connected();
      },
    );
  }

  @override
  void dispose() {
    timer?.cancel();
    super.dispose();
  }
}

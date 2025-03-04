import 'dart:async';

import 'package:flutter/material.dart';
import 'package:go_router/go_router.dart';

class ConnectedView extends StatefulWidget {
  const ConnectedView({super.key});

  @override
  State<ConnectedView> createState() => ConnectedViewState();
}

class ConnectedViewState extends State<ConnectedView> {
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
        GoRouter.of(context).go('/idle/mode_select');
      },
    );
  }

  @override
  void dispose() {
    timer?.cancel();
    super.dispose();
  }
}

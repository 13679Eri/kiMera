import 'package:flutter/material.dart';
import 'dart:async';

import 'package:kimera_app/widgets/kimera/modeselect_view.dart';

class ConnectedView extends StatefulWidget {
  const ConnectedView({super.key});

  @override
  State<ConnectedView> createState() => ConnectedViewState();
}

class ConnectedViewState extends State<ConnectedView> {
  Timer? timer;

  @override
  Widget build(BuildContext context) {
    return Center(
      child: Column(
        mainAxisSize: MainAxisSize.min,
        children: [
          Text('接続完了'),
          const SizedBox(height: 16),
          const CircularProgressIndicator(),
        ],
      ),
    );
  }

  @override
  void initState() {
    super.initState();
    timer = Timer(
      const Duration(seconds: 2),
      () {
        Navigator.push(
          context,
          MaterialPageRoute(builder: (context) => const ModeSelectView()),
        );
      },
    );
  }
}

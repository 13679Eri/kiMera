import 'package:flutter/material.dart';

class RowTextImage extends StatelessWidget {
  final Text text;
  final Image image;

  const RowTextImage({super.key, required this.text, required this.image});

  @override
  Widget build(BuildContext context) {
    return Material(
      child: Row(
        children: [
          const SizedBox(width: 64),
          text,
          const SizedBox(width: 96),
          image,
        ],
      ),
    );
  }
}

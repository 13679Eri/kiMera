import 'package:flutter/material.dart';

class SubscribedView extends StatelessWidget {
  final List<String> receivedData;
  final int receivedCount;

  const SubscribedView({
    super.key,
    required this.receivedData,
    required this.receivedCount,
  });

  @override
  Widget build(BuildContext context) {
    return Center(
      child: Column(
        children: [
          Text('Subscribed. Received count: $receivedCount'),
          const SizedBox(height: 16),
          Expanded(
            child: ListView.builder(
              itemCount: receivedData.length,
              itemBuilder: (context, index) {
                return ListTile(
                  title: Text(receivedData[index]),
                );
              },
            ),
          ),
        ],
      ),
    );
  }
}

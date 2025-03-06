import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:kimera_app/notifiers/pairing_notifier.dart';
import 'package:collection/collection.dart';

typedef MenuEntry = DropdownMenuEntry<String>;

class IdleView extends ConsumerStatefulWidget {
  const IdleView({super.key});
  @override
  IdleViewState createState() => IdleViewState();
}

class IdleViewState extends ConsumerState<IdleView> {
  final Map<String, String> map = <String, String>{
    'ピンクのkiMera': 'kimera1',
    '赤い透明なkiMera': 'kimera2',
  };
  late final List<MenuEntry> menuEntries;
  late String dropdownValue;

  @override
  void initState() {
    super.initState();
    menuEntries = UnmodifiableListView<MenuEntry>(
      map.keys
          .toList()
          .map<MenuEntry>((String name) => MenuEntry(value: name, label: name)),
    );
    dropdownValue = map.keys.toList().first;
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('kiMera App  v1.0'),
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
      ),
      body: Center(
        child: Column(
          mainAxisSize: MainAxisSize.min,
          children: [
            const Text('kiMeraが接続されていません'),
            const Text('接続したいkiMeraを指定して下さい'),
            const SizedBox(height: 64),
            DropdownMenu<String>(
              initialSelection: map.keys.toList().first,
              onSelected: (String? value) {
                // This is called when the user selects an item.
                setState(() {
                  dropdownValue = value ?? map.keys.toList().first;
                  debugPrint('Selected: $dropdownValue');
                });
              },
              dropdownMenuEntries: menuEntries,
            ),
            const SizedBox(height: 64),
            ElevatedButton(
              onPressed: () {
                ref.read(pairingNotifierProvider.notifier).disconnect();
                ref
                    .read(pairingNotifierProvider.notifier)
                    .setDeviceNamePattern(map[dropdownValue]!);
                ref.read(pairingNotifierProvider.notifier).startScan();
              },
              child: const Text('指定したkiMeraとペアリングする'),
            ),
          ],
        ),
      ),
    );
  }
}

# kimera_app

## 実行方法
1. このリポジトリを git clone
2. VSCodeで`kimera_app`ディレクトリを開く
3. developブランチに切り替える。コマンドでもいいが、VSCode上のGUIで選択すると楽。
4. `flutter pub get` でパッケージを取得
5. ビルド対象のデバイスを選択。ネイティブのBluetoothを使用しているので、スマートフォン実機、またはスマートフォンのシミュレータを対象にする。
6. VSCodeのデバッガを起動して実行。

## 使用ライブラリ
- RiverPod
  - 状態管理用
- Flutter reactive BLE
  - [APIドキュメント](https://pub.dev/documentation/flutter_reactive_ble/latest/)

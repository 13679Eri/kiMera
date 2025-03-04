# kimera_app

## 実行方法
1. このリポジトリを git clone
2. VSCodeで`kimera_app`ディレクトリを開く
3. `flutter pub get` でパッケージを取得
4. ビルド対象のデバイスを選択。ネイティブのBluetoothを使用しているので、スマートフォン実機、またはスマートフォンのシミュレータを対象にする。
5. VSCodeのデバッガを起動して実行。

## 使用ライブラリ
- RiverPod
  - 状態管理用
- Flutter reactive BLE
  - [APIドキュメント](https://pub.dev/documentation/flutter_reactive_ble/latest/)

## 通信用プロトコル
スキーマ
``` json
{ "mode": piano | trumpet | flute | violin }
```

例  
`{ "mode": piano }`  
`{ "mode": flute }`  


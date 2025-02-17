## kiMera
TODO: 論文のリンクを貼る  
kiMeraの拡張のひとつ、Bluetoothアプリケーションの実装です。  
Flutter製のアプリケーション`kimera_app`と、アプリと連携するためのArduinoライブラリ`bluetooth-m5`を提供します。  

### ディレクトリ構成
詳細は、それぞれのディレクトリ下にある`README.md`を参照してください。  
**bluetooth-m5**  
- kimera_appと連携するためのコード
- `Platform IO`の`Arduino Framework`が必要
- 移植性を考えてライブラリとしてまとめてある

**kimera_app**  
- Bluetooth経由でkiMeraの設定をするためのアプリケーション
- `Flutter`で作成

### 進捗 TODO
- [ ] (少しズレるが) kiMera_mainの整理
- [ ] bluetooth-m5をライブラリとしてまとめる cppとhppの分離
- [ ] アプリ、まずは楽器切り替え機能
  - [ ] 適当なjsonを送ってみて、bluetooth-m5で受け取れるかどうか





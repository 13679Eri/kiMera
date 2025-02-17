#pragma once
#include <FastLED.h>

namespace cps {
namespace led {

class Controller {
 public:
  // LED制御に必要な定数
  static constexpr int PIN_LED = 21;  // 本体フルカラーLEDの使用端子（G21）
  static constexpr int NUM_LEDS = 1;  // 本体フルカラーLEDの数

  // コンストラクタ（インスタンス生成時にセットアップを行う）
  Controller();

  // 各色に変更するメソッド
  void red();
  void green();
  void blue();
  void black();
  void white();

  // 任意の色に設定するメソッド
  void setColor(const CRGB& color);

 private:
  // LEDの状態を保持するメンバ変数（NUM_LEDS = 1 の場合、1つの CRGB で十分）
  CRGB led_;
};

}  // namespace led
}  // namespace cps

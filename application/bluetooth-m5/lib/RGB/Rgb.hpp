#pragma once

#include <Arduino.h>
#include <FastLED.h>

namespace cps {
namespace led {

class Controller {
 public:
  // コンストラクタ（インスタンス生成時にセットアップを行う）
  Controller(int8_t pin_leds, int8_t num_leds);

  // 各色に変更するメソッド
  void red();
  void yellow();
  void green();
  void blue();
  void black();
  void white();

  // 任意の色に設定するメソッド
  void setColor(const CRGB& color);

 private:
  const int8_t pin_leds_;
  const int8_t num_leds_ = 1;

  CRGB led_;
};

}  // namespace led
}  // namespace cps

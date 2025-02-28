#include "Led.hpp"

namespace cps {
namespace led {

Controller::Controller(int8_t pin_leds, int8_t num_leds)
    : pin_leds_(pin_leds), num_leds_(num_leds) {
  // FastLEDにLEDの配置を登録
  // 1要素の配列として &led_ を渡す
  FastLED.addLeds<WS2812B, pin_leds_, CGRB>(&led_, num_leds_);
  // 初期色（ここでは各色40/255の明るさの色）
  led_ = CRGB(40, 40, 40);
}

void Controller::red() { setColor(CRGB::Red); }

void Controller::yellow() { setColor(CRGB::Yellow); }

void Controller::green() { setColor(CRGB::Green); }

void Controller::blue() { setColor(CRGB::Blue); }

void Controller::black() { setColor(CRGB::Black); }

void Controller::white() { setColor(CRGB::White); }

void Controller::setColor(const CRGB& color) {
  led_ = color;
  FastLED.show();
}

}  // namespace led
}  // namespace cps

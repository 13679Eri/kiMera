#include "Led.hpp"

namespace cps {
namespace led {

Controller::Controller() {
  // FastLEDにLEDの配置を登録
  // 1要素の配列として &led_ を渡す
  FastLED.addLeds<WS2812B, PIN_LED, GRB>(&led_, NUM_LEDS);
  // 初期色（ここでは各色40/255の明るさの色）
  led_ = CRGB(40, 40, 40);
}

void Controller::red() { setColor(CRGB::Red); }

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

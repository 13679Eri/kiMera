#include <FastLED.h>

namespace cps {
// グローバル変数（プログラム全体で使用する変数の定義をします。）
constexpr int PIN_LED = 21;  // 本体フルカラーLEDの使用端子（G21）
constexpr int NUM_LEDS = 1;  // 本体フルカラーLEDの数

CRGB led{};

void setup_fast_led() {
  FastLED.addLeds<WS2812B, PIN_LED, GRB>(&led, NUM_LEDS);
  led =
      CRGB(40, 40, 40);  // 白色（赤, 緑, 青）※3色それぞれの明るさを0〜255で指定
}

void led_red() {
  led = CRGB::Red;
  FastLED.show();
}
void led_green() {
  led = CRGB::Green;
  FastLED.show();
}
void led_blue() {
  led = CRGB::Blue;
  FastLED.show();
}
void led_black() {
  led = CRGB::Black;
  FastLED.show();
}
void led_white() {
  led = CRGB::White;
  FastLED.show();
}
}  // namespace cps

#include "Seeed_MPR121_driver.h"
#include <Adafruit_DPS310.h>
Mpr121 mpr121;
Adafruit_DPS310 dps;
#define DPS310_CS 10
u16 touch_status_flag[CHANNEL_NUM] = { 0 };
void setup() {
  s32 ret = 0;
  Serial.begin(115200);
  if (mpr121.begin() < 0) {
    Serial.println("Can't detect device!!!!");
  } else {
    Serial.println("mpr121 init OK!");
  }
  if (!dps.begin_I2C()) {  // Can pass in I2C address here
                           //if (! dps.begin_SPI(DPS310_CS)) {  // If you want to use SPI
    Serial.println("Failed to find DPS");
    while (1) yield();
  }
  dps.configurePressure(DPS310_64HZ, DPS310_64SAMPLES);
  dps.configureTemperature(DPS310_64HZ, DPS310_64SAMPLES);
  delay(100);
}
void loop() {
  u16 result = 0;
  u16 filtered_data_buf[CHANNEL_NUM] = { 0 };
  u8 baseline_buf[CHANNEL_NUM] = { 0 };
  result = mpr121.check_status_register();
  mpr121.get_filtered_reg_data(&result, filtered_data_buf);
  sensors_event_t temp_event, pressure_event;
  while (!dps.temperatureAvailable() || !dps.pressureAvailable()) {
    return;  // wait until there's something to read
  }
  for (int i = 0; i < CHANNEL_NUM; i++) {
    if (result & (1 << i)) /*key i is pressed!!*/
    {
      if (0 == touch_status_flag[i]) {
        touch_status_flag[i] = 1;
        Serial.print("key ");
        Serial.print(i);
        Serial.println("pressed");
        dps.getEvents(&temp_event, &pressure_event);
        Serial.print(F("Temperature = "));
        Serial.print(temp_event.temperature);
        Serial.println(" *C");
        Serial.print(F("Pressure = "));
        Serial.print(pressure_event.pressure);
        Serial.println(" hPa");
        Serial.println();
      }
    } else {
      if (1 == touch_status_flag[i]) {
        touch_status_flag[i] = 0;
        Serial.print("key ");
        Serial.print(i);
        Serial.println("release");
      }
    }
  }
  delay(50);
}

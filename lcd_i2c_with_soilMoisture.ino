#include <LiquidCrystal_I2C.h>
const int threshold = 500;
const int sensor_pin = A0;
LiquidCrystal_I2C lcd(39, 16, 2);

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();

  pinMode(sensor_pin, INPUT);
}

void loop() {
  lcd.setCursor(0, 0);
  int moisture_val = analogRead(sensor_pin);
  if (moisture_val > threshold) {
    lcd.print("Soil is Dry!");
    delay(100);
    lcd.clear();
  } else {
    lcd.print("Soil is Moist!");
    delay(100);
    lcd.clear();
  }
}



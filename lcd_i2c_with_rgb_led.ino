#include <LiquidCrystal_I2C.h>

const int redPin = 9, greenPin = 10, bluePin = 11;
LiquidCrystal_I2C lcd(39, 16, 2);

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.clear();
  setColor(255, 0, 0, "Red");
  delay(1000);
  lcd.clear();
  setColor(255, 128, 0, "Orange");
  delay(1000);
  lcd.clear();
  setColor(255, 255, 0, "Yellow");
  delay(1000);
  lcd.clear();
  setColor(0, 255, 0, "Green");
  delay(1000);
  lcd.clear();
  setColor(0, 0, 255, "Blue");
  delay(1000);
  lcd.clear();
  setColor(75, 0, 130, "Indigo");
  delay(1000);
  lcd.clear();
  setColor(127, 0, 255, "Violet");
  delay(1000);
}

void setColor(int red, int green, int blue, String statement) {
  analogWrite(redPin, 255-red);
  analogWrite(greenPin, 255-green);
  analogWrite(bluePin, 255-blue);
  lcd.print(statement);

}

#include<LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(39, 16, 2);

void setup() {
  // put your setup code here, to run once:
  lcd.init();
  lcd.backlight();
  lcd.clear();
}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("hello pipuls");
  lcd.setCursor(4, 1);
  lcd.print("LCD_I2C here");

}

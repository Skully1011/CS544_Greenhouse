#include <LiquidCrystal_I2C.h> // Library for LCD
LiquidCrystal_I2C lcd(0x27, 20, 4); // I2C address 0x27, 16 column and 2 rows



void setup() {
  // put your setup code here, to run once:
  lcd.init(); //initialize the lcd
  lcd.backlight(); //open the backlight 
  lcd.setCursor(0, 0);
  lcd.print("Hello World!");

}

void loop() {
  // put your main code here, to run repeatedly:

}

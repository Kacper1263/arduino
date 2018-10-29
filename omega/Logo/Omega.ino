#include <LiquidCrystal.h>

byte omega[8]={
  B01110,
  B10001,
  B10001,
  B10001,
  B01010,
  B01010,
  B11011,
  B00000
};

LiquidCrystal lcd(7,6,5,4,3,2);

void setup() {
  lcd.begin(16,2);
  
  lcd.createChar(1, omega);

  lcd.setCursor(0,0);
  lcd.write(1);
  lcd.print(" Omega Serwis");
}

void loop() {

}

#include <Servo.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(7,6,5,4,3,2);

Servo servo;

int pos = 0;
int timer = 11;

void setup() {
  servo.attach(9);
  servo.write(140);
  lcd.print("Odliczanie: ");
}

void loop() {
  if (timer == 0){
  servo.write(118);
  delay(500);
  servo.write(140);
  timer = 11;
  delay(10);
  }
  delay(1000);
  timer = timer - 1;
  lcd.setCursor(12,0);
  lcd.print(timer);
  if (timer < 10){
    lcd.setCursor(13,0);
    lcd.print(" ");
  }
  

}

#include <Servo.h>

#include <Wire.h> 
#include <LiquidCrystal.h>

LiquidCrystal lcd(7,6,5,4,3,2);     // inicjacja LCD

Servo servo;                        // inicjacja serwa

int pos = 0;                        // zmienne
int timer = 11;
int czestotliwosc = 11;
int pWart = 10;

void setup() {
  lcd.begin(16, 2);
  servo.attach(9);                  // pin z serwem
  servo.write(140);                 // poczatkowa wartosc
  lcd.setCursor(0,0);
  lcd.print("Odliczanie: ");
}

void loop() {
  pWart = analogRead(A0);           // odczyt z potencjometru
  pWart = map(pWart, 0, 1023, 1, 15);
  czestotliwosc = pWart;

  lcd.setCursor(0,1);                 // wypisanie czestotliwosci na LCD
  lcd.print("Czestotli.: ");
  lcd.setCursor(12,1);
  lcd.print(czestotliwosc);
  if (czestotliwosc < 10){
    lcd.setCursor(13,1);
    lcd.print(" ");
  }
  
  if (timer == 0){                     // kiedy odliczanie: 0 uruchom serwo
  servo.write(118);
  delay(500);
  servo.write(140);
  timer = czestotliwosc;               // nastepny czas == czestotliwosci (od 1 do 15 sec)
  delay(10);
  }
  delay(1000);
  timer = timer - 1;                   // odejmowanie czasu i pisanie go na ekran
  lcd.setCursor(12,0);
  lcd.print(timer);
  if (timer < 10){
    lcd.setCursor(13,0);
    lcd.print(" ");
  }
  

}

#include <Servo.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(7,6,5,4,3,2);     // inicjacja LCD

Servo servo;                        // inicjacja serwa

int pos = 0;                        // zmienne
int timer = 11;
int czestotliwosc = 11;
int pWart = 10;

void setup() {
  lcd.begin(16,2);
  servo.attach(9);                  // pin z serwem
  servo.write(140);                 // poczatkowa wartosc
  lcd.setCursor(0,0);
  lcd.print("Odliczanie: ");
}

void loop() {
  pWart = analogRead(A0);           // odczyt z potencjometru
  
  if (pWart < 64){                  // ustawienie skali od 1 do 15 (sec)
    czestotliwosc = 1;
  }
  if (pWart > 63 && pWart < 128){
    czestotliwosc = 2;
  }
  if (pWart > 127 && pWart < 192){
    czestotliwosc = 3;
  }
  if (pWart > 191 && pWart < 256){
    czestotliwosc = 4;
  }
  if (pWart > 255 && pWart < 320){
    czestotliwosc = 5;
  }
  if (pWart > 319 && pWart < 384){
    czestotliwosc = 5;
  }
  if (pWart > 383 && pWart < 448){
    czestotliwosc = 6;
  }
  if (pWart > 447 && pWart < 512){
    czestotliwosc = 7;
  }
  if (pWart > 511 && pWart < 576){
    czestotliwosc = 8;
  }
  if (pWart > 575 && pWart < 640){
    czestotliwosc = 9;
  }
  if (pWart > 539 && pWart < 704){
    czestotliwosc = 10;
  }
  if (pWart > 703 && pWart < 768){
    czestotliwosc = 11;
  }
  if (pWart > 767 && pWart < 832){
    czestotliwosc = 12;
  }
  if (pWart > 831 && pWart < 896){
    czestotliwosc = 13;
  }
  if (pWart > 895 && pWart < 960){
    czestotliwosc = 14;
  }
  if (pWart > 959){
    czestotliwosc = 15;
  }

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

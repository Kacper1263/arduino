#include <Servo.h>

#include <Wire.h> 
#include <LiquidCrystal.h>

LiquidCrystal lcd(7,6,5,4,3,2);     // inicjacja LCD

Servo servo;                        // inicjacja serwa

int pos = 0;                        // zmienne
int timer = 11;
int dTimer = 0;
int czestotliwosc = 11;
int pWart = 10;
int menu = 0;
int servoStart = 140;
int servoDown = 118;

void setup() {
  pinMode(13, INPUT_PULLUP);
  lcd.begin(16, 2);
  servo.attach(9);                  // pin z serwem
  servo.write(servoStart);                 // poczatkowa wartosc
  lcd.setCursor(0,0);
  lcd.print("Odliczanie: ");
}

void loop() {
  if (digitalRead(13) == LOW){
    menu = menu + 1;
    if (menu == 3){
      menu = 0;
    }
    delay(100);
  }
  
  if (menu == 0){
    pWart = analogRead(A0);           // odczyt z potencjometru
    pWart = map(pWart, 0, 1023, 1, 15);
    czestotliwosc = pWart;
  }
  if (menu == 0){
    lcd.setCursor(0,1);                 // wypisanie czestotliwosci na LCD
    lcd.print("Czestotli.: ");
    lcd.setCursor(12,1);
    lcd.print(czestotliwosc);
    if (czestotliwosc < 10){
      lcd.setCursor(13,1);
      lcd.print(" ");
    }
    lcd.print("  ");
  }

  if (menu == 1){                      // menu 1
    pWart = analogRead(A0);
    pWart = map(pWart, 0, 1023, 0, 180);
    servoStart = pWart;
    
    lcd.setCursor(0,1);
    lcd.print("Poz. START: ");
    lcd.setCursor(12,1);
    lcd.print(servoStart);
    lcd.print(" ");
  }

    if (menu == 2){                      // menu 2
    pWart = analogRead(A0);
    pWart = map(pWart, 0, 1023, 0, 180);
    servoDown = pWart;
    
    lcd.setCursor(0,1);
    lcd.print("Poz.  Down: ");
    lcd.setCursor(12,1);
    lcd.print(servoDown);
    lcd.print(" ");
  }
  
  if (timer == 0){                     // kiedy odliczanie: 0 uruchom serwo
  servo.write(servoDown);
  delay(500);
  servo.write(servoStart);
  timer = czestotliwosc;               // nastepny czas == czestotliwosci (od 1 do 15 sec)
  delay(10);
  }
  delay(100);
  dTimer = dTimer + 100;
  
  if (dTimer == 1000){
    timer = timer - 1;                   // odejmowanie czasu i pisanie go na ekran
    lcd.setCursor(12,0);
    lcd.print(timer);
    if (timer < 10){
      lcd.setCursor(13,0);
      lcd.print(" ");
    }
    dTimer = 0;
  }
  

}

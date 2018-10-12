#include <LiquidCrystal.h>

#define bStart 13 //button Start
#define bStop 12  //button Stop

#define green A0  //green LED
#define red1 A1   //1 red LED
#define red2 A2   //2 red LED
#define red3 A3   //3 red LED

boolean stan = 0;
float timerGLed = 0;
float timerRLed = 0;
int godz = 0;
int minuty = 0;
int sec = 0;
int milSec =0 ;

LiquidCrystal lcd(7,6,5,4,3,2);

void setup() {
  pinMode(bStart, INPUT_PULLUP);
  pinMode(bStop, INPUT_PULLUP);

  pinMode(green, OUTPUT);
  pinMode(red1, OUTPUT);
  pinMode(red2, OUTPUT);
  pinMode(red3, OUTPUT);
  
  lcd.begin(16,2);
  lcd.print("Gotowy!");
}

void loop() {
  if (stan == 0){                 // miganie diody green podczas stanu gotowosci
    timerGLed = timerGLed + 1;
    delay(1);
    
    if (timerGLed > 3000 && stan == 0){
      digitalWrite(green, HIGH);
      timerGLed = timerGLed + 1;
      delay(1);
      
      if (timerGLed == 3200){
        digitalWrite(green, LOW);
        timerGLed = 0;
      }
      
    }
  }                               // koniec migania diody green

  if (digitalRead(bStart) == LOW){
    stan = 1;
  }
  
  if (stan == 1){
    lcd.setCursor(0,0);
    lcd.print("Stoper ON:");
    lcd.setCursor(0,1);

    if (godz < 10){
      lcd.print("0");
    }
    lcd.print(godz);
    lcd.print(":");
    if (minuty < 10){
      lcd.print("0");
    }
    lcd.print(minuty);
    lcd.print(":");
    if (sec < 10){
      lcd.print("0");
    }
    lcd.print(sec);
    
    milSec = milSec + 10;

    if (milSec == 620){
      sec = sec + 1;
      milSec = 0;
    }
    if (sec == 60){
      minuty = minuty + 1;
      sec = 0;
      milSec = milSec + 300; // BARDZO WAZNE poprawia opoznienie stopera w kazdej nowej minucie
    }
    if (minuty == 60){
      godz = godz + 1;
      minuty = 0;
    }

    timerRLed = timerRLed + 10;
    delay(10);
  }

  if (digitalRead(bStop) == LOW && stan == 1){
    stan = 0;
    digitalWrite(red1, LOW);
    digitalWrite(red2, LOW);
    digitalWrite(red3, LOW);
    lcd.setCursor(0,0);
    lcd.print("Wynik:    ");
    delay(500);
  }
  if (digitalRead(bStop) == LOW && stan == 0){
    lcd.clear();
    lcd.print("Gotowy!    ");
    godz = 0;
    minuty = 0;
    sec = 0;
    milSec = 0;
  }

  if (milSec > 0 && milSec < 100 && stan == 1){
    digitalWrite(red1, HIGH);
  }
  else {
    digitalWrite(red1, LOW);
  }

  if (milSec > 99 && milSec < 200 && stan == 1){
    digitalWrite(red2, HIGH);
  }
  else{
    digitalWrite(red2, LOW);
  }

  if (milSec > 199 && milSec < 300 && stan == 1){
    digitalWrite(red3, HIGH);
  }
  else{
    digitalWrite(red3, LOW);
  }
}

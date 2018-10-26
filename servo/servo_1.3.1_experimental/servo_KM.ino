#include <Servo.h>

#include <Wire.h> 
#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);     // inicjacja LCD

Servo servo;                        // inicjacja serwa

int pos = 0;                        // zmienne
int timer = 11;
int dTimer = 0;
int czestotliwosc = 11;
int pWart = 10;
int menu = 0;
int servoStart = 140;
int servoDown = 118;

int optionIn;
int power = 1;

int buzzer = 0;

void setup() {
	pinMode(13, INPUT_PULLUP);
	pinMode(12, OUTPUT);
	lcd.begin(16, 2);
	servo.attach(9);                  // pin z serwem
	servo.write(servoStart);                 // poczatkowa wartosc
	lcd.setCursor(0, 0);
	lcd.print("Odliczanie: ");
	Serial.begin(9600);


	Serial.println("");
	Serial.println("Menu:");
	Serial.println("0 - Buzzer off");
	Serial.println("1 - Buzzer on");
	Serial.println("2 - 1st menu page");
	Serial.println("3 - 2nd menu page");
	Serial.println("4 - 3rd menu page");
	Serial.println("5 - Power = 0");
	Serial.println("6 - Power = 1");
	Serial.println("");
}

void loop() {

	if (Serial.available() > 0) {
		optionIn = Serial.readString().toInt();
		
		switch (optionIn) {
		
			case 0:
				{
					Serial.println("Buzzer OFF");
					buzzer = 0;
					break;
				}
			case 1:
				{
					Serial.println("Buzzer ON");
					buzzer = 1;
					break;
				}
			case 2:
				{
					Serial.println("Menu 1st page");
					menu = 0;
					break;
				}
			case 3:
				{
					Serial.println("Menu 2nd page");
					menu = 1;
					break;
				}
			case 4:
				{
					Serial.println("Menu 3rd page");
					menu = 2;
					break;
				}
			case 5:
				{
					Serial.println("Power is now 0");
					power = 0;
					break;
				}
			case 6:
				{
					Serial.println("Power is now 1");
					power = 1;
					break;
				}
		}
	}
	
	if (digitalRead(13) == LOW) {
		menu = menu + 1;
		if (menu == 3) {
			menu = 0;
		}
		delay(100);
	}

	if (menu == 0) {
		pWart = analogRead(A0);           // odczyt z potencjometru
		pWart = map(pWart, 0, 1023, 1, 15);
		czestotliwosc = pWart;
	}
	if (menu == 0) {
		lcd.setCursor(0, 1);                 // wypisanie czestotliwosci na LCD
		lcd.print("Czestotli.: ");
		lcd.setCursor(12, 1);
		lcd.print(czestotliwosc);
		if (czestotliwosc < 10) {
			lcd.setCursor(13, 1);
			lcd.print(" ");
		}
		lcd.print("  ");
	}

	if (menu == 1) {                      // menu 1
		pWart = analogRead(A0);
		pWart = map(pWart, 0, 1023, 0, 180);
		servoStart = pWart;

		lcd.setCursor(0, 1);
		lcd.print("Poz. START: ");
		lcd.setCursor(12, 1);
		lcd.print(servoStart);
		lcd.print(" ");
	}

	if (menu == 2) {                      // menu 2
		pWart = analogRead(A0);
		pWart = map(pWart, 0, 1023, 0, 180);
		servoDown = pWart;

		lcd.setCursor(0, 1);
		lcd.print("Poz.  Down: ");
		lcd.setCursor(12, 1);
		lcd.print(servoDown);
		lcd.print(" ");
	}

if (power == 1) {
		if (timer == 0) {                     // kiedy odliczanie: 0 uruchom serwo
			if (buzzer == 1) {
				digitalWrite(12, HIGH);
				delay(200);
				digitalWrite(12, LOW);
				delay(50);
				digitalWrite(12, HIGH);
				delay(200);
				digitalWrite(12, LOW);
				delay(50);
				digitalWrite(12, HIGH);
				delay(200);
				digitalWrite(12, LOW);
			}
			servo.write(servoDown);
			delay(500);
			servo.write(servoStart);
			timer = czestotliwosc;               // nastepny czas == czestotliwosci (od 1 do 15 sec)
			delay(10);

		}
		delay(100);
		dTimer = dTimer + 100;

		if (dTimer == 1000) {
			timer = timer - 1;                   // odejmowanie czasu i pisanie go na ekran
			lcd.setCursor(12, 0);
			lcd.print(timer);
			if (timer < 10) {
				lcd.setCursor(13, 0);
				lcd.print(" ");
			}
			dTimer = 0;
		}

}
}
// Version 2.0.0

//------------------------------------------------------------------------------------------//
//										Configuration										//
//																							//
// Enable NFC authorization (true or false)		 											//
bool nfcAuthEnable = true;																	//
// After that time, the logout will be logged out											//
unsigned long timeToLogout = 300000;				// in ms. default 5 min	(300 000)		//
// Authorized UID tags																		//
const byte UID[] = { 0x96, 0x5F, 0xD3, 0x13 };		// White card							//
const byte UID2[] = { 0xD5, 0x88, 0x44, 0xC6 };		// m card								//
//																							//
//------------------------------------------------------------------------------------------//
//																							//
// DHT Pins Configuration																	//
int DHTPIN	= 3;									// DHT22 sensor	on Digital PIN (OUT)	//
int DHTPIN2 = 4;									// DHT11 sensor on Digital PIN (IN)		//
//																							//
//------------------------------------------------------------------------------------------//

//------------------------------------------------------------------------------------------//
//										Terminal commands									//
//																							//
// D or d		- to start the DHT Terminal													//
// s			- to stop the DHT Terminal													//
// L or l		- ON/OFF LCD backlight														//
// e			- logout																	//
//																							//
//------------------------------------------------------------------------------------------//

#define CUSTOM_SETTINGS					// 1Sheeld settings
#define INCLUDE_VOICE_RECOGNIZER_SHIELD
#define INCLUDE_TEXT_TO_SPEECH_SHIELD
#define INCLUDE_TERMINAL_SHIELD			// terminal print last command

#include <OneSheeld.h>
#include "DHT.h"
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16 ,2);		// LCD

#define DHTTYPE DHT22					// DHT sensors
#define DHTTYPE2 DHT11
DHT dht(DHTPIN, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE2);

MFRC522 rfid(10, 9);					// RFID
MFRC522::MIFARE_Key key;

const char onCommand[] = "led on";
const char statusCommand[] = "status";
const char offCommand[] = "led off";
const char helpCommand[] = "help";
const char unoCommand[] = "arduino";
const char stateCommand[] = "state";
const char focusCommand[] = "focus";
const char focusOffCommand[] = "focus off";
const char focusNameCommand[] = "focus name";
const char focusNameOffCommand[] = "focus name off";
const char pinonCommand[] = "power on";
const char pinoffCommand[] = "power off";
const char dhtSensorCommand[] = "dht read";
const char dhtTerminalOnCommand[] = "dht terminal on";
const char dhtTerminalOn2Command[] = "dhd terminal on";
const char dhtTerminalOffCommand[] = "dht terminal off";
const char dhtTerminalOff2Command[] = "dhd terminal off";
const char logoutCommand[] = "logout";
const char logoutCommand2[] = "log out";

char error = 0;
char lamp = 13;
int buzzer = 2;

char DHTTerminal = 0;
char focusNameOn = 0;
String focusName = "Focus pin name";

bool focusOn = 0;
char pinFocus = 0;
char enableFocus = 0;
String pin;
char pinNb;
char dhtLcdStart = 0;
char termRead[10];
bool backlightLCD = 1;
bool isLoged = 0;
unsigned long loginTime = 0;
char menu = 0;

byte degree[8] =
{										// custom char degree
	B00111,
	B00101,
	B00111,
	B00000,
	B00000,
	B00000,
	B00000,
	B00000
};

byte locked[8] = {
  B01110,
  B10001,
  B10001,
  B10001,
  B11111,
  B11111,
  B11111,
  B11111
};
void setup() {
	OneSheeld.begin();
	
	lcd.init();
	lcd.backlight();
	lcd.setCursor(0, 0);
	lcd.print(F("TTS project by"));
	delay(2000);
	lcd.setCursor(0, 1);
	lcd.print(F("Kacper M"));
	delay(2000);
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(F("Ready!"));

	pinMode(lamp, OUTPUT);
	pinMode(8, OUTPUT);
	pinMode(A0, INPUT_PULLUP);
	pinMode(A1, INPUT_PULLUP);
	pinMode(buzzer, OUTPUT);

	dht.begin();
	dht2.begin();

	SPI.begin();
	rfid.PCD_Init();

	if (nfcAuthEnable == 0) {
		isLoged = 1;
	}
}

void loop()
{
	if (digitalRead(A0) == LOW) {
		menu++;
		if (menu > 4) {
			menu = 1;
		}
		delay(200);
	}
	writeMenu();

char lastTermRead;			// Save last Terminal char
	if (nfcAuthEnable == 1 && isLoged == 0 && menu == 0) {
		lastTermRead = Terminal.read();
		if (DHTTerminal != 1) {
			lcd.setCursor(0,0);
			lcd.print("Use your NFC tag");
			lcd.setCursor(0, 1);
			lcd.print("Press 1 for menu");
		}

		if (VoiceRecognition.isNewCommandReceived()) {
			VoiceRecognition.getLastCommand();
			TextToSpeech.say(F("To get acces, use your NFC tag"));
			delay(3000);
		}
		else if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
			if (rfid.uid.uidByte[0] == UID[0] &&
				rfid.uid.uidByte[1] == UID[1] &&
				rfid.uid.uidByte[2] == UID[2] &&
				rfid.uid.uidByte[3] == UID[3] ||
				rfid.uid.uidByte[0] == UID2[0] &&
				rfid.uid.uidByte[1] == UID2[1] &&
				rfid.uid.uidByte[2] == UID2[2] &&
				rfid.uid.uidByte[3] == UID2[3]
				)
			{
				digitalWrite(buzzer, HIGH);
				delay(50);
				digitalWrite(buzzer, LOW);
				TextToSpeech.say(F("Access granted"));
				isLoged = 1;
				loginTime = millis();
				lcd.clear();
				lcd.print("Access granted!");
				delay(3000);
			}
			else {
				digitalWrite(buzzer, HIGH);
				delay(100);
				digitalWrite(buzzer, LOW);
				delay(50);
				digitalWrite(buzzer, HIGH);
				delay(100);
				digitalWrite(buzzer, LOW);
				delay(50);
				digitalWrite(buzzer, HIGH);
				delay(100);
				digitalWrite(buzzer, LOW);

				TextToSpeech.say(F("Access denied"));
				isLoged = 0;
				lcd.clear();
				lcd.print("Access denied!");
				delay(3000);
			}
				rfid.PICC_HaltA();
				rfid.PCD_StopCrypto1();
		}
		else if (lastTermRead == 'L' || lastTermRead == 'l') {				// LCD backlight ON/OFF
			backlightLCD = !backlightLCD;
			if (backlightLCD == 1) lcd.backlight();
			else if (backlightLCD == 0) lcd.noBacklight();
		}
	}
	if (isLoged == 1) {
		
		if (dhtLcdStart == 0 && menu == 0) {
			lcd.setCursor(0, 0);
			lcd.print("Ready!          ");
			lcd.setCursor(0, 1);
			lcd.print("Press 1 for menu");
		}
		if (focusOn == 1) {																					// check focus
			focus();
		}
		if (VoiceRecognition.isNewCommandReceived() || Terminal.available()) {
			lastTermRead = Terminal.read();

			if (!strcmp(onCommand, VoiceRecognition.getLastCommand()) && digitalRead(lamp) == LOW) {			// LED on
				digitalWrite(lamp, HIGH);
				TextToSpeech.say(F("The LED is now ON."));
				delay(4000);
			}
			else if (!strcmp(onCommand, VoiceRecognition.getLastCommand()) && digitalRead(lamp) == HIGH) {	//	LED still on
				TextToSpeech.say(F("The LED is still ON."));
				delay(4000);
			}
			else if (!strcmp(offCommand, VoiceRecognition.getLastCommand()) && digitalRead(lamp) == HIGH) {	// LED off
				digitalWrite(lamp, LOW);
				TextToSpeech.say(F("The LED is now OFF."));
				delay(4000);
			}
			else if (!strcmp(offCommand, VoiceRecognition.getLastCommand()) && digitalRead(lamp) == LOW) {	// LED still off
				TextToSpeech.say(F("The LED is still OFF."));
				delay(4000);
			}
			else if (!strcmp(stateCommand, VoiceRecognition.getLastCommand())) {		// pin status function (state)
				TextToSpeech.say(F("Please tell me the digital pin number"));
				delay(2000);
				while (!VoiceRecognition.isNewCommandReceived())
				{
					Terminal.println();												// without this "while" will not work ¯\_(ツ)_/¯
				}
				pin = VoiceRecognition.getLastCommand();

				pinToInt();

				delay(1000);
				if (digitalRead(pinNb) == LOW && error == 0) TextToSpeech.say(F("The state is currently OFF."));
				else if (digitalRead(pinNb) == HIGH && error == 0) TextToSpeech.say(F("The state is currently ON."));
				delay(4000);
				error = 0;
			}
			else if (!strcmp(helpCommand, VoiceRecognition.getLastCommand())) {		// help function
				help();
			}
			else if (!strcmp(statusCommand, VoiceRecognition.getLastCommand())) {		// status function
				TextToSpeech.say(F("1Sheeld is redy to use!"));
			}
			else if (!strcmp(pinonCommand, VoiceRecognition.getLastCommand())) {		// pinON function. Must be in loop()
				TextToSpeech.say(F("Tell me the digital pin number"));
				delay(2000);

				while (!VoiceRecognition.isNewCommandReceived())
				{
					Terminal.println();
				}
				pin = VoiceRecognition.getLastCommand();

				pinToInt();

				delay(1000);
				if (error == 0)digitalWrite(pinNb, HIGH);
				if (error == 0)TextToSpeech.say(F("Done"));
				delay(3000);
				error = 0;
			}
			else if (!strcmp(pinoffCommand, VoiceRecognition.getLastCommand())) {   // pinOFF function. Must be in loop()
				TextToSpeech.say(F("Tell me the digital pin number"));
				delay(2000);

				while (!VoiceRecognition.isNewCommandReceived())
				{
					Terminal.println();
				}
				pin = VoiceRecognition.getLastCommand();

				pinToInt();

				delay(1000);
				if (error == 0)digitalWrite(pinNb, LOW);
				if (error == 0)TextToSpeech.say(F("Done"));
				delay(3000);
				error = 0;

			}
			else if (!strcmp(unoCommand, VoiceRecognition.getLastCommand())) {				// UNO
				TextToSpeech.say(F("UNO!"));
				delay(1500);
			}
			else if (!strcmp(focusCommand, VoiceRecognition.getLastCommand())) {				// focus function
				focusOn = 1;
				TextToSpeech.say(F("Please tell me a digital pin number to focus"));
				delay(3500);

				while (!VoiceRecognition.isNewCommandReceived()) {
					Terminal.println();
				}
				pin = VoiceRecognition.getLastCommand();

				pinToInt();

				delay(1000);
				if (error == 0) {
					enableFocus = 1;
					pinFocus = pinNb;
					TextToSpeech.say(F("Focus has been activated"));
				}
				else if (error == 1) {
					focusOn = 0;
					enableFocus = 0;
					error = 0;
				}
			}
			else if (!strcmp(focusOffCommand, VoiceRecognition.getLastCommand())) {				// focus off function
				focusOff();
			}
			else if (!strcmp(focusNameCommand, VoiceRecognition.getLastCommand())) {				// focus name function
				focusNameOn = 1;
				TextToSpeech.say(F("Please tell me a new focus name"));
				delay(3500);

				while (!VoiceRecognition.isNewCommandReceived()) {
					Terminal.println();
				}
				focusName = VoiceRecognition.getLastCommand();
				TextToSpeech.say(F("The new focus name has been added"));
				delay(3000);

			}
			else if (!strcmp(focusNameOffCommand, VoiceRecognition.getLastCommand())) {				// focus name off function
				focusNameOn = 0;
				TextToSpeech.say(F("The focus name has been deactivated"));
				delay(3000);
			}
			else if (!strcmp(dhtSensorCommand, VoiceRecognition.getLastCommand())) {					// DHT read sensor value
				TextToSpeech.say(F("Please tell me the DHT OUT pin number"));
				delay(3000);

				while (!VoiceRecognition.isNewCommandReceived()) {										// wait for pin number
					Terminal.println();
				}

				pin = VoiceRecognition.getLastCommand();

				pinToInt();

				if (error == 0) {
					dhtReadVal();
				}
				delay(2000);
				error = 0;
			}
			else if (!strcmp(dhtTerminalOnCommand, VoiceRecognition.getLastCommand()) || !strcmp(dhtTerminalOn2Command, VoiceRecognition.getLastCommand()) || lastTermRead == 'd' || lastTermRead == 'D') {				// DHT write in terminal ON
				/*TextToSpeech.say(F("Please tell me the DHT OUT pin number"));
				delay(3000);

				while (!VoiceRecognition.isNewCommandReceived()) {										// wait for pin number
					Terminal.println();
				}

				pin = VoiceRecognition.getLastCommand();

				pinToInt();
				*/
			dhtTerminalOn:
				if (error == 0) {
					DHTTerminal = 1;
					dhtLcdStart = 1;
					//DHTPIN = pinNb;
					TextToSpeech.say(F("DHT terminal has been activated"));
				}
				delay(3000);
				error = 0;
			}
			else if (!strcmp(dhtTerminalOffCommand, VoiceRecognition.getLastCommand()) || !strcmp(dhtTerminalOff2Command, VoiceRecognition.getLastCommand()) || lastTermRead == 's') {				// DHT write in terminal OFF
				DHTTerminal = 0;
				dhtLcdStart = 0;
				TextToSpeech.say(F("DHT terminal has been deactivated"));
				delay(3000);
			}
			else if (lastTermRead == 'L' || lastTermRead == 'l') {				// LCD backlight ON/OFF
				backlightLCD = !backlightLCD;
				if (backlightLCD == 1) lcd.backlight();
				else if (backlightLCD == 0) lcd.noBacklight();
			}
			else if (!strcmp(logoutCommand, VoiceRecognition.getLastCommand()) || !strcmp(logoutCommand2, VoiceRecognition.getLastCommand()) || lastTermRead == 'e') {				// logout
				isLoged = 0;
				TextToSpeech.say(F("You have been logged out"));
				lcd.clear();
				lcd.print("Logged out!");
				delay(4000);
			}
			else {																						// command not found
				TextToSpeech.say(F("I don't understand. Try the help command."));
				delay(5000);
			}
			String last;									// terminal print last command
			last = VoiceRecognition.getLastCommand();		// terminal print last command
			Terminal.println(last);							// terminal print last command
		}

		
		// VoiceRecognition.start(); Auto ON
	theEnd:
		if (millis() < 0) {
			Terminal.print("");
		}
	}

	if (millis() - loginTime >= timeToLogout && isLoged == 1 && nfcAuthEnable == 1)				// auto logout
	{
		isLoged = 0;
		TextToSpeech.say(F("You have been logged out"));
		lcd.clear();
		lcd.print("Logged out!");
		delay(4000);
	}

	if (DHTTerminal == 1) {
		dhtTerminalWrite();
	}
}

void help()
{
	TextToSpeech.say(F("To see a list of available commands, switch the 1Sheeld application window to Text To Speech after running the help command"));
	delay(7000);
	TextToSpeech.say(F("Here is a list of available commands:"));
	delay(3500);
	TextToSpeech.say(F("Power ON,"));
	delay(2500);
	TextToSpeech.say(F("Power OFF,"));
	delay(2500);
	TextToSpeech.say(F("DHT read,"));
	delay(2500);
	TextToSpeech.say(F("DHT terminal ON,"));
	delay(2500);
	TextToSpeech.say(F("DHT terminal OFF,"));
	delay(2500);
	TextToSpeech.say(F("LED ON,"));
	delay(2500);
	TextToSpeech.say(F("LED OFF,"));
	delay(2500);
	TextToSpeech.say(F("Focus,"));
	delay(2500);
	TextToSpeech.say(F("Focus OFF,"));
	delay(2500);
	TextToSpeech.say(F("Focus name,"));
	delay(2500);
	TextToSpeech.say(F("Focus name OFF,"));
	delay(2500);
	TextToSpeech.say(F("State,"));
	delay(2500);
	TextToSpeech.say(F("Status."));
	delay(2500);
	//TextToSpeech.say("Help.");
	//delay(2500);

}

void writeMenu() 
{
	if (menu == 0 && isLoged == 1 && dhtLcdStart == 0) {
		lcd.setCursor(0, 0);
		lcd.print(F("Ready!          "));
		lcd.setCursor(0, 1);
		lcd.print(F("Press 1 for menu"));
	}
	else if (isLoged == 0 && menu == 0 && dhtLcdStart == 0) {
		lcd.setCursor(0, 0);
		lcd.print(F("Use your NFC tag"));
		lcd.setCursor(0, 1);
		lcd.print(F("Press 1 for menu"));
	}
	else if (menu == 1) {
		lcd.setCursor(0, 0);
		lcd.print(F("Backlight ON/OFF"));
		lcd.setCursor(0, 1);
		lcd.print(F("1/4             "));
		if (digitalRead(A1) == LOW) {
			backlightLCD = !backlightLCD;
			if (backlightLCD == 1) lcd.backlight();
			else if (backlightLCD == 0) lcd.noBacklight();
			delay(300);
		}
	}
	else if (menu == 2) {
		lcd.setCursor(0, 0);
		lcd.print(F("DHT Terminal ON "));
		lcd.setCursor(0, 1);
		lcd.print(F("2/4             "));
		if (digitalRead(A1) == LOW) {
			DHTTerminal = 1;
			dhtLcdStart = 1;
			TextToSpeech.say(F("DHT terminal has been activated"));
			delay(3000);
			menu = 0;
		}
	}
	else if (menu == 3) {
		lcd.setCursor(0, 0);
		lcd.print(F("Logout          "));
		lcd.setCursor(0, 1);
		lcd.print(F("3/4             "));
		if (digitalRead(A1) == LOW && isLoged == 1) {
			isLoged = 0;
			TextToSpeech.say(F("You have been logged out"));
			lcd.clear();
			lcd.print(F("Logged out!"));
			delay(4000);
			menu = 0;
		}
	}
	else if (menu == 4) {
		lcd.setCursor(0, 0);
		lcd.print(F("Exit from menu  "));
		lcd.setCursor(0, 1);
		lcd.print(F("4/4             "));
		if (digitalRead(A1) == LOW) {
			menu = 0;
		}
	}
}

void focus()																// if focus function is ON --> check pin condition
{
	if (enableFocus == 1 && digitalRead(pinFocus) == HIGH)
	{
		TextToSpeech.say(F("High condition on the pin: "));
		delay(2000);
		if (focusNameOn == 0) {
			TextToSpeech.say(String(pinFocus));
		}
		else if (focusNameOn == 1) {
			TextToSpeech.say(focusName);
		}
		delay(4000);
	}
}

void focusOff()
{
	focusOn = 0;
	enableFocus = 0;
	TextToSpeech.say(F("Focus has been deactivated"));
	delay(3500);
}

void dhtReadVal()
{
	DHTPIN = pinNb;					// set new sensor pin
	DHT dht(DHTPIN, DHTTYPE);
	dht.begin();
	delay(200);

	int h = dht.readHumidity();		
	int t = dht.readTemperature() - 1;	// -1 only for my sensor!!!

	String hStr = String(h);		// Convert float to string for TTS
	String tStr = String(t);		// Convert float to string for TTS

	TextToSpeech.say(F("The temperature is "));
	delay(1500);
	TextToSpeech.say(tStr);
	TextToSpeech.say(F(" degrees"));
	delay(3000);
	TextToSpeech.say(F(" and humidity is"));
	delay(2000);
	TextToSpeech.say(hStr);
	TextToSpeech.say(F(" percent"));
	delay(5000);
	
}

unsigned long lastMillisDHT = 0;					// Delay timer
unsigned long lastMillisInOut = 0;					// Delay timer
int stateDhtLcd = 0;
void dhtTerminalWrite()								// DHT write values to terminal if enabled
{		
	unsigned long currentMillis = millis();
	unsigned long currentMillisInOut = millis();
	int state = 0;
	
	if (currentMillis - lastMillisDHT >= 1500) {	// Delay timer
		lastMillisDHT = currentMillis;				// Delay timer

		DHT dht(DHTPIN, DHTTYPE);					// set new sensor pin
		DHT dht2(DHTPIN2, DHTTYPE2);				// set new sensor pin
		dht.begin();
		dht2.begin();
			float h = dht.readHumidity();
			float t = dht.readTemperature() - 1;		// -1 only for my sensor!!!
			int h2 = dht2.readHumidity();
			int t2 = dht2.readTemperature() - 1;

			String hStr = String(h);					// Convert float to string for TTS
			String tStr = String(t);					// Convert float to string for TTS
			String hStr2 = String(h2);					// Convert float to string for TTS
			String tStr2 = String(t2);					// Convert float to string for TTS

			Terminal.println("Temperature 1: " + tStr + "C and humidity 1: " + hStr + "%");
			delay(200);
			Terminal.println("Temperature 2: " + tStr2 + "C and humidity 2: " + hStr2 + "%");

			lcd.createChar(1, locked);
			if (currentMillisInOut - lastMillisInOut >= 6000) {		// Change LCD temp IN/OUT
				lastMillisInOut = currentMillisInOut;
				stateDhtLcd = !stateDhtLcd;
			}
			if (stateDhtLcd == 0 && menu == 0) {
				lcd.createChar(0, degree);
				lcd.setCursor(0, 0);
				lcd.print("Temp: OUT Humi: ");
				lcd.setCursor(0, 1);
				lcd.print(tStr);
				lcd.write(0);
				lcd.print("C ");
				lcd.setCursor(8, 1);
				lcd.print("  ");
				lcd.setCursor(10, 1);
				lcd.print(hStr);
				lcd.print("%      ");
				if (isLoged == 0) {
					lcd.setCursor(7, 1);
					lcd.write(1);
				}
			}
			if (stateDhtLcd == 1 && menu == 0) {
				lcd.createChar(0, degree);
				lcd.setCursor(0, 0);
				lcd.print("Temp: IN  Humi: ");
				lcd.setCursor(0, 1);
				lcd.print(tStr2);
				lcd.write(0);
				lcd.print("C   ");
				lcd.setCursor(8, 1);
				lcd.print("  ");
				lcd.setCursor(10, 1);
				lcd.print(hStr2);
				lcd.print("%    ");
				if (isLoged == 0) {
					lcd.setCursor(7, 1);
					lcd.write(1);
				}

			}
	}
}

void pinToInt()										//convert e.g. "five" to "5"
{
	if (pin == F("zero")) {
		pinNb = 0;
		TextToSpeech.say(F("OK"));
	}
	else if (pin == F("one")) {
		pinNb = 1;
		TextToSpeech.say(F("OK"));
	}
	else if (pin == F("two") || pin == F("to")) {
		pinNb = 2;
		TextToSpeech.say(F("OK"));
	}
	else if (pin == F("three") || pin == F("free")) {
		pinNb = 3;
		TextToSpeech.say(F("OK"));
	}
	else if (pin == F("four") || pin == F("for") || pin == F("far")) {
		pinNb = 4;
		TextToSpeech.say(F("OK"));
	}
	else if (pin == F("five")) {
		pinNb = 5;
		TextToSpeech.say(F("OK"));
	}
	else if (pin == F("six")) {
		pinNb = 6;
		TextToSpeech.say(F("OK"));
	}
	else if (pin == F("seven")) {
		pinNb = 7;
		TextToSpeech.say(F("OK"));
	}
	else if (pin == F("eight")) {
		pinNb = 8;
		TextToSpeech.say(F("OK"));
	}
	else if (pin == F("nine")) {
		pinNb = 9;
		TextToSpeech.say(F("OK"));
	}
	else if (pin == F("ten") || pin == F("10")) {
		pinNb = 10;
		TextToSpeech.say(F("OK"));
	}
	else if (pin == F("eleven") || pin == F("11")) {
		pinNb = 11;
		TextToSpeech.say(F("OK"));
	}
	else if (pin == F("twelve") || pin == F("12")) {
		pinNb = 12;
		TextToSpeech.say(F("OK"));
	}
	else if (pin == F("thirteen") || pin == F("13")) {
		pinNb = 13;
		TextToSpeech.say(F("OK"));
	}
	else {
		TextToSpeech.say(F("error"));
		error = 1;
	}
}
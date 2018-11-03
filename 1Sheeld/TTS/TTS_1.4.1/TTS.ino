#define CUSTOM_SETTINGS
#define INCLUDE_VOICE_RECOGNIZER_SHIELD
#define INCLUDE_TEXT_TO_SPEECH_SHIELD

#define INCLUDE_TERMINAL_SHIELD    // terminal print last command

#include <OneSheeld.h>



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
int state = 1;
int error = 0;
int lamp = 13;

int focusNameOn = 0;
String focusName = "Focus pin name";

int focusOn = 0;
int pinFocus = 0;
int enableFocus = 0;
String pin;
int pinNb;
void setup() {
	OneSheeld.begin();

	pinMode(lamp, OUTPUT);
	pinMode(8, OUTPUT);
}

void loop()
{
	if (state == 1) {

		if (focusOn == 1) {																					// check focus
			focus();
		}
		if (VoiceRecognition.isNewCommandReceived()) {

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
				state = 6;
			stateRead:
				if (state == 6) {
					TextToSpeech.say(F("Please tell me the digital pin number"));
					state = 7;
					delay(2000);
				}
				if (VoiceRecognition.isNewCommandReceived()) {
					pin = VoiceRecognition.getLastCommand();

					pinToInt();

					delay(1000);
					if (digitalRead(pinNb) == LOW && error == 0) TextToSpeech.say(F("The state is currently OFF."));
					else if (digitalRead(pinNb) == HIGH && error == 0) TextToSpeech.say(F("The state is currently ON."));
					delay(4000);
					error = 0;
					state = 1;
				}

			}
			else if (!strcmp(helpCommand, VoiceRecognition.getLastCommand())) {		// help function
				help();
			}
			else if (!strcmp(statusCommand, VoiceRecognition.getLastCommand())) {		// status function
				TextToSpeech.say(F("1Sheeld is redy to use!"));
			}
			else if (!strcmp(pinonCommand, VoiceRecognition.getLastCommand())) {		// pinON function. Must be in loop()
				state = 2;
			poweron:
				if (state == 2) {
					TextToSpeech.say(F("Tell me the digital pin number"));
					state = 3;
					delay(2000);
				}
				if (VoiceRecognition.isNewCommandReceived())
				{
					pin = VoiceRecognition.getLastCommand();

					pinToInt();

					delay(1000);
					if (error == 0)digitalWrite(pinNb, HIGH);
					if (error == 0)TextToSpeech.say(F("Done"));
					delay(3000);
					error = 0;
					state = 1;
				}
			}
			else if (!strcmp(pinoffCommand, VoiceRecognition.getLastCommand())) {   // pinOFF function. Must be in loop()
				state = 4;
			poweroff:
				if (state == 4) {
					TextToSpeech.say(F("Tell me the digital pin number"));
					state = 5;
					delay(2000);
				}
				if (VoiceRecognition.isNewCommandReceived())
				{
					pin = VoiceRecognition.getLastCommand();

					pinToInt();

					delay(1000);
					if (error == 0)digitalWrite(pinNb, LOW);
					if (error == 0)TextToSpeech.say(F("Done"));
					delay(3000);
					error = 0;
					state = 1;
				}
			}
			else if (!strcmp(unoCommand, VoiceRecognition.getLastCommand())) {				// UNO
				TextToSpeech.say(F("UNO!"));
				delay(1500);
			}
			else if (!strcmp(focusCommand, VoiceRecognition.getLastCommand())) {				// focus function
				state = 8;
				focusOn = 1;
			focus:
				if (state == 8) {
					TextToSpeech.say(F("Please tell me a digital pin number to focus"));
					state = 9;
					delay(3500);
				}
				if (VoiceRecognition.isNewCommandReceived()) {
					pin = VoiceRecognition.getLastCommand();

					pinToInt();

					delay(1000);
					if (error == 0) {
						enableFocus = 1;
						pinFocus = pinNb;
						TextToSpeech.say(F("Focus has been activated"));
						state = 1;
					}
					else if (error == 1) {
						focusOn = 0;
						enableFocus = 0;
						state = 1;
						error = 0;
					}
				}


			}
			else if (!strcmp(focusOffCommand, VoiceRecognition.getLastCommand())) {				// focus off function
				focusOff();
			}
			else if (!strcmp(focusNameCommand, VoiceRecognition.getLastCommand())) {				// focus name function
				focusNameOn = 1;
				state = 10;
			focusName:
				if (state == 10) {
					TextToSpeech.say(F("Please tell me a new focus name"));
					state = 11;
					delay(3500);
				}
				if (VoiceRecognition.isNewCommandReceived()) {
					focusName = VoiceRecognition.getLastCommand();
					TextToSpeech.say(F("The new focus name has been added"));
					delay(3000);
					state = 1;
				}
			}
			else if (!strcmp(focusNameOffCommand, VoiceRecognition.getLastCommand())) {				// focus name off function
				focusNameOn = 0;
				TextToSpeech.say(F("The focus name has been deactivated"));
				delay(3000);
			}
			else {
				TextToSpeech.say(F("I don't understand. Try the help command."));
				delay(5000);
			}
			String last;									// terminal print last command
			last = VoiceRecognition.getLastCommand();		// terminal print last command
			Terminal.println(last);						// terminal print last command
		}
		// VoiceRecognition.start(); Auto ON
	}
	if (state == 2 || state == 3) {					// state power on
		goto poweron;
	}
	if (state == 4 || state == 5) {					// state power off
		goto poweroff;
	}
	if (state == 6 || state == 7) {					// state "state"
		goto stateRead;
	}
	if (state == 8 || state == 9) {					// state "focus"
		goto focus;
	}
	if (state == 10 || state == 11) {					// state "focusName"
		goto focusName;
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

void pinToInt()									//convert e.g. "five" to "5"
{
	if (pin == "one") {
		pinNb = 1;
		TextToSpeech.say(F("OK"));
	}
	else if (pin == "two" || pin == "to") {
		pinNb = 2;
		TextToSpeech.say(F("OK"));
	}
	else if (pin == "three" || pin == "free") {
		pinNb = 3;
		TextToSpeech.say(F("OK"));
	}
	else if (pin == "four" || pin == "for" || pin == "far") {
		pinNb = 4;
		TextToSpeech.say(F("OK"));
	}
	else if (pin == "five") {
		pinNb = 5;
		TextToSpeech.say(F("OK"));
	}
	else if (pin == "six") {
		pinNb = 6;
		TextToSpeech.say(F("OK"));
	}
	else if (pin == "seven") {
		pinNb = 7;
		TextToSpeech.say(F("OK"));
	}
	else if (pin == "eight") {
		pinNb = 8;
		TextToSpeech.say(F("OK"));
	}
	else if (pin == "nine") {
		pinNb = 9;
		TextToSpeech.say(F("OK"));
	}
	else if (pin == "ten") {
		pinNb = 10;
		TextToSpeech.say(F("OK"));
	}
	else if (pin == "eleven" || pin == "11") {
		pinNb = 11;
		TextToSpeech.say(F("OK"));
	}
	else if (pin == "twelve" || pin == "12") {
		pinNb = 12; 
		TextToSpeech.say(F("OK"));
	}
	else if (pin == "thirteen" || pin == "13") {
		pinNb = 13;
		TextToSpeech.say(F("OK"));
	}
	else {
		TextToSpeech.say(F("error"));
		error = 1;
	}
}
#define CUSTOM_SETTINGS
#define INCLUDE_VOICE_RECOGNIZER_SHIELD
#define INCLUDE_TEXT_TO_SPEECH_SHIELD

#define INCLUDE_TERMINAL_SHIELD		// terminal print last command

#include <OneSheeld.h>



const char onCommand[] = "power on";
const char offCommand[] = "power off";
const char helpCommand[] = "help";
const char unoCommand[] = "arduino";
const char stateCommand[] = "state";
int state = 0;

int lamp = 13;

void setup() {
	OneSheeld.begin();

	pinMode(lamp, OUTPUT);
}

void loop()
{
	if (state == 1) {

		if (VoiceRecognition.isNewCommandReceived()) {
			state = 0;
			if (!strcmp(onCommand, VoiceRecognition.getLastCommand()) && digitalRead(lamp) == LOW) {
				digitalWrite(lamp, HIGH);
				TextToSpeech.say("The LED is now ON.");
				delay(4000);
			}
			else if (!strcmp(onCommand, VoiceRecognition.getLastCommand()) && digitalRead(lamp) == HIGH) {
				TextToSpeech.say("The LED is still ON.");
				delay(4000);
			}
			else if (!strcmp(offCommand, VoiceRecognition.getLastCommand()) && digitalRead(lamp) == HIGH) {
				digitalWrite(lamp, LOW);
				TextToSpeech.say("The LED is now OFF.");
				delay(4000);
			}
			else if (!strcmp(offCommand, VoiceRecognition.getLastCommand()) && digitalRead(lamp) == LOW) {
				TextToSpeech.say("The LED is still OFF.");
				delay(4000);
			}
			else if (!strcmp(stateCommand, VoiceRecognition.getLastCommand()) && digitalRead(lamp) == HIGH) {
				TextToSpeech.say("The LED is currently ON.");
				delay(4000);
			}
			else if (!strcmp(stateCommand, VoiceRecognition.getLastCommand()) && digitalRead(lamp) == LOW) {
				TextToSpeech.say("The LED is currently OFF.");
				delay(4000);
			}
			else if (!strcmp(helpCommand, VoiceRecognition.getLastCommand())) {
				help();
			}
			else if (!strcmp(unoCommand, VoiceRecognition.getLastCommand())) {
				TextToSpeech.say("UNO!");
				delay(1500);
			}
			else{
				TextToSpeech.say("I don't understand. Try the help command.");
				delay(5000);
			}
			String last;									// terminal print last command
			last = VoiceRecognition.getLastCommand();		// terminal print last command
			Terminal.println(last);							// terminal print last command
		}
		// VoiceRecognition.start(); Auto ON
	}
	state = 1;
	
}

void help()
{
	TextToSpeech.say("Here is a list of available commands:");
	delay(3500);
	TextToSpeech.say("Power ON,");
	delay(2500);
	TextToSpeech.say("Power OFF,");
	delay(2500);
	TextToSpeech.say("State,");
	delay(2500);
	TextToSpeech.say("Help.");
	delay(2500);
	TextToSpeech.say("To see a list of available commands, switch the 1Sheeld application window to Text To Speech after running the help command");
	delay(7000);
}

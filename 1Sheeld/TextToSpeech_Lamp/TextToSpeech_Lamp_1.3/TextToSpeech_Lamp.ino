#define CUSTOM_SETTINGS
#define INCLUDE_VOICE_RECOGNIZER_SHIELD
#define INCLUDE_TEXT_TO_SPEECH_SHIELD

#define INCLUDE_TERMINAL_SHIELD    // terminal print last command

#include <OneSheeld.h>



const char onCommand[] = "led on";
const char offCommand[] = "led off";
const char helpCommand[] = "help";
const char unoCommand[] = "arduino";
const char stateCommand[] = "state";
const char pinonCommand[] = "power on";
const char pinoffCommand[] = "power off";
int state = 1;
int error = 0;
int lamp = 13;
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

    if (VoiceRecognition.isNewCommandReceived()) {
      
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
      else if (!strcmp(stateCommand, VoiceRecognition.getLastCommand())) {
		state = 6;
		stateRead:
		if (state == 6) {
			TextToSpeech.say("Ok. Please tell me the digital pin number");
			state = 7;
			delay(2000);
		}
		if (VoiceRecognition.isNewCommandReceived()) {
			pin = VoiceRecognition.getLastCommand();

			pinToInt();

			delay(1000);
			if (digitalRead(pinNb) == LOW && error == 0) TextToSpeech.say("The state is currently OFF.");
			else if (digitalRead(pinNb) == HIGH && error == 0) TextToSpeech.say("The state is currently ON.");
			delay(4000);
			error = 0;
			state = 1;
		}
		
      }
      else if (!strcmp(helpCommand, VoiceRecognition.getLastCommand())) {
        help();
      }
      else if (!strcmp(pinonCommand, VoiceRecognition.getLastCommand())) {    // must be in loop()
        state = 2;
        poweron:
        if (state == 2) {
          TextToSpeech.say("Tell me the digital pin number");
          state = 3;
          delay(2000);
        }
        if (VoiceRecognition.isNewCommandReceived())
        {
          pin = VoiceRecognition.getLastCommand();

		  pinToInt();

          delay(1000);
          if (error == 0)digitalWrite(pinNb, HIGH);
          if (error == 0)TextToSpeech.say("Done");
          delay(3000);
		  error = 0;
          state = 1;
        }
      }
      else if (!strcmp(pinoffCommand, VoiceRecognition.getLastCommand())) {   // must be in loop()
        state = 4;
      poweroff:
        if (state == 4) {
          TextToSpeech.say("Tell me the digital pin number");
          state = 5;
          delay(2000);
        }
        if (VoiceRecognition.isNewCommandReceived())
        {
          pin = VoiceRecognition.getLastCommand();

		  pinToInt();

          delay(1000);
          if (error == 0)digitalWrite(pinNb, LOW);
		  if (error == 0)TextToSpeech.say("Done");
		  delay(3000);
		  error = 0;
		  state = 1;
        }
      }
      else if (!strcmp(unoCommand, VoiceRecognition.getLastCommand())) {
        TextToSpeech.say("UNO!");
        delay(1500);
      }
      else{
        TextToSpeech.say("I don't understand. Try the help command.");
        delay(5000);
      }
      String last;                  // terminal print last command
      last = VoiceRecognition.getLastCommand();   // terminal print last command
      Terminal.println(last);             // terminal print last command
    }
    // VoiceRecognition.start(); Auto ON
  }
  if (state == 2 || state == 3) {
    goto poweron;
  }
  if (state == 4 || state == 5) {
    goto poweroff;
  }
  if (state == 6 || state == 7) {
	 goto stateRead;
  }
  
}

void help()
{
	TextToSpeech.say("To see a list of available commands, switch the 1Sheeld application window to Text To Speech after running the help command");
	delay(7000);
	TextToSpeech.say("Here is a list of available commands:");
	delay(3500);
	TextToSpeech.say("Power ON,");
	delay(2500);
	TextToSpeech.say("Power OFF,");
	delay(2500);
	TextToSpeech.say("LED ON,");
	delay(2500);
	TextToSpeech.say("LED OFF,");
	delay(2500);
	TextToSpeech.say("State,");
	delay(2500);
	TextToSpeech.say("Help.");
	delay(2500);
  
}

void pinToInt()									//convert e.g. "five" to "5"
{
	if (pin == "one") {
		pinNb = 1;
		TextToSpeech.say("OK");
	}
	else if (pin == "two" || pin == "to") {
		pinNb = 2;
		TextToSpeech.say("OK");
	}
	else if (pin == "three" || pin == "free") {
		pinNb = 3;
		TextToSpeech.say("OK");
	}
	else if (pin == "four" || pin ==  "for" || pin == "far") {
		pinNb = 4;
		TextToSpeech.say("OK");
	}
	else if (pin == "five") {
		pinNb = 5;
		TextToSpeech.say("OK");
	}
	else if (pin == "six") {
		pinNb = 6;
		TextToSpeech.say("OK");
	}
	else if (pin == "seven") {
		pinNb = 7;
		TextToSpeech.say("OK");
	}
	else if (pin == "eight") {
		pinNb = 8;
		TextToSpeech.say("OK");
	}
	else if (pin == "nine") {
		pinNb = 9;
		TextToSpeech.say("OK");
	}
	else if (pin == "ten") {
		pinNb = 10;
		TextToSpeech.say("OK");
	}
	else if (pin == "eleven" || pin == "11") {
		pinNb = 11;
		TextToSpeech.say("OK");
	}
	else if (pin == "twelve" || pin == "12") {
		pinNb = 12; TextToSpeech.say("OK");
	}
	else if (pin == "thirteen" || pin == "13") {
		pinNb = 13;
		TextToSpeech.say("OK");
	}
	else {
		TextToSpeech.say("error");
		error = 1;
	}
}
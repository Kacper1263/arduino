#define CUSTOM_SETTINGS
#define INCLUDE_VOICE_RECOGNIZER_SHIELD
#define INCLUDE_TEXT_TO_SPEECH_SHIELD


#include <OneSheeld.h>



const char onCommand[] = "power on";
const char offCommand[] = "power off";
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
			if (!strcmp(onCommand, VoiceRecognition.getLastCommand())) {
				digitalWrite(lamp, HIGH);
				TextToSpeech.say("The lamp is now ON.");
				delay(4000);
			}
			if (!strcmp(offCommand, VoiceRecognition.getLastCommand())) {
				digitalWrite(lamp, LOW);
				TextToSpeech.say("The lamp is now OFF.");
				delay(4000);
			}
			//delay(5000);
		}
		// VoiceRecognition.start(); Auto ON
	}
	state = 1;
}

/*

OPTIONAL:
To reduce the library compiled size and limit its memory usage, you
can specify which shields you want to include in your sketch by
defining CUSTOM_SETTINGS and the shields respective INCLUDE_ define.

*/

#define CUSTOM_SETTINGS
#define INCLUDE_INTERNET_SHIELD
#define INCLUDE_VOICE_RECOGNIZER_SHIELD
#define INCLUDE_TEXT_TO_SPEECH_SHIELD
#define INCLUDE_MUSIC_PLAYER_SHIELD
#define INCLUDE_CLOCK_SHIELD


/* Include 1Sheeld library. */
#include <OneSheeld.h>

/* create an initial value for the millis counter. */
unsigned long previousMillis = 0;
/* create an intial state for calling alexa. */
bool state = 0;

/* Call an api that will return the weather status in cairo,egypt. */
HttpRequest request1("http://api.openweathermap.org/data/2.5/weather?q=Cairo,EG&appid=43d95b4cf5d0573e2dfe5186c160017a");

/* define the 3 transistors on pins 3,5 and 6. */
#define strip1 3
#define strip2 6
#define strip3 5
/* define the lamp on pin 13. */
int lamp = 13;

/* define the variable that will hold the hours and miniuts . */
int hour, minute;

/* Create 2 charachter arrys to catch the string you want from the sentence. */
char b[4];
char c[4];

/* define variables to hold the hour and miniuts in string. */
String strh, strm;
/* define variable  to hold the day status am or pm. */
String noon;

/* Voice commands set by the user. */

const char mainCommand[] = "arduino";

const char weatherCommand[] = "how is the weather today";

const char playCommand[] = "play music";

const char clockCommand[] = "what time is it";

const char stopCommand[] = "stop";

const char lightonCommand[] = "turn on the lights";

const char lightoffCommand[] = "turn off the lights";



void setup() {
	/* Start Communication. */
	OneSheeld.begin();
	/* set the led strips and the lamp to output. */
	pinMode(strip1, OUTPUT);
	pinMode(strip2, OUTPUT);
	pinMode(strip3, OUTPUT);
	pinMode(lamp, OUTPUT);



	/* Subscribe to success callback for the request. */
	request1.setOnSuccess(&onSuccess1);
	/* Subscribe to json value replies. */
	request1.getResponse().setOnJsonResponse(&onJsonReply1);
	/* on a voice recogniced call the function "myfunction". */
	VoiceRecognition.setOnNewCommand(&myFunction);
}

void loop()
{
	/* make a delay for 5 seconds using millis counter to keep the voice detection alive. */
	unsigned long currentMillis = millis();

	if (currentMillis - previousMillis >= 5000) {

		previousMillis = currentMillis;

		/* get the current time in your phone. */
		Clock.queryDateAndTime();
		/* start voice recognition. */
		VoiceRecognition.start();
	}
	/* save the smartphone hour and minutes in variables. */
	hour = Clock.getHours();
	minute = Clock.getMinutes();

	/* detect the time status if it am or pm. */
	if (hour > 12)
	{
		hour -= 12;
		noon = "P M";
	}
	else {
		noon = "A M";
	}


	/* save the hour and minutes as string. */
	strh = String(hour);
	strm = String(minute);

	/* save the string in a charchter array. */
	strh.toCharArray(b, 4);
	strm.toCharArray(c, 4);
}

/* This function will be invoked each time a new command is given. */
void myFunction(char *commandSpoken)
{
	/* check if any one call alexa or not. */
	if (!strcmp(mainCommand, VoiceRecognition.getLastCommand()))
	{
		alexon();
		MusicPlayer.setVolume(5);
		/* if yes make the state =1 . */
		state = 1;
	}
	/* check if you asked to play music after calling alexa. */
	if (!strcmp(playCommand, VoiceRecognition.getLastCommand()) && state == 1)
	{

		alexoff();
		MusicPlayer.play();
		delay(10000);
		MusicPlayer.stop();
		state = 0;
	}
	/* check if you asked to get the weather status after calling alexa. */
	else if (!strcmp(weatherCommand, VoiceRecognition.getLastCommand()) && state == 1)
	{

		alexoff();
		Internet.performGet(request1);
		/* Wait for 5 seconds. */
		OneSheeld.delay(5000);
		state = 0;
	}
	/* check if you asked for the time after calling alexa. */
	if (!strcmp(clockCommand, VoiceRecognition.getLastCommand()) && state == 1)
	{

		alexoff();
		/* 1Sheeld responds using text-to-speech. */
		TextToSpeech.say("time in cairo is");
		delay(1300);
		TextToSpeech.say(b);
		delay(600);
		TextToSpeech.say(c);
		delay(800);
		TextToSpeech.say(noon);
		delay(500);
		state = 0;
	}
	/* check if you asked to turn the lamp on after calling alexa. */
	if (!strcmp(lightonCommand, VoiceRecognition.getLastCommand()) && state == 1)
	{
		alexoff();
		digitalWrite(lamp, HIGH);
		TextToSpeech.say("the lights turned on");
		delay(6000);
		state = 0;
	}
	/* check if you asked to turn the lamp off after calling alexa. */
	else if (!strcmp(lightoffCommand, VoiceRecognition.getLastCommand()) && state == 1)
	{
		alexoff();
		digitalWrite(lamp, LOW);
		TextToSpeech.say("the lights turned off");
		delay(3000);
		state = 0;
	}
}

void onSuccess1(HttpResponse & response1)
{
	/* Using the response to query the Json chain till the required value. */
	/* i.e. Get the value of 'main' in the first object of the array 'weather' in the response. */
	/* Providing that the response is in JSON format. */
	response1["weather"][0]["main"].query();
	delay(200);
}

void onJsonReply1(JsonKeyChain & hell, char * output)
{
	if (hell == request1.getResponse()["weather"][0]["main"])
	{
		TextToSpeech.say("the weather today in cairo is");
		delay(2000);
		/* 1Sheeld responds using text-to-speech shield. */
		TextToSpeech.say(output);
	}
}

void alexon() {
	/* Turn on the strips in order like the same oder of amazon echo. */
	digitalWrite(strip1, HIGH);
	delay(200);
	digitalWrite(strip2, HIGH);
	delay(200);
	digitalWrite(strip3, HIGH);
}



void alexoff() {

	/* Turn off the strips. */
	digitalWrite(strip1, LOW);
	digitalWrite(strip2, LOW);
	digitalWrite(strip3, LOW);
}

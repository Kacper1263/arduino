/*
    Name:       MQTT_alarm.ino
    Created:	06.01.2019 17:05:36
    Author:     PC-KACPRA\Kacper
*/

// Code ONLY for Arduino Mega!!!


#include <SPI.h>
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <WiFiEsp.h>
#include <WiFiEspClient.h>
#include <WiFiEspUdp.h>
#include <PubSubClient.h>

byte p20[8] = {
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
};
byte p40[8] = {
    B11000,
    B11000,
    B11000,
    B11000,
    B11000,
    B11000,
    B11000,
};
byte p60[8] = {
    B11100,
    B11100,
    B11100,
    B11100,
    B11100,
    B11100,
    B11100,
};
byte p80[8] = {
    B11110,
    B11110,
    B11110,
    B11110,
    B11110,
    B11110,
    B11110,
};
byte p100[8] = {
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
};


int loadingProgress;
int i = 0;

byte buzzer = 22;
byte ledYelow = 23;
byte ledGreen = 24;

// Alarm state
int alarmState = 0;

// WiFi configuration
char ssid[] = "SSID"; // your network SSID (name)
char pass[] = "Password"; // your network password
char server[] = "broker.hivemq.com"; // your network password
int status = WL_IDLE_STATUS; // the Wifi radio's status

// Initialize the Ethernet client object
WiFiEspClient espClient;
PubSubClient client(espClient);

LiquidCrystal_I2C lcd(0x27, 20, 4);
//LiquidCrystal lcd1(7, 6, 5, 4, 3, 2);

void setup() {
    
    pinMode(buzzer, OUTPUT);
    pinMode(ledYelow, OUTPUT);
    pinMode(ledGreen, OUTPUT);
    pinMode(2, INPUT_PULLUP);

    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.print("Loading: ");

	digitalWrite(ledYelow, HIGH);

    lcd.createChar(0, p20);
    lcd.createChar(1, p40);
    lcd.createChar(2, p60);
    lcd.createChar(3, p80);
    lcd.createChar(4, p100);

    delay(1500);
    lcd.clear();
    // initialize serial for debugging
    Serial.begin(115200);
    // initialize serial for ESP module
    Serial1.begin(115200);
    // initialize ESP module
    lcd.setCursor(0, 0);
    lcd.print("ESP initialize  ");
    lcd.setCursor(12, 1);
    lcd.print("0%");
    loadingProgress = 5;

    WiFi.init(&Serial1);

    lcd.setCursor(12, 1);
    lcd.print("40%");
    printLoadingBar();

    lcd.setCursor(0, 0);
    lcd.print("Connect to WiFi ");
    loadingProgress = 8;

    // check for the presence of the shield
    if (WiFi.status() == WL_NO_SHIELD) {
        Serial.println("WiFi shield not present");
        // don't continue
        while (true);
    }

    // attempt to connect to WiFi network
    while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to WPA SSID: ");
        Serial.println(ssid);
        // Connect to WPA/WPA2 network
        status = WiFi.begin(ssid, pass);
    }

    lcd.setCursor(12, 1);
    lcd.print("60%");
    printLoadingBar();

    lcd.setCursor(0, 0);
    lcd.print("Connect to MQTT ");
    loadingProgress = 10;

    Serial.println("You're connected to the network");

    //connect to MQTT server
    client.setServer(server, 1883);
    client.setCallback(callback);
    client.subscribe("km/buzzer/input", 0);

    lcd.setCursor(12, 1);
    lcd.print("80%");
    printLoadingBar();

    lcd.setCursor(0, 0);
    lcd.print("Connect to MQTT ");
    loadingProgress = 10;
    printLoadingBar();
    lcd.setCursor(12, 1);
    lcd.print("100%");
    delay(400);
    lcd.clear();

	digitalWrite(ledYelow, LOW);
	digitalWrite(ledGreen, HIGH);
}

String propablyCurrentTime = "Rano";    // Time to print on LCD
//print any message received for subscribed topic
void callback(char* topic, byte* payload, unsigned int length) {
    payload[length] = '\0';
    String value = String((char*)payload);


    if (value == "ON") {
		digitalWrite(ledYelow, HIGH);
		digitalWrite(ledGreen, LOW);
        int itsTimeToStop = 0;
		int count = 0;

        alarmState = 1;
        lcd.clear();
        lcd.print("Alarm!");
        lcd.setCursor(0, 1);
        lcd.print("Budzik ");
        lcd.print(propablyCurrentTime);
        while (alarmState == 1 && itsTimeToStop <= 40 && digitalRead(2) == HIGH) {
			int sleep = 0;
			digitalWrite(buzzer, HIGH);
            delay(250);
            digitalWrite(buzzer, LOW);
            delay(250);
            itsTimeToStop++;
			count++;
			while (count == 3 && digitalRead(2) == HIGH && sleep < 17) {
				delay(250);
				sleep++;
			}
			if (sleep != 0) count = 0;
        }
    }
    else {
        propablyCurrentTime = value;
    }
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();

	digitalWrite(ledYelow, LOW);
	digitalWrite(ledGreen, HIGH);

    lcd.setCursor(0, 0);
    lcd.print("Oczekiwanie");
    lcd.setCursor(0, 1);
    lcd.print("na budzik       ");
    delay(250);
    
}

void printLoadingBar() {
    lcd.setCursor(0, 1);
    //lcd.print("                ");

    for (i; i <= loadingProgress; i++) {
        for (int j = 0; j < 5; j++) {
            lcd.setCursor(i, 1);
            lcd.write(j);
            delay(25);
        }
    }
}

void reconnect() {
    // Loop until we're reconnected
    lcd.clear();

	digitalWrite(ledYelow, HIGH);
	digitalWrite(ledGreen, LOW);

    while (!client.connected()) {
        Serial.print("Attempting connection ");
        lcd.setCursor(0, 0);
        lcd.print("Reconnecting!");
        // Attempt to connect, just a name to identify the client
        if (client.connect("AMega")) {
            Serial.println("connected");
            lcd.clear();
            //and resubscribe
            client.subscribe("km/buzzer/input", 0);

			digitalWrite(ledYelow, LOW);
			digitalWrite(ledGreen, HIGH);
        }
        else {			
            Serial.print("failed");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

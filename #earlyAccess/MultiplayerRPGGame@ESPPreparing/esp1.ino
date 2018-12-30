
#include <LiquidCrystal.h>
#include <WiFiEsp.h>
#include <WiFiEspClient.h>
#include <WiFiEspUdp.h>
#include <PubSubClient.h>
#include <SPI.h>

char ssid[] = "Nimnul-2.4GHz"; // your network SSID (name)
char pass[] = "XXXXXXXXXXXXX"; // your network password
char server[] = "broker.hivemq.com"; // your network password
int status = WL_IDLE_STATUS; // the Wifi radio's status

// Initialize the Ethernet client object
WiFiEspClient espClient;
PubSubClient client(espClient);

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

//SoftwareSerial soft(2,3); // RX, TX //not used
void setup() {
    lcd.begin(16, 2);
    lcd.clear();
    lcd.print("Loading: ");
    delay(1500);
    lcd.clear();
    // initialize serial for debugging
    Serial.begin(115200);
    // initialize serial for ESP module
    Serial1.begin(115200);
    // initialize ESP module
    lcd.setCursor(0, 0);
    lcd.print("ESP initialize  ");
    lcd.setCursor(0, 1);
    lcd.print("40%");

    WiFi.init(&Serial1);
    
    lcd.setCursor(0, 0);
    lcd.print("Connect to WiFi ");
    lcd.setCursor(0, 1);
    lcd.print("65%");
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

    lcd.setCursor(0, 0);
    lcd.print("Connect to MQTT ");
    lcd.setCursor(0, 1);
    lcd.print("80%");

    Serial.println("You're connected to the network");

    //connect to MQTT server
    client.setServer(server , 1883);
    client.setCallback(callback);
    
    lcd.setCursor(0, 0);
    lcd.print("Connect to MQTT ");
    lcd.setCursor(0, 1);
    lcd.print("100%");
    delay(400);
    lcd.clear();
}

//print any message received for subscribed topic
void callback(char* topic, byte* payload, unsigned int length) {
    payload[length] = '\0';
    String value = String((char*)payload);
   
    Serial.println(value);
    lcd.clear();
    lcd.print(value);

    if (value == "State" || value == "state") {
        lcd.clear();
        lcd.print("ESP is ready");
        client.publish("km/esp/data", "ESP is still ready");
    }
    else
    {
        client.publish("km/esp/data", "OK");
    }
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
    delay(250);
}

void reconnect() {
    // Loop until we're reconnected
    while (!client.connected()) {
        Serial.print("Attempting connection ");
        // Attempt to connect, just a name to identify the client
        if (client.connect("AMega")) {
            Serial.println("connected");
            // Once connected, publish an announcement…
            client.publish("km/esp/data","ESP is ready");
            // … and resubscribe
            client.subscribe("km/esp/input", 0);

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
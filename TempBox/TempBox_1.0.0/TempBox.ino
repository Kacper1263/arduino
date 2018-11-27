// Version 1.0.0
// Author Kacper M.

// DHT Pins Configuration
int DHTPIN = 3;										// DHT22 sensor	on Digital PIN (OUT)
int DHTPIN2 = 4;									// DHT11 sensor on Digital PIN (IN)

#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);		// LCD

#define DHTTYPE DHT22					// DHT sensors type
#define DHTTYPE2 DHT11

DHT dht(DHTPIN, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE2);

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

void setup() {
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print(F("TempBox v.1 by"));
    delay(2000);
    lcd.setCursor(0, 1);
    lcd.print(F("Kacper M"));
    delay(2000);
    lcd.clear();

    dht.begin();
    dht2.begin();
}

void loop()
{
    writeTemp();
}


unsigned long lastMillisDHT = 0;					// Delay timer
unsigned long lastMillisInOut = 0;					// Delay timer
int stateDhtLcd = 0;
void writeTemp()								
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

        String hStr = String(h);					// Convert float to string
        String tStr = String(t);					// Convert float to string
        String hStr2 = String(h2);					// Convert float to string
        String tStr2 = String(t2);					// Convert float to string

        if (currentMillisInOut - lastMillisInOut >= 6000) {		// Change LCD temp IN/OUT
            lastMillisInOut = currentMillisInOut;
            stateDhtLcd = !stateDhtLcd;
        }
        if (stateDhtLcd == 0) {
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
        }
        if (stateDhtLcd == 1) {
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
        }
    }
}

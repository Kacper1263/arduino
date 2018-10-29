#include <Adafruit_TFTLCD.h> 
#include <Adafruit_GFX.h>    
#include <TouchScreen.h>

#define LCD_CS A3 
#define LCD_CD A2 
#define LCD_WR A1 
#define LCD_RD A0 
#define LCD_RESET A4 

#define TS_MINX 121
#define TS_MINY 83
#define TS_MAXX 906
#define TS_MAXY 903

#define YP A3
#define XM A2
#define YM 9
#define XP 8

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 364);

boolean buttonEnabled = true;

void setup() {
	Serial.begin(9600);
	tft.reset();
	uint16_t identifier = tft.readID();
	tft.begin(identifier);
	tft.setRotation(1);
	tft.fillScreen(WHITE);
	tft.drawRect(0, 0, 319, 240, YELLOW);

	tft.setCursor(120, 40);
	tft.setTextColor(BLACK);
	tft.setTextSize(2);
	tft.print("Omega");
	tft.drawLine(110,70,190,70, RED);

	tft.setCursor(115, 80);
	tft.setTextColor(BLACK);
	tft.setTextSize(2);
	tft.print("Serwis");

	tft.setCursor(70, 130);
	tft.setTextColor(BLUE);
	tft.setTextSize(3);
	tft.fillRect(0,110,320,240,BLACK);
	tft.print("Komputerow");
	tft.drawRect(60, 120, 200 , 40, WHITE);

	

}

void loop() {
	TSPoint p = ts.getPoint();

	if (p.z > ts.pressureThreshhold) {

		Serial.print("X = "); Serial.print(p.x);
		Serial.print("\tY = "); Serial.print(p.y);
		Serial.print("\n");

		p.x = map(p.x, TS_MAXX, TS_MINX, 0, 320);
		p.y = map(p.y, TS_MAXY, TS_MINY, 0, 240);

		

			//This is important, because the libraries are sharing pins
			pinMode(XM, OUTPUT);
			pinMode(YP, OUTPUT);

			tft.fillScreen(BLACK);
			tft.setCursor(110, 110);
			tft.print("OMEGA!");
			
			tft.setCursor(75, 150);
			tft.print("X:");

			tft.setCursor(110, 150);
			tft.print(p.x);

			tft.setCursor(75, 190);
			tft.print("Y:");

			tft.setCursor(110, 190);
			tft.print(p.y);

	}
}

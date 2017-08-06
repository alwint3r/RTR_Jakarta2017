#include <Arduino.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>

#define OLED_SDA 21
#define OLED_SCL 22

Adafruit_SH1106 display(OLED_SDA, OLED_SCL);
Adafruit_BMP085 bmp;

void readAndDisplayBMPData();
void drawText(const char* text, int x, int y);

void setup() {
    Serial.begin(9600);
    if (!bmp.begin()) {
	    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
	    while (1) {}
    }

    display.begin(SH1106_SWITCHCAPVCC, 0x3C); 
    display.display();
    delay(2000);
    display.clearDisplay();
}

void loop() {
    readAndDisplayBMPData();
    delay(500);
}

void readAndDisplayBMPData() {
    display.clearDisplay();

    String msg = "";
    float temperatureC = bmp.readTemperature();
    float pressure = bmp.readPressure();
    float altitude = bmp.readAltitude();

    msg = "Temperature = ";
    msg += temperatureC;
    msg += " C";

    Serial.println(msg);
    drawText(msg.c_str(), 0, 0);
    

    msg = "Pressure = ";
    msg += (pressure / 1000);
    msg += " kPa";
    Serial.println(msg);
    drawText(msg.c_str(), 0, 24);

    msg = "Altitude = ";
    msg += altitude;
    msg += " m";
    Serial.println(msg);
    drawText(msg.c_str(), 0, 48);
}

void drawText(const char* text, int x, int y) {
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(x, y);
    display.println(text);
    display.display();
}
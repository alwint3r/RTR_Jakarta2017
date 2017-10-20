#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include <Adafruit_BME280.h>

#define OLED_SDA 21
#define OLED_SCL 22

Adafruit_SH1106 display(OLED_SDA, OLED_SCL);
Adafruit_BME280 bme;

void readAndDisplayBMPData();
void drawText(const char* text, int x, int y);

void setup() {
    Serial.begin(9600);
    if (!bme.begin()) {
	    Serial.println("Could not find a valid BME280 sensor, check wiring!");
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
    float temperatureC = bme.readTemperature();
    float pressure = bme.readPressure();
    float humidity = bme.readHumidity();

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

    msg = "Humidity = ";
    msg += humidity;
    msg += " ";
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

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <Arduino.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include <esp_log.h>

#define OLED_SDA 21
#define OLED_SCL 22

Adafruit_SH1106 display(OLED_SDA, OLED_SCL);
Adafruit_BMP085 bmp;

QueueHandle_t queue;
typedef struct {
    float temp;
    int32_t alt;
    float press;
} sensor_data_t;

void drawText(const char* text, int x, int y) {
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(x, y);
    display.println(text);
    display.display();
}

// Display BMP sensor reading task

void displayBMPReadingTask(void* param) {
    display.begin(SH1106_SWITCHCAPVCC, 0x3C);
    display.display();

    vTaskDelay(2000 / portTICK_PERIOD_MS);
    display.clearDisplay();

    sensor_data_t reading;
    char message[255] = {0};
    UBaseType_t recvStatus;

    while (true) {
        display.clearDisplay();

        recvStatus = xQueueReceive(queue, &reading, portMAX_DELAY);
        if (recvStatus != pdPASS) {
            vTaskDelay(500 / portTICK_PERIOD_MS);
            continue;
        }

        memset(message, 0, 255);
        sprintf(message, "Temperature = %.02f C", reading.temp);
        drawText((const char*)message, 0, 0);
        
        memset(message, 0, 255);
        sprintf(message, "Pressure = %.02f kPa", (reading.press / 1000));
        drawText((const char*)message, 0, 24);

        memset(message, 0, 255);
        sprintf(message, "Altitude = %i m", reading.alt);
        drawText((const char*)message, 0, 48);

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

// Read BMP sensor task

void readBMPSensorTask(void* param) {
    if (!bmp.begin()) {
        ESP_LOGE("BMP", "Failed to initialize sensor. Please check wiring!");
        while (true);
    }

    xTaskCreate(displayBMPReadingTask, "displayBMPReadingTask", 4096, NULL, 5, NULL);

    while (true) {
        sensor_data_t reading;
        reading.temp = bmp.readTemperature();
        reading.press = bmp.readPressure();
        reading.alt = bmp.readAltitude();

        xQueueSendToBack(queue, &reading, portMAX_DELAY);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

extern "C" void app_main() {
    queue = xQueueCreate(1, sizeof(sensor_data_t));
    initArduino();

    xTaskCreate(readBMPSensorTask, "readBMPSensorTask", 4096, NULL, 5, NULL);
}
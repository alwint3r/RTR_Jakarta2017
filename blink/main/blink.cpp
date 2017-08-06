#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>

void blinkTask(void* pvParameter) {
    gpio_num_t led = GPIO_NUM_15;
    gpio_set_direction(led, GPIO_MODE_OUTPUT);
    gpio_pad_select_gpio(led);

    int level = 0;

    while (true) {
        gpio_set_level(led, level);
        level = !level;
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

extern "C" void app_main() {
    xTaskCreate(blinkTask, "blinkTask", 2048, NULL, 5, NULL);
}
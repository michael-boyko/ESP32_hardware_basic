#include "creative.h"

void power_on_dht11() {
    gpio_set_direction(GPIO_EN_DHT11, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_EN_DHT11, 1);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
}

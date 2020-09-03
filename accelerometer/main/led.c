#include "accelerometer.h"

void led_on_off(_Bool on_off) {
    gpio_set_direction(GPIO_LED1, GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_LED2, GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_LED3, GPIO_MODE_OUTPUT);
    if (on_off == true) {
        gpio_set_level(GPIO_LED1, 1);
        gpio_set_level(GPIO_LED2, 1);
        gpio_set_level(GPIO_LED3, 1);
    }
    else {
        gpio_set_level(GPIO_LED1, 0);
        gpio_set_level(GPIO_LED2, 0);
        gpio_set_level(GPIO_LED3, 0);
    }
}

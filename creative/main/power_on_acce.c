#include "creative.h"

void power_on_acce() {
    gpio_set_direction(GPIO_EN_ACCE, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_EN_ACCE, 1);
}
#include "creative.h"

void app_main() {
    uint8_t **screen = create_arr();
    // char str[] = "Hello World!";

    power_on_dht11();
    power_on_oled();
    init_i2c();
    init_display();
    fill_screen_0_or_1(screen, 0);
    // str_in_screen(str, screen);
    screen_onto_display(screen);
    xTaskCreate(get_temperature_and_humidity, "get_temperature_and_humidity", 4088, NULL, 10, NULL);
    xTaskCreate(change_screen, "change_screen", 4088, screen, 11, NULL);
    xTaskCreate(update_display, "update_display", 4088, screen, 12, NULL);
}

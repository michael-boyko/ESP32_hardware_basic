#include "creative.h"

void update_display(uint8_t **screen) {
    int t = 0;
    int h = 0;

    while (true) {
        if (t != temperature || h != humidity) {
            t = temperature;
            h = humidity;
            select_screen(screen);
        }
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

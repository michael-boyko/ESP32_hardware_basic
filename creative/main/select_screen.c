#include "creative.h"

void select_screen(uint8_t **screen) {
    char buf_t[strlen("Temperature: 00C ")];
    char buf_h[strlen("Humidity: 00% ")];

    memset(buf_t, '\0', strlen("Temperature: 00C"));
    memset(buf_h, '\0', strlen("Humidity: 00%"));
    if (pick_screen == 1) {
        sprintf(buf_t, "%s %d%s", "Temperature:", temperature, "C");
        fill_screen_0_or_1(screen, 0);
        str_in_screen(buf_t, screen);
    }
    else {
        sprintf(buf_h, "%s %d%s", "Humidity:", humidity, "%");
        fill_screen_0_or_1(screen, 0);
        str_in_screen(buf_h, screen);
    }
    screen_onto_display(screen);
}

#include "creative.h"

static void cancantination(char *dst, char *first, char *third) {
    char *data = malloc(sizeof(char) * 3);
    int t_or_h = pick_screen == true ? temperature : humidity; 

    memset(data, 0, 3);
    data = itoa(t_or_h, data, 10);
    strcat(dst, first);
    strcat(dst, data);
    strcat(dst, third);
    free(data);
}

void select_screen(uint8_t **screen) {
    char *dst = malloc(sizeof(char) * strlen("Temperature: 00C"));

    memset(dst, '\0', strlen("Temperature: 00C"));
    if (pick_screen == 1) {
        cancantination(dst, "Temperature: ", "C");
    }
    else {
        cancantination(dst, "Humidity: ", "%");	
    }
    fill_screen_0_or_1(screen, 0);
    str_in_screen(dst, screen);
    screen_onto_display(screen);
    free(dst);
}


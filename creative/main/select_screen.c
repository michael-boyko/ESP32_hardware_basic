#include "creative.h"

// static void cancantination(char *dst, char *first, char *third) {
    // char *data = (char*) malloc(sizeof(char) * 3);
    // if (data == NULL)
    // {
    //     write(2, "ERROR 1", 7);
    // }

    // int t_or_h = pick_screen == true ? temperature : humidity; 

    // memset(data, 0, 3);
    // data = itoa(t_or_h, data, 10);
    // strcat(dst, first);
    // strcat(dst, data);
    // strcat(dst, third);
    // free(data);
// }


void select_screen(uint8_t **screen) {
    // char *dst = (char*) malloc(sizeof(char) * strlen("Temperature: 00C"));
    // if (dst == NULL)
    // {
    //     write(2, "ERROR 1", 7);
    // }
    char buf_t[strlen("Temperature: 00C ")];
    char buf_h[strlen("Humidity: 00% ")];
    memset(buf_t, '\0', strlen("Temperature: 00C"));
    memset(buf_h, '\0', strlen("Humidity: 00%"));
    if (pick_screen == 1) {
        sprintf(buf_t, "%s %d%s", "Temperature:", temperature, "C");
        // cancantination(dst, "Temperature: ", "C");
        fill_screen_0_or_1(screen, 0);
        str_in_screen(buf_t, screen);
    }
    else {
        sprintf(buf_t, "%s %d%s", "Humidity:", humidity, "%");
        // cancantination(dst, "Humidity: ", "%");
        fill_screen_0_or_1(screen, 0);
        str_in_screen(buf_h, screen);	
    }
    
    
    screen_onto_display(screen);
    // free(dst);
}


#include "creative.h"

void get_temperature_and_humidity() {
    int *arr_byte = NULL;

    while (true) {
        arr_byte = get_data_dht11();
        temperature = arr_byte[2];
        humidity = arr_byte[0];
        free(arr_byte);
        vTaskDelay(2100 / portTICK_PERIOD_MS);
    }
}

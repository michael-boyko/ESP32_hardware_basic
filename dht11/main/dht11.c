#include <driver/gpio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include <string.h>
#include <unistd.h>

#define GPIO_EN_DHT11 2
#define GPIO_DHT11    4

static int get_level(int time, bool status) {
    int sec = 0;

    while (gpio_get_level(GPIO_DHT11) == status) {
        if (sec > time) 
            return -1;
        sec++;
        ets_delay_us(1);
    }
    return sec;
}

static void set_up_dht11(int *check) {
    gpio_set_direction(GPIO_DHT11, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_DHT11, 0);
    ets_delay_us(20000);
    gpio_set_level(GPIO_DHT11, 1);
    ets_delay_us(40);
    gpio_set_direction(GPIO_DHT11, GPIO_MODE_INPUT);
    
    *check = get_level(80, 0);
    if (*check <= 0) 
        write(2, "ERROR: set_up_dht11 0\n", 22);
    *check = get_level(80, 1);
    if (*check <= 0) 
        write(2, "ERROR: set_up_dht11 1\n", 22);
}

static void get_5_byte_dht11(int *arr_byte, int *check) {
    for (int bit = 1, byte = 0; bit < 41; bit++) {
        *check = get_level(50, 0);
        if (*check <= 0) 
            write(2, "ERROR: get_5_byte_dht11 0\n", 26);
        *check = get_level(72, 1);
        if (*check <= 0) 
            write(2, "ERROR: get_5_byte_dht11 1\n", 26);
        if (*check > 40) {
            arr_byte[byte] <<= 1;
            arr_byte[byte] += 1;
        }
        else
            arr_byte[byte] <<= 1;
        if (bit % 8 == 0)
            byte++;
    }
    if (arr_byte[4] != (arr_byte[0] + arr_byte[1] + arr_byte[2] + arr_byte[3])) {
        write(2, "ERROR: wrong checksum\n", 22);
    }
}

static void power_on_dht11() {
    gpio_set_direction(GPIO_EN_DHT11, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_EN_DHT11, 1);
    sleep(2);
}

void app_main() {
    int check = 0;
    int arr_byte[5] = {0, 0, 0, 0, 0,};

    power_on_dht11();
    set_up_dht11(&check);
    get_5_byte_dht11(arr_byte, &check);
    printf("Temperature: %d C\n", arr_byte[2]);
    printf("Humidity: %d %%\n", arr_byte[0]);
}

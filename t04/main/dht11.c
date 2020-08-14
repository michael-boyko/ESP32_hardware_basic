#include <driver/gpio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include <string.h>
#include <unistd.h>

const int PIN = 4;
const int MAX_BYTE = 5;

static int get_level(int time, bool status) {
 int sec = 0;
 while (gpio_get_level(PIN) == status) {
  if (sec > time) 
            return -1;  
  sec++;
  ets_delay_us(1);  
 }
 return sec;
}

void app_main() {
    int check = 0;
    int arr_num[MAX_BYTE];

    bzero(&arr_num, sizeof(arr_num));
    gpio_set_direction(2, GPIO_MODE_OUTPUT);
    gpio_set_level(2, 1);
    sleep(2);

    gpio_set_direction(PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(PIN, 0);
    ets_delay_us(20000);
    gpio_set_level(PIN, 1);
    ets_delay_us(40);
    gpio_set_direction(PIN, GPIO_MODE_INPUT);
    
    check = get_level(80, 0);
    if (check <= 0) 
        printf("1 ERROR\n");
    check = get_level(80, 1);
    if (check <= 0) 
        printf("2 ERROR\n");

    for (int bit = 1, byte = 0; bit < 41; bit++) {
        check = get_level(50, 0);
        if (check <= 0) 
            printf("3 ERROR iter #%d\n", bit);
        check = get_level(72, 1);
        if (check <= 0) 
            printf("4 ERROR iter #%d\n", bit);
        if (check > 40) {
            arr_num[byte] <<= 1;
            arr_num[byte] += 1;
        }
        else
            arr_num[byte] <<= 1;
        if (bit % 8 == 0)
            byte++;
    }
    
    printf("Temperature: %d C\n", arr_num[2]);
    printf("Humidity: %d %%\n", arr_num[0]);

    if (arr_num[4] != ((arr_num[0] + arr_num[1] + arr_num[2] + arr_num[3]))) {
        printf("Everything ok\n");
    }
    
}

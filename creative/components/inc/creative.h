#ifndef CREATIVE_H
#define CREATIVE_H

#include <dirent.h>
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
// #include "esp_spi_flash.h"
#include "driver/dac.h"
#include <string.h>
#include <unistd.h>
#include "graphic.h"

#define GPIO_EN_DHT11 2
#define GPIO_DHT11 4
//dht11
void power_on_dht11();
int *get_data_dht11();

//creative
void get_temperature_and_humidity();








#endif

#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "driver/dac.h"

#define GPIO_LED1  27
#define GPIO_LED2  26
#define GPIO_LED3  33
#define GPIO_MISO  12
#define GPIO_MOSI  13
#define GPIO_CLK   14
#define GPIO_CS    15

#define READ_REGISTER       0x80u
#define MULTIBYTE_REGISTER  0x40u
#define BW_RATE_REGISTER    0x2Cu
#define POWER_CTL_REGISTER  0x2Du
#define DATAX0_REGISTER     0x32u
#define POWER_CTL_MEASURE   0x08u

void make_beep_sound();
void led_on_off(_Bool on_off);

#endif
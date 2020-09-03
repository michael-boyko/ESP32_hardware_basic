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

#define GPIO_LED1 27
#define GPIO_LED2 26
#define GPIO_LED3 33

void make_beep_sound();
void led_on_off(_Bool on_off);

#endif
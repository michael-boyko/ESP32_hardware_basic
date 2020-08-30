#ifndef CREATIVE_H
#define CREATIVE_H

#include <dirent.h>
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/dac.h"
#include <string.h>
#include <unistd.h>
#include "font6x8.h"

#define GPIO_EN_DHT11 2
#define GPIO_DHT11    4
#define GPIO_EN_OLED  32
#define GPIO_SDA      21
#define GPIO_SCL      22
#define DISPLAY_PORT  I2C_NUM_0
#define DISPLAY_ADDR  0x3C 
#define GPIO_BUTTON1  39
#define GPIO_BUTTON2  18
#define GPIO_INPUT_PIN_SEL  ((1ULL<<GPIO_BUTTON1) | (1ULL<<GPIO_BUTTON2))
int temperature;
int humidity;
_Bool pick_screen;

//dht11
void power_on_dht11();
int *get_data_dht11();

//button
void change_screen(uint8_t **screen);

//creative
void get_temperature_and_humidity();
uint8_t **create_arr();
void select_screen(uint8_t **screen);
void update_display(uint8_t **screen);

//sounds
void make_beep_sound();

//oled
void init_i2c();
void power_on_oled();
void init_display();
void screen_onto_display(uint8_t **screen);
void str_in_screen(char *str, uint8_t **screen);
void fill_screen_0_or_1(uint8_t **screen, _Bool on_off);

#endif

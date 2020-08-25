#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"
#include "driver/dac.h"
#include "driver/ledc.h"

#define GPIO_LED1 27

void pulse_led_use_DAC() {
	dac_output_enable(DAC_CHANNEL_2);
	for (int i = 120; i < 220; i++) {
		dac_output_voltage(DAC_CHANNEL_2, i);
		// vTaskDelay((221 - i) / portTICK_PERIOD_MS);
		vTaskDelay(40 / portTICK_PERIOD_MS);
	}
	for (int i = 220; i > 120; i--) {
		dac_output_voltage(DAC_CHANNEL_2, i);
		// vTaskDelay((221 - i) / portTICK_PERIOD_MS);
		vTaskDelay(40 / portTICK_PERIOD_MS);
	}
}

void pulse_led_use_PWM() {
	ledc_timer_config_t led_timer = {
		.speed_mode = LEDC_HIGH_SPEED_MODE,
		.timer_num = LEDC_TIMER_0,
		.freq_hz = 5000,
		.duty_resolution = LEDC_TIMER_13_BIT,
		.clk_cfg = LEDC_AUTO_CLK,
	};

	ledc_channel_config_t channel = {
		.channel    = LEDC_CHANNEL_0,
        .duty       = 0,
        .gpio_num   = 27,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .hpoint     = 0,
        .timer_sel  = LEDC_TIMER_0
	};

	ledc_timer_config(&led_timer);
	ledc_channel_config(&channel);
	ledc_fade_func_install(0);

	ledc_set_fade_with_time(channel.speed_mode, channel.channel, 3000, 4000);
    ledc_fade_start(channel.speed_mode, channel.channel, LEDC_FADE_NO_WAIT);
    vTaskDelay(4000 / portTICK_PERIOD_MS);
    ledc_set_fade_with_time(channel.speed_mode, channel.channel, 0, 4000);
    ledc_fade_start(channel.speed_mode, channel.channel, LEDC_FADE_NO_WAIT);
    vTaskDelay(4000 / portTICK_PERIOD_MS);
    
}

void app_main() {
	for (int i = 0; i < 10; ++i) {
		pulse_led_use_DAC();
		pulse_led_use_PWM();
	 } 
}

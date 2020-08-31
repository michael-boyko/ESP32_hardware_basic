#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"

/** @brief Uses SPI3 (VSPI). */
#define ADXL_HOST  VSPI_HOST
/** @brief DMA channel is not used. */
#define DMA_CHAN  0

#define ADXL_HOST  VSPI_HOST
/** @brief GPIO# for MISO. */
#define PIN_NUM_MISO  12
/** @brief GPIO# for MOSI. */
#define PIN_NUM_MOSI  13
/** @brief GPIO# for SCLK. */
#define PIN_NUM_CLK  14

#define PIN_NUM_CS  15


/** @brief ADXL345 register read flag. */
#define ADXL345_REG_READ_FLAG  0x80u
/** @brief ADXL345 register multibyte flag. */
#define ADXL345_REG_MB_FLAG  0x40u
/** @brief ADXL345 register: DEVID. */
#define ADXL345_REG_DEVID  0x00u
/** @brief ADXL345 register: BW_RATE. */
#define ADXL345_REG_BW_RATE  0x2Cu
/** @brief ADXL345 register: POWER_CTL. */
#define ADXL345_REG_POWER_CTL  0x2Du
/** @brief ADXL345 register: DATAX0. */
#define ADXL345_REG_DATAX0  0x32u

/** @brief ADXL345 POWER_CTL flag: Measure. */
#define ADXL345_POWER_CTL_MEASURE  0x08u

/** @brief ADXL345 delay to update (200ms). */
#define ADXL345_UPDATE_DELAY  (200u / portTICK_PERIOD_MS)

static uint8_t adxl345_read (spi_device_handle_t spi, uint8_t address) {
    esp_err_t ret;
    spi_transaction_t trans = {
		.flags = SPI_TRANS_USE_RXDATA | SPI_TRANS_USE_TXDATA,
		.cmd = address | ADXL345_REG_READ_FLAG,
		.length = 8 // in bits
	};
    ret = spi_device_polling_transmit(spi, &trans);
    assert(ret == ESP_OK);
	return trans.rx_data[0];
}

static void adxl345_write (
		spi_device_handle_t spi,
		uint8_t address,
		uint8_t value)
{
	esp_err_t ret;
	spi_transaction_t trans = {
		.flags = SPI_TRANS_USE_RXDATA,
		.cmd = address,
		.tx_buffer = &value,
		.length = 8 // in bits
	};
	ret = spi_device_polling_transmit(spi, &trans);
	assert(ret == ESP_OK);
}

static void adxl345_read_acceleration (spi_device_handle_t spi, int16_t* accs) {
	esp_err_t ret;
	uint8_t tx_buffer[3u * sizeof(uint16_t)]; // a dummy buffer
	spi_transaction_t trans = {
		.cmd = ADXL345_REG_READ_FLAG |
			ADXL345_REG_MB_FLAG |
			ADXL345_REG_DATAX0,
		.length = sizeof(tx_buffer) * 8, // in bits
		.tx_buffer = tx_buffer,
		.rx_buffer = accs
	};
	ret = spi_device_polling_transmit(spi, &trans);
	assert(ret == ESP_OK);
}

static void adxl345_init (spi_device_handle_t spi) {
	uint8_t out;
    out = adxl345_read(spi, ADXL345_REG_DEVID);
    printf("DEVID: 0x%X\n", out);
	out = adxl345_read(spi, ADXL345_REG_BW_RATE);
	printf("BW_RATE: 0x%X\n", out);
}

static void adxl345_start (spi_device_handle_t spi) {
	adxl345_write(spi, ADXL345_REG_POWER_CTL, ADXL345_POWER_CTL_MEASURE);
	vTaskDelay(ADXL345_UPDATE_DELAY);
}

static void adxl345_read_acceleration_task (void* pvParameters) {
	int16_t accs[3];
	spi_device_handle_t spi = (spi_device_handle_t)pvParameters;
	while (1) {
		adxl345_read_acceleration(spi, accs);
		printf(
			"ax, ay, az: %d, %d, %d\n",
			(int)accs[0],
			(int)accs[1],
			(int)accs[2]);
		if ((int)accs[1] >= 250 || (int)accs[0] >= 258 || (int)accs[2] >= 250)
		{
			printf("____________-----------___________\n");
		}
		vTaskDelay(500 / portTICK_PERIOD_MS);
	}
	// return accs;
}

void app_main() {
	gpio_set_direction(23, GPIO_MODE_OUTPUT);
    gpio_set_level(32, 1);
    esp_err_t ret;
    spi_device_handle_t spi;
    spi_bus_config_t buscfg = {
        .miso_io_num = PIN_NUM_MISO,
        .mosi_io_num = PIN_NUM_MOSI,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1
    };
    spi_device_interface_config_t devcfg = {
		.clock_speed_hz = 1000000, // 1Mbps. nearest clock will be chosen.
        .mode = 3, // CPOL=1, CPHA=1
        .spics_io_num = PIN_NUM_CS,
		.command_bits = 8, // ADXL345 always takes 1+7 bit command (address).
        .queue_size = 1 // I do not know an appropriate size.
    };
    // initializes the SPI bus
    ret = spi_bus_initialize(ADXL_HOST, &buscfg, DMA_CHAN);
    ESP_ERROR_CHECK(ret);
    // attaches the ADXL to the SPI bus
    ret = spi_bus_add_device(ADXL_HOST, &devcfg, &spi);
    ESP_ERROR_CHECK(ret);
    // initializes the ADXL
    adxl345_init(spi);
	// starts sampling
	adxl345_start(spi);
	
		adxl345_read_acceleration_task((void*)spi);
}

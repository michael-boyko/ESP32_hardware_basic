#include "accelerometer.h"

/** @brief Uses SPI3 (VSPI). */
// #define ADXL_HOST  VSPI_HOST
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

static void adxl345_init (spi_device_handle_t spi) {
	spi_transaction_t trans = {
		.flags = SPI_TRANS_USE_RXDATA | SPI_TRANS_USE_TXDATA,
		.cmd = ADXL345_REG_BW_RATE | ADXL345_REG_READ_FLAG,
		.length = 8 // in bits
	};
    spi_device_polling_transmit(spi, &trans);
}

static void adxl345_start (spi_device_handle_t spi) {
	int8_t value;
	spi_transaction_t trans = {
		.flags = SPI_TRANS_USE_RXDATA,
		.cmd = ADXL345_REG_POWER_CTL,
		.tx_buffer = &value,
		.length = 8 // in bits
	};
	spi_device_polling_transmit(spi, &trans);
}

static void adxl345_read_acceleration (spi_device_handle_t spi, int16_t* accs) {
	uint8_t tx_buffer[3u * sizeof(uint16_t)]; // a dummy buffer
	spi_transaction_t trans = {
		.cmd = ADXL345_REG_READ_FLAG | ADXL345_REG_MB_FLAG | ADXL345_REG_DATAX0,
		.length = sizeof(tx_buffer) * 8, // in bits
		.tx_buffer = tx_buffer,
		.rx_buffer = accs
	};
	spi_device_polling_transmit(spi, &trans);
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
		if ((int)accs[2] <= 25 && (int)accs[2] >= 0) {
			// make_beep_sound();
			// led_on_off(true);
			vTaskDelay(100 / portTICK_PERIOD_MS);
		}
		else {
            led_on_off(false);
		}
		vTaskDelay(500 / portTICK_PERIOD_MS);
	}
}

void app_main() {
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

    spi_bus_initialize(ADXL_HOST, &buscfg, DMA_CHAN);

    spi_bus_add_device(ADXL_HOST, &devcfg, &spi);

    adxl345_init(spi);

	adxl345_start(spi);
	
		adxl345_read_acceleration_task((void*)spi);
}

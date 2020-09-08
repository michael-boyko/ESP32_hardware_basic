#include "creative.h"

static void accelerometer_write(spi_device_handle_t spi, uint8_t address, uint8_t value) {
    spi_transaction_t trans = {
        .flags = SPI_TRANS_USE_RXDATA,
        .cmd = address,
        .tx_buffer = &value,
        .length = 8
    };
    spi_device_polling_transmit(spi, &trans);
}

static void read_acceleration(spi_device_handle_t spi, int16_t* accs) {
    uint8_t tx_buffer[3u * sizeof(uint16_t)];
    spi_transaction_t trans = {
        .cmd = READ_REGISTER | MULTIBYTE_REGISTER | DATAX0_REGISTER,\
        .length = sizeof(tx_buffer) * 8, 
        .tx_buffer = tx_buffer,
        .rx_buffer = accs
    };

    spi_device_polling_transmit(spi, &trans);
}

static void init_accelerometer(spi_device_handle_t spi) {
    spi_transaction_t trans = {
        .flags = SPI_TRANS_USE_RXDATA | SPI_TRANS_USE_TXDATA,
        .cmd = BW_RATE_REGISTER | READ_REGISTER,
        .length = 8
    };

    spi_device_polling_transmit(spi, &trans);
    accelerometer_write(spi, POWER_CTL_REGISTER, POWER_CTL_MEASURE);
    vTaskDelay(200 / portTICK_PERIOD_MS);
}

static void read_accelerometer(spi_device_handle_t spi, uint8_t **screen) {
    int16_t accs[3];
    _Bool flipped = true;

    while (1) {
        read_acceleration(spi, accs);
        printf("ax, ay, az: %d, %d, %d\n", (int)accs[0], (int)accs[1], (int)accs[2]);
        if ((int)accs[1] < -150 && flipped == true) {
            reinit_display(false);
            select_screen(screen);
            flipped = false;
        }
        if ((int)accs[1] > 150 && flipped == false) {
            reinit_display(true);
            select_screen(screen);
            flipped = true;
        }
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void flippe(uint8_t **screen) {
     spi_device_handle_t spi;
    spi_bus_config_t buscfg = {
        .miso_io_num = GPIO_MISO,
        .mosi_io_num = GPIO_MOSI,
        .sclk_io_num = GPIO_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1
    };
    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 1000000,
        .mode = 3,
        .spics_io_num = GPIO_CS,
        .command_bits = 8,
        .queue_size = 1,
    };

    spi_bus_initialize(VSPI_HOST, &buscfg, 0);
    spi_bus_add_device(VSPI_HOST, &devcfg, &spi);
    init_accelerometer(spi);
    read_accelerometer(spi, screen);
}
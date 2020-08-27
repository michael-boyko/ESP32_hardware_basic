#include <creative.h>

int temperature = 0;
int humidity = 0;

void app_main() {
    power_on_dht11();
    get_temperature_and_humidity();
}

// const int PIN = 4;
// const int MAX_BYTE = 5;

// #define SH1106_DEFAULT_ADDR 0x3C      // default I2C address
// #define SH1106_DEFAULT_PORT I2C_NUM_0 // default I2C interface port

// #define SDA_PIN  GPIO_NUM_21
// #define SCL_PIN  GPIO_NUM_22
// #define I2C_ADDR SH1106_DEFAULT_ADDR
// #define I2C_PORT SH1106_DEFAULT_PORT
// #define EN_OLED_PIN GPIO_NUM_32

// #define GPIO_LED1 27
// #define GPIO_LED2 26
// #define GPIO_BUTTON1 39
// #define GPIO_BUTTON2 18
// #define GPIO_OUTPUT_PIN_SEL  ((1ULL<<GPIO_LED1) | (1ULL<<GPIO_LED2))
// #define GPIO_INPUT_PIN_SEL  ((1ULL<<GPIO_BUTTON1) | (1ULL<<GPIO_BUTTON2))
// #define ESP_INTR_FLAG_DEFAULT 0

// static xQueueHandle gpio_evt_queue = NULL;

// static void IRAM_ATTR gpio_isr_handler(void* arg) {
//     uint32_t gpio_num = (uint32_t) arg;

//     xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
// }






// typedef struct {
//     uint8_t addr;           // I2C address
//     i2c_port_t port;        // I2C interface port
//     uint16_t changes;       // page change bit to optimize writes
//     uint8_t pages[8][128]; // page buffer
// } sh1106_t;


// void on_oled_power() {
//     gpio_set_direction(EN_OLED_PIN, GPIO_MODE_OUTPUT);
//     gpio_set_level(EN_OLED_PIN, 1);
//     vTaskDelay(1000 / portTICK_PERIOD_MS);
// }

// void init_i2c() {
//     i2c_config_t i2c_config = {
//         .mode = I2C_MODE_MASTER,
//         .sda_io_num = SDA_PIN,
//         .scl_io_num = SCL_PIN,
//         .sda_pullup_en = GPIO_PULLUP_ENABLE,
//         .scl_pullup_en = GPIO_PULLUP_ENABLE,
//         .master.clk_speed = 1000000
//     };
//     i2c_param_config(I2C_PORT, &i2c_config);
//     i2c_driver_install(I2C_PORT, I2C_MODE_MASTER, 0, 0, 0);
// }

// void sh1106_init(sh1106_t *display) {
//     i2c_cmd_handle_t cmd = i2c_cmd_link_create();
//     i2c_master_start(cmd);
//     i2c_master_write_byte(cmd, (display->addr << 1) | I2C_MASTER_WRITE, true);
//     i2c_master_write_byte(cmd, 0x00, true); // command stream
//     i2c_master_write_byte(cmd, 0xAE, true); // off
//     i2c_master_write_byte(cmd, 0xD5, true); // clock div
//     i2c_master_write_byte(cmd, 0x80, true);
//     i2c_master_write_byte(cmd, 0xA8, true); // multiplex
//     i2c_master_write_byte(cmd, 0xFF, true);

//     i2c_master_write_byte(cmd, 0x20, true);
//     i2c_master_write_byte(cmd, 0x00, true);

//     i2c_master_write_byte(cmd, 0x8D, true); // charge pump
//     i2c_master_write_byte(cmd, 0x14, true);
//     i2c_master_write_byte(cmd, 0x10, true); // high column
//     i2c_master_write_byte(cmd, 0xB0, true);
//     i2c_master_write_byte(cmd, 0xC8, true);
//     i2c_master_write_byte(cmd, 0x00, true); // low column
//     i2c_master_write_byte(cmd, 0x10, true);
//     i2c_master_write_byte(cmd, 0x40, true);
//     i2c_master_write_byte(cmd, 0xA1, true); // segment remap
//     i2c_master_write_byte(cmd, 0xA6, true);
//     i2c_master_write_byte(cmd, 0x81, true); // contrast
//     i2c_master_write_byte(cmd, 0xFF, true);
//     i2c_master_write_byte(cmd, 0xAF, true); // on
//     i2c_master_stop(cmd);
//     i2c_master_cmd_begin(display->port, cmd, 10 / portTICK_PERIOD_MS);
//     i2c_cmd_link_delete(cmd);
// }

// void sh1106_write_page(sh1106_t *display, uint8_t page) {
//     i2c_cmd_handle_t cmd = i2c_cmd_link_create();
//     i2c_master_start(cmd);
//     i2c_master_write_byte(cmd, (display->addr << 1) | I2C_MASTER_WRITE, true);
//     i2c_master_write_byte(cmd, 0x80, true); // single command
//     i2c_master_write_byte(cmd, 0xB0, true);
//     i2c_master_write_byte(cmd, 0x40, true); // data stream
//     i2c_master_write(cmd, *(display->pages), 128 * 8, true);
//     i2c_master_stop(cmd);
//     i2c_master_cmd_begin(display->port, cmd, 10 / portTICK_PERIOD_MS);
//     i2c_cmd_link_delete(cmd);
// }




// void sh1106_fill(sh1106_t *display, _Bool on_off) {
//     for (uint8_t i = 0; i < 8; i++) {
//         for (uint8_t j = 0; j < 128; j++) {
//             if (on_off == true)
//                 display->pages[i][j] = 0xFF;
//             else
//                 display->pages[i][j] = 0x00;
//         }
//     }
//     display->changes = 0xffff;
// }


// static uint8_t *get_code_symbol(int num_char, uint8_t *arr_symbols) {
//     uint8_t *symbol = malloc(sizeof(uint8_t) * 6);
//     int start = (num_char - 32) * 6;

//     for (int i = 0; i < 6; ++i) {
//         symbol[i] = arr_symbols[start + i];
//     }
//     return symbol;
// }

// static void write_str(char *str, uint8_t *arr_symbols, sh1106_t *display) {
//     uint8_t *symbol = NULL;
//     int start_point = (128 - strlen(str) * 6) / 2;
//     for (int j = 0; str[j] != '\0'; ++j) {
//         symbol = get_code_symbol(str[j], arr_symbols);
//         for (int i = 0; i < 6; ++i) {
//             display->pages[3][start_point + (j * 6) + i] = symbol[i];
//         }
//         free(symbol);
//     }
// }

// static void make_beep() {
//     int a = 100;

//     dac_output_enable(DAC_CHANNEL_1);
//     while (a > 0) {
//         for (int i = 0; i < 225; i+=30) {
//             dac_output_voltage(DAC_CHANNEL_1, i);
//             ets_delay_us(100);
//         }
//         a--;
//     }   
// }

// static void display_t_h(_Bool *screen) {
//     uint32_t io_num;
//     sh1106_t display;
//     display.addr = I2C_ADDR;
//     display.port = I2C_PORT;
//     char temperature[] = "Temperature: 00C";
//     char humidity[] = "Humidity: 00%";
//     sh1106_fill(&display, 0);

//     while (true) {
//         if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
//             *screen = io_num == 39 ? true : false;
//             // make_beep();
//             if (*screen == true) {
//                 // memset(temperature, '\0', strlen("Temperature: 00C"));
//                 // temperature = strcat(strcat(strcat(temperature, "Temperature: "), tem), "C");
//                 sh1106_fill(&display, 0);
//                 write_str(temperature, arr_symbols, &display);
//                 sh1106_write_page(&display, 4);
//             }
//             else {
//                 // memset(temperature, '\0', strlen("Temperature: 00C"));
//                 // temperature = strcat(strcat(strcat(temperature, "Humidity: "), tem), "%");
//                 sh1106_fill(&display, 0);
//                 write_str(humidity, arr_symbols, &display);
//                 sh1106_write_page(&display, 4);
//             }
//         }  
//     }
// }

// void app_main() {
// ///////////////////////////////////
//     char *temperature = malloc(sizeof(char) * strlen("Temperature: 00C"));
//     char *tem = malloc(sizeof(char) * 3);
//     int *data_temper_humid = NULL;
//     int t = 0;
//     int h = 0;
//     _Bool screen = true;
//     on_oled_power();
//     init_i2c(); //important

//     memset(temperature, '\0', strlen("Temperature: 00C"));
//     memset(tem, '\0', 3);
// ////////////////////////////////////
//     // sh1106_t display;
//     sh1106_t display;
//     display.addr = I2C_ADDR;
//     display.port = I2C_PORT;    

//     sh1106_init(&display);
//     sh1106_fill(&display, 0);
// ////////////////////////////////////

//     gpio_config_t io_conf;

//     io_conf.intr_type = GPIO_PIN_INTR_POSEDGE;
//     io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
//     io_conf.mode = GPIO_MODE_INPUT;
//     gpio_config(&io_conf);
// ////////////////////////////////////
//     gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));

//     set_up_dht11();
    
    
    

//     xTaskCreate(display_t_h, "display_t_h", 2048, &screen, 10, NULL);

//     gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
//     gpio_isr_handler_add(GPIO_BUTTON1, gpio_isr_handler, (void*) GPIO_BUTTON1);
//     gpio_isr_handler_add(GPIO_BUTTON2, gpio_isr_handler, (void*) GPIO_BUTTON2);

//     // uint32_t io_num;

    
//     // display_t_h(&display);
//     while (true){
//         data_temper_humid = get_temperature_and_humidity();
//         if (data_temper_humid[2] != t || data_temper_humid[0] != h) {
//          if (screen == true){
//             memset(temperature, '\0', strlen("Temperature: 00C"));
//             memset(tem, '\0', 3);
//             tem = itoa(data_temper_humid[2], tem, 10);
//             temperature = strcat(strcat(strcat(temperature, "Temperature: "), tem), "C");
//             sh1106_fill(&display, 0);
//             write_str(temperature, arr_symbols, &display);
//             sh1106_write_page(&display, 4);
//             t = data_temper_humid[2];
//             h = data_temper_humid[0];
//         }
//         else {
//             memset(temperature, '\0', strlen("Temperature: 00C"));
//             memset(tem, '\0', 3);
//             tem = itoa(data_temper_humid[0], tem, 10);
//             temperature = strcat(strcat(strcat(temperature, "Humidity: "), tem), "%");
//             sh1106_fill(&display, 0);
//             write_str(temperature, arr_symbols, &display);
//             sh1106_write_page(&display, 4);
//             t = data_temper_humid[2];
//             h = data_temper_humid[0];
//         }
// }

//         free(data_temper_humid);

//         vTaskDelay(2100 / portTICK_PERIOD_MS);
//     }

// }

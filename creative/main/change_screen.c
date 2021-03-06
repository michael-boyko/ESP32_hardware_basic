#include "creative.h"

static xQueueHandle gpio_evt_queue = NULL;

static void IRAM_ATTR gpio_isr_handler(void* arg) {
    uint32_t gpio_num = (uint32_t) arg;
    
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

static void set_up_button() {
    gpio_config_t io_conf;

    io_conf.intr_type = GPIO_PIN_INTR_POSEDGE;
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
    io_conf.mode = GPIO_MODE_INPUT;
    gpio_config(&io_conf);

    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));

    gpio_install_isr_service(0);
    gpio_isr_handler_add(GPIO_BUTTON1, gpio_isr_handler, (void*) GPIO_BUTTON1);
    gpio_isr_handler_add(GPIO_BUTTON2, gpio_isr_handler, (void*) GPIO_BUTTON2);
}

static void off_on_inter(_Bool on_off) {
    if (on_off == 1) {
            gpio_intr_enable(GPIO_BUTTON1);
            gpio_intr_enable(GPIO_BUTTON2);
       }
    else {
        gpio_intr_disable(GPIO_BUTTON1);
        gpio_intr_disable(GPIO_BUTTON2);
    }   
}

void change_screen(uint8_t **screen) {
    uint32_t io_num;

    set_up_button();
    while(true) {
        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            if (io_num == 39) {
                off_on_inter(0);
                make_beep_sound();
                pick_screen = true;
                vTaskDelay(20 / portTICK_PERIOD_MS);
                off_on_inter(1); 
            }
            else {
                off_on_inter(0);
                make_beep_sound();
                pick_screen = false;
                vTaskDelay(20 / portTICK_PERIOD_MS);
                off_on_inter(1); 
            }
            select_screen(screen);
        }
    }
}
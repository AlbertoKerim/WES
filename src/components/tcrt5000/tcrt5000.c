#include "tcrt5000.h"

void tcrt5000_init(void){
    gpio_config_t io_conf = {
        .pin_bit_mask = 1ULL << IR_PIN,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);
}
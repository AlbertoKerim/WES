#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"             // For esp_timer_get_time()
#include "esp_rom_sys.h"           // For esp_rom_delay_us()


#define TRIG_PIN 26// Change based on wiring (e.g., P1-18)
#define ECHO_PIN 27// Change based on wiring (e.g., P1-19)
#define TAG "HC-SR04"

void hcsr04_init(void);
void hcsr04_read_distance(float *distance);

#include "sensors.h"

static char array[32];

static TimerHandle_t SENSOR_timer;

veml7700_handle_t veml7700_dev;

float humidity = 0.f;
float temperature = 0.f;

double lux_als = 0.f;
double fc_als = 0.f;

float distance = 0.f;

static void sensor_cb(TimerHandle_t handle);

void bar_update_task(void *arg)
{
    while(1)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void sensors_init(void)
{
    ESP_ERROR_CHECK(sht31_init());

    hcsr04_init();

    ESP_ERROR_CHECK(veml7700_initialize(&veml7700_dev, I2C_MASTER_NUM));

    SENSOR_timer = xTimerCreate("Sensor_timer", (SENSOR_REPORTING_PERIOD * 1000) / portTICK_PERIOD_MS,
                            pdTRUE, NULL, sensor_cb);

    xTimerStart(SENSOR_timer, 0);

    //xTaskCreate(bar_update_task, "bar_update_task", 2048, NULL, 5, NULL);
    //xTaskCreatePinnedToCore(color_invert_task, "color_invert_task", 2048, NULL, 5, NULL, 1);

}

static void sensor_cb(TimerHandle_t handle){
    //printf("Hello");
    sht31_read_temp_humi(&temperature, &humidity);
    //ESP_LOGI(TAG, "Periodic timer called, time since boot: %lld us", time_since_boot);
    sprintf(array, "Temperature:  %0.2f", temperature);

    // Read the ALS data
    ESP_ERROR_CHECK(veml7700_read_als_lux_auto(veml7700_dev, &lux_als) );
    // Convert to foot candles
    fc_als = lux_als * LUX_FC_COEFFICIENT;

    if((fc_als < ALS_THRESHOLD && is_inverted()) || (fc_als > ALS_THRESHOLD && !is_inverted())) {
        toggle_invert_colors();
    } 

    hcsr04_read_distance(&distance);

    sprintf(array, "Distance: %0.f", distance);

    lv_label_set_text(ui_TempsText, array);
}







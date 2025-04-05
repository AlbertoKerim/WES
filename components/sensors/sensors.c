#include "sensors.h"


static char array[32];

static TimerHandle_t SENSOR_timer;

static float humidity = 0.f;
static float temperature = 0.f;

static void sensor_cb(TimerHandle_t handle);

void bar_update_task(void *arg)
{
    while(1)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    } 
    //vTaskDelay(1000 / portTICK_PERIOD_MS);
}

void sensors_init(void)
{
    sht31_init();

    SENSOR_timer = xTimerCreate("Sensor_timer", (SENSOR_REPORTING_PERIOD * 1000) / portTICK_PERIOD_MS,
                            pdTRUE, NULL, sensor_cb);

    xTimerStart(SENSOR_timer, 0);

}

static void sensor_cb(TimerHandle_t handle){
    //printf("Hello");
    sht31_read_temp_humi(&temperature, &humidity);
    //ESP_LOGI(TAG, "Periodic timer called, time since boot: %lld us", time_since_boot);
    sprintf(array, "Temperature:  %0.2f", temperature);
    lv_label_set_text(ui_TempsText, array);
}


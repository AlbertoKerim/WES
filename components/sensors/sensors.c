#include "sensors.h"
#include "esp_timer.h"

static char array[32];

static float humidity = 0.f;
static float temperature = 0.f;

static void periodic_timer_callback(void* arg);
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
   // xTaskCreate(bar_update_task, "bar_update_task", 2048, NULL, 5, NULL);
    ESP_ERROR_CHECK(sht31_init());

    const esp_timer_create_args_t periodic_timer_args = {
        .callback = &periodic_timer_callback,
        /* name is optional, but may help identify the timer when debugging */
        .name = "periodic"
    };

    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));

    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, 500000));

}

static void periodic_timer_callback(void* arg)
{
    ESP_ERROR_CHECK(sht31_read_temp_humi(&temperature, &humidity));

    sprintf(array, "Temperature:    %0.2f°C", temperature);

    lv_label_set_text(ui_TempsText, array);
    
}



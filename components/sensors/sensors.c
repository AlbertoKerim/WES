#include "sensors.h"

void bar_update_task(void *arg)
{
    while(1)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        uint32_t v = lv_bar_get_value(ui_Bar3);
        v == 100 ? lv_bar_set_value(ui_Bar3, 0, LV_ANIM_OFF) : lv_bar_set_value(ui_Bar3, v + 10, LV_ANIM_OFF);
    }
}

void sensors_init(void)
{
    xTaskCreate(bar_update_task, "bar_update_task", 2048, NULL, 5, NULL);
}
/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"


#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "driver/i2c.h"

#include "ambient.h"

#define SDA_PIN GPIO_NUM_21
#define SCL_PIN GPIO_NUM_22

#define I2C_MASTER_FREQ_HZ 100000
#define I2C_MASTER_NUM 0

void i2c_master_setup(void)
{
    i2c_config_t conf;

    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = SDA_PIN;
    conf.scl_io_num = SCL_PIN;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    conf.clk_flags = 0;

    i2c_param_config(I2C_MASTER_NUM, &conf);
    i2c_driver_install(I2C_MASTER_NUM, I2C_MODE_MASTER, 0, 0, 0);
}

void app_main(void)
{
    printf("Hello world!\n");
   
    i2c_master_setup();

    veml7700_handle_t veml7700_dev;

    esp_err_t init_result = veml7700_initialize(&veml7700_dev, I2C_MASTER_NUM);
    if (init_result != ESP_OK) {
        return;
    }

    while (true) {
        double lux_als, lux_white, fc_als, fc_white;

        // Read the ALS data
        ESP_ERROR_CHECK( veml7700_read_als_lux_auto(veml7700_dev, &lux_als) );
        // Convert to foot candles
        fc_als = lux_als * LUX_FC_COEFFICIENT;

        // Read the White data
        ESP_ERROR_CHECK( veml7700_read_white_lux_auto(veml7700_dev, &lux_white) );
        // Convert to foot candles
        fc_white = lux_white * LUX_FC_COEFFICIENT;

        printf("VEML7700 measured ALS %0.4f lux or %0.4f fc \n", lux_als, fc_als);
        printf("VEML7700 measured White %0.4f lux or %0.4f fc \n\n", lux_white, fc_white);

        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }

    
}


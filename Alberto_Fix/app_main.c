/* 
  sht31 example
*/



#include <stdio.h>
#include "sht31.h"
#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "sdkconfig.h"
/*


static const char *TAG = "sht31 example";

void app_main(void)
{  
    float temp, humi;
    ESP_ERROR_CHECK(sht31_init());
    while(true) {
      ESP_ERROR_CHECK(sht31_read_temp_humi(&temp, &humi));

      ESP_LOGI(TAG, "temp = %.2f, humi = %.2f\n", temp, humi);
      vTaskDelay(2000 / portTICK_PERIOD_MS); 
    }

}
    */

/*

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"

#define I2C_MASTER_SCL_IO    21    //!< GPIO number used for I2C master clock 
#define I2C_MASTER_SDA_IO    22    //!< GPIO number used for I2C master data 
#define I2C_MASTER_NUM       I2C_NUM_1 //!< I2C master I2C port number 
#define I2C_MASTER_FREQ_HZ   10000    //!< I2C master clock frequency 
#define SHT31_SENSOR_ADDR    0x44      //!< SHT31 I2C address 

static esp_err_t i2c_master_init(void) {
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    esp_err_t err = i2c_param_config(I2C_MASTER_NUM, &conf);
    if (err != ESP_OK) {
        return err;
    }
    return i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);
}

void read_sht31_sensor() {
    uint8_t data[6];
    i2c_master_write_read_device(I2C_MASTER_NUM, SHT31_SENSOR_ADDR, NULL, 0, data, sizeof(data), pdMS_TO_TICKS(1000));

    uint16_t temp_raw = (data[0] << 8) | data[1];
    uint16_t hum_raw = (data[3] << 8) | data[4];

    float temperature = -45 + 175 * ((float)temp_raw / 65535.0);
    float humidity = 100 * ((float)hum_raw / 65535.0);

    printf("Temperature: %.2f °C, Humidity: %.2f %%\n", temperature, humidity);
}
*/
void app_main() {
    ESP_ERROR_CHECK(sht31_init());

    float temp = 0.f;
    float humi = 0.f;

    while(1){
      sht31_read_temp_humi(&temp, &humi);
      printf("Temperature: %.2f °C, Humidity: %.2f %%\n", temp, humi);
      vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}


static i2c_port_t i2c_port = I2C_NUM_1;

esp_err_t sht31_init(void)
{
    i2c_config_t conf = { };
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = SDA_IO_NUM;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = SCL_IO_NUM;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = 10000;
    conf.clk_flags = 0;
    i2c_param_config(i2c_port, &conf);
    return i2c_driver_install(i2c_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}

uint8_t sht31_crc(uint8_t *data) {

    uint8_t crc = 0xff;
    int i, j;
    for(i = 0; i < 2; i++) {
        crc ^= data[i];
        for(j = 0; j < 8; j++) {
            if(crc & 0x80) {
                crc <<= 1;
                crc ^= 0x131;
            }
            else
                crc <<= 1;
        }
    }
    return crc;
}

esp_err_t sht31_read_temp_humi(float *temp, float *humi)
{
    // See http://wiki.seeedstudio.com/Grove-TempAndHumi_Sensor-SHT31/
    // Write 0x00 to address 0x24
    unsigned char data_wr[] = {0x24, 0x00};
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (0x44 << 1) | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write(cmd, data_wr, sizeof(data_wr), ACK_CHECK_EN);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_port, cmd, 1000 / portTICK_PERIOD_MS);
    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            printf("nevalja fail\n");
        }
        else if (ret == ESP_ERR_INVALID_STATE) {
            printf("nevalja state\n");
        }
        else if (ret == ESP_ERR_TIMEOUT) {
            printf("nevalja timeout\n");
        }
        else {
            printf("nevalja nesto\n");
        }

        printf("nevalja 1\n");
        return ret;
    }
    i2c_cmd_link_delete(cmd);

    // Delay 20 ms
    vTaskDelay(2);

    // Read 6 bytes
    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (0x44 << 1) | READ_BIT, ACK_CHECK_EN);
    size_t size = 6;
    uint8_t *data_rd = malloc(size);
    if (size > 1) {
        i2c_master_read(cmd, data_rd, size - 1, ACK_VAL);
    }
    i2c_master_read_byte(cmd, data_rd + size - 1, NACK_VAL);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(i2c_port, cmd, 1000 / portTICK_PERIOD_MS);
    if (ret != ESP_OK) {
        printf("nevalja 2\n");
        return ret;
    }
    i2c_cmd_link_delete(cmd);


    // check error
    if(data_rd[2] != sht31_crc(data_rd) || 
       data_rd[5] != sht31_crc(data_rd + 3)) {
        printf("nevalja 3\n");
        return ESP_ERR_INVALID_CRC;
    }
    
    *temp = -45 + (175 *(float)(data_rd[0] * 256 + data_rd[1])/ 65535.0);
    *humi = 100 * (float)(data_rd[3] * 256 + data_rd[4]) / 65535.0;

    return ESP_OK;

  }

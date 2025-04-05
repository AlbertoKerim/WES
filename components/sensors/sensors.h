#ifndef SENSORS_H
#define SENSORS_H

#include "../gui/squareline/project/ui.h"
#include "../gui/gui.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "veml7700.h"
#include "hc-sr04.h"
#include "sht31.h"
#include "tcrt5000.h"

#define ALS_THRESHOLD 5

#define SENSOR_REPORTING_PERIOD 1

#define SDA_PIN GPIO_NUM_22
#define SCL_PIN GPIO_NUM_21

#define I2C_MASTER_FREQ_HZ 100000
#define I2C_MASTER_NUM 1

void sensors_init(void);

#endif
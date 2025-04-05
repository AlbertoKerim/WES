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

#define SENSOR_REPORTING_PERIOD 1

void sensors_init(void);

#endif
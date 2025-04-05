/**
* @file main.c

* @brief
*
* COPYRIGHT NOTICE: (c) 2025 Byte Lab Grupa d.o.o.
* All rights reserved.
*/

//--------------------------------- INCLUDES ----------------------------------
#include "gui/gui.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "../components/sensors/sensors.h"

//---------------------------------- MACROS -----------------------------------

//-------------------------------- DATA TYPES ---------------------------------

//---------------------- PRIVATE FUNCTION PROTOTYPES --------------------------

//------------------------- STATIC DATA & CONSTANTS ---------------------------

//------------------------------- GLOBAL DATA ---------------------------------

//------------------------------ PUBLIC FUNCTIONS -----------------------------
void app_main(void)
{
    gui_init();

    sensors_init();
}

//---------------------------- PRIVATE FUNCTIONS ------------------------------

//---------------------------- INTERRUPT HANDLERS -----------------------------

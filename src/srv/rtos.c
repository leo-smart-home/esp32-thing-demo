/**
 * @file rtos.c
 * Simple wrapper over FreeRTOS specific functions
 */

#include "rtos.h"

void rtos_createTask(TaskFunction_t function, const char *name, uint32_t stack_depth, BaseType_t priority)
{ 
    xTaskCreate(function, name, stack_depth, NULL, priority, NULL);
}

void rtos_delay(uint32_t delay_ms)
{
    vTaskDelay(delay_ms / portTICK_PERIOD_MS);
}

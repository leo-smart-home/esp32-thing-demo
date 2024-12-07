/**
 * @file rtos.h
 * Simple wrapper over FreeRTOS specific functions
 */

#ifndef RTOS_H
#define RTOS_H

#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void rtos_createTask(TaskFunction_t function, const char *name, uint32_t stack_depth, BaseType_t priority);
void rtos_delay(uint32_t delay_ms);

#endif // RTOS_H
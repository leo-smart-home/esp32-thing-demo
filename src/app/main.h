#ifndef MAIN_H
#define MAIN_H

#include "driver/uart.h"

static void init_uart();

static void task_rgb(void *pvParameter);
static void task_button_control(void *pvParameter);

#endif // MAIN_H
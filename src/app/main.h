#ifndef MAIN_H
#define MAIN_H

#include "driver/uart.h"

static void init_uart();
static void task_publish_button_state(void *pvParameter);

#endif // MAIN_H
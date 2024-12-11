#ifndef MAIN_H
#define MAIN_H

#include "driver/uart.h"
#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"

typedef struct gpioif_pin_config_t
{
    gpio_num_t gpio_num;
    gpio_mode_t mode;
} gpioif_pin_config_t;

static void init_gpio(const gpioif_pin_config_t *pin_config, uint8_t pin_count);
static void init_uart(uart_port_t uart_num, uint16_t baud_rate);

static void task_rgb(void *pvParameter);
static void task_hello_world(void *pvParameter);
static void task_button_control(void *pvParameter);

#endif // MAIN_H
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

static void initGPIO(const gpioif_pin_config_t *pin_config, uint8_t pin_count);
static void initUART(uart_port_t uart_num, uint16_t baud_rate);

static void taskEnableRgb(void *pvParameter);
static void taskPrintHelloWorld(void *pvParameter);
static void taskButtonControl(void *pvParameter);

#endif // MAIN_H
#include "main.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define PIN_BUTTON      GPIO_NUM_34
#define PIN_LED         GPIO_NUM_21
#define UART_DEBUG      UART_NUM_0
#define UART_BAUD_RATE  9600u
#define TASK_STACK_SIZE 2048u

void app_main(void)
{
    const gpioif_pin_config_t pin_config[2u] =
        {
            {PIN_BUTTON, GPIO_MODE_INPUT},
            {PIN_LED, GPIO_MODE_OUTPUT}};

    initGPIO(pin_config, 2u);
    initUART(UART_DEBUG, UART_BAUD_RATE);

    xTaskCreate(&taskPrintHelloWorld, "hello_world_task", TASK_STACK_SIZE, NULL, 5u, NULL);
    xTaskCreate(&taskButtonControl, "button_led_task", TASK_STACK_SIZE, NULL, 1u, NULL);
}

static void taskPrintHelloWorld(void *pvParameter)
{
    while (true)
    {
        printf("Hello from ESP32\n");
        vTaskDelay(pdMS_TO_TICKS(1000u));
    }
}

static void taskButtonControl(void *pvParameter)
{
    while (true)
    {
        uint8_t button_state = (uint8_t)gpio_get_level(PIN_BUTTON);
        gpio_set_level(PIN_LED, button_state);
        vTaskDelay(pdMS_TO_TICKS(50u));
    }
}

static void initUART(uart_port_t uart_num, uint16_t baud_rate)
{
    uart_config_t uart_config = {
        .baud_rate = baud_rate,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE};
    uart_param_config(uart_num, &uart_config);
    uart_driver_install(uart_num, 1024, 0, 0, NULL, 0);
}

static void initGPIO(const gpioif_pin_config_t *pin_config, uint8_t pin_count)
{
    for (uint8_t i = 0u; i < pin_count; i++)
    {
        gpio_set_direction(pin_config[i].gpio_num, pin_config[i].mode);
    }
}

int _write(int file, char *ptr_data, int len)
{
    (void)file; // Not used
    return uart_write_bytes(UART_DEBUG, (char *)ptr_data, (size_t)len);
}

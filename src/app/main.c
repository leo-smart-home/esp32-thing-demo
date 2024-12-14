#include "main.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "nvs_flash.h"

#include "button.h"
#include "rgb_led.h"
#include "task_wifi.h"
#include "task_mqtt.h"

#define TASK_STACK_SIZE 2048u

void app_main(void)
{
    init_uart();
    nvs_flash_init();

    button_init();
    rgb_led_init();

    xTaskCreate(&task_rgb, "rbg_led_task", TASK_STACK_SIZE, NULL, 1u, NULL);
    // xTaskCreate(&task_button_control, "button_led_task", TASK_STACK_SIZE, NULL, 1u, NULL);

    task_wifi_init();
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    task_mqtt_init();
}

static void task_rgb(void *pvParameter)
{
    rgb_led_set_color(255u, 0u, 255u);
    while (true)
    {
        vTaskDelay(pdMS_TO_TICKS(100u));
    }
}

static void task_button_control(void *pvParameter)
{
    while (true)
    {
        printf("Button state: %d\n", (uint8_t)button_get_state());
        vTaskDelay(pdMS_TO_TICKS(100u));
    }
}

static void init_uart()
{
    uart_config_t uart_config =
    {
        .baud_rate = 9600u,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_NUM_0, &uart_config);
    uart_driver_install(UART_NUM_0, 1024, 0, 0, NULL, 0);
}

int _write(int file, char *ptr_data, int len)
{
    (void)file; // Not used
    return uart_write_bytes(UART_NUM_0, (char *)ptr_data, (size_t)len);
}

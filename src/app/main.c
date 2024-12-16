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
    task_wifi_init();
    vTaskDelay(5000 / portTICK_PERIOD_MS); // Wait for WiFi to init completely
    task_mqtt_init();

    xTaskCreate(&task_publish_button_state, "publish_button_state", TASK_STACK_SIZE, NULL, 1u, NULL);
    // task_mqtt_subscribe("/test2", NULL);
}

static void task_publish_button_state(void *arg)
{
    (void)arg; // Not used
    button_state_e button_state;
    char message[32u];

    while (true)
    {
        button_state = button_get_state();
        snprintf(message, sizeof(message), "%s", (button_state == BUTTON_ACTIVE) ? "ACTIVE" : "INACTIVE");
        task_mqtt_publish_message("/esp32/button_state", message);
        vTaskDelay(pdMS_TO_TICKS(50u));
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

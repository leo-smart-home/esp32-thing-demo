#include "main.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "nvs_flash.h"

#include "button.h"
#include "rgb_led.h"
#include "task_wifi.h"
#include "task_mqtt.h"

#define TASK_STACK_SIZE 2048u

static void init_uart();
static void set_rgb_color_callback(const char *data, uint32_t data_len);
static void test_1_callback(const char *data, uint32_t data_len);
static void test_2_callback(const char *data, uint32_t data_len);
static void task_publish_button_state(void *pvParameter);

void app_main(void)
{
    init_uart();
    nvs_flash_init();
    button_init();
    rgb_led_init();
    task_wifi_init();
    vTaskDelay(5000u / portTICK_PERIOD_MS); // Wait for WiFi to init completely

    mqtt_subs_config subs_config = 
    {
        .number_of_subscribers = 3u,
        .map = 
        {
            {.mqtt_topic = "/test_1",               .callback = (subscriber_callback)test_1_callback       },
            {.mqtt_topic = "/test_2",               .callback = (subscriber_callback)test_2_callback       },
            {.mqtt_topic = "/esp32/set_rgb_color",  .callback = (subscriber_callback)set_rgb_color_callback},
        }
    };
    task_mqtt_init(subs_config);
    xTaskCreate(&task_publish_button_state, "publish_button_state", TASK_STACK_SIZE, NULL, 1u, NULL);
}

static void task_publish_button_state(void *arg)
{
    (void)arg; // Not used
    button_state button_state;
    char message[32u];

    while (true)
    {
        button_state = button_get_state();
        snprintf(message, sizeof(message), "%s", (button_state == BUTTON_ACTIVE) ? "ACTIVE" : "INACTIVE");
        task_mqtt_publish_message("/esp32/button_state", message);
        vTaskDelay(pdMS_TO_TICKS(100u));
    }
}

static void set_rgb_color_callback(const char *data, uint32_t data_len)
{
    if (data_len != 6u) {
        printf("Invalid data length\n");
        return;
    }

    char hex_color[7u] = {0u};
    strncpy(hex_color, data, 6u);

    uint32_t rgb_color = (uint32_t)strtol(hex_color, NULL, 16u);

    uint8_t red = (rgb_color >> 16u) & 0xFFu;
    uint8_t green = (rgb_color >> 8u) & 0xFFu;
    uint8_t blue = rgb_color & 0xFFu;

    rgb_led_set_color(red, green, blue);
}

static void test_1_callback(const char *data, uint32_t data_len)
{
    printf("Test 1 callback\n");
}

static void test_2_callback(const char *data, uint32_t data_len)
{
    printf("Test 2 callback\n");
}

static void init_uart()
{
    uart_config_t uart_config =
        {
            .baud_rate = 9600u,
            .data_bits = UART_DATA_8_BITS,
            .parity = UART_PARITY_DISABLE,
            .stop_bits = UART_STOP_BITS_1,
            .flow_ctrl = UART_HW_FLOWCTRL_DISABLE};
    uart_param_config(UART_NUM_0, &uart_config);
    uart_driver_install(UART_NUM_0, 1024u, 0u, 0u, NULL, 0u);
}

int _write(int file, char *ptr_data, int len)
{
    (void)file; // Not used
    return uart_write_bytes(UART_NUM_0, (char *)ptr_data, (size_t)len);
}

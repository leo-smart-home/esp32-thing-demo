#include "main.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_adc/adc_oneshot.h"

#define BLINK_GPIO GPIO_NUM_21 // GPIO_NUM_2
#define DHT_GPIO GPIO_NUM_23

#define UART_BAUD_RATE 9600
#define UART_NUM UART_NUM_0

int uart_printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char buffer[256];
    int len = vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    if (len > 0)
    {
        uart_write_bytes(UART_NUM, buffer, len);
    }
    return len;
}

void blink_task(void *pvParameter)
{
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    while (1)
    {
        gpio_set_level(BLINK_GPIO, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        gpio_set_level(BLINK_GPIO, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void hello_task(void *pvParameter)
{
    uart_config_t uart_config = {
        .baud_rate = UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE};
    uart_param_config(UART_NUM, &uart_config);
    uart_driver_install(UART_NUM, 1024, 0, 0, NULL, 0);

    while (1)
    {
        // uart_printf("Hello world\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void adc_task(void *pvParameter)
{
    adc_oneshot_unit_handle_t adc2_handle;
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT_2,
    };
    adc_oneshot_new_unit(&init_config, &adc2_handle);

    adc_oneshot_chan_cfg_t config = {
        .atten = ADC_ATTEN_DB_0,
        .bitwidth = ADC_BITWIDTH_13,
    };
    adc_oneshot_config_channel(adc2_handle, ADC_CHANNEL_3, &config);

    while (1)
    {
        int adc_reading;
        adc_oneshot_read(adc2_handle, ADC_CHANNEL_3, &adc_reading);
        uart_printf("ADC Reading: %d\n", adc_reading);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }

    adc_oneshot_del_unit(adc2_handle);
}

void button_task(void *pvParameter)
{
    gpio_set_direction(GPIO_NUM_34, GPIO_MODE_INPUT);
    // gpio_pulldown_en(GPIO_NUM_34);

    while (1)
    {
        int button_state = gpio_get_level(GPIO_NUM_34);
        uart_printf("Button state: %s\n", button_state == 0 ? "Pressed" : "Released");
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void app_main()
{
    xTaskCreate(&blink_task, "blink_task", configMINIMAL_STACK_SIZE, NULL, 5, NULL);
    xTaskCreate(&hello_task, "hello_task", configMINIMAL_STACK_SIZE, NULL, 5, NULL);
    xTaskCreate(&adc_task, "adc_task", configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL);
    xTaskCreate(&button_task, "button_task", configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL);
}

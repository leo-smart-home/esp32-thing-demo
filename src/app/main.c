#include "main.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_adc/adc_oneshot.h"

#include "rtos.h"
#include "uartif.h"

#define BLINK_GPIO GPIO_NUM_21 // GPIO_NUM_2
#define DHT_GPIO GPIO_NUM_23

#define UART_BAUDRATE 9600
#define UART_NUM UART_NUM_0

// void adc_task(void *pvParameter)
// {
//     adc_oneshot_unit_handle_t adc2_handle;
//     adc_oneshot_unit_init_cfg_t init_config = {
//         .unit_id = ADC_UNIT_2,
//     };
//     adc_oneshot_new_unit(&init_config, &adc2_handle);

//     adc_oneshot_chan_cfg_t config = {
//         .atten = ADC_ATTEN_DB_0,
//         .bitwidth = ADC_BITWIDTH_13,
//     };
//     adc_oneshot_config_channel(adc2_handle, ADC_CHANNEL_3, &config);

//     while (1)
//     {
//         int adc_reading;
//         adc_oneshot_read(adc2_handle, ADC_CHANNEL_3, &adc_reading);
//         uart_printf("ADC Reading: %d\n", adc_reading);
//         vTaskDelay(500 / portTICK_PERIOD_MS);
//     }

//     adc_oneshot_del_unit(adc2_handle);
// }

void blink_task(void *pvParameter)
{
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    while (1)
    {
        gpio_set_level(BLINK_GPIO, 1);
        rtos_delay(1000);
        gpio_set_level(BLINK_GPIO, 0);
        rtos_delay(1000);
    }
}

void button_task(void *pvParameter)
{
    gpio_set_direction(GPIO_NUM_34, GPIO_MODE_INPUT);

    while (1)
    {
        int button_state = gpio_get_level(GPIO_NUM_34);
        printf("Button state: %s\n", button_state == 0 ? "Pressed" : "Released");
        rtos_delay(100);
    }
}

void app_main()
{
    uartif_init(UART_NUM, UART_BAUDRATE);

    rtos_createTask(&button_task, "button_task", configMINIMAL_STACK_SIZE * 3, 5);
    rtos_createTask(&blink_task, "blink_task", configMINIMAL_STACK_SIZE, 5);
}

int _write(int file, char *ptr_data, int len)
{
    (void)file; // Not used
    return uartif_transmit(UART_NUM, (uint8_t *)ptr_data, (uint16_t)len);
}

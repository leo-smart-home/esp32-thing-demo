#include "main.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "rgb_led.h"

// BLUE - 5
// GREEN - 18
// RED - 21

#define PIN_BUTTON      GPIO_NUM_34
#define PIN_LED_RED     GPIO_NUM_21
#define PIN_LED_GREEN   GPIO_NUM_18
#define PIN_LED_BLUE    GPIO_NUM_5
#define UART_DEBUG      UART_NUM_0
#define UART_BAUD_RATE  9600u
#define TASK_STACK_SIZE 2048u




void app_main(void)
{
    const gpioif_pin_config_t pin_config[] =
        {
            {PIN_BUTTON, GPIO_MODE_INPUT},
            // {PIN_LED_RED, GPIO_MODE_OUTPUT}
        };

    init_gpio(pin_config, sizeof(pin_config) / sizeof(pin_config[0]));
    init_uart(UART_DEBUG, UART_BAUD_RATE);
    rgb_led_init();

    ledc_timer_config_t ledc_timer = {
        .timer_num = LEDC_TIMER_0,
        .duty_resolution = LEDC_TIMER_8_BIT,
        .freq_hz = 500u,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .clk_cfg = LEDC_USE_APB_CLK
    };
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel_0 = {
        .timer_sel = LEDC_TIMER_0,
        .channel = LEDC_CHANNEL_0,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = PIN_LED_RED,
        .duty = 0u,
        .hpoint = 0u
    };
    ledc_channel_config(&ledc_channel_0);

    ledc_channel_config_t ledc_channel_1 = {
        .timer_sel = LEDC_TIMER_0,
        .channel = LEDC_CHANNEL_1,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = PIN_LED_GREEN,
        .duty = 0u,
        .hpoint = 0u
    };
    ledc_channel_config(&ledc_channel_1);

    ledc_channel_config_t ledc_channel_2 = {
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .channel = LEDC_CHANNEL_2,
        .timer_sel = LEDC_TIMER_0,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = PIN_LED_BLUE,
        .duty = 0u,
        .hpoint = 0u
    };
    ledc_channel_config(&ledc_channel_1);

    xTaskCreate(&task_rgb, "rbg_led_task", TASK_STACK_SIZE, NULL, 1u, NULL);
    // xTaskCreate(&task_hello_world, "hello_world_task", TASK_STACK_SIZE, NULL, 5u, NULL);
    // xTaskCreate(&task_button_control, "button_led_task", TASK_STACK_SIZE, NULL, 1u, NULL);
}


static void task_rgb(void *pvParameter)
{
    // // NOTE: Common anode LED, i.e. 0 is full brightness, 255 is off
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, 255);
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, 0);
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_2, 0);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_2);
    // rgb_led_set_color(255u, 0u, 0u);
    while (true)
    {
        // static uint8_t brightness = 0;
        // uint8_t button_state = (uint8_t)gpio_get_level(PIN_BUTTON);
        // if (button_state == 0u) // Active low
        // {
        //     brightness += 10;
        //     ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, brightness);
        //     ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
        // }
        vTaskDelay(pdMS_TO_TICKS(100u));
    }
}


static void task_hello_world(void *pvParameter)
{
    while (true)
    {
        rgb_led_set_color(255u, 255u, 255u);
        // printf("Hello from ESP32\n");
        vTaskDelay(pdMS_TO_TICKS(1000u));
    }
}

static void task_button_control(void *pvParameter)
{
    while (true)
    {
        uint8_t button_state = (uint8_t)gpio_get_level(PIN_BUTTON);
        gpio_set_level(PIN_LED_RED, button_state);
        vTaskDelay(pdMS_TO_TICKS(50u));
    }
}

static void init_uart(uart_port_t uart_num, uint16_t baud_rate)
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

static void init_gpio(const gpioif_pin_config_t *pin_config, uint8_t pin_count)
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

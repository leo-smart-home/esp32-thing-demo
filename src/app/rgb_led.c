/**
 * Simple interface for RGB LED on the breadboard
 */

#include "rgb_led.h"

#include <stdint.h>

#include "driver/gpio.h"
#include "driver/ledc.h"

#define RED_PIN GPIO_NUM_21
#define GREEN_PIN GPIO_NUM_18
#define BLUE_PIN GPIO_NUM_5

typedef struct map_pin_to_channel_t
{
    gpio_num_t pin_number;
    ledc_channel_t channel;
} map_pin_to_channel_t;

void rgb_led_init()
{
    map_pin_to_channel_t map[3u] = 
    {
        {RED_PIN,   LEDC_CHANNEL_0},
        {GREEN_PIN, LEDC_CHANNEL_1},
        {BLUE_PIN,  LEDC_CHANNEL_2},
    };

    ledc_timer_config_t ledc_timer = 
    {
        .timer_num =        LEDC_TIMER_0,
        .duty_resolution =  LEDC_TIMER_8_BIT,
        .freq_hz =          500u,
        .speed_mode =       LEDC_HIGH_SPEED_MODE,
        .clk_cfg =          LEDC_USE_APB_CLK,
    };
    ledc_timer_config(&ledc_timer);

    for (uint8_t i = 0u; i < 3u; i++)
    {
        ledc_channel_config_t ledc_channel = 
        {
            .timer_sel =    LEDC_TIMER_0,
            .channel =      map[i].channel,
            .speed_mode =   LEDC_HIGH_SPEED_MODE,
            .intr_type =    LEDC_INTR_DISABLE,
            .gpio_num =     map[i].pin_number,
            .duty =         0u,
            .hpoint =       0u,
        };
        ledc_channel_config(&ledc_channel);
    }
}

void rgb_led_set_color(uint8_t red, uint8_t green, uint8_t blue)
{
    // Common anode RGB LED
    red = UINT8_MAX - red;
    green = UINT8_MAX - green;
    blue = UINT8_MAX - blue;

    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, red);
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, green);
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_2, blue);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_2);
}
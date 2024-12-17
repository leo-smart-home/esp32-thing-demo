/**
 * Simple interface for one particular button on the breadboard
 */
#include "button.h"
#include "driver/gpio.h"

#define BUTTON_PIN GPIO_NUM_34

void button_init(void)
{
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
}

button_state button_get_state(void)
{
    return (button_state)gpio_get_level(BUTTON_PIN);
}

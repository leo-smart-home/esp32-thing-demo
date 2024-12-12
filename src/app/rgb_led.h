/**
 * Simple interface for RGB LED on the breadboard
 */

#ifndef RGB_LED_H
#define RGB_LED_H

#include <stdint.h>

#include "soc/gpio_num.h"

void rgb_led_init();
void rgb_led_set_color(uint8_t red, uint8_t green, uint8_t blue);

#endif // RGB_LED_H
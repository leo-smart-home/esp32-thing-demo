/**
 * Simple interface for RGB LED on the breadboard
 */

#ifndef RGB_LED_H
#define RGB_LED_H

#include <stdint.h>

#include "soc/gpio_num.h"

#define RGB_LED_SET_COLOR_RED()       rgb_led_set_color(255u, 0u, 0u)
#define RGB_LED_SET_COLOR_GREEN()     rgb_led_set_color(0u, 255u, 0u)
#define RGB_LED_SET_COLOR_BLUE()      rgb_led_set_color(0u, 0u, 255u)
#define RGB_LED_SET_COLOR_YELLOW()    rgb_led_set_color(255u, 255u, 0u)
#define RGB_LED_SET_COLOR_CYAN()      rgb_led_set_color(0u, 255u, 255u)
#define RGB_LED_SET_COLOR_MAGENTA()   rgb_led_set_color(255u, 0u, 255u)
#define RGB_LED_SET_COLOR_WHITE()     rgb_led_set_color(255u, 255u, 255u)

void rgb_led_init();
void rgb_led_set_color(uint8_t red, uint8_t green, uint8_t blue);

#endif // RGB_LED_H
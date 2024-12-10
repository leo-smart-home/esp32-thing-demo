#ifndef MAIN_RGB_LED_H_
#define MAIN_RGB_LED_H_

#include <stdint.h>

#include "driver/ledc.h"

#define RGB_LED_RED_GPIO    21
#define RGB_LED_GREEN_GPIO  22
#define RGB_LED_BLUE_GPIO   23

#define RGB_LED_CHANNEL_NUM 3

typedef struct
{
    int channel;
    int gpio;
    int mode;
    int timer_index;
} ledc_info_t;

void rgb_led_pwm_init(void);
void rgb_led_set_color(uint8_t red, uint8_t green, uint8_t blue);

#endif /* MAIN_RGB_LED_H_ */
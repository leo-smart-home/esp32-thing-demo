/**
 * Simple interface for one particular button on the breadboard
 */

#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>

typedef enum
{
    BUTTON_ACTIVE = 0,
    BUTTON_INACTIVE = 1,
} button_state_e;

void button_init(void);
uint32_t button_get_state(void);

#endif // BUTTON_H
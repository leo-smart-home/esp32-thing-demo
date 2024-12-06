#ifndef UARTIF_H
#define UARTIF_H

#include <stdint.h>

void uartif_init(void);
uint8_t uartif_transmit(uint8_t *ptr_data, uint16_t length);
uint8_t uartif_receive(uint8_t *ptr_data, uint16_t length);


#endif // UARTIF_H
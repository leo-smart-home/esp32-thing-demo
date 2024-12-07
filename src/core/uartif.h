/**
 * @file uartif.h
 * Simple wrapper over platform-specific UART driver
 */

#ifndef UARTIF_H
#define UARTIF_H

#include <stdint.h>
#include "driver/uart.h"

void uartif_init(uart_port_t uart_num, uint16_t baud_rate);
uint8_t uartif_transmit(uart_port_t uart_num, uint8_t *ptr_data, uint16_t length);
uint8_t uartif_receive(uart_port_t uart_num, uint8_t *ptr_data, uint16_t length);

#endif // UARTIF_H
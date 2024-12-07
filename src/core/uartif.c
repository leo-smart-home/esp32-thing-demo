/**
 * @file uartif.c
 * Simple wrapper over platform-specific UART driver
 */

#include "uartif.h"

void uartif_init(uart_port_t uart_num, uint16_t baud_rate)
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

uint8_t uartif_transmit(uart_port_t uart_num, uint8_t *ptr_data, uint16_t length)
{
    int tx_bytes = uart_write_bytes(uart_num, (const char *)ptr_data, length);
    return (tx_bytes == length) ? 0 : 1;
}

uint8_t uartif_receive(uart_port_t uart_num, uint8_t *ptr_data, uint16_t length)
{
    int rx_bytes = uart_read_bytes(uart_num, ptr_data, length, pdMS_TO_TICKS(1000));
    return (rx_bytes > 0) ? 0 : 1;
}

#include "uartif.h"
#include "driver/uart.h"



void uartif_init(void)
{
    uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_NUM_0, &uart_config);
    uart_driver_install(UART_NUM_0, 1024, 0, 0, NULL, 0);
}

uint8_t uartif_transmit(uint8_t *ptr_data, uint16_t length)
{
    int tx_bytes = uart_write_bytes(UART_NUM_0, (const char *)ptr_data, length);
    return (tx_bytes == length) ? 0 : 1;
}

uint8_t uartif_receive(uint8_t *ptr_data, uint16_t length)
{
    int rx_bytes = uart_read_bytes(UART_NUM_0, ptr_data, length, pdMS_TO_TICKS(1000));
    return (rx_bytes > 0) ? 0 : 1;
}

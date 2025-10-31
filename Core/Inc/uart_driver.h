#ifndef UART_DRIVER_IRQ_H
#define UART_DRIVER_IRQ_H

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

// Configuration
#define UART_IRQ_RX_BUFFER_SIZE 64
#define UART_IRQ_TX_BUFFER_SIZE 64

// Error codes
#define UART_IRQ_OK 0
#define UART_IRQ_ERROR -1
#define UART_IRQ_BUFFER_FULL -2
#define UART_IRQ_BUFFER_EMPTY -3

// External handle (must be initialized by main or MX_USARTx_Init)
extern UART_HandleTypeDef huart6;

// Initialization function for the IRQ driver
int8_t uart_irq_init(UART_HandleTypeDef* huart);

// Send functions (non-blocking)
int8_t uart_irq_send_char(char c);
int8_t uart_irq_send_string(const char* str);

// Receive functions (non-blocking)
int8_t uart_irq_receive_char(char* c);
int8_t uart_irq_receive_string(char* str, uint16_t max_len); // Reads until newline or max_len-1

// Process function (call this periodically in main loop)
void uart_irq_process();

// Interrupt handler (called from stm32f4xx_it.c)
void uart_irq_handler(UART_HandleTypeDef *huart);

#endif // UART_DRIVER_IRQ_H

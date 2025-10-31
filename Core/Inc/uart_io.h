#ifndef __UART_IO_H
#define __UART_IO_H

#include "stm32f4xx_hal.h"
#include "usart.h"
#include "uart_driver_irq.h" // Include the new IRQ driver header

// --- NEW: Enum for UART mode ---
typedef enum {
    UART_MODE_POLLING,
    UART_MODE_IRQ
} UartMode_t;
// --- END NEW ---


// --- NEW: Global variable to track current mode ---
static UartMode_t current_uart_mode = UART_MODE_POLLING;
// Function to initialize the selected driver
void init_uart_driver(UartMode_t mode);

// Function to switch the active driver
int8_t switch_to_irq_driver(void);
int8_t switch_to_polling_driver(void);

// Generic non-blocking read function
HAL_StatusTypeDef read_char_nonblocking(char* io_char);

// Generic process function for active driver
void process_active_uart_driver(void);

#endif // __UART_IO_H

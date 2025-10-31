#ifndef UART_DRIVER_IRQ_H
#define UART_DRIVER_IRQ_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"  // Adjust based on your specific STM32 family
#include <stdint.h>
#include <stdbool.h>

// Buffer sizes - can be adjusted as needed
#ifndef UART_IRQ_RX_BUFFER_SIZE
#define UART_IRQ_RX_BUFFER_SIZE 128
#endif

#ifndef UART_IRQ_TX_BUFFER_SIZE
#define UART_IRQ_TX_BUFFER_SIZE 128
#endif

// Return codes
#define UART_IRQ_OK              0
#define UART_IRQ_ERROR          -1
#define UART_IRQ_BUFFER_FULL    -2
#define UART_IRQ_BUFFER_EMPTY   -3

/**
 * @brief Initialize the UART driver with interrupt mode
 * @param huart Pointer to UART handle
 * @return UART_IRQ_OK on success, UART_IRQ_ERROR on failure
 */
int8_t uart_irq_init(UART_HandleTypeDef* huart);

/**
 * @brief Send a single character via UART (interrupt-driven)
 * @param c Character to send
 * @return UART_IRQ_OK on success, UART_IRQ_BUFFER_FULL if buffer full, UART_IRQ_ERROR on other errors
 */
int8_t uart_irq_send_char(char c);

/**
 * @brief Send a null-terminated string via UART (interrupt-driven)
 * @param str String to send
 * @return UART_IRQ_OK on success, error code on failure
 */
int8_t uart_irq_send_string(const char* str);

/**
 * @brief Receive a single character from the RX buffer
 * @param c Pointer to store the received character
 * @return UART_IRQ_OK on success, UART_IRQ_BUFFER_EMPTY if no data available
 */
int8_t uart_irq_receive_char(char* c);

/**
 * @brief Receive a string until newline, buffer full, or no more data
 * @param str Buffer to store the received string
 * @param max_len Maximum length of the buffer (including null terminator)
 * @return UART_IRQ_OK if data received, UART_IRQ_BUFFER_EMPTY if no data, UART_IRQ_ERROR on invalid parameters
 */
int8_t uart_irq_receive_string(char* str, uint16_t max_len);

/**
 * @brief Process housekeeping tasks (should be called periodically in main loop)
 */
void uart_irq_process(void);

/**
 * @brief UART interrupt handler - must be called from the actual IRQ handler
 * @param huart Pointer to UART handle that triggered the interrupt
 */
void uart_irq_handler(UART_HandleTypeDef *huart);

#ifdef __cplusplus
}
#endif

#endif /* UART_DRIVER_IRQ_H */

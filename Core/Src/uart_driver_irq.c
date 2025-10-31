#include "uart_driver_irq.h"
#include <string.h>
#include <stdio.h> // For debug prints if needed
#include "stm32f4xx_hal.h"
#include "usart.h"

static UART_HandleTypeDef* huart_handle = NULL;
static char rx_buffer[UART_IRQ_RX_BUFFER_SIZE];
static volatile uint16_t rx_head = 0;
static volatile uint16_t rx_tail = 0;
static volatile bool rx_overflow = false;

static char tx_buffer[UART_IRQ_TX_BUFFER_SIZE];
static volatile uint16_t tx_head = 0;
static volatile uint16_t tx_tail = 0;
static volatile bool tx_busy = false;

static uint16_t buffer_increment(uint16_t idx, uint16_t size) {
    return (idx + 1) % size;
}

static uint16_t buffer_count(volatile uint16_t head, volatile uint16_t tail, uint16_t size) {
    return (head - tail + size) % size;
}

static bool buffer_is_full(uint16_t head, volatile uint16_t tail, uint16_t size) {
    return buffer_count(buffer_increment(head, size), tail, size) == 0;
}

static bool buffer_is_empty(volatile uint16_t head, volatile uint16_t tail) {
    return head == tail;
}

// --- Public Functions ---

int8_t uart_irq_init(UART_HandleTypeDef* huart) {
    if (!huart) {
        return UART_IRQ_ERROR;
    }
    huart_handle = huart;

    // Initialize buffer indices
    rx_head = 0;
    rx_tail = 0;
    tx_head = 0;
    tx_tail = 0;
    rx_overflow = false;
    tx_busy = false;

    char write_buffer[100];
    sprintf(write_buffer, "Start interation\n");
    HAL_UART_Transmit(&huart6, (uint8_t *) write_buffer, strlen(write_buffer), 100);
    // Start receiving the first byte in interrupt mode
    // This primes the interrupt to fire when the first byte arrives
    if (HAL_UART_Receive_IT(huart_handle, (uint8_t*)&rx_buffer[rx_head], 1) != HAL_OK) {
        return UART_IRQ_ERROR;
    }

    return UART_IRQ_OK;
}

int8_t uart_irq_send_char(char c) {

    if (!huart_handle || buffer_is_full(tx_head, tx_tail, UART_IRQ_TX_BUFFER_SIZE)) {
        return UART_IRQ_BUFFER_FULL;
    }
    

    tx_buffer[tx_head] = c;
    tx_head = buffer_increment(tx_head, UART_IRQ_TX_BUFFER_SIZE);

    // If transmission is not ongoing, start it
    if (!tx_busy) {
        tx_busy = true;
        if (HAL_UART_Transmit_IT(huart_handle, (uint8_t*)&tx_buffer[tx_tail], 1) != HAL_OK) {
            tx_busy = false; // Reset flag on error
            return UART_IRQ_ERROR;
        }
    }
    return UART_IRQ_OK;
}

int8_t uart_irq_send_string(const char* str) {
    if (!str) return UART_IRQ_ERROR;

    int8_t res;
    while (*str) {
        res = uart_irq_send_char(*str++);
        if (res != UART_IRQ_OK) {
            return res; // Return error code if buffer full or other error
        }
    }
    return UART_IRQ_OK;
}

int8_t uart_irq_receive_char(char* c) {
    if (!c || buffer_is_empty(rx_head, rx_tail)) {
        return UART_IRQ_BUFFER_EMPTY;
    }

    *c = rx_buffer[rx_tail];
    rx_tail = buffer_increment(rx_tail, UART_IRQ_RX_BUFFER_SIZE);
    return UART_IRQ_OK;
}

int8_t uart_irq_receive_string(char* str, uint16_t max_len) {
    if (!str || max_len == 0) return UART_IRQ_ERROR;

    uint16_t i = 0;
    int8_t res;
    char ch;

    while (i < max_len - 1) { // Leave space for null terminator
        res = uart_irq_receive_char(&ch);
        if (res == UART_IRQ_OK) {
            str[i++] = ch;
            if (ch == '\n' || ch == '\r') { // End on newline or carriage return
                break;
            }
        } else if (res == UART_IRQ_BUFFER_EMPTY) {
            // No more characters available right now
            break;
        } else {
            // Should not happen with uart_irq_receive_char
            return res;
        }
    }
    str[i] = '\0'; // Null terminate
    return (i > 0) ? UART_IRQ_OK : UART_IRQ_BUFFER_EMPTY; // Return OK if at least one char read, else EMPTY
}

void uart_irq_process() {
    // This function can be used for housekeeping tasks if needed.
    // For this simple implementation, the interrupt handler manages the state.
    // We could potentially restart reception here if it was stopped due to overflow,
    // but for now, we'll just ensure reception continues after each byte.
    // The reception is automatically restarted in the interrupt handler.
    // If transmission was started, it's handled by the interrupt handler.
    // So, for now, this function might be empty or just ensure flags are consistent.
    // It's called from the main loop.
    // Potentially check for overflow and notify user if needed.
    if (rx_overflow) {
        // Could print a warning or set a flag for main to handle
        // printf("UART IRQ RX Overflow!\n"); // Requires semihosting or RTT
        rx_overflow = false; // Reset overflow flag after handling
    }
}

void uart_irq_handler(UART_HandleTypeDef *huart) {

    char write_buffer[100];
    sprintf(write_buffer, "Got interation\n");
    HAL_UART_Transmit(&huart6, (uint8_t *) write_buffer, strlen(write_buffer), 100);


    if (huart != huart_handle) {
        // This interrupt is not for our driver instance
        return;
    }

    // Handle Receive Interrupt
    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_RXNE) && __HAL_UART_GET_IT_SOURCE(huart, UART_IT_RXNE)) {
        char received_char = (char)huart->Instance->DR; // Read data register

        uint16_t next_head = buffer_increment(rx_head, UART_IRQ_RX_BUFFER_SIZE);
        if (next_head == rx_tail) {
            // Buffer is full, set overflow flag
            rx_overflow = true;
            // Optionally, drop the oldest character to make space
            // rx_tail = buffer_increment(rx_tail, UART_IRQ_RX_BUFFER_SIZE);
        } else {
            rx_buffer[rx_head] = received_char;
            rx_head = next_head;
        }

        // Restart reception for the next byte
        if (HAL_UART_Receive_IT(huart_handle, (uint8_t*)&rx_buffer[rx_head], 1) != HAL_OK) {
            // Error restarting reception - handle if necessary
            // Could set an error flag
        }
    }

    // Handle Transmit Interrupt
    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_TC) && __HAL_UART_GET_IT_SOURCE(huart, UART_IT_TC)) {
        // Transmission Complete
        if (!buffer_is_empty(tx_head, tx_tail)) {
            // There are more bytes to send
            if (HAL_UART_Transmit_IT(huart_handle, (uint8_t*)&tx_buffer[tx_tail], 1) == HAL_OK) {
                tx_tail = buffer_increment(tx_tail, UART_IRQ_TX_BUFFER_SIZE);
            } else {
                 // Error starting next transmission - handle if necessary
                 tx_busy = false; // Reset busy flag on error
            }
        } else {
            // No more bytes to send, transmission is complete
            tx_busy = false;
            // Disable TC interrupt to prevent repeated triggering if desired
            // __HAL_UART_DISABLE_IT(huart, UART_IT_TC);
        }
    }

    // Handle Error Interrupts (Parity, Noise, Framing, Overrun)
    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_PE) || __HAL_UART_GET_FLAG(huart, UART_FLAG_NE) ||
        __HAL_UART_GET_FLAG(huart, UART_FLAG_FE) || __HAL_UART_GET_FLAG(huart, UART_FLAG_ORE)) {
        // Clear error flags
        __HAL_UART_CLEAR_PEFLAG(huart);
        // Optionally, handle the error (reset buffers, log error)
        // For now, just clear the flags and continue
    }
}

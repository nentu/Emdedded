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

char write_buffer[200];

// --- Helper Functions ---
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

    // Start receiving the first byte in interrupt mode
    // This primes the interrupt to fire when the first byte arrives
    // --- IMPROVED: Check return value ---
    if (HAL_UART_Receive_IT(huart_handle, (uint8_t*)&rx_buffer[rx_head], 1) != HAL_OK) {
        // If starting the first receive fails, the driver cannot function correctly
        // This might happen if the HAL state is not ready or if interrupts are disabled
        // at the NVIC level for the UART instance (though they should be enabled via CubeMX).
        // It could also happen if the UART peripheral itself is in an error state.
        // For now, just return error.
        return UART_IRQ_ERROR;
    }

    return UART_IRQ_OK;
}

int8_t uart_irq_send_char(char c) {

    if (!huart_handle || buffer_is_full(tx_head, tx_tail, UART_IRQ_TX_BUFFER_SIZE)) {
        return UART_IRQ_BUFFER_FULL;
    }
    
    HAL_UART_Transmit( &huart6, "sending char", strlen("sending char"), 200);

    tx_buffer[tx_head] = c;
    tx_head = buffer_increment(tx_head, UART_IRQ_TX_BUFFER_SIZE);
    HAL_UART_Transmit( &huart6, "buffer_increment", strlen("buffer_increment"), 200);

    // If transmission is not ongoing, start it
    if (!tx_busy) {
        tx_busy = true;
        HAL_UART_Transmit( &huart6, "buffer free: ", strlen("buffer free: "), 200);
        sprintf(write_buffer, "\n tx_head: %d,tx_tail: %d, tx_busy: %d, char: '%c'\n", tx_head, tx_tail, tx_busy, c );
        HAL_UART_Transmit( &huart6, write_buffer, strlen(write_buffer), 200);

        HAL_StatusTypeDef res = HAL_UART_Transmit_IT(&huart6, (uint8_t*) &c, 1);
        if (res == HAL_ERROR){
            HAL_UART_Transmit( &huart6, "HAL_ERROR", strlen("HAL_ERROR"), 200);
        } else if (res == HAL_BUSY){
            HAL_UART_Transmit( &huart6, "HAL_BUSY", strlen("HAL_BUSY"), 200);
        } else if (res == HAL_TIMEOUT){
            HAL_UART_Transmit( &huart6, "HAL_TIMEOUT", strlen("HAL_TIMEOUT"), 200);
        } 
        if (res != HAL_OK) {
            tx_busy = false; // Reset flag on error

            uint16_t start_tx_index = tx_tail; // Capture the index of the character to send NOW

            if (HAL_UART_Transmit_IT(huart_handle, (uint8_t*)&tx_buffer[start_tx_index], 1) != HAL_OK) {
                tx_busy = false; // Reset flag on error
                return UART_IRQ_ERROR;
            }
        }
        HAL_UART_Transmit( &huart6, "send ok", strlen("send ok"), 200);

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

        // --- IMPROVED: Restart reception for the next byte and check return value ---
        if (HAL_UART_Receive_IT(huart_handle, (uint8_t*)&rx_buffer[rx_head], 1) != HAL_OK) {
            // Error restarting reception - handle if necessary
            // Could set an error flag, log error, or try to recover
            // For now, we just don't restart, which means reception stops.
            // This is a critical error state.
            // It might be better to attempt a reset of the UART handle or flag the error globally.
            // For this implementation, let's assume a critical failure and stop trying to restart.
            // A more robust system might have a watchdog or reset mechanism here.
            // For now, log the error (if logging is available) or set an internal error flag.
            // This situation indicates a problem with the UART HAL state or configuration.
            // The driver is now in a non-functional state for receiving.
            // A real system might need to switch back to polling or reset the UART.
            // For simulation purposes, we'll just stop trying to restart.
            // In a real system, you'd want to handle this more gracefully.
            // Example: static bool rx_error = false; rx_error = true;
            // Then, read_char could return an error if rx_error is set.
        }
        // --- END IMPROVED ---

    }

    // Handle Transmit Interrupt
    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_TC) && __HAL_UART_GET_IT_SOURCE(huart, UART_IT_TC)) {
        // Transmission Complete for the *current* byte
        // The byte that was pointed to by tx_tail when HAL_UART_Transmit_IT was called is now complete.
        // So, we must advance tx_tail now to point to the next byte to be sent.
        tx_tail = buffer_increment(tx_tail, UART_IRQ_TX_BUFFER_SIZE); // NEW: Move tail forward FIRST

        if (!buffer_is_empty(tx_head, tx_tail)) { // Are there more bytes in our buffer?
            // Yes, there are more bytes to send
            // tx_tail now points to the next byte to send
            if (HAL_UART_Transmit_IT(huart_handle, (uint8_t*)&tx_buffer[tx_tail], 1) == HAL_OK) {
                // HAL_UART_Transmit_IT scheduled for the next byte
                // tx_tail already points to the byte scheduled
            } else {
                 // Error starting *next* transmission
                 tx_busy = false; // Reset busy flag on error - CRITICAL
                 // Could set an error flag or log the error
                 // The transmission chain is broken.
            }
        } else {
            // No more bytes to send in our buffer
            tx_busy = false; // Correctly reset the busy flag
            // Disable TC interrupt if desired (not done here)
        }
    }

    // Handle Error Interrupts (Parity, Noise, Framing, Overrun)
    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_PE) || __HAL_UART_GET_FLAG(huart, UART_FLAG_NE) ||
        __HAL_UART_GET_FLAG(huart, UART_FLAG_FE) || __HAL_UART_GET_FLAG(huart, UART_FLAG_ORE)) {
        // Clear error flags
        __HAL_UART_CLEAR_PEFLAG(huart);
        // Optionally, handle the error (e.g., reset buffers, log error)
        // For now, just clear the flags and continue
        // Could set an error flag indicating a reception error occurred
    }
}

#include "uart_io.h"
#include <string.h> // For strlen if needed elsewhere

void init_uart_driver(UartMode_t mode) {
    if (mode == UART_MODE_IRQ) {
        if (uart_irq_init(&huart6) == UART_IRQ_OK) {
            current_uart_mode = UART_MODE_IRQ;
        } else {
            // Fallback or error handling
            // For now, stay in polling mode if IRQ init fails
            current_uart_mode = UART_MODE_POLLING;
        }
    } else { // Assume Polling mode
        // Hardware initialization is already done by MX_USART6_UART_Init
        // No specific init needed for polling besides hardware setup
        current_uart_mode = UART_MODE_POLLING;
    }
}

int8_t switch_to_irq_driver(void) {
    if (current_uart_mode == UART_MODE_IRQ) {
        return 0; // Already in IRQ mode
    }
    // Deinitialize polling mode resources if any (HAL handles hardware state)
    // Initialize IRQ mode
    if (uart_irq_init(&huart6) == UART_IRQ_OK) {
        current_uart_mode = UART_MODE_IRQ;
        return 0; // Success
    }
    return -1; // Failure
}

int8_t switch_to_polling_driver(void) {
    if (current_uart_mode == UART_MODE_POLLING) {
        return 0; // Already in Polling mode
    }
    // Deinitialize IRQ mode resources (stop ongoing transfers, clear buffers)
    // This is tricky with HAL. We need to ensure no IT transfers are pending.
    // Let's assume deinit/init of HAL handle or just stop IT transfers.
    HAL_UART_Abort_IT(&huart6); // Abort any ongoing IT transfers
    // Reset IRQ driver state variables (if any external state needs reset)
    // Reinitialize hardware for polling (it's already initialized)
    current_uart_mode = UART_MODE_POLLING;
    return 0; // Success
}

HAL_StatusTypeDef read_char_nonblocking(char* io_char) {

    if (!io_char) return HAL_ERROR;

    if (current_uart_mode == UART_MODE_POLLING) {
        // Use the original polling method, but make it truly non-blocking
        // HAL_UART_Receive with 0 timeout returns immediately if no data
        return HAL_UART_Receive(&huart6, (uint8_t *) io_char, 1, 0);
    } else { // UART_MODE_IRQ
        // Use the IRQ driver's buffer check
        int8_t res = uart_irq_receive_char(io_char);
        if (res == UART_IRQ_OK) {
            return HAL_OK; // Map success
        } else if (res == UART_IRQ_BUFFER_EMPTY) {
            return HAL_TIMEOUT; // Map empty buffer to timeout for consistency
        } else {
            return HAL_ERROR; // Map other errors
        }
    }
}

void process_active_uart_driver(void) {
    if (current_uart_mode == UART_MODE_IRQ) {
        uart_irq_process();
    }
    // Polling mode typically doesn't need processing in the main loop
    // as it handles everything synchronously during calls.
}

// --- OLD FUNCTION (can be kept if needed for other parts, but main uses read_char_nonblocking now) ---
// HAL_StatusTypeDef read_char(char* io_char){
//     return HAL_UART_Receive(&huart6, (uint8_t *) io_char, 1, 1000); // Blocking
// }
// --- END OLD FUNCTION ---

#include "uart_io.h"
#include <string.h> // For strlen if needed elsewhere

// --- NEW: Define current_uart_mode globally (not static) ---
UartMode_t current_uart_mode = UART_MODE_POLLING;
// --- END NEW ---

void init_uart_driver(UartMode_t mode) {
    if (mode == UART_MODE_IRQ) {
        // Ensure no ongoing IT operations before initializing IRQ driver
        HAL_UART_Abort_IT(&huart6);
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
    // Abort any ongoing IT transfers that might have been started by polling driver accidentally
    // This is crucial before initializing the IRQ driver
    HAL_UART_Abort_IT(&huart6);

    // Wait for the driver to become READY. This is crucial.
//    uint32_t start_tick = HAL_GetTick();
//    while (huart6.gState != HAL_UART_STATE_READY) {
//        // Add a timeout to prevent infinite loop in case of hardware error
//        if ((HAL_GetTick() - start_tick) > 100) { // 100 ms timeout
//            // If it doesn't become ready, something is wrong.
//            // For now, we'll force the state to READY as a last resort.
//            // This is a bit of a hack but can recover from a stuck state.
//            // A more robust system would handle this error properly.
//            huart6.gState = HAL_UART_STATE_READY;
//            break;
//        }
//    }
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
    // Let the IRQ driver handle clearing its own buffers via its process function if needed
    // Abort any ongoing IT transfers started by the IRQ driver
    HAL_UART_Abort_IT(&huart6); // Stop ongoing IT transfers
    // Reinitialize hardware for polling (it's already initialized)
    current_uart_mode = UART_MODE_POLLING;
    return 0; // Success
}

HAL_StatusTypeDef read_char(char* io_char) {

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


HAL_StatusTypeDef write_string_len(const char* string, size_t str_len) {

    if (!string) return HAL_ERROR;

    if (current_uart_mode == UART_MODE_POLLING) {
        // Use the original polling method, but make it truly non-blocking
        // HAL_UART_Receive with 0 timeout returns immediately if no data
        HAL_UART_Transmit( &huart6, (uint8_t *) string, str_len, 200 );

    } else { // UART_MODE_IRQ
        // Use the IRQ driver's buffer check

        uart_irq_send_string(string, str_len);
    }
}


HAL_StatusTypeDef write_string(const char* string) {
    write_string_len(string, strlen(string));
}



//void process_active_uart_driver(void) {
//    if (current_uart_mode == UART_MODE_IRQ) {
//        uart_irq_process();
//    }
//    // Polling mode typically doesn't need processing in the main loop
//    // as it handles everything synchronously during calls.
//}

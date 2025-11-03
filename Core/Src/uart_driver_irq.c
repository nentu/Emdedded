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

static volatile int uart6_sending_ongoing;

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
    uart6_sending_ongoing = 0;

    if (HAL_UART_Receive_IT(huart_handle, (uint8_t*)&rx_buffer[rx_head], 1) != HAL_OK) {
        return UART_IRQ_ERROR;
    }

    return UART_IRQ_OK;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart == huart_handle) {
        // Process the byte that was just received into rx_buffer[rx_head]

        // Move to next buffer position
        rx_head = buffer_increment(rx_head, UART_IRQ_RX_BUFFER_SIZE);

        // Check for buffer overflow
        if (rx_head == rx_tail) {
            rx_overflow = true;
            // Handle overflow - maybe move tail forward?
            rx_tail = buffer_increment(rx_tail, UART_IRQ_RX_BUFFER_SIZE);
        }

        // RESTART reception for next byte - this keeps the chain going
        HAL_UART_Receive_IT(huart_handle, (uint8_t*)&rx_buffer[rx_head], 1);
    }
}

static inline bool uart_ready_for_transmit() {
    return (uart6_sending_ongoing == 0);
}



void uart_irq_send_string(const char* str, uint16_t str_len) {
    while (!uart_ready_for_transmit()){
//    	write_string("Not ready\n");
    }
    uart6_sending_ongoing = 1;
	HAL_UART_Transmit_IT(&huart6, str, str_len);
	return 1;
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart == huart_handle) {
		uart6_sending_ongoing = 0;
	}
}


int8_t uart_irq_receive_char(char* c) {
    if (!c || buffer_is_empty(rx_head, rx_tail)) {
        return UART_IRQ_BUFFER_EMPTY;
    }

    *c = rx_buffer[rx_tail];
    rx_tail = buffer_increment(rx_tail, UART_IRQ_RX_BUFFER_SIZE);
    return UART_IRQ_OK;
}

#include "uart_driver_irq.h"
#include <string.h>
#include <stdio.h> // For debug prints if needed
#include "stm32f4xx_hal.h"
#include "usart.h"

static UART_HandleTypeDef* huart_handle = NULL;

// Receive buffer
static char rx_buffer[UART_IRQ_RX_BUFFER_SIZE];
static volatile uint16_t rx_head = 0;
static volatile uint16_t rx_tail = 0;
static volatile bool rx_overflow = false;

// Transmit buffer
static char tx_buffer[UART_IRQ_TX_BUFFER_SIZE];
static volatile uint16_t tx_head = 0;
static volatile uint16_t tx_tail = 0;
static volatile bool tx_busy = false;

// --- Helper Functions ---
static uint16_t buffer_increment(uint16_t idx, uint16_t size) {
    return (idx + 1) % size;
}

static uint16_t buffer_count(volatile uint16_t head, volatile uint16_t tail, uint16_t size) {
    return (head - tail + size) % size;
}

static bool buffer_is_full(uint16_t head, volatile uint16_t tail, uint16_t size) {
    return buffer_increment(head, size) == tail;
}

static bool buffer_is_empty(volatile uint16_t head, volatile uint16_t tail) {
    return head == tail;
}

// --- Private Functions ---
static void uart_start_transmission(void) {
    if (!tx_busy && !buffer_is_empty(tx_head, tx_tail)) {
        // Get the next character to transmit
        char c = tx_buffer[tx_tail];
        tx_tail = buffer_increment(tx_tail, UART_IRQ_TX_BUFFER_SIZE);
        
        if (HAL_UART_Transmit_IT(huart_handle, (uint8_t*)&c, 1) == HAL_OK) {
            tx_busy = true;
        }
    }
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

    if (HAL_UART_Receive_IT(huart_handle, (uint8_t*)&rx_buffer[rx_head], 1) != HAL_OK) {
        return UART_IRQ_ERROR;
    }

    return UART_IRQ_OK;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart == huart_handle) {
        rx_head = buffer_increment(rx_head, UART_IRQ_RX_BUFFER_SIZE);

        if (rx_head == rx_tail) {
            rx_overflow = true;
        }

        HAL_UART_Receive_IT(huart_handle, (uint8_t*)&rx_buffer[rx_head], 1);
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart == huart_handle) {
        tx_busy = false;
        // Check if there's more data to send
        uart_start_transmission();
    }
}

int8_t uart_irq_send_char(char c) {
    if (buffer_is_full(tx_head, tx_tail, UART_IRQ_TX_BUFFER_SIZE)) {
        return UART_IRQ_BUFFER_FULL;
    }

    tx_buffer[tx_head] = c;
    tx_head = buffer_increment(tx_head, UART_IRQ_TX_BUFFER_SIZE);
    
    // Start transmission if not already in progress
    uart_start_transmission();
    
    return UART_IRQ_OK;
}

void uart_irq_send_string(const char* str, uint16_t str_len) {


    for (uint16_t i = 0; i < str_len; i++) {
        int8_t result = uart_irq_send_char(str[i]);
        if (result != UART_IRQ_OK) {
            return result; // Return error if buffer full
        }
    }
    
}

void uart_irq_send_data(const uint8_t* data, uint16_t data_len) {
    uart_irq_send_string((const char*)data, data_len);
}

int8_t uart_irq_receive_char(char* c) {
    if (!c || buffer_is_empty(rx_head, rx_tail)) {
        return UART_IRQ_BUFFER_EMPTY;
    }

    *c = rx_buffer[rx_tail];
    rx_tail = buffer_increment(rx_tail, UART_IRQ_RX_BUFFER_SIZE);
    rx_overflow = false; // Clear overflow flag once we read data
    return UART_IRQ_OK;
}

uint16_t uart_irq_get_rx_count(void) {
    return buffer_count(rx_head, rx_tail, UART_IRQ_RX_BUFFER_SIZE);
}

uint16_t uart_irq_get_tx_count(void) {
    return buffer_count(tx_head, tx_tail, UART_IRQ_TX_BUFFER_SIZE);
}

bool uart_irq_is_tx_busy(void) {
    return tx_busy;
}

bool uart_irq_is_rx_overflow(void) {
    return rx_overflow;
}

void uart_irq_flush_tx_buffer(void) {
    tx_head = 0;
    tx_tail = 0;
    tx_busy = false;
}

void uart_irq_flush_rx_buffer(void) {
    rx_head = 0;
    rx_tail = 0;
    rx_overflow = false;
}

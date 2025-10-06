#ifndef __UART_IO_H
#define __UART_IO_H
#include "stm32f4xx_hal.h"
#include "usart.h"


HAL_StatusTypeDef read_char(char* io_char);

#endif

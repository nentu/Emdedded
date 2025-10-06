#include "uart_io.h"


HAL_StatusTypeDef read_char(char* io_char){
	return HAL_UART_Receive(&huart6, io_char, 1, 10);
}

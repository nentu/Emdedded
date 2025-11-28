/*
 * keyboard_driver.c
 *
 *  Created on: Nov 28, 2025
 *      Author: zam12
 */


#include "keyboard_driver.h"
#include <stdint.h>
#include "i2c.h"


void kb_write_reg(I2C_HandleTypeDef * i2c, uint16_t MemAddress, uint8_t data) {
    HAL_I2C_Mem_Write(i2c, KEYBOARD_DEVICE, MemAddress, 1, &data, 1, 100);
}

void kb_read_reg(I2C_HandleTypeDef * i2c, uint16_t MemAddress, uint8_t* data) {
    HAL_I2C_Mem_Read(i2c, KEYBOARD_DEVICE, MemAddress, 1, data, 1, 100);
}


void kb_write_config(I2C_HandleTypeDef * i2c, uint8_t data) {
	kb_write_reg(i2c, KB_CONFIG_REG, data);
}

void kb_select_row(I2C_HandleTypeDef * i2c, uint8_t row) {
	kb_write_config(i2c, ~((uint8_t) (1 << row)));
}


void kb_read_row(I2C_HandleTypeDef * i2c, uint8_t row, uint8_t *res){
	kb_select_row(i2c, row);
	kb_write_reg(i2c, KB_OUTPUT_REG, (uint8_t) (1 << row));
	kb_read_reg(i2c, KB_INPUT_REG, res);
}

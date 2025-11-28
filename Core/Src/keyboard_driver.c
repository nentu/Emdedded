/*
 * keyboard_driver.c
 *
 *  Created on: Nov 28, 2025
 *      Author: zam12
 */


#include "keyboard_driver.h"
#include <stdint.h>
//#include "i2c.h"

HAL_StatusTypeDef kb_write_reg(I2C_HandleTypeDef * i2c, uint16_t MemAddress, uint8_t data) {
    return HAL_I2C_Mem_Write(i2c, KEYBOARD_DEVICE & 0xfffe, MemAddress, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
}

void kb_init(I2C_HandleTypeDef * i2c){

	kb_write_reg(i2c, KB_CONFIG_REG, 0b1110000);
	kb_write_reg(i2c, KB_POLARITY_REG, 0);
	kb_write_reg(i2c, KB_OUTPUT_REG, 0xf);
}




HAL_StatusTypeDef kb_read_reg(I2C_HandleTypeDef * i2c, uint16_t MemAddress, uint8_t* data) {
    return HAL_I2C_Mem_Read(i2c, KEYBOARD_DEVICE | 1, MemAddress, I2C_MEMADD_SIZE_8BIT, data, 1, 10);
}

void kb_select_row(I2C_HandleTypeDef * i2c, uint8_t row) {
	uint8_t bin_mask = (uint8_t) (1 << row);
//	kb_write_reg(i2c, KB_CONFIG_REG, ~bin_mask);
	kb_write_reg(i2c, KB_OUTPUT_REG, ~bin_mask);
}


void kb_read_row(I2C_HandleTypeDef * i2c, uint8_t row, uint8_t *res){
	kb_select_row(i2c, row);
	HAL_Delay(1);
	kb_read_reg(i2c, KB_INPUT_REG, res);
}



void kb_get_status(I2C_HandleTypeDef * i2c, uint8_t** kb_matrix){
	uint8_t row_reg;

	for (int row_i=0; row_i<4; row_i++){
		kb_read_row(i2c, row_i, &row_reg);
		for (int col_i=0; col_i<3; col_i++){
			kb_matrix[row_i][col_i] = (row_reg >> (col_i + 4)) & 1;
		}
	}
}

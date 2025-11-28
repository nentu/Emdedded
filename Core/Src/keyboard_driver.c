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
			kb_matrix[row_i][col_i] = 1 - ((row_reg >> (col_i + 4)) & 1);
		}
	}
}

uint8_t** createMatrix(int rows, int cols) {
	  uint8_t** matrix = malloc(rows * sizeof(uint8_t*));
    for (int i = 0; i < rows; i++) {
        matrix[i] = malloc(cols * sizeof(uint8_t));
    }
    return matrix;
}

uint8_t** prev_kb_state;
uint8_t** cur_kb_state;


int read_keyboard(I2C_HandleTypeDef * i2c){
    static int initialized = 0;

    // Initialize previous state to all zeros on first call
    if (!initialized) {
		prev_kb_state = createMatrix(4, 3);
		cur_kb_state = createMatrix(4, 3);
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 3; j++) {
                prev_kb_state[i][j] = 0;
            }
        }
        initialized = 1;
    }

    // 1) Get current keyboard state
    kb_get_status(i2c, cur_kb_state);

    int pressed_key = -1;  // -1 means no key or multiple keys pressed
    int press_count = 0;

    // 2) Compare with previous state
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 3; col++) {
            // 3) Check if key was 0 and became 1
            if (prev_kb_state[row][col] == 0 && cur_kb_state[row][col] == 1) {
                press_count++;
                pressed_key = row * 3 + col; // Calculate index: 0-11
            }
        }
    }

    // Update previous state for next call
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            prev_kb_state[i][j] = cur_kb_state[i][j];
        }
    }

    // Return key index only if exactly one key was pressed
    return (press_count == 1) ? pressed_key : -1;
}

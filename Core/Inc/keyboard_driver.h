/*
 * keyboard.h
 *
 *  Created on: Nov 28, 2025
 *      Author: zam12
 */

#ifndef INC_KEYBOARD_H_
#define INC_KEYBOARD_H_

#define KEYBOARD_DEVICE 0xE2

#define KB_INPUT_REG 0x0
#define KB_OUTPUT_REG 0x1
#define KB_POLARITY_REG 0x2
#define KB_CONFIG_REG 0x3


#include "i2c.h"

/*
btn_id: 
- -1 - not clicked
- 0-11 btn id which is clicked
*/
int read_keyboard();
void kb_read_row(I2C_HandleTypeDef * i2c, uint8_t row, uint8_t *res);
void kb_init(I2C_HandleTypeDef * i2c);
#endif /* INC_KEYBOARD_H_ */

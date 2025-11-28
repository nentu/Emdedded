/*
 * keyboard.h
 *
 *  Created on: Nov 28, 2025
 *      Author: zam12
 */

#ifndef INC_KEYBOARD_H_
#define INC_KEYBOARD_H_

#define KEYBOARD_ADDRESS 0xE2


/*
btn_id: 
- -1 - not clicked
- 0-11 btn id which is clicked
*/
void read_keyboard(int &btn_id);

#endif /* INC_KEYBOARD_H_ */

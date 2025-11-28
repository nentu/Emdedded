/*
 * keyboard_logic.h
 *
 *  Created on: Nov 28, 2025
 *      Author: zam12
 */

#ifndef INC_KEYBOARD_LOGIC_H_
#define INC_KEYBOARD_LOGIC_H_

#include "keyboard_driver.h"

enum ProgramState {
    TESTING,
    SETUP,
    WORK,
    STATE,
    COLOR,
    POWER
};

void keyboard(int btn_id, int mode);


#endif /* INC_KEYBOARD_LOGIC_H_ */

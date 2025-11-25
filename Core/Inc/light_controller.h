/*
 * light_controller.h
 *
 *  Created on: Nov 25, 2025
 *      Author: zam12
 */

#ifndef INC_LIGHT_CONTROLLER_H_
#define INC_LIGHT_CONTROLLER_H_

#include "tim.h"

enum BtnColor{
	GREEN=0, YELLOW=1, RED=2
};
#define MAX_POWER 100
int set_light_power(enum BtnColor btn, int power);
void clear_lights();

#endif /* INC_LIGHT_CONTROLLER_H_ */

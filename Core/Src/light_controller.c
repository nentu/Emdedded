/*
 * light_controller.c
 *
 *  Created on: Nov 25, 2025
 *      Author: zam12
 */


#include "light_controller.h"

int set_light_power(enum BtnColor btn, int power){
	if ((power < 0) || (power >= MAX_POWER))
		return 1;

	if (btn == GREEN){
    	htim4.Instance->CCR2 = (power * 5);
	}
	if (btn == RED){
    	htim4.Instance->CCR3 = (power * 5);
	}
	if (btn == YELLOW){
    	htim4.Instance->CCR4 = (power * 5);
	}
	return 0;
}

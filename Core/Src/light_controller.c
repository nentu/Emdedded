/*
 * light_controller.c
 *
 *  Created on: Nov 25, 2025
 *      Author: zam12
 */


#include "light_controller.h"

int set_light_power(enum BtnColor btn, int power){
	if ((power < 0) || (power >= 99))
		return 1;

	if (btn == GREEN){
    	htim4.Instance->CCR2 = (power * 10);
	}
	if (btn == YELLOW){
    	htim4.Instance->CCR3 = (power * 10);
	}
	if (btn == RED){
    	htim4.Instance->CCR4 = (power * 10);
	}
	return 0;
}

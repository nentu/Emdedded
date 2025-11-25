/*
 * light_controller.c
 *
 *  Created on: Nov 25, 2025
 *      Author: zam12
 */


#include "light_controller.h"
#include "tim.h"

int set_light_power(enum BtnColor btn, int power){
	if (power == MAX_POWER) power --;
	if ((power < 0) || (power >= MAX_POWER))
		return 1;
	uint32_t green_pow = 0, yellow_pow = 0, red_pow = 0;
	

	if (btn == GREEN){
    	green_pow = (power * 5);
	}
	if (btn == RED){
    	yellow_pow = (power * 5);
	}
	if (btn == YELLOW){
    	red_pow = (power * 5);
	}

	htim4.Instance->CCR2 = green_pow;
	htim4.Instance->CCR3 = red_pow;
	htim4.Instance->CCR4 = yellow_pow;
	return 0;
}

void clear_lights(){
	htim4.Instance->CCR2 = 0;
	htim4.Instance->CCR3 = 0;
	htim4.Instance->CCR4 = 0;
}

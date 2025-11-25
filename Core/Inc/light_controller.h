/*
 * light_controller.h
 *
 *  Created on: Nov 25, 2025
 *      Author: zam12
 */

#ifndef INC_LIGHT_CONTROLLER_H_
#define INC_LIGHT_CONTROLLER_H_

#define MAX_POWER 100


enum BtnColor{
	GREEN=0, YELLOW=1, RED=2
};

typedef struct {
    enum BtnColor color;
    int power;
} ColorMode;


int set_light_power(enum BtnColor btn, int power);
void clear_lights();


void choose_mode(int mode_num);

#endif /* INC_LIGHT_CONTROLLER_H_ */

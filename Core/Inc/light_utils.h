/*
 * light_utils.h
 *
 *  Created on: Sep 23, 2025
 *      Author: zam12
 */

#ifndef INC_LIGHT_UTILS_H_
#define INC_LIGHT_UTILS_H_


#include "stm32f4xx_hal_gpio.h"

#define GREEN_PIN GPIO_PIN_13
#define YELLOW_PIN GPIO_PIN_14
#define RED_PIN GPIO_PIN_15

void no_light();
void green_light();
void yellow_light();
void red_light();


#endif /* INC_LIGHT_UTILS_H_ */

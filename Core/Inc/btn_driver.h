#ifndef __BtnDriver_H
#define __BtnDriver_H

#include "stm32f4xx_hal.h"

#define BTN_PERIOD 50

#define BTN_DOWN 0
#define BTN_UP 1
#define BTN_CLICKED 2
#define BTN_RELEASED 3

#define BTN_PIN GPIO_PIN_15

uint8_t getBtnState();

#endif

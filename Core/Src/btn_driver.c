#include "btn_driver.h"

uint32_t tick_count = 0;
uint32_t prevState = BTN_UP;

uint8_t getBtnState(){
    // if (HAL_GetTick() - tick_count < BTN_PERIOD)
    //     return prevState;
    
    GPIO_PinState btn_state = HAL_GPIO_ReadPin(GPIOC, BTN_PIN);
    
    if (btn_state == GPIO_PIN_RESET) {		//clicked
        if (prevState == BTN_DOWN)
            prevState = BTN_DOWN;
        else if (prevState == BTN_UP)
            prevState = BTN_CLICKED;
        else if (prevState == BTN_CLICKED)
            prevState = BTN_DOWN;
        else if (prevState == BTN_RELEASED)
            prevState = BTN_CLICKED;
    } else { 								//released
        if (prevState == BTN_DOWN)
            prevState = BTN_RELEASED;
        else if (prevState == BTN_UP)
            prevState = BTN_UP;
        else if (prevState == BTN_CLICKED)
            prevState = BTN_RELEASED;
        else if (prevState == BTN_RELEASED)
            prevState = BTN_UP;
    }

    tick_count = HAL_GetTick();
    return prevState;
};

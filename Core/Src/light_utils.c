/*
 * light_utils.c
 *
 *  Created on: Sep 23, 2025
 *      Author: zam12
 */
#include "stm32f4xx_hal.h"
#include "light_utils.h"

  void no_light(){
    HAL_GPIO_WritePin(GPIOD, GREEN_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, YELLOW_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, RED_PIN, GPIO_PIN_RESET);
  }
  void green_light(){
    HAL_GPIO_WritePin(GPIOD, GREEN_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, YELLOW_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, RED_PIN, GPIO_PIN_RESET);
  }
  void yellow_light(){
    HAL_GPIO_WritePin(GPIOD, GREEN_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, YELLOW_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, RED_PIN, GPIO_PIN_RESET);
  }
  void red_light(){
    HAL_GPIO_WritePin(GPIOD, GREEN_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, YELLOW_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, RED_PIN, GPIO_PIN_SET);
  }
  void red_green_light(){
    HAL_GPIO_WritePin(GPIOD, GREEN_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, YELLOW_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, RED_PIN, GPIO_PIN_SET);
  }
  void yellow_green_light(){
    HAL_GPIO_WritePin(GPIOD, GREEN_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, YELLOW_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, RED_PIN, GPIO_PIN_RESET);
  }

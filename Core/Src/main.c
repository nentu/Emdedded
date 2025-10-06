/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  my_GPIO_Init();
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */

  void switch_color(uint8_t color_id){
      if (color_id == STATE_N)
        no_light();
      else if (color_id == STATE_G)
        green_light();
      else if (color_id == STATE_Y)
        yellow_light();
      else if (color_id == STATE_R)
        red_light();
        else if (color_id == STATE_RG)
            red_green_light();
        else if (color_id == STATE_YG)
            yellow_green_light();
  }

  uint32_t state_id = 0;
  uint32_t color_scheme = 0;
  uint32_t tick_count = 0;
  uint32_t inp_scheme = 0;

  char input_symbol;
  char write_buffer[50];

  uint32_t scheme_count = 4;
  uint32_t step_period_array[8];
  for (int i = 0; i < 8; i++){
	  step_period_array[i] = STEP_PERIOD;
  }

  uint32_t scheme_len_array[8];
  for (int i = 0; i < 8; i++){
	  scheme_len_array[i] = 10;
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    if (HAL_GetTick() - tick_count >= step_period_array[color_scheme]) {

		switch_color(state_list[color_scheme][state_id]);
		state_id = (state_id + 1) % scheme_len_array[color_scheme];
		tick_count = HAL_GetTick();

    }

    if (getBtnState() == BTN_CLICKED){
    	color_scheme = (color_scheme + 1) % scheme_count;
    	state_id = 0;

        sprintf(write_buffer, "Color scheme: %d, period: %d\n", color_scheme, step_period_array[color_scheme]);
    	HAL_UART_Transmit( &huart6, (uint8_t *) write_buffer, strlen( write_buffer ), 100 );
    }


    if (read_char(&input_symbol) == HAL_OK){
    	parser_res = parser_step(input_symbol);

    	if (parser_res == PRSR_OK){
    		continue;
    	} else if (parser_res == UNKNOWN){
            sprintf(write_buffer, "Incorrect input! Please try again\n");
            HAL_UART_Transmit( &huart6, (uint8_t *) write_buffer, strlen( write_buffer ), 100 );
    	} else if (parser_res == PRSR_CREATE_SCHEME) {
            scheme_len_array[5+(inp_scheme%4)]
    		for (int i = 0; i < strlen(parser_buffer); i++){
              switch (parser_buffer[i]){
                case "y":
                  state_list[5+(inp_scheme%4)][i] = STATE_Y;
                  break;
                case "r":
                  state_list[5+(inp_scheme%4)][i] = STATE_R;
                  break;
                case "g":
                  state_list[5+(inp_scheme%4)][i] = STATE_G;
                  break;
                case "n":
                  state_list[5+(inp_scheme%4)][i] = STATE_N;
                  break;
              }
            }
            sprintf(write_buffer, "Please input LED switching period. Enter 1 for for fast (200 ms), 2 for medium (500 ms), 3 for slow (1000 ms)\n");
            HAL_UART_Transmit( &huart6, (uint8_t *) write_buffer, strlen( write_buffer ), 100 );
    	}
    	else if (parser_res == PRSR_PERIOD){
          switch (parser_buffer[0]){
            case 1:
              step_period_array[5+(inp_scheme%4)] = 200;
              break;
            case 2:
              step_period_array[5+(inp_scheme%4)] = 500;
              break;
            case 3:
              step_period_array[5+(inp_scheme%4)] = 1000;
              break;
          }
          sprintf(write_buffer, "New scheme number: %d\n", (5+(inp_scheme%4)));
          HAL_UART_Transmit( &huart6, (uint8_t *) write_buffer, strlen( write_buffer ), 100 );
          inp_scheme += 1;
        }
        else if (parser_res == PRSR_CHANGE_SCHEME){
            color_scheme = parser_buffer[0];
            state_id = 0;

            sprintf(write_buffer, "Color scheme: %d, period: %d\n", color_scheme, step_period_array[color_scheme]);
            HAL_UART_Transmit( &huart6, (uint8_t *) write_buffer, strlen( write_buffer ), 100 );
        }

        sprintf(write_buffer, "Got: ");
    	HAL_UART_Transmit( &huart6, (uint8_t *) write_buffer, 1, 100 );
    }

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

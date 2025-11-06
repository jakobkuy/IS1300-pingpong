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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "leds.h"
#include "button.h"
#include "timer.h"
#include "score.h"
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
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void ping_pong_game(void);
void test_leds(void);
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

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  leds_init();
  button_init();

  /* Uncomment test_leds() to run LED test instead of game */
  /* test_leds(); */

  ping_pong_game();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* Configure LED pins as outputs (ping-pong board) */

  /* LEDs on GPIOB: PB1, PB2, PB11, PB12 */
  GPIO_InitStruct.Pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_11 | GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* LEDs on GPIOA: PA11, PA12 */
  GPIO_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* LEDs on GPIOC: PC5, PC6 */
  GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* Configure button pins as inputs with pull-up (ping-pong board) */

  /* Right button on PC8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* Left button on PB15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* Set all LEDs to initial state (off) */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_11 | GPIO_PIN_12, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11 | GPIO_PIN_12, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5 | GPIO_PIN_6, GPIO_PIN_RESET);

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* See GAME_GUIDE.md for game rules and instructions */

/* Game configuration constants */
#define WINNING_SCORE      5
#define INITIAL_SPEED    200
#define MIN_SPEED        100
#define SPEED_DECREASE    20
#define SCORE_DISPLAY_TIME 2000

typedef enum {
    GAME_START,
    BALL_MOVING_RIGHT,
    BALL_MOVING_LEFT,
    POINT_SCORED,
    GAME_OVER
} GameState;

/**
 * Main ping-pong game loop (never returns)
 */
void ping_pong_game(void) {
    GameState state = GAME_START;
    int ball_position = 4;
    int ball_direction = 1;
    uint32_t ball_speed = INITIAL_SPEED;
    uint8_t left_score = 0;
    uint8_t right_score = 0;
    int button_pressed = 0;

    leds_clear();
    HAL_Delay(500);

    /* Flash LEDs to signal game start */
    for (int i = 0; i < 3; i++) {
        leds_all();
        HAL_Delay(200);
        leds_clear();
        HAL_Delay(200);
    }

    HAL_Delay(500);

    while (1) {
        switch (state) {

        case GAME_START:
            ball_position = 4;

            if ((HAL_GetTick() % 2) == 0) {
                ball_direction = 1;
                state = BALL_MOVING_RIGHT;
            } else {
                ball_direction = -1;
                state = BALL_MOVING_LEFT;
            }

            ball_speed = INITIAL_SPEED;
            break;

        case BALL_MOVING_RIGHT:
            leds_index(ball_position);
            timer_init(ball_speed);

            while (!timer_now()) {
                button_pressed = button_read();

                if (button_pressed == RIGHT_BUTTON && ball_position == 8) {
                    ball_direction = -1;
                    state = BALL_MOVING_LEFT;

                    if (ball_speed > MIN_SPEED) {
                        ball_speed -= SPEED_DECREASE;
                    }

                    break;
                }
            }

            if (state == BALL_MOVING_RIGHT) {
                ball_position++;

                if (ball_position > 8) {
                    left_score++;
                    state = POINT_SCORED;

                    for (int i = 0; i < 3; i++) {
                        leds_all();
                        HAL_Delay(100);
                        leds_clear();
                        HAL_Delay(100);
                    }
                }
            }
            break;

        case BALL_MOVING_LEFT:
            leds_index(ball_position);
            timer_init(ball_speed);

            while (!timer_now()) {
                button_pressed = button_read();

                if (button_pressed == LEFT_BUTTON && ball_position == 1) {
                    ball_direction = 1;
                    state = BALL_MOVING_RIGHT;

                    if (ball_speed > MIN_SPEED) {
                        ball_speed -= SPEED_DECREASE;
                    }

                    break;
                }
            }

            if (state == BALL_MOVING_LEFT) {
                ball_position--;

                if (ball_position < 1) {
                    right_score++;
                    state = POINT_SCORED;

                    for (int i = 0; i < 3; i++) {
                        leds_all();
                        HAL_Delay(100);
                        leds_clear();
                        HAL_Delay(100);
                    }
                }
            }
            break;

        case POINT_SCORED:
            show_score(right_score, left_score, SCORE_DISPLAY_TIME);

            if (left_score >= WINNING_SCORE) {
                show_winner(0, 3000);
                state = GAME_OVER;
            } else if (right_score >= WINNING_SCORE) {
                show_winner(1, 3000);
                state = GAME_OVER;
            } else {
                state = GAME_START;
            }
            break;

        case GAME_OVER:
            HAL_Delay(1000);
            show_score(right_score, left_score, 3000);

            left_score = 0;
            right_score = 0;
            state = GAME_START;

            HAL_Delay(2000);

            for (int i = 0; i < 2; i++) {
                leds_all();
                HAL_Delay(300);
                leds_clear();
                HAL_Delay(300);
            }
            break;

        default:
            state = GAME_START;
            break;
        }
    }
}

/**
 * LED hardware test function - cycles through test patterns
 * To use: Call test_leds() instead of ping_pong_game() in main()
 */
void test_leds(void)
{
    while(1)
    {
        for(int i = 1; i <= 8; i++) {
            leds_index(i);
            HAL_Delay(500);
        }

        leds_clear();
        HAL_Delay(500);

        for(int i = 1; i <= 8; i++) {
            leds_index(i);
            HAL_Delay(100);
        }

        for(int i = 8; i >= 1; i--) {
            leds_index(i);
            HAL_Delay(100);
        }

        leds_clear();
        HAL_Delay(500);

        for(int i = 0; i < 3; i++) {
            leds_all();
            HAL_Delay(300);
            leds_clear();
            HAL_Delay(300);
        }

        HAL_Delay(1000);
    }
}

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
#ifdef USE_FULL_ASSERT
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

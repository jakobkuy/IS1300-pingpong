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
  leds_init();      // Initialize LED control module
  button_init();    // Initialize button handling module

  // Uncomment the line below to run the LED test instead of the game
  // test_leds();

  ping_pong_game();  // Start the ping-pong game
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

/**
 * ===========================================================================
 *                          PING-PONG GAME
 * ===========================================================================
 *
 * Game Rules:
 * -----------
 * 1. The "ball" (lit LED) moves from one side of the board to the other
 * 2. Left player uses LEFT button (PB15) when ball reaches their end (LED 1)
 * 3. Right player uses RIGHT button (PC8) when ball reaches their end (LED 8)
 * 4. If button is pressed at the correct time, ball bounces back
 * 5. If player misses, opponent scores a point
 * 6. First player to reach WINNING_SCORE wins the game
 * 7. Game displays score after each point and winner at the end
 *
 * Ball Physics:
 * -------------
 * - Ball starts at center and moves in random direction
 * - Ball speed increases slightly after each successful hit (up to max)
 * - Ball speed resets after a miss
 * - Hit timing window: Must press button when ball is at end position
 *
 * Visual Feedback:
 * ----------------
 * - Moving ball: Single LED travels across the board
 * - Successful hit: Brief flash of all LEDs
 * - Miss: Flash all LEDs 3 times rapidly
 * - Score display: LEDs light up from each player's side
 * - Winner: Flashing LEDs on winner's side
 */

/* Game configuration constants */
#define WINNING_SCORE      5    // Points needed to win the game
#define INITIAL_SPEED    200    // Starting ball speed (milliseconds per LED)
#define MIN_SPEED        100    // Fastest ball speed (after multiple hits)
#define SPEED_DECREASE    20    // Speed increase per hit (lower = faster)
#define SCORE_DISPLAY_TIME 2000 // How long to show score (milliseconds)

/* Game state enumeration */
typedef enum {
    GAME_START,          // Initial game state
    BALL_MOVING_RIGHT,   // Ball traveling left to right
    BALL_MOVING_LEFT,    // Ball traveling right to left
    POINT_SCORED,        // A point was just scored
    GAME_OVER            // Game has ended
} GameState;

/**
 * Main ping-pong game function
 *
 * This function contains the complete game loop and never returns.
 * It manages all game states, ball movement, collision detection,
 * scoring, and visual feedback.
 */
void ping_pong_game(void) {
    /* Game state variables */
    GameState state = GAME_START;
    int ball_position = 4;          // Current LED position (1-8)
    int ball_direction = 1;         // 1 = moving right, -1 = moving left
    uint32_t ball_speed = INITIAL_SPEED;  // Current ball speed (ms per LED)
    uint8_t left_score = 0;         // Left player's score
    uint8_t right_score = 0;        // Right player's score
    int button_pressed = 0;         // Which button was pressed

    /* Game initialization */
    leds_clear();
    HAL_Delay(500);

    /* Flash all LEDs 3 times to signal game start */
    for (int i = 0; i < 3; i++) {
        leds_all();
        HAL_Delay(200);
        leds_clear();
        HAL_Delay(200);
    }

    HAL_Delay(500);

    /* Main game loop - runs forever */
    while (1) {
        switch (state) {

        /* ----------------------------------------------------------------
         * GAME_START: Initialize new round
         * ---------------------------------------------------------------- */
        case GAME_START:
            // Reset ball to center position
            ball_position = 4;

            // Randomly choose starting direction based on system tick
            if ((HAL_GetTick() % 2) == 0) {
                ball_direction = 1;   // Move right
                state = BALL_MOVING_RIGHT;
            } else {
                ball_direction = -1;  // Move left
                state = BALL_MOVING_LEFT;
            }

            // Reset ball speed for new round
            ball_speed = INITIAL_SPEED;
            break;

        /* ----------------------------------------------------------------
         * BALL_MOVING_RIGHT: Ball traveling toward right player
         * ---------------------------------------------------------------- */
        case BALL_MOVING_RIGHT:
            // Display ball at current position
            leds_index(ball_position);

            // Start non-blocking timer for ball movement
            timer_init(ball_speed);

            // Wait for timer to expire while checking for button presses
            while (!timer_now()) {
                button_pressed = button_read();

                // Check if right player pressed button at the right time
                if (button_pressed == RIGHT_BUTTON && ball_position == 8) {
                    // Successful hit! Ball bounces back
                    ball_direction = -1;  // Reverse direction
                    state = BALL_MOVING_LEFT;

                    // Increase ball speed (decrease delay) for more challenge
                    if (ball_speed > MIN_SPEED) {
                        ball_speed -= SPEED_DECREASE;
                    }

                    // No visual feedback - just continue playing
                    break;  // Exit timer loop to continue with new state
                }
                // Check for early/wrong button press (penalty could be added here)
                else if (button_pressed == RIGHT_BUTTON && ball_position != 8) {
                    // Right player pressed too early - ignored in this version
                    // Could add penalty: left_score++; state = POINT_SCORED;
                }
            }

            // Timer expired - move ball to next position
            if (state == BALL_MOVING_RIGHT) {  // Still moving right
                ball_position++;

                // Check if ball reached right end without being hit
                if (ball_position > 8) {
                    // Right player missed! Left player scores
                    left_score++;
                    state = POINT_SCORED;

                    // Visual feedback: flash all LEDs rapidly
                    for (int i = 0; i < 3; i++) {
                        leds_all();
                        HAL_Delay(100);
                        leds_clear();
                        HAL_Delay(100);
                    }
                }
            }
            break;

        /* ----------------------------------------------------------------
         * BALL_MOVING_LEFT: Ball traveling toward left player
         * ---------------------------------------------------------------- */
        case BALL_MOVING_LEFT:
            // Display ball at current position
            leds_index(ball_position);

            // Start non-blocking timer for ball movement
            timer_init(ball_speed);

            // Wait for timer to expire while checking for button presses
            while (!timer_now()) {
                button_pressed = button_read();

                // Check if left player pressed button at the right time
                if (button_pressed == LEFT_BUTTON && ball_position == 1) {
                    // Successful hit! Ball bounces back
                    ball_direction = 1;  // Reverse direction
                    state = BALL_MOVING_RIGHT;

                    // Increase ball speed (decrease delay) for more challenge
                    if (ball_speed > MIN_SPEED) {
                        ball_speed -= SPEED_DECREASE;
                    }

                    // No visual feedback - just continue playing
                    break;  // Exit timer loop to continue with new state
                }
                // Check for early/wrong button press (penalty could be added here)
                else if (button_pressed == LEFT_BUTTON && ball_position != 1) {
                    // Left player pressed too early - ignored in this version
                    // Could add penalty: right_score++; state = POINT_SCORED;
                }
            }

            // Timer expired - move ball to next position
            if (state == BALL_MOVING_LEFT) {  // Still moving left
                ball_position--;

                // Check if ball reached left end without being hit
                if (ball_position < 1) {
                    // Left player missed! Right player scores
                    right_score++;
                    state = POINT_SCORED;

                    // Visual feedback: flash all LEDs rapidly
                    for (int i = 0; i < 3; i++) {
                        leds_all();
                        HAL_Delay(100);
                        leds_clear();
                        HAL_Delay(100);
                    }
                }
            }
            break;

        /* ----------------------------------------------------------------
         * POINT_SCORED: Handle scoring and check for game end
         * ---------------------------------------------------------------- */
        case POINT_SCORED:
            // Display current score
            show_score(right_score, left_score, SCORE_DISPLAY_TIME);

            // Check if either player has won
            if (left_score >= WINNING_SCORE) {
                // Left player wins!
                show_winner(0, 3000);  // 0 = left player
                state = GAME_OVER;
            } else if (right_score >= WINNING_SCORE) {
                // Right player wins!
                show_winner(1, 3000);  // 1 = right player
                state = GAME_OVER;
            } else {
                // Game continues - start new round
                state = GAME_START;
            }
            break;

        /* ----------------------------------------------------------------
         * GAME_OVER: End of game - wait for reset or restart
         * ---------------------------------------------------------------- */
        case GAME_OVER:
            // Game has ended - display final score one more time
            HAL_Delay(1000);
            show_score(right_score, left_score, 3000);

            // Reset scores and restart game
            left_score = 0;
            right_score = 0;
            state = GAME_START;

            // Long delay before starting new game
            HAL_Delay(2000);

            // Flash LEDs to signal new game starting
            for (int i = 0; i < 2; i++) {
                leds_all();
                HAL_Delay(300);
                leds_clear();
                HAL_Delay(300);
            }
            break;

        default:
            // Should never reach here - reset to safe state
            state = GAME_START;
            break;
        }
    }
}

/**
 * LED Test Program
 *
 * This function tests all LED hardware connections by cycling through
 * various patterns. Useful for debugging and verifying hardware setup.
 *
 * Test Sequence:
 * 1. Cycle through LEDs 1-8 individually (slow - 500ms each)
 * 2. Cycle through LEDs 1-8 individually (fast - 100ms each)
 * 3. Cycle backwards through LEDs 8-1 (fast - 100ms each)
 * 4. Flash all LEDs on/off 3 times
 * 5. Repeat sequence
 *
 * To use: Uncomment the test_leds() call in main() and comment out ping_pong_game()
 */
void test_leds(void)
{
    while(1)
    {
        /* Test 1: Cycle through each LED slowly (500ms each) */
        for(int i = 1; i <= 8; i++) {
            leds_index(i);
            HAL_Delay(500);
        }

        /* Small pause */
        leds_clear();
        HAL_Delay(500);

        /* Test 2: Cycle through each LED quickly (100ms each) */
        for(int i = 1; i <= 8; i++) {
            leds_index(i);
            HAL_Delay(100);
        }

        /* Test 3: Cycle backwards */
        for(int i = 8; i >= 1; i--) {
            leds_index(i);
            HAL_Delay(100);
        }

        /* Small pause */
        leds_clear();
        HAL_Delay(500);

        /* Test 4: Flash all LEDs on/off 3 times */
        for(int i = 0; i < 3; i++) {
            leds_all();
            HAL_Delay(300);
            leds_clear();
            HAL_Delay(300);
        }

        /* Long pause before repeating */
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

/*
 * score.c
 *
 * Score display module for ping-pong game
 */

#include "score.h"
#include "leds.h"
#include "stm32l4xx_hal.h"

/**
 * Display current score on LEDs
 * @param right_score Right player score (0-4)
 * @param left_score Left player score (0-4)
 * @param duration_ms Display duration in milliseconds
 */
void show_score(uint8_t right_score, uint8_t left_score, uint32_t duration_ms)
{
    leds_clear();
    HAL_Delay(100);

    for (int i = 1; i <= left_score && i <= 4; i++)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1 << (i - 1), GPIO_PIN_SET);
    }

    if (right_score >= 1) {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
    }
    if (right_score >= 2) {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
    }
    if (right_score >= 3) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);
    }
    if (right_score >= 4) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
    }

    HAL_Delay(duration_ms);
    leds_clear();
}

/**
 * Show winner by blinking their side's LEDs
 * @param winner 0 (left player) or 1 (right player)
 * @param duration_ms Display duration in milliseconds
 */
void show_winner(uint8_t winner, uint32_t duration_ms)
{
    const int num_blinks = 5;
    const int blink_on_time = 300;
    const int blink_off_time = 200;

    for (int i = 0; i < num_blinks; i++)
    {
        if (winner == 0)
        {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
        }
        else
        {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
        }

        HAL_Delay(blink_on_time);
        leds_clear();
        HAL_Delay(blink_off_time);
    }

    leds_all();
    HAL_Delay(500);
    leds_clear();
}

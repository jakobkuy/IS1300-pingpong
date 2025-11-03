/*
 * score.c
 *
 * Score display module implementation for ping-pong game
 *
 * This module provides visual feedback for scores and game end.
 * It uses the 8 LEDs to display:
 * - Current score (LEDs lit from each side)
 * - Winner announcement (flashing LEDs on winner's side)
 *
 * LED Layout:
 * [LED1][LED2][LED3][LED4]  [LED5][LED6][LED7][LED8]
 *  <-- Left Player Side -->  <-- Right Player Side -->
 *
 * Score Display Example (Left: 2, Right: 3):
 * [ON][ON][OFF][OFF]  [OFF][ON][ON][ON]
 */

#include "score.h"
#include "leds.h"
#include "stm32l4xx_hal.h"

/**
 * Display current score on LEDs
 *
 * Shows the score by lighting up LEDs from each player's side:
 * - Left player score: Light up LEDs 1, 2, 3, 4 from left
 * - Right player score: Light up LEDs 8, 7, 6, 5 from right
 *
 * Example: If left has 2 points and right has 3 points:
 *   LEDs 1-2 ON (left side), LEDs 8-7-6 ON (right side)
 *
 * @param right_score Right player score (0-4)
 * @param left_score Left player score (0-4)
 * @param duration_ms How long to display the score (milliseconds)
 *
 * Visual representation:
 * Score 0: All LEDs off on that side
 * Score 1: 1 LED on from that side
 * Score 2: 2 LEDs on from that side
 * Score 3: 3 LEDs on from that side
 * Score 4: 4 LEDs on from that side (max score)
 */
void show_score(uint8_t right_score, uint8_t left_score, uint32_t duration_ms) {
    // Clear all LEDs first
    leds_clear();

    // Small delay to create visual separation
    HAL_Delay(100);

    // Light up LEDs based on left player score (LEDs 1-4)
    for (int i = 1; i <= left_score && i <= 4; i++) {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1 << (i-1), GPIO_PIN_SET);
    }

    // Light up LEDs based on right player score (LEDs 5-8, from right to left)
    // LED 5 = first right LED, LED 6 = second, LED 7 = third, LED 8 = fourth
    if (right_score >= 1) {  // Light LED 8 (rightmost)
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
    }
    if (right_score >= 2) {  // Light LED 7
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
    }
    if (right_score >= 3) {  // Light LED 6
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);
    }
    if (right_score >= 4) {  // Light LED 5
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
    }

    // Display the score for specified duration
    HAL_Delay(duration_ms);

    // Clear LEDs after display
    leds_clear();
}

/**
 * Show winner by blinking their side's LEDs
 *
 * Creates a celebration effect by flashing the winner's LEDs
 * multiple times. This provides clear visual feedback about
 * who won the game.
 *
 * @param winner 0: left player wins, 1: right player wins
 * @param duration_ms Total duration of winner display (milliseconds)
 *
 * Animation:
 * - Blinks winner's 4 LEDs on/off 5 times
 * - Each blink is 300ms on, 200ms off
 * - Creates an exciting celebration effect
 */
void show_winner(uint8_t winner, uint32_t duration_ms) {
    const int num_blinks = 5;  // Number of times to blink
    const int blink_on_time = 300;   // LED on duration (ms)
    const int blink_off_time = 200;  // LED off duration (ms)

    // Perform blinking animation
    for (int i = 0; i < num_blinks; i++) {
        // Turn on winner's LEDs
        if (winner == 0) {
            // Left player wins - light up LEDs 1-4
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);   // LED 1
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);   // LED 2
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);  // LED 3
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);  // LED 4
        } else {
            // Right player wins - light up LEDs 5-8
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);  // LED 5
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);  // LED 6
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);   // LED 7
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);   // LED 8
        }

        HAL_Delay(blink_on_time);  // Keep LEDs on

        // Turn off all LEDs
        leds_clear();

        HAL_Delay(blink_off_time);  // Keep LEDs off
    }

    // Final display: light up all LEDs briefly as grand finale
    leds_all();
    HAL_Delay(500);
    leds_clear();
}

/*
 * score.h
 *
 * ============================================================================
 * Score Display Module for Ping-Pong Game
 * ============================================================================
 *
 * This module provides visual feedback for game scores and winner announcement
 * using the 8 LEDs on the ping-pong board. Scores are displayed by lighting
 * up LEDs from each player's side.
 *
 * LED Layout:
 * -----------
 * [LED1][LED2][LED3][LED4]    [LED5][LED6][LED7][LED8]
 *  <--- Left Player Side -->   <--- Right Player Side -->
 *
 * Score Display Logic:
 * --------------------
 * - Left player score:  Light LEDs from left (1, 2, 3, 4)
 * - Right player score: Light LEDs from right (8, 7, 6, 5)
 *
 * Example Displays:
 * -----------------
 * Left: 2, Right: 3
 *   [ON][ON][OFF][OFF]  [OFF][ON][ON][ON]
 *
 * Left: 0, Right: 4
 *   [OFF][OFF][OFF][OFF]  [ON][ON][ON][ON]
 *
 * Left: 4, Right: 1
 *   [ON][ON][ON][ON]  [OFF][OFF][OFF][ON]
 *
 * Winner Display:
 * ---------------
 * - Blinks winner's 4 LEDs multiple times
 * - Creates celebration effect
 * - Ends with all LEDs flash
 *
 * Usage Example:
 * --------------
 *   uint8_t left = 3, right = 2;
 *   show_score(right, left, 2000);  // Show for 2 seconds
 *
 *   if (right >= 5) {
 *       show_winner(1, 3000);       // Right player wins (1 = right)
 *   }
 */

#ifndef SCORE_H_
#define SCORE_H_

#include "main.h"
#include <stdint.h>

/* Function prototypes */

/**
 * Display current score on LEDs
 *
 * Shows the score by lighting up LEDs from each player's side:
 * - Left player:  LEDs 1-4 (from left)
 * - Right player: LEDs 5-8 (from right)
 *
 * The number of lit LEDs on each side represents that player's score.
 * Maximum displayable score is 4 per side (all 4 LEDs lit).
 *
 * @param right_score Right player score (0-4)
 * @param left_score  Left player score (0-4)
 * @param duration_ms How long to display the score in milliseconds
 *
 * Example:
 *   show_score(2, 3, 2000);  // Right: 2, Left: 3, show for 2 seconds
 *
 * Visual Result (Right: 2, Left: 3):
 *   [ON][ON][ON][OFF]  [OFF][OFF][ON][ON]
 *    <-- Left: 3 -->      <-- Right: 2 -->
 *
 * Note: Clears all LEDs before and after display for visual separation
 */
void show_score(uint8_t right_score, uint8_t left_score, uint32_t duration_ms);

/**
 * Show winner by blinking their side's LEDs
 *
 * Creates a celebration effect by rapidly blinking the winner's 4 LEDs
 * on and off multiple times, followed by a final all-LED flash.
 * Provides clear visual feedback about who won the game.
 *
 * @param winner      0: left player wins, 1: right player wins
 * @param duration_ms Total duration of winner display in milliseconds
 *                    (Note: Actual duration depends on blink count and timing)
 *
 * Example:
 *   show_winner(0, 3000);  // Left player wins, ~3 second display
 *   show_winner(1, 3000);  // Right player wins, ~3 second display
 *
 * Animation Pattern:
 * - 5 blinks: 300ms ON, 200ms OFF
 * - Final: All LEDs ON for 500ms
 * - Ends with all LEDs OFF
 */
void show_winner(uint8_t winner, uint32_t duration_ms);

#endif /* SCORE_H_ */

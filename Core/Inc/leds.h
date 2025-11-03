/*
 * leds.h
 *
 * ============================================================================
 * LED Control Module for Ping-Pong Game
 * ============================================================================
 *
 * This module provides control functions for the 8 LEDs used in the ping-pong
 * game. These LEDs represent the playing field and display the ball position,
 * scores, and other game states.
 *
 * Hardware Pin Mapping:
 * ---------------------
 * LED 1 (leftmost)  -> PB1  (GPIOB Pin 1)
 * LED 2             -> PB2  (GPIOB Pin 2)
 * LED 3             -> PB11 (GPIOB Pin 11)
 * LED 4             -> PB12 (GPIOB Pin 12)
 * LED 5             -> PA11 (GPIOA Pin 11)
 * LED 6             -> PA12 (GPIOA Pin 12)
 * LED 7             -> PC5  (GPIOC Pin 5)
 * LED 8 (rightmost) -> PC6  (GPIOC Pin 6)
 *
 * Physical Layout:
 * ----------------
 *  [1] [2] [3] [4]     [5] [6] [7] [8]
 *  <-- Left Side -->   <-- Right Side -->
 *
 * Game Usage:
 * -----------
 * - Ball position: Use leds_index(position) to show moving ball
 * - Hit feedback:  Use leds_all() to flash on successful hit
 * - Miss feedback: Use leds_all() with repeated flashing
 * - Score display: Individual LED control via GPIO functions
 * - Transitions:   Use leds_clear() between game states
 *
 * Usage Example:
 * --------------
 *   leds_init();           // Initialize LED system
 *
 *   // Animate ball moving right
 *   for (int i = 1; i <= 8; i++) {
 *       leds_index(i);     // Show ball at position i
 *       delay(200);        // Wait 200ms
 *   }
 *
 *   leds_all();            // Flash all LEDs
 *   delay(500);
 *   leds_clear();          // Turn all off
 */

#ifndef LEDS_H_
#define LEDS_H_

#include "main.h"

/* Function prototypes */

/**
 * Initialize all 8 LED GPIO pins
 *
 * Configures the LED control module and sets all LEDs to OFF state.
 * The actual GPIO pin configuration is handled by MX_GPIO_Init() in main.c.
 *
 * This function should be called once during system initialization before
 * using any other LED functions.
 *
 * Note: The STM32 GPIO pins must be properly configured as outputs with
 *       push-pull mode before calling this function.
 */
void leds_init(void);

/**
 * Light up LED at specific position and turn off all others
 *
 * This is the primary function for ball animation in the game. It ensures
 * only one LED is lit at a time, creating the illusion of a moving ball.
 *
 * @param i LED position (1-8, where 1 is leftmost, 8 is rightmost)
 *          Values outside this range are ignored (no LEDs change)
 *
 * Example:
 *   leds_index(1);  // Only LED 1 (leftmost) is ON
 *   leds_index(4);  // Only LED 4 is ON
 *   leds_index(8);  // Only LED 8 (rightmost) is ON
 *   leds_index(9);  // Invalid - no change (out of range)
 *
 * Implementation:
 *   1. Turn off all 8 LEDs
 *   2. Turn on only the specified LED
 *   3. Creates atomic LED state (only one LED lit)
 */
void leds_index(int i);

/**
 * Turn off all LEDs
 *
 * Sets all 8 LEDs to OFF state. Used during game transitions, after
 * displaying scores, or to clear the display between animations.
 *
 * Example usage:
 *   leds_clear();  // All LEDs off
 *   delay(500);    // Pause with no LEDs
 *   leds_all();    // Then turn all on for contrast
 *
 * This function is guaranteed to leave all LEDs in OFF state regardless
 * of their previous state.
 */
void leds_clear(void);

/**
 * Turn on all LEDs
 *
 * Sets all 8 LEDs to ON state simultaneously. Used for visual feedback:
 * - Successful ball hit (brief flash)
 * - Miss detection (rapid flashing)
 * - Game start/end indicators
 * - Winner celebration
 *
 * Example usage:
 *   // Flash all LEDs 3 times
 *   for (int i = 0; i < 3; i++) {
 *       leds_all();
 *       delay(300);
 *       leds_clear();
 *       delay(300);
 *   }
 *
 * This function is guaranteed to leave all LEDs in ON state.
 */
void leds_all(void);

#endif /* LEDS_H_ */

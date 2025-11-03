/*
 * timer.h
 *
 * ============================================================================
 * Non-Blocking Timer Module for Ping-Pong Game
 * ============================================================================
 *
 * This module provides a simple non-blocking timer that allows the game to
 * continue checking for button presses and updating state while waiting for
 * time to elapse, unlike HAL_Delay() which blocks all execution.
 *
 * Implementation:
 * ---------------
 * Uses the STM32 HAL system tick (HAL_GetTick()) which increments every
 * millisecond. The timer stores the start time and duration, then calculates
 * elapsed time on each check.
 *
 * Key Advantages:
 * ---------------
 * - Non-blocking: Program can do other work while timer runs
 * - Responsive: Can check buttons during wait periods
 * - Simple API: Just init and check
 *
 * Usage Example:
 * --------------
 *   // Wait for 500ms while checking buttons
 *   timer_init(500);
 *   while (!timer_now()) {
 *       int button = button_read();  // Can still check buttons
 *       if (button) {
 *           // Handle button press during wait
 *       }
 *   }
 *   // Timer has expired, continue
 *
 * Usage Pattern in Game Loop:
 * ----------------------------
 *   timer_init(ball_speed);          // Start timer for ball movement
 *   while (!timer_now()) {           // While timer still running
 *       check_buttons();             // Can still check for input
 *       update_display();            // Can still update display
 *   }
 *   move_ball();                     // Timer expired, move the ball
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "main.h"
#include <stdint.h>

/* Function prototypes */

/**
 * Initialize a non-blocking timer
 *
 * Starts a timer that will expire after the specified duration.
 * Uses HAL_GetTick() (1ms resolution) for timing.
 *
 * This function should be called before entering a loop that checks timer_now().
 * It can be called multiple times to restart the timer with a new duration.
 *
 * @param ms Duration in milliseconds (0 to 4,294,967,295)
 *
 * Example:
 *   timer_init(1000);  // Start a 1 second timer
 *   timer_init(50);    // Start a 50 millisecond timer
 */
void timer_init(uint32_t ms);

/**
 * Check if timer has expired
 *
 * This function should be called repeatedly in a loop to check if the
 * timer duration has elapsed. It does NOT block execution - it simply
 * returns the current timer state.
 *
 * The function remains accurate even if HAL_GetTick() wraps around
 * (which happens after approximately 49.7 days of continuous operation).
 *
 * @return 0: Timer is still running (duration has not elapsed)
 *         1: Timer has expired (duration has elapsed or been exceeded)
 *
 * Example:
 *   timer_init(200);
 *   while (!timer_now()) {
 *       // Loop continues until 200ms have passed
 *       do_other_work();
 *   }
 *   // Now timer has expired
 */
int timer_now(void);

#endif /* TIMER_H_ */

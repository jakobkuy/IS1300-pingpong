/*
 * timer.c
 *
 * Non-blocking timer module implementation for ping-pong game
 *
 * This module provides a simple non-blocking timer that allows
 * the main game loop to run without using HAL_Delay() which would
 * block all other operations.
 *
 * Usage pattern:
 *   timer_init(500);        // Start a 500ms timer
 *   while (!timer_now()) {  // Wait for timer to expire
 *       // Do other things while waiting
 *   }
 *
 * Features:
 * - Non-blocking: Game can check buttons and update state while waiting
 * - Millisecond precision using HAL_GetTick()
 * - Simple interface with init and check functions
 */

#include "timer.h"
#include "stm32l4xx_hal.h"

/* Timer state variables */
static uint32_t timer_start_time = 0;  // Timestamp when timer was started (ms)
static uint32_t timer_duration = 0;    // Timer duration in milliseconds

/**
 * Initialize a non-blocking timer
 *
 * Starts a timer that will expire after the specified duration.
 * Uses the HAL system tick (1ms resolution) for timing.
 *
 * @param ms Duration in milliseconds
 *
 * Example:
 *   timer_init(1000);  // Start a 1 second timer
 */
void timer_init(uint32_t ms) {
    timer_start_time = HAL_GetTick();  // Get current system time in milliseconds
    timer_duration = ms;                // Store duration for later comparison
}

/**
 * Check if timer has expired
 *
 * This function should be called repeatedly in a loop to check
 * if the timer duration has elapsed. It does not block execution.
 *
 * @return 0: timer is still running (duration not yet elapsed)
 *         1: timer has expired (duration has elapsed)
 *
 * Example:
 *   timer_init(500);
 *   while (!timer_now()) {
 *       // Do work while timer is running
 *       button_read();  // Can still check buttons
 *   }
 *   // Timer has now expired, continue with next step
 *
 * Note: Uses 32-bit arithmetic which handles HAL_GetTick() rollover
 *       (occurs after ~49.7 days of continuous operation)
 */
int timer_now(void) {
    uint32_t current_time = HAL_GetTick();  // Get current system time
    uint32_t elapsed_time = current_time - timer_start_time;  // Calculate elapsed time

    // Check if elapsed time exceeds the desired duration
    if (elapsed_time >= timer_duration) {
        return 1;  // Timer has expired
    } else {
        return 0;  // Timer still running
    }
}

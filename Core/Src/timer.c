/*
 * timer.c
 *
 * Non-blocking timer module using HAL_GetTick()
 */

#include "timer.h"
#include "stm32l4xx_hal.h"

static uint32_t timer_start_time = 0;
static uint32_t timer_duration = 0;

/**
 * Start a non-blocking timer
 * @param ms Duration in milliseconds
 */
void timer_init(uint32_t ms) {
    timer_start_time = HAL_GetTick();
    timer_duration = ms;
}

/**
 * Check if timer has expired
 * @return 0 (still running) or 1 (expired)
 */
int timer_now(void) {
    uint32_t current_time = HAL_GetTick();
    uint32_t elapsed_time = current_time - timer_start_time;

    if (elapsed_time >= timer_duration) {
        return 1;
    } else {
        return 0;
    }
}

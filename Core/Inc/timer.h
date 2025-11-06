/*
 * timer.h
 *
 * Non-blocking timer module using HAL_GetTick()
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "main.h"
#include <stdint.h>

/**
 * Start a non-blocking timer
 * @param ms Duration in milliseconds
 */
void timer_init(uint32_t ms);

/**
 * Check if timer has expired
 * @return 0 (still running) or 1 (expired)
 */
int timer_now(void);

#endif /* TIMER_H_ */

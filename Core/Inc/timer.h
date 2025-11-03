/*
 * timer.h
 *
 * Non-blocking timer module for ping-pong game
 * TODO: Implement in Phase 4
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "main.h"
#include <stdint.h>

/* Function prototypes */

/**
 * Initialize a non-blocking timer
 * @param ms Duration in milliseconds
 */
void timer_init(uint32_t ms);

/**
 * Check if timer has expired
 * @return 0: timer still running, 1: timer expired
 */
int timer_now(void);

#endif /* TIMER_H_ */

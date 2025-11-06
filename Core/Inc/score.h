/*
 * score.h
 *
 * Score display module for ping-pong game
 *
 * LED Layout:
 * [LED1][LED2][LED3][LED4]  [LED5][LED6][LED7][LED8]
 *  <-- Left Player Side-->   <-- Right Player Side-->
 */

#ifndef SCORE_H_
#define SCORE_H_

#include "main.h"
#include <stdint.h>

/**
 * Display current score on LEDs
 * @param right_score Right player score (0-4)
 * @param left_score Left player score (0-4)
 * @param duration_ms Display duration in milliseconds
 */
void show_score(uint8_t right_score, uint8_t left_score, uint32_t duration_ms);

/**
 * Show winner by blinking their side's LEDs
 * @param winner 0 (left player) or 1 (right player)
 * @param duration_ms Display duration in milliseconds
 */
void show_winner(uint8_t winner, uint32_t duration_ms);

#endif /* SCORE_H_ */

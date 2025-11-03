/*
 * leds.h
 *
 * LED control module for ping-pong game
 * Controls 8 LEDs connected to various GPIO pins
 */

#ifndef LEDS_H_
#define LEDS_H_

#include "main.h"

/* Function prototypes */

/**
 * Initialize all 8 LED GPIO pins
 * Configures pins as outputs and sets initial state (all off)
 */
void leds_init(void);

/**
 * Light up LED at position i (1-8) and turn off all others
 * This is the primary function for ball animation
 *
 * @param i LED position (1-8, where 1 is leftmost, 8 is rightmost)
 */
void leds_index(int i);

/**
 * Turn off all LEDs
 * Used during transitions and resets
 */
void leds_clear(void);

/**
 * Turn on all LEDs
 * Used for visual feedback (miss detection, etc.)
 */
void leds_all(void);

#endif /* LEDS_H_ */

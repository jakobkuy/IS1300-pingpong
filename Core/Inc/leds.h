/*
 * leds.h
 *
 * LED control module for ping-pong game
 *
 * Hardware Pin Mapping:
 * LED 1 (leftmost)  -> PB1  (GPIOB Pin 1)
 * LED 2             -> PB2  (GPIOB Pin 2)
 * LED 3             -> PB11 (GPIOB Pin 11)
 * LED 4             -> PB12 (GPIOB Pin 12)
 * LED 5             -> PA11 (GPIOA Pin 11)
 * LED 6             -> PA12 (GPIOA Pin 12)
 * LED 7             -> PC5  (GPIOC Pin 5)
 * LED 8 (rightmost) -> PC6  (GPIOC Pin 6)
 *
 * Physical Layout: [1] [2] [3] [4]  [5] [6] [7] [8]
 */

#ifndef LEDS_H_
#define LEDS_H_

#include "main.h"

/**
 * Initialize LED control module (call once at startup)
 * Note: GPIO pins configured by MX_GPIO_Init() in main.c
 */
void leds_init(void);

/**
 * Light up LED at position i (1-8), turn off all others
 * @param i LED position (1-8), values out of range are ignored
 */
void leds_index(int i);

/**
 * Turn off all LEDs
 */
void leds_clear(void);

/**
 * Turn on all LEDs
 */
void leds_all(void);

#endif /* LEDS_H_ */

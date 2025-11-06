/*
 * button.h
 *
 * Button input module with debouncing and edge detection
 *
 * Hardware Configuration:
 * Left Button:  PB15 (GPIOB Pin 15) - Active LOW with internal pull-up
 * Right Button: PC8  (GPIOC Pin 8)  - Active LOW with internal pull-up
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include "main.h"

#define LEFT_BUTTON  1
#define RIGHT_BUTTON 2

/**
 * Initialize button state tracking
 * Note: GPIO pins configured by MX_GPIO_Init() in main.c
 */
void button_init(void);

/**
 * Read button state with debouncing and edge detection
 * @return 0 (no press), LEFT_BUTTON, or RIGHT_BUTTON
 */
int button_read(void);

#endif /* BUTTON_H_ */

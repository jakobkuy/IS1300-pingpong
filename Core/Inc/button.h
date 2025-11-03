/*
 * button.h
 *
 * ============================================================================
 * Button Handling Module for Ping-Pong Game
 * ============================================================================
 *
 * This module provides button input functionality with software debouncing
 * and edge detection to prevent false triggers and multiple reads from
 * a single button press.
 *
 * Hardware Configuration:
 * -----------------------
 * Left Button:  PB15 (GPIOB Pin 15) - Active LOW with internal pull-up
 * Right Button: PC8  (GPIOC Pin 8)  - Active LOW with internal pull-up
 *
 * Features:
 * ---------
 * - Software debouncing (20ms delay)
 * - Edge detection (only triggers on button press, not hold)
 * - Returns button ID for easy game logic integration
 *
 * Usage Example:
 * --------------
 *   button_init();                    // Initialize button module
 *
 *   while (1) {
 *       int button = button_read();   // Check for button press
 *       if (button == LEFT_BUTTON) {
 *           // Left button was pressed
 *       } else if (button == RIGHT_BUTTON) {
 *           // Right button was pressed
 *       }
 *   }
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include "main.h"

/* Button return value definitions */
#define LEFT_BUTTON  1    // Return value for left button press
#define RIGHT_BUTTON 2    // Return value for right button press

/* Function prototypes */

/**
 * Initialize button GPIO pins and state tracking
 *
 * This function initializes the button state tracking variables used for
 * debouncing and edge detection. The actual GPIO pin configuration is
 * handled by MX_GPIO_Init() in main.c.
 *
 * Call this once during system initialization before using button_read().
 */
void button_init(void);

/**
 * Read button state with debouncing and edge detection
 *
 * This function checks both buttons and returns which button was just pressed.
 * It implements software debouncing to filter out noise and edge detection
 * to ensure each physical button press is only reported once.
 *
 * The function should be called repeatedly in the main game loop. It will
 * return 0 most of the time and only return a button ID when a button is
 * first pressed (not during the hold).
 *
 * @return 0:            No button pressed or still in debounce period
 *         LEFT_BUTTON:  Left button was just pressed
 *         RIGHT_BUTTON: Right button was just pressed
 *
 * Note: If both buttons are pressed simultaneously, right button takes priority.
 */
int button_read(void);

#endif /* BUTTON_H_ */

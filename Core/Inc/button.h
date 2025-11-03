/*
 * button.h
 *
 * Button handling module for ping-pong game
 * TODO: Implement in Phase 3
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include "main.h"

/* Button definitions */
#define LEFT_BUTTON  1
#define RIGHT_BUTTON 2

/* Function prototypes */

/**
 * Initialize button GPIO pins
 */
void button_init(void);

/**
 * Read button state
 * @return 0: no button pressed, 1: left button, 2: right button
 */
int button_read(void);

#endif /* BUTTON_H_ */

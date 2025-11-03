/*
 * button.c
 *
 * Button handling module implementation for ping-pong game
 *
 * Hardware Configuration:
 * - Left button:  PB15 (GPIOB Pin 15) - Active LOW (reads 0 when pressed)
 * - Right button: PC8  (GPIOC Pin 8)  - Active LOW (reads 0 when pressed)
 *
 * Features:
 * - Software debouncing to prevent false triggering
 * - Returns button state only on initial press (not continuous hold)
 * - Thread-safe single button press detection
 */

#include "button.h"
#include "stm32l4xx_hal.h"

/* Hardware pin definitions for buttons */
#define LEFT_BUTTON_PORT   GPIOB
#define LEFT_BUTTON_PIN    GPIO_PIN_15
#define RIGHT_BUTTON_PORT  GPIOC
#define RIGHT_BUTTON_PIN   GPIO_PIN_8

/* Debouncing parameters */
#define DEBOUNCE_DELAY_MS  20  // 20ms debounce delay to filter noise

/* Button state tracking for edge detection */
static uint8_t left_button_prev_state = 1;   // 1 = not pressed (pull-up resistor)
static uint8_t right_button_prev_state = 1;  // 1 = not pressed
static uint32_t last_press_time = 0;         // Timestamp of last button press

/**
 * Initialize button GPIO pins
 *
 * Note: GPIO initialization is already handled in MX_GPIO_Init() in main.c
 * This function initializes the button state tracking variables.
 */
void button_init(void) {
    // Initialize previous states to "not pressed"
    left_button_prev_state = 1;
    right_button_prev_state = 1;
    last_press_time = 0;
}

/**
 * Read button state with debouncing and edge detection
 *
 * This function reads both buttons and returns which button was pressed.
 * It implements:
 * 1. Debouncing: Ignores button presses within DEBOUNCE_DELAY_MS of last press
 * 2. Edge detection: Only returns press on transition from not-pressed to pressed
 * 3. Priority: If both buttons pressed simultaneously, right button takes priority
 *
 * @return 0: no button pressed
 *         LEFT_BUTTON (1): left button was just pressed
 *         RIGHT_BUTTON (2): right button was just pressed
 */
int button_read(void) {
    uint32_t current_time = HAL_GetTick();  // Get current system time in milliseconds

    // Check if enough time has passed since last button press (debouncing)
    if ((current_time - last_press_time) < DEBOUNCE_DELAY_MS) {
        return 0;  // Still in debounce period, ignore all inputs
    }

    // Read current state of both buttons
    // Note: Buttons are active LOW (0 = pressed, 1 = not pressed)
    uint8_t left_current = HAL_GPIO_ReadPin(LEFT_BUTTON_PORT, LEFT_BUTTON_PIN);
    uint8_t right_current = HAL_GPIO_ReadPin(RIGHT_BUTTON_PORT, RIGHT_BUTTON_PIN);

    int result = 0;  // Default: no button pressed

    // Check for right button press (falling edge: was 1, now 0)
    if (right_button_prev_state == 1 && right_current == 0) {
        result = RIGHT_BUTTON;
        last_press_time = current_time;  // Update debounce timestamp
    }
    // Check for left button press (falling edge: was 1, now 0)
    else if (left_button_prev_state == 1 && left_current == 0) {
        result = LEFT_BUTTON;
        last_press_time = current_time;  // Update debounce timestamp
    }

    // Update previous states for next call
    left_button_prev_state = left_current;
    right_button_prev_state = right_current;

    return result;
}

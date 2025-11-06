/*
 * button.c
 *
 * Button handling module with debouncing and edge detection
 *
 * Hardware Configuration:
 * Left button:  PB15 (GPIOB Pin 15) - Active LOW
 * Right button: PC8  (GPIOC Pin 8)  - Active LOW
 */

#include "button.h"
#include "stm32l4xx_hal.h"

#define LEFT_BUTTON_PORT   GPIOB
#define LEFT_BUTTON_PIN    GPIO_PIN_15
#define RIGHT_BUTTON_PORT  GPIOC
#define RIGHT_BUTTON_PIN   GPIO_PIN_8

#define DEBOUNCE_DELAY_MS  20

static uint8_t left_button_prev_state = 1;
static uint8_t right_button_prev_state = 1;
static uint32_t last_press_time = 0;

/**
 * Initialize button state tracking
 * Note: GPIO pins configured by MX_GPIO_Init() in main.c
 */
void button_init(void) {
    left_button_prev_state = 1;
    right_button_prev_state = 1;
    last_press_time = 0;
}

/**
 * Read button state with debouncing and edge detection
 * @return 0 (no press), LEFT_BUTTON, or RIGHT_BUTTON
 */
int button_read(void) {
    uint32_t current_time = HAL_GetTick();

    if ((current_time - last_press_time) < DEBOUNCE_DELAY_MS) {
        return 0;
    }

    uint8_t left_current = HAL_GPIO_ReadPin(LEFT_BUTTON_PORT, LEFT_BUTTON_PIN);
    uint8_t right_current = HAL_GPIO_ReadPin(RIGHT_BUTTON_PORT, RIGHT_BUTTON_PIN);

    int result = 0;

    if (right_button_prev_state == 1 && right_current == 0) {
        result = RIGHT_BUTTON;
        last_press_time = current_time;
    }
    else if (left_button_prev_state == 1 && left_current == 0) {
        result = LEFT_BUTTON;
        last_press_time = current_time;
    }

    left_button_prev_state = left_current;
    right_button_prev_state = right_current;

    return result;
}

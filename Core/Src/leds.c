/*
 * leds.c
 *
 * LED control module implementation
 *
 * Pin mappings:
 * LED 1 (leftmost)  - PB1
 * LED 2             - PB2
 * LED 3             - PB11
 * LED 4             - PB12
 * LED 5             - PA11
 * LED 6             - PA12
 * LED 7             - PC5
 * LED 8 (rightmost) - PC6
 */

#include "leds.h"
#include "stm32l4xx_hal.h"

/* LED pin definitions */
typedef struct {
    GPIO_TypeDef* port;
    uint16_t pin;
} LED_Pin;

static const LED_Pin led_pins[8] = {
    {GPIOB, GPIO_PIN_1},   // LED 1
    {GPIOB, GPIO_PIN_2},   // LED 2
    {GPIOB, GPIO_PIN_11},  // LED 3
    {GPIOB, GPIO_PIN_12},  // LED 4
    {GPIOA, GPIO_PIN_11},  // LED 5
    {GPIOA, GPIO_PIN_12},  // LED 6
    {GPIOC, GPIO_PIN_5},   // LED 7
    {GPIOC, GPIO_PIN_6}    // LED 8
};

/**
 * Initialize all 8 LED GPIO pins
 * Note: GPIO pins are configured by STM32CubeMX in main.c
 * This function can be used for additional initialization if needed
 */
void leds_init(void) {
    // GPIO initialization is handled by MX_GPIO_Init() in main.c
    // Turn off all LEDs initially
    leds_clear();
}

/**
 * Light up LED at position i (1-8) and turn off all others
 */
void leds_index(int i) {
    // Validate input range
    if (i < 1 || i > 8) {
        return;  // Invalid index, do nothing
    }

    // Turn off all LEDs first
    for (int j = 0; j < 8; j++) {
        HAL_GPIO_WritePin(led_pins[j].port, led_pins[j].pin, GPIO_PIN_RESET);
    }

    // Turn on the specified LED (convert 1-8 to 0-7 array index)
    HAL_GPIO_WritePin(led_pins[i-1].port, led_pins[i-1].pin, GPIO_PIN_SET);
}

/**
 * Turn off all LEDs
 */
void leds_clear(void) {
    for (int i = 0; i < 8; i++) {
        HAL_GPIO_WritePin(led_pins[i].port, led_pins[i].pin, GPIO_PIN_RESET);
    }
}

/**
 * Turn on all LEDs
 */
void leds_all(void) {
    for (int i = 0; i < 8; i++) {
        HAL_GPIO_WritePin(led_pins[i].port, led_pins[i].pin, GPIO_PIN_SET);
    }
}
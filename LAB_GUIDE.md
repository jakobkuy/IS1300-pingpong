# IS1300 Ping-Pong Game - Complete Lab Guide

**A comprehensive guide to understanding, implementing, and debugging the STM32 Ping-Pong game**

---

## 📚 Table of Contents

1. [Project Overview](#project-overview)
2. [Learning Objectives](#learning-objectives)
3. [Hardware Setup](#hardware-setup)
4. [Software Architecture](#software-architecture)
5. [Project Setup from Scratch](#project-setup-from-scratch)
6. [Module Implementation Deep Dive](#module-implementation-deep-dive)
7. [State Machine Explained](#state-machine-explained)
8. [Game Logic Implementation](#game-logic-implementation)
9. [Debugging and Testing](#debugging-and-testing)
10. [Common Issues and Solutions](#common-issues-and-solutions)
11. [Study Questions](#study-questions)

---

## 🎯 Project Overview

### What Are We Building?

A two-player ping-pong game on an STM32 microcontroller where:
- A "ball" (LED) moves back and forth
- Players press buttons to bounce the ball back
- First player to 5 points wins
- Game displays scores and winner using LEDs

### Why This Project?

This lab teaches fundamental embedded systems concepts:
- **GPIO Control**: Reading inputs (buttons) and writing outputs (LEDs)
- **State Machines**: Organizing complex behavior into manageable states
- **Non-blocking Timing**: Keeping the system responsive
- **Debouncing**: Handling noisy button inputs
- **Modular Design**: Breaking code into reusable components

---

## 🎓 Learning Objectives

By completing this lab, you will understand:

### Hardware Concepts
- ✅ GPIO pin configuration (input vs output)
- ✅ Pull-up/pull-down resistors
- ✅ Active HIGH vs active LOW signals
- ✅ LED current limiting
- ✅ Button debouncing

### Software Concepts
- ✅ State machine design and implementation
- ✅ Modular programming (separation of concerns)
- ✅ Non-blocking vs blocking delays
- ✅ Edge detection (detecting button press vs hold)
- ✅ Hardware abstraction layers

### Embedded Systems Concepts
- ✅ Real-time responsiveness
- ✅ System tick and timing
- ✅ Interrupt vs polling
- ✅ Peripheral configuration
- ✅ Build and debug workflow

---

## 🔌 Hardware Setup

### Components Needed

| Component | Quantity | Purpose |
|-----------|----------|---------|
| STM32 Nucleo-L476RG | 1 | Microcontroller board |
| LEDs | 8 | Game display (ball and score) |
| Push buttons | 2 | Player controls |
| Resistors (220Ω) | 8 | LED current limiting (if needed) |
| Jumper wires | ~20 | Connections |
| Breadboard | 1 | Circuit assembly |

### Pin Connections Explained

#### LEDs (Outputs)

**Why these pins?**
- We need 8 GPIO pins configured as outputs
- Distributed across different ports for better organization
- These pins are available and not used by other peripherals

| LED | Pin | Port | Physical Location | Purpose |
|-----|-----|------|-------------------|---------|
| 1 | PB1 | GPIOB | Leftmost | Left player's side |
| 2 | PB2 | GPIOB | | Left player's side |
| 3 | PB11 | GPIOB | | Center-left |
| 4 | PB12 | GPIOB | | Center-left |
| 5 | PA11 | GPIOA | | Center-right |
| 6 | PA12 | GPIOA | | Center-right |
| 7 | PC5 | GPIOC | | Right player's side |
| 8 | PC6 | GPIOC | Rightmost | Right player's side |

**Connection:**
```
STM32 Pin (e.g., PB1) ──> LED Anode (+) ──> LED Cathode (-) ──> GND
                          (long leg)         (short leg)

Note: Some boards have built-in resistors; otherwise add 220Ω resistor in series
```

#### Buttons (Inputs)

**Why these pins?**
- Need GPIO pins configured as inputs with pull-up resistors
- Selected pins that support internal pull-up configuration

| Button | Pin | Port | Active State | Purpose |
|--------|-----|------|--------------|---------|
| Left | PB15 | GPIOB | Active LOW | Left player control |
| Right | PC8 | GPIOC | Active LOW | Right player control |

**Connection:**
```
STM32 Pin (e.g., PB15) ──> Button ──> GND
         │
         └──> Internal Pull-Up Resistor (3.3V)

When button is NOT pressed: Pin reads HIGH (1)
When button IS pressed: Pin reads LOW (0) - "Active LOW"
```

**Why Active LOW?**
- Simpler wiring (button connects directly to ground)
- Internal pull-up keeps signal HIGH when not pressed
- Pressing button creates solid ground connection (LOW)
- More reliable than active HIGH in noisy environments

### Circuit Diagram

```
                    STM32 Nucleo-L476RG
                   ┌─────────────────────┐
                   │                     │
    LED1  ◄────────┤ PB1                 │
    LED2  ◄────────┤ PB2                 │
    LED3  ◄────────┤ PB11                │
    LED4  ◄────────┤ PB12                │
    LED5  ◄────────┤ PA11                │
    LED6  ◄────────┤ PA12                │
    LED7  ◄────────┤ PC5                 │
    LED8  ◄────────┤ PC6                 │
                   │                     │
Left BTN ──────────┤ PB15                │
Right BTN ─────────┤ PC8                 │
                   │                     │
    GND ───────────┤ GND                 │
                   │                     │
                   └─────────────────────┘
```

---

## 🏗️ Software Architecture

### Modular Design Philosophy

**Why Modular?**
- Each module has ONE clear responsibility
- Easy to test individual components
- Easy to understand and maintain
- Reusable in other projects

### Module Breakdown

```
┌─────────────────────────────────────────────────────────────┐
│                        main.c                               │
│                   (Game Logic Layer)                        │
│  - State machine implementation                             │
│  - Game rules and scoring                                   │
│  - Overall game flow control                                │
└────────┬──────────────┬──────────────┬──────────────┬───────┘
         │              │              │              │
         v              v              v              v
    ┌────────┐    ┌─────────┐    ┌────────┐    ┌────────┐
    │ leds.c │    │button.c │    │timer.c │    │score.c │
    │        │    │         │    │        │    │        │
    │Control │    │ Button  │    │  Non-  │    │ Score  │
    │  LEDs  │    │handling │    │blocking│    │display │
    │        │    │+ debounce   │ timing │    │        │
    └────────┘    └─────────┘    └────────┘    └────────┘
         │              │              │              │
         └──────────────┴──────────────┴──────────────┘
                            │
                            v
                   ┌────────────────┐
                   │  STM32 HAL     │
                   │  (Hardware)    │
                   └────────────────┘
```

### Layer Responsibilities

#### 1. Hardware Abstraction Layer (HAL)
- Provided by ST Microelectronics
- Direct hardware control
- Functions: `HAL_GPIO_WritePin()`, `HAL_GPIO_ReadPin()`, `HAL_GetTick()`, etc.

#### 2. Driver Layer (leds.c, button.c, timer.c)
- Wraps HAL functions with game-specific interfaces
- Hides hardware details from game logic
- Example: `leds_index(5)` instead of multiple `HAL_GPIO_WritePin()` calls

#### 3. Application Layer (main.c, score.c)
- Implements game rules and logic
- Uses driver layer functions
- Contains state machine

---

## 🚀 Project Setup from Scratch

### Step 1: Create New STM32 Project

1. **Launch STM32CubeIDE**
   ```
   File → New → STM32 Project
   ```

2. **Select Target**
   - Board Selector tab
   - Search for "NUCLEO-L476RG"
   - Select the board
   - Click "Next"

3. **Project Configuration**
   - Project Name: `pingpong`
   - Location: Choose your workspace
   - Click "Finish"
   - Initialize peripherals: "Yes"

### Step 2: Configure GPIO Pins in CubeMX

**Why CubeMX?**
- Visual pin configuration tool
- Generates initialization code automatically
- Prevents pin conflicts
- Configures clock settings

**Configuration Steps:**

1. **Open .ioc file** (double-click `pingpong.ioc` in project explorer)

2. **Configure LED Pins (Outputs)**
   - Click on pin PB1 → Select "GPIO_Output"
   - Repeat for: PB2, PB11, PB12, PA11, PA12, PC5, PC6
   - For each pin:
     - Right-click → Enter User Label (e.g., "LED1", "LED2", etc.)
     - Configuration tab → GPIO mode: Output Push Pull
     - GPIO Pull-up/Pull-down: No pull-up and no pull-down
     - Maximum output speed: Low

3. **Configure Button Pins (Inputs)**
   - Click on pin PB15 → Select "GPIO_Input"
   - Click on pin PC8 → Select "GPIO_Input"
   - For each pin:
     - User Label: "LEFT_BUTTON", "RIGHT_BUTTON"
     - GPIO mode: Input mode
     - GPIO Pull-up/Pull-down: Pull-up
     - (This enables internal pull-up resistor)

4. **Generate Code**
   ```
   Project → Generate Code
   ```

**What Code Was Generated?**
- `main.c`: Main program file with initialization functions
- `stm32l4xx_it.c`: Interrupt handlers
- `main.h`: Pin definitions and constants
- `MX_GPIO_Init()`: Function that configures all pins

### Step 3: Create Module Files

Create these files in your project:

**Header Files (Core/Inc/):**
- `leds.h`
- `button.h`
- `timer.h`
- `score.h`

**Source Files (Core/Src/):**
- `leds.c`
- `button.c`
- `timer.c`
- `score.c`

**In STM32CubeIDE:**
```
Right-click on Core/Inc → New → File → leds.h
Right-click on Core/Src → New → File → leds.c
(Repeat for other modules)
```

---

## 🔍 Module Implementation Deep Dive

### Module 1: LED Control (leds.c/h)

#### Purpose
Provide simple interface to control 8 LEDs without exposing hardware details.

#### Key Concepts

**1. Pin Mapping Array**
```c
typedef struct {
    GPIO_TypeDef* port;  // Which GPIO port (GPIOA, GPIOB, GPIOC)
    uint16_t pin;        // Which pin number (GPIO_PIN_1, GPIO_PIN_2, etc.)
} LED_Pin;

static const LED_Pin led_pins[8] = {
    {GPIOB, GPIO_PIN_1},   // LED 1
    {GPIOB, GPIO_PIN_2},   // LED 2
    // ... etc
};
```

**Why this approach?**
- Centralized pin definitions
- Easy to change hardware without modifying game logic
- Loop through all LEDs easily
- Type-safe access to hardware

**2. Function: leds_index(int i)**

**Purpose:** Light up ONE specific LED, turn off all others

**Implementation Logic:**
```c
void leds_index(int i) {
    // 1. Validate input
    if (i < 1 || i > 8) return;  // Safety check

    // 2. Turn off ALL LEDs first
    for (int j = 0; j < 8; j++) {
        HAL_GPIO_WritePin(led_pins[j].port, led_pins[j].pin, GPIO_PIN_RESET);
    }

    // 3. Turn on ONLY the requested LED
    HAL_GPIO_WritePin(led_pins[i-1].port, led_pins[i-1].pin, GPIO_PIN_SET);
    //                          ^
    //                          Note: Array is 0-indexed, but function uses 1-8
}
```

**Key Points:**
- Input range: 1-8 (user-friendly, matches physical LED numbering)
- Array index: 0-7 (C arrays are 0-indexed)
- Conversion: `i-1` converts user input to array index
- Atomic operation: Ensures only one LED is on at a time

**3. Function: leds_clear()**

**Purpose:** Turn off all LEDs

```c
void leds_clear(void) {
    for (int i = 0; i < 8; i++) {
        HAL_GPIO_WritePin(led_pins[i].port, led_pins[i].pin, GPIO_PIN_RESET);
        //                                                     ^^^^^^^^^^^^^^^
        //                                                     0 = LED OFF
    }
}
```

**When used:**
- Between game states
- After score display
- During transitions

**4. Function: leds_all()**

**Purpose:** Turn on all LEDs simultaneously

```c
void leds_all(void) {
    for (int i = 0; i < 8; i++) {
        HAL_GPIO_WritePin(led_pins[i].port, led_pins[i].pin, GPIO_PIN_SET);
        //                                                     ^^^^^^^^^^^^^
        //                                                     1 = LED ON
    }
}
```

**When used:**
- Visual feedback (flash on miss)
- Game start animation
- Winner celebration

---

### Module 2: Button Handling (button.c/h)

#### Purpose
Read button presses reliably, handling debouncing and preventing multiple triggers from one press.

#### Key Concepts

**1. Button Debouncing - Why It's Needed**

**The Problem:**
When you press a physical button, the contacts don't make a clean connection immediately. They "bounce" multiple times in a few milliseconds:

```
Button Press:
Time:     0ms    5ms    10ms   15ms   20ms   25ms
Signal:   HIGH   LOW    HIGH   LOW    LOW    LOW
          ↑      ↑      ↑      ↑      ↑      ↑
          Not    False  False  False  Stable Stable
          pressed press press  press  press  press
```

Without debouncing, one physical press might register as 3-5 presses!

**The Solution: Software Debouncing**

```c
#define DEBOUNCE_DELAY_MS  20  // Ignore inputs for 20ms after a press

static uint32_t last_press_time = 0;  // Timestamp of last button press

int button_read(void) {
    uint32_t current_time = HAL_GetTick();  // Current time in ms

    // If less than 20ms since last press, ignore this read
    if ((current_time - last_press_time) < DEBOUNCE_DELAY_MS) {
        return 0;  // Still in debounce period
    }

    // Rest of button reading logic...
}
```

**How it works:**
1. Record timestamp when button is pressed
2. Ignore all button reads for next 20ms
3. After 20ms, button is stable and can be read again

**2. Edge Detection - Press vs Hold**

**The Problem:**
If we just check if button is pressed, we'd get:
```
Button held for 1 second = 1000+ detections (checked every millisecond)
```

**The Solution: Detect Falling Edge**

A "falling edge" is the transition from HIGH (not pressed) to LOW (pressed):

```c
static uint8_t left_button_prev_state = 1;   // Remember previous state

int button_read(void) {
    // Read current state
    uint8_t left_current = HAL_GPIO_ReadPin(LEFT_BUTTON_PORT, LEFT_BUTTON_PIN);

    // Detect falling edge (was HIGH, now LOW)
    if (left_button_prev_state == 1 && left_current == 0) {
        // Button was just pressed!
        left_button_prev_state = left_current;  // Update for next time
        return LEFT_BUTTON;
    }

    // Update state for next call
    left_button_prev_state = left_current;
    return 0;
}
```

**State Diagram:**
```
Button State:     [HIGH] ──press──> [LOW] ──release──> [HIGH]
                    ↑                 ↑                   ↑
Detection:       Not pressed      DETECTED!          Not pressed
                                 (falling edge)
```

**Why this works:**
- Only returns button press ONCE per physical press
- Holding button down doesn't trigger multiple times
- Must release and press again to trigger again

**3. Complete button_read() Logic**

```c
int button_read(void) {
    // Step 1: Check debounce timing
    uint32_t current_time = HAL_GetTick();
    if ((current_time - last_press_time) < DEBOUNCE_DELAY_MS) {
        return 0;  // Still debouncing
    }

    // Step 2: Read current button states
    uint8_t left_current = HAL_GPIO_ReadPin(LEFT_BUTTON_PORT, LEFT_BUTTON_PIN);
    uint8_t right_current = HAL_GPIO_ReadPin(RIGHT_BUTTON_PORT, RIGHT_BUTTON_PIN);

    int result = 0;

    // Step 3: Check for right button falling edge
    if (right_button_prev_state == 1 && right_current == 0) {
        result = RIGHT_BUTTON;
        last_press_time = current_time;  // Start debounce timer
    }
    // Step 4: Check for left button falling edge
    else if (left_button_prev_state == 1 && left_current == 0) {
        result = LEFT_BUTTON;
        last_press_time = current_time;  // Start debounce timer
    }

    // Step 5: Update previous states
    left_button_prev_state = left_current;
    right_button_prev_state = right_current;

    return result;
}
```

**Flow Chart:**
```
┌─────────────────────┐
│ Check debounce time │
└──────────┬──────────┘
           │
     ┌─────▼──────┐
     │ Still in   │ Yes ──> Return 0
     │ debounce?  │
     └─────┬──────┘
           │ No
           ▼
┌────────────────────┐
│ Read button states │
└──────────┬─────────┘
           │
     ┌─────▼─────────┐
     │ Right button  │ Yes ──> Return RIGHT_BUTTON
     │ falling edge? │         Update debounce timer
     └─────┬─────────┘
           │ No
     ┌─────▼─────────┐
     │ Left button   │ Yes ──> Return LEFT_BUTTON
     │ falling edge? │         Update debounce timer
     └─────┬─────────┘
           │ No
           ▼
     Return 0
```

---

### Module 3: Non-Blocking Timer (timer.c/h)

#### Purpose
Allow the program to wait for time to pass WITHOUT freezing (blocking) the entire system.

#### The Problem with HAL_Delay()

**Blocking Delay:**
```c
leds_index(5);      // Light LED 5
HAL_Delay(200);     // FREEZE HERE for 200ms - can't check buttons!
leds_index(6);      // Light LED 6
```

**Problem:** During `HAL_Delay()`, the CPU is stuck. It can't:
- Check for button presses
- Update display
- Respond to anything

**Non-Blocking Delay:**
```c
leds_index(5);          // Light LED 5
timer_init(200);        // Start 200ms timer
while (!timer_now()) {  // While timer hasn't expired...
    button_read();      // Can still check buttons!
    // Can do other work here
}
leds_index(6);          // Timer expired, continue
```

#### How It Works

**1. System Tick Counter**

STM32 has a built-in millisecond counter called SysTick:
```c
uint32_t HAL_GetTick(void);  // Returns milliseconds since boot
```

**Example:**
```
Boot time:  HAL_GetTick() = 0
1 second:   HAL_GetTick() = 1000
1 minute:   HAL_GetTick() = 60000
1 hour:     HAL_GetTick() = 3600000
```

**2. Timer Implementation**

```c
// Global variables to store timer state
static uint32_t timer_start_time = 0;  // When timer started
static uint32_t timer_duration = 0;    // How long to wait

void timer_init(uint32_t ms) {
    timer_start_time = HAL_GetTick();  // Record current time
    timer_duration = ms;                // Store duration
}

int timer_now(void) {
    uint32_t current_time = HAL_GetTick();          // Get current time
    uint32_t elapsed = current_time - timer_start_time;  // Calculate elapsed

    return (elapsed >= timer_duration);  // Has enough time passed?
}
```

**3. Usage Pattern**

```c
// Wait for 500ms while checking buttons
timer_init(500);                 // Start timer
while (!timer_now()) {           // While not expired
    int button = button_read();  // Check buttons
    if (button) {
        // Can respond immediately!
    }
}
// Timer expired, continue
```

**Visual Timeline:**
```
Time (ms):     0        100       200       300       400       500
               ↓         ↓         ↓         ↓         ↓         ↓
timer_init()  [START]
timer_now()            false     false     false     false     false     TRUE!
button_read()    ✓         ✓         ✓         ✓         ✓         ✓
               Can check  buttons   every     loop      iteration while
               waiting!
```

---

### Module 4: Score Display (score.c/h)

#### Purpose
Visualize scores and winner using the LED array.

#### Score Display Logic

**Concept:** Use LEDs to show each player's score
- Left player: Light LEDs from left (1, 2, 3, 4)
- Right player: Light LEDs from right (8, 7, 6, 5)

**Example Displays:**

```
Left: 2, Right: 3
[ON][ON][OFF][OFF]  [OFF][ON][ON][ON]
 LED 1-2 lit           LED 8-7-6 lit

Left: 0, Right: 4
[OFF][OFF][OFF][OFF]  [ON][ON][ON][ON]
 No LEDs               All right LEDs

Left: 4, Right: 1
[ON][ON][ON][ON]  [OFF][OFF][OFF][ON]
 All left LEDs         LED 8 only
```

**Implementation:**
```c
void show_score(uint8_t right_score, uint8_t left_score, uint32_t duration_ms) {
    leds_clear();  // Clear display first
    HAL_Delay(100);  // Brief pause

    // Light left player's LEDs (1-4)
    for (int i = 1; i <= left_score && i <= 4; i++) {
        // Turn on LED i for left player
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1 << (i-1), GPIO_PIN_SET);
    }

    // Light right player's LEDs (8-7-6-5)
    if (right_score >= 1) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);  // LED 8
    if (right_score >= 2) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);  // LED 7
    if (right_score >= 3) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET); // LED 6
    if (right_score >= 4) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET); // LED 5

    HAL_Delay(duration_ms);  // Display for specified time
    leds_clear();  // Clear after display
}
```

#### Winner Display

**Animation:** Flash winner's LEDs multiple times

```c
void show_winner(uint8_t winner, uint32_t duration_ms) {
    for (int i = 0; i < 5; i++) {  // Blink 5 times
        if (winner == 0) {
            // Left player wins - light LEDs 1-4
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
        } else {
            // Right player wins - light LEDs 5-8
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
        }

        HAL_Delay(300);  // LEDs on
        leds_clear();
        HAL_Delay(200);  // LEDs off
    }

    // Grand finale - all LEDs
    leds_all();
    HAL_Delay(500);
    leds_clear();
}
```

---

## 🎮 State Machine Explained

### What is a State Machine?

A **state machine** is a way to organize complex behavior by breaking it into:
1. **States**: Distinct modes of operation
2. **Transitions**: Rules for moving between states
3. **Events**: Things that trigger transitions
4. **Actions**: What happens in each state

**Analogy: Traffic Light**
```
States: [RED] [YELLOW] [GREEN]
Transitions: RED → GREEN (timer expires)
             GREEN → YELLOW (timer expires)
             YELLOW → RED (timer expires)
Actions: RED = stop, GREEN = go, YELLOW = slow down
```

### Our Game State Machine

#### States Defined

```c
typedef enum {
    GAME_START,          // Initialize new round
    BALL_MOVING_RIGHT,   // Ball traveling toward right player
    BALL_MOVING_LEFT,    // Ball traveling toward left player
    POINT_SCORED,        // A point was just scored
    GAME_OVER            // Game has ended
} GameState;
```

#### State Transition Diagram

```
                        ┌──────────────┐
                        │  GAME_START  │
                        │              │
                        │ Actions:     │
                        │ - Reset ball │
                        │ - Random dir │
                        │ - Reset speed│
                        └───┬──────────┘
                            │
              ┌─────────────┴─────────────┐
              │                           │
        Random Left                  Random Right
              │                           │
              v                           v
    ┌──────────────────┐         ┌──────────────────┐
    │ BALL_MOVING_LEFT │         │ BALL_MOVING_RIGHT│
    │                  │         │                  │
    │ Actions:         │         │ Actions:         │
    │ - Show ball LED  │         │ - Show ball LED  │
    │ - Check left btn │         │ - Check right btn│
    │ - Move ball left │         │ - Move ball right│
    └────┬────────┬────┘         └────┬────────┬────┘
         │        │                   │        │
         │        │                   │        │
    Hit at LED1  Miss            Hit at LED8  Miss
    (Button press) (Ball off)   (Button press) (Ball off)
         │        │                   │        │
         │        └───────┬───────────┘        │
         │                │                    │
         └────────────────┼────────────────────┘
                          │
                          v
                   ┌──────────────┐
                   │ POINT_SCORED │
                   │              │
                   │ Actions:     │
                   │ - Show score │
                   │ - Check win  │
                   └──────┬───────┘
                          │
                  ┌───────┴────────┐
                  │                │
            Score < 5         Score >= 5
                  │                │
                  v                v
           Back to START    ┌────────────┐
                            │ GAME_OVER  │
                            │            │
                            │ Actions:   │
                            │ - Winner!  │
                            │ - Reset    │
                            └─────┬──────┘
                                  │
                           Back to START
```

#### State Table

| Current State | Event | Next State | Actions |
|--------------|-------|------------|---------|
| GAME_START | Random = Right | BALL_MOVING_RIGHT | Reset ball to center, speed = 200ms |
| GAME_START | Random = Left | BALL_MOVING_LEFT | Reset ball to center, speed = 200ms |
| BALL_MOVING_RIGHT | Button press at LED 8 | BALL_MOVING_LEFT | Reverse direction, increase speed |
| BALL_MOVING_RIGHT | Ball reaches end (pos > 8) | POINT_SCORED | Left player scores, flash LEDs |
| BALL_MOVING_LEFT | Button press at LED 1 | BALL_MOVING_RIGHT | Reverse direction, increase speed |
| BALL_MOVING_LEFT | Ball reaches end (pos < 1) | POINT_SCORED | Right player scores, flash LEDs |
| POINT_SCORED | Score < 5 | GAME_START | Continue game |
| POINT_SCORED | Score >= 5 | GAME_OVER | Show winner |
| GAME_OVER | After display | GAME_START | Reset scores, new game |

---

## 🎯 Game Logic Implementation

### Main Game Loop Structure

```c
void ping_pong_game(void) {
    GameState state = GAME_START;
    int ball_position = 4;
    uint32_t ball_speed = INITIAL_SPEED;
    uint8_t left_score = 0;
    uint8_t right_score = 0;

    // Initialize game
    leds_clear();
    HAL_Delay(500);

    // Flash LEDs to signal game start
    for (int i = 0; i < 3; i++) {
        leds_all();
        HAL_Delay(200);
        leds_clear();
        HAL_Delay(200);
    }

    // Main game loop - runs forever
    while (1) {
        switch (state) {
            case GAME_START:
                // Initialize new round
                break;

            case BALL_MOVING_RIGHT:
                // Handle ball moving right
                break;

            case BALL_MOVING_LEFT:
                // Handle ball moving left
                break;

            case POINT_SCORED:
                // Handle scoring
                break;

            case GAME_OVER:
                // Handle game end
                break;
        }
    }
}
```

### GAME_START State

**Purpose:** Initialize a new round

```c
case GAME_START:
    // Reset ball to center
    ball_position = 4;  // LED 4 (center-left)

    // Randomly choose starting direction
    if ((HAL_GetTick() % 2) == 0) {
        // Even tick = move right
        ball_direction = 1;
        state = BALL_MOVING_RIGHT;
    } else {
        // Odd tick = move left
        ball_direction = -1;
        state = BALL_MOVING_LEFT;
    }

    // Reset ball speed for new round
    ball_speed = INITIAL_SPEED;  // 200ms
    break;
```

**Why random direction?**
- Makes game unpredictable
- Fairer for both players
- Uses system tick as pseudo-random number (constantly changing)

### BALL_MOVING_RIGHT State

**Purpose:** Handle ball moving toward right player

```c
case BALL_MOVING_RIGHT:
    // 1. Display ball at current position
    leds_index(ball_position);

    // 2. Start non-blocking timer
    timer_init(ball_speed);

    // 3. Wait for timer while checking for button press
    while (!timer_now()) {
        button_pressed = button_read();

        // Check if right player hit the ball
        if (button_pressed == RIGHT_BUTTON && ball_position == 8) {
            // SUCCESSFUL HIT!
            ball_direction = -1;  // Reverse direction
            state = BALL_MOVING_LEFT;  // Change state

            // Increase difficulty - speed up ball
            if (ball_speed > MIN_SPEED) {
                ball_speed -= SPEED_DECREASE;  // Lower delay = faster
            }

            break;  // Exit while loop
        }
    }

    // 4. Timer expired - move ball to next position
    if (state == BALL_MOVING_RIGHT) {  // Still moving right (not hit)
        ball_position++;  // Move one LED to the right

        // Check if ball went off the right end
        if (ball_position > 8) {
            // RIGHT PLAYER MISSED!
            left_score++;  // Left player scores
            state = POINT_SCORED;

            // Visual feedback - flash LEDs
            for (int i = 0; i < 3; i++) {
                leds_all();
                HAL_Delay(100);
                leds_clear();
                HAL_Delay(100);
            }
        }
    }
    break;
```

**Key Points:**

1. **Non-blocking timing:** Game can check buttons during wait
2. **Hit detection:** Only counts if button pressed when `ball_position == 8`
3. **State change:** Immediately switches to `BALL_MOVING_LEFT` on hit
4. **Speed increase:** Ball gets faster with each hit (max = MIN_SPEED)
5. **Miss detection:** If ball goes past LED 8 without hit, opponent scores

### BALL_MOVING_LEFT State

**Purpose:** Handle ball moving toward left player

```c
case BALL_MOVING_LEFT:
    // Identical logic to BALL_MOVING_RIGHT, but:
    // - Check LEFT_BUTTON
    // - Check ball_position == 1 (not 8)
    // - Move ball left (ball_position--)
    // - Check ball_position < 1 (not > 8)
    // - Right player scores on miss (not left)

    leds_index(ball_position);
    timer_init(ball_speed);

    while (!timer_now()) {
        button_pressed = button_read();

        if (button_pressed == LEFT_BUTTON && ball_position == 1) {
            // Left player hit!
            ball_direction = 1;
            state = BALL_MOVING_RIGHT;

            if (ball_speed > MIN_SPEED) {
                ball_speed -= SPEED_DECREASE;
            }
            break;
        }
    }

    if (state == BALL_MOVING_LEFT) {
        ball_position--;

        if (ball_position < 1) {
            // Left player missed
            right_score++;
            state = POINT_SCORED;

            for (int i = 0; i < 3; i++) {
                leds_all();
                HAL_Delay(100);
                leds_clear();
                HAL_Delay(100);
            }
        }
    }
    break;
```

### POINT_SCORED State

**Purpose:** Handle scoring and check for game end

```c
case POINT_SCORED:
    // 1. Display current score
    show_score(right_score, left_score, SCORE_DISPLAY_TIME);

    // 2. Check if either player won
    if (left_score >= WINNING_SCORE) {
        // Left player wins!
        show_winner(0, 3000);  // 0 = left
        state = GAME_OVER;
    }
    else if (right_score >= WINNING_SCORE) {
        // Right player wins!
        show_winner(1, 3000);  // 1 = right
        state = GAME_OVER;
    }
    else {
        // Game continues - start new round
        state = GAME_START;
    }
    break;
```

**Flow:**
```
Point scored
    ↓
Display score (2 seconds)
    ↓
Check scores
    ↓
    ├─ Left >= 5? → Show winner → GAME_OVER
    ├─ Right >= 5? → Show winner → GAME_OVER
    └─ Neither → GAME_START (new round)
```

### GAME_OVER State

**Purpose:** End game and prepare for restart

```c
case GAME_OVER:
    // 1. Pause before final score
    HAL_Delay(1000);

    // 2. Show final score one more time
    show_score(right_score, left_score, 3000);

    // 3. Reset game
    left_score = 0;
    right_score = 0;
    state = GAME_START;

    // 4. Delay before new game
    HAL_Delay(2000);

    // 5. Signal new game starting
    for (int i = 0; i < 2; i++) {
        leds_all();
        HAL_Delay(300);
        leds_clear();
        HAL_Delay(300);
    }
    break;
```

### Game Constants and Tuning

```c
#define WINNING_SCORE      5    // Points to win
#define INITIAL_SPEED    200    // Starting ball speed (ms)
#define MIN_SPEED        100    // Fastest ball speed (ms)
#define SPEED_DECREASE    20    // Speed increase per hit (ms)
#define SCORE_DISPLAY_TIME 2000 // Score display duration (ms)
```

**Speed Progression Example:**
```
Hit 0: 200ms per LED (slow)
Hit 1: 180ms per LED
Hit 2: 160ms per LED
Hit 3: 140ms per LED
Hit 4: 120ms per LED
Hit 5: 100ms per LED (max speed - stays here)
```

**After miss:** Speed resets to 200ms

---

## 🐛 Debugging and Testing

### Build Process

**1. Build the Project**
```
Right-click on project → Build Project
Or: Press Ctrl+B
```

**What happens during build:**
1. Compiler (arm-none-eabi-gcc) compiles each .c file to .o object file
2. Linker combines all .o files into single .elf executable
3. Creates .bin and .hex files for flashing

**Build Output Location:**
```
pingpong/
└── Debug/
    ├── pingpong.elf   (Main executable with debug symbols)
    ├── pingpong.bin   (Binary for flashing)
    └── pingpong.map   (Memory map)
```

### Common Build Errors

#### Error: "undefined reference to function"
```
undefined reference to `leds_init'
```
**Cause:** Function declared in .h but not implemented in .c, or .c file not added to project

**Fix:**
1. Check .c file exists
2. Right-click project → Properties → C/C++ Build → Settings
3. Verify .c file is in source list

#### Error: "No such file or directory"
```
fatal error: leds.h: No such file or directory
```
**Cause:** Header file not found

**Fix:**
1. Verify .h file is in Core/Inc/
2. Include correct path: `#include "leds.h"` (not `#include <leds.h>`)

#### Warning: "implicit declaration of function"
```
warning: implicit declaration of function 'HAL_GPIO_WritePin'
```
**Cause:** Missing include

**Fix:** Add `#include "stm32l4xx_hal.h"` at top of file

### Flashing and Running

**1. Connect Hardware**
- Connect STM32 board via USB
- Connect LEDs and buttons to correct pins
- Verify connections with multimeter if unsure

**2. Create Debug Configuration**
```
Click Debug button dropdown → Debug Configurations
Double-click "STM32 Cortex-M C/C++ Application"
Configuration:
  - Name: pingpong Debug
  - C/C++ Application: Debug/pingpong.elf
  - Debug probe: ST-LINK (ST-LINK GDB server)
  - Interface: SWD
Click Apply, then Debug
```

**3. Debug Perspective**

When debug starts, you'll see:
- **Code window:** Current execution line highlighted
- **Variables window:** View variable values
- **Expressions window:** Watch specific variables
- **Call Stack:** Function call hierarchy
- **Breakpoints:** List of breakpoints

**Debug Controls:**
- **Resume (F8):** Run until next breakpoint
- **Suspend:** Pause execution
- **Terminate:** Stop debugging
- **Step Over (F6):** Execute current line, don't enter functions
- **Step Into (F5):** Execute current line, enter functions
- **Step Return:** Run until current function returns

### Testing Strategy

#### Phase 1: Test LEDs

**Test Code:**
```c
// In main(), replace ping_pong_game() with:
test_leds();
```

**Expected behavior:**
1. LEDs 1-8 cycle slowly (500ms each)
2. Short pause
3. LEDs 1-8 cycle quickly (100ms each)
4. LEDs 8-1 cycle backwards (100ms each)
5. All LEDs flash 3 times
6. Repeat

**If LEDs don't work:**
- Check wiring (LED polarity!)
- Verify pin configuration in .ioc file
- Test with multimeter (should see ~3.3V on active pins)
- Check LED current limiting resistors

#### Phase 2: Test Buttons

**Test Code:**
```c
while (1) {
    int button = button_read();
    if (button == LEFT_BUTTON) {
        leds_index(1);  // Show left button pressed
        HAL_Delay(500);
        leds_clear();
    } else if (button == RIGHT_BUTTON) {
        leds_index(8);  // Show right button pressed
        HAL_Delay(500);
        leds_clear();
    }
}
```

**Expected behavior:**
- Press left button → LED 1 lights for 0.5s
- Press right button → LED 8 lights for 0.5s
- Holding button doesn't repeat (edge detection working)

**If buttons don't work:**
- Check wiring
- Verify pull-up resistors enabled
- Test with multimeter (should read 3.3V normally, 0V when pressed)
- Check debounce timing (maybe too long?)

#### Phase 3: Test Timer

**Test Code:**
```c
while (1) {
    leds_index(1);
    timer_init(1000);  // 1 second
    while (!timer_now()) {
        // Wait non-blocking
    }

    leds_index(8);
    timer_init(1000);
    while (!timer_now()) {
        // Wait non-blocking
    }
}
```

**Expected behavior:**
- LED 1 on for 1 second
- LED 8 on for 1 second
- Repeat

**Timing check:**
- Should be accurate to ±10ms
- Use stopwatch to verify

#### Phase 4: Test Score Display

**Test Code:**
```c
while (1) {
    show_score(0, 0, 2000);  // 0-0
    show_score(1, 2, 2000);  // 1-2
    show_score(3, 1, 2000);  // 3-1
    show_score(4, 4, 2000);  // 4-4
    show_winner(0, 3000);    // Left wins
    show_winner(1, 3000);    // Right wins
}
```

**Expected behavior:**
- Various score displays
- Winner animations
- All LEDs used correctly

#### Phase 5: Test Full Game

**Test checklist:**
- [ ] Game starts with 3 flashes
- [ ] Ball appears and moves
- [ ] Left button bounces ball at LED 1
- [ ] Right button bounces ball at LED 8
- [ ] Ball speeds up after hits
- [ ] Missing ball causes score
- [ ] Score displays correctly
- [ ] Game ends at 5 points
- [ ] Winner animation plays
- [ ] Game restarts automatically

### Using Breakpoints

**Setting Breakpoints:**
1. Double-click in left margin of code line
2. Red dot appears

**Strategic Breakpoint Locations:**

```c
// In BALL_MOVING_RIGHT state
if (button_pressed == RIGHT_BUTTON && ball_position == 8) {
    // Set breakpoint HERE to verify hit detection
    ball_direction = -1;
    state = BALL_MOVING_LEFT;
}

// In POINT_SCORED state
case POINT_SCORED:
    // Set breakpoint HERE to check scores
    show_score(right_score, left_score, SCORE_DISPLAY_TIME);
```

**Inspecting Variables:**
1. Hover over variable name
2. Or add to Expressions window
3. See current value during breakpoint

**Example Debug Session:**
```
1. Set breakpoint in BALL_MOVING_RIGHT state
2. Run game
3. When ball reaches LED 8, press right button
4. Breakpoint hits
5. Check variables:
   - ball_position should be 8
   - button_pressed should be RIGHT_BUTTON (2)
   - State will change to BALL_MOVING_LEFT
6. Press F8 to continue
```

### Logic Analyzer (Advanced)

**View GPIO pin timing:**
1. Debug perspective → Windows → Show View → SWV → SWV Data Trace Timeline Graph
2. Configure which pins to monitor
3. See exact timing of LED on/off

---

## ❗ Common Issues and Solutions

### Issue 1: Button Presses Not Detected

**Symptoms:**
- Ball passes end without detection
- Button works sometimes but not always

**Possible Causes & Solutions:**

1. **Debounce delay too long**
   ```c
   #define DEBOUNCE_DELAY_MS  20  // Try reducing to 10
   ```

2. **Wrong button timing window**
   ```c
   // Check you're testing at correct position
   if (button_pressed == RIGHT_BUTTON && ball_position == 8)
   //                                                      ^
   //                                     Correct position?
   ```

3. **Button read not in timer loop**
   ```c
   while (!timer_now()) {
       button_pressed = button_read();  // MUST be inside loop!
   }
   ```

4. **Pull-up not enabled**
   - Check .ioc file → Pin Configuration → Pull-up enabled

### Issue 2: Ball Movement Too Fast/Slow

**Symptoms:**
- Can't react fast enough
- Game too boring

**Solutions:**

1. **Adjust initial speed**
   ```c
   #define INITIAL_SPEED    300  // Slower (was 200)
   ```

2. **Adjust minimum speed**
   ```c
   #define MIN_SPEED        150  // Slower max speed (was 100)
   ```

3. **Adjust speed increase**
   ```c
   #define SPEED_DECREASE    10  // Smaller increases (was 20)
   ```

### Issue 3: LEDs Don't Light Up

**Symptoms:**
- No LEDs light
- Some LEDs work, others don't

**Solutions:**

1. **Check polarity**
   - LED long leg = anode (+) → connects to STM32 pin
   - LED short leg = cathode (-) → connects to GND

2. **Check pin configuration**
   ```c
   // In MX_GPIO_Init(), verify:
   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  // Push-pull
   GPIO_InitStruct.Pull = GPIO_NOPULL;          // No pull resistor
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; // Speed setting
   ```

3. **Test individual pins**
   ```c
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
   HAL_Delay(1000);
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
   ```

4. **Check current limiting resistors**
   - Should have ~220Ω resistor per LED
   - Too high = LED too dim
   - Too low = risk burning LED or pin

### Issue 4: Score Display Wrong

**Symptoms:**
- Wrong LEDs light up
- Score shows incorrectly

**Solutions:**

1. **Verify pin mappings**
   ```c
   // Check led_pins array matches your wiring
   static const LED_Pin led_pins[8] = {
       {GPIOB, GPIO_PIN_1},   // LED 1 - verify this pin!
       // ... etc
   };
   ```

2. **Check score display logic**
   ```c
   // Left score should light LEDs 1-4
   for (int i = 1; i <= left_score && i <= 4; i++) {
       // Verify correct port/pin for each LED
   }
   ```

### Issue 5: Build Errors

**Error:** `Multiple definition of 'main'`

**Cause:** Multiple files have `main()` function

**Solution:** Only one .c file should have `main()`, typically `main.c`

---

**Error:** `Region 'RAM' overflowed`

**Cause:** Too many global variables or stack too large

**Solution:**
1. Reduce global variables
2. Use `static` keyword for module-local variables
3. Check for large arrays

---

**Error:** `HAL_GPIO_WritePin undeclared`

**Cause:** Missing include

**Solution:**
```c
#include "stm32l4xx_hal.h"  // Add at top of file
```

### Issue 6: Game Freezes/Hangs

**Symptoms:**
- Game stops responding
- LEDs freeze in one state

**Possible Causes:**

1. **Infinite loop without exit**
   ```c
   while (!timer_now()) {
       // If timer never expires, stuck here!
       // Make sure timer_init() was called
   }
   ```

2. **Missing state transition**
   ```c
   case BALL_MOVING_RIGHT:
       // ... code ...
       // Did you forget to set state = BALL_MOVING_LEFT?
       break;  // Make sure break is here!
   ```

3. **Watchdog timer** (if enabled)
   - Game too slow to pet watchdog
   - Disable watchdog or add `HAL_IWDG_Refresh()` in main loop

**Debug technique:**
1. Add LED toggles to track execution
2. Set breakpoints in each state
3. Step through code with F6/F5

---

## 📝 Study Questions

Test your understanding with these questions:

### Hardware Questions

1. **Q:** Why do we use pull-up resistors on button inputs?
   **A:** To ensure the pin has a defined HIGH state when button is not pressed. Without it, the pin would "float" and read random values.

2. **Q:** What is the difference between active HIGH and active LOW?
   **A:** Active HIGH means signal is 1/HIGH when active. Active LOW means signal is 0/LOW when active. We use active LOW buttons because it's simpler to wire (button to ground).

3. **Q:** Why do we need current-limiting resistors on LEDs?
   **A:** LEDs have very low resistance. Without current limiting, too much current flows, damaging the LED and/or GPIO pin. Resistor limits current to safe level (~10-20mA).

4. **Q:** Can we control more than 8 LEDs with this setup?
   **A:** Yes! The STM32 has many GPIO pins. We could add more LEDs to other available pins and extend the led_pins array.

### Software Questions

5. **Q:** What is the purpose of the `static` keyword in `static const LED_Pin led_pins[8]`?
   **A:** `static` means the variable is only visible within this file (module), preventing name conflicts. `const` means the array cannot be modified at runtime.

6. **Q:** Why do we use `i-1` in `leds_index(int i)` when accessing the array?
   **A:** Users think of LEDs as 1-8 (human-friendly), but C arrays are 0-indexed (0-7). So LED 1 is at array index 0, LED 2 at index 1, etc.

7. **Q:** What would happen if we removed the edge detection in `button_read()`?
   **A:** One button press would register hundreds of times (checked every millisecond). The ball would bounce back and forth rapidly with one press.

8. **Q:** Why is non-blocking timing important in this game?
   **A:** So we can check for button presses during ball movement. With blocking delay (HAL_Delay), we'd miss button presses that occur during the delay.

### State Machine Questions

9. **Q:** What is the purpose of the GAME_START state?
   **A:** Initialize a new round - reset ball position, choose random starting direction, reset ball speed. Provides clean separation between rounds.

10. **Q:** Why do we check `if (state == BALL_MOVING_RIGHT)` before moving the ball?
    **A:** Because if player hit the ball during the timer wait, we already changed state to BALL_MOVING_LEFT. We don't want to move ball right AND change state - that would be wrong.

11. **Q:** What triggers the transition from POINT_SCORED to GAME_OVER?
    **A:** When either player's score reaches WINNING_SCORE (5 points).

12. **Q:** Could we add more states? Give an example.
    **A:** Yes! Examples:
        - MENU state for difficulty selection
        - PAUSED state when both buttons pressed
        - COUNTDOWN state with 3-2-1 before starting

### Design Questions

13. **Q:** Why is the code organized into modules (leds.c, button.c, etc.) instead of all in main.c?
    **A:** Modularity improves:
        - Readability (find specific functionality easily)
        - Testability (test each module independently)
        - Reusability (use button.c in other projects)
        - Maintainability (changes isolated to one module)

14. **Q:** How would you add a third player?
    **A:** Need:
        - Third button (new GPIO pin)
        - Modify button.c to detect third button
        - New state machine state or modify existing states
        - Change from 2-player to 3-player game logic
        - Modify score display for three players

15. **Q:** How could you make the game save high scores?
    **A:** Use STM32's Flash memory or external EEPROM to store scores persistently. Would need:
        - Flash writing functions
        - Score structure
        - Read scores on startup
        - Write scores when they change

### Challenge Questions

16. **Q:** The ball speeds up but never slows down. How would you implement "paddle miss slows ball"?
    **A:** In POINT_SCORED state, reset `ball_speed = INITIAL_SPEED` before going to GAME_START.

17. **Q:** How would you implement "perfect timing bonus" (extra point for hitting at exact moment)?
    **A:** Modify timer to track exact milliseconds. If button pressed within, say, 20ms window of LED reaching end, award bonus point.

18. **Q:** Could you implement this without a state machine? Why or why not?
    **A:** Technically yes, but would be very messy with lots of if-else statements and flags. State machine provides clean structure and makes code much more readable/maintainable.

---

## 🎯 Lab Checklist

Use this checklist to verify your implementation:

### Hardware Setup
- [ ] All 8 LEDs connected to correct pins
- [ ] Current limiting resistors on all LEDs (if needed)
- [ ] Left button connected to PB15 with pull-up
- [ ] Right button connected to PC8 with pull-up
- [ ] All grounds connected
- [ ] Power supplied (USB or external)
- [ ] Connections verified with multimeter

### Project Setup
- [ ] STM32 project created in CubeIDE
- [ ] GPIO pins configured in .ioc file
- [ ] All module files created (.c and .h)
- [ ] All modules included in build
- [ ] Project builds without errors

### Module Implementation
- [ ] leds.c: All 4 functions implemented
- [ ] button.c: Debouncing implemented
- [ ] button.c: Edge detection working
- [ ] timer.c: Non-blocking timer working
- [ ] score.c: Score display working
- [ ] score.c: Winner animation working

### Game Logic
- [ ] State machine with 5 states
- [ ] GAME_START: Initializes correctly
- [ ] BALL_MOVING_RIGHT: Ball moves and detects hit
- [ ] BALL_MOVING_LEFT: Ball moves and detects hit
- [ ] POINT_SCORED: Displays score correctly
- [ ] GAME_OVER: Shows winner and restarts

### Testing
- [ ] LED test mode works (all LEDs cycle)
- [ ] Button test shows button presses
- [ ] Ball moves smoothly across LEDs
- [ ] Ball responds to button at correct time
- [ ] Ball speeds up after each hit
- [ ] Missing ball increments correct player's score
- [ ] Score displays correctly after each point
- [ ] Game ends when player reaches 5 points
- [ ] Winner animation plays
- [ ] Game restarts automatically

### Documentation
- [ ] All functions have comments
- [ ] State machine documented
- [ ] Pin mappings documented
- [ ] README.md created
- [ ] Code is readable and well-formatted

---

## 📚 Additional Resources

### STM32 Resources
- [STM32L476 Reference Manual](https://www.st.com/resource/en/reference_manual/dm00083560.pdf)
- [HAL Library Documentation](https://www.st.com/resource/en/user_manual/dm00105879.pdf)
- [STM32CubeIDE User Guide](https://www.st.com/resource/en/user_manual/dm00603964.pdf)

### C Programming
- Understanding `static` keyword
- Understanding `const` keyword
- Pointer basics (for GPIO port access)
- Bitwise operations (for GPIO pin masking)

### Embedded Systems Concepts
- GPIO configuration
- Pull-up/pull-down resistors
- Debouncing techniques
- State machine design
- Real-time constraints

### Debugging Tools
- Using breakpoints effectively
- Variable inspection
- Step-through debugging
- Logic analyzer usage

---

## 🎓 Conclusion

You now have a complete understanding of:

✅ **Hardware**: GPIO, LEDs, buttons, pull-resistors
✅ **Software**: State machines, modular design, non-blocking code
✅ **Embedded**: Real-time systems, timing, peripheral configuration
✅ **Debugging**: Build process, testing strategies, common issues

**This lab demonstrates:**
- Professional embedded systems development workflow
- Clean code architecture and modularity
- Real-world debugging techniques
- State machine implementation
- Hardware/software integration

**Next Steps:**
1. Study this guide thoroughly
2. Understand each module's purpose
3. Be able to explain the state machine
4. Practice debugging techniques
5. Consider the study questions
6. Think about possible extensions

---

**Good luck with your lab! 🎮🏓**

*This guide prepared for KTH IS1300 Embedded Systems*

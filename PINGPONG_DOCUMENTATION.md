# Ping-Pong Game - STM32L4 Embedded Systems Project

## Table of Contents
1. [Overview](#overview)
2. [Hardware Configuration](#hardware-configuration)
3. [File Structure](#file-structure)
4. [State Machine](#state-machine)
5. [Code Walkthrough](#code-walkthrough)
6. [Game Mechanics](#game-mechanics)
7. [Building and Running](#building-and-running)

---

## Overview

This is an embedded systems project implementing a **ping-pong game** on an STM32L476RG microcontroller. The game uses 8 LEDs to represent a playing field and 2 buttons for player input. A "ball" (represented by a lit LED) moves back and forth, and players must press their button at the correct time to hit it back.

### Key Features
- **8 LEDs** representing the playing field
- **2 buttons** (left and right players)
- **State machine-based** game logic
- **Non-blocking timers** for smooth gameplay
- **Dynamic ball speed** (increases with successful hits)
- **Score tracking** and winner display
- **Visual feedback** for hits, misses, and game events

### Game Rules
1. The ball (lit LED) moves from one side to the other
2. Left player uses LEFT button (PB15) when ball reaches LED 1
3. Right player uses RIGHT button (PC8) when ball reaches LED 8
4. Successful hit: Ball bounces back and speeds up
5. Miss: Opponent scores a point
6. First to 5 points wins
7. Game automatically restarts after displaying winner

---

## Hardware Configuration

### Microcontroller
- **Board**: STM32L476RG Nucleo
- **Architecture**: ARM Cortex-M4
- **Clock**: 80 MHz (PLL from 16 MHz HSI)

### LED Configuration (8 LEDs)

The 8 LEDs are mapped to various GPIO pins across three ports:

| LED Position | Description   | GPIO Port | GPIO Pin | Physical Pin |
|--------------|---------------|-----------|----------|--------------|
| LED 1        | Leftmost      | GPIOB     | Pin 1    | PB1          |
| LED 2        |               | GPIOB     | Pin 2    | PB2          |
| LED 3        |               | GPIOB     | Pin 11   | PB11         |
| LED 4        | Center-left   | GPIOB     | Pin 12   | PB12         |
| LED 5        | Center-right  | GPIOA     | Pin 11   | PA11         |
| LED 6        |               | GPIOA     | Pin 12   | PA12         |
| LED 7        |               | GPIOC     | Pin 5    | PC5          |
| LED 8        | Rightmost     | GPIOC     | Pin 6    | PC6          |

**LED Layout:**
```
[1] [2] [3] [4]     [5] [6] [7] [8]
<-- Left Side -->   <-- Right Side -->
```

All LEDs are configured as:
- Mode: `GPIO_MODE_OUTPUT_PP` (push-pull)
- Pull: `GPIO_NOPULL`
- Speed: `GPIO_SPEED_FREQ_LOW`

### Button Configuration (2 Buttons)

| Button       | Player       | GPIO Port | GPIO Pin      | Active State |
|--------------|--------------|-----------|---------------|--------------|
| Left Button  | Left Player  | GPIOB     | Pin 15 (PB15) | Active LOW   |
| Right Button | Right Player | GPIOC     | Pin 8 (PC8)   | Active LOW   |

Both buttons are configured with:
- Mode: `GPIO_MODE_INPUT`
- Pull: `GPIO_PULLUP` (internal pull-up resistor)
- Active: LOW (button press reads 0)

### Other Peripherals
- **USART2**: 115200 baud, 8-N-1 (for debugging, if needed)
- **LD2**: Onboard LED (PA5) - not used in game

---

## File Structure

```
pingpong/
├── Core/
│   ├── Inc/                      # Header files
│   │   ├── main.h                # Main application header, GPIO defines
│   │   ├── leds.h                # LED control interface
│   │   ├── button.h              # Button handling interface
│   │   ├── timer.h               # Non-blocking timer interface
│   │   ├── score.h               # Score display interface
│   │   ├── stm32l4xx_hal_conf.h  # HAL configuration
│   │   └── stm32l4xx_it.h        # Interrupt handlers header
│   │
│   ├── Src/                      # Source files
│   │   ├── main.c                # Main game logic and state machine
│   │   ├── leds.c                # LED control implementation
│   │   ├── button.c              # Button handling with debouncing
│   │   ├── timer.c               # Non-blocking timer implementation
│   │   ├── score.c               # Score and winner display
│   │   ├── stm32l4xx_it.c        # Interrupt handlers
│   │   ├── stm32l4xx_hal_msp.c   # HAL MSP initialization
│   │   ├── system_stm32l4xx.c    # System clock configuration
│   │   ├── syscalls.c            # System calls (newlib)
│   │   └── sysmem.c              # System memory (newlib)
│   │
│   └── Startup/
│       └── startup_stm32l476rgtx.s  # Startup assembly code
│
├── Drivers/                      # STM32 HAL and CMSIS drivers
│   ├── STM32L4xx_HAL_Driver/     # Hardware Abstraction Layer
│   └── CMSIS/                    # ARM CMSIS interface
│
├── Debug/                        # Build output directory
├── STM32L476RGTX_FLASH.ld        # Linker script (Flash)
├── STM32L476RGTX_RAM.ld          # Linker script (RAM)
└── pingpong.ioc                  # STM32CubeMX configuration file
```

### Module Descriptions

#### 1. **main.c / main.h**
- **Purpose**: Core game logic and state machine implementation
- **Key Functions**:
  - `main()`: Entry point, initializes peripherals and starts game
  - `ping_pong_game()`: Main game loop with state machine
  - `test_leds()`: Hardware test function for LED verification
  - `SystemClock_Config()`: Configures system clock to 80 MHz
  - `MX_GPIO_Init()`: Initializes all GPIO pins
  - `MX_USART2_UART_Init()`: Initializes UART for debugging
- **Lines of Code**: ~643 lines

#### 2. **leds.c / leds.h**
- **Purpose**: Abstraction layer for LED control
- **Key Functions**:
  - `leds_init()`: Initialize LED system
  - `leds_index(int i)`: Light LED at position i (1-8)
  - `leds_clear()`: Turn off all LEDs
  - `leds_all()`: Turn on all LEDs
- **Implementation**: Uses lookup table mapping positions to GPIO pins
- **Lines of Code**: ~82 lines (source), ~133 lines (header with docs)

#### 3. **button.c / button.h**
- **Purpose**: Button input handling with debouncing
- **Key Functions**:
  - `button_init()`: Initialize button state tracking
  - `button_read()`: Read button state (returns `LEFT_BUTTON`, `RIGHT_BUTTON`, or 0)
- **Features**:
  - 20ms software debouncing
  - Edge detection (only triggers on press, not hold)
  - Handles simultaneous presses (right button priority)
- **Lines of Code**: ~91 lines (source), ~79 lines (header)

#### 4. **timer.c / timer.h**
- **Purpose**: Non-blocking timer for game timing
- **Key Functions**:
  - `timer_init(uint32_t ms)`: Start timer with duration
  - `timer_now()`: Check if timer has expired (returns 0 or 1)
- **Implementation**: Uses HAL_GetTick() (1ms system tick)
- **Advantage**: Allows button checking during wait periods
- **Lines of Code**: ~76 lines (source), ~95 lines (header)

#### 5. **score.c / score.h**
- **Purpose**: Visual feedback for scores and winners
- **Key Functions**:
  - `show_score(right, left, duration)`: Display current score on LEDs
  - `show_winner(winner, duration)`: Animate winner celebration
- **Display Logic**:
  - Left score: LEDs 1-4 from left
  - Right score: LEDs 8-5 from right
  - Winner: Blinking animation on winner's side
- **Lines of Code**: ~128 lines (source), ~105 lines (header)

---

## State Machine

The game is implemented as a **finite state machine** with 5 states. The state machine controls all game flow, from initialization to game end.

### State Diagram

```
                    ┌─────────────┐
                    │ GAME_START  │
                    └──────┬──────┘
                           │
            ┌──────────────┴─────────────┐
            │                            │
            ▼                            ▼
    ┌───────────────┐           ┌──────────────┐
    │ BALL_MOVING_  │◄─────────►│ BALL_MOVING_ │
    │    LEFT       │           │    RIGHT     │
    └───────┬───────┘           └───────┬──────┘
            │                           │
            │      Miss detected        │
            └──────────┬────────────────┘
                       │
                       ▼
              ┌────────────────┐
              │ POINT_SCORED   │
              └────────┬───────┘
                       │
                ┌──────┴───────┐
                │              │
                ▼              ▼
         ┌──────────┐    ┌──────────┐
         │Continue  │    │GAME_OVER │
         │(< 5 pts) │    │(>= 5 pts)│
         └────┬─────┘    └────┬─────┘
              │               │
              └───►GAME_START◄┘
```

### State Descriptions

#### 1. **GAME_START**
- **Purpose**: Initialize a new round
- **Actions**:
  - Reset ball to center position (LED 4)
  - Randomly choose starting direction based on system tick
  - Reset ball speed to `INITIAL_SPEED` (200ms)
- **Transitions**:
  - → `BALL_MOVING_RIGHT` if random direction is right
  - → `BALL_MOVING_LEFT` if random direction is left
- **Code Location**: main.c:369-384

#### 2. **BALL_MOVING_RIGHT**
- **Purpose**: Ball traveling toward right player (positions 1→8)
- **Actions**:
  - Display ball at current position using `leds_index()`
  - Start non-blocking timer with `ball_speed`
  - Poll for button presses while timer runs
  - Move ball one position right when timer expires
- **Transitions**:
  - → `BALL_MOVING_LEFT` if right button pressed at LED 8 (successful hit)
  - → `POINT_SCORED` if ball reaches position > 8 (miss)
  - → Stay in state while ball is moving (positions 1-7)
- **Special Logic**:
  - Successful hit: Ball speed increases (min 100ms)
  - Early button press: Ignored (could be penalized)
  - Visual feedback on miss: Flash all LEDs 3 times
- **Code Location**: main.c:389-440

#### 3. **BALL_MOVING_LEFT**
- **Purpose**: Ball traveling toward left player (positions 8→1)
- **Actions**:
  - Display ball at current position using `leds_index()`
  - Start non-blocking timer with `ball_speed`
  - Poll for button presses while timer runs
  - Move ball one position left when timer expires
- **Transitions**:
  - → `BALL_MOVING_RIGHT` if left button pressed at LED 1 (successful hit)
  - → `POINT_SCORED` if ball reaches position < 1 (miss)
  - → Stay in state while ball is moving (positions 8-2)
- **Special Logic**:
  - Successful hit: Ball speed increases (min 100ms)
  - Early button press: Ignored (could be penalized)
  - Visual feedback on miss: Flash all LEDs 3 times
- **Code Location**: main.c:445-496

#### 4. **POINT_SCORED**
- **Purpose**: Handle scoring and check for game end
- **Actions**:
  - Display current score using `show_score()`
  - Check if either player has reached `WINNING_SCORE` (5 points)
  - Show winner if game is over
- **Transitions**:
  - → `GAME_OVER` if left_score >= 5 (show left winner)
  - → `GAME_OVER` if right_score >= 5 (show right winner)
  - → `GAME_START` if game continues (score < 5)
- **Code Location**: main.c:501-518

#### 5. **GAME_OVER**
- **Purpose**: End of game, display final results and restart
- **Actions**:
  - Display final score one more time
  - Reset both scores to 0
  - Flash LEDs to signal new game starting
  - Long delay before restart
- **Transitions**:
  - → `GAME_START` after animations complete
- **Code Location**: main.c:523-543

### State Transition Table

| Current State     | Event                 | Next State        | Actions                              |
|-------------------|-----------------------|-------------------|--------------------------------------|
| GAME_START        | Random = Right        | BALL_MOVING_RIGHT | Reset ball position, set direction   |
| GAME_START        | Random = Left         | BALL_MOVING_LEFT  | Reset ball position, set direction   |
| BALL_MOVING_RIGHT | Right button at LED 8 | BALL_MOVING_LEFT  | Reverse direction, increase speed    |
| BALL_MOVING_RIGHT | Ball position > 8     | POINT_SCORED      | Left player scores, flash LEDs       |
| BALL_MOVING_LEFT  | Left button at LED 1  | BALL_MOVING_RIGHT | Reverse direction, increase speed    |
| BALL_MOVING_LEFT  | Ball position < 1     | POINT_SCORED      | Right player scores, flash LEDs      |
| POINT_SCORED      | Score < 5             | GAME_START        | Continue game, new round             |
| POINT_SCORED      | Score >= 5            | GAME_OVER         | Show winner animation                |
| GAME_OVER         | After animations      | GAME_START        | Reset scores, restart game           |

---

## Code Walkthrough

### Program Flow

```
1. System Initialization
   ├── HAL_Init()
   ├── SystemClock_Config() → 80 MHz
   ├── MX_GPIO_Init() → Configure all pins
   └── MX_USART2_UART_Init() → Setup UART

2. Game Module Initialization
   ├── leds_init() → Clear all LEDs
   └── button_init() → Reset button state

3. Game Start Animation
   └── Flash all LEDs 3 times (game ready)

4. Main Game Loop (ping_pong_game)
   └── while(1) {
       switch(state) {
         case GAME_START: ...
         case BALL_MOVING_RIGHT: ...
         case BALL_MOVING_LEFT: ...
         case POINT_SCORED: ...
         case GAME_OVER: ...
       }
     }
```

### Critical Code Sections

#### Ball Movement Logic (BALL_MOVING_RIGHT)

```c
// Display ball at current position
leds_index(ball_position);

// Start non-blocking timer
timer_init(ball_speed);

// Wait for timer while checking buttons
while (!timer_now()) {
    button_pressed = button_read();

    // Check for hit at the right time
    if (button_pressed == RIGHT_BUTTON && ball_position == 8) {
        ball_direction = -1;  // Reverse
        state = BALL_MOVING_LEFT;

        // Increase speed (up to MIN_SPEED)
        if (ball_speed > MIN_SPEED) {
            ball_speed -= SPEED_DECREASE;
        }
        break;
    }
}

// Timer expired - move ball
if (state == BALL_MOVING_RIGHT) {
    ball_position++;

    // Check for miss
    if (ball_position > 8) {
        left_score++;
        state = POINT_SCORED;
        // Flash LEDs to indicate miss
    }
}
```

**Key Points**:
- Non-blocking timer allows button polling during wait
- Precise timing required: button must be pressed when ball is at end position
- Speed increases with each successful hit (game gets harder)
- Visual feedback on miss (3 rapid flashes)

#### Button Debouncing Algorithm

```c
int button_read(void) {
    uint32_t current_time = HAL_GetTick();

    // Debouncing: ignore presses within 20ms
    if ((current_time - last_press_time) < DEBOUNCE_DELAY_MS) {
        return 0;
    }

    // Read current button states (active LOW)
    uint8_t left_current = HAL_GPIO_ReadPin(LEFT_BUTTON_PORT, LEFT_BUTTON_PIN);
    uint8_t right_current = HAL_GPIO_ReadPin(RIGHT_BUTTON_PORT, RIGHT_BUTTON_PIN);

    // Edge detection: trigger only on falling edge (1 → 0)
    if (right_button_prev_state == 1 && right_current == 0) {
        result = RIGHT_BUTTON;
        last_press_time = current_time;
    }
    else if (left_button_prev_state == 1 && left_current == 0) {
        result = LEFT_BUTTON;
        last_press_time = current_time;
    }

    // Update state for next call
    left_button_prev_state = left_current;
    right_button_prev_state = right_current;

    return result;
}
```

**Key Points**:
- 20ms debounce window filters out mechanical bounce
- Edge detection ensures one press = one action (not continuous)
- State tracking with previous values
- Right button has priority if both pressed simultaneously

#### LED Position Mapping

```c
// LED pin lookup table
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

void leds_index(int i) {
    if (i < 1 || i > 8) return;  // Bounds check

    // Turn off all LEDs
    for (int j = 0; j < 8; j++) {
        HAL_GPIO_WritePin(led_pins[j].port, led_pins[j].pin, GPIO_PIN_RESET);
    }

    // Turn on specified LED (convert 1-8 to 0-7 index)
    HAL_GPIO_WritePin(led_pins[i-1].port, led_pins[i-1].pin, GPIO_PIN_SET);
}
```

**Key Points**:
- Abstraction layer hides GPIO complexity
- Lookup table for O(1) access
- Atomic operation (all off, then one on)
- 1-indexed for intuitive usage (LED 1-8, not 0-7)

#### Score Display Logic

```c
void show_score(uint8_t right_score, uint8_t left_score, uint32_t duration_ms) {
    leds_clear();
    HAL_Delay(100);  // Visual separation

    // Left player score: LEDs 1-4 from left
    for (int i = 1; i <= left_score && i <= 4; i++) {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1 << (i-1), GPIO_PIN_SET);
    }

    // Right player score: LEDs 5-8 from right
    if (right_score >= 1) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);   // LED 8
    if (right_score >= 2) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);   // LED 7
    if (right_score >= 3) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);  // LED 6
    if (right_score >= 4) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);  // LED 5

    HAL_Delay(duration_ms);
    leds_clear();
}
```

**Visual Example** (Left: 2, Right: 3):
```
[ON][ON][OFF][OFF]  [OFF][ON][ON][ON]
 L1  L2   L3   L4    L5  L6  L7  L8
 <-- Left: 2 -->       <-- Right: 3 -->
```

#### Non-Blocking Timer Implementation

```c
static uint32_t timer_start_time = 0;
static uint32_t timer_duration = 0;

void timer_init(uint32_t ms) {
    timer_start_time = HAL_GetTick();  // Current time in ms
    timer_duration = ms;
}

int timer_now(void) {
    uint32_t current_time = HAL_GetTick();
    uint32_t elapsed_time = current_time - timer_start_time;

    return (elapsed_time >= timer_duration) ? 1 : 0;
}
```

**Key Points**:
- Uses HAL_GetTick() (1ms system tick)
- Non-blocking: returns immediately with status
- Handles 32-bit rollover automatically (after ~49.7 days)
- Enables responsive gameplay (can check buttons during wait)

---

## Game Mechanics

### Game Constants

```c
#define WINNING_SCORE      5     // Points needed to win
#define INITIAL_SPEED      200   // Starting speed (ms per LED)
#define MIN_SPEED          100   // Maximum speed (fastest)
#define SPEED_DECREASE     20    // Speed increase per hit (ms)
#define SCORE_DISPLAY_TIME 2000  // Score display duration (ms)
```

### Ball Physics

1. **Starting Position**: Center (LED 4)
2. **Starting Direction**: Random (based on system tick parity)
3. **Starting Speed**: 200ms per LED position
4. **Speed Progression**:
   - Initial: 200ms
   - After 1st hit: 180ms
   - After 2nd hit: 160ms
   - After 3rd hit: 140ms
   - After 4th hit: 120ms
   - After 5th hit: 100ms (maximum speed)
   - Resets to 200ms after a miss

### Timing Requirements

Players must press their button **exactly when the ball is at their end position**:
- Left player: Press when ball is at LED 1
- Right player: Press when ball is at LED 8

Early or late presses are ignored (though penalty could be added).

### Visual Feedback

| Event          | Visual Effect                                  |
|----------------|------------------------------------------------|
| Game Start     | All LEDs flash 3 times (200ms on/off)         |
| Ball Movement  | Single LED lit at ball position                |
| Successful Hit | Immediate direction change (no flash)          |
| Miss           | All LEDs flash 3 times rapidly (100ms on/off)  |
| Point Scored   | Score display (LEDs lit from each side)        |
| Winner         | Winning side blinks 5 times + all LEDs finale  |
| Game Restart   | All LEDs flash 2 times (300ms on/off)          |

### Scoring System

- **Point Award**: Opponent scores when a player misses
- **Win Condition**: First to 5 points
- **Score Display**: Number of LEDs lit = score (max 4 per side)
- **Post-Game**: Scores reset to 0, new game begins

---

## Building and Running

### Prerequisites

1. **Hardware**: STM32L476RG Nucleo board
2. **Software**:
   - STM32CubeIDE (Eclipse-based)
   - ARM GCC toolchain
   - ST-Link drivers
3. **Custom Hardware**: 8 LEDs + 2 buttons connected per pin mapping

### Build Instructions

#### Using STM32CubeIDE (Recommended)

1. Open STM32CubeIDE
2. File → Import → Existing Projects into Workspace
3. Select the `pingpong` directory
4. Project → Build All (Ctrl+B)
5. Binary will be in `Debug/` directory

#### Using Command Line (Make)

```bash
cd /Users/jakob/dev/kth/IS1300/pingpong/Debug
make clean
make all
```

Output: `pingpong.elf`, `pingpong.bin`, `pingpong.hex`

### Flash to Board

#### Using STM32CubeIDE

1. Connect STM32 Nucleo board via USB
2. Run → Debug (F11) or Run → Run (Ctrl+F11)
3. IDE will automatically flash and start program

#### Using Command Line (st-flash)

```bash
st-flash write pingpong.bin 0x8000000
```

### Hardware Connections

Connect your custom hardware according to the pin mapping:

**LEDs** (with current-limiting resistors):
```
LED1 → PB1    LED5 → PA11
LED2 → PB2    LED6 → PA12
LED3 → PB11   LED7 → PC5
LED4 → PB12   LED8 → PC6
```

**Buttons** (active LOW, pull-up enabled):
```
Left Button  → PB15
Right Button → PC8
```

### Testing

#### LED Test Mode

To verify all LEDs are working correctly, enable the test mode in main.c:100-101:

```c
// Uncomment the line below to run the LED test instead of the game
test_leds();
```

Test sequence:
1. Slow cycle (500ms per LED)
2. Fast forward cycle (100ms per LED)
3. Fast backward cycle (100ms per LED)
4. All LEDs flash 3 times
5. Repeat

#### Button Test

Buttons can be tested by observing game behavior:
- Press left button at LED 1: Ball should reverse
- Press right button at LED 8: Ball should reverse
- Press buttons at wrong time: Should be ignored

### Debugging

#### Serial Monitor (Optional)

USART2 is configured but not used in current implementation. To add debug output:

```c
#include <stdio.h>

// Add in main.c after UART init
printf("Game starting...\n");
```

Connect serial terminal:
- Baud: 115200
- Data: 8 bits
- Parity: None
- Stop: 1 bit

#### LED Debugging

If LEDs aren't working:
1. Run `test_leds()` to verify all LEDs
2. Check GPIO pin connections with multimeter
3. Verify pull-up/down resistor values
4. Check LED polarity (anode/cathode)

#### Button Debugging

If buttons aren't responding:
1. Verify pull-up resistors are enabled (internal or external)
2. Check active LOW logic (button press = 0V)
3. Increase `DEBOUNCE_DELAY_MS` if false triggers occur
4. Use oscilloscope to check for bounce

---

## Advanced Topics

### Potential Enhancements

1. **Penalty for Early Press**:
   ```c
   // In BALL_MOVING_RIGHT state
   else if (button_pressed == RIGHT_BUTTON && ball_position != 8) {
       left_score++;  // Opponent scores
       state = POINT_SCORED;
   }
   ```

2. **Difficulty Levels**:
   ```c
   typedef enum { EASY, MEDIUM, HARD } Difficulty;

   // Easy:   INITIAL_SPEED = 300, MIN_SPEED = 150
   // Medium: INITIAL_SPEED = 200, MIN_SPEED = 100
   // Hard:   INITIAL_SPEED = 150, MIN_SPEED = 50
   ```

3. **Sound Effects**: Add buzzer for hits/misses

4. **Multiplayer Modes**:
   - 2v2 (4 players)
   - Time trial
   - Survival mode

5. **Power-ups**: Special LED patterns for temporary effects

### Performance Considerations

- **CPU Usage**: State machine + polling = minimal CPU load
- **Timing Accuracy**: ±1ms (limited by HAL_GetTick resolution)
- **Responsiveness**: Button latency < 20ms (debounce window)
- **Memory**: ~2KB RAM, ~15KB Flash (excluding HAL)

### Code Quality

- **Modularity**: Separated concerns (LEDs, buttons, timer, score)
- **Documentation**: Extensive inline comments and headers
- **Portability**: Can be adapted to other STM32 chips
- **Maintainability**: Clear state machine, magic numbers avoided

---

## Conclusion

This ping-pong game demonstrates fundamental embedded systems concepts:
- **GPIO Control**: Digital output (LEDs) and input (buttons)
- **State Machines**: Organized game flow
- **Non-blocking I/O**: Responsive gameplay without delays
- **Debouncing**: Reliable button input
- **Timing**: Precise game mechanics

The modular code structure makes it easy to extend with new features or adapt to different hardware platforms.

---

## References

- **STM32L476RG Reference Manual**: RM0351
- **STM32L4 HAL Documentation**: [ST.com](https://www.st.com)
- **Course**: KTH IS1300 - Datorteknik och komponenter
- **Author**: Jakob (KTH Student)
- **Date**: January 2025

---

**End of Documentation**
# STM32 Ping-Pong Game

A classic two-player ping-pong game implemented on STM32L4 microcontroller with LED display and button controls.

## 📋 Table of Contents
- [Overview](#overview)
- [Hardware Requirements](#hardware-requirements)
- [Pin Configuration](#pin-configuration)
- [Game Rules](#game-rules)
- [Software Architecture](#software-architecture)
- [Building and Running](#building-and-running)
- [Code Structure](#code-structure)
- [How to Play](#how-to-play)
- [Customization](#customization)

## 🎮 Overview

This project implements a two-player ping-pong game on an STM32 microcontroller. A "ball" (represented by a lit LED) travels back and forth across 8 LEDs. Players must press their button at the right time to bounce the ball back to their opponent. Miss the ball, and your opponent scores a point!

**Features:**
- Real-time LED ball animation
- Button-based player controls with debouncing
- Progressive difficulty (ball speeds up with each hit)
- Score display using LEDs
- Winner celebration animation
- Non-blocking timer implementation
- Clean, modular code architecture

## 🔧 Hardware Requirements

- **STM32 Nucleo-L476RG** (or compatible STM32L4 board)
- **8 LEDs** for the playing field
- **2 Push buttons** for player controls
- **Jumper wires** for connections
- **USB cable** for programming and power

## 📌 Pin Configuration

### LEDs (Playing Field)
| LED Position | Pin  | Port | Description |
|--------------|------|------|-------------|
| LED 1 (Left) | PB1  | GPIOB | Leftmost LED |
| LED 2        | PB2  | GPIOB | |
| LED 3        | PB11 | GPIOB | |
| LED 4        | PB12 | GPIOB | Center-left |
| LED 5        | PA11 | GPIOA | Center-right |
| LED 6        | PA12 | GPIOA | |
| LED 7        | PC5  | GPIOC | |
| LED 8 (Right)| PC6  | GPIOC | Rightmost LED |

### Buttons (Player Controls)
| Button | Pin  | Port  | Active State | Description |
|--------|------|-------|--------------|-------------|
| Left   | PB15 | GPIOB | Active LOW   | Left player button |
| Right  | PC8  | GPIOC | Active LOW   | Right player button |

### Physical Layout
```
[LED1][LED2][LED3][LED4]    [LED5][LED6][LED7][LED8]
   <-- Left Side -->            <-- Right Side -->

      [Left Button]              [Right Button]
```

## 🎯 Game Rules

1. **Objective**: Be the first player to score 5 points

2. **Gameplay**:
   - The ball (lit LED) travels from one side to the other
   - Left player controls the left side (LED 1)
   - Right player controls the right side (LED 8)
   - Press your button when the ball reaches your end position
   - Successfully hitting the ball bounces it back to your opponent
   - Missing the ball gives your opponent a point

3. **Ball Physics**:
   - Ball starts at center, moves in random direction
   - Speed: Starts at 200ms per LED
   - Speeds up by 20ms with each successful hit
   - Maximum speed: 100ms per LED
   - Speed resets after a miss

4. **Scoring**:
   - Miss timing = Opponent scores 1 point
   - First to 5 points wins
   - Score is displayed on LEDs after each point
   - Winner celebration with flashing LEDs

5. **Visual Feedback**:
   - Successful hit: Brief flash of all LEDs
   - Miss: Rapid flashing of all LEDs (3 times)
   - Score display: LEDs light up from each player's side
   - Winner: Flashing animation on winner's side

## 🏗️ Software Architecture

The project is organized into modular components for maintainability and clarity:

### Module Overview

```
pingpong/
├── Core/
│   ├── Inc/              # Header files
│   │   ├── leds.h        # LED control interface
│   │   ├── button.h      # Button handling interface
│   │   ├── timer.h       # Non-blocking timer interface
│   │   ├── score.h       # Score display interface
│   │   └── main.h        # Main system header
│   │
│   └── Src/              # Implementation files
│       ├── leds.c        # LED control implementation
│       ├── button.c      # Button handling with debouncing
│       ├── timer.c       # Non-blocking timer implementation
│       ├── score.c       # Score display implementation
│       └── main.c        # Game logic and state machine
│
└── README.md             # This file
```

### Module Descriptions

#### 1. LED Module (`leds.h/c`)
- **Purpose**: Controls 8 LEDs for ball animation and display
- **Key Functions**:
  - `leds_init()` - Initialize LED system
  - `leds_index(i)` - Light single LED at position i
  - `leds_all()` - Turn on all LEDs
  - `leds_clear()` - Turn off all LEDs

#### 2. Button Module (`button.h/c`)
- **Purpose**: Handles button input with debouncing
- **Key Functions**:
  - `button_init()` - Initialize button system
  - `button_read()` - Read button state (returns LEFT_BUTTON, RIGHT_BUTTON, or 0)
- **Features**:
  - Software debouncing (20ms)
  - Edge detection (press, not hold)
  - Priority handling for simultaneous presses

#### 3. Timer Module (`timer.h/c`)
- **Purpose**: Non-blocking timing for responsive gameplay
- **Key Functions**:
  - `timer_init(ms)` - Start timer with duration
  - `timer_now()` - Check if timer expired
- **Advantage**: Allows button checking during delays

#### 4. Score Module (`score.h/c`)
- **Purpose**: Visual score feedback using LEDs
- **Key Functions**:
  - `show_score(right, left, duration)` - Display current score
  - `show_winner(winner, duration)` - Celebration animation

#### 5. Main Game Logic (`main.c`)
- **Purpose**: Implements game state machine and logic
- **States**:
  - `GAME_START` - Initialize new round
  - `BALL_MOVING_RIGHT` - Ball traveling toward right player
  - `BALL_MOVING_LEFT` - Ball traveling toward left player
  - `POINT_SCORED` - Handle scoring
  - `GAME_OVER` - End of game

## 🚀 Building and Running

### Prerequisites
1. Install **STM32CubeIDE** (available from STMicroelectronics)
2. Install **ST-LINK drivers** for your platform

### Steps

1. **Open the project**:
   ```
   - Launch STM32CubeIDE
   - File → Open Projects from File System
   - Select the 'pingpong' directory
   ```

2. **Connect hardware**:
   - Wire LEDs and buttons according to pin configuration
   - Connect STM32 board to computer via USB

3. **Build the project**:
   ```
   - Right-click on project → Build Project
   - Or press Ctrl+B
   - Wait for compilation to complete
   ```

4. **Flash and run**:
   ```
   - Click Debug button (bug icon)
   - Or press F11
   - Click Resume (F8) to start game
   ```

### Testing LEDs (Optional)

If you want to test the LED hardware before playing:

1. Open `Core/Src/main.c`
2. Comment out `ping_pong_game();`
3. Uncomment `test_leds();`
4. Rebuild and flash

The test program will cycle through all LEDs to verify connections.

## 📂 Code Structure

### State Machine Flow
```
┌──────────────┐
│  GAME_START  │ ──> Initialize ball position and direction
└──────┬───────┘
       │
       ├──> BALL_MOVING_RIGHT ──> Check right button
       │         │
       │         ├──> Hit? ──> Increase speed, flash LEDs
       │         │             Change to BALL_MOVING_LEFT
       │         │
       │         └──> Miss? ──> Left scores
       │                        Go to POINT_SCORED
       │
       └──> BALL_MOVING_LEFT  ──> Check left button
                 │
                 ├──> Hit? ──> Increase speed, flash LEDs
                 │             Change to BALL_MOVING_RIGHT
                 │
                 └──> Miss? ──> Right scores
                                Go to POINT_SCORED

┌───────────────┐
│ POINT_SCORED  │ ──> Display score
└───────┬───────┘
        │
        ├──> Winner? ──> GAME_OVER ──> Show winner
        │                               Reset scores
        │                               Back to GAME_START
        │
        └──> Continue? ──> Back to GAME_START
```

### Key Design Patterns

1. **Modular Architecture**: Each module has clear responsibility
2. **Non-blocking Design**: Game remains responsive during timing delays
3. **State Machine**: Clean separation of game states
4. **Hardware Abstraction**: LED/button details hidden in modules

## 🎮 How to Play

1. **Starting the Game**:
   - Flash board with program
   - All LEDs will flash 3 times to indicate game start
   - Ball appears at center and begins moving

2. **Gameplay**:
   - **Left Player**: Press left button when ball reaches LED 1
   - **Right Player**: Press right button when ball reaches LED 8
   - Time your button press correctly or opponent scores!

3. **Timing**:
   - Press button when ball is at your end position
   - Too early or too late = miss!
   - Ball speeds up with each successful hit

4. **Scoring**:
   - After each miss, current score is displayed for 2 seconds
   - LEDs light up from each player's side (more LEDs = higher score)

5. **Winning**:
   - First to 5 points wins
   - Winner's LEDs flash in celebration
   - Game automatically restarts after displaying final score

## ⚙️ Customization

You can easily customize the game by modifying constants in `main.c`:

```c
/* Game configuration constants */
#define WINNING_SCORE      5    // Points needed to win (default: 5)
#define INITIAL_SPEED    200    // Starting ball speed in ms (default: 200)
#define MIN_SPEED        100    // Fastest ball speed in ms (default: 100)
#define SPEED_DECREASE    20    // Speed increase per hit (default: 20)
#define SCORE_DISPLAY_TIME 2000 // Score display duration (default: 2000ms)
```

### Example Customizations

**Easy Mode** (slower, longer game):
```c
#define WINNING_SCORE      7
#define INITIAL_SPEED    300
#define MIN_SPEED        150
#define SPEED_DECREASE    15
```

**Hard Mode** (faster, shorter game):
```c
#define WINNING_SCORE      3
#define INITIAL_SPEED    150
#define MIN_SPEED         50
#define SPEED_DECREASE    30
```

**Marathon Mode** (long game):
```c
#define WINNING_SCORE     10
```

## 📝 Notes

- **Button Debouncing**: 20ms debounce delay prevents false triggers
- **Timer Rollover**: Timer handles HAL_GetTick() rollover (49.7 days)
- **GPIO Configuration**: Pins configured in CubeMX (MX_GPIO_Init)
- **Power**: Board can be powered via USB or external power
- **Compatibility**: Designed for STM32L4, portable to other STM32 families

## 🎓 Learning Outcomes

This project demonstrates:
- ✅ GPIO control (LEDs and buttons)
- ✅ Software debouncing techniques
- ✅ Non-blocking timing methods
- ✅ State machine design
- ✅ Modular software architecture
- ✅ Real-time embedded systems
- ✅ Hardware abstraction layers

## 🐛 Troubleshooting

**LEDs not lighting up?**
- Check all wiring connections
- Verify GPIO pins are configured as outputs
- Use `test_leds()` to verify hardware

**Buttons not responding?**
- Check button connections (active LOW with pull-up)
- Verify buttons are connected to correct pins (PB15, PC8)
- Check for loose wires

**Game too fast/slow?**
- Adjust INITIAL_SPEED and MIN_SPEED constants
- Rebuild and reflash

**Build errors?**
- Ensure all .c and .h files are included in project
- Check that HAL library is properly linked
- Verify STM32CubeIDE version compatibility

## 👨‍💻 Author

**Jakob** - KTH IS1300 Embedded Systems Project

## 📄 License

This project is created for educational purposes as part of the IS1300 course at KTH Royal Institute of Technology.

---

**Enjoy the game! May the best player win! 🏓**

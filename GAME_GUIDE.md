# Ping-Pong Game Guide

A two-player LED-based ping-pong game for STM32 microcontroller.

## How to Play

### Game Rules

1. The "ball" (lit LED) moves from one side of the board to the other
2. Left player uses LEFT button (PB15) when ball reaches their end (LED 1)
3. Right player uses RIGHT button (PC8) when ball reaches their end (LED 8)
4. If button is pressed at the correct time, ball bounces back
5. If player misses, opponent scores a point
6. First player to reach winning score (5 points) wins the game
7. Game displays score after each point and winner at the end

### Ball Physics

- Ball starts at center and moves in random direction
- Ball speed increases slightly after each successful hit (up to max speed)
- Ball speed resets after a miss
- Hit timing window: Must press button when ball is at end position

### Visual Feedback

- **Moving ball:** Single LED travels across the board
- **Successful hit:** Brief flash of all LEDs
- **Miss:** Flash all LEDs 3 times rapidly
- **Score display:** LEDs light up from each player's side
- **Winner:** Flashing LEDs on winner's side

## Hardware Setup

### LED Layout
```
[1] [2] [3] [4]     [5] [6] [7] [8]
 ^                               ^
Left                          Right
Player                        Player
```

### Controls
- **Left Player:** Button on PB15 (active LOW)
- **Right Player:** Button on PC8 (active LOW)

## Game Configuration

Default settings (can be modified in main.c):
- Winning score: 5 points
- Initial speed: 200ms per LED
- Minimum speed: 100ms per LED
- Speed increase per hit: 20ms faster

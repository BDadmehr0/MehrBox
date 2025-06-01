# MehrBox Game

A simple terminal-based world simulation game built with ncurses.

## Game Description

Manage your farm by planting crops, collecting food, and maintaining your health. The game features:
- Day/night cycle (every 30 seconds)
- Hunger system (health decreases if you don't eat)
- Plant growth mechanics (plants need water nearby)
- Inventory system for collected food

## Controls

- **Arrow keys**: Move cursor
- **W**: Select Water tile
- **S**: Select Soil tile
- **P**: Select Plant tile
- **Space**: Place selected tile
- **E**: Pick up food
- **F**: Eat food (restores health)
- **Q**: Quit game

## Tile Types

1. **Water (~)**: Blue colored, needed for plant growth
2. **Soil (.)**: Yellow colored, base for planting
3. **Plant**: Green colored, grows through stages (' → * → ^ → *)
4. **Food (F)**: Red colored, can be collected and eaten

## Game Mechanics

- Plants grow when placed next to water
- Each plant goes through 3 growth stages before becoming food
- Every 30 seconds (1 game day):
  - Your hunger increases
  - If hunger exceeds 3, you lose health
  - Plants grow or wither depending on water availability
- Eating food resets hunger and restores health

## Requirements

- Linux/Unix system
- ncurses library ([How to install?](https://github.com/mirror/ncurses/blob/master/INSTALL)
- C compiler

## Compilation & Run

```bash
gcc game.c -o game -lncurses
./game
```

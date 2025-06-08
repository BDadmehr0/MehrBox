## Core Mechanics

### Time System
- Each in-game day lasts **30 real-world seconds**
- At each new day:
  - All plants grow or wither based on conditions
  - Player hunger increases by 1
  - Health decreases if hunger exceeds limit

### Plant Growth System
1. **Growth Requirements**:
   - Must be planted on Soil (.)
   - Requires at least one adjacent Water tile (~) in any direction
   - Grows through 4 stages: `'` → `*` → `^` → `*`

2. **Growth Process**:
   - With water: Growth stage increases by +1 each day
   - Without water: Growth stage decreases by -1 each day
   - At maximum growth stage (3), plant transforms into Food (F)

3. **Visual Indicators**:
   - Stage 0: `'` (newly planted)
   - Stage 1: `*` (growing)
   - Stage 2: `^` (maturing)
   - Stage 3: `*` (ready for harvest)

### Health & Hunger System
- **Maximum Health**: 5 hearts
- **Hunger Limit**: 3
  - Hunger increases by 1 each day
  - If hunger > 3: lose 1 health per day
- **Food Consumption**:
  - Eating 1 food (F key) resets hunger to 0
  - Restores 1 health (if not at maximum)

## Tile Types

| Tile | Character | Color  | Description |
|------|-----------|--------|-------------|
| Water | ~ | Blue | Required near plants for growth |
| Soil | . | Yellow | Base terrain for planting |
| Plant | varies | Green | Grows through stages to become food |
| Food | F | Red | Collectible resource for survival |
| Empty | (space) | White | Blank space |

## Complete Controls

| Key | Action |
|-----|--------|
| Arrow Keys | Move cursor |
| W | Select Water tile |
| S | Select Soil tile |
| P | Select Plant tile |
| Space | Place selected tile |
| E | Collect food (when on Food tile) |
| F | Eat food (consumes 1 from inventory) |
| D | Delete tile (Soil, Plant, Water) |
| R | Reset map (Food tiles included) |
| Y | Confirm map Reset |
| Q | Quit game |

## Game Rules

1. **Survival Requirements**:
   - Maintain health above 0
   - Manage hunger by regularly eating food

2. **Farming Rules**:
   - Plants only grow when placed on Soil
   - Water must be adjacent (8-directional) to support growth
   - Plants die if left without water for too long

3. **Inventory**:
   - Maximum food storage: Unlimited
   - Food doesn't spoil over time

## Winning Condition
There is no final win condition - the game continues indefinitely. Your score is measured in days survived.

## Tips for Success
1. Create efficient farm layouts with water sources every 2-3 soil tiles
2. Maintain a buffer of food (3-5 minimum) for emergencies
3. Expand your farm gradually as your food supply stabilizes
4. Monitor your health/hunger status regularly

## Technical Notes
- The game uses ncurses for terminal rendering
- All growth calculations happen at day transitions
- The map size is fixed at 20x40 tiles with a 30-column info panel

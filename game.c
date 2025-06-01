#include <ncurses.h>
#include <unistd.h>
#include <time.h>

#define MAP_HEIGHT 20
#define MAP_WIDTH 40
#define PANEL_WIDTH 30
#define MAX_GROWTH_STAGE 3
#define MAX_HEALTH 5
#define HUNGER_LIMIT 3

typedef enum {
    EMPTY,
    WATER,
    SOIL,
    PLANT,
    FOOD
} TileType;

int growth_stage[MAP_HEIGHT][MAP_WIDTH];
int inventory_food = 0;
int player_health = MAX_HEALTH;
int player_hunger = 0;

char tile_char(TileType t, int growth) {
    switch(t) {
        case WATER: return '~';
        case SOIL: return '.';
        case PLANT:
            switch(growth) {
                case 0: return '\'';
                case 1: return '*';
                case 2: return '^';
                case 3: return '*';
                default: return '*';
            }
        case FOOD: return 'F';
        default: return ' ';
    }
}

int has_water_nearby(TileType map[MAP_HEIGHT][MAP_WIDTH], int y, int x) {
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            int ny = y + dy, nx = x + dx;
            if (ny >= 0 && ny < MAP_HEIGHT && nx >= 0 && nx < MAP_WIDTH) {
                if (map[ny][nx] == WATER)
                    return 1;
            }
        }
    }
    return 0;
}

void draw_panel(int start_x, int start_y, int width, int height, int day, TileType map[MAP_HEIGHT][MAP_WIDTH], int cursor_y, int cursor_x) {
    for (int y = 0; y <= height; y++) {
        for (int x = 0; x <= width; x++) {
            if (y == 0 || y == height)
                mvaddch(start_y + y, start_x + x, '-');
            else if (x == 0 || x == width)
                mvaddch(start_y + y, start_x + x, '|');
        }
    }

    mvprintw(start_y + 1, start_x + 2, "Sandbox Game");
    mvprintw(start_y + 2, start_x + 2, "Day: %d", day);
    mvprintw(start_y + 4, start_x + 2, "Inventory: Food x %d", inventory_food);
    mvprintw(start_y + 6, start_x + 2, "Health: %d", player_health);
    mvprintw(start_y + 7, start_x + 2, "Hunger: %d", player_hunger);

    mvprintw(start_y + 9, start_x + 2, "Tile Info:");
    TileType current_tile = map[cursor_y][cursor_x];
    int g = growth_stage[cursor_y][cursor_x];
    switch(current_tile) {
        case WATER: mvprintw(start_y + 10, start_x + 4, "Water (~)"); break;
        case SOIL: mvprintw(start_y + 10, start_x + 4, "Soil (.)"); break;
        case PLANT: mvprintw(start_y + 10, start_x + 4, "Plant (stage %d)", g); break;
        case FOOD: mvprintw(start_y + 10, start_x + 4, "Food (F) - Press E"); break;
        default: mvprintw(start_y + 10, start_x + 4, "Empty"); break;
    }

    mvprintw(start_y + 12, start_x + 2, "Controls:");
    mvprintw(start_y + 13, start_x + 4, "Arrow keys - Move");
    mvprintw(start_y + 14, start_x + 4, "W - Water, S - Soil, P - Plant");
    mvprintw(start_y + 15, start_x + 4, "Space - Place");
    mvprintw(start_y + 16, start_x + 4, "E - Pick up, F - Eat food");
    mvprintw(start_y + 17, start_x + 4, "Q - Quit");
}

int main() {
    initscr(); noecho(); curs_set(FALSE); keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_RED, COLOR_BLACK);

    TileType map[MAP_HEIGHT][MAP_WIDTH];
    for (int y = 0; y < MAP_HEIGHT; y++)
        for (int x = 0; x < MAP_WIDTH; x++) {
            map[y][x] = EMPTY;
            growth_stage[y][x] = 0;
        }

    int day = 1;
    time_t last_time = time(NULL);
    int cursor_x = 0, cursor_y = 0;
    TileType selected_tile = SOIL;

    while(1) {
        clear();

        for (int y = 0; y <= MAP_HEIGHT+1; y++)
            for (int x = 0; x <= MAP_WIDTH+1; x++)
                mvaddch(y, x, (y==0||y==MAP_HEIGHT+1)?'-':(x==0||x==MAP_WIDTH+1)?'|':' ');

        for (int y = 0; y < MAP_HEIGHT; y++) {
            for (int x = 0; x < MAP_WIDTH; x++) {
                TileType t = map[y][x];
                int growth = growth_stage[y][x];
                if (t == WATER) attron(COLOR_PAIR(1));
                else if (t == SOIL) attron(COLOR_PAIR(2));
                else if (t == PLANT) attron(COLOR_PAIR(3));
                else if (t == FOOD) attron(COLOR_PAIR(4));

                mvaddch(y+1, x+1, tile_char(t, growth));

                if (t == WATER) attroff(COLOR_PAIR(1));
                else if (t == SOIL) attroff(COLOR_PAIR(2));
                else if (t == PLANT) attroff(COLOR_PAIR(3));
                else if (t == FOOD) attroff(COLOR_PAIR(4));
            }
        }

        mvaddch(cursor_y+1, cursor_x+1, 'X' | A_REVERSE);
        draw_panel(MAP_WIDTH + 3, 0, PANEL_WIDTH, MAP_HEIGHT + 4, day, map, cursor_y, cursor_x);
        refresh();

        time_t current_time = time(NULL);
        if (current_time - last_time >= 30) {
            day++; last_time = current_time;
            player_hunger++;
            if (player_hunger > HUNGER_LIMIT) {
                player_health--;
                if (player_health < 0) player_health = 0;
            }
            for (int y = 0; y < MAP_HEIGHT; y++) {
                for (int x = 0; x < MAP_WIDTH; x++) {
                    if (map[y][x] == PLANT) {
                        if (has_water_nearby(map, y, x)) {
                            if (growth_stage[y][x] < MAX_GROWTH_STAGE)
                                growth_stage[y][x]++;
                            else {
                                map[y][x] = FOOD;
                                growth_stage[y][x] = 0;
                            }
                        } else {
                            if (growth_stage[y][x] > 0)
                                growth_stage[y][x]--;
                        }
                    }
                }
            }
        }

        nodelay(stdscr, TRUE);
        int ch = getch();
        switch(ch) {
            case 'q': case 'Q': endwin(); return 0;
            case KEY_UP: if (cursor_y > 0) cursor_y--; break;
            case KEY_DOWN: if (cursor_y < MAP_HEIGHT-1) cursor_y++; break;
            case KEY_LEFT: if (cursor_x > 0) cursor_x--; break;
            case KEY_RIGHT: if (cursor_x < MAP_WIDTH-1) cursor_x++; break;
            case 'w': case 'W': selected_tile = WATER; break;
            case 's': case 'S': selected_tile = SOIL; break;
            case 'p': case 'P': selected_tile = PLANT; break;
            case ' ': map[cursor_y][cursor_x] = selected_tile;
                      if (selected_tile == PLANT) growth_stage[cursor_y][cursor_x] = 0; break;
            case 'e': case 'E':
                if (map[cursor_y][cursor_x] == FOOD) {
                    inventory_food++;
                    map[cursor_y][cursor_x] = EMPTY;
                }
                break;
            case 'f': case 'F':
                if (inventory_food > 0 && player_health < MAX_HEALTH) {
                    inventory_food--;
                    player_hunger = 0;
                    player_health++;
                }
                break;
        }

        usleep(50000);
    }

    endwin();
    return 0;
}


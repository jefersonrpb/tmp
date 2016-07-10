#include "game.h"

// 2d array
int **map;

// key code
int keypress;

// game state
int current_state;

// 40x40 + 2 cells for bounds
int map_width = 42;
int map_height = 22; 

int max_players = 4;
int remaining_players = 0;

// pointers
LightCycle **ptr_players;
WINDOW *ptr_window;

int main(int argc, char **argv)
{
    create_window();
    create_menu(); 
    srand(time(NULL));
    tick();
    return 0;
}

void start()
{
    create_map();
    create_players();
    current_state = PLAY;
}

void tick()
{
    do {
        update();
        draw();
        usleep(FPS);
    } while (true);
} 

void create_map()
{
    map = malloc(map_height * map_width * sizeof(int));
    for (int x = 0; x < map_width; x++) {
        map[x] = malloc(sizeof(int) * map_height);
        for (int y = 0; y < map_height; y++) {
            map[x][y] = EMPTY;
        }
    }
    create_bound(0, 0, map_width - 1, map_height - 1);
} 

LightCycle *new_player(int x, int y, enum directions direction, int color)
{
    LightCycle *player = malloc(sizeof(LightCycle));
    player->position = new_point(x, y);
    player->direction = direction;
    player->color = color;
    player->dead = false;
    return player;
}

Point *new_point(int x, int y)
{
    Point *position = malloc(sizeof(Point));
    position->x = x;
    position->y = y;
    return position;
}

void update()
{
    keypress = getch();
    if (keypress == 'q' || keypress == 'Q') {
        quit();
    }

    if (current_state != PLAY) {
        if (keypress == KEY_ENTER || keypress == 10) {
            start();
        }
        return;
    }

    input_player_direction(ptr_players[0]);
    for (int i = 0; i < max_players; i++) {
        if (ptr_players[i]->dead) continue;
        if (i > 0) update_dumb_ai(ptr_players[i]);
        update_player_position(ptr_players[i]);
        check_collisions(ptr_players[i], i > 0);
        fulfill(ptr_players[i]);
    }
} 

void draw()
{
    if (current_state == GAME_OVER) return;
    if (current_state == PLAY) erase();
    for (int x = 0; x < map_width; x++) {
        for (int y = 0; y < map_height; y++) {
            draw_char(x, y, map[x][y]);
        }
    } 
    refresh();
}

void create_players()
{
    ptr_players = malloc(sizeof(LightCycle) * max_players);
    remaining_players = max_players;

    // 2d array with x, y, direction, color
    int positions[4][4] = {
        // top left
        {1, 3, RIGHT, COLOR_BLUE},
        // bottom right
        {map_width - 2, map_height - 4, LEFT, COLOR_RED},
        // top right
        {map_width - 2, 3, LEFT, COLOR_GREEN},
        // bottom left
        {1, map_height - 4, RIGHT, COLOR_YELLOW}
    };

    for (int i = 0; i < max_players; i++) {
        ptr_players[i] = new_player(positions[i][0], positions[i][1], positions[i][2], positions[i][3]);
    }
}

void restore_window()
{
    // curses call to restore the original window and leave
    endwin(); 
}

void quit()
{
    restore_window();
    exit(0);
} 

void draw_char(int x, int y, int value)
{
    if (value != EMPTY) mvaddch(y, x, value); 
}

void input_player_direction(LightCycle *player)
{
    int *directions = get_allowed_directions(player);
    int direction = player->direction;
    if (keypress == KEY_LEFT) direction = LEFT;
    if (keypress == KEY_RIGHT) direction = RIGHT;
    if (keypress == KEY_UP) direction = UP;
    if (keypress == KEY_DOWN) direction = DOWN;
    if (direction == directions[0] || direction == directions[1])  player->direction = direction;
}

void update_player_position(LightCycle *player)
{
    player->position = get_next_position(player->position, player->direction);
}

void fulfill(LightCycle *player)
{
    map[player->position->x][player->position->y] = WALL | COLOR_PAIR(player->color);
}

Point *get_next_position(Point *current_position, int direction)
{
    Point *position = new_point(current_position->x, current_position->y);
    if (direction == RIGHT) position->x += 1;
    if (direction == LEFT) position->x -= 1;
    if (direction == UP) position->y -= 1;
    if (direction == DOWN) position->y += 1;
    return position;
}

bool check_collision(Point *position)
{
    if (map[position->x][position->y] == EMPTY) return false;
    return true;
}

void check_collisions(LightCycle *player, bool is_ai)
{
    if (!check_collision(player->position)) return;
    if (!is_ai) return game_over(true);
    remaining_players -= 1;
    if (remaining_players == 1) return game_over(false);
    player->dead = true;
}

void update_dumb_ai(LightCycle *ai)
{
    if (rand() % 10 == 9) ai->direction = try_new_direction(ai);

    // if will collide force new direction
    if (check_collision(get_next_position(ai->position, ai->direction))) {
        ai->direction = try_new_direction(ai);
    }
}

enum directions try_new_direction(LightCycle *player)
{
    int *directions = get_allowed_directions(player);
    enum directions direction = rand() % 1 == 1 ? directions[0] : directions[1]; 
    // it was at this moment, he knew he fucked up
    if (check_collision(get_next_position(player->position, direction))) {
        direction = direction == directions[0] ? directions[1]: directions[0]; 
    }
    return direction;
}

int *get_allowed_directions(LightCycle *player)
{
    int *directions = malloc(sizeof(int) * 2);
    switch(player->direction) {
        case UP:
        case DOWN:
            directions[0] = RIGHT;
            directions[1] = LEFT;
        break;
        case LEFT:
        case RIGHT:
            directions[0] = DOWN;
            directions[1] = UP;
        break;
    }
    return directions; 
}

void create_menu()
{
    current_state = MENU;
    create_map();
    draw();
    int center_x = map_width/2;
    mvprintw(1, center_x - 14, " _____   ___    ___    _  _ ");
    mvprintw(2, center_x - 14, "|_   _| | _ \\  / _ \\  | \\| |");
    mvprintw(3, center_x - 14, "  | |   |   / | (_) | | .` |");
    mvprintw(4, center_x - 14, "  |_|   |_|_\\  \\___/  |_|\\_|");
    mvprintw(8, center_x-14, "PRESS 'ENTER' TO START/RESTART"); 
    mvprintw(10, center_x-2, "OR"); 
    mvprintw(12, center_x-7, "'Q' TO QUIT"); 
}

void create_window()
{
    // initialize window
    ptr_window = initscr(); 

    int screen_height, screen_width;
    getmaxyx(ptr_window, screen_height, screen_width); 

    if (screen_height < map_height || screen_width < map_width) {
        restore_window();
        printf(" - you need resize terminal to: cols %d rows %d", map_width, map_height);
        printf("\n - current size: cols %d rows %d\n", screen_width, screen_height);
        exit(1);
    } 

    // set screen size
    wresize(ptr_window, map_height, map_width);

    // set no waiting for Enter key
    cbreak(); 

    // disable Echo
    noecho(); 

    // clear screen, send cursor to position (0,0)
    clear(); 

    // implement all changes since last refresh
    refresh(); 

    // don't display a cursor
    curs_set(false);

    // enable keyboard mapping
    keypad(stdscr, TRUE);

    //Immediately record keystroke & don't interpret control characters
    raw();

    // disable waiting for user input
    nodelay(stdscr, TRUE);

    use_default_colors();
    assume_default_colors(-1,-1);
    start_color();

    init_pair(COLOR_WHITE, COLOR_BLACK, COLOR_WHITE);
    init_pair(COLOR_BLUE, COLOR_BLUE, -1);
    init_pair(COLOR_RED, COLOR_RED, -1);
    init_pair(COLOR_GREEN, COLOR_GREEN, -1);
    init_pair(COLOR_YELLOW, COLOR_YELLOW, -1);
}

void game_over(bool player_loses)
{
    current_state = GAME_OVER;
    char *text = " YOU LOST ";
    if (!player_loses) {
        text = " YOU WON ";
    }
    int x = (map_width / 2) - strlen(text)/2;
    attron(COLOR_PAIR(COLOR_WHITE));
    mvprintw(0, x, "%s", text); 
    attroff(COLOR_PAIR(COLOR_WHITE));
}

void create_bound(int x, int y, int width, int height)
{
    for (int _y = y; _y < height; _y++) {
        map[x][_y] = BOUND_LINE_VERTICAL;
        map[width][_y] = BOUND_LINE_VERTICAL;
    }
    for (int _x = x; _x < width; _x++) {
        map[_x][0] = BOUND_LINE_HORIZONTAL;
        map[_x][height] = BOUND_LINE_HORIZONTAL;
    }
    map[x][y] = BOUND_CORNER;
    map[x][height] = BOUND_CORNER;
    map[width][y] = BOUND_CORNER;
    map[width][height] = BOUND_CORNER;
}

#include "game.h"

// pointers
Moto **ptr_players;
WINDOW *ptr_window;

// 2d array
int **map;

// key code
int keypress;

// game state
int current_state;

// sizes
int screen_height, screen_width;
int map_width, map_height; 
int max_players, remaining_players;

int main(int argc, char **argv)
{
    process_args(argc, argv);
    create_window();
    create_menu(); 
    set_locale(); 
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

void update()
{
    keypress = getch();
    if (keypress == 'q') {
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
        fulfill_wall(ptr_players[i]);
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

void process_args(int argc, char **argv)
{
    map_width = 42, map_height = 22; 
    max_players = 4;
}

void set_locale()
{
    setlocale(LC_ALL, "");
}

void create_window()
{
    // initialize window
    ptr_window = initscr(); 

    getmaxyx(ptr_window, screen_height, screen_width); 

    if (screen_height < map_height || screen_width < map_width) {
        restore_window();
        printf(" - you need resize terminal to: cols %d rows %d", map_width, map_height);
        printf("\n - current size: cols %d rows %d\n", screen_width, screen_height);
        exit(1);
    } 

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
    init_pair(COLOR_WHITE, COLOR_WHITE, -1);
    init_pair(COLOR_BLUE, COLOR_BLUE, -1);
    init_pair(COLOR_RED, COLOR_RED, -1);
    init_pair(COLOR_GREEN, COLOR_GREEN, -1);
    init_pair(COLOR_YELLOW, COLOR_YELLOW, -1);
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

int **create_positions(int length)
{
    int **positions = malloc(sizeof(int) * length);

    // top left
    positions[0] = malloc(sizeof(int) * 4);
    positions[0][0] = 1;
    positions[0][1] = 3;
    positions[0][2] = RIGHT;
    positions[0][3] = COLOR_BLUE;

    // bottom right
    positions[1] = malloc(sizeof(int) * 4);
    positions[1][0] = map_width - 2;
    positions[1][1] = map_height - 4;
    positions[1][2] = LEFT;
    positions[1][3] = COLOR_RED;

    // top right
    positions[2] = malloc(sizeof(int) * 4);
    positions[2][0] = map_width - 2;
    positions[2][1] = 3;
    positions[2][2] = LEFT;
    positions[2][3] = COLOR_GREEN;

    // bottom left
    positions[3] = malloc(sizeof(int) * 4);
    positions[3][0] = 1;
    positions[3][1] = map_height - 4;
    positions[3][2] = RIGHT;
    positions[3][3] = COLOR_YELLOW;

    return positions;
}

void create_players()
{
    ptr_players = malloc(sizeof(Moto) * max_players);
    int **positions = create_positions(max_players);
    remaining_players = max_players;

    for (int i = 0; i < max_players; i++) {
        ptr_players[i] = new_player(positions[i][0], positions[i][1], positions[i][2], positions[i][3]);
    }
}

Moto *new_player(int x, int y, enum directions direction, int color)
{
    Moto *player = malloc(sizeof(Moto));
    player->position = new_point(x, y);
    player->direction = direction;
    player->color = color;
    player->dead = false;
    return player;
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

void input_player_direction(Moto *player)
{
    int *directions = get_allowed_directions(player);
    int direction = player->direction;
    if (keypress == KEY_LEFT) direction = LEFT;
    if (keypress == KEY_RIGHT) direction = RIGHT;
    if (keypress == KEY_UP) direction = UP;
    if (keypress == KEY_DOWN) direction = DOWN;
    if (direction == directions[0] || direction == directions[1])  player->direction = direction;
}

void update_player_position(Moto *player)
{
    player->position = get_next_position(player->position, player->direction);
}

void fulfill_wall(Moto *player)
{
    map[player->position->x][player->position->y] = WALL | COLOR_PAIR(player->color);
}

Point *new_point(int x, int y)
{
    Point *position = malloc(sizeof(Point));
    position->x = x;
    position->y = y;
    return position;
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

void check_collisions(Moto *player, bool is_ai)
{
    if (!check_collision(player->position)) return;
    if (!is_ai) return game_over(true);
    remaining_players -= 1;
    if (remaining_players == 1) return game_over(false);
    player->dead = true;
}

void update_dumb_ai(Moto *ai)
{
    if (rand() % 10 == 9) {
        int *directions = get_allowed_directions(ai);
        ai->direction = rand() % 1 == 1 ? directions[0] : directions[1]; 

        // new direction will collide, get adjacent
        if (check_collision(get_next_position(ai->position, ai->direction))) {
            ai->direction = ai->direction == directions[0] ? directions[1]: directions[0]; 
        }
    }

    // if will collide force new direction
    if (check_collision(get_next_position(ai->position, ai->direction))) {

        int *directions = get_allowed_directions(ai);
        ai->direction = directions[0];
        if (check_collision(get_next_position(ai->position, ai->direction))) {
            ai->direction = directions[1];
        }
    }
}

int *get_allowed_directions(Moto *player)
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
    mvprintw(8, center_x-14, "PRESS ENTER TO START/RESTART"); 
    mvprintw(10, center_x-2, "OR"); 
    mvprintw(12, center_x-7, "'Q' TO QUIT"); 
}

void game_over(bool player_loses)
{
    current_state = GAME_OVER;
    char *text = " YOU LOST ";
    if (!player_loses) {
        text = " YOU WON ";
    }
    int x = (map_width / 2) - 5;
    mvprintw(0, x, "%s", text); 
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

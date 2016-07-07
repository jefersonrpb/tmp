#include "game.h"

// pointers
Moto **ptr_players;
WINDOW *ptr_window;

// 2d array
char **map;

// key code
int keypress;

// game state
int current_state;

// sizes
int screen_height, screen_width;
int map_width, map_height; 
int max_players;

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
    for (size_t i = 0; i < max_players; i++) {
        if (i > 0) update_dumb_ai(ptr_players[i]);
        update_player_position(ptr_players[i]);
        if (check_collision(ptr_players[i]->position)) return game_over(i == 0);
        fulfill_wall(ptr_players[i]);
    }
} 

void draw()
{
    if (current_state == GAME_OVER) {
        return;
    }
    if (current_state == PLAY) {
        erase();
    }

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
    max_players = 2;
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
}

void create_map()
{
    map = malloc(map_height * map_width * sizeof(char *));

    for (int x = 0; x < map_width; x++) {
        map[x] = malloc(map_height * sizeof(char *));
        for (int y = 0; y < map_height; y++) {
            map[x][y] = EMPTY;
        }
    }
    create_bound(0, 0, map_width - 1, map_height - 1);
} 

int **create_positions(int length)
{
    int **positions = malloc(sizeof(int) * length);

    // @todo usar for, criar aleatorimanete posicoes
    /* for (size_t i = 0; i < length; i++) { } */
    positions[0] = malloc(sizeof(int) * 3);
    positions[0][0] = 0;
    positions[0][1] = 3;
    positions[0][2] = RIGHT;

    positions[1] = malloc(sizeof(int) * 3);
    positions[1][0] = map_width - 2;
    positions[1][1] = map_height - 4;
    positions[1][2] = LEFT;

    return positions;
}

void create_players()
{
    ptr_players = malloc(sizeof(Moto) * max_players);
    int **positions = create_positions(max_players);

    for (size_t i = 0; i < max_players; i++) {
        ptr_players[i] = new_player(positions[i][0], positions[i][1], positions[i][2]);
    }
}

Moto *new_player(int x, int y, enum directions direction)
{
    Moto *player = malloc(sizeof(Moto));
    player->position = new_point(x, y);
    player->direction = direction;
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

void draw_char(int x, int y, char value)
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
    map[player->position->x][player->position->y] = WALL;
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

void update_dumb_ai(Moto *ai)
{
    if (rand() % 10 == 8) {
        int *directions = get_allowed_directions(ai);
        ai->direction = rand() % 1 == 1 ? directions[0] : directions[1]; 

        // new direction will collide, get adjacent
        if (check_collision(get_next_position(ai->position, ai->direction))) {
            ai->direction = ai->direction == directions[0] ? directions[1]: directions[0]; 
        }
        return;
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
    for (size_t _y = y; _y < height; _y++) {
        map[x][_y] = BOUND_LINE_VERTICAL;
        map[width][_y] = BOUND_LINE_VERTICAL;
    }
    for (size_t _x = x; _x < width; _x++) {
        map[_x][0] = BOUND_LINE_HORIZONTAL;
        map[_x][height] = BOUND_LINE_HORIZONTAL;
    }
    map[x][y] = BOUND_CORNER;
    map[x][height] = BOUND_CORNER;
    map[width][y] = BOUND_CORNER;
    map[width][height] = BOUND_CORNER;
}

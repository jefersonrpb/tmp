#include "game.h"

// pointers
Moto **ptr_players;
WINDOW *ptr_window;

// 2d array
char **map;

// key code
int keypress;

// sizes
int screen_height, screen_width;
int map_width, map_height; 
int game_width, game_height;
int max_players;

int main(int argc, char **argv)
{
    process_args(argc, argv);
    // initialize random seed
    srand (time(NULL));
    set_locale(); 
    create_window();
    create_map();
    create_players();
    tick(); 
    return 0;
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

    input_player_direction(ptr_players[0]);
    for (size_t i = 0; i < max_players; i++) {
        if (i > 0) update_dumb_ai(ptr_players[i]);
        update_player_velocity(ptr_players[i]);
        update_player_position(ptr_players[i]);
        check_player_collision(ptr_players[i]);
        fulfill_wall(ptr_players[i]);
    }
} 

void draw()
{
    erase();
    draw_rectangle(0, 0, game_width-1, game_height-1);
    mvprintw(0, 0, "%dx%d | %dx%d", ptr_players[0]->position.x, ptr_players[0]->position.y, map_width, map_height); 
    for (int x = 0; x < map_width; x++) {
        for (int y = 0; y < map_height; y++) {
            draw_char(x, y, map[x][y]);
        }
    } 
    refresh();
}

void process_args(int argc, char **argv)
{
    map_width = 40, map_height = 20; 
    game_width = map_width + 2, game_height = map_height + 2;
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

    if (screen_height < game_height || screen_width < game_width) {
        restore_window();
        printf(" - you need resize terminal to: cols %d rows %d", game_width, game_height);
        printf("\n - current size: cols %d rows %d\n", screen_width, screen_height);
        exit(1);
    } 

    wresize(ptr_window, game_height, game_width);

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
} 

int **create_positions(int length)
{
    int **positions = malloc(sizeof(int) * length);

    // @todo usar for, criar aleatorimanete posicoes
    /* for (size_t i = 0; i < length; i++) { } */
    positions[0] = malloc(sizeof(int) * 3);
    positions[0][0] = RIGHT;
    positions[0][1] = 0;
    positions[0][2] = 3;

    positions[1] = malloc(sizeof(int) * 3);
    positions[1][0] = LEFT;
    positions[1][1] = map_width - 1;
    positions[1][2] = map_height - 3;

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

Moto *new_player(int direction, int x, int y)
{
    Moto *player = malloc(sizeof(Moto));

    player->direction = direction;
    player->position.x = x;
    player->position.y = y;
    player->velocity.x = 0;
    player->velocity.y = 0;

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
    if (value == WALL) {
        mvaddch(y+1, x+1, ACS_BLOCK); 
    }
}

void input_player_direction(Moto *player)
{
    if (keypress == KEY_LEFT) player->direction = LEFT;
    if (keypress == KEY_RIGHT) player->direction = RIGHT;
    if (keypress == KEY_UP) player->direction = UP;
    if (keypress == KEY_DOWN) player->direction = DOWN;
}

void update_player_velocity(Moto *player)
{
    player->velocity.x = 0;
    player->velocity.y = 0;
    if (player->direction == RIGHT) player->velocity.x = +1;
    if (player->direction == LEFT) player->velocity.x = -1;
    if (player->direction == UP) player->velocity.y = -1;
    if (player->direction == DOWN) player->velocity.y = +1;
}

void update_player_position(Moto *player)
{
    player->position.x += player->velocity.x;
    player->position.y += player->velocity.y;
}

void check_player_collision(Moto *player)
{
    if (player->position.x < 0 || player->position.x >= map_width) game_over();
    if (player->position.y < 0 || player->position.y >= map_height) game_over();
    if (map[player->position.x][player->position.y] == WALL) game_over();
}

void fulfill_wall(Moto *player)
{
    map[player->position.x][player->position.y] = WALL;
}

void update_dumb_ai(Moto *ai)
{
    int try_new_direction = rand() % 10;
    int *directions = get_allowed_directions(ai);
    if (try_new_direction == 8) ai->direction = directions[0]; 
    if (try_new_direction == 9) ai->direction = directions[1]; 
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

void game_over()
{
    sleep(2);
    quit();
}

void draw_rectangle(int x, int y, int width, int height)
{
    box(ptr_window, 0, 0);
    return;
    mvhline(y, x, 0, width-x);
    mvhline(height, x, 0, width-x);
    mvvline(y, x, 0, height-y);
    mvvline(y, width, 0, height-y);
    mvaddch(y, x, ACS_ULCORNER);
    mvaddch(height, x, ACS_LLCORNER);
    mvaddch(y, width, ACS_URCORNER);
    mvaddch(height, width, ACS_LRCORNER);
}

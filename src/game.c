#include "game.h"

// pointers
Moto *ptr_player;
Moto *ptr_ai;
WINDOW *ptr_window;

// players
Moto player, ai; 

// screen size
int screen_height, screen_width;

// key code
int keypress;

// 2d array
char **map;

int main(int argc, char **argv)
{
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

    update_player();
    update_dumb_ai();
} 

void draw()
{
    erase();
    draw_rectangle(0, 0, GAME_WIDTH-1, GAME_HEIGHT-1);
    mvprintw(0, 0, "%dx%d | %dx%d", player.position.x, player.position.y, MAP_WIDTH, MAP_HEIGHT); 

    for (int x = 0; x < MAP_WIDTH; x++) {
        for (int y = 0; y < MAP_HEIGHT; y++) {
            draw_char(x, y, map[x][y]);
        }
    } 
    refresh();
}

void set_locale()
{
    char *locale;
    locale = setlocale(LC_ALL, "");
}

void create_window()
{
    // initialize window
    ptr_window = initscr(); 

    getmaxyx(ptr_window, screen_height, screen_width); 

    if (screen_height < GAME_HEIGHT || screen_width < GAME_WIDTH) {
        restore_window();
        printf(" - you need resize terminal to: cols %d rows %d", GAME_WIDTH, GAME_HEIGHT);
        printf("\n - current size: cols %d rows %d\n", screen_width, screen_height);
        exit(1);
    } 

    wresize(ptr_window, GAME_HEIGHT, GAME_WIDTH);

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
    map = malloc(MAP_HEIGHT * MAP_WIDTH * sizeof(char *));

    for (int x = 0; x < MAP_WIDTH; x++) {
        map[x] = malloc(MAP_HEIGHT * sizeof(char *));
        for (int y = 0; y < MAP_HEIGHT; y++) {
            map[x][y] = EMPTY;
        }
    }
} 

void create_players()
{
    ptr_player = malloc(sizeof(ptr_player));
    ptr_ai = malloc(sizeof(ptr_ai));

    player = *ptr_player;
    ai = *ptr_ai;

    initialize_player(&player, RIGHT, 0, 3);
    initialize_player(&ai, LEFT, MAP_WIDTH - 1, MAP_HEIGHT - 3);
}

void initialize_player(Moto *player, int direction, int x, int y)
{
    player->direction = direction;
    player->position.x = x;
    player->position.y = y;
    player->velocity.x = 0;
    player->velocity.y = 0;
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
        /* mvaddch(y, x, ACS_CKBOARD); */
        /* mvaddch(y, x, ACS_BOARD);  */
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
    if (player->position.x < 0 || player->position.x >= MAP_WIDTH) game_over();
    if (player->position.y < 0 || player->position.y >= MAP_HEIGHT) game_over();
    if (map[player->position.x][player->position.y] == WALL) game_over();
}

void update_player()
{
    input_player_direction(&player);

    // @todo - por no plural, atualizar os 2, pq eles fazem a mesma coisa
    update_player_velocity(&player);
    update_player_position(&player);
    check_player_collision(&player);
    draw_tail(&player);
}

void draw_tail(Moto *player)
{
    map[player->position.x][player->position.y] = WALL;
}

void update_dumb_ai()
{
    int try_new_direction = rand() % 10;
    int *directions = get_allowed_directions(&ai);
    if (try_new_direction == 8) ai.direction = directions[0]; 
    if (try_new_direction == 9) ai.direction = directions[1]; 

    // @todo - por no plural, atualizar os 2, pq eles fazem a mesma coisa
    update_player_velocity(&ai);
    update_player_position(&ai);
    check_player_collision(&ai);
    draw_tail(&ai);
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

#include "game.h"

// pointers
Moto * ptr_player;
Moto * ptr_enemy;
WINDOW * ptr_window;

Moto player;
Moto enemy; 

// tick
const int FPS = 1000000 / 10;

// size of screen
int HEIGHT;
int WIDTH;

// key code
int keypress;

// 2d array
char **map;

int main(int argc, char **argv)
{
    set_current_user_locale(); 
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
} 

void draw()
{
    erase();
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0, it = 0; y < HEIGHT; y++) {
            draw_char(x, y, map[x][y]);
        }
    } 
    refresh();
}

void set_current_user_locale()
{
    char *locale;
    locale = setlocale(LC_ALL, "");
}

void create_window()
{
    // initialize window
    ptr_window = initscr(); 

    // set no waiting for Enter key
    cbreak(); 

    // disable Echo
    noecho(); 

    // find size of window
    getmaxyx(ptr_window, HEIGHT, WIDTH); 

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
    map = malloc(HEIGHT * WIDTH * sizeof(char *));

    for (int x = 0; x < WIDTH; x++) {
        map[x] = malloc(HEIGHT * sizeof(char *));
        for (int y = 0, it = 0; y < HEIGHT; y++) {
            map[x][y] = EMPTY;
        }
    }
} 

void create_players()
{
    ptr_player = malloc(sizeof(player));
    ptr_enemy = malloc(sizeof(enemy));

    player = *ptr_player;
    enemy = *ptr_enemy;

    player.direction = RIGHT;
    player.x = 0;
    player.y = 0;
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
        mvaddch(y, x, ACS_CKBOARD);
    }
}

void find_player_direction()
{
    if (keypress == KEY_LEFT) player.direction = LEFT;
    if (keypress == KEY_RIGHT) player.direction = RIGHT;
    if (keypress == KEY_UP) player.direction = UP;
    if (keypress == KEY_DOWN) player.direction = DOWN;
}

void update_player_position()
{
    if (player.direction == RIGHT) player.x += 1;
    if (player.direction == LEFT) player.x -= 1;
    if (player.direction == UP) player.y -= 1;
    if (player.direction == DOWN) player.y += 1;
}

void check_player_collision()
{
    if (map[player.x][player.y] == WALL) {
        game_over();
    }
}

void update_player()
{
    find_player_direction();
    update_player_position();
    check_player_collision();

    map[player.x][player.y] = WALL;
}

void game_over()
{
    sleep(2);
    quit();
}

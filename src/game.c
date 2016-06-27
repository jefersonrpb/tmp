#include "game.h"

// pointers
Moto * ptr_player;
Moto * ptr_ai;
WINDOW * ptr_window;

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
} 

void draw()
{
    erase();
    draw_rectangle(0, 0, GAME_WIDTH-1, GAME_HEIGHT-1);
    mvprintw(0, 0, "%dx%d | %dx%d", player.x, player.y, MAP_WIDTH, MAP_HEIGHT); 

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
    ptr_player = malloc(sizeof(player));
    ptr_ai = malloc(sizeof(ai));

    player = *ptr_player;
    ai = *ptr_ai;

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
        /* mvaddch(y, x, ACS_CKBOARD); */
        /* mvaddch(y, x, ACS_BOARD);  */
        mvaddch(y+1, x+1, ACS_BLOCK); 
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
    if (player.x < 0 || player.x >= MAP_WIDTH) game_over();
    if (player.y < 0 || player.y >= MAP_HEIGHT) game_over();
    if (map[player.x][player.y] == WALL) game_over();
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

void draw_rectangle(int x, int y, int width, int height)
{
    mvhline(y, x, 0, width-x);
    mvhline(height, x, 0, width-x);
    mvvline(y, x, 0, height-y);
    mvvline(y, width, 0, height-y);
    mvaddch(y, x, ACS_ULCORNER);
    mvaddch(height, x, ACS_LLCORNER);
    mvaddch(y, width, ACS_URCORNER);
    mvaddch(height, width, ACS_LRCORNER);
}

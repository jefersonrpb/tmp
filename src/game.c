#include "game.h"

Player * player;
WINDOW * window;

const int FPS = 1000000 / 60;
int HEIGHT;
int WIDTH;
int keypress;
char **map;

int main(int argc, char **argv)
{
    set_current_user_locale(); 
    create_window();
    create_map();
    create_player();
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
            draw_char(y, x, map[x][y]);
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
    window = initscr(); 

    // set no waiting for Enter key
    cbreak(); 

    // disable Echo
    noecho(); 

    // find size of window
    getmaxyx(window, HEIGHT, WIDTH); 

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
            map[x][y] = ' ';
        }
    }
} 

void create_player()
{
    player = malloc(sizeof(Player));
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

void draw_char(int y, int x, char value)
{
    if (value != ' ') {
        mvaddch(y, x, ACS_CKBOARD);
    }
}

void update_player()
{
    if (keypress == KEY_LEFT) {
        player->direction = LEFT;
    }

    if (keypress == KEY_RIGHT) {
        player->direction = RIGHT;
    }

    if (keypress == KEY_UP) {
        player->direction = UP;
    }

    if (keypress == KEY_DOWN) {
        player->direction = DOWN;
    }
}

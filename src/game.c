#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <string.h>

// PDF: http://heather.cs.ucdavis.edu/~matloff/UnixAndC/CLanguage/Curses.pdf
#include <curses.h>

// sleep
#include <unistd.h>

#define fatal(msg)\
    fprintf(stderr, "[error] line %d: %s\n", __LINE__, msg);\
abort();

typedef struct {
    char **grid;
    int rows;
    int cols;
} Board;

typedef struct {
    int row;
    int col;
    bool fire;
} Aim;

typedef struct {
    Board *board;
    int keypress;
} Game; 

Game * game;
Aim * aim;

void set_current_user_locale()
{
    char *locale;
    locale = setlocale(LC_ALL, "");
}

void create_window(int *rows, int *cols)
{
    WINDOW *wnd;

    // curses call to initialize window
    wnd = initscr(); 

    // curses call to set no waiting for Enter key
    cbreak(); 

    // curses call to set no echoing
    noecho(); 

    // curses call to find size of window
    getmaxyx(wnd, *rows, *cols); 

    // curses call to clear screen, send cursor to position (0,0)
    clear(); 

    // curses call to implement all changes since last refresh
    refresh(); 

    // don't display a cursor
    curs_set(false);

    // enable keyboard mapping
    keypad(stdscr, TRUE);

    // input timeout
    timeout(0);//1000/8);   
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

void update_aim()
{
    game->board->grid[aim->row][aim->col -1] = '[';
    game->board->grid[aim->row][aim->col +1] = ']';
}

void draw_char(int row, int col, char value)
{
    move(row, col);
    /* delch(); */
    insch(value);

    /* mvprintw(row, col, value);  */
}

void update()
{
    game->board->grid[aim->row][aim->col -1] = ' ';
    game->board->grid[aim->row][aim->col +1] = ' ';

    game->keypress = getch();

    if (game->keypress == 'q') {
        quit();
    }

    if (game->keypress == KEY_LEFT) {
        aim->col -= 1;
    }

    if (game->keypress == KEY_RIGHT) {
        aim->col += 1;
    }

    if (game->keypress == KEY_UP) {
        aim->row -= 1;
    }

    if (game->keypress == KEY_DOWN) {
        aim->row += 1;
    }

    if (aim->row > game->board->rows - 1) {
        aim->row = game->board->rows - 1;
    }

    if (aim->col > game->board->cols - 2) {
        aim->col = game->board->cols - 2;
    }

    if (aim->col < 1) {
        aim->col = 1;
    }
    if (aim->row < 0) {
        aim->row = 0;
    }

    update_aim();
} 

void draw()
{
    erase();
    for (int row = 0, it = 0; row < game->board->rows; row++) {
        for (int col = 0; col < game->board->cols; col++) {
            draw_char(row, col, game->board->grid[row][col]);
        }
    } 
    refresh();
}

Board *create_board(int rows, int cols)
{
    char **grid;

    Board * board = malloc(sizeof(Board));
    grid = malloc(rows * cols * sizeof(int *));

    board->grid = grid;
    board->rows = rows;
    board->cols = cols;

    for (int row = 0, it = 0; row < board->rows; row++) {
        board->grid[row] = malloc(cols * sizeof(int));
        for (int col = 0; col < board->cols; col++) {
            board->grid[row][col] = ' ';
        }
    }

    return board;
}

void tick()
{
    int iterations = 10, fps = 1000/60;

    do {
        update();
        draw();
        /* usleep(fps); */
    } while (true);
}

Game *create_game(Board *board)
{
    game = malloc(sizeof(Game));
    game->board = board;

    aim = malloc(sizeof(Aim));

    return game;
}

int main(int argc, char **argv)
{
    set_current_user_locale(); 

    // get terminal size
    int winsize_rows, winsize_cols;
    create_window(&winsize_rows, &winsize_cols);

    Board *board = create_board(winsize_rows, winsize_cols);
    create_game(board);

    // loop game
    tick(); 

    quit();

    return 0;
}

#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <string.h>

// lib for get size of terminal
#include <sys/ioctl.h>
#include <unistd.h>

// libs for getch() 
#include <termios.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

#define fatal(msg)\
        fprintf(stderr, "[error] line %d: %s\n", __LINE__, msg);\
        abort();

typedef struct {
    int **grid;
    int rows;
    int cols;
} Board;

typedef struct {
    Board *board;
} Game; 

int getch(void) 
{
    int key = 0;

    struct termios org_opts, new_opts;
    int res = 0;

    // store old settings
    res=tcgetattr(STDIN_FILENO, &org_opts);
    assert(res == 0);

    // set new terminal parms 
    memcpy(&new_opts, &org_opts, sizeof(new_opts));
    new_opts.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ECHOPRT | ECHOKE | ICRNL);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_opts);
    key = getchar();

    // restore old settings 
    res = tcsetattr(STDIN_FILENO, TCSANOW, &org_opts);
    assert(res == 0);

    return(key);
}

void clear()
{
    printf("\033[2J\033[H");
}

void draw_col(int row, int col, char *str)
{
    printf("%c[%d;%df%s", 0x1B, row, col, str);
}

void set_current_user_locale()
{
    char *locale;
    locale = setlocale(LC_ALL, "");
}

void get_winsize(int *rows, int *cols)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    *rows = w.ws_row;
    *cols = w.ws_col;
}

void update()
{

} 

void draw(Game * game)
{
    clear();
    for (int row = 0, it = 0; row < game->board->rows; row++) {
        for (int col = 0; col < game->board->cols; col++) {
            /* draw_col(row, col, "."); */
            printf("[%d][%d] = %d\n", row, col, game->board->grid[row][col]);
        }
    } 
}

Board *create_board(int rows, int cols)
{
    int **grid;

    Board * board = malloc(sizeof(Board));
    grid = malloc(rows * cols * sizeof(int *));

    board->grid = grid;
    board->rows = rows;
    board->cols = cols;

    for (int row = 0, it = 0; row < board->rows; row++) {
        board->grid[row] = malloc(cols * sizeof(int));
        for (int col = 0; col < board->cols; col++) {
            board->grid[row][col] = col + row * rows;
        }
    }

    return board;
}

void tick(Game *game)
{
    update();
    draw(game);
}

Game *create_game(Board *board)
{
    Game *game = malloc(sizeof(Game));
    game->board = board;

    return game;
}

int main(int argc, char **argv)
{
    set_current_user_locale(); 
    
    // get terminal size
    int winsize_rows, winsize_cols;
    get_winsize(&winsize_rows, &winsize_cols);

    printf(" - hunter game\n");
    printf(" - lines %d\n", winsize_rows);
    printf(" - columns %d\n", winsize_cols);

    Board *board = create_board(winsize_rows, winsize_cols);
    Game *game = create_game(board);

    // loop game
    tick(game); 

    return 0;
}

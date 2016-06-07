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
    int *grid;
    int rows;
    int cols;
} Board;

typedef struct {
    Board *board;
} Game; 

int getch(void) 
{
    int c=0;

    struct termios org_opts, new_opts;
    int res=0;
    //-----  store old settings -----------
    res=tcgetattr(STDIN_FILENO, &org_opts);
    assert(res==0);
    //---- set new terminal parms --------
    memcpy(&new_opts, &org_opts, sizeof(new_opts));
    new_opts.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ECHOPRT | ECHOKE | ICRNL);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_opts);
    c=getchar();
    //------  restore old settings ---------
    res=tcsetattr(STDIN_FILENO, TCSANOW, &org_opts);
    assert(res==0);
    return(c);
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

int get_grid_pos(Board *board, int row, int col) 
{
    // sequencial 
    return (row * board->cols + col);
    // create 2d indices
    // return (col * board->rows + row);
}

int get_grid_value(Board *board, int row, int col) 
{
    int id = get_grid_pos(board, row, col);
    return board->grid[id];
} 

void set_grid_value(Board *board, int row, int col, int value) 
{
    int id = get_grid_pos(board, row, col);
    board->grid[id] = value;
}

void update()
{

} 

void draw(Game * game)
{
    clear();
    for (int row = 0, it = 0; row < game->board->rows; row++) {
        for (int col = 0; col < game->board->cols; col++) {
            draw_col(row, col, ".");
            /* printf("[%d][%d] = %d\n", row, col, game->board->grid[it++]); */
        }
    } 
}

Board *create_board(int rows, int cols)
{
    Board * board = malloc(sizeof(Board));
    board = board;
    board->grid = malloc(rows * cols * sizeof(int));
    board->rows = rows;
    board->cols = cols;

    for (int row = 0, it = 0; row < board->rows; row++) {
        for (int col = 0; col < board->cols; col++) {
            board->grid[it++] = 0;
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

    int debug_row = 37;
    int debug_col = 119;

    set_grid_value(board, debug_row, debug_col, 3);
    set_grid_value(board, 37, 150, 33);

    tick(game); 

    printf(
        "\ndebug: [%d][%d] >> %d = %d\n", 
        debug_row, 
        debug_col, 
        get_grid_pos(board, debug_row, debug_col), 
        get_grid_value(board, debug_row, debug_col)
    );

    return 0;
}

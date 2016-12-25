#include "board.h"

Board* board_create()
{
    Board* board = malloc(sizeof(Board));

    // 40x20 + 2 cells for bounds
    board->width = 42;
    board->height = 22;
    board->length = board->width * board->height;

    return board;
}

void board_destroy(Board* board)
{
    free(board);
}

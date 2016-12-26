#include "board.h"

Board* board_create(int width, int height)
{
    Board* board = malloc(sizeof(Board));
    board->width = width;
    board->height = height;
    board->length = width * height;

    return board;
}

void board_destroy(Board* board)
{
    free(board);
}

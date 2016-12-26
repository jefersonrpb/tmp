#ifndef BOARD_H

#define BOARD_H

#include <stdlib.h>

typedef struct {

    // sizes
    int width, height, length;

    // allow directions to move
    int moves[4];

} Board;

Board* board_create(int width, int height);
void board_destroy(Board* board);

#endif

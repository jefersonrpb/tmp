#ifndef GAME_H
#define GAME_H

#include <stdlib.h>

#include <int_array.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

#include <window.h>
#include <board.h>

#define SECOND 1000000
#define FPS SECOND / 10

typedef struct {

    Window* window;
    Board* board;

    int state;

} Game;

Game* game_create();
void game_destroy(Game* game);

void game_start(Game* game);
void game_tick(Game* game);

#endif

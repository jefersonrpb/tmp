#include "game.h"

void static tick(Game* game);

Game* game_create(Window* window, Board* board, Input* input)
{
    Game* game = malloc(sizeof(Game));
    game->window = window;
    game->board = board;
    game->input = input;
    return game;
}

void game_destroy(Game* game)
{
    board_destroy(game->board);
    window_destroy(game->window);
    free(game);
}

void game_set_state(Game* game, State* state) 
{
    game->state = state;
}

void game_start(Game* game)
{
    tick(game);
}

void static tick(Game* game)
{
    do {
        game->state->update();
        game->state->draw();
        usleep(FPS);
    } while (true);
}

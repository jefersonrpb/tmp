#include "game.h"

void static tick(Game* game);

Game* game_create(Window* window, Board* board, State* state)
{
    Game* game = malloc(sizeof(Game));
    game->window = window;
    game->board = board;
    game->state = state;
    return game;
}

void game_destroy(Game* game)
{
    board_destroy(game->board);
    window_destroy(game->window);
    free(game);
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

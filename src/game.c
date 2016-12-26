#include "game.h"

void static tick(Game* game);
void static update(Game* game);
void static draw(Game* game); 

Game* game_create(Window* window, Board* board)
{
    Game* game = malloc(sizeof(Game));
    game->window = window;
    game->board = board;
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

void static update(Game* game) 
{

}

void static draw(Game* game) 
{

}

void static tick(Game* game)
{
    do {
        update(game);
        draw(game);
        usleep(FPS);
    } while (true);
}

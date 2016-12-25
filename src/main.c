#include <main.h>

int main()
{
    Board* board = board_create();
    Window* window = window_create(board);
    Game* game = game_create(window, board);
    game_start(game);

    return 0;
}
